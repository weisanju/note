The OutputStreamAppender provides the base for many of the other Appenders such as the File and Socket appenders that write the event to an Output Stream. It cannot be directly configured. Support for immediateFlush and buffering is provided by the OutputStreamAppender. The OutputStreamAppender uses an OutputStreamManager to handle the actual I/O, allowing the stream to be shared by Appenders in multiple configurations.

OutputStreamAppender 为许多其他 Appender 提供了基础，例如将事件写入输出流的 File 和 Socket appender。不能直接配置。 OutputStreamAppender 提供了对即时刷新和缓冲的支持。 OutputStreamAppender 使用 OutputStreamManager 来处理实际的 I/O，允许在多个配置中由 Appender 共享流。



