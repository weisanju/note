# FailoverAppender

FailoverAppender 包装了一组 appender。如果主要 Appender 失败，则将按顺序尝试次要 Appender，直到成功或没有更多次要 Appender 可以尝试。



# FailoverAppender Parameters

| Parameter Name       | Type     | Description                                                  |
| :------------------- | :------- | :----------------------------------------------------------- |
| filter               | Filter   | 一个过滤器，用于确定事件是否应该由这个 Appender 处理。使用 CompositeFilter 可以使用多个过滤器 |
| primary              | String   | 要使用的主要 Appender 的名称                                 |
| failovers            | String[] | 要使用的辅助 Appender 的名称                                 |
| name                 | String   | The name of the Appender.                                    |
| retryIntervalSeconds | integer  | 在重试主 Appender 之前应该经过的秒数。默认值为 60            |
| ignoreExceptions     | boolean  | 默认值为 true，导致在附加事件时遇到异常以进行内部记录然后被忽略。当设置为 false 时，异常将传播给调用者。 |
| target               | String   | Either "SYSTEM_OUT" or "SYSTEM_ERR". The default is "SYSTEM_ERR". |

# 配置

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="warn" name="MyApp" packages="">
  <Appenders>
    <RollingFile name="RollingFile" fileName="logs/app.log" filePattern="logs/app-%d{MM-dd-yyyy}.log.gz"
                 ignoreExceptions="false">
      <PatternLayout>
        <Pattern>%d %p %c{1.} [%t] %m%n</Pattern>
      </PatternLayout>
      <TimeBasedTriggeringPolicy />
    </RollingFile>
    <Console name="STDOUT" target="SYSTEM_OUT" ignoreExceptions="false">
      <PatternLayout pattern="%m%n"/>
    </Console>
    <Failover name="Failover" primary="RollingFile">
      <Failovers>
        <AppenderRef ref="Console"/>
      </Failovers>
    </Failover>
  </Appenders>
  <Loggers>
    <Root level="error">
      <AppenderRef ref="Failover"/>
    </Root>
  </Loggers>
</Configuration>
```

