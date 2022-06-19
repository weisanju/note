# RoutingAppender

RoutingAppender 评估 LogEvents，然后将它们路由到下级 Appender。

目标 Appender 可能是之前配置的一个 appender，可以通过它的名字来引用，也可以根据需要动态创建 Appender。 

RoutingAppender 应该在它引用的任何 Appender 之后配置，以允许它正确关闭。



您还可以使用脚本配置 RoutingAppender：您可以在 appender 启动以及为日志事件选择路由时运行脚本。



# RoutingAppender Parameters

| Parameter Name   | Type          | Description                                                  |
| :--------------- | :------------ | :----------------------------------------------------------- |
| Filter           | Filter        | A Filter to determine if the event should be handled by this Appender. More than one Filter may be used by using a CompositeFilter. |
| name             | String        | The name of the Appender.                                    |
| RewritePolicy    | RewritePolicy | The RewritePolicy that will manipulate the LogEvent.         |
| Routes           | Routes        | 包含一个或多个 Route 声明以标识选择 Appenders 的标准。       |
| Script           | Script        | This Script runs when Log4j starts the RoutingAppender and returns a String Route key to determine the default Route.This script is passed the following variables:RoutingAppender Script ParametersParameter NameTypeDescriptionconfigurationConfigurationThe active Configuration.staticVariablesMapA Map shared between all script invocations for this appender instance. This is the same map passed to the Routes Script. |
| ignoreExceptions | boolean       | The default is true, causing exceptions encountered while appending events to be internally logged and then ignored. When set to false exceptions will be propagated to the caller, instead. You must set this to false when wrapping this Appender in a [FailoverAppender](http://logging.apache.org/log4j/2.x/manual/appenders.html#FailoverAppender). |

在此示例中，脚本使“ServiceWindows”路由成为 Windows 上的默认路由，而“ServiceOther”则成为所有其他操作系统上的默认路由。

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="WARN" name="RoutingTest">
  <Appenders>
    <Routing name="Routing">
      <Script name="RoutingInit" language="JavaScript"><![CDATA[
        importPackage(java.lang);
        System.getProperty("os.name").search("Windows") > -1 ? "ServiceWindows" : "ServiceOther";]]>
      </Script>
      <Routes>
        <Route key="ServiceOther">
          <List name="List1" />
        </Route>
        <Route key="ServiceWindows">
          <List name="List2" />
        </Route>
      </Routes>
    </Routing>
  </Appenders>
  <Loggers>
    <Root level="error">
      <AppenderRef ref="Routing" />
    </Root>
  </Loggers>
</Configuration>
```



# Routes

Routes 元素接受名为“pattern”的单个属性。

该模式针对所有注册的 lookups 进行评估，结果用于选择路由。

每个路由都可以配置一个 key。

如果key 与评估模式的结果匹配，则将选择该路由。

如果没有在 Route 上指定键，则该 Route 是默认值。

默认只能配置一个Route。



Routes 元素可能包含一个 Script 子元素。

如果指定，则为每个日志事件运行脚本并返回要使用的字符串路由键。

您必须指定模式属性或脚本元素，但不能同时指定两者。



每个 Route 必须引用一个 Appender。

如果 Route 包含 ref 属性，则 Route 将引用在配置中定义的 Appender。

如果 Route 包含 Appender 定义，则 Appender 将在 RoutingAppender 的上下文中创建，并且每次通过 Route 引用匹配的 Appender 名称时都会重用。



该脚本传递了以下变量：

RoutingAppender Routes Script Parameters

| Parameter Name  | Type          | Description                                                  |
| :-------------- | :------------ | :----------------------------------------------------------- |
| configuration   | Configuration | The active Configuration.                                    |
| staticVariables | Map           | A Map shared between all script invocations for this appender instance. This is the same map passed to the Routes Script. |
| logEvent        | LogEvent      | The log event.                                               |

在此示例中，脚本针对每个日志事件运行，并根据名为“AUDIT”的标记的存在选择路由。

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
        <Script name="RoutingInit" language="JavaScript"><![CDATA[
          if (logEvent.getMarker() != null && logEvent.getMarker().isInstanceOf("AUDIT")) {
                return "AUDIT";
            } else if (logEvent.getContextMap().containsKey("UserId")) {
                return logEvent.getContextMap().get("UserId");
            }
            return "STDOUT";]]>
        </Script>
        <Route>
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

# Purge Policy

outingAppender 可以配置 PurgePolicy，其目的是停止和删除由 RoutingAppender 动态创建的休眠 Appender。 

Log4j 当前提供 IdlePurgePolicy 作为唯一可用于清理 Appenders 的 PurgePolicy。 

IdlePurgePolicy 接受 2 个属性； 

timeToLive，这是 Appender 在没有任何事件发送给它的情况下应该存活的 timeUnits 的数量，以及 timeUnit，java.util.concurrent.TimeUnit 的 String 表示，与 timeToLive 属性一起使用。



下面是一个示例配置，它使用 RoutingAppender 将所有 Audit 事件路由到 FlumeAppender，所有其他事件将路由到仅捕获特定事件类型的 RollingFileAppender。

请注意， AuditAppender 是预定义的，而 RollingFileAppenders 是根据需要创建的。



```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="warn" name="MyApp" packages="">
  <Appenders>
    <Flume name="AuditLogger" compress="true">
      <Agent host="192.168.10.101" port="8800"/>
      <Agent host="192.168.10.102" port="8800"/>
      <RFC5424Layout enterpriseNumber="18060" includeMDC="true" appName="MyApp"/>
    </Flume>
    <Routing name="Routing">
      <Routes pattern="$${sd:type}">
        <Route>
          <RollingFile name="Rolling-${sd:type}" fileName="${sd:type}.log"
                       filePattern="${sd:type}.%i.log.gz">
            <PatternLayout>
              <pattern>%d %p %c{1.} [%t] %m%n</pattern>
            </PatternLayout>
            <SizeBasedTriggeringPolicy size="500" />
          </RollingFile>
        </Route>
        <Route ref="AuditLogger" key="Audit"/>
      </Routes>
      <IdlePurgePolicy timeToLive="15" timeUnit="minutes"/>
    </Routing>
  </Appenders>
  <Loggers>
    <Root level="error">
      <AppenderRef ref="Routing"/>
    </Root>
  </Loggers>
</Configuration>
```

