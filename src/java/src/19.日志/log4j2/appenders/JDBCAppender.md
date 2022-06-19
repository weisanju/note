# JDBCAppender

JDBCAppender 使用标准 JDBC 将日志事件写入关系数据库表

它可以配置为使用 JNDI 数据源或自定义工厂方法获取 JDBC 连接。无论您采用哪种方法，它都必须由连接池支持,否则，日志性能将受到很大影响



如果配置的 JDBC 驱动程序支持批处理语句并且 bufferSize 配置为正数，则日志事件将被批处理

从 Log4j 2.8 开始，有两种方法可以将日志事件配置为列映射：仅允许字符串和时间戳的原始 ColumnConfig 样式，以及使用 Log4j 的内置类型转换以允许更多数据类型的新 ColumnMapping 插件（这与 Cassandra Appender 中的插件相同）。

为了在开发过程中快速起步，使用基于 JNDI 的连接源的替代方法是使用非池化DriverManager 连接源。

此连接源使用 JDBC 连接字符串、用户名和密码。或者，您还可以使用属性。

# JDBCAppender Parameters



| Parameter Name          | Type             | Description                                                  |
| :---------------------- | :--------------- | :----------------------------------------------------------- |
| name                    | String           | *Required.* The name of the Appender.                        |
| ignoreExceptions        | boolean          | The default is true, causing exceptions encountered while appending events to be internally logged and then ignored. When set to false exceptions will be propagated to the caller, instead. You must set this to false when wrapping this Appender in a [FailoverAppender](https://logging.apache.org/log4j/2.x/manual/appenders.html#FailoverAppender). |
| filter                  | Filter           | A Filter to determine if the event should be handled by this Appender. More than one Filter may be used by using a CompositeFilter. |
| bufferSize              | int              | If an integer greater than 0, this causes the appender to buffer log events and flush whenever the buffer reaches this size. |
| connectionSource        | ConnectionSource | *Required.* The connections source from which database connections should be retrieved. |
| tableName               | String           | *Required.* The name of the database table to insert log events into. |
| columnConfigs           | ColumnConfig[]   | *Required (and/or columnMappings).* 有关应插入记录事件数据的列以及如何插入该数据的信息。这用多个 `<Column>` 元素表示。 |
| columnMappings          | ColumnMapping[]  | *Required (and/or columnConfigs).* 列映射配置列表. 每列必须指定一个列名. 每个列都可以具有由其完全限定的类名指定的转换类型。默认情况下，转换类型为 String. If the configured type is assignment-compatible with [ReadOnlyStringMap](https://logging.apache.org/log4j/2.x/log4j-api/apidocs/org/apache/logging/log4j/util/ReadOnlyStringMap.html) / [ThreadContextMap](https://logging.apache.org/log4j/2.x/log4j-api/apidocs/org/apache/logging/log4j/spi/ThreadContextMap.html) or [ThreadContextStack](https://logging.apache.org/log4j/2.x/log4j-api/apidocs/org/apache/logging/log4j/spi/ThreadContextStack.html), 然后该列将分别用 MDC 或 NDC 填充（这是特定于数据库的，它们处理插入 Map 或 List 值的方式）.如果配置的类型与 java.util.Date 的赋值兼容，则日志时间戳将转换为该配置的日期类型. If the configured type is assignment-compatible with java.sql.Clob or java.sql.NClob, then the formatted event will be set as a Clob or NClob respectively (similar to the traditional ColumnConfig plugin). 如果给出了文字属性，则其值将在 INSERT 查询中按原样使用，而不会进行任何转义。否则，指定的布局或模式将转换为配置的类型并存储在该列中。 |
| immediateFail           | boolean          | 设置为 true 时，日志事件不会等待尝试重新连接，如果 JDBC 资源不可用，则会立即失败。 2.11.2 新功能 |
| reconnectIntervalMillis | long             | 如果设置为大于 0 的值，则在发生错误后，JDBCDatabaseManager 将在等待指定的毫秒数后尝试重新连接到数据库<br />如果重新连接失败，则会抛出异常（如果 ignoreExceptions 设置为 false，则应用程序可以捕获该异常） 2.11.2 中的新功能 |





# ConnectionSource implementation

在配置 JDBCAppender 时，您必须指定一个 ConnectionSource implementation，Appender 从中获取 JDBC 连接。您必须恰好使用以下嵌套元素之一：

* \<DataSource>: Uses JNDI.
* \<ConnectionFactory>: Points to a class-method pair to provide JDBC connections.
* \<DriverManager>: A quick and dirty way to get off the ground, no connection pooling.
* \<PoolingDriver>: Uses Apache Commons DBCP to provide connection pooling

# DataSource Parameters

| Parameter Name | Type   | Description                                                  |
| :------------- | :----- | :----------------------------------------------------------- |
| jndiName       | String | *Required.* The full, prefixed JNDI name that the javax.sql.DataSource is bound to, such as java:/comp/env/jdbc/LoggingDatabase. The DataSource must be backed by a connection pool; otherwise, logging will be very slow. |

# ConnectionFactory Parameters

| Parameter Name | Type   | Description                                                  |
| :------------- | :----- | :----------------------------------------------------------- |
| class          | Class  | *Required. 包含用于获取 JDBC 连接的静态工厂方法的类的完全限定名称。 |
| method         | Method | *Required.* 用于获取 JDBC 连接的静态工厂方法的名称。此方法必须没有参数，并且其返回类型必须是 java.sql.Connection 或 DataSource。如果该方法返回连接，则必须从连接池中获取它们（并且它们会在 Log4j 完成时返回到连接池中）；否则，日志记录会很慢。如果该方法返回一个 DataSource，则该 DataSource 只会被检索一次，并且出于同样的原因，它必须由连接池支持。 |

# DriverManager Parameters

| Parameter Name   | Type       | Description                                                  |
| :--------------- | :--------- | :----------------------------------------------------------- |
| connectionString | String     | *Required.* 特定于驱动程序的 JDBC 连接字符串。               |
| userName         | String     | The database user name. 数据库用户名。您不能同时指定 `properties` 和 `userName`或 `password` |
| password         | String     | 数据库密码。您不能同时指定属性和用户名或密码                 |
| driverClassName  | String     | JDBC 驱动程序类名称。某些旧的 JDBC 驱动程序只能通过按类名显式加载它们来发现 |
| properties       | Property[] | 属性列表。您不能同时指定属性和用户名或密码。                 |





| Parameter Name            | Type                              | Description                                                  |
| :------------------------ | :-------------------------------- | :----------------------------------------------------------- |
| DriverManager parameters  | DriverManager parameters          | This connection source inherits all parameter from the DriverManager connection source. |
| poolName                  | String                            | The pool name used to pool JDBC Connections. Defaults to example. You can use the JDBC connection string prefix jdbc:apache:commons:dbcp: followed by the pool name if you want to use a pooled connection elsewhere. For example: jdbc:apache:commons:dbcp:example. |
| PoolableConnectionFactory | PoolableConnectionFactory element | Defines a PoolableConnectionFactory.                         |

# PoolingDriver Parameters (Apache Commons DBCP)

| Parameter Name           | Type                     | Description                                                  |
| :----------------------- | :----------------------- | :----------------------------------------------------------- |
| DriverManager parameters | DriverManager parameters | 此连接源继承了 DriverManager 连接源的所有参数                |
| poolName                 | String                   | 用于池化 JDBC 连接的池名称。默认为示例。如果要在其他地方使用池连接，可以使用 JDBC 连接字符串前缀 jdbc:apache:commons:dbcp: 后跟池名称。例如：jdbc:apache:commons:dbcp:example。 |

# PoolableConnectionFactory Parameters (Apache Commons DBCP)

| Parameter Name                | Type    | Description                                                  |
| :---------------------------- | :------ | :----------------------------------------------------------- |
| autoCommitOnReturn            | boolean | See [Apache Commons DBCP PoolableConnectionFactory.](http://commons.apache.org/proper/commons-dbcp/api-2.4.0/org/apache/commons/dbcp2/PoolableConnectionFactory.html) |
| cacheState                    | boolean | See [Apache Commons DBCP PoolableConnectionFactory.](http://commons.apache.org/proper/commons-dbcp/api-2.4.0/org/apache/commons/dbcp2/PoolableConnectionFactory.html) |
| ConnectionInitSqls            | Strings | See [Apache Commons DBCP PoolableConnectionFactory.](http://commons.apache.org/proper/commons-dbcp/api-2.4.0/org/apache/commons/dbcp2/PoolableConnectionFactory.html) |
| defaultAutoCommit             | boolean | See [Apache Commons DBCP PoolableConnectionFactory.](http://commons.apache.org/proper/commons-dbcp/api-2.4.0/org/apache/commons/dbcp2/PoolableConnectionFactory.html) |
| defaultCatalog                | String  | See [Apache Commons DBCP PoolableConnectionFactory.](http://commons.apache.org/proper/commons-dbcp/api-2.4.0/org/apache/commons/dbcp2/PoolableConnectionFactory.html) |
| defaultQueryTimeoutSeconds    | integer | See [Apache Commons DBCP PoolableConnectionFactory.](http://commons.apache.org/proper/commons-dbcp/api-2.4.0/org/apache/commons/dbcp2/PoolableConnectionFactory.html) |
| defaultReadOnly               | boolean | See [Apache Commons DBCP PoolableConnectionFactory.](http://commons.apache.org/proper/commons-dbcp/api-2.4.0/org/apache/commons/dbcp2/PoolableConnectionFactory.html) |
| defaultTransactionIsolation   | integer | See [Apache Commons DBCP PoolableConnectionFactory.](http://commons.apache.org/proper/commons-dbcp/api-2.4.0/org/apache/commons/dbcp2/PoolableConnectionFactory.html) |
| disconnectionSqlCodes         | Strings | See [Apache Commons DBCP PoolableConnectionFactory.](http://commons.apache.org/proper/commons-dbcp/api-2.4.0/org/apache/commons/dbcp2/PoolableConnectionFactory.html) |
| fastFailValidation            | boolean | See [Apache Commons DBCP PoolableConnectionFactory.](http://commons.apache.org/proper/commons-dbcp/api-2.4.0/org/apache/commons/dbcp2/PoolableConnectionFactory.html) |
| maxConnLifetimeMillis         | long    | See [Apache Commons DBCP PoolableConnectionFactory.](http://commons.apache.org/proper/commons-dbcp/api-2.4.0/org/apache/commons/dbcp2/PoolableConnectionFactory.html) |
| maxOpenPreparedStatements     | integer | See [Apache Commons DBCP PoolableConnectionFactory.](http://commons.apache.org/proper/commons-dbcp/api-2.4.0/org/apache/commons/dbcp2/PoolableConnectionFactory.html) |
| poolStatements                | boolean | See [Apache Commons DBCP PoolableConnectionFactory.](http://commons.apache.org/proper/commons-dbcp/api-2.4.0/org/apache/commons/dbcp2/PoolableConnectionFactory.html) |
| rollbackOnReturn              | boolean | See [Apache Commons DBCP PoolableConnectionFactory.](http://commons.apache.org/proper/commons-dbcp/api-2.4.0/org/apache/commons/dbcp2/PoolableConnectionFactory.html) |
| validationQuery               | String  | See [Apache Commons DBCP PoolableConnectionFactory.](http://commons.apache.org/proper/commons-dbcp/api-2.4.0/org/apache/commons/dbcp2/PoolableConnectionFactory.html) |
| validationQueryTimeoutSeconds | integer | See [Apache Commons DBCP PoolableConnectionFactory.](http://commons.apache.org/proper/commons-dbcp/api-2.4.0/org/apache/commons/dbcp2/PoolableConnectionFactory.html) |

配置 JDBCAppender 时，使用嵌套的 `<Column>` 元素指定应写入表中的哪些列以及如何写入它们。 JDBCAppender 使用此信息制定 PreparedStatement 以插入没有 SQL 注入漏洞的记录。

# Column Parameters

| Parameter Name   | Type    | Description                                                  |
| :--------------- | :------ | :----------------------------------------------------------- |
| name             | String  | *Required.* The name of the database column.                 |
| pattern          | String  | Use this attribute to insert a value or values from the log event in this column using a PatternLayout pattern. Simply specify any legal pattern in this attribute. Either this attribute, literal, or isEventTimestamp="true" must be specified, but not more than one of these. |
| literal          | String  | Use this attribute to insert a literal value in this column. The value will be included directly in the insert SQL, without any quoting (which means that if you want this to be a string, your value should contain single quotes around it like this: literal="'Literal String'"). This is especially useful for databases that don't support identity columns. For example, if you are using Oracle you could specify literal="NAME_OF_YOUR_SEQUENCE.NEXTVAL" to insert a unique ID in an ID column. Either this attribute, pattern, or isEventTimestamp="true" must be specified, but not more than one of these. |
| parameter        | String  | Use this attribute to insert an expression with a parameter marker '?' in this column. The value will be included directly in the insert SQL, without any quoting (which means that if you want this to be a string, your value should contain single quotes around it like this:<ColumnMapping name="instant" parameter="TIMESTAMPADD('MILLISECOND', ?, TIMESTAMP '1970-01-01')"/>You can only specify one of literal or parameter. |
| isEventTimestamp | boolean | Use this attribute to insert the event timestamp in this column, which should be a SQL datetime. The value will be inserted as a java.sql.Types.TIMESTAMP. Either this attribute (equal to true), pattern, or isEventTimestamp must be specified, but not more than one of these. |
| isUnicode        | boolean | This attribute is ignored unless pattern is specified. If true or omitted (default), the value will be inserted as unicode (setNString or setNClob). Otherwise, the value will be inserted non-unicode (setString or setClob). |
| isClob           | boolean | This attribute is ignored unless pattern is specified. Use this attribute to indicate that the column stores Character Large Objects (CLOBs). If true, the value will be inserted as a CLOB (setClob or setNClob). If false or omitted (default), the value will be inserted as a VARCHAR or NVARCHAR (setString or setNString). |



# ColumnMapping Parameters

| Parameter Name | Type   | Description                                                  |
| :------------- | :----- | :----------------------------------------------------------- |
| name           | String | *Required.* The name of the database column.                 |
| pattern        | String | Use this attribute to insert a value or values from the log event in this column using a PatternLayout pattern. Simply specify any legal pattern in this attribute. Either this attribute, literal, or isEventTimestamp="true" must be specified, but not more than one of these. |
| literal        | String | Use this attribute to insert a literal value in this column. The value will be included directly in the insert SQL, without any quoting (which means that if you want this to be a string, your value should contain single quotes around it like this: literal="'Literal String'"). This is especially useful for databases that don't support identity columns. For example, if you are using Oracle you could specify literal="NAME_OF_YOUR_SEQUENCE.NEXTVAL" to insert a unique ID in an ID column. Either this attribute, pattern, or isEventTimestamp="true" must be specified, but not more than one of these. |
| layout         | Layout | The Layout to format the LogEvent.                           |
| type           | String | Conversion type name, a fully-qualified class name.          |

# Example

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="error">
  <Appenders>
    <JDBC name="databaseAppender" tableName="dbo.application_log">
      <DataSource jndiName="java:/comp/env/jdbc/LoggingDataSource" />
      <Column name="eventDate" isEventTimestamp="true" />
      <Column name="level" pattern="%level" />
      <Column name="logger" pattern="%logger" />
      <Column name="message" pattern="%message" />
      <Column name="exception" pattern="%ex{full}" />
    </JDBC>
  </Appenders>
  <Loggers>
    <Root level="warn">
      <AppenderRef ref="databaseAppender"/>
    </Root>
  </Loggers>
</Configuration>
```

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="error">
  <Appenders>
    <JDBC name="databaseAppender" tableName="LOGGING.APPLICATION_LOG">
      <ConnectionFactory class="net.example.db.ConnectionFactory" method="getDatabaseConnection" />
      <Column name="EVENT_ID" literal="LOGGING.APPLICATION_LOG_SEQUENCE.NEXTVAL" />
      <Column name="EVENT_DATE" isEventTimestamp="true" />
      <Column name="LEVEL" pattern="%level" />
      <Column name="LOGGER" pattern="%logger" />
      <Column name="MESSAGE" pattern="%message" />
      <Column name="THROWABLE" pattern="%ex{full}" />
    </JDBC>
  </Appenders>
  <Loggers>
    <Root level="warn">
      <AppenderRef ref="databaseAppender"/>
    </Root>
  </Loggers>
</Configuration>
```

```java
package net.example.db;
 
import java.sql.Connection;
import java.sql.SQLException;
import java.util.Properties;
 
import javax.sql.DataSource;
 
import org.apache.commons.dbcp.DriverManagerConnectionFactory;
import org.apache.commons.dbcp.PoolableConnection;
import org.apache.commons.dbcp.PoolableConnectionFactory;
import org.apache.commons.dbcp.PoolingDataSource;
import org.apache.commons.pool.impl.GenericObjectPool;
 
public class ConnectionFactory {
    private static interface Singleton {
        final ConnectionFactory INSTANCE = new ConnectionFactory();
    }
 
    private final DataSource dataSource;
 
    private ConnectionFactory() {
        Properties properties = new Properties();
        properties.setProperty("user", "logging");
        properties.setProperty("password", "abc123"); // or get properties from some configuration file
 
        GenericObjectPool<PoolableConnection> pool = new GenericObjectPool<PoolableConnection>();
        DriverManagerConnectionFactory connectionFactory = new DriverManagerConnectionFactory(
                "jdbc:mysql://example.org:3306/exampleDb", properties
        );
        new PoolableConnectionFactory(
                connectionFactory, pool, null, "SELECT 1", 3, false, false, Connection.TRANSACTION_READ_COMMITTED
        );
 
        this.dataSource = new PoolingDataSource(pool);
    }
 
    public static Connection getDatabaseConnection() throws SQLException {
        return Singleton.INSTANCE.dataSource.getConnection();
    }
}
```

The following configuration uses a MessageLayout to indicate that the Appender should match the keys of a MapMessage to the names of ColumnMappings when setting the values of the Appender's SQL INSERT statement. This let you insert rows for custom values in a database table based on a Log4j MapMessage instead of values from LogEvents.

```xml
<Configuration status="debug">
 
  <Appenders>
    <Console name="STDOUT">
      <PatternLayout pattern="%C{1.} %m %level MDC%X%n"/>
    </Console>
    <Jdbc name="databaseAppender" tableName="dsLogEntry" ignoreExceptions="false">
      <DataSource jndiName="java:/comp/env/jdbc/TestDataSourceAppender" />
      <ColumnMapping name="Id" />
      <ColumnMapping name="ColumnA" />
      <ColumnMapping name="ColumnB" />
      <MessageLayout />
    </Jdbc>
  </Appenders>
 
  <Loggers>
    <Logger name="org.apache.logging.log4j.core.appender.db" level="debug" additivity="false">
      <AppenderRef ref="databaseAppender" />
    </Logger>
 
    <Root level="fatal">
      <AppenderRef ref="STDOUT"/>
    </Root>
  </Loggers>
 
</Configuration>
```

