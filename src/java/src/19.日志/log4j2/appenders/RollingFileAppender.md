# RollingFileAppender

RollingFileAppender 是一个 OutputStreamAppender，它写入 fileName 参数中命名的 File 并根据 **TriggeringPolicy** 和 **RolloverPolicy** 滚动文件



RollingFileAppender 使用 RollingFileManager（它扩展了 OutputStreamManager）来实际执行文件 I/O 并执行翻转。



虽然无法共享来自不同配置的 RolloverFileAppender，但如果 Manager 可访问，则可以共享 RollingFileManagers。



例如，servlet 容器中的两个 Web 应用程序可以有自己的配置，并且如果 Log4j 位于它们公共的 ClassLoader 中，则可以安全地写入同一个文件。



# TriggeringPolicy 与RolloverStrategy

1. RollingFileAppender 需要一个 TriggeringPolicy 和一个 RolloverStrategy。

2. 触发策略确定是否应该执行翻转，而 RolloverStrategy 定义应该如何完成翻转。

3. 如果没有配置 RolloverStrategy，RollingFileAppender 将使用 DefaultRolloverStrategy。

4. 从 log4j-2.5 开始，可以在 DefaultRolloverStrategy 中配置自定义删除操作以在翻转时运行。

5. 从 2.8 开始，如果没有配置文件名，那么将使用 DirectWriteRolloverStrategy 而不是 DefaultRolloverStrategy。
6. 从 log4j-2.9 开始，可以在 DefaultRolloverStrategy 中配置自定义 POSIX 文件属性视图操作以在翻转时运行，如果未定义，将从 RollingFileAppender 继承的 POSIX 文件属性视图将被应用。
7. RollingFileAppender 不支持文件锁定。





# Parameters

