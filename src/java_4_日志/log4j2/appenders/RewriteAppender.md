# RewriteAppender

RewriteAppender 允许在 LogEvent 被另一个 Appender 处理之前对其进行操作。

这可用于屏蔽敏感信息（例如密码）或将信息注入每个事件。 

RewriteAppender 必须配置有 RewritePolicy。 

# Parameters

| Parameter Name   | Type          | Description                                                  |
| :--------------- | :------------ | :----------------------------------------------------------- |
| AppenderRef      | String        | 在操作 LogEvent 后要调用的 Appender 的名称。<br/>可以配置多个 AppenderRef 元素。 |
| filter           | Filter        | 一个过滤器，用于确定事件是否应该由这个 Appender 处理。<br/>使用 CompositeFilter 可以使用多个过滤器。 |
| name             | String        | The name of the Appender.                                    |
| rewritePolicy    | RewritePolicy | 将操作 LogEvent 的 RewritePolicy。                           |
| ignoreExceptions | boolean       | The default is true, causing exceptions encountered while appending events to be internally logged and then ignored. When set to false exceptions will be propagated to the caller, instead. You must set this to false when wrapping this Appender in a [FailoverAppender](http://logging.apache.org/log4j/2.x/manual/appenders.html#FailoverAppender). |

# RewritePolicy

RewritePolicy 是一个接口，它允许实现在传递给 Appender 之前检查并可能修改 LogEvent。 

RewritePolicy 声明了一个必须实现的名为 rewrite 的方法。

该方法通过 LogEvent 传递，可以返回相同的事件或创建一个新的事件。



##### MapRewritePolicy

MapRewritePolicy will evaluate LogEvents that contain a MapMessage and will add or update elements of the Map.

| Parameter Name | Type           | Description                        |
| :------------- | :------------- | :--------------------------------- |
| mode           | String         | "Add" or "Update"                  |
| keyValuePair   | KeyValuePair[] | An array of keys and their values. |



以下配置显示了一个 RewriteAppender 配置为将产品密钥及其值添加到 MapMessage。：

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="warn" name="MyApp" packages="">
  <Appenders>
    <Console name="STDOUT" target="SYSTEM_OUT">
      <PatternLayout pattern="%m%n"/>
    </Console>
    <Rewrite name="rewrite">
      <AppenderRef ref="STDOUT"/>
      <MapRewritePolicy mode="Add">
        <KeyValuePair key="product" value="TestProduct"/>
      </MapRewritePolicy>
    </Rewrite>
  </Appenders>
  <Loggers>
    <Root level="error">
      <AppenderRef ref="Rewrite"/>
    </Root>
  </Loggers>
</Configuration>
```



**PropertiesRewritePolicy**

PropertiesRewritePolicy 会将在策略上配置的属性添加到正在记录的 ThreadContext Map。



| Parameter Name | Type       | Description                                                  |
| :------------- | :--------- | :----------------------------------------------------------- |
| properties     | Property[] | One of more Property elements to define the keys and values to be added to the ThreadContext Map. |

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="warn" name="MyApp" packages="">
  <Appenders>
    <Console name="STDOUT" target="SYSTEM_OUT">
      <PatternLayout pattern="%m%n"/>
    </Console>
    <Rewrite name="rewrite">
      <AppenderRef ref="STDOUT"/>
      <PropertiesRewritePolicy>
        <Property name="user">${sys:user.name}</Property>
        <Property name="env">${sys:environment}</Property>
      </PropertiesRewritePolicy>
    </Rewrite>
  </Appenders>
  <Loggers>
    <Root level="error">
      <AppenderRef ref="Rewrite"/>
    </Root>
  </Loggers>
</Configuration>
```

**LoggerNameLevelRewritePolicy**

您可以使用此策略通过更改事件级别来减少第三方代码中的记录器

LoggerNameLevelRewritePolicy 将重写给定记录器名称前缀的日志事件级别。

您可以使用记录器名称前缀和一对级别配置 LoggerNameLevelRewritePolicy，其中一对定义源级别和目标级别。

| Parameter Name | Type           | Description                                                  |
| :------------- | :------------- | :----------------------------------------------------------- |
| logger         | String         | A logger name used as a prefix to test each event's logger name. |
| LevelPair      | KeyValuePair[] | An array of keys and their values, each key is a source level, each value a target level. |

以下配置显示了一个 RewriteAppender，配置为将级别 INFO 映射到 DEBUG，并将级别 WARN 映射到以 com.foo.bar 开头的所有记录器的 INFO。

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="warn" name="MyApp">
  <Appenders>
    <Console name="STDOUT" target="SYSTEM_OUT">
      <PatternLayout pattern="%m%n"/>
    </Console>
    <Rewrite name="rewrite">
      <AppenderRef ref="STDOUT"/>
      <LoggerNameLevelRewritePolicy logger="com.foo.bar">
        <KeyValuePair key="INFO" value="DEBUG"/>
        <KeyValuePair key="WARN" value="INFO"/>
      </LoggerNameLevelRewritePolicy>
    </Rewrite>
  </Appenders>
  <Loggers>
    <Root level="error">
      <AppenderRef ref="Rewrite"/>
    </Root>
  </Loggers>
</Configuration>
```

