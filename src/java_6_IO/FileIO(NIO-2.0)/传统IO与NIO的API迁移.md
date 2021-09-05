# Legacy File I/O Code

与遗留代码的互操作性

在 Java SE 7 版本之前，java.io.File 类是用于文件 I/O 的机制，但它有几个缺点。

- 许多方法在失败时不会抛出异常，因此无法获得有用的错误消息。

  例如，如果文件删除失败，程序会收到“删除失败”，但不知道是因为文件不存在、用户没有权限还是存在其他问题。 .

- `rename` 方法在跨平台上无法一致地工作。.

- 没有对符号链接的真正支持。.

- 需要对元数据提供更多支持，例如文件权限、文件所有者和其他安全属性。.

- 访问文件元数据效率低下。.

- 许多 `File` 方法没有扩展。

  通过服务器请求大型目录列表可能会导致挂起。

  大目录还可能导致内存资源问题，从而导致拒绝服务。.

- 如果存在循环符号链接，则无法编写可靠的代码来递归遍历文件树并做出适当的响应



也许您有使用 `java.io.File` 的遗留代码，并希望利用 `java.nio.file.Path` 功能对您的代码影响最小。

`java.io.File` 类提供了 [`toPath`](https://docs.oracle.com/javase/8/docs/api/java/io/File.html#toPath--) 方法，它

将旧式 `File` 实例转换为 `java.nio.file.Path` 实例，如下所示：

```
Path input = file.toPath();
```

然后，您可以利用“Path”类可用的丰富功能集。

例如，假设您有一些删除文件的代码：

```
file.delete();
```

您可以修改此代码以使用 `Files.delete` 方法:

```
Path fp = file.toPath();
Files.delete(fp);
```

Conversely, the [`Path.toFile`](https://docs.oracle.com/javase/8/docs/api/java/nio/file/Path.html#toFile--) method constructs a `java.io.File` object for a `Path` object.



# Mapping java.io.File Functionality to java.nio.file

由于文件 I/O 的 Java 实现已在 Java SE 7 版本中完全重新架构，因此您无法将一种方法替换为另一种方法。

如果您想使用 java.nio.file 包提供的丰富功能，最简单的解决方案是使用 File.toPath 方法，如上一节中建议的那样。

但是，如果您不想使用该方法或它不足以满足您的需要，则必须重写文件 I/O 代码。

两个 API 之间没有一一对应关系，但下表让您大致了解 java.io.File API 中的哪些功能映射到 java.nio.file API 中的功能，并告诉您可以在哪里

| java.io.File Functionality                                   | java.nio.file Functionality                                  | Tutorial Coverage                                            |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| `java.io.File`                                               | `java.nio.file.Path`                                         | [The Path Class](https://docs.oracle.com/javase/tutorial/essential/io/pathClass.html) |
| `java.io.RandomAccessFile`                                   | The `SeekableByteChannel` functionality.                     | [Random Access Files](https://docs.oracle.com/javase/tutorial/essential/io/rafs.html) |
| `File.canRead`, `canWrite`, `canExecute`                     | `Files.isReadable`, `Files.isWritable`, and `Files.isExecutable`. On UNIX file systems, the [Managing Metadata (File and File Store Attributes)](https://docs.oracle.com/javase/tutorial/essential/io/fileAttr.html) package is used to check the nine file permissions. | [Checking a File or Directory](https://docs.oracle.com/javase/tutorial/essential/io/check.html) [Managing Metadata](https://docs.oracle.com/javase/tutorial/essential/io/fileAttr.html) |
| `File.isDirectory()`, `File.isFile()`, and `File.length()`   | `Files.isDirectory(Path, LinkOption...)`, `Files.isRegularFile(Path, LinkOption...)`, and `Files.size(Path)` | [Managing Metadata](https://docs.oracle.com/javase/tutorial/essential/io/fileAttr.html) |
| `File.lastModified()` and `File.setLastModified(long)`       | `Files.getLastModifiedTime(Path, LinkOption...)` and `Files.setLastMOdifiedTime(Path, FileTime)` | [Managing Metadata](https://docs.oracle.com/javase/tutorial/essential/io/fileAttr.html) |
| The `File` methods that set various attributes: `setExecutable`, `setReadable`, `setReadOnly`, `setWritable` | These methods are replaced by the `Files` method `setAttribute(Path, String, Object, LinkOption...)`. | [Managing Metadata](https://docs.oracle.com/javase/tutorial/essential/io/fileAttr.html) |
| `new File(parent, "newfile")`                                | `parent.resolve("newfile")`                                  | [Path Operations](https://docs.oracle.com/javase/tutorial/essential/io/pathOps.html) |
| `File.renameTo`                                              | `Files.move`                                                 | [Moving a File or Directory](https://docs.oracle.com/javase/tutorial/essential/io/move.html) |
| `File.delete`                                                | `Files.delete`                                               | [Deleting a File or Directory](https://docs.oracle.com/javase/tutorial/essential/io/delete.html) |
| `File.createNewFile`                                         | `Files.createFile`                                           | [Creating Files](https://docs.oracle.com/javase/tutorial/essential/io/file.html#createFile) |
| `File.deleteOnExit`                                          | Replaced by the `DELETE_ON_CLOSE` option specified in the `createFile` method. | [Creating Files](https://docs.oracle.com/javase/tutorial/essential/io/file.html#createFile) |
| `File.createTempFile`                                        | `Files.createTempFile(Path, String, FileAttributes<?>)`, `Files.createTempFile(Path, String, String, FileAttributes<?>)` | [Creating Files](https://docs.oracle.com/javase/tutorial/essential/io/file.html#createFile) [Creating and Writing a File by Using Stream I/O](https://docs.oracle.com/javase/tutorial/essential/io/file.html#createStream) [Reading and Writing Files by Using Channel I/O](https://docs.oracle.com/javase/tutorial/essential/io/file.html#channelio) |
| `File.exists`                                                | `Files.exists` and `Files.notExists`                         | [Verifying the Existence of a File or Directory](https://docs.oracle.com/javase/tutorial/essential/io/check.html) |
| `File.compareTo` and `equals`                                | `Path.compareTo` and `equals`                                | [Comparing Two Paths](https://docs.oracle.com/javase/tutorial/essential/io/pathOps.html#compare) |
| `File.getAbsolutePath` and `getAbsoluteFile`                 | `Path.toAbsolutePath`                                        | [Converting a Path](https://docs.oracle.com/javase/tutorial/essential/io/pathOps.html#convert) |
| `File.getCanonicalPath` and `getCanonicalFile`               | `Path.toRealPath` or `normalize`                             | [Converting a Path (`toRealPath`)](https://docs.oracle.com/javase/tutorial/essential/io/pathOps.html#convert) [Removing Redundancies From a Path (`normalize`)](https://docs.oracle.com/javase/tutorial/essential/io/pathOps.html#normal) |
| `File.toURI`                                                 | `Path.toURI`                                                 | [Converting a Path](https://docs.oracle.com/javase/tutorial/essential/io/pathOps.html#convert) |
| `File.isHidden`                                              | `Files.isHidden`                                             | [Retrieving Information About the Path](https://docs.oracle.com/javase/tutorial/essential/io/pathOps.html#info) |
| `File.list` and `listFiles`                                  | `Path.newDirectoryStream`                                    | [Listing a Directory's Contents](https://docs.oracle.com/javase/tutorial/essential/io/dirs.html#listdir) |
| `File.mkdir` and `mkdirs`                                    | `Files.createDirectory`                                      | [Creating a Directory](https://docs.oracle.com/javase/tutorial/essential/io/dirs.html#create) |
| `File.listRoots`                                             | `FileSystem.getRootDirectories`                              | [Listing a File System's Root Directories](https://docs.oracle.com/javase/tutorial/essential/io/dirs.html#listall) |
| `File.getTotalSpace`, `File.getFreeSpace`, `File.getUsableSpace` | `FileStore.getTotalSpace`, `FileStore.getUnallocatedSpace`, `FileStore.getUsableSpace`, `FileStore.getTotalSpace` | [File Store Attributes](https://docs.oracle.com/javase/tutorial/essential/io/fileAttr.html#store) |

