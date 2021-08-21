# 配置方式

Log4j 2 的配置可以通过以下 4 种方式中的一种来完成： 

1. 通过以 XML、JSON、YAML 或属性格式编写的配置文件。

2. 以编程方式，通过创建 ConfigurationFactory 和 Configuration 实现。

3. 以编程方式，通过调用 Configuration 接口中公开的 API 将组件添加到默认配置中。

4. 以编程方式，通过调用内部 Logger 类上的方法。





# Automatic Configuration

Log4j 具有在初始化期间自动配置自身的能力。

当 Log4j 启动时，它会定位所有 ConfigurationFactory 插件，并按从高到低的加权顺序排列它们。

交付时，Log4j 包含四种 ConfigurationFactory 实现：

1. 一种用于 JSON，
2. 一种用于 YAML，
3. 一种用于 properties，
4. 一种用于 XML。

1. Log4j 将检查“log4j2.configurationFile”系统属性，如果设置，将尝试使用与文件扩展名匹配的 ConfigurationFactory 加载配置。请注意，这不限于本地文件系统上的某个位置，并且可能包含一个 URL。
2. 如果未设置系统属性，则Properties ConfigurationFactory 将在类路径中查找 log4j2-test.properties。
3. 如果未找到此类文件，YAML ConfigurationFactory 将在类路径中查找 log4j2-test.yaml 或 log4j2-test.yml。
4. 如果没有找到这样的文件，JSON ConfigurationFactory 将在类路径中查找 log4j2-test.json 或 log4j2-test.jsn。
5. 如果没有找到这样的文件，XML ConfigurationFactory 将在类路径中查找 log4j2-test.xml。
6. 如果无法找到测试文件，则属性 ConfigurationFactory 将在类路径上查找 log4j2.properties。
7. 如果无法找到属性文件，YAML ConfigurationFactory 将在类路径上查找 log4j2.yaml 或 log4j2.yml。
8. 如果找不到 YAML 文件，JSON ConfigurationFactory 将在类路径上查找 log4j2.json 或 log4j2.jsn。
9. 如果找不到 JSON 文件，XML ConfigurationFactory 将尝试在类路径上定位 log4j2.xml。
10. 如果找不到配置文件，则将使用 DefaultConfiguration。这将导致日志输出进入控制台。


DefaultConfiguration 类中提供的默认配置将设置：

