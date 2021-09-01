# 前言

CassandraAppender 将其输出写入 [Apache Cassandra](https://cassandra.apache.org/) 数据库。

A keyspace and table must be configured ahead of time, and the columns of that table are mapped in a configuration file

 Each column can specify either a [StringLayout](http://logging.apache.org/log4j/2.x/manual/layouts.html) (e.g., a [PatternLayout](http://logging.apache.org/log4j/2.x/manual/layouts.html#PatternLayout)) along with an optional conversion type, or only a conversion type for org.apache.logging.log4j.spi.ThreadContextMap or org.apache.logging.log4j.spi.ThreadContextStack to store the [MDC or NDC](http://logging.apache.org/log4j/2.x/manual/thread-context.html) in a map or list column respectively



与 java.util.Date 兼容的转换类型将使用转换为该类型的日志事件时间戳（例如，在 Cassandra 中使用 java.util.Date 填充时间戳列类型）。

# CassandraAppender Parameters

| Parameter Name                | Type                                                         | Description                                                  |
| :---------------------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| batched                       | boolean                                                      | Whether or not to use batch statements to write log messages to Cassandra. By default, this is false. |
| batchType                     | [BatchStatement.Type](http://docs.datastax.com/en/drivers/java/3.0/com/datastax/driver/core/BatchStatement.Type.html) | The batch type to use when using batched writes. By default, this is LOGGED. |
| bufferSize                    | int                                                          | The number of log messages to buffer or batch before writing. By default, no buffering is done. |
| clusterName                   | String                                                       | The name of the Cassandra cluster to connect to.             |
| columns                       | ColumnMapping[]                                              | A list of column mapping configurations. Each column must specify a column name. Each column can have a conversion type specified by its fully qualified class name. By default, the conversion type is String. If the configured type is assignment-compatible with [ReadOnlyStringMap](http://logging.apache.org/log4j/2.x/log4j-api/apidocs/org/apache/logging/log4j/util/ReadOnlyStringMap.html) / [ThreadContextMap](http://logging.apache.org/log4j/2.x/log4j-api/apidocs/org/apache/logging/log4j/spi/ThreadContextMap.html) or [ThreadContextStack](http://logging.apache.org/log4j/2.x/log4j-api/apidocs/org/apache/logging/log4j/spi/ThreadContextStack.html), then that column will be populated with the MDC or NDC respectively. If the configured type is assignment-compatible with java.util.Date, then the log timestamp will be converted to that configured date type. If a literal attribute is given, then its value will be used as is in the INSERT query without any escaping. Otherwise, the layout or pattern specified will be converted into the configured type and stored in that column. |
| contactPoints                 | SocketAddress[]                                              | A list of hosts and ports of Cassandra nodes to connect to. These must be valid hostnames or IP addresses. By default, if a port is not specified for a host or it is set to 0, then the default Cassandra port of 9042 will be used. By default, localhost:9042 will be used. |
| filter                        | Filter                                                       | A Filter to determine if the event should be handled by this Appender. More than one Filter may be used by using a CompositeFilter. |
| ignoreExceptions              | boolean                                                      | The default is true, causing exceptions encountered while appending events to be internally logged and then ignored. When set to false exceptions will be propagated to the caller, instead. You must set this to false when wrapping this Appender in a [FailoverAppender](http://logging.apache.org/log4j/2.x/manual/appenders.html#FailoverAppender). |
| keyspace                      | String                                                       | The name of the keyspace containing the table that log messages will be written to. |
| name                          | String                                                       | The name of the Appender.                                    |
| password                      | String                                                       | The password to use (along with the username) to connect to Cassandra. |
| table                         | String                                                       | The name of the table to write log messages to.              |
| useClockForTimestampGenerator | boolean                                                      | Whether or not to use the configured org.apache.logging.log4j.core.util.Clock as a [TimestampGenerator](http://docs.datastax.com/en/drivers/java/3.0/com/datastax/driver/core/TimestampGenerator.html). By default, this is false. |
| username                      | String                                                       | The username to use to connect to Cassandra. By default, no username or password is used. |
| useTls                        | boolean                                                      | Whether or not to use TLS/SSL to connect to Cassandra. This is false by default. |

# ExampleConfiguration

```xml
<Configuration name="CassandraAppenderTest">
  <Appenders>
    <Cassandra name="Cassandra" clusterName="Test Cluster" keyspace="test" table="logs" bufferSize="10" batched="true">
      <SocketAddress host="localhost" port="9042"/>
      <ColumnMapping name="id" pattern="%uuid{TIME}" type="java.util.UUID"/>
      <ColumnMapping name="timeid" literal="now()"/>
      <ColumnMapping name="message" pattern="%message"/>
      <ColumnMapping name="level" pattern="%level"/>
      <ColumnMapping name="marker" pattern="%marker"/>
      <ColumnMapping name="logger" pattern="%logger"/>
      <ColumnMapping name="timestamp" type="java.util.Date"/>
      <ColumnMapping name="mdc" type="org.apache.logging.log4j.spi.ThreadContextMap"/>
      <ColumnMapping name="ndc" type="org.apache.logging.log4j.spi.ThreadContextStack"/>
    </Cassandra>
  </Appenders>
  <Loggers>
    <Logger name="org.apache.logging.log4j.cassandra" level="DEBUG">
      <AppenderRef ref="Cassandra"/>
    </Logger>
    <Root level="ERROR"/>
  </Loggers>
</Configuration>
```

This example configuration uses the following table schema:

```sql
CREATE TABLE logs (
    id timeuuid PRIMARY KEY,
    timeid timeuuid,
    message text,
    level text,
    marker text,
    logger text,
    timestamp timestamp,
    mdc map<text,text>,
    ndc list<text>
);
```



