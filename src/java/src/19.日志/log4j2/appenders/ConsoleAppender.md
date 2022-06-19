# ConsoleAppender

正如人们所料，ConsoleAppender 将其输出写入 System.out 或 System.err，其中 System.out 是默认目标。

**必须提供布局来格式化 LogEvent。**



| Parameter Name   | Type    | Description                                                  |
| :--------------- | :------ | :----------------------------------------------------------- |
| filter           | Filter  | 个过滤器，用于确定事件是否应该由这个 Appender 处理。<br/>使用 CompositeFilter 可以使用多个过滤器。 |
| layout           | Layout  | 用于格式化 LogEvent 的布局。<br/><br/>如果未提供布局，则将使用“%m%n”的默认模式布局。 |
| follow           | boolean | 标识 appender在配置后  是否通过 System.setOut 或 System.setErr对  System.out 或 System.err 的重新分配<br />请注意，follow 属性不能与 Windows 上的 Jansi 一起使用，不能与 `direct` 一起使用。 |
| direct           | boolean | 直接写入java.io.FileDescriptor，绕过java.lang.System.out/.err.当输出被重定向到文件或其他进程时，可以提供高达 10 倍的性能提升.不能在 Windows 上与 Jansi 一起使用。<br/>不能与 `follow`一起使用. 输出将不尊重 java.lang.System.setOut()/.setErr() 并且可能会与多线程应用程序中 java.lang.System.out/.err 的其他输出交织在一起. 自 2.6.2 以来的新功能 目前仅在 Linux 和 Windows 上使用 Oracle JVM 进行过测试 |
| name             | String  | Appender 的名称                                              |
| ignoreExceptions | boolean | The default is true, 致在附加事件时遇到异常被内部记录然后被忽略. 当设置为 false 时，异常将传播给调用者<br /> You must set this to false when wrapping this Appender in a [FailoverAppender](http://logging.apache.org/log4j/2.x/manual/appenders.html#FailoverAppender). |
| target           | String  | Either "SYSTEM_OUT" or "SYSTEM_ERR". The default is "SYSTEM_OUT". |

