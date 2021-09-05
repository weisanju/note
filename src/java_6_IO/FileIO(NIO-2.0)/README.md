# 简介

Java I/O 支持包含在 java.io 和 java.nio 包中。

这些软件包包括以下功能：

* Input and output through data streams, serialization and the file system.
* Charsets, decoders, and encoders, for translating between bytes and Unicode characters.
* Access to file, file attributes and file systems.
* APIs for building scalable servers using asynchronous or multiplexed, non-blocking I/O.







# FileI/O NIO2.0 feature

**前言**

java.nio.file 包及其相关包 java.nio.file.attribute 为文件 I/O 和访问默认文件系统提供全面支持。

尽管 API 有很多类，但您只需关注几个入口点。

**Path**

本教程首先询问  [what is a path?](https://docs.oracle.com/javase/tutorial/essential/io/path.html)  然后，

引入了 java.nio.file 包中的主要入口点 Path 类。

Path 类中与句法操作相关的方法。

**Files**

然后，本教程转到包中的另一个主要类 Files 类，其中包含处理文件操作的方法。

首先，介绍了许多文件操作共有的一些概念,本教程随后介绍了[checking](https://docs.oracle.com/javase/tutorial/essential/io/check.html), [deleting](https://docs.oracle.com/javase/tutorial/essential/io/delete.html), [copying](https://docs.oracle.com/javase/tutorial/essential/io/copy.html), and [moving](https://docs.oracle.com/javase/tutorial/essential/io/move.html) files.

**元数据管理**

本教程展示了如何管理[元数据](https://docs.oracle.com/javase/tutorial/essential/io/fileAttr.html)，然后再介绍[文件 I/O](https://docs.oracle.com/javase/tutorial/essential/io/file.html) 和[目录 I/O](https://docs.oracle.com/javase/tutorial/essential/io/dirs.html)。

解释了[随机访问文件](https://docs.oracle.com/javase/tutorial/essential/io/rafs.html)，issues specific to [symbolic and hard links](https://docs.oracle.com/javase/tutorial/essential/io/links.html) are examined.

**遍历目录树**

接下来，将介绍一些非常强大但更高级的主题。

首先，演示了[递归遍历文件树](https://docs.oracle.com/javase/tutorial/essential/io/walk.html)的能力，然后是有关如何使用[通配符搜索文件](https://docs.oracle.com/javase/tutorial/essential/io/find.html)的信息。

接下来，将解释和[演示如何观察目录的变化](https://docs.oracle.com/javase/tutorial/essential/io/notification.html)。





# 总结

`java.io` 包包含许多类，您的程序可以使用这些类来读取和写入数据。

大多数类实现顺序访问流，顺序访问流可以分为两组 **读写字节的**和**读写 Unicode 字符的**

每个顺序访问流都有其特殊性，例如读取或写入文件、在读取或写入数据时过滤数据或序列化对象。



`java.nio.file` 包为文件和文件系统 I/O 提供了广泛的支持。

这是一个非常全面的API，但关键的入口点如下：

- `Path` 类具有操作路径的方法.
- `Files` 类具有文件操作的方法，例如移动、复制、删除，以及检索和设置文件属性的方法。.
- `FileSystem` 类有多种获取文件系统信息的方法.

更多关于 NIO.2 的信息可以在 [OpenJDK: NIO](http://openjdk.java.net/projects/nio/) 项目网站上找到。

该站点包含 NIO.2 提供的特性的资源，这些特性超出了本教程的范围，例如多播、异步 I/O 和创建您自己的文件系统实现。



# 参考链接

[Oracle官方教程](https://docs.oracle.com/javase/8/docs/technotes/guides/io/index.html)

[Enhancements in Java I/O](https://docs.oracle.com/javase/8/docs/technotes/guides/io/enhancements.html#pre)

