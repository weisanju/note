# RollingRandomAccessFileAppender

RollingRandomAccessFileAppender 类似于标准的 RollingFileAppender，除了它总是被缓冲（不能关闭）并且在内部它使用 ByteBuffer + RandomAccessFile 而不是 BufferedOutputStream。



在我们的测量中，与使用“bufferedIO=true”的 RollingFileAppender 相比，我们看到了 20-200% 的性能提升。 



RollingRandomAccessFileAppender 写入在 fileName 参数中命名的 File 并根据 TriggeringPolicy 和 RolloverPolicy 滚动文件。

与 RollingFileAppender 类似，RollingRandomAccessFileAppender 使用 RollingRandomAccessFileManager 来实际执行文件 I/O 并执行翻转。

虽然无法共享来自不同配置的 RollingRandomAccessFileAppender，但如果 Manager 可访问，则 RollingRandomAccessFileManagers 可以共享。

例如，servlet 容器中的两个 Web 应用程序可以有自己的配置，并且如果 Log4j 位于它们公共的 ClassLoader 中，则可以安全地写入同一个文件。



RollingRandomAccessFileAppender 需要 TriggeringPolicy 和 RolloverStrategy。

触发策略确定是否应该执行翻转，而 RolloverStrategy 定义应该如何完成翻转。

如果没有配置 RolloverStrategy，RollingRandomAccessFileAppender 将使用 DefaultRolloverStrategy。

从 log4j-2.5 开始，可以在 DefaultRolloverStrategy 中配置自定义删除操作以在翻转时运行。



RollingRandomAccessFileAppender 不支持文件锁定。



# RollingRandomAccessFileAppender Parameters

| Parameter Name   | Type             | Description                                                  |
| :--------------- | :--------------- | :----------------------------------------------------------- |
| append           | boolean          | When true - the default, records will be appended to the end of the file. When set to false, the file will be cleared before new records are written. |
| filter           | Filter           | A Filter to determine if the event should be handled by this Appender. More than one Filter may be used by using a CompositeFilter. |
| fileName         | String           | The name of the file to write to. If the file, or any of its parent directories, do not exist, they will be created. |
| filePattern      | String           | The pattern of the file name of the archived log file. The format of the pattern should is dependent on the RolloverStrategu that is used. The DefaultRolloverStrategy will accept both a date/time pattern compatible with [SimpleDateFormat](http://download.oracle.com/javase/7/docs/api/java/text/SimpleDateFormat.html) and/or a %i which represents an integer counter. The integer counter allows specifying a padding, like %3i for space-padding the counter to 3 digits or (usually more useful) %03i for zero-padding the counter to 3 digits. The pattern also supports interpolation at runtime so any of the Lookups (such as the [DateLookup](http://logging.apache.org/log4j/2.x/manual/lookups.html#DateLookup) can be included in the pattern. |
| immediateFlush   | boolean          | When set to true - the default, each write will be followed by a flush. This will guarantee the data is written to disk but could impact performance.Flushing after every write is only useful when using this appender with synchronous loggers. Asynchronous loggers and appenders will automatically flush at the end of a batch of events, even if immediateFlush is set to false. This also guarantees the data is written to disk but is more efficient. |
| bufferSize       | int              | The buffer size, defaults to 262,144 bytes (256 * 1024).     |
| layout           | Layout           | The Layout to use to format the LogEvent. If no layout is supplied the default pattern layout of "%m%n" will be used. |
| name             | String           | The name of the Appender.                                    |
| policy           | TriggeringPolicy | The policy to use to determine if a rollover should occur.   |
| strategy         | RolloverStrategy | The strategy to use to determine the name and location of the archive file. |
| ignoreExceptions | boolean          | The default is true, causing exceptions encountered while appending events to be internally logged and then ignored. When set to false exceptions will be propagated to the caller, instead. You must set this to false when wrapping this Appender in a [FailoverAppender](http://logging.apache.org/log4j/2.x/manual/appenders.html#FailoverAppender). |
| filePermissions  | String           | File attribute permissions in POSIX format to apply whenever the file is created.Underlying files system shall support [POSIX](https://docs.oracle.com/javase/7/docs/api/java/nio/file/attribute/PosixFileAttributeView.html) file attribute view.Examples: rw------- or rw-rw-rw- etc... |
| fileOwner        | String           | File owner to define whenever the file is created.Changing file's owner may be restricted for security reason and Operation not permitted IOException thrown. Only processes with an effective user ID equal to the user ID of the file or with appropriate privileges may change the ownership of a file if [_POSIX_CHOWN_RESTRICTED](http://www.gnu.org/software/libc/manual/html_node/Options-for-Files.html) is in effect for path.Underlying files system shall support file [owner](https://docs.oracle.com/javase/7/docs/api/java/nio/file/attribute/FileOwnerAttributeView.html) attribute view. |
| fileGroup        | String           | File group to define whenever the file is created.Underlying files system shall support [POSIX](https://docs.oracle.com/javase/7/docs/api/java/nio/file/attribute/PosixFileAttributeView.html) file attribute view. |



# Triggering Policies

See [RollingFileAppender Triggering Policies](http://logging.apache.org/log4j/2.x/manual/appenders.html#TriggeringPolicies).



# Rollover Strategies

See [RollingFileAppender Rollover Strategies](http://logging.apache.org/log4j/2.x/manual/appenders.html#RolloverStrategies).