| Parameter Name   | Type             | Description                                                  |
| :--------------- | :--------------- | :----------------------------------------------------------- |
| append           | boolean          | When true - the default, records will be appended to the end of the file. When set to false, the file will be cleared before new records are written. |
| bufferedIO       | boolean          | When true - the default, records will be written to a buffer and the data will be written to disk when the buffer is full or, if immediateFlush is set, when the record is written. File locking cannot be used with bufferedIO. Performance tests have shown that using buffered I/O significantly improves performance, even if immediateFlush is enabled. |
| bufferSize       | int              | When bufferedIO is true, this is the buffer size, the default is 8192 bytes. |
| createOnDemand   | boolean          | The appender creates the file on-demand. The appender only creates the file when a log event passes all filters and is routed to this appender. Defaults to false. |
| filter           | Filter           | A Filter to determine if the event should be handled by this Appender. More than one Filter may be used by using a CompositeFilter. |
| fileName         | String           | The name of the file to write to. If the file, or any of its parent directories, do not exist, they will be created. |
| filePattern      | String           | 归档日志文件的文件名模式。模式的格式取决于所使用的 RolloverPolicy。 .DefaultRolloverPolicy 将接受与 [SimpleDateFormat](http://download.oracle.com/javase/7/docs/api/java/text/SimpleDateFormat.html) 兼容的日期/时间模式和 或  %i 计算器. 该模式还支持运行时插值，因此任何 任何 Lookups (such as the [DateLookup](http://logging.apache.org/log4j/2.x/manual/lookups.html#DateLookup)) can be included in the pattern. |
| immediateFlush   | boolean          | When set to true - the default, each write will be followed by a flush. This will guarantee the data is written to disk but could impact performance.Flushing after every write is only useful when using this appender with synchronous loggers. Asynchronous loggers and appenders will automatically flush at the end of a batch of events, even if immediateFlush is set to false. This also guarantees the data is written to disk but is more efficient. |
| layout           | Layout           | 用于格式化 LogEvent 的布局。<br/><br/>如果未提供布局，则将使用“%m%n”的默认模式布局。 |
| name             | String           | The name of the Appender.                                    |
| policy           | TriggeringPolicy | 用于确定是否应发生翻转的策略。                               |
| strategy         | RolloverStrategy | 用于确定存档文件的名称和位置的策略                           |
| ignoreExceptions | boolean          | The default is true, causing exceptions encountered while appending events to be internally logged and then ignored. When set to false exceptions will be propagated to the caller, instead. You must set this to false when wrapping this Appender in a [FailoverAppender](http://logging.apache.org/log4j/2.x/manual/appenders.html#FailoverAppender). |
| filePermissions  | String           | File attribute permissions in POSIX format to apply whenever the file is created.Underlying files system shall support [POSIX](https://docs.oracle.com/javase/7/docs/api/java/nio/file/attribute/PosixFileAttributeView.html) file attribute view.Examples: rw------- or rw-rw-rw- etc... |
| fileOwner        | String           | File owner to define whenever the file is created.Changing file's owner may be restricted for security reason and Operation not permitted IOException thrown. Only processes with an effective user ID equal to the user ID of the file or with appropriate privileges may change the ownership of a file if [_POSIX_CHOWN_RESTRICTED](http://www.gnu.org/software/libc/manual/html_node/Options-for-Files.html) is in effect for path.Underlying files system shall support file [owner](https://docs.oracle.com/javase/7/docs/api/java/nio/file/attribute/FileOwnerAttributeView.html) attribute view. |
| fileGroup        | String           | File group to define whenever the file is created.Underlying files system shall support [POSIX](https://docs.oracle.com/javase/7/docs/api/java/nio/file/attribute/PosixFileAttributeView.html) file attribute view. |

# Triggering Policies

## Composite Triggering Policy

CompositeTriggeringPolicy 组合了多个触发策略，如果任何配置的策略返回 true，则返回 true。

CompositeTriggeringPolicy 只需将其他策略包装在 Policies 元素中即可配置。



例如，以下 XML 片段定义了在 JVM 启动、日志大小达到 20 兆字节、以及当前日期不再与日志的开始日期匹配时滚动日志的策略。

```xml
<Policies>
  <OnStartupTriggeringPolicy />
  <SizeBasedTriggeringPolicy size="20 MB" />
  <TimeBasedTriggeringPolicy />
</Policies>
```



## Cron Triggering Policy

CronTriggeringPolicy 基于 cron 表达式触发翻转

### Parameters

| Parameter Name    | Type    | Description                                                  |
| :---------------- | :------ | :----------------------------------------------------------- |
| schedule          | String  | cron 表达式。该表达式与 Quartz 调度程序中允许的表达式相同。 See [CronExpression](http://logging.apache.org/log4j/2.x/log4j-core/apidocs/org/apache/logging/log4j/core/util/CronExpression.html) for a full description of the expression. |
| evaluateOnStartup | boolean | 启动时，cron 表达式将根据文件的最后修改时间戳进行评估。<br/>如果 cron 表达式指示在该时间和当前时间之间应该发生翻转，则文件将立即翻转。 |

## OnStartup Triggering Policy

如果日志文件早于当前 JVM 的启动时间并且达到或超过最小文件大小，则 OnStartupTriggeringPolicy 策略会导致翻转。

| Parameter Name | Type | Description                                                  |
| :------------- | :--- | :----------------------------------------------------------- |
| minSize        | long | 文件必须翻转的最小大小。<br/>无论文件大小如何，大小为零都会导致翻转。<br/>默认值为 1，这将防止滚动空文件。 |

## SizeBased Triggering Policy

一旦文件达到指定的大小，SizeBasedTriggeringPolicy 会导致翻转。

大小可以以字节为单位指定，后缀为 KB、MB 或 GB，例如 20MB。

当与基于时间的触发策略结合使用时，文件模式必须包含 %i 否则目标文件将在每次翻转时被覆盖，因为 SizeBased 触发策略不会导致文件名中的时间戳值发生更改。

当在没有基于时间的触发策略的情况下使用时，SizeBased 触发策略将导致时间戳值发生变化。

## TimeBased Triggering Policy

一旦日期/时间模式不再适用于活动文件， TimeBasedTriggeringPolicy 会导致翻转。

此策略接受一个间隔属性，该属性指示基于时间模式和  一个boolean属性指示是否 应该调整间隔以使得下一次翻转发生在边界上。

### Parameters

| Parameter Name | Type    | Description                                                  |
| :------------- | :------ | :----------------------------------------------------------- |
| interval       | integer | 根据日期模式中最具体的时间单位，应多久进行一次翻转。.例如，日期模式以小时为最具体的项目，并且每 4 小时会发生 4 次翻转。. The default value is 1. |
| modulate       | boolean | 指示是否应调整间隔以使下一次翻转发生在间隔边界上。. 例如，如果项目是小时，当前小时是凌晨 3 点，间隔是 4，那么第一次翻转将在凌晨 4 点发生，然后下一次翻转将在上午 8 点、中午、下午 4 点等发生。 |
| maxRandomDelay | integer | Indicates the maximum number of seconds to randomly delay a rollover. By default, this is 0 which indicates no delay. This setting is useful on servers where multiple applications are configured to rollover log files at the same time and can spread the load of doing so across time. |

# Rollover Strategies

## Default Rollover Strategy

默认翻转策略接受日期/时间模式和一个在整数 

上述属性在 RollingFileAppender  本身上指定该

如果存在日期/时间模式，它将被当前的日期和时间值替换。

如果模式包含一个整数，它将在每次翻转时递增。

如果模式中同时包含日期/时间和整数，则整数将递增，直到日期/时间模式的结果发生变化。

如果文件模式以“.gz”、“.zip”、“.bz2”、“.deflate”、“.pack200”或“.xz”结尾，则生成的存档将使用与后缀匹配的压缩方案进行压缩



bzip2、Deflate、Pack200 和 XZ 格式需要 Apache Commons Compress。

此外，XZ 需要 XZ for Java。

该模式还可能包含可以在运行时解析的查找引用，如下例所示。

默认翻转策略支持三种递增计数器的变体。

假设 min 属性设置为 1，max 属性设置为 3，文件名为“foo.log”，文件名模式为“foo-%i.log”。

| Number of rollovers | Active output target | Archived log files              | Description                                                  |
| :------------------ | :------------------- | :------------------------------ | :----------------------------------------------------------- |
| 0                   | foo.log              | -                               | All logging is going to the initial file.                    |
| 1                   | foo.log              | foo-1.log                       | 在第一次翻转期间，foo.log 被重命名为 foo-1.log。一个新的 foo.log 文件被创建并开始写入。 |
| 2                   | foo.log              | foo-2.log, foo-1.log            | 在第二次翻转期间，foo.log 被重命名为 foo-2.log。<br />一个新的 foo.log 文件被创建并开始写入。 |
| 3                   | foo.log              | foo-3.log, foo-2.log, foo-1.log | 在第三次翻转期间，foo.log 被重命名为 foo-3.log。一个新的 foo.log 文件被创建并开始写入。 |
| 4                   | foo.log              | foo-3.log, foo-2.log, foo-1.log | 在第四次及以后的翻转中，foo-1.log被删除，foo-2.log被重命名为foo-1.log，foo-3.log被重命名为foo-2.log，foo.log被重命名为foo-3.日志。<br/>一个新的 foo.log 文件被创建并开始写入。 |

相比之下，当 fileIndex 属性设置为“min”但所有其他设置都相同时，将执行“固定窗口”策略。

| Number of rollovers | Active output target | Archived log files              | Description                                                  |
| :------------------ | :------------------- | :------------------------------ | :----------------------------------------------------------- |
| 0                   | foo.log              | -                               | All logging is going to the initial file.                    |
| 1                   | foo.log              | foo-1.log                       | During the first rollover foo.log is renamed to foo-1.log. A new foo.log file is created and starts being written to. |
| 2                   | foo.log              | foo-1.log, foo-2.log            | During the second rollover foo-1.log is renamed to foo-2.log and foo.log is renamed to foo-1.log. A new foo.log file is created and starts being written to. |
| 3                   | foo.log              | foo-1.log, foo-2.log, foo-3.log | During the third rollover foo-2.log is renamed to foo-3.log, foo-1.log is renamed to foo-2.log and foo.log is renamed to foo-1.log. A new foo.log file is created and starts being written to. |
| 4                   | foo.log              | foo-1.log, foo-2.log, foo-3.log | In the fourth and subsequent rollovers, foo-3.log is deleted, foo-2.log is renamed to foo-3.log, foo-1.log is renamed to foo-2.log and foo.log is renamed to foo-1.log. A new foo.log file is created and starts being written to. |

最后，从 2.8 版开始，如果 fileIndex 属性设置为“nomax”，那么最小值和最大值将被忽略，文件编号将增加 1，并且每次翻转都会有一个递增的更高值，没有最大文件数。

### Parameters

| Parameter Name            | Type    | Description                                                  |
| :------------------------ | :------ | :----------------------------------------------------------- |
| fileIndex                 | String  | If set to "max" (the default), files with a higher index will be newer than files with a smaller index. If set to "min", file renaming and the counter will follow the Fixed Window strategy described above. |
| min                       | integer | The minimum value of the counter. The default value is 1.    |
| max                       | integer | The maximum value of the counter. Once this values is reached older archives will be deleted on subsequent rollovers. The default value is 7. |
| compressionLevel          | integer | Sets the compression level, 0-9, where 0 = none, 1 = best speed, through 9 = best compression. Only implemented for ZIP files. |
| tempCompressedFilePattern | String  | The pattern of the file name of the archived log file during compression. |

## DirectWrite Rollover Strategy

DirectWriteRolloverStrategy 使日志事件直接写入由文件模式表示的文件。

使用此策略不会执行文件重命名。

如果基于大小的触发策略导致在指定时间段内写入多个文件，它们将从 1 开始编号并不断递增，直到发生基于时间的翻转。

警告：如果文件模式有一个表示应该进行压缩的后缀，则当应用程序关闭时，当前文件将不会被压缩。

此外，如果时间改变以致文件模式不再与当前文件匹配，则它也不会在启动时被压缩。

### Parameters

| Parameter Name            | Type    | Description                                                  |
| :------------------------ | :------ | :----------------------------------------------------------- |
| maxFiles                  | String  | The maximum number of files to allow in the time period matching the file pattern. If the number of files is exceeded the oldest file will be deleted. If specified, the value must be greater than 1. If the value is less than zero or omitted then the number of files will not be limited. |
| compressionLevel          | integer | Sets the compression level, 0-9, where 0 = none, 1 = best speed, through 9 = best compression. Only implemented for ZIP files. |
| tempCompressedFilePattern | String  | The pattern of the file name of the archived log file during compression. |

下面是一个示例配置，它使用具有基于时间和大小的触发策略的 RollingFileAppender，将在同一天 (1-7) 创建多达 7 个存档，这些存档存储在基于当前年和月的目录中，并将

使用 gzip 压缩每个存档：



```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="warn" name="MyApp" packages="">
  <Appenders>
    <RollingFile name="RollingFile" fileName="logs/app.log"
                 filePattern="logs/$${date:yyyy-MM}/app-%d{MM-dd-yyyy}-%i.log.gz">
      <PatternLayout>
        <Pattern>%d %p %c{1.} [%t] %m%n</Pattern>
      </PatternLayout>
      <Policies>
        <TimeBasedTriggeringPolicy />
        <SizeBasedTriggeringPolicy size="250 MB"/>
      </Policies>
    </RollingFile>
  </Appenders>
  <Loggers>
    <Root level="error">
      <AppenderRef ref="RollingFile"/>
    </Root>
  </Loggers>
</Configuration>

```

第二个示例显示了一个翻转策略，该策略将在删除之前最多保留 20 个文件。

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="warn" name="MyApp" packages="">
  <Appenders>
    <RollingFile name="RollingFile" fileName="logs/app.log"
                 filePattern="logs/$${date:yyyy-MM}/app-%d{MM-dd-yyyy}-%i.log.gz">
      <PatternLayout>
        <Pattern>%d %p %c{1.} [%t] %m%n</Pattern>
      </PatternLayout>
      <Policies>
        <TimeBasedTriggeringPolicy />
        <SizeBasedTriggeringPolicy size="250 MB"/>
      </Policies>
      <DefaultRolloverStrategy max="20"/>
    </RollingFile>
  </Appenders>
  <Loggers>
    <Root level="error">
      <AppenderRef ref="RollingFile"/>
    </Root>
  </Loggers>
</Configuration>
```

下面是一个示例配置，它使用具有基于时间和大小的触发策略的 RollingFileAppender，将在同一天 (1-7) 创建多达 7 个存档，这些存档存储在基于当前年和月的目录中，并将

使用 gzip 压缩每个存档，当小时可被 6 整除时，将每 6 小时滚动一次：

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="warn" name="MyApp" packages="">
  <Appenders>
    <RollingFile name="RollingFile" fileName="logs/app.log"
                 filePattern="logs/$${date:yyyy-MM}/app-%d{yyyy-MM-dd-HH}-%i.log.gz">
      <PatternLayout>
        <Pattern>%d %p %c{1.} [%t] %m%n</Pattern>
      </PatternLayout>
      <Policies>
        <TimeBasedTriggeringPolicy interval="6" modulate="true"/>
        <SizeBasedTriggeringPolicy size="250 MB"/>
      </Policies>
    </RollingFile>
  </Appenders>
  <Loggers>
    <Root level="error">
      <AppenderRef ref="RollingFile"/>
    </Root>
  </Loggers>
</Configuration>
```

此示例配置使用具有基于 cron 和大小的触发策略的 RollingFileAppender，并直接写入无限数量的存档文件。 

cron 触发器每小时导致一次翻转，而文件大小限制为 250MB

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="warn" name="MyApp" packages="">
  <Appenders>
    <RollingFile name="RollingFile" filePattern="logs/app-%d{yyyy-MM-dd-HH}-%i.log.gz">
      <PatternLayout>
        <Pattern>%d %p %c{1.} [%t] %m%n</Pattern>
      </PatternLayout>
      <Policies>
        <CronTriggeringPolicy schedule="0 0 * * * ?"/>
        <SizeBasedTriggeringPolicy size="250 MB"/>
      </Policies>
    </RollingFile>
  </Appenders>
  <Loggers>
    <Root level="error">
      <AppenderRef ref="RollingFile"/>
    </Root>
  </Loggers>
</Configuration>
```



此示例配置与前面的相同，但将每小时保存的文件数限制为 10：

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="warn" name="MyApp" packages="">
  <Appenders>
    <RollingFile name="RollingFile" filePattern="logs/app-%d{yyyy-MM-dd-HH}-%i.log.gz">
      <PatternLayout>
        <Pattern>%d %p %c{1.} [%t] %m%n</Pattern>
      </PatternLayout>
      <Policies>
        <CronTriggeringPolicy schedule="0 0 * * * ?"/>
        <SizeBasedTriggeringPolicy size="250 MB"/>
      </Policies>
      <DirectWriteRolloverStrategy maxFiles="10"/>
    </RollingFile>
  </Appenders>
  <Loggers>
    <Root level="error">
      <AppenderRef ref="RollingFile"/>
    </Root>
  </Loggers>
</Configuration>
```



## Log Archive Retention Policy

> Delete on Rollover

Log4j-2.5 引入了一个 Delete 操作，与 DefaultRolloverStrategy max 属性相比，它使用户可以更好地控制在翻转时删除哪些文件。

删除操作允许用户配置一个或多个条件来选择相对于基本目录删除的文件。



请注意，可以删除任何文件，而不仅仅是滚动日志文件，因此请谨慎使用此操作！

使用 testMode 参数，您可以测试您的配置，而不会意外删除错误的文件。



### Parameters

| Parameter Name  | Type            | Description                                                  |
| :-------------- | :-------------- | :----------------------------------------------------------- |
| basePath        | String          | *Required.* Base path from where to start scanning for files to delete. |
| maxDepth        | int             | 要访问的目录的最大级别数。<br/>值 0 表示仅访问起始文件（基本路径本身），除非被安全管理器拒绝。 <br/>Integer.MAX_VALUE 值表示应该访问所有级别。默认值为 1，表示仅指定基目录中的文件。 |
| followLinks     | boolean         | Whether to follow symbolic links. Default is false.          |
| testMode        | boolean         | 如果为 true，则不会删除文件，而是在 INFO 级别将消息打印到 [状态记录器](http://logging.apache.org/log4j/2.x/manual/configuration.html#StatusMessages)。 |
| pathSorter      | PathSorter      | 实现 [PathSorter](http://logging.apache.org/log4j/2.x/log4j-core/apidocs/org/apache/logging/log4j/core/appender/rolling/action/PathSorter.html)<br/>在选择要删除的文件之前对文件进行排序的界面。<br/>默认设置是首先对最近修改的文件进行排序。 |
| pathConditions  | PathCondition[] | 如果未指定 ScriptCondition 则为必需 ,一个或多个 PathCondition 元素。如果指定了多个条件，则它们都需要接受条件才能删除, 条件可以嵌套，在这种情况下，仅当外部条件接受路径时才评估内部条件.如果条件不是嵌套的，它们可以按任何顺序进行计算。条件也可以通过使用 IfAll、IfAny 和 IfNot 复合条件与逻辑运算符 AND、OR 和 NOT 组合。.Users can create custom conditions or use the built-in conditions:[IfFileName](http://logging.apache.org/log4j/2.x/manual/appenders.html#DeleteIfFileName) - accepts files whose path (relative to the base path) matches a [regular expression](https://docs.oracle.com/javase/7/docs/api/java/util/regex/Pattern.html) or a [glob](https://docs.oracle.com/javase/7/docs/api/java/nio/file/FileSystem.html#getPathMatcher(java.lang.String)).[IfLastModified](http://logging.apache.org/log4j/2.x/manual/appenders.html#DeleteIfLastModified) - accepts files that are as old as or older than the specified [duration](http://logging.apache.org/log4j/2.x/log4j-core/apidocs/org/apache/logging/log4j/core/appender/rolling/action/Duration.html#parseCharSequence).[IfAccumulatedFileCount](http://logging.apache.org/log4j/2.x/manual/appenders.html#DeleteIfAccumulatedFileCount) - accepts paths after some count threshold is exceeded during the file tree walk.[IfAccumulatedFileSize](http://logging.apache.org/log4j/2.x/manual/appenders.html#DeleteIfAccumulatedFileSize) - accepts paths after the accumulated file size threshold is exceeded during the file tree walk.IfAll - accepts a path if all nested conditions accept it (logical AND). Nested conditions may be evaluated in any order.IfAny - accepts a path if one of the nested conditions accept it (logical OR). Nested conditions may be evaluated in any order.IfNot - accepts a path if the nested condition does not accept it (logical NOT). |
| scriptCondition | ScriptCondition | *Required if no PathConditions are specified.* A ScriptCondition element specifying a script.The ScriptCondition should contain a [Script, ScriptRef or ScriptFile](http://logging.apache.org/log4j/2.x/manual/appenders.html#ScriptCondition) element that specifies the logic to be executed. (See also the [ScriptFilter](http://logging.apache.org/log4j/2.x/manual/filters.html#Script) documentation for more examples of configuring ScriptFiles and ScriptRefs.)The script is passed a number of [parameters](http://logging.apache.org/log4j/2.x/manual/appenders.html#ScriptParameters), including a list of paths found under the base path (up to maxDepth) and must return a list with the paths to delete. |





### IfFileName Condition Parameters

| Parameter Name   | Type            | Description                                                  |
| :--------------- | :-------------- | :----------------------------------------------------------- |
| glob             | String          | *Required if regex not specified.* Matches the relative path (relative to the base path) using a limited pattern language that resembles regular expressions but with a [simpler syntax](https://docs.oracle.com/javase/7/docs/api/java/nio/file/FileSystem.html#getPathMatcher(java.lang.String)). |
| regex            | String          | *Required if glob not specified.* Matches the relative path (relative to the base path) using a regular expression as defined by the [Pattern](https://docs.oracle.com/javase/7/docs/api/java/util/regex/Pattern.html) class. |
| nestedConditions | PathCondition[] | An optional set of nested [PathConditions](http://logging.apache.org/log4j/2.x/manual/appenders.html#DeletePathCondition). If any nested conditions exist they all need to accept the file before it is deleted. Nested conditions are only evaluated if the outer condition accepts a file (if the path name matches). |

### IfLastModified Condition Parameters

| Parameter Name   | Type            | Description                                                  |
| :--------------- | :-------------- | :----------------------------------------------------------- |
| age              | String          | *Required.* Specifies a [duration](http://logging.apache.org/log4j/2.x/log4j-core/apidocs/org/apache/logging/log4j/core/appender/rolling/action/Duration.html#parseCharSequence). The condition accepts files that are as old or older than the specified duration. |
| nestedConditions | PathCondition[] | An optional set of nested [PathConditions](http://logging.apache.org/log4j/2.x/manual/appenders.html#DeletePathCondition). If any nested conditions exist they all need to accept the file before it is deleted. Nested conditions are only evaluated if the outer condition accepts a file (if the file is old enough). |

### IfAccumulatedFileCount Condition Parameters

| Parameter Name   | Type            | Description                                                  |
| :--------------- | :-------------- | :----------------------------------------------------------- |
| exceeds          | int             | *Required.* The threshold count from which files will be deleted. |
| nestedConditions | PathCondition[] | An optional set of nested [PathConditions](http://logging.apache.org/log4j/2.x/manual/appenders.html#DeletePathCondition). If any nested conditions exist they all need to accept the file before it is deleted. Nested conditions are only evaluated if the outer condition accepts a file (if the threshold count has been exceeded). |

### IfAccumulatedFileSize Condition Parameters

| Parameter Name   | Type            | Description                                                  |
| :--------------- | :-------------- | :----------------------------------------------------------- |
| exceeds          | String          | *Required.* The threshold accumulated file size from which files will be deleted. The size can be specified in bytes, with the suffix KB, MB or GB, for example 20MB. |
| nestedConditions | PathCondition[] | An optional set of nested [PathConditions](http://logging.apache.org/log4j/2.x/manual/appenders.html#DeletePathCondition). If any nested conditions exist they all need to accept the file before it is deleted. Nested conditions are only evaluated if the outer condition accepts a file (if the threshold accumulated file size has been exceeded). |

下面是一个示例配置，它使用 RollingFileAppender 和配置为每天午夜触发的 cron 触发策略。

档案存储在基于当前年份和月份的目录中。

基目录下与“*/app-*.log.gz”glob 匹配且存在于 60 天或更旧的所有文件将在翻转时被删除。



```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="warn" name="MyApp" packages="">
  <Properties>
    <Property name="baseDir">logs</Property>
  </Properties>
  <Appenders>
    <RollingFile name="RollingFile" fileName="${baseDir}/app.log"
          filePattern="${baseDir}/$${date:yyyy-MM}/app-%d{yyyy-MM-dd}.log.gz">
      <PatternLayout pattern="%d %p %c{1.} [%t] %m%n" />
      <CronTriggeringPolicy schedule="0 0 0 * * ?"/>
      <DefaultRolloverStrategy>
        <Delete basePath="${baseDir}" maxDepth="2">
          <IfFileName glob="*/app-*.log.gz" />
          <IfLastModified age="60d" />
        </Delete>
      </DefaultRolloverStrategy>
    </RollingFile>
  </Appenders>
  <Loggers>
    <Root level="error">
      <AppenderRef ref="RollingFile"/>
    </Root>
  </Loggers>
</Configuration>
```



下面是一个示例配置，它使用具有基于时间和大小的触发策略的 RollingFileAppender，将在同一天 (1-100) 创建多达 100 个存档，这些存档存储在基于当前年和月的目录中，并将

使用 gzip 压缩每个存档，并将每小时滚动一次。

在每次翻转期间，此配置将删除与“*/app-*.log.gz”匹配且 30 天或更旧的文件，但保留最近的 100 GB 或最近的 10 个文件，以先到者为准。



```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="warn" name="MyApp" packages="">
  <Properties>
    <Property name="baseDir">logs</Property>
  </Properties>
  <Appenders>
    <RollingFile name="RollingFile" fileName="${baseDir}/app.log"
          filePattern="${baseDir}/$${date:yyyy-MM}/app-%d{yyyy-MM-dd-HH}-%i.log.gz">
      <PatternLayout pattern="%d %p %c{1.} [%t] %m%n" />
      <Policies>
        <TimeBasedTriggeringPolicy />
        <SizeBasedTriggeringPolicy size="250 MB"/>
      </Policies>
      <DefaultRolloverStrategy max="100">
        <!--
        Nested conditions: the inner condition is only evaluated on files
        for which the outer conditions are true.
        -->
        <Delete basePath="${baseDir}" maxDepth="2">
          <IfFileName glob="*/app-*.log.gz">
            <IfLastModified age="30d">
              <IfAny>
                <IfAccumulatedFileSize exceeds="100 GB" />
                <IfAccumulatedFileCount exceeds="10" />
              </IfAny>
            </IfLastModified>
          </IfFileName>
        </Delete>
      </DefaultRolloverStrategy>
    </RollingFile>
  </Appenders>
  <Loggers>
    <Root level="error">
      <AppenderRef ref="RollingFile"/>
    </Root>
  </Loggers>
</Configuration>
```

### ScriptCondition Parameters

| Parameter Name | Type                            | Description                                                  |
| :------------- | :------------------------------ | :----------------------------------------------------------- |
| script         | Script, ScriptFile or ScriptRef | The Script element that specifies the logic to be executed. The script is passed a list of paths found under the base path and must return the paths to delete as a java.util.List<[PathWithAttributes](http://logging.apache.org/log4j/2.x/log4j-core/apidocs/org/apache/logging/log4j/core/appender/rolling/action/PathWithAttributes.html)>. See also the [ScriptFilter](http://logging.apache.org/log4j/2.x/manual/filters.html#Script) documentation for an example of how ScriptFiles and ScriptRefs can be configured. |

### Script Parameters

| Parameter Name | Type                                                         | Description                                                  |
| :------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| basePath       | java.nio.file.Path                                           | The directory from where the Delete action started scanning for files to delete. Can be used to relativize the paths in the pathList. |
| pathList       | java.util.List<[PathWithAttributes](http://logging.apache.org/log4j/2.x/log4j-core/apidocs/org/apache/logging/log4j/core/appender/rolling/action/PathWithAttributes.html)> | The list of paths found under the base path up to the specified max depth, sorted most recently modified files first. The script is free to modify and return this list. |
| statusLogger   | StatusLogger                                                 | The StatusLogger that can be used to log internal events during script execution. |
| configuration  | Configuration                                                | The Configuration that owns this ScriptCondition.            |
| substitutor    | StrSubstitutor                                               | The StrSubstitutor used to replace lookup variables.         |
| ?              | String                                                       | Any properties declared in the configuration.                |

下面是一个示例配置，它使用 RollingFileAppender 和配置为每天午夜触发的 cron 触发策略。

档案存储在基于当前年份和月份的目录中。

该脚本返回日期为 13 日星期五的基本目录下的滚动文件列表。

删除操作将删除脚本返回的所有文件。

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="trace" name="MyApp" packages="">
  <Properties>
    <Property name="baseDir">logs</Property>
  </Properties>
  <Appenders>
    <RollingFile name="RollingFile" fileName="${baseDir}/app.log"
          filePattern="${baseDir}/$${date:yyyy-MM}/app-%d{yyyyMMdd}.log.gz">
      <PatternLayout pattern="%d %p %c{1.} [%t] %m%n" />
      <CronTriggeringPolicy schedule="0 0 0 * * ?"/>
      <DefaultRolloverStrategy>
        <Delete basePath="${baseDir}" maxDepth="2">
          <ScriptCondition>
            <Script name="superstitious" language="groovy"><![CDATA[
                import java.nio.file.*;
 
                def result = [];
                def pattern = ~/\d*\/app-(\d*)\.log\.gz/;
 
                pathList.each { pathWithAttributes ->
                  def relative = basePath.relativize pathWithAttributes.path
                  statusLogger.trace 'SCRIPT: relative path=' + relative + " (base=$basePath)";
 
                  // remove files dated Friday the 13th
 
                  def matcher = pattern.matcher(relative.toString());
                  if (matcher.find()) {
                    def dateString = matcher.group(1);
                    def calendar = Date.parse("yyyyMMdd", dateString).toCalendar();
                    def friday13th = calendar.get(Calendar.DAY_OF_MONTH) == 13 \
                                  && calendar.get(Calendar.DAY_OF_WEEK) == Calendar.FRIDAY;
                    if (friday13th) {
                      result.add pathWithAttributes;
                      statusLogger.trace 'SCRIPT: deleting path ' + pathWithAttributes;
                    }
                  }
                }
                statusLogger.trace 'SCRIPT: returning ' + result;
                result;
              ]] >
            </Script>
          </ScriptCondition>
        </Delete>
      </DefaultRolloverStrategy>
    </RollingFile>
  </Appenders>
  <Loggers>
    <Root level="error">
      <AppenderRef ref="RollingFile"/>
    </Root>
  </Loggers>
</Configuration>
```

## Log Archive File Attribute View Policy

> Custom file attribute on Rollover



og4j-2.9 引入了 PosixViewAttribute 操作，使用户可以更好地控制应应用哪些文件属性权限、所有者和组。 

PosixViewAttribute 操作允许用户配置一个或多个条件来选择相对于基目录的合格文件。

### PosixViewAttribute Parameters

| Parameter Name  | Type            | Description                                                  |
| :-------------- | :-------------- | :----------------------------------------------------------- |
| basePath        | String          | *Required.* Base path from where to start scanning for files to apply attributes. |
| maxDepth        | int             | The maximum number of levels of directories to visit. A value of 0 means that only the starting file (the base path itself) is visited, unless denied by the security manager. A value of Integer.MAX_VALUE indicates that all levels should be visited. The default is 1, meaning only the files in the specified base directory. |
| followLinks     | boolean         | Whether to follow symbolic links. Default is false.          |
| pathConditions  | PathCondition[] | see [DeletePathCondition](http://logging.apache.org/log4j/2.x/manual/appenders.html#DeletePathCondition) |
| filePermissions | String          | File attribute permissions in POSIX format to apply when action is executed.Underlying files system shall support [POSIX](https://docs.oracle.com/javase/7/docs/api/java/nio/file/attribute/PosixFileAttributeView.html) file attribute view.Examples: rw------- or rw-rw-rw- etc... |
| fileOwner       | String          | File owner to define when action is executed.Changing file's owner may be restricted for security reason and Operation not permitted IOException thrown. Only processes with an effective user ID equal to the user ID of the file or with appropriate privileges may change the ownership of a file if [_POSIX_CHOWN_RESTRICTED](http://www.gnu.org/software/libc/manual/html_node/Options-for-Files.html) is in effect for path.Underlying files system shall support file [owner](https://docs.oracle.com/javase/7/docs/api/java/nio/file/attribute/FileOwnerAttributeView.html) attribute view. |
| fileGroup       | String          | File group to define when action is executed.Underlying files system shall support [POSIX](https://docs.oracle.com/javase/7/docs/api/java/nio/file/attribute/PosixFileAttributeView.html) file attribute view. |

以下是使用 RollingFileAppender 并为当前和滚动日志文件定义不同 POSIX 文件属性视图的示例配置。



```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="trace" name="MyApp" packages="">
  <Properties>
    <Property name="baseDir">logs</Property>
  </Properties>
  <Appenders>
    <RollingFile name="RollingFile" fileName="${baseDir}/app.log"
          		 filePattern="${baseDir}/$${date:yyyy-MM}/app-%d{yyyyMMdd}.log.gz"
                 filePermissions="rw-------">
      <PatternLayout pattern="%d %p %c{1.} [%t] %m%n" />
      <CronTriggeringPolicy schedule="0 0 0 * * ?"/>
      <DefaultRolloverStrategy stopCustomActionsOnError="true">
        <PosixViewAttribute basePath="${baseDir}/$${date:yyyy-MM}" filePermissions="r--r--r--">
        	<IfFileName glob="*.gz" />
        </PosixViewAttribute>
      </DefaultRolloverStrategy>
    </RollingFile>
  </Appenders>
 
  <Loggers>
    <Root level="error">
      <AppenderRef ref="RollingFile"/>
    </Root>
  </Loggers>
 
</Configuration>
```





