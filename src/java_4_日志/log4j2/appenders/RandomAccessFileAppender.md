# RandomAccessFileAppender

RandomAccessFileAppender 类似于标准的 FileAppender，除了它总是被缓冲（不能关闭），并且在内部它使用 ByteBuffer + RandomAccessFile 而不是 BufferedOutputStream

在我们的 [measurements](https://logging.apache.org/log4j/2.x/performance.html#whichAppender) 中，与带有“bufferedIO=true”的 FileAppender 相比，我们看到了 20-200% 的性能提升。

与 FileAppender 类似，RandomAccessFileAppender 使用 RandomAccessFileManager 来实际执行文件 I/O。

虽然无法共享来自不同配置的 RandomAccessFileAppender，但如果 Manager 可访问，则可以共享 RandomAccessFileManagers



例如，servlet 容器中的两个 Web 应用程序可以有自己的配置，并且如果 Log4j 位于它们公共的 ClassLoader 中，则可以安全地写入同一个文件。





# Paramter



| Parameter Name   | Type    | Description                                                  |
| :--------------- | :------ | :----------------------------------------------------------- |
| append           | boolean | 当 true - 默认值时，记录将被附加到文件的末尾。<br/>设置为 false 时，将在写入新记录之前清除文件。 |
| fileName         | String  | 要写入的文件的名称。<br/>如果文件或其任何父目录不存在，则将创建它们。 |
| filters          | Filter  | 一个过滤器，用于确定事件是否应该由这个 Appender 处理。<br/>使用 CompositeFilter 可以使用多个过滤器。 |
| immediateFlush   | boolean | 当设置为 true - 默认值时，每次写入后都会进行刷新。这将保证将数据写入磁盘，但可能会影响性能。每次写入后刷新仅在将此 appender 与同步记录器一起使用时才有用。异步记录器和附加器将在一批事件结束时自动刷新，即使immediateFlush 设置为false。<br/>这也保证了将数据写入磁盘但效率更高。 |
| bufferSize       | int     | The buffer size, defaults to 262,144 bytes (256 * 1024).     |
| layout           | Layout  | 用于格式化 LogEvent 的布局。<br/>如果未提供布局，则将使用“%m%n”的默认模式布局。 |
| name             | String  | The name of the Appender.                                    |
| ignoreExceptions | boolean | The default is true, causing exceptions encountered while appending events to be internally logged and then ignored. When set to false exceptions will be propagated to the caller, instead. You must set this to false when wrapping this Appender in a [FailoverAppender](http://logging.apache.org/log4j/2.x/manual/appenders.html#FailoverAppender). |

# Example

Here is a sample RandomAccessFile configuration:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="warn" name="MyApp" packages="">
  <Appenders>
    <RandomAccessFile name="MyFile" fileName="logs/app.log">
      <PatternLayout>
        <Pattern>%d %p %c{1.} [%t] %m%n</Pattern>
      </PatternLayout>
    </RandomAccessFile>
  </Appenders>
  <Loggers>
    <Root level="error">
      <AppenderRef ref="MyFile"/>
    </Root>
  </Loggers>
</Configuration>
```

