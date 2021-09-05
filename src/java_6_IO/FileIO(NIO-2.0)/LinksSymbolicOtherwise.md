# Links, Symbolic or Otherwise

如前所述，`java.nio.file` 包，特别是 `Path` 类，是“链接感知的”。

每个“Path”方法要么检测遇到符号链接时要做什么，要么提供一个选项，使您可以配置遇到符号链接时的行为。



到目前为止的讨论是关于 [符号或 *软* 链接](https://docs.oracle.com/javase/tutorial/essential/io/path.html#symlink)，但一些文件系统也支持硬链接。 

*硬链接*比符号链接更具限制性，如下所示：-

- 链接的目标必须存在。 

- 目录上通常不允许使用硬链接。.

- 硬链接不允许跨分区或卷。因此，它们不能跨文件系统存在。

- 硬链接的外观和行为都与普通文件相似，因此很难找到它们。

- 就所有意图和目的而言，硬链接是与原始文件相同的实体。它们具有相同的文件权限、时间戳等。

  所有属性都相同。

由于这些限制，硬链接不像符号链接那样经常使用，但是`Path` 方法可以与硬链接无缝协作。

几种方法专门处理链接，并在以下部分中介绍：:

- [Creating a Symbolic Link](https://docs.oracle.com/javase/tutorial/essential/io/links.html#symLink)
- [Creating a Hard Link](https://docs.oracle.com/javase/tutorial/essential/io/links.html#hardLink)
- [Detecting a Symbolic Link](https://docs.oracle.com/javase/tutorial/essential/io/links.html#detect)
- [Finding the Target of a Link](https://docs.oracle.com/javase/tutorial/essential/io/links.html#read)

# Creating a Symbolic Link

如果您的文件系统支持它，您可以使用 [`createSymbolicLink(Path, Path, FileAttribute)`](https://docs.oracle.com/javase/8/docs/api/java/nio /file/Files.html#createSymbolicLink-java.nio.file.Path-java.nio.file.Path-java.nio.file.attribute.FileAttribute...-) 方法。

第二个“Path”参数代表目标文件或目录，可能存在也可能不存在。

以下代码片段创建了一个具有默认权限的符号链接

```
Path newLink = ...;
Path target = ...;
try {
    Files.createSymbolicLink(newLink, target);
} catch (IOException x) {
    System.err.println(x);
} catch (UnsupportedOperationException x) {
    // Some file systems do not support symbolic links.
    System.err.println(x);
}
```

`FileAttributes` 可变参数使您能够指定在创建链接时自动设置的初始文件属性。

但是，此参数旨在供将来使用，目前尚未实现。 

# Creating a Hard Link

您可以使用 [`createLink(Path, Path)`](https://docs.oracle.com/javase/8/docs/api/java/ nio/file/Files.html#createLink-java.nio.file.Path-java.nio.file.Path-) 方法。

第二个 `Path` 参数定位现有文件，它必须存在，否则会抛出 `NoSuchFileException`。

以下代码片段显示了如何创建链接：

```java
Path newLink = ...;
Path existingFile = ...;
try {
    Files.createLink(newLink, existingFile);
} catch (IOException x) {
    System.err.println(x);
} catch (UnsupportedOperationException x) {
    // Some file systems do not
    // support adding an existing
    // file to a directory.
    System.err.println(x);
}
```

# Detecting a Symbolic Link

要确定一个 `Path` 实例是否是符号链接，可以使用 [`isSymbolicLink(Path)`](https://docs.oracle.com/javase/8/docs/api/java/nio/file/ Files.html#isSymbolicLink-java.nio.file.Path-) 方法。

以下代码片段显示了如何：

```
Path file = ...;
boolean isSymbolicLink =
    Files.isSymbolicLink(file);
```

For more information, see [Managing Metadata](https://docs.oracle.com/javase/tutorial/essential/io/fileAttr.html).

# Finding the Target of a Link

您可以使用 [`readSymbolicLink(Path)`](https://docs.oracle.com/javase/8/docs/api/java/nio/file/Files.html#readSymbolicLink) 获取符号链接的目标方法，如下

```

Path link = ...;
try {
    System.out.format("Target of link" +
        " '%s' is '%s'%n", link,
        Files.readSymbolicLink(link));
} catch (IOException x) {
    System.err.println(x);
}
```

如果 `Path` 不是符号链接，则此方法会抛出一个 `NotLinkException`。