1. A [ConsoleAppender](http://logging.apache.org/log4j/2.x/log4j-core/apidocs/org/apache/logging/log4j/core/appender/ConsoleAppender.html) attached to the root logger.
2. A [PatternLayout](http://logging.apache.org/log4j/2.x/log4j-core/apidocs/org/apache/logging/log4j/core/layout/PatternLayout.html) set to the pattern "%d{HH:mm:ss.SSS} [%t] %-5level %logger{36} - %msg%n" attached to the ConsoleAppender

默认情况下 Log4j 将根记录器分配给 Level.ERROR。

与默认值等效的配置如下所示：

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="WARN">
  <Appenders>
    <Console name="Console" target="SYSTEM_OUT">
      <PatternLayout pattern="%d{HH:mm:ss.SSS} [%t] %-5level %logger{36} - %msg%n"/>
    </Console>
  </Appenders>
  <Loggers>
    <Root level="error">
      <AppenderRef ref="Console"/>
    </Root>
  </Loggers>
</Configuration>
```



# Additivity

# Automatic Reconfiguration

从文件配置时，Log4j 能够自动检测对配置文件的更改并重新配置自身。如果在配置元素上指定了 monitorInterval 属性并将其设置为非零值，则在下次评估和/或记录日志事件时将检查该文件，并且自上次检查以来已经过去了 monitorInterval。下面的示例显示了如何配置属性，以便仅在至少 30 秒过去后才检查配置文件的更改。**最小间隔为 5 秒。**

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration monitorInterval="30">
...
</Configuration>
```

## Advertising appender 配置

[Chainsaw](http://logging.apache.org/chainsaw/index.html) 可以自动处理您的日志文件（Advertising appender 配置）

Log4j 提供了为所有基于文件的appenders以及基于套接字的appenders  “通告”appenders配置细节的能力。

例如，对于基于文件的 appender，文件中的文件位置和模式布局都包含在通告中

Chainsaw 和其他外部系统可以发现这些通告并使用该信息智能地处理日志文件。



展示通告的机制以及通告格式特定于每个通告商实现。

想要使用特定通告商实现的外部系统必须了解如何定位通告配置以及通告格式。

例如，“数据库”广告商可以将配置细节存储在数据库表中。

外部系统可以读取该数据库表以发现文件位置和文件格式。

Log4j 提供了一个 Advertiser 实现，即“multicastdns”Advertiser，它使用 http://jmdns.sourceforge.net 库通过 IP 多播来通告 appender 配置详细信息。



Chainsaw 自动发现 log4j 的多播dns 生成的通告

并在 Chainsaw 的 Zeroconf 选项卡中显示那些发现的通告（如果 jmdns 库在 Chainsaw 的类路径中）

要开始解析和跟踪通告中提供的日志文件，只需双击 Chainsaw 的 Zeroconf 选项卡中的通告条目。

目前，Chainsaw 仅支持 FileAppender通告。

要通告 appender 配置：

- Add the JmDns library from [http://jmdns.sourceforge.net](http://jmdns.sourceforge.net/) to the application classpath
- Set the 'advertiser' attribute of the configuration element to 'multicastdns'
- Set the 'advertise' attribute on the appender element to 'true'
- If advertising a FileAppender-based configuration, set the 'advertiseURI' attribute on the appender element to an appropriate URI

基于 FileAppender 的配置需要在 appender 上指定一个额外的“advertiseURI”属性。 

'advertiseURI' 属性为 Chainsaw 提供有关如何访问文件的信息。

例如，通过指定 Commons VFS (http://commons.apache.org/proper/commons-vfs/) sftp:// URI，一个 http:// URI，Chainsaw 可以通过 ssh/sftp 远程访问该文件

如果文件可通过 Web 服务器访问，则可以使用；如果从本地运行的 Chainsaw 实例访问文件，则可以指定 file:// URI。

这是一个启用通告的 appender 配置示例，本地运行的 Chainsaw 可以使用它来自动跟踪日志文件（注意 file://advertiseURI）：



请注意，您必须将来自 http://jmdns.sourceforge.net 的 JmDns 库添加到您的应用程序类路径中，以便通过“multicastdns”通告商进行通告。

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration advertiser="multicastdns">
...
</Configuration>
<Appenders>
  <File name="File1" fileName="output.log" bufferedIO="false" advertiseURI="file://path/to/output.log" advertise="true">
  ...
  </File>
</Appenders>
```



# Configuration Syntax

从 version 2.9 开始，出于安全原因，Log4j 不会处理 XML 文件中的 DTD。

如果要将配置拆分为多个文件，请使用 XInclude 或 Composite Configuration。

Log4j 允许您轻松地重新定义日志记录行为，而无需修改您的应用程序

1. 可以禁用应用程序的某些部分的日志记录，

2. 仅在满足特定条件时才记录日志，
3. 例如为特定用户执行的操作、路由输出到 Flume 或日志报告系统等。

能够做到这一点需要理解配置文件的语法。 

## **Configuration接受几个属性：**

| Attribute Name  | Description                                                  |
| :-------------- | :----------------------------------------------------------- |
| advertiser      | 将用于通过告单个 FileAppender 或 SocketAppender 配置的通过商插件名称<br />提供的唯一广告商插件是“multicastdns”。 |
| dest            | Either "err" for stderr, "out" for stdout, a file path, or a URL. |
| monitorInterval | 在检查文件配置更改之前必须经过的最短时间（以秒为单位）       |
| name            | 配置的名称。                                                 |
| packages        | 用于搜索插件的以逗号分隔的软件包名称列表。<br/><br/>每个类加载器只加载一次插件，因此更改此值可能不会对重新配置产生任何影响。 |
| schema          | 标识用于定位用于验证配置的 XML 架构的类加载器的位置。<br/><br/>仅在strict 设置为true 时有效。<br/><br/>如果未设置，则不会进行架构验证。 |
| shutdownHook    | 指定当 JVM 关闭时 Log4j 是否应该自动关闭。<br/><br/>关闭挂钩默认启用，但可以通过将此属性设置为“禁用”来禁用 |
| shutdownTimeout | Specifies how many milliseconds appenders and background tasks will get to shutdown when the JVM shuts down. Default is zero which mean that each appender uses its default timeout, and don't wait for background tasks. Not all appenders will honor this, it is a hint and not an absolute guarantee that the shutdown procedure will not take longer. Setting this too low increase the risk of losing outstanding log events not yet written to the final destination. See [LoggerContext.stop(long, java.util.concurrent.TimeUnit)](http://logging.apache.org/log4j/2.x/log4j-core/target/site/apidocs/org/apache/logging/log4j/core/LoggerContext.html#stoplong_java.util.concurrent.TimeUnit). (Not used if shutdownHook is set to "disable".) |
| status          | 应该记录到控制台的**内部 Log4j 事件**的级别。<br/><br/>此属性的有效值为“trace”、“debug”、“info”、“warn”、“error”和“fatal”。 <br/><br/>Log4j 会将有关初始化、翻转和其他内部操作的详细信息记录到状态记录器中。<br/><br/>如果您需要对 log4j 进行故障排除，设置 status="trace" 是您可以使用的首批工具之一。（或者，设置系统属性 log4j2.debug 也会将内部 Log4j2 日志记录打印到控制台，包括在配置之前发生的内部日志记录<br/><br/>找到了文件。） |
| strict          | 允许使用严格的 XML 格式。 <br/><br/>JSON 配置不支持。        |
| verbose         | 在加载插件时启用诊断信息。                                   |



# Configuration with XML

Log4j 可以使用两种 XML 风格进行配置；

concise and strict.

简洁的格式使配置变得非常容易，因为元素名称与它们所代表的组件相匹配，但无法使用 XML 模式进行验证。

例如，ConsoleAppender 是通过在其父 appenders 元素下声明一个名为 Console 的 XML 元素来配置的

此外，属性可以指定为 XML 属性，也可以指定为没有属性但具有文本值的 XML 元素。

```xml
<PatternLayout pattern="%m%n"/>
与
<PatternLayout>
  <Pattern>%m%n</Pattern>
</PatternLayout>
```

```xml
<?xml version="1.0" encoding="UTF-8"?>;
<Configuration>
  <Properties>
    <Property name="name1">value</property>
    <Property name="name2" value="value2"/>
  </Properties>
  <filter  ... />
  <Appenders>
    <appender ... >
      <filter  ... />
    </appender>
    ...
  </Appenders>
  <Loggers>
    <Logger name="name1">
      <filter  ... />
    </Logger>
    ...
    <Root level="level">
      <AppenderRef ref="name"/>
    </Root>
  </Loggers>
</Configuration>
```

```xml
<?xml version="1.0" encoding="UTF-8"?>;
<Configuration>
  <Properties>
    <Property name="name1">value</property>
    <Property name="name2" value="value2"/>
  </Properties>
  <Filter type="type" ... />
  <Appenders>
    <Appender type="type" name="name">
      <Filter type="type" ... />
    </Appender>
    ...
  </Appenders>
  <Loggers>
    <Logger name="name1">
      <Filter type="type" ... />
    </Logger>
    ...
    <Root level="level">
      <AppenderRef ref="name"/>
    </Root>
  </Loggers>
</Configuration>

与
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="debug" strict="true" name="XMLConfigTest"
               packages="org.apache.logging.log4j.test">
  <Properties>
    <Property name="filename">target/test.log</Property>
  </Properties>
  <Filter type="ThresholdFilter" level="trace"/>
 
  <Appenders>
    <Appender type="Console" name="STDOUT">
      <Layout type="PatternLayout" pattern="%m MDC%X%n"/>
      <Filters>
        <Filter type="MarkerFilter" marker="FLOW" onMatch="DENY" onMismatch="NEUTRAL"/>
        <Filter type="MarkerFilter" marker="EXCEPTION" onMatch="DENY" onMismatch="ACCEPT"/>
      </Filters>
    </Appender>
    <Appender type="Console" name="FLOW">
      <Layout type="PatternLayout" pattern="%C{1}.%M %m %ex%n"/><!-- class and line number -->
      <Filters>
        <Filter type="MarkerFilter" marker="FLOW" onMatch="ACCEPT" onMismatch="NEUTRAL"/>
        <Filter type="MarkerFilter" marker="EXCEPTION" onMatch="ACCEPT" onMismatch="DENY"/>
      </Filters>
    </Appender>
    <Appender type="File" name="File" fileName="${filename}">
      <Layout type="PatternLayout">
        <Pattern>%d %p %C{1.} [%t] %m%n</Pattern>
      </Layout>
    </Appender>
  </Appenders>
 
  <Loggers>
    <Logger name="org.apache.logging.log4j.test1" level="debug" additivity="false">
      <Filter type="ThreadContextMapFilter">
        <KeyValuePair key="test" value="123"/>
      </Filter>
      <AppenderRef ref="STDOUT"/>
    </Logger>
 
    <Logger name="org.apache.logging.log4j.test2" level="debug" additivity="false">
      <AppenderRef ref="File"/>
    </Logger>
 
    <Root level="trace">
      <AppenderRef ref="STDOUT"/>
    </Root>
  </Loggers>
 
</Configuration>
```

# Configuring Loggers

**配置*LoggConfig***

LoggerConfig 是使用 logger 元素配置的。 

logger 元素必须指定一个 name 属性，通常指定一个 level 属性，也可能指定一个 additivity 属性。

该级别可以配置为 TRACE、DEBUG、INFO、WARN、ERROR、ALL 或 OFF 之一。

如果未指定级别，则默认为 ERROR。



**捕获位置信息**

捕获位置信息（类名、文件名、方法名和调用者的行号）可能很慢。 

Log4j 试图通过减少必须遍历以找到日志记录方法的调用者的堆栈的大小来优化这一点。

它通过确定可能被访问的任何组件是否需要位置信息来实现这一点。

如果在跟踪或调试等级别配置记录器并期望大多数日志将在 Appender 引用或 Appender 上过滤，则这可能会导致性能问题，因为即使日志事件将被丢弃，Log4j 也会计算位置信息。

要禁用此行为，可以在 LoggerConfig 上将 includeLocation 属性设置为 false。

这将导致 Log4j 推迟计算位置信息，直到绝对必要。



**属性替换**

LoggerConfig（包括根 LoggerConfig）可以配置属性，这些属性将添加到从 ThreadContextMap 复制的属性中。

这些属性可以从 Appender、过滤器、布局等中引用，就像它们是 ThreadContext Map 的一部分一样。

属性可以包含在解析配置时或在记录每个事件时动态解析的变量。

有关使用变量的更多信息，请参阅属性替换。



**配置多个Appender**

1. LoggerConfig 也可以配置一个或多个 AppenderRef 元素。

2. 引用的每个 appender 都将与指定的 LoggerConfig 相关联。

3. 如果在 LoggerConfig 上配置了多个 appender，则在处理日志事件时会调用它们中的每一个。



**默认存在根配置**

每个配置都必须有一个根记录器。

如果未配置，则将使用默认根 LoggerConfig，其级别为 ERROR 并附加了 Console appender。

根记录器和其他记录器之间的主要区别是

1. The root logger does not have a name attribute.
2. The root logger does not support the additivity attribute since it has no parent.



# Configuring Appenders

使用特定的 appender 插件的名称或使用 appender 元素和包含 appender 插件名称的 type 属性来配置 appender。

此外，每个 appender 必须有一个 name 属性，指定一个值，该值在 appender 集合中是唯一的。

记录器将使用该名称来引用前一节中所述的附加程序。

大多数 appender 还支持要配置的布局（同样可以使用特定的 Layout 插件的名称作为元素或使用“layout”作为元素名称以及包含布局插件名称的 type 属性来指定。各种 appender 将

包含它们正常运行所需的其他属性或元素。





# Configuring Filters

Log4j 允许在 4 个位置中的任何一个指定过滤器

1. 与 appender、loggers 和 properties 元素处于同一级别。这些过滤器可以在事件被传递到 LoggerConfig 之前接受或拒绝事件。
2. 在logger 元素中。这些过滤器可以接受或拒绝特定loggers的事件。
3. 在 appender 元素中。这些过滤器可以阻止或导致事件被附加程序处理。
4. 在 appender 引用元素中。这些过滤器用于确定 Logger 是否应该将事件路由到 appender。

尽管只能配置单个过滤器元素，但该元素可能是表示 CompositeFilter 的过滤器元素。

过滤器元素允许在其中配置任意数量的过滤器元素。

以下示例显示了如何在 ConsoleAppender 上配置多个过滤器。



```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="debug" name="XMLConfigTest" packages="org.apache.logging.log4j.test">
  <Properties>
    <Property name="filename">target/test.log</Property>
  </Properties>
  <ThresholdFilter level="trace"/>
 
  <Appenders>
    <Console name="STDOUT">
      <PatternLayout pattern="%m MDC%X%n"/>
    </Console>
    <Console name="FLOW">
      <!-- this pattern outputs class name and line number -->
      <PatternLayout pattern="%C{1}.%M %m %ex%n"/>
      <filters>
        <MarkerFilter marker="FLOW" onMatch="ACCEPT" onMismatch="NEUTRAL"/>
        <MarkerFilter marker="EXCEPTION" onMatch="ACCEPT" onMismatch="DENY"/>
      </filters>
    </Console>
    <File name="File" fileName="${filename}">
      <PatternLayout>
        <pattern>%d %p %C{1.} [%t] %m%n</pattern>
      </PatternLayout>
    </File>
  </Appenders>
 
  <Loggers>
    <Logger name="org.apache.logging.log4j.test1" level="debug" additivity="false">
      <ThreadContextMapFilter>
        <KeyValuePair key="test" value="123"/>
      </ThreadContextMapFilter>
      <AppenderRef ref="STDOUT"/>
    </Logger>
 
    <Logger name="org.apache.logging.log4j.test2" level="debug" additivity="false">
      <Property name="user">${sys:user.name}</Property>
      <AppenderRef ref="File">
        <ThreadContextMapFilter>
          <KeyValuePair key="test" value="123"/>
        </ThreadContextMapFilter>
      </AppenderRef>
      <AppenderRef ref="STDOUT" level="error"/>
    </Logger>
 
    <Root level="trace">
      <AppenderRef ref="STDOUT"/>
    </Root>
  </Loggers>
 
</Configuration>
```





# Property Substitution

Log4j 2 支持在配置中指定 tokens  作为对其他地方定义的属性的引用的能力。



其中一些属性将在解释配置文件时解析，而其他属性可能会传递给组件在运行时解析。

为了实现这一点，Log4j 使用了 Apache Commons Lang 的 StrSubstitutor 和 StrLookup 类的变体

以类似于 Ant 或 Maven 的方式，这允许使用配置本身中声明的属性解析声明为 ${name} 的变量。



例如，以下示例显示了被声明为属性的滚动文件附加程序的文件名。

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="debug" name="RoutingTest" packages="org.apache.logging.log4j.test">
  <Properties>
    <Property name="filename">target/rolling1/rollingtest-$${sd:type}.log</Property>
  </Properties>
  <ThresholdFilter level="debug"/>
 
  <Appenders>
    <Console name="STDOUT">
      <PatternLayout pattern="%m%n"/>
      <ThresholdFilter level="debug"/>
    </Console>
    <Routing name="Routing">
      <Routes pattern="$${sd:type}">
        <Route>
          <RollingFile name="Rolling-${sd:type}" fileName="${filename}"
                       filePattern="target/rolling1/test1-${sd:type}.%i.log.gz">
            <PatternLayout>
              <pattern>%d %p %c{1.} [%t] %m%n</pattern>
            </PatternLayout>
            <SizeBasedTriggeringPolicy size="500" />
          </RollingFile>
        </Route>
        <Route ref="STDOUT" key="Audit"/>
      </Routes>
    </Routing>
  </Appenders>
 
  <Loggers>
    <Logger name="EventLogger" level="info" additivity="false">
      <AppenderRef ref="Routing"/>
    </Logger>
 
    <Root level="error">
      <AppenderRef ref="STDOUT"/>
    </Root>
  </Loggers>
 
</Configuration>
```

虽然这很有用，但还有更多的地方可以创建属性。

为了适应这一点，Log4j 还支持语法 ${prefix:name} ，其中前缀标识告诉 Log4j 应该在特定上下文中评估变量名称。

Log4j 内置的上下文是：

| Prefix     | Context                                                      |
| :--------- | :----------------------------------------------------------- |
| base64     | Base64 encoded data. The format is ${base64:Base64_encoded_data}. For example: ${base64:SGVsbG8gV29ybGQhCg==} yields Hello World!. |
| bundle     | Resource bundle. The format is ${bundle:BundleName:BundleKey}. The bundle name follows package naming conventions, for example: ${bundle:com.domain.Messages:MyKey}. |
| ctx        | Thread Context Map (MDC)                                     |
| date       | 使用指定格式插入当前日期和/或时间                            |
| env        | System environment variables. The formats are ${env:ENV_NAME} and ${env:ENV_NAME:-default_value}. |
| jndi       | A value set in the default JNDI Context.                     |
| jvmrunargs | A JVM input argument accessed through JMX, but not a main argument; see [RuntimeMXBean.getInputArguments()](http://docs.oracle.com/javase/6/docs/api/java/lang/management/RuntimeMXBean.html#getInputArguments--). Not available on Android. |
| log4j      | Log4j configuration properties. The expressions ${log4j:configLocation} and ${log4j:configParentLocation} respectively provide the absolute path to the log4j configuration file and its parent folder. |
| main       | A value set with [MapLookup.setMainArguments(String[\])](http://logging.apache.org/log4j/2.x/log4j-core/apidocs/org/apache/logging/log4j/core/lookup/MapLookup.html#setMainArguments-java.lang.String:A-) |
| map        | A value from a MapMessage                                    |
| sd         | 来自 StructuredDataMessage 的值. The key "id" will return the name of the StructuredDataId without the enterprise number. The key "type" will return the message type. Other keys will retrieve individual elements from the Map. |
| sys        | System properties. The formats are ${sys:some.property} and ${sys:some.property:-default_value}. |

# Default Properites

通过将 Properties 元素直接放在 Configuration 元素之后和任何 Loggers、Filters、Appenders 等声明之前，可以在配置文件中声明默认属性映射。

如果在指定的查找中无法找到该值，则将使用默认属性映射中的值。

默认映射预先填充了“hostName”的值，它是当前系统的主机名或 IP 地址，“contextName”是当前日志记录上下文的值。

也可以使用语法 ${lookupName:\key:-defaultValue} 在 Lookup 中指定默认属性。

在某些情况下，键可能包含前导“-”。

在这种情况下，必须包含转义字符，例如 ${main:\--file:-app.properties}。



# Disables Message Pattern Lookups

消息由查找处理（默认情况下），例如，如果您定义了 

```
<Property name="foo.bar">FOO_BAR </Property>
```

则 logger.info("${foo.bar}") 将输出 FOO_BAR 

${foo.bar} 的。

您可以通过将系统属性 log4j2.formatMsgNoLookups 设置为 true 或使用 %m{nolookups} 定义消息模式来全局禁用消息模式查找。



# Lookup Variables with Multiple Leading '$' Characters

StrLookup 处理的一个有趣特性是，当变量引用在每次解析变量时使用多个前导 '$' 字符声明时，前导 '$' 会被简单地删除。

在前面的示例中，“Routes”元素能够在运行时解析变量。

为此，将前缀值指定为带有两个前导 '$' 字符的变量。

当第一次处理配置文件时，第一个“$”字符被简单地删除。

因此，当 Routes 元素在运行时被评估时，它是变量声明“${sd:type}”，它导致事件被检查 StructuredDataMessage，如果存在，则其类型属性的值将用作路由钥匙。

并非所有元素都支持在运行时解析变量。执行此操作的组件将在其文档中明确指出这一点。



如果在与前缀关联的 Lookup 中找不到键的值，则将使用与配置文件中的属性声明中的键关联的值。

如果没有找到值，变量声明将作为值返回。

可以通过执行以下操作在配置中声明默认值：

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration>
  <Properties>
    <Property name="type">Audit</property>
  </Properties>
  ...
</Configuration>
```

值得指出的是，在处理配置时也不会评估 RollingFile appender 声明中的变量。

这仅仅是因为整个 RollingFile 元素的解析被推迟到匹配发生。



