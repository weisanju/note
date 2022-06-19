# Lookups

**Lookups**  提供了一种在任意位置向 Log4j 配置添加值的方法。它们是实现 StrLookup 接口的特殊类型的插件

关于如何在配置文件中使用查找的信息可以在  [configuration](https://logging.apache.org/log4j/2.x/manual/configuration.html) 页面的  [Property Substitution](https://logging.apache.org/log4j/2.x/manual/configuration.html#PropertySubstitution) 部分找到。



# Context Map Lookup

ContextMapLookup 允许应用程序将数据存储在 Log4j ThreadContext Map 中，然后在Log4j 配置 检索值。

在下面的示例中，应用程序将使用键“loginId”将当前用户的登录 ID 存储在 ThreadContext Map 中。



在初始配置处理期间，第一个“$”将被删除。 

PatternLayout 支持使用 Lookups 进行插值，然后将为每个事件解析变量。

请注意，模式 "%X{loginId}" 将获得相同的结果。

```xml
<File name="Application" fileName="application.log">
  <PatternLayout>
    <pattern>%d %p %c{1.} [%t] $${ctx:loginId} %m%n</pattern>
  </PatternLayout>
</File>
```



# Date Lookup

DateLookup 与其他查找有些不同，因为它不使用键来定位项目。

相反，该键可用于指定对 SimpleDateFormat 有效的日期格式字符串。

当前日期或与当前日志事件关联的日期将按照指定的格式进行格式化。

```xml
<RollingFile name="Rolling-${map:type}" fileName="${filename}" filePattern="target/rolling1/test1-$${date:MM-dd-yyyy}.%i.log.gz">
  <PatternLayout>
    <pattern>%d %p %c{1.} [%t] %m%n</pattern>
  </PatternLayout>
  <SizeBasedTriggeringPolicy size="500" />
</RollingFile>
```

# Docker Lookup

DockerLookup 可用于从运行应用程序的 Docker 容器中查找属性。

Log4j Docker provides access to the following container attributes:

|                  |                                              |
| ---------------- | -------------------------------------------- |
| containerId      | The full id assigned to the container.       |
| containerName    | The name assigned to the container.          |
| imageId          | The id assigned to the image.                |
| imageName        | The name assigned to the image.              |
| shortContainerId | The first 12 characters of the container id. |
| shortImageId     | The first 12 characters of the image id.     |

```xml
<JsonLayout properties="true" compact="true" eventEol="true">
  <KeyValuePair key="containerId" value="${docker:containerId}"/>
  <KeyValuePair key="containerName" value="${docker:containerName}"/>
  <KeyValuePair key="imageName" value="${docker:imageName}"/>
</JsonLayout>
```

This Lookup is subject to the requirements listed at [Log4j Docker Support](https://logging.apache.org/log4j/2.x/log4j-docker/index.html)

# Environment Lookup

EnvironmentLookup 允许系统在全局文件（如 /etc/profile）或应用程序的启动脚本中配置环境变量，然后从日志配置中检索这些变量。

```xml
<File name="Application" fileName="application.log">
  <PatternLayout>
    <pattern>%d %p %c{1.} [%t] $${env:USER} %m%n</pattern>
  </PatternLayout>
</File>
```

此查找还支持默认值语法。

```xml
<File name="Application" fileName="application.log">
  <PatternLayout>
    <pattern>%d %p %c{1.} [%t] $${env:USER:-jdoe} %m%n</pattern>
  </PatternLayout>
</File>
```



# EventLookup

EventLookup 提供对配置中  日志事件中 字段的访问。

| Key        | Description                                  |
| :--------- | :------------------------------------------- |
| Exception  | 如果事件中包含异常，则返回异常的简单类名。   |
| Level      | 返回事件的日志记录级别。                     |
| Logger     | 返回记录器的名称。                           |
| Marker     | 返回与日志事件关联的标记的名称（如果存在）。 |
| Message    | 返回格式化的消息字符串。                     |
| ThreadId   | 返回与日志事件关联的线程 ID。                |
| ThreadName | 返回与日志事件关联的线程的名称               |
| Timestamp  | 返回事件发生的时间（以毫秒为单位）。         |

在此示例中，RoutingAppender 根据日志事件中存在的名为“AUDIT”的标记的存在来选择路由。

```xml
          <?xml version="1.0" encoding="UTF-8"?>
          <Configuration status="WARN" name="RoutingTest">
            <Appenders>
              <Console name="STDOUT" target="SYSTEM_OUT" />
              <Flume name="AuditLogger" compress="true">
                <Agent host="192.168.10.101" port="8800"/>
                <Agent host="192.168.10.102" port="8800"/>
                <RFC5424Layout enterpriseNumber="18060" includeMDC="true" appName="MyApp"/>
              </Flume>
              <Routing name="Routing">
                <Routes>
                  <Route pattern="$${event:Marker}">
                    <RollingFile
                        name="Rolling-${mdc:UserId}"
                        fileName="${mdc:UserId}.log"
                        filePattern="${mdc:UserId}.%i.log.gz">
                      <PatternLayout>
                        <pattern>%d %p %c{1.} [%t] %m%n</pattern>
                      </PatternLayout>
                      <SizeBasedTriggeringPolicy size="500" />
                    </RollingFile>
                  </Route>
                  <Route ref="AuditLogger" key="AUDIT"/>
                  <Route ref="STDOUT" key="STDOUT"/>
                </Routes>
                <IdlePurgePolicy timeToLive="15" timeUnit="minutes"/>
              </Routing>
            </Appenders>
            <Loggers>
              <Root level="error">
                <AppenderRef ref="Routing" />
              </Root>
            </Loggers>
          </Configuration>
```

# Java Lookup

JavaLookup 允许使用 java: 前缀在方便的预格式化字符串中检索 Java 环境信息。

| Key     | Description                                                  |
| :------ | :----------------------------------------------------------- |
| version | The short Java version, like:Java version 1.7.0_67           |
| runtime | The Java runtime version, like:Java(TM) SE Runtime Environment (build 1.7.0_67-b01) from Oracle Corporation |
| vm      | The Java VM version, like:Java HotSpot(TM) 64-Bit Server VM (build 24.65-b04, mixed mode) |
| os      | The OS version, like:Windows 7 6.1 Service Pack 1, architecture: amd64-64 |
| locale  | Hardware information, like:default locale: en_US, platform encoding: Cp1252 |
| hw      | Hardware information, like:processors: 4, architecture: amd64-64, instruction sets: amd64 |

```java
<File name="Application" fileName="application.log">
  <PatternLayout header="${java:runtime} - ${java:vm} - ${java:os}">
    <Pattern>%d %m%n</Pattern>
  </PatternLayout>
</File>
```

# Jndi Lookup

JndiLookup 允许通过 JNDI 检索变量。

默认情况下，键将以 java:comp/env/ 为前缀，但是如果键包含“:”，则不会添加前缀。

```xml
<File name="Application" fileName="application.log">
  <PatternLayout>
    <pattern>%d %p %c{1.} [%t] $${jndi:logging/context-name} %m%n</pattern>
  </PatternLayout>
</File>
```

Java 的 JNDI 模块在 Android 上不可用。



# Log4j Configuration Location Lookup

Log4j 配置属性。

表达式 ${log4j:configLocation} 和 ${log4j:configParentLocation} 分别提供 log4j 配置文件及其父文件夹的绝对路径。

```xml
<File name="Application" fileName="${log4j:configParentLocation}/logs/application.log">
  <PatternLayout>
    <pattern>%d %p %c{1.} [%t] %m%n</pattern>
  </PatternLayout>
</File>
```

# Lower Lookup

LowerLookup 将传入的参数转换为小写。据推测，该值将是嵌套查找的结果。

```xml
<File name="Application" fileName="application.log">
  <PatternLayout>
    <pattern>%d %p %c{1.} [%t] $${lower:{${spring:spring.application.name}} %m%n</pattern>
  </PatternLayout>
</File>
```

### Upper Lookup

The UpperLookup converts the passed in argument to upper case. Presumably the value will be the result of a nested lookup.

```xml
<File name="Application" fileName="application.log">  <PatternLayout>    <pattern>%d %p %c{1.} [%t] $$upper{${spring:spring.application.name}} %m%n</pattern>  </PatternLayout></File>
```



# Main Arguments Lookup (Application)

此查找要求您手动向 Log4j 提供应用程序的主要参数：

```xml
import org.apache.logging.log4j.core.lookup.MainMapLookup;
 
public static void main(String args[]) {
  MainMapLookup.setMainArguments(args);
  ...
}
```

如果已设置主要参数，则此查找允许应用程序从日志记录配置中检索这些主要参数值。 

main: 前缀后面的键可以是参数列表中从 0 开始的索引，也可以是字符串，其中 ${main:myString} 替换为 main 参数列表中 myString 后面的值。



注意：许多应用程序使用前导破折号来标识命令参数。

指定 ${main:--file} 将导致查找失败，因为它会查找名为“main”的变量，默认值为“-file”。

为避免这种情况，将 Lookup 名称与键分开的“:”必须后跟反斜杠作为转义字符，如 ${main:\--file}

Then the following substitutions are possible:

```
--file foo.txt --verbose -x bar
```

| Expression             | Result    |
| :--------------------- | :-------- |
| ${main:0}              | --file    |
| ${main:1}              | foo.txt   |
| ${main:2}              | --verbose |
| ${main:3}              | -x        |
| ${main:4}              | bar       |
| ${main:\--file}        | foo.txt   |
| ${main:\-x}            | bar       |
| ${main:bar}            | null      |
| ${main:\--quiet:-true} | true      |

```xml
<File name="Application" fileName="application.log">
  <PatternLayout header="File: ${main:--file}">
    <Pattern>%d %m%n</Pattern>
  </PatternLayout>
</File>
```



# Map Lookup

MapLookup 有多种用途

1. 为配置文件中声明的属性 提供 存储。
2. 从 LogEvents 中的 MapMessages 中检索值。
3. 检索使用 MapLookup.setMainArguments(String[]) 设置的值



1. 第一项仅表示 MapLookup 用于替换配置文件中定义的属性。这些变量没有前缀指定

2. 第二种用法允许替换当前 MapMessage 中的值（如果一个值是当前日志事件的一部分）。

   RoutingAppender 将为 MapMessage 中名为“type”的键的每个唯一值使用不同的 RollingFileAppender

   当以这种方式使用时，应在属性声明中声明“type”的值，以在消息不是 MapMessage 或 MapMessage 不包含键的情况下提供默认值。

   ```xml
   <Routing name="Routing">
     <Routes pattern="$${map:type}">
       <Route>
         <RollingFile name="Rolling-${map:type}" fileName="${filename}"
                      filePattern="target/rolling1/test1-${map:type}.%i.log.gz">
           <PatternLayout>
             <pattern>%d %p %c{1.} [%t] %m%n</pattern>
           </PatternLayout>
           <SizeBasedTriggeringPolicy size="500" />
         </RollingFile>
       </Route>
     </Routes>
   </Routing>
   ```

   



# Marker Lookup

标记查找允许您在有趣的配置中使用标记，例如路由附加程序。

考虑以下基于标记记录到不同文件的 YAML 配置和代码：

```xml
Configuration:
  status: debug
 
  Appenders:
    Console:
    RandomAccessFile:
      - name: SQL_APPENDER
        fileName: logs/sql.log
        PatternLayout:
          Pattern: "%d{ISO8601_BASIC} %-5level %logger{1} %X %msg%n"
      - name: PAYLOAD_APPENDER
        fileName: logs/payload.log
        PatternLayout:
          Pattern: "%d{ISO8601_BASIC} %-5level %logger{1} %X %msg%n"
      - name: PERFORMANCE_APPENDER
        fileName: logs/performance.log
        PatternLayout:
          Pattern: "%d{ISO8601_BASIC} %-5level %logger{1} %X %msg%n"
 
    Routing:
      name: ROUTING_APPENDER
      Routes:
        pattern: "$${marker:}"
        Route:
        - key: PERFORMANCE
          ref: PERFORMANCE_APPENDER
        - key: PAYLOAD
          ref: PAYLOAD_APPENDER
        - key: SQL
          ref: SQL_APPENDER
 
  Loggers:
    Root:
      level: trace
      AppenderRef:
        - ref: ROUTING_APPENDER
```

```xml
public static final Marker SQL = MarkerFactory.getMarker("SQL");
public static final Marker PAYLOAD = MarkerFactory.getMarker("PAYLOAD");
public static final Marker PERFORMANCE = MarkerFactory.getMarker("PERFORMANCE");
 
final Logger logger = LoggerFactory.getLogger(Logger.ROOT_LOGGER_NAME);
 
logger.info(SQL, "Message in Sql.log");
logger.info(PAYLOAD, "Message in Payload.log");
logger.info(PERFORMANCE, "Message in Performance.log");
```



注意配置的关键部分是模式：“$${marker:}”。

这将产生三个日志文件，每个文件都有一个特定标记的日志事件。 

Log4j 会将带有 SQL 标记的日志事件路由到 sql.log，将带有 PAYLOAD 标记的日志事件路由到 payload.log，依此类推。

您可以使用符号“${marker:name}”和“$${marker:name}”来检查是否存在名称为标记名称的标记。

如果标记存在，则表达式返回名称，否则返回 null。



# Spring Boot Lookup

Spring Boot Lookup 从 Spring 配置中检索 Spring 属性的值以及活动和默认配置文件的值。

指定“profiles.active”键将返回活动配置文件，而“profiles.default”键将返回默认配置文件。

默认和活动配置文件可以是一个数组。



如果存在多个配置文件，它们将作为逗号分隔列表返回。

要从数组中检索单个项目，请将“[{index}]”附加到键。

例如，要返回列表中的第一个活动配置文件，请指定“profiles.active[0]”。

```xml
<File name="Application" fileName="application-${spring:profiles.active[0]}.log">
  <PatternLayout>
    <pattern>%d %p %c{1.} [%t] $${spring:spring.application.name} %m%n</pattern>
  </PatternLayout>
</File>
```

# Structured Data Lookup

StructuredDataLookup 与 MapLookup 非常相似，因为它将从 StructuredDataMessages 中检索值。

除了 Map 值，它还将返回 id 的名称部分（不包括企业编号）和类型字段。

下面的示例与 MapMessage 示例之间的主要区别在于，“type”是 StructuredDataMessage 的一个属性，而“type”必须是 MapMessage 中 Map 中的一个 item。



```xml
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
  </Routes>
</Routing>
```

# System Properties Lookup

由于使用系统属性在应用程序内部和外部定义值是很常见的，因此可以通过查找访问它们是很自然的。

由于系统属性通常是在应用程序之外定义的，因此通常会看到以下内容：

```xml
<Appenders>
  <File name="ApplicationLog" fileName="${sys:logPath}/app.log"/>
</Appenders>
```

此查找还支持默认值语法。

在下面的示例中，当 logPath 系统属性未定义时，将使用默认值 /var/logs：

```xml
<Appenders>
  <File name="ApplicationLog" fileName="${sys:logPath:-/var/logs}/app.log"/>
</Appenders>
```

# Web Lookup

WebLookup 允许应用程序检索与 ServletContext 关联的变量。

除了能够检索 ServletContext 中的各种字段外，WebLookup 还支持查找存储为属性或配置为初始化参数的值。

下表列出了可以检索的各种键：

| Key                   | Description                                                  |
| :-------------------- | :----------------------------------------------------------- |
| attr.*name*           | Returns the ServletContext attribute with the specified name |
| contextPath           | The context path of the web application                      |
| contextPathName       | The first token in the context path of the web application splitting on "/" characters. |
| effectiveMajorVersion | Gets the major version of the Servlet specification that the application represented by this ServletContext is based on. |
| effectiveMinorVersion | Gets the minor version of the Servlet specification that the application represented by this ServletContext is based on. |
| initParam.*name*      | Returns the ServletContext initialization parameter with the specified name |
| majorVersion          | Returns the major version of the Servlet API that this servlet container supports. |
| minorVersion          | Returns the minor version of the Servlet API that this servlet container supports. |
| rootDir               | Returns the result of calling getRealPath with a value of "/". |
| serverInfo            | Returns the name and version of the servlet container on which the servlet is running. |
| servletContextName    | Returns the name of the web application as defined in the display-name element of the deployment descriptor |

将首先检查指定的任何其他键名以查看是否存在具有该名称的 ServletContext 属性，然后将检查以查看是否存在该名称的初始化参数。

如果找到了键，则将返回相应的值。

```xml
<Appenders>
  <File name="ApplicationLog" fileName="${web:rootDir}/app.log"/>
</Appenders>
```

