

# FileAppender

FileAppender 是一个 OutputStreamAppender，它写入 fileName 参数中命名的 File。 FileAppender 使用 FileManager（它扩展了 OutputStreamManager）来实际执行文件 I/O。虽然不能共享来自不同配置的 FileAppender，但如果 Manager 可访问，则可以共享 FileManager。



例如，servlet 容器中的两个 Web 应用程序可以有自己的配置，并且如果 Log4j 位于它们公共的 ClassLoader 中，则可以安全地写入同一个文件。



# FileAppender Parameters

| Parameter Name   | Type    | Description                                                  |
| :--------------- | :------ | :----------------------------------------------------------- |
| append           | boolean | 当 true - 默认值时，记录将被附加到文件的末尾。设置为 false 时，将在写入新记录之前清除文件 |
| bufferedIO       | boolean | 当为 true - 默认值时，记录将被写入缓冲区，当缓冲区已满时，数据将写入磁盘，如果设置了 immediateFlush，则在写入记录时将数据写入磁盘。文件锁定不能与 bufferedIO 一起使用。性能测试表明，即使启用了即时刷新，使用缓冲 I/O 也能显着提高性能。 |
| bufferSize       | int     | 当 bufferedIO 为 true 时，这是缓冲区大小，默认为 8192 字节   |
| createOnDemand   | boolean | 按需创建文件. 仅当日志事件通过所有过滤器并路由到此 appender 时，appender 才会创建文件. Defaults to false. |
| filter           | Filter  | 一个过滤器，用于确定事件是否应该由这个 Appender 处理。使用 CompositeFilter 可以使用多个过滤器。 |
| fileName         | String  | 要写入的文件的名称。如果文件或其任何父目录不存在，则将创建它们 |
| immediateFlush   | boolean | 当设置为 true - 默认值时，每次写入后都会进行刷新。这将保证将数据写入磁盘，但可能会影响性能. 每次写入后刷新仅在同步记录器中有用. 异步记录器和附加器将在一批事件结束时自动刷新，即使immediateFlush 设置为false。这也保证了将数据写入磁盘但效率更高。 |
| layout           | Layout  | The Layout to use to format the LogEvent. If no layout is supplied the default pattern layout of "%m%n" will be used. |
| locking          | boolean | 当设置为 true 时，I/O 操作只会在持有文件锁时发生，允许多个 JVM 中的 FileAppenders 和潜在的多个主机同时写入同一个文件. 这将显着影响性能，因此应谨慎使用。.此外，在许多系统上，文件锁是“建议性的”，这意味着其他应用程序可以在不获取锁的情况下对文件执行操作。默认值为假。 |
| name             | String  | The name of the Appender.                                    |
| ignoreExceptions | boolean | The default is true, causing exceptions encountered while appending events to be internally logged and then ignored. When set to false exceptions will be propagated to the caller, instead. You must set this to false when wrapping this Appender in a [FailoverAppender](http://logging.apache.org/log4j/2.x/manual/appenders.html#FailoverAppender). |
| filePermissions  | String  | POSIX 格式的文件属性权限在创建文件时应用。底层文件系统应支持 [POSIX](https://docs.oracle.com/javase/7/docs/api/java/nio/file/attribute/PosixFileAttributeView .html) 文件属性视图。示例：rw------- 或 rw-rw-rw- 等... |
| fileOwner        | String  | 文件所有者在创建文件时定义。出于安全原因，可能会限制更改文件的所有者，并且操作不允许，抛出 IOException. Only processes with an effective user ID equal to the user ID of the file or with appropriate privileges may change the ownership of a file if [_POSIX_CHOWN_RESTRICTED](http://www.gnu.org/software/libc/manual/html_node/Options-for-Files.html) is in effect for path.Underlying files system shall support file [owner](https://docs.oracle.com/javase/7/docs/api/java/nio/file/attribute/FileOwnerAttributeView.html) attribute view. |
| fileGroup        | String  | File group to define whenever the file is created.Underlying files system shall support [POSIX](https://docs.oracle.com/javase/7/docs/api/java/nio/file/attribute/PosixFileAttributeView.html) file attribute view. |





# Example

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="warn" name="MyApp" packages="">
  <Appenders>
    <File name="MyFile" fileName="logs/app.log">
      <PatternLayout>
        <Pattern>%d %p %c{1.} [%t] %m%n</Pattern>
      </PatternLayout>
    </File>
  </Appenders>
  <Loggers>
    <Root level="error">
      <AppenderRef ref="MyFile"/>
    </Root>
  </Loggers>
</Configuration>
```

