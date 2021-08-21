# 日志介绍

​	几乎每个大型应用程序都包含自己的日志记录或跟踪 API，the E.U. [SEMPER](http://www.semper.org/) project  决定实现自己记录API、这是1996年初，经过无数次的改进、several incarnations 和大量工作，API 已经演变成 log4j，一个流行的 Java 日志记录包，该软件包是在 [Apache 软件许可证](https://logging.apache.org/log4j/2.x/LICENSE)下分发的，

​	Apache 软件许可证是由[开源计划](http://www.opensource.org/)认证的成熟的开源许可证。可以在 http://logging.apache.org/log4j/2.x/index.html 找到最新的 log4j 版本，包括完整的源代码、类文件和文档



​		将日志语句插入代码是一种低技术的调试方法。这也可能是唯一的方法，因为调试器并不总是可用的。这通常适用于多线程应用程序和整个分布式应用程序。

​		经验表明，日志记录是开发周期的重要组成部分。它提供了几个优点。

1. 它提供有关应用程序运行的精确上下文
2. 一旦插入到代码中，日志输出的生成就不需要人工干预
3. 此外，日志输出可以保存在持久性介质中，以备日后研究
4. 除了在开发周期中使用之外，一个足够丰富的日志包也可以被视为一个审计工具





# Log4j 2介绍

Log4j 1.x 已被广泛采用并用于许多应用程序。然而，多年来它的发展已经放缓。

由于需要兼容非常旧的 Java 版本，它变得更加难以维护，并于 2015 年 8 月终止。它的替代方案 SLF4J/Logback 对该框架进行了许多必要的改进。那么为什么要为 Log4j 2 烦恼呢？

1. Log4j 2 旨在用作审计日志记录框架。 Log4j 1.x 和 Logback 在重新配置时都会丢失事件。 Log4j 2 不会。
2. Log4j 2 包含基于 [LMAX Disruptor](https://lmax-exchange.github.io/disruptor/) 库的下一代异步记录器。在多线程场景中，异步 Logger 的吞吐量比 Log4j 1.x 和 Logback 高 10 倍，延迟低几个数量级。
3. Log4j 2 对于独立应用程序是 [garbage free](https://logging.apache.org/log4j/2.x/manual/garbagefree.html)，在稳定状态日志记录期间对于 Web 应用程序来说是低垃圾。这减少了垃圾收集器的压力，并且可以提供更好的响应时间性能。
4. Log4j 2 使用[插件系统](https://logging.apache.org/log4j/2.x/manual/plugins.html)，通过添加新的 [Appenders](https://logging.apache.org/log4j/2.x/manual/appenders.html), [Filters](https://logging.apache.org/log4j/2.x/manual/filters.html), [Layouts](https://logging.apache.org/log4j/2.x/manual/layouts.html), [Lookups](https://logging.apache.org/log4j/2.x/manual/lookups.html)  and Pattern Converters ，无需对 Log4j 进行任何更改，就可以非常轻松地[扩展框架](https://logging.apache.org/log4j/2.x/manual/extending.html)。
5. 由于插件系统配置更简单。配置中的条目不需要指定类名。
6. 支持[自定义日志级别](https://logging.apache.org/log4j/2.x/manual/customloglevels.html)。自定义日志级别可以在代码或配置中定义。
7. 支持 lambda 表达式。仅当启用了请求的日志级别时，在 Java 8 上运行的客户端代码才能使用 lambda 表达式来延迟构建日志消息。不需要显式级别检查，从而使代码更清晰。
8. 支持[消息对象](https://logging.apache.org/log4j/2.x/manual/messages.html)。消息允许通过日志系统传递有趣和复杂的结构并进行有效操作。 Users are free to create their own [Message](https://logging.apache.org/log4j/2.x/log4j-api/apidocs/org/apache/logging/log4j/message/Message.html) types and write custom [Layouts](https://logging.apache.org/log4j/2.x/manual/layouts.html), [Filters](https://logging.apache.org/log4j/2.x/manual/filters.html) and [Lookups](https://logging.apache.org/log4j/2.x/manual/lookups.html) to manipulate them.
9. Log4j 1.x supports Filters on Appenders. Logback added TurboFilters to allow filtering of events before they are processed by a Logger. Log4j 2 supports Filters that can be configured to process events before they are handled by a Logger, as they are processed by a Logger or on an Appender.
10. 大多数 Log4j 2 Appender 接受 Layout，允许以任何所需的格式传输数据。
11. Log4j 1.x 和 Logback 中的布局返回一个字符串。这导致了 Logback Encoders 中讨论的问题。 Log4j 2 采用更简单的方法，即 Layouts 总是返回一个字节数组。这样做的好处是，意味着它们几乎可以在任何 Appender 中使用，而不仅仅是写入 OutputStream Appender。
12. Syslog Appender 支持 TCP 和 UDP，并支持 BSD syslog 和 RFC 5424 格式。
13. Log4j 2 利用 Java 5 并发支持并在可能的最低级别执行锁定
14. 它是一个 Apache 软件基金会项目，遵循所有 ASF 项目使用的社区和支持模型。如果您想贡献或获得提交更改的权利，只需遵循[贡献](http://jakarta.apache.org/site/contributing.html)中概述的路径即可。




# 架构

![image-20210817202907031](..\..\images\log4j2-architecture.png)



1. 使用 Log4j 2 API 的应用程序将从 LogManager 请求具有特定名称的 Logger。 
2. LogManager 将定位适当的 LoggerContext，然后从中获取 Logger。
3. 如果必须创建 Logger，它将与包含 a) 与 Logger 相同的名称，b) 父包的名称或 c) 根 LoggerConfig 的 LoggerConfig 相关联。 
4. LoggerConfig 对象是从配置中的 Logger 声明创建的。 
5. LoggerConfig 与实际交付 LogEvents 的 Appender 相关联。



## Logger Hierarchy

在 Log4j 1.x 中，Logger Hierarchy 是通过 Logger 之间的关系维护的。在 Log4j 2 中，这种关系不再存在。

相反，层次结构在 LoggerConfig 对象之间的关系中维护。

Loggers 和 LoggerConfigs 是命名实体。 Logger 名称区分大小写，并遵循分层命名规则：



**Named Hierarchy**

1. 通过 点分命名

2. com.foo 是 com.foo.Bar 的祖先

3. 根Config位于 LoggerConfig层级结构的顶部，它的特殊之处在于它始终存在并且它是每个层次结构的一部分

4. 直接 获取 根LoggerConfig的Logger可以通过如下方式获取 

   ```java
   Logger logger = LogManager.getLogger(LogManager.ROOT_LOGGER_NAME);
   //或者
   Logger logger = LogManager.getRootLogger();
   ```

   

## LoggerContext

LoggerContext 作为 Logging 系统的锚点。但是，根据情况，应用程序中可能有多个活动的 LoggerContexts。

有关 LoggerContext 的更多详细信息，请参见日志分离部分





## Configuration

每个 LoggerContext 都有一个活动配置。

配置包含所有 Appender、上下文范围的过滤器、LoggerConfig 并包含对 StrSubstitutor 的引用。

在重新配置期间，将存在两个 Configuration 对象。一旦所有记录器都被重定向到新配置，旧配置将被停止并丢弃。



## Logger

如前所述，Logger 是通过调用 LogManager.getLogger 创建的。 

Logger 本身不执行任何直接操作。

它只有一个名称并与 LoggerConfig 相关联。

它扩展了 AbstractLogger 并实现了所需的方法。

随着配置被修改，Logger 可能会与不同的 LoggerConfig 相关联，从而导致它们的行为被修改。

## LoggerConfig

1. LoggerConfig 对象是在日志配置中声明 Logger 时创建的。 LoggerConfig 包含一组过滤器，必须允许 LogEvent 在它被传递给任何 Appender 之前通过。它包含对应该用于处理事件的 Appender 集的引用。
2. oggerConfigs 将被分配一个日志级别。内置级别集包括 TRACE、DEBUG、INFO、WARN、ERROR 和 FATAL。 Log4j 2 还支持自定义日志级别。
3. 另一种获得更多粒度的机制是改用标记。 Log4j 1.x 和 Logback 都有“级别继承”的概念。在 Log4j 2 中，Loggers 和 LoggerConfigs 是两个不同的对象，因此这个概念的实现方式不同。每个 Logger 引用适当的 LoggerConfig，后者又可以引用其父级，从而达到相同的效果


**日志级别默认支持自动级别过滤**

横坐标是日志事件的级别、纵坐标是日志配置的级别

| Event Level | LoggerConfig Level |       |      |      |       |       |      |
| :---------- | :----------------- | :---- | :--- | :--- | :---- | :---- | :--- |
|             | TRACE              | DEBUG | INFO | WARN | ERROR | FATAL | OFF  |
| ALL         | YES                | YES   | YES  | YES  | YES   | YES   | NO   |
| TRACE       | YES                | NO    | NO   | NO   | NO    | NO    | NO   |
| DEBUG       | YES                | YES   | NO   | NO   | NO    | NO    | NO   |
| INFO        | YES                | YES   | YES  | NO   | NO    | NO    | NO   |
| WARN        | YES                | YES   | YES  | YES  | NO    | NO    | NO   |
| ERROR       | YES                | YES   | YES  | YES  | YES   | NO    | NO   |
| FATAL       | YES                | YES   | YES  | YES  | YES   | YES   | NO   |
| OFF         | NO                 | NO    | NO   | NO   | NO    | NO    | NO   |

## Filter

除了上一节中描述的自动日志级别过滤之外，Log4j 还提供了过滤器，

1. 这些过滤器可以在控制传递给任何 LoggerConfig 之前，
2. 在控制传递给 LoggerConfig 之后但在调用任何 Appenders 之前
3. 在控制传递之后应用到 LoggerConfig 但在调用特定 Appender 之前，
4. 以及在每个 Appender 上。

以与防火墙过滤器非常相似的方式，每个过滤器可以返回三个结果之一，接受、拒绝或中立。 

Accept, Deny or Neutral

* Accept 的响应意味着不应调用其他过滤器并且事件应该进行。

* 拒绝响应意味着应立即忽略该事件并将控制权返回给调用者。 

* Neutral 响应表示该事件应传递给其他过滤器。

如果没有其他过滤器，则将处理该事件。

## Appender

目前，存在用于控制台、文件、远程套接字服务器、Apache Flume、JMS、远程 UNIX Syslog 守护进程和各种数据库 API 的附加程序。

有关可用的各种类型的更多详细信息，请参阅 Appenders 部分。一个 Logger 可以附加多个 Appender。可以通过调用当前 Configuration 的 addLoggerAppender 方法将 Appender 添加到 Logger。如果与 Logger 名称匹配的 LoggerConfig 不存在，则将创建一个，将 Appender 附加到它，然后将通知所有 Loggers 更新它们的 LoggerConfig 引用。



**appender可加性原则**

给定记录器的每个启用的日志记录请求都将转发到该 Logger 的 LoggerConfig 中的所有 appender 以及 LoggerConfig 父级的 Appender。

换句话说，Appender 从 LoggerConfig 层次结构中附加地继承。

例如，如果将控制台 appender 添加到根记录器，则所有启用的日志记录请求至少会在控制台上打印。

如果另外将文件附加程序添加到 LoggerConfig，例如 C，则为 C 和 C 的子项启用的日志记录请求将打印在文件和控制台上。

可以通过在配置文件中的 Logger 声明中设置 additivity="false" 来覆盖此默认行为，以便 Appender 累积不再是可加的。

下面总结了管理 appender 可加性的规则。

 **L 的一条日志语句的输出将转到与 L 关联的 LoggerConfig 中的所有 Appender 以及该 LoggerConfig 的祖先。**

这就是术语“appender 可加性”的含义。

但是，如果与 Logger L 关联的 LoggerConfig 的祖先，例如 P，将可加性标志设置为 false，那么 L 的输出将被定向到 L 的 LoggerConfig 中的所有 appender，并且它的祖先一直到并包括 P，但不包括在 

The table below shows an example:

| Logger Name     | Added Appenders | Additivity Flag | Output Targets         | Comment                                                      |
| :-------------- | :-------------- | :-------------- | :--------------------- | :----------------------------------------------------------- |
| root            | A1              | not applicable  | A1                     | The root logger has no parent so additivity does not apply to it. |
| x               | A-x1, A-x2      | true            | A1, A-x1, A-x2         | Appenders of "x" and root.                                   |
| x.y             | none            | true            | A1, A-x1, A-x2         | Appenders of "x" and root. It would not be typical to configure a Logger with no Appenders. |
| x.y.z           | A-xyz1          | true            | A1, A-x1, A-x2, A-xyz1 | Appenders in "x.y.z", "x" and root.                          |
| security        | A-sec           | false           | A-sec                  | No appender accumulation since the additivity flag is set to false. |
| security.access | none            | true            | A-sec                  | Only appenders of "security" because the additivity flag in "security" is set to false. |



## Layout

通常情况下，用户不仅希望自定义输出目的地，还希望自定义输出格式。

这是通过将 Layout 与 Appender 相关联来实现的。 

Layout 负责根据用户的意愿格式化 LogEvent，而 appender 负责将格式化的输出发送到其目的地。 

PatternLayout 是标准 log4j 发行版的一部分，它允许用户根据类似于 C 语言 printf 函数的转换模式来指定输出格式。

例如，具有转换模式“%r [%t] %-5p %c - %m%n”的 PatternLayout 将输出类似于：

```
176 [main] INFO  org.foo.Bar - Located nearest gas station.
```

第一个字段是自程序启动以来经过的毫秒数。

第二个字段是发出日志请求的线程。

第三个字段是日志语句的级别。

第四个字段是与日志请求关联的记录器的名称。 

“-”后面的文字是语句的信息。

Log4j 为各种用例提供了许多不同的布局，例如 JSON、XML、HTML 和 Syslog（包括新的 RFC 5424 版本）。

其他附加程序（例如数据库连接器）填充指定的字段而不是特定的文本布局。

同样重要的是，log4j 将根据用户指定的标准呈现日志消息的内容。

例如，如果您经常需要记录当前项目中使用的对象类型 Oranges，那么您可以创建一个接受 Orange 实例的 OrangeMessage 并将其传递给 Log4j，以便在以下情况下可以将 Orange 对象格式化为适当的字节数组

## StrSubstitutor and StrLookup

StrSubstitutor 类和 StrLookup 接口是从 Apache Commons Lang 借来的，然后经过修改以支持评估 LogEvents。

此外，Interpolator 类是从 Apache Commons Configuration 借来的，以允许 StrSubstitutor 评估来自多个 StrLookups 的变量。

它也经过修改以支持评估 LogEvents。

这些共同提供了一种机制，允许配置引用来自系统属性、配置文件、ThreadContext Map、LogEvent 中的 StructuredData 的变量。

如果组件能够处理它，则可以在处理配置时或在处理每个事件时解析变量。

有关详细信息，请参阅查找。
