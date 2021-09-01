# AsyncAppender

1. AsyncAppender 接受对其他 Appender 的引用，并在单独的线程上将 LogEvents 写入它们。

2. 请注意，写入这些 Appender 时的异常将对应用程序隐藏。 

3. AsyncAppender 应该在它引用的 appender 之后配置，以允许它正确关闭。

默认情况下，AsyncAppender 使用不需要任何外部库的 java.util.concurrent.ArrayBlockingQueue。

请注意，多线程应用程序在使用这个 appender 时应该小心

阻塞队列容易受到锁争用的影响，我们的测试表明，当更多线程同时记录时，性能可能会变得更糟。

考虑使用 [无锁异步记录器](https://logging.apache.org/log4j/2.x/manual/async.html) 以获得最佳性能。

AsyncAppender Parameters

| Parameter Name       | Type                 | Description                                                  | 默认值                       |
| :------------------- | :------------------- | :----------------------------------------------------------- | ---------------------------- |
| AppenderRef          | String               | 要异步调用的 Appender 的名称。<br/>可以配置多个 AppenderRef 元素 |                              |
| blocking             | boolean              | 如果为 true，appender 将等到队列中有空闲插槽。<br />如果为 false，则如果队列已满，则事件将写入错误附加程序。 | true                         |
| shutdownTimeout      | integer              | 在关闭时   Appender 应该等待多少毫秒 才能刷新队列中的未完成日志事件 | 默认值为零，这意味着永远等待 |
| bufferSize           | integer              | 指定可以排队的最大事件数。. 请注意，当使用 a disruptor-style BlockingQueue, 此缓冲区大小必须是 2 的幂. 当记录日志过快导致底层异步记录器队列满了时 其行为决定于 [AsyncQueueFullPolicy](https://logging.apache.org/log4j/2.x/log4j-core/apidocs/org/apache/logging/log4j/core/async/AsyncQueueFullPolicy.html). | 1024                         |
| errorRef             | String               | 如果由于 appender 中的错误或队列已满而无法调用任何appender，则该值为要调用的appender的名称。. 如果未指定，则将忽略错误。 |                              |
| filter               | Filter               | 一个过滤器，用于确定事件是否应该由这个 Appender 处理。使用 CompositeFilter 可以使用多个过滤器。 |                              |
| name                 | String               | Appender 的名称。                                            |                              |
| ignoreExceptions     | boolean              | 设置为true,导致在附加事件时遇到异常被内部记录然后被忽略  当设置为 false 时，异常将传播给调用者。将此 Appender 包装在 [FailoverAppender](https://logging.apache.org/log4j/2.x/manual/appenders.html#FailoverAppender) 中时，您必须将此设置为 false。 | true                         |
| includeLocation      | boolean              | 提取位置是一项昂贵的操作（它可以使日志记录速度慢 5 - 20 倍）。. 为了提高性能，将日志事件添加到队列时，默认情况下不包括位置. 您可以通过设置 includeLocation="true" 来更改此设置。 | false                        |
| BlockingQueueFactory | BlockingQueueFactory | This element overrides what type of BlockingQueue to use. See [below documentation](http://logging.apache.org/log4j/2.x/manual/appenders.html#BlockingQueueFactory) for more details. |                              |

还有一些系统属性可用于维持应用程序吞吐量，即使底层 appender 无法跟上日志记录速率并且队列正在填满。

请参阅系统属性 [log4j2.AsyncQueueFullPolicy 和 log4j2.DiscardThreshold](http://logging.apache.org/log4j/2.x/manual/configuration.html#log4j2.AsyncQueueFullPolicy) 的详细信息。



# Example

## 标准写法

A typical AsyncAppender configuration might look like:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="warn" name="MyApp" packages="">
  <Appenders>
    <File name="MyFile" fileName="logs/app.log">
      <PatternLayout>
        <Pattern>%d %p %c{1.} [%t] %m%n</Pattern>
      </PatternLayout>
    </File>
    <Async name="Async">
      <AppenderRef ref="MyFile"/>
    </Async>
  </Appenders>
  <Loggers>
    <Root level="error">
      <AppenderRef ref="Async"/>
    </Root>
  </Loggers>
</Configuration>
```



## 覆盖默认队列

```xml
<Configuration name="LinkedTransferQueueExample">
  <Appenders>
    <List name="List"/>
    <Async name="Async" bufferSize="262144">
      <AppenderRef ref="List"/>
      <LinkedTransferQueue/>
    </Async>
  </Appenders>
  <Loggers>
    <Root>
      <AppenderRef ref="Async"/>
    </Root>
  </Loggers>
</Configuration>
```

# **默认队列实现内置实现**

| Plugin Name            | Description                                                  |
| :--------------------- | :----------------------------------------------------------- |
| ArrayBlockingQueue     | 默认实现                                                     |
| DisruptorBlockingQueue | This uses the [Conversant Disruptor](https://github.com/conversant/disruptor) implementation of BlockingQueue. 这个插件有一个可选的属性，spinPolicy |
| JCToolsBlockingQueue   | This uses [JCTools](https://jctools.github.io/JCTools/), specifically the MPSC bounded lock-free queue. |
| LinkedTransferQueue    | 这使用了 Java 7 中的新实现 LinkedTransferQueue。<br/><br/>请注意，此队列不使用 AsyncAppender 的 bufferSize 配置属性，因为 LinkedTransferQueue 不支持最大容量。 |

