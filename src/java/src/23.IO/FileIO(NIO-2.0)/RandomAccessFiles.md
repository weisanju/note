# Random Access Files

*随机访问文件*允许对文件内容进行非顺序或随机访问。

要随机访问文件，您可以打开文件，查找特定位置，然后读取或写入该文件。

此功能可通过 [`SeekableByteChannel`](https://docs.oracle.com/javase/8/docs/api/java/nio/channels/SeekableByteChannel.html) 接口实现。 `SeekableByteChannel` 接口使用当前位置的概念扩展了通道 I/O。

方法使您能够设置或查询位置，然后您可以从该位置读取数据或将数据写入该位置。 

API 由几个易于使用的方法组成：

- [`position`](https://docs.oracle.com/javase/8/docs/api/java/nio/channels/SeekableByteChannel.html#position--) – 返回channel的当前位置 
- [`position(long)`](https://docs.oracle.com/javase/8/docs/api/java/nio/channels/SeekableByteChannel.html#position-long-) – 设置channel位置
- [`read(ByteBuffer)`](https://docs.oracle.com/javase/8/docs/api/java/nio/channels/SeekableByteChannel.html#read-java.nio.ByteBuffer-) – Reads bytes into the buffer from the channel
- [`write(ByteBuffer)`](https://docs.oracle.com/javase/8/docs/api/java/nio/channels/SeekableByteChannel.html#write-java.nio.ByteBuffer-) – Writes bytes from the buffer to the channel
- [`truncate(long)`](https://docs.oracle.com/javase/8/docs/api/java/nio/channels/SeekableByteChannel.html#truncate-long-) – 截断连接到通道的文件（或其他实体）

[使用通道 I/O 读取和写入文件](https://docs.oracle.com/javase/tutorial/essential/io/file.html#channelio) 显示 `Path.newByteChannel` 方法返回一个

`SeekableByteChannel`。

在默认文件系统上，您可以按原样使用该通道，也可以将其转换为 [`FileChannel`](https://docs.oracle.com/javase/8/docs/api/java/nio/channels /FileChannel.html) 让您可以访问更高级的功能，例如将文件区域直接映射到内存以加快访问速度、锁定文件区域或从绝对位置读取和写入字节而不影响通道的当前位置

The following code snippet opens a file for both reading and writing by using one of the `newByteChannel` methods.

以下代码片段使用“newByteChannel”方法之一打开一个文件以供读取和写入。

返回的 `SeekableByteChannel` 被转换为 `FileChannel`。

然后，从文件的开头读取 12 个字节，以及字符串“我在这里！” 写在那个位置。

将文件中的当前位置移到末尾，并附加从开头开始的 12 个字节。

最后附加字符串：“我在这里！”

```java
String s = "I was here!\n";
byte data[] = s.getBytes();
ByteBuffer out = ByteBuffer.wrap(data);

ByteBuffer copy = ByteBuffer.allocate(12);

try (FileChannel fc = (FileChannel.open(file, READ, WRITE))) {
    // Read the first 12
    // bytes of the file.
    int nread;
    do {
        nread = fc.read(copy);
    } while (nread != -1 && copy.hasRemaining());

    // Write "I was here!" at the beginning of the file.
    fc.position(0);
    while (out.hasRemaining())
        fc.write(out);
    out.rewind();

    // Move to the end of the file.  Copy the first 12 bytes to
    // the end of the file.  Then write "I was here!" again.
    long length = fc.size();
    fc.position(length-1);
    copy.flip();
    while (copy.hasRemaining())
        fc.write(copy);
    while (out.hasRemaining())
        fc.write(out);
} catch (IOException x) {
    System.out.println("I/O Exception: " + x);
}
```

