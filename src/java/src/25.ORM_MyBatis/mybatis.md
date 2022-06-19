<!-- toc -->

{% raw %}

**[Mybatis通用mapper](https://www.toutiao.com/i6824762402345583111/)**





## sqlsession

### 构建 SqlSessionFactory

#### 重载的各个方法

```
SqlSessionFactory build(InputStream inputStream)
SqlSessionFactory build(InputStream inputStream, String environment)
SqlSessionFactory build(InputStream inputStream, Properties properties)
SqlSessionFactory build(InputStream inputStream, String env, Properties props)
SqlSessionFactory build(Configuration config)
```

#### 从核心配置文件构建

```java
String resource = "org/mybatis/example/mybatis-config.xml";
InputStream inputStream = Resources.getResourceAsStream(resource);
SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);
```

#### 使用 Java 代码构建

```java
DataSource dataSource = BlogDataSourceFactory.getBlogDataSource();
TransactionFactory transactionFactory = new JdbcTransactionFactory();
Environment environment = new Environment("development", transactionFactory, dataSource);
Configuration configuration = new Configuration(environment);
configuration.addMapper(BlogMapper.class);
SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(configuration);
```

### 构建 sqlsession

#### 重载方法

```
SqlSession openSession()
SqlSession openSession(boolean autoCommit)
SqlSession openSession(Connection connection)
SqlSession openSession(TransactionIsolationLevel level)
SqlSession openSession(ExecutorType execType, TransactionIsolationLevel level)
SqlSession openSession(ExecutorType execType)
SqlSession openSession(ExecutorType execType, boolean autoCommit)
SqlSession openSession(ExecutorType execType, Connection connection)
Configuration getConfiguration();
```

进行数据库操作要考虑的问题

- **数据库连接**
- **事务处理**
- **语句执行**

默认的 openSession() 方法没有参数，它会创建具备如下特性的 SqlSession：

- 事务作用域将会开启（也就是不自动提交）。
- 将由当前环境配置的 DataSource 实例中获取 Connection 对象。
- 事务隔离级别将会使用驱动或数据源的默认设置。
- 预处理语句不会被复用，也不会批量处理更新。

#### _ExecutorType_

- `ExecutorType.SIMPLE`：该类型的执行器没有特别的行为。它为每个语句的执行创建一个新的预处理语句。
- `ExecutorType.REUSE`：该类型的执行器会复用预处理语句。
- `ExecutorType.BATCH`：该类型的执行器会批量执行所有更新语句，如果 SELECT 在多个更新中间执行，将在必要时将多条更新语句分隔开来，以方便理解。

### 语句执行

#### 语句执行方法

```
<T> T selectOne(String statement, Object parameter)
<E> List<E> selectList(String statement, Object parameter)
<T> Cursor<T> selectCursor(String statement, Object parameter)
<K,V> Map<K,V> selectMap(String statement, Object parameter, String mapKey)
int insert(String statement, Object parameter)
int update(String statement, Object parameter)
int delete(String statement, Object parameter)
```

游标（Cursor）与列表（List）返回的结果相同，不同的是，游标借助迭代器实现了数据的惰性加载。

#### 限制返回行数

```
<E> List<E> selectList (String statement, Object parameter, RowBounds rowBounds)
<T> Cursor<T> selectCursor(String statement, Object parameter, RowBounds rowBounds)
<K,V> Map<K,V> selectMap(String statement, Object parameter, String mapKey, RowBounds rowbounds)
void select (String statement, Object parameter, ResultHandler<T> handler)
void select (String statement, Object parameter, RowBounds rowBounds, ResultHandler<T> handler)
```

```
RowBounds rowBounds = new RowBounds(offset, limit);
```

1. 数据库驱动决定了略过记录时的查询效率。为了获得最佳的性能，建议将 ResultSet 类型设置为 SCROLL_SENSITIVE 或 SCROLL_INSENSITIVE（换句话说：不要使用 FORWARD_ONLY）。

2. 如果 PreparedStatement 对象初始化时 resultSetType 参数设置为 TYPE_FORWARD_ONLY，在从 ResultSet（结果集）中读取记录的时，对于访问过的记录就自动释放了内存。而设置为 TYPE_SCROLL_INSENSITIVE 或 TYPE_SCROLL_SENSITIVE 时为了保证能游标能向上移动到任意位置，已经访问过的所有都保留在内存中不能释放。所以大量数据加载的时候，就 OOM 了。

#### 结果行自定义处理

从版本 3.4.6 开始，`ResultHandler` 会在存储过程的 REFCURSOR 输出参数中传递使用的 `CALLABLE` 语句。

```
package org.apache.ibatis.session;
public interface ResultHandler<T> {
  void handleResult(ResultContext<? extends T> context);
}
```

ResultContext 参数允许你访问结果对象和当前已被创建的对象数目，另外还提供了一个返回值为 Boolean 的 stop 方法，你可以使用此 stop 方法来停止 MyBatis 加载更多的结果。

使用 ResultHandler 的时候需要注意以下两个限制：

- 使用带 ResultHandler 参数的方法时，收到的数据不会被缓存。
- 当使用高级的结果映射集（resultMap）时，MyBatis 很可能需要数行结果来构造一个对象。如果你使用了 ResultHandler，你可能会接收到关联（association）或者集合（collection）中尚未被完整填充的对象。

#### 立即批量更新方法

```
List<BatchResult> flushStatements()
```

当你将 `ExecutorType` 设置为 `ExecutorType.BATCH` 时，可以使用这个方法清除（执行）缓存在 JDBC 驱动类中的批量更新语句。

#### 事务控制方法

有四个方法用来控制事务作用域。

如果你已经设置了自动提交或你使用了外部事务管理器

这些方法就没什么作用了。然而，如果你正在使用由 Connection 实例控制的 JDBC 事务管理器，那么这四个方法就会派上用场：

```
void commit()
void commit(boolean force)
void rollback()
void rollback(boolean force)
```

## configuration（配置）

### properties（属性）

#### 可以外部配置,可以字面写入

```
<properties resource="org/mybatis/example/config.properties">
  <property name="username" value="dev_user"/>
  <property name="password" value="F2Fa3!33TYyg"/>
</properties>
```

#### 可以使用载入的配置

```

<dataSource type="POOLED">
  <property name="driver" value="${driver}"/>
  <property name="url" value="${url}"/>
  <property name="username" value="${username}"/>
  <property name="password" value="${password}"/>
</dataSource>
```

#### 代码传入变量

```
可以通过
SqlSessionFactory factory = new SqlSessionFactoryBuilder().build(reader, environment, props);传入变量
```

#### 变量优先级

```
变量优先级
properties标签体内的属性 < properties指定的resource < 方法传入的properties
```

#### 默认值

```
开启占位符功能
<properties resource="org/mybatis/example/config.properties">
  <!-- ... -->
  <property name="org.apache.ibatis.parsing.PropertyParser.enable-default-value" value="true"/> <!-- 启用默认值特性 -->
</properties>

<property name="username" value="${username:ut_user}"/>

```

### 设置

- _useActualParamName_ true|false

  - 允许使用方法签名中的名称作为语句参数名称

  - 采用 Java 8 编译，并且加上 `-parameters` 选项
  - (新增于 3.4.1)

### typeAliases（类型别名）

#### 类型别名

```
<typeAliases>
  <typeAlias alias="Author" type="domain.blog.Author"/>
  <typeAlias alias="Blog" type="domain.blog.Blog"/>
  <typeAlias alias="Comment" type="domain.blog.Comment"/>
  <typeAlias alias="Post" type="domain.blog.Post"/>
  <typeAlias alias="Section" type="domain.blog.Section"/>
  <typeAlias alias="Tag" type="domain.blog.Tag"/>
</typeAliases>
```

#### 指定包名

```
<typeAliases>
  <package name="domain.blog"/>
</typeAliases>
```

#### 注解

```
@Alias("author")
```

### typeHandlers（类型处理器）

#### 内置类型处理器

从 3.4.5 开始，MyBatis 默认支持 JSR-310

| 类型处理器                   | Java 类型                       | JDBC 类型                                                                   |
| :--------------------------- | :------------------------------ | :-------------------------------------------------------------------------- |
| `BooleanTypeHandler`         | `java.lang.Boolean`, `boolean`  | 数据库兼容的 `BOOLEAN`                                                      |
| `ByteTypeHandler`            | `java.lang.Byte`, `byte`        | 数据库兼容的 `NUMERIC` 或 `BYTE`                                            |
| `ShortTypeHandler`           | `java.lang.Short`, `short`      | 数据库兼容的 `NUMERIC` 或 `SMALLINT`                                        |
| `IntegerTypeHandler`         | `java.lang.Integer`, `int`      | 数据库兼容的 `NUMERIC` 或 `INTEGER`                                         |
| `LongTypeHandler`            | `java.lang.Long`, `long`        | 数据库兼容的 `NUMERIC` 或 `BIGINT`                                          |
| `FloatTypeHandler`           | `java.lang.Float`, `float`      | 数据库兼容的 `NUMERIC` 或 `FLOAT`                                           |
| `DoubleTypeHandler`          | `java.lang.Double`, `double`    | 数据库兼容的 `NUMERIC` 或 `DOUBLE`                                          |
| `BigDecimalTypeHandler`      | `java.math.BigDecimal`          | 数据库兼容的 `NUMERIC` 或 `DECIMAL`                                         |
| `StringTypeHandler`          | `java.lang.String`              | `CHAR`, `VARCHAR`                                                           |
| `ClobReaderTypeHandler`      | `java.io.Reader`                | -                                                                           |
| `ClobTypeHandler`            | `java.lang.String`              | `CLOB`, `LONGVARCHAR`                                                       |
| `NStringTypeHandler`         | `java.lang.String`              | `NVARCHAR`, `NCHAR`                                                         |
| `NClobTypeHandler`           | `java.lang.String`              | `NCLOB`                                                                     |
| `BlobInputStreamTypeHandler` | `java.io.InputStream`           | -                                                                           |
| `ByteArrayTypeHandler`       | `byte[]`                        | 数据库兼容的字节流类型                                                      |
| `BlobTypeHandler`            | `byte[]`                        | `BLOB`, `LONGVARBINARY`                                                     |
| `DateTypeHandler`            | `java.util.Date`                | `TIMESTAMP`                                                                 |
| `DateOnlyTypeHandler`        | `java.util.Date`                | `DATE`                                                                      |
| `TimeOnlyTypeHandler`        | `java.util.Date`                | `TIME`                                                                      |
| `SqlTimestampTypeHandler`    | `java.sql.Timestamp`            | `TIMESTAMP`                                                                 |
| `SqlDateTypeHandler`         | `java.sql.Date`                 | `DATE`                                                                      |
| `SqlTimeTypeHandler`         | `java.sql.Time`                 | `TIME`                                                                      |
| `ObjectTypeHandler`          | Any                             | `OTHER` 或未指定类型                                                        |
| `EnumTypeHandler`            | Enumeration Type                | VARCHAR 或任何兼容的字符串类型，用来存储枚举的名称（而不是索引序数值）      |
| `EnumOrdinalTypeHandler`     | Enumeration Type                | 任何兼容的 `NUMERIC` 或 `DOUBLE` 类型，用来存储枚举的序数值（而不是名称）。 |
| `SqlxmlTypeHandler`          | `java.lang.String`              | `SQLXML`                                                                    |
| `InstantTypeHandler`         | `java.time.Instant`             | `TIMESTAMP`                                                                 |
| `LocalDateTimeTypeHandler`   | `java.time.LocalDateTime`       | `TIMESTAMP`                                                                 |
| `LocalDateTypeHandler`       | `java.time.LocalDate`           | `DATE`                                                                      |
| `LocalTimeTypeHandler`       | `java.time.LocalTime`           | `TIME`                                                                      |
| `OffsetDateTimeTypeHandler`  | `java.time.OffsetDateTime`      | `TIMESTAMP`                                                                 |
| `OffsetTimeTypeHandler`      | `java.time.OffsetTime`          | `TIME`                                                                      |
| `ZonedDateTimeTypeHandler`   | `java.time.ZonedDateTime`       | `TIMESTAMP`                                                                 |
| `YearTypeHandler`            | `java.time.Year`                | `INTEGER`                                                                   |
| `MonthTypeHandler`           | `java.time.Month`               | `INTEGER`                                                                   |
| `YearMonthTypeHandler`       | `java.time.YearMonth`           | `VARCHAR` 或 `LONGVARCHAR`                                                  |
| `JapaneseDateTypeHandler`    | `java.time.chrono.JapaneseDate` | `DATE`                                                                      |

#### 自定义类型转换器

- 实现 `org.apache.ibatis.type.TypeHandler` 接口

- 或继承一个很便利的类 `org.apache.ibatis.type.BaseTypeHandler`

```java
@MappedJdbcTypes(JdbcType.VARCHAR)
public class ExampleTypeHandler extends BaseTypeHandler<String> {

  @Override
  public void setNonNullParameter(PreparedStatement ps, int i, String parameter, JdbcType jdbcType) throws SQLException {
    ps.setString(i, parameter);
  }

  @Override
  public String getNullableResult(ResultSet rs, String columnName) throws SQLException {
    return rs.getString(columnName);
  }

  @Override
  public String getNullableResult(ResultSet rs, int columnIndex) throws SQLException {
    return rs.getString(columnIndex);
  }

  @Override
  public String getNullableResult(CallableStatement cs, int columnIndex) throws SQLException {
    return cs.getString(columnIndex);
  }
}
```

#### 映射 javaType 与 JDBCtype 的方式

- 泛型
- @MappedTypes,@MappedJdbcTypes
- `<typeHandlers>` 标签中给定 javaType="String"属性

#### 手动给定或自动查找类型处理器

注意在使用自动发现功能的时候，只能通过注解方式来指定 JDBC 的类型。

```
<!-- mybatis-config.xml -->
<typeHandlers>
  <typeHandler handler="org.mybatis.example.ExampleTypeHandler"/>
</typeHandlers>
```

```
<!-- mybatis-config.xml -->
<typeHandlers>
  <package name="org.mybatis.example"/>
</typeHandlers>
```

#### 泛型类型处理器

- 需要增加一个接受该类的 class 作为参数的构造器,这样 MyBatis 会在构造一个类型处理器实例的时候传入一个具体的类
- `EnumTypeHandler` 和 `EnumOrdinalTypeHandler` 都是泛型类型处理器

```
public class GenericTypeHandler<E extends MyObject> extends BaseTypeHandler<E> {

  private Class<E> type;

  public GenericTypeHandler(Class<E> type) {
    if (type == null) throw new IllegalArgumentException("Type argument cannot be null");
    this.type = type;
  }
```

#### 枚举类型

```xml
<!-- mybatis-config.xml -->
<typeHandlers>
  <typeHandler handler="org.apache.ibatis.type.EnumOrdinalTypeHandler" javaType="java.math.RoundingMode"/>
</typeHandlers>
```

显示的为 resultMap 指定 typehandler

```xml
<result column="roundingMode" property="roundingMode" typeHandler="org.apache.ibatis.type.EnumTypeHandler"/>
```

### objectFactory（对象工厂）

#### 简述

- 每次 MyBatis 创建结果对象的新实例时，它都会使用一个对象工厂实例来完成实例化工作
- 默认的对象工厂需要做的仅仅是实例化目标类
- 要么通过默认无参构造方法，要么通过存在的参数映射来调用带有参数的构造方法
- 如果想覆盖对象工厂的默认行为，可以通过创建自己的对象工厂来实现

#### 自定义对象工厂

```java
public class ExampleObjectFactory extends DefaultObjectFactory {
  public Object create(Class type) {
    return super.create(type);
  }
  public Object create(Class type, List<Class> constructorArgTypes, List<Object> constructorArgs) {
    return super.create(type, constructorArgTypes, constructorArgs);
  }
  public void setProperties(Properties properties) {
    super.setProperties(properties);
  }
  public <T> boolean isCollection(Class<T> type) {
    return Collection.class.isAssignableFrom(type);
  }}
```

#### 配置

```
<objectFactory type="org.mybatis.example.ExampleObjectFactory">
  <property name="someProperty" value="100"/>
</objectFactory>
```

### plugins（插件）

#### 简述

- MyBatis 允许你在映射语句执行过程中的某一点进行拦截调用
- 可以拦截的列表
  - Executor (update, query, flushStatements, commit, rollback, getTransaction, close, isClosed)
  - ParameterHandler (getParameterObject, setParameters)
  - ResultSetHandler (handleResultSets, handleOutputParameters)
  - StatementHandler (prepare, parameterize, batch, update, query)

#### 自定义插件

```
@Intercepts({@Signature(
  type= Executor.class,
  method = "update",
  args = {MappedStatement.class,Object.class})})
public class ExamplePlugin implements Interceptor {
  private Properties properties = new Properties();
  public Object intercept(Invocation invocation) throws Throwable {
    // implement pre processing if need
    Object returnObject = invocation.proceed();
    // implement post processing if need
    return returnObject;
  }
  public void setProperties(Properties properties) {
    this.properties = properties;
  }
}
```

```
<!-- mybatis-config.xml -->
<plugins>
  <plugin interceptor="org.mybatis.example.ExamplePlugin">
    <property name="someProperty" value="100"/>
  </plugin>
</plugins>
```

#### 覆盖配置类

- 除了用插件来修改 MyBatis 核心行为以外,还可以通过完全覆盖配置类来达到目的
- 只需继承配置类后覆盖其中的某个方法，再把它传递到 SqlSessionFactoryBuilder.build(myConfig) 方法即可
- 这可能会极大影响 MyBatis 的行为，务请慎之又慎

### environments（环境配置）

#### 简述

- MyBatis 可以配置成适应多种环境,多数据库
- 每个数据库对应一个 SqlSessionFactory 实例,每个 SqlSessionFactory 实例只能选择一种环境。

```java
SqlSessionFactory factory = new SqlSessionFactoryBuilder().build(reader, environment);
SqlSessionFactory factory = new SqlSessionFactoryBuilder().build(reader, environment, properties);
```

#### environment 定义了如何配置数据库环境

```
<environments default="development">
  <environment id="development">
    <transactionManager type="JDBC">
      <property name="..." value="..."/>
    </transactionManager>
    <dataSource type="POOLED">
      <property name="driver" value="${driver}"/>
      <property name="url" value="${url}"/>
      <property name="username" value="${username}"/>
      <property name="password" value="${password}"/>
    </dataSource>
  </environment>
</environments>
```

### transactionManager（事务管理器）

#### MyBatis 中有两种类型的事务管理器（也就是 type="[JDBC|MANAGED]"）：

- jdbc

  JDBC – 这个配置直接使用了 JDBC 的提交和回滚设施，它依赖从数据源获得的连接来管理事务作用域。

- MANAGED

  - 不做任何操作,让容器来管理事务的整个生命周期
  - 默认情况下它会关闭连接,然而一些容器并不希望连接被关闭，因此需要将 closeConnection 属性设置为 false 来阻止默认的关闭行为

  ```xml
  <transactionManager type="MANAGED">
    <property name="closeConnection" value="false"/>
  </transactionManager>
  ```

- 使用 Spring + MyBatis，则没有必要配置事务管理器,Spring 模块会使用自带的管理器来覆盖前面的配置。

#### 自定义事务工厂,事务处理类

```
public interface TransactionFactory {
  default void setProperties(Properties props) { // 从 3.5.2 开始，该方法为默认方法
    // 空实现
  }
  Transaction newTransaction(Connection conn);
  Transaction newTransaction(DataSource dataSource, TransactionIsolationLevel level, boolean autoCommit);
}
public interface Transaction {
  Connection getConnection() throws SQLException;
  void commit() throws SQLException;
  void rollback() throws SQLException;
  void close() throws SQLException;
  Integer getTimeout() throws SQLException;
}
```

### dataSource（数据源）

- dataSource 元素使用标准的 JDBC 数据源接口来配置 JDBC 连接对象的资源

- 有三种内建的数据源类型（也就是 type="[UNPOOLED|POOLED|JNDI]"）：

  - unpooled
    - `driver` – 这是 JDBC 驱动的 Java 类全限定名（并不是 JDBC 驱动中可能包含的数据源类）。
    - `url` – 这是数据库的 JDBC URL 地址。
    - `username` – 登录数据库的用户名。
    - `password` – 登录数据库的密码。
    - `defaultTransactionIsolationLevel` – 默认的连接事务隔离级别。
    - `defaultNetworkTimeout` – 等待数据库操作完成的默认网络超时时间（单位：毫秒）。查看 `java.sql.Connection#setNetworkTimeout()` 的 API 文档以获取更多信息。
    - driver.encoding=UTF8,这将通过 DriverManager.getConnection(url, driverProperties) 方法传递值为 `UTF8` 的 `encoding` 属性给数据库驱动。
  - pooled
    - `poolMaximumActiveConnections` – 在任意时间可存在的活动（正在使用）连接数量，默认值：10
    - `poolMaximumIdleConnections` – 任意时间可能存在的空闲连接数。
    - `poolMaximumCheckoutTime` – 在被强制返回之前，池中连接被检出（checked out）时间，默认值：20000 毫秒（即 20 秒）
    - `poolTimeToWait` – 这是一个底层设置，如果获取连接花费了相当长的时间，连接池会打印状态日志并重新尝试获取一个连接（避免在误配置的情况下一直失败且不打印日志），默认值：20000 毫秒（即 20 秒）。
    - `poolMaximumLocalBadConnectionTolerance` – 这是一个关于坏连接容忍度的底层设置， 作用于每一个尝试从缓存池获取连接的线程。 如果这个线程获取到的是一个坏的连接，那么这个数据源允许这个线程尝试重新获取一个新的连接，但是这个重新尝试的次数不应该超过 `poolMaximumIdleConnections` 与 `poolMaximumLocalBadConnectionTolerance` 之和。 默认值：3（新增于 3.4.5）
    - `poolPingQuery` – 发送到数据库的侦测查询，用来检验连接是否正常工作并准备接受请求。默认是“NO PING QUERY SET”，这会导致多数数据库驱动出错时返回恰当的错误消息。
    - `poolPingEnabled` – 是否启用侦测查询。若开启，需要设置 `poolPingQuery` 属性为一个可执行的 SQL 语句（最好是一个速度非常快的 SQL 语句），默认值：false。
    - `poolPingConnectionsNotUsedFor` – 配置 poolPingQuery 的频率。可以被设置为和数据库连接超时时间一样，来避免不必要的侦测，默认值：0（即所有连接每一时刻都被侦测 — 当然仅当 poolPingEnabled 为 true 时适用）。
  - JNDI:这个数据源实现是为了能在如 EJB 或应用服务器这类容器中使用
    - `initial_context` – 这个属性用来在 InitialContext 中寻找上下文（即，initialContext.lookup(initial_context)）。这是个可选属性，如果忽略，那么将会直接从 InitialContext 中寻找 data_source 属性。
    - `data_source` – 这是引用数据源实例位置的上下文路径。提供了 initial_context 配置时会在其返回的上下文中进行查找，没有提供时则直接在 InitialContext 中查找

- 自定义数据源

  ```
  public interface DataSourceFactory {
    void setProperties(Properties props);
    DataSource getDataSource();
  }
  <dataSource type="org.myproject.C3P0DataSourceFactory">
    <property name="driver" value="org.postgresql.Driver"/>
    <property name="url" value="jdbc:postgresql:mydb"/>
    <property name="username" value="postgres"/>
    <property name="password" value="root"/>
  </dataSource>
  ```

databaseIdProvider（数据库厂商标识）

- MyBatis 可以根据不同的数据库厂商执行不同的语句
- 这种多厂商的支持是基于映射语句中的 `databaseId` 属性

- MyBatis 会加载带有匹配当前数据库 `databaseId` 属性和所有不带 `databaseId` 属性的语句。 如果同时找到带有 `databaseId` 和不带 `databaseId` 的相同语句，则后者会被舍弃

  ```
  <databaseIdProvider type="DB_VENDOR" />
  
  对应的 DB_VENDOR 实现会将 databaseId 设置为 DatabaseMetaData#getDatabaseProductName() 返回的字符串
  ```

  <databaseIdProvider type="DB_VENDOR">
    <property name="SQL Server" value="sqlserver"/>
    <property name="DB2" value="db2"/>
    <property name="Oracle" value="oracle" />
  </databaseIdProvider>

  ```
  
  ```

- 自定义数据库别名解析

  ```java
  public interface DatabaseIdProvider {
    default void setProperties(Properties p) { // 从 3.5.2 开始，该方法为默认方法
      // 空实现
    }
    String getDatabaseId(DataSource dataSource) throws SQLException;
  }
  ```

### mappers 配置（映射器）

告诉 MyBatis 到哪里去找到这些语句

```
<!-- 使用相对于类路径的资源引用 -->
<mappers>
  <mapper resource="org/mybatis/builder/AuthorMapper.xml"/>
  <mapper resource="org/mybatis/builder/BlogMapper.xml"/>
  <mapper resource="org/mybatis/builder/PostMapper.xml"/>
</mappers>
```

```
<!-- 使用完全限定资源定位符（URL） -->
<mappers>
  <mapper url="file:///var/mappers/AuthorMapper.xml"/>
  <mapper url="file:///var/mappers/BlogMapper.xml"/>
  <mapper url="file:///var/mappers/PostMapper.xml"/>
</mappers>
```

```
<!-- 使用映射器接口实现类的完全限定类名 -->
<mappers>
  <mapper class="org.mybatis.builder.AuthorMapper"/>
  <mapper class="org.mybatis.builder.BlogMapper"/>
  <mapper class="org.mybatis.builder.PostMapper"/>
</mappers>
```

```
<!-- 将包内的映射器接口实现全部注册为映射器 -->
<mappers>
  <package name="org.mybatis.builder"/>
</mappers>
```

## XML 映射文件

### XML 顶级元素

`cache` – 该命名空间的缓存配置。

`cache-ref` – 引用其它命名空间的缓存配置。

`resultMap` – 描述如何从数据库结果集中加载对象，是最复杂也是最强大的元素。

`parameterMap` – 老式风格的参数映射。此元素已被废弃，并可能在将来被移除！请使用行内参数映射。文档中不会介绍此元素。

`sql` – 可被其它语句引用的可重用语句块。

`insert` – 映射插入语句。

`update` – 映射更新语句。

`delete` – 映射删除语句。

`select` – 映射查询语句。

### select 元素属性

| `id`            | 在命名空间中唯一的标识符，可以被用来引用这条语句。                                                                                                                                                        |
| --------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `parameterType` | 将会传入这条语句的参数的类全限定名或别名。这个属性是可选的，因为 MyBatis 可以通过类型处理器（TypeHandler）推断出具体传入语句的参数，默认值为未设置（unset）。                                             |
| parameterMap    | 用于引用外部 parameterMap 的属性，目前已被废弃。请使用行内参数映射和 parameterType 属性。                                                                                                                 |
| `resultType`    | 期望从这条语句中返回结果的类全限定名或别名。 注意，如果返回的是集合，那应该设置为集合包含的类型，而不是集合本身的类型。 resultType 和 resultMap 之间只能同时使用一个。                                    |
| `resultMap`     | 对外部 resultMap 的命名引用。结果映射是 MyBatis 最强大的特性，如果你对其理解透彻，许多复杂的映射问题都能迎刃而解。 resultType 和 resultMap 之间只能同时使用一个。                                         |
| `flushCache`    | 将其设置为 true 后，只要语句被调用，都会导致本地缓存和二级缓存被清空，默认值：false。                                                                                                                     |
| `useCache`      | 将其设置为 true 后，将会导致本条语句的结果被二级缓存缓存起来，默认值：对 select 元素为 true。                                                                                                             |
| `timeout`       | 这个设置是在抛出异常之前，驱动程序等待数据库返回请求结果的秒数。默认值为未设置（unset）（依赖数据库驱动）。                                                                                               |
| `fetchSize`     | 这是一个给驱动的建议值，尝试让驱动程序每次批量返回的结果行数等于这个设置值。 默认值为未设置（unset）（依赖驱动）。                                                                                        |
| `statementType` | 可选 STATEMENT，PREPARED 或 CALLABLE。这会让 MyBatis 分别使用 Statement，PreparedStatement 或 CallableStatement，默认值：PREPARED。                                                                       |
| `resultSetType` | FORWARD_ONLY，SCROLL_SENSITIVE, SCROLL_INSENSITIVE 或 DEFAULT（等价于 unset） 中的一个，默认值为 unset （依赖数据库驱动）。                                                                               |
| `databaseId`    | 如果配置了数据库厂商标识（databaseIdProvider），MyBatis 会加载所有不带 databaseId 或匹配当前 databaseId 的语句；如果带和不带的语句都有，则不带的会被忽略。                                                |
| `resultOrdered` | 这个设置仅针对嵌套结果 select 语句：如果为 true，将会假设包含了嵌套结果集或是分组，当返回一个主结果行时，就不会产生对前面结果集的引用。 这就使得在获取嵌套结果集的时候不至于内存不够用。默认值：`false`。 |
| `resultSets`    | 这个设置仅适用于多结果集的情况。它将列出语句执行后返回的结果集并赋予每个结果集一个名称，多个名称之间以逗号分隔。                                                                                          |

### insert,update,delete

| 属性               | 描述                                                                                                                                                                                                                      |
| :----------------- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `id`               | 在命名空间中唯一的标识符，可以被用来引用这条语句。                                                                                                                                                                        |
| `parameterType`    | 将会传入这条语句的参数的类全限定名或别名。这个属性是可选的，因为 MyBatis 可以通过类型处理器（TypeHandler）推断出具体传入语句的参数，默认值为未设置（unset）。                                                             |
| `parameterMap`     | 用于引用外部 parameterMap 的属性，目前已被废弃。请使用行内参数映射和 parameterType 属性。                                                                                                                                 |
| `flushCache`       | 将其设置为 true 后，只要语句被调用，都会导致本地缓存和二级缓存被清空，默认值：（对 insert、update 和 delete 语句）true。                                                                                                  |
| `timeout`          | 这个设置是在抛出异常之前，驱动程序等待数据库返回请求结果的秒数。默认值为未设置（unset）（依赖数据库驱动）。                                                                                                               |
| `statementType`    | 可选 STATEMENT，PREPARED 或 CALLABLE。这会让 MyBatis 分别使用 Statement，PreparedStatement 或 CallableStatement，默认值：PREPARED。                                                                                       |
| `useGeneratedKeys` | （仅适用于 insert 和 update）这会令 MyBatis 使用 JDBC 的 getGeneratedKeys 方法来取出由数据库内部生成的主键（比如：像 MySQL 和 SQL Server 这样的关系型数据库管理系统的自动递增字段），默认值：false。                      |
| `keyProperty`      | （仅适用于 insert 和 update）指定能够唯一识别对象的属性，MyBatis 会使用 getGeneratedKeys 的返回值或 insert 语句的 selectKey 子元素设置它的值，默认值：未设置（`unset`）。如果生成列不止一个，可以用逗号分隔多个属性名称。 |
| `keyColumn`        | （仅适用于 insert 和 update）设置生成键值在表中的列名，在某些数据库（像 PostgreSQL）中，当主键列不是表中的第一列的时候，是必须设置的。如果生成列不止一个，可以用逗号分隔多个属性名称。                                    |
| `databaseId`       | 如果配置了数据库厂商标识（databaseIdProvider），MyBatis 会加载所有不带 databaseId 或匹配当前 databaseId 的语句；如果带和不带的语句都有，则不带的会被忽略。                                                                |

### 使用 selectKey

```
<insert id="insertAuthor">
  <selectKey keyProperty="id" resultType="int" order="BEFORE">
    select CAST(RANDOM()*1000000 as INTEGER) a from SYSIBM.SYSDUMMY1
  </selectKey>
  insert into Author
    (id, username, password, email,bio, favourite_section)
  values
    (#{id}, #{username}, #{password}, #{email}, #{bio}, #{favouriteSection,jdbcType=VARCHAR})
</insert>

<selectKey
  keyProperty="id"
  resultType="int"
  order="BEFORE"
  statementType="PREPARED">
```

### 使用 sql 片段

```
定义
<sql id="userColumns"> ${alias}.id,${alias}.username,${alias}.password </sql>

使用
<select id="select" resultType="map">
  select
    field1, field2, field3
  <include refid="someinclude">
    <property name="prefix" value="Some"/>
    <property name="include_target" value="sometable"/>
  </include>
</select>

```

### 参数

大多时候，你只须简单指定属性名，顶多要为可能为空的列指定 `jdbcType`，其他的事情交给 MyBatis 自己去推断就行了。

```
简单类型的参数引用
#{id}
指定java与jdbc类型
#{property,javaType=int,jdbcType=NUMERIC}
指定类型处理器
#{age,javaType=int,jdbcType=NUMERIC,typeHandler=MyTypeHandler}
数值类型使用精度
#{height,javaType=double,jdbcType=NUMERIC,numericScale=2}
当参数模式时 out时 可能会修改
#{department, mode=OUT, jdbcType=CURSOR, javaType=ResultSet, resultMap=departmentResultMap}
jdbcType为游标时
#{department, mode=OUT, jdbcType=CURSOR, javaType=ResultSet, resultMap=departmentResultMap}

```

### 字符串替换

- 使用 `#{}` 参数语法时，MyBatis 会创建 `PreparedStatement` 参数占位符

```
ORDER BY ${columnName}

@Select("select * from user where ${column} = #{value}")
User findByColumn(@Param("column") String column, @Param("value") String value);

```

### 结果映射（resultMap）

#### resultMap 的子元素

- constructor

  \- 用于在实例化类时，注入结果到构造方法中

  - `idArg` - ID 参数；标记出作为 ID 的结果可以帮助提高整体性能
  - `arg` - 将被注入到构造方法的一个普通结果

- `id` – 一个 ID 结果；标记出作为 ID 的结果可以帮助提高整体性能

- `result` – 注入到字段或 JavaBean 属性的普通结果

- association– 一个复杂类型的关联；许多结果将包装成这种类型

  - 嵌套结果映射 – 关联可以是 `resultMap` 元素，或是对其它结果映射的引用

- collection– 一个复杂类型的集合

  - 嵌套结果映射 – 集合可以是 `resultMap` 元素，或是对其它结果映射的引用

- discriminator– 使用结果值来决定使用哪个 resultMap

  - case– 基于某些值的结果映射
  - 嵌套结果映射 – `case` 也是一个结果映射，因此具有相同的结构和元素；或者引用其它的结果映射

#### resultMap 属性

| 属性          | 描述                                                                                                                                   |
| :------------ | :------------------------------------------------------------------------------------------------------------------------------------- |
| `id`          | 当前命名空间中的一个唯一标识，用于标识一个结果映射。                                                                                   |
| `type`        | 类的完全限定名, 或者一个类型别名（关于内置的类型别名，可以参考上面的表格）。                                                           |
| `autoMapping` | 如果设置这个属性，MyBatis 将会为本结果映射开启或者关闭自动映射。 这个属性会覆盖全局的属性 autoMappingBehavior。默认值：未设置（unset） |

#### id & result 元素

- 数据库字段名,字段类型
- Java 属性名,属性类型
- 类型处理器

#### 支持的 JDBC 类型

| `BIT`      | `FLOAT`   | `CHAR`        | `TIMESTAMP`     | `OTHER`   | `UNDEFINED` |
| ---------- | --------- | ------------- | --------------- | --------- | ----------- |
| `TINYINT`  | `REAL`    | `VARCHAR`     | `BINARY`        | `BLOB`    | `NVARCHAR`  |
| `SMALLINT` | `DOUBLE`  | `LONGVARCHAR` | `VARBINARY`     | `CLOB`    | `NCHAR`     |
| `INTEGER`  | `NUMERIC` | `DATE`        | `LONGVARBINARY` | `BOOLEAN` | `NCLOB`     |
| `BIGINT`   | `DECIMAL` | `TIME`        | `NULL`          | `CURSOR`  | `ARRAY`     |

#### constructor

```
<constructor>
   <idArg column="id" javaType="int"/>
   <arg column="username" javaType="String"/>
   <arg column="age" javaType="_int"/>
</constructor>
```

从版本 3.4.3 开始，可以在指定参数名称的前提下，以任意顺序编写 arg 元素。

为了通过名称来引用构造方法参数，你可以添加 `@Param` 注解，或者使用 '-parameters' 编译选项并启用 `useActualParamName` 选项（默认开启）来编译项目

constructor 的属性

| 属性          | 描述                                                                                                                                                                                                                                                                                                                                                           |
| :------------ | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `column`      | 数据库中的列名，或者是列的别名。一般情况下，这和传递给 `resultSet.getString(columnName)` 方法的参数一样。                                                                                                                                                                                                                                                      |
| `javaType`    | 一个 Java 类的完全限定名，或一个类型别名（关于内置的类型别名，可以参考上面的表格）。 如果你映射到一个 JavaBean，MyBatis 通常可以推断类型。然而，如果你映射到的是 HashMap，那么你应该明确地指定 javaType 来保证行为与期望的相一致。                                                                                                                             |
| `jdbcType`    | JDBC 类型，所支持的 JDBC 类型参见这个表格之前的“支持的 JDBC 类型”。 只需要在可能执行插入、更新和删除的且允许空值的列上指定 JDBC 类型。这是 JDBC 的要求而非 MyBatis 的要求。如果你直接面向 JDBC 编程，你需要对可能存在空值的列指定这个类型。                                                                                                                    |
| `typeHandler` | 我们在前面讨论过默认的类型处理器。使用这个属性，你可以覆盖默认的类型处理器。 这个属性值是一个类型处理器实现类的完全限定名，或者是类型别名。                                                                                                                                                                                                                    |
| `select`      | 用于加载复杂类型属性的映射语句的 ID，它会从 column 属性中指定的列检索数据，作为参数传递给此 select 语句。具体请参考关联元素。                                                                                                                                                                                                                                  |
| `resultMap`   | 结果映射的 ID，可以将嵌套的结果集映射到一个合适的对象树中。 它可以作为使用额外 select 语句的替代方案。它可以将多表连接操作的结果映射成一个单一的 `ResultSet`。这样的 `ResultSet` 将会将包含重复或部分数据重复的结果集。为了将结果集正确地映射到嵌套的对象树中，MyBatis 允许你 “串联”结果映射，以便解决嵌套结果集的问题。想了解更多内容，请参考下面的关联元素。 |
| `name`        | 构造方法形参的名字。从 3.4.3 版本开始，通过指定具体的参数名，你可以以任意顺序写入 arg 元素。参看上面的解释。                                                                                                                                                                                                                                                   |

#### 关联

MyBatis 有两种不同的方式加载关联：

- 嵌套 Select 查询：通过执行另外一个 SQL 映射语句来加载期望的复杂类型。
- 嵌套结果映射：使用嵌套的结果映射来处理连接结果的重复子集。

#### 关联的嵌套 Select 查询的

| 属性        | 描述                                                                                                                                                                                                                                                                                                                                             |
| :---------- | :----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `column`    | 数据库中的列名，或者是列的别名。一般情况下，这和传递给 `resultSet.getString(columnName)` 方法的参数一样。 注意：在使用复合主键的时候，你可以使用 `column="{prop1=col1,prop2=col2}"` 这样的语法来指定多个传递给嵌套 Select 查询语句的列名。这会使得 `prop1` 和 `prop2` 作为参数对象，被设置为对应嵌套 Select 语句的参数。                         |
| `select`    | 用于加载复杂类型属性的映射语句的 ID，它会从 column 属性指定的列中检索数据，作为参数传递给目标 select 语句。 具体请参考下面的例子。注意：在使用复合主键的时候，你可以使用 `column="{prop1=col1,prop2=col2}"` 这样的语法来指定多个传递给嵌套 Select 查询语句的列名。这会使得 `prop1` 和 `prop2` 作为参数对象，被设置为对应嵌套 Select 语句的参数。 |
| `fetchType` | 可选的。有效值为 `lazy` 和 `eager`。 指定属性后，将在映射中忽略全局配置参数 `lazyLoadingEnabled`，使用属性的值。                                                                                                                                                                                                                                 |

```
<resultMap id="blogResult" type="Blog">
  <association property="author" column="author_id" javaType="Author" select="selectAuthor"/>
</resultMap>

<select id="selectBlog" resultMap="blogResult">
  SELECT * FROM BLOG WHERE ID = #{id}
</select>

<select id="selectAuthor" resultType="Author">
  SELECT * FROM AUTHOR WHERE ID = #{id}
</select>
```

#### 关联的嵌套结果映射

| 属性            | 描述                                                                                                                                                                                                                                                                                                                                               |
| :-------------- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `resultMap`     | 结果映射的 ID，可以将此关联的嵌套结果集映射到一个合适的对象树中。 它可以作为使用额外 select 语句的替代方案。它可以将多表连接操作的结果映射成一个单一的 `ResultSet`。这样的 `ResultSet` 有部分数据是重复的。 为了将结果集正确地映射到嵌套的对象树中, MyBatis 允许你“串联”结果映射，以便解决嵌套结果集的问题。使用嵌套结果映射的一个例子在表格以后。 |
| `columnPrefix`  | 当连接多个表时，你可能会不得不使用列别名来避免在 `ResultSet` 中产生重复的列名。指定 columnPrefix 列名前缀允许你将带有这些前缀的列映射到一个外部的结果映射中。 详细说明请参考后面的例子。                                                                                                                                                           |
| `notNullColumn` | 默认情况下，在至少一个被映射到属性的列不为空时，子对象才会被创建。 你可以在这个属性上指定非空的列来改变默认行为，指定后，Mybatis 将只在这些列非空时才创建一个子对象。可以使用逗号分隔来指定多个列。默认值：未设置（unset）。                                                                                                                       |
| `autoMapping`   | 如果设置这个属性，MyBatis 将会为本结果映射开启或者关闭自动映射。 这个属性会覆盖全局的属性 autoMappingBehavior。注意，本属性对外部的结果映射无效，所以不能搭配 `select` 或 `resultMap` 元素使用。默认值：未设置（unset）。                                                                                                                          |

#### 关联的多结果集（ResultSet）

```
SELECT * FROM BLOG WHERE ID = #{id}

SELECT * FROM AUTHOR WHERE ID = #{id}

<select id="selectBlog" resultSets="blogs,authors" resultMap="blogResult" statementType="CALLABLE">
  {call getBlogsAndAuthors(#{id,jdbcType=INTEGER,mode=IN})}
</select>

<resultMap id="blogResult" type="Blog">
  <id property="id" column="id" />
  <result property="title" column="title"/>
  <association property="author" javaType="Author" resultSet="authors" column="author_id" foreignColumn="id">
    <id property="id" column="id"/>
    <result property="username" column="username"/>
    <result property="password" column="password"/>
    <result property="email" column="email"/>
    <result property="bio" column="bio"/>
  </association>
</resultMap>
```

存储过程执行下面的查询并返回两个结果集

#### 集合嵌套 select 查询

```
<collection property="posts" ofType="domain.blog.Post">
  <id property="id" column="post_id"/>
  <result property="subject" column="post_subject"/>
  <result property="body" column="post_body"/>
</collection>

<resultMap id="blogResult" type="Blog">
  <collection property="posts" javaType="ArrayList" column="id" ofType="Post" select="selectPostsForBlog"/>
</resultMap>
<select id="selectBlog" resultMap="blogResult">
  SELECT * FROM BLOG WHERE ID = #{id}
</select>
<select id="selectPostsForBlog" resultType="Post">
  SELECT * FROM POST WHERE BLOG_ID = #{id}
</select>
```

#### 集合的嵌套结果映射

```
<resultMap id="blogResult" type="Blog">
  <id property="id" column="blog_id" />
  <result property="title" column="blog_title"/>
  <collection property="posts" ofType="Post">
    <id property="id" column="post_id"/>
    <result property="subject" column="post_subject"/>
    <result property="body" column="post_body"/>
  </collection>
</resultMap>
```

#### 集合的多结果集（ResultSet）

```
<resultMap id="blogResult" type="Blog">
  <id property="id" column="id" />
  <result property="title" column="title"/>
  <collection property="posts" ofType="Post" resultSet="posts" column="id" foreignColumn="blog_id">
    <id property="id" column="id"/>
    <result property="subject" column="subject"/>
    <result property="body" column="body"/>
  </collection>
</resultMap>
```

#### 鉴别器

```
<discriminator javaType="int" column="draft">
  <case value="1" resultType="DraftPost"/>
</discriminator>
```

- 一个数据库查询可能会返回多个不同的结果集

- 一个鉴别器的定义需要指定 column 和 javaType 属性。

- column 指定了 MyBatis 查询被比较值的地方。 而 javaType 用来确保使用正确的相等测试
- 用什么字段 怎么测

```
<resultMap id="vehicleResult" type="Vehicle">
  <id property="id" column="id" />
  <result property="vin" column="vin"/>
  <result property="year" column="year"/>
  <result property="make" column="make"/>
  <result property="model" column="model"/>
  <result property="color" column="color"/>
  <discriminator javaType="int" column="vehicle_type">
    <case value="1" resultMap="carResult"/>
    <case value="2" resultMap="truckResult"/>
    <case value="3" resultMap="vanResult"/>
    <case value="4" resultMap="suvResult"/>
  </discriminator>
</resultMap>
```

#### 自动映射

1. MyBatis 会获取结果中返回的列名并在 Java 类中查找相同名字的属性（忽略大小写）
2. 驼峰命名 与 下划线命名 转换 需要将 `mapUnderscoreToCamelCase` 设置为 true。
3. 对于每一个结果映射，在 ResultSet 出现的列，如果没有设置手动映射，将被自动映射。在自动映射处理完毕后，再处理手动映射

有三种自动映射等级：

- `NONE` - 禁用自动映射。仅对手动映射的属性进行映射。
- `PARTIAL` - 对除在内部定义了嵌套结果映射（也就是连接的属性）以外的属性进行映射
- `FULL` - 自动映射所有属性。

### 缓存

- 默认情况下，只启用了本地的会话缓存，它仅仅对一个会话中的数据进行缓存
- 要启用全局的二级缓存，只需要在你的 SQL 映射文件中添加一行：`<cache/>`

#### 缓存规则

- 映射语句文件中的所有 select 语句的结果将会被缓存。

- 映射语句文件中的所有 insert、update 和 delete 语句会刷新缓存。

- 缓存会使用最近最少使用算法（LRU, Least Recently Used）算法来清除不需要的缓存。

- 缓存不会定时进行刷新（也就是说，没有刷新间隔）。

- 缓存会保存列表或对象（无论查询方法返回哪种）的 1024 个引用。

- 缓存会被视为读/写缓存，这意味着获取到的对象并不是共享的，可以安全地被调用者修改，而不干扰其他调用者或线程所做的潜在修改。

  ```
  <cache
    eviction="FIFO"
    flushInterval="60000"
    size="512"
    readOnly="true"/>
  ```

#### 清理策略

- `LRU` – 最近最少使用：移除最长时间不被使用的对象。
- `FIFO` – 先进先出：按对象进入缓存的顺序来移除它们。
- `SOFT` – 软引用：基于垃圾回收器状态和软引用规则移除对象。
- `WEAK` – 弱引用：更积极地基于垃圾收集器状态和弱引用规则移除对象。

二级缓存是事务性的。这意味着，当 SqlSession 完成并提交时，或是完成并回滚，但没有执行 flushCache=true 的 insert/delete/update 语句时，缓存会获得更新。

#### 使用自定义缓存

```
<cache type="com.domain.something.MyCustomCache"/>

public interface Cache {
  String getId();
  int getSize();
  void putObject(Object key, Object value);
  Object getObject(Object key);
  boolean hasKey(Object key);
  Object removeObject(Object key);
  void clear();
}

<cache type="com.domain.something.MyCustomCache">
  <property name="cacheFile" value="/tmp/my-custom-cache.tmp"/>
</cache>
```

注意事项

- 缓存的配置和缓存实例会被绑定到 SQL 映射文件的命名空间中
- 同一命名空间中的所有语句和缓存将通过命名空间绑定在一起
- 每条语句可以自定义与缓存交互的方式
- 共享不同命名空间缓存

```
<cache-ref namespace="com.someone.application.data.SomeMapper"/>
```

```
<select ... flushCache="false" useCache="true"/>
```

## 动态 sql

[动态 sql](mybatis标签.md)

## sql 语句 javaAPI 构建

### 示例

#### 示例 1

```
private String selectPersonSql() {
  return new SQL() {{
    SELECT("P.ID, P.USERNAME, P.PASSWORD, P.FULL_NAME");
    SELECT("P.LAST_NAME, P.CREATED_ON, P.UPDATED_ON");
    FROM("PERSON P");
    FROM("ACCOUNT A");
    INNER_JOIN("DEPARTMENT D on D.ID = P.DEPARTMENT_ID");
    INNER_JOIN("COMPANY C on D.COMPANY_ID = C.ID");
    WHERE("P.ID = A.ID");
    WHERE("P.FIRST_NAME like ?");
    OR();
    WHERE("P.LAST_NAME like ?");
    GROUP_BY("P.ID");
    HAVING("P.LAST_NAME like ?");
    OR();
    HAVING("P.FIRST_NAME like ?");
    ORDER_BY("P.ID");
    ORDER_BY("P.FULL_NAME");
  }}.toString();
}
```

#### 示例 2

```
// 匿名内部类风格
public String deletePersonSql() {
  return new SQL() {{
    DELETE_FROM("PERSON");
    WHERE("ID = #{id}");
  }}.toString();
}

// Builder / Fluent 风格
public String insertPersonSql() {
  String sql = new SQL()
    .INSERT_INTO("PERSON")
    .VALUES("ID, FIRST_NAME", "#{id}, #{firstName}")
    .VALUES("LAST_NAME", "#{lastName}")
    .toString();
  return sql;
}

// 动态条件（注意参数需要使用 final 修饰，以便匿名内部类对它们进行访问）
public String selectPersonLike(final String id, final String firstName, final String lastName) {
  return new SQL() {{
    SELECT("P.ID, P.USERNAME, P.PASSWORD, P.FIRST_NAME, P.LAST_NAME");
    FROM("PERSON P");
    if (id != null) {
      WHERE("P.ID like #{id}");
    }
    if (firstName != null) {
      WHERE("P.FIRST_NAME like #{firstName}");
    }
    if (lastName != null) {
      WHERE("P.LAST_NAME like #{lastName}");
    }
    ORDER_BY("P.LAST_NAME");
  }}.toString();
}

public String deletePersonSql() {
  return new SQL() {{
    DELETE_FROM("PERSON");
    WHERE("ID = #{id}");
  }}.toString();
}

public String insertPersonSql() {
  return new SQL() {{
    INSERT_INTO("PERSON");
    VALUES("ID, FIRST_NAME", "#{id}, #{firstName}");
    VALUES("LAST_NAME", "#{lastName}");
  }}.toString();
}

public String updatePersonSql() {
  return new SQL() {{
    UPDATE("PERSON");
    SET("FIRST_NAME = #{firstName}");
    WHERE("ID = #{id}");
  }}.toString();
}
```

### 构建 sql 的 API

| 方法                                                                                                                                                                                                                             | 描述                                                                                                                                                                                                       |
| -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | :--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| ` SELECT(String)``SELECT(String...) `                                                                                                                                                                                            | 开始新的或追加到已有的 `SELECT`子句。可以被多次调用，参数会被追加到 `SELECT` 子句。 参数通常使用逗号分隔的列名和别名列表，但也可以是数据库驱动程序接受的任意参数。                                         |
| `SELECT_DISTINCT(String)`<br />`SELECT_DISTINCT(String...)`                                                                                                                                                                      | 开始新的或追加到已有的 `SELECT`子句，并添加 `DISTINCT` 关键字到生成的查询中。可以被多次调用，参数会被追加到 `SELECT` 子句。 参数通常使用逗号分隔的列名和别名列表，但也可以是数据库驱动程序接受的任意参数。 |
| ` FROM(String)``FROM(String...) `                                                                                                                                                                                                | 开始新的或追加到已有的 `FROM`子句。可以被多次调用，参数会被追加到 `FROM`子句。 参数通常是一个表名或别名，也可以是数据库驱动程序接受的任意参数。                                                            |
| `JOIN(String)`<br />`JOIN(String...)`<br />`INNER_JOIN(String)`<br />`INNER_JOIN(String...)`<br />`LEFT_OUTER_JOIN(String)`<br />`LEFT_OUTER_JOIN(String...)`<br />`RIGHT_OUTER_JOIN(String)`<br />`RIGHT_OUTER_JOIN(String...)` | 基于调用的方法，添加新的合适类型的 `JOIN` 子句。 参数可以包含一个由列和连接条件构成的标准连接。                                                                                                            |
| `WHERE(String)`<br />`WHERE(String...)`                                                                                                                                                                                          | 插入新的 `WHERE` 子句条件，并使用 `AND` 拼接。可以被多次调用，对于每一次调用产生的新条件，会使用 `AND` 拼接起来。要使用 `OR` 分隔，请使用 `OR()`。                                                         |
| `OR()`                                                                                                                                                                                                                           | 使用 `OR` 来分隔当前的 `WHERE` 子句条件。 可以被多次调用，但在一行中多次调用会生成错误的 `SQL`。                                                                                                           |
| `AND()`                                                                                                                                                                                                                          | 使用 `AND` 来分隔当前的 `WHERE`子句条件。 可以被多次调用，但在一行中多次调用会生成错误的 `SQL`。由于 `WHERE` 和 `HAVING`都会自动使用 `AND` 拼接, 因此这个方法并不常用，只是为了完整性才被定义出来。        |
| `GROUP_BY(String)`<br />`GROUP_BY(String...)`                                                                                                                                                                                    | 追加新的 `GROUP BY` 子句，使用逗号拼接。可以被多次调用，每次调用都会使用逗号将新的条件拼接起来。                                                                                                           |
| `HAVING(String)`<br />`HAVING(String...)`                                                                                                                                                                                        | 追加新的 `HAVING` 子句。使用 AND 拼接。可以被多次调用，每次调用都使用`AND`来拼接新的条件。要使用 `OR` 分隔，请使用 `OR()`。                                                                                |
| `ORDER_BY(String)`<br />`ORDER_BY(String...)`                                                                                                                                                                                    | 追加新的 `ORDER BY` 子句，使用逗号拼接。可以多次被调用，每次调用会使用逗号拼接新的条件。                                                                                                                   |
| ` LIMIT(String)``LIMIT(int) `                                                                                                                                                                                                    | 追加新的 `LIMIT` 子句。 仅在 SELECT()、UPDATE()、DELETE() 时有效。 当在 SELECT() 中使用时，应该配合 OFFSET() 使用。（于 3.5.2 引入）                                                                       |
| ` OFFSET(String)``OFFSET(long) `                                                                                                                                                                                                 | 追加新的 `OFFSET` 子句。 仅在 SELECT() 时有效。 当在 SELECT() 时使用时，应该配合 LIMIT() 使用。（于 3.5.2 引入）                                                                                           |
| `OFFSET_ROWS(String)`<br />`OFFSET_ROWS(long)`                                                                                                                                                                                   | 追加新的 `OFFSET n ROWS` 子句。 仅在 SELECT() 时有效。 该方法应该配合 FETCH_FIRST_ROWS_ONLY() 使用。（于 3.5.2 加入）                                                                                      |
| `FETCH_FIRST_ROWS_ONLY(String)`<br />`FETCH_FIRST_ROWS_ONLY(int)`                                                                                                                                                                | 追加新的 `FETCH FIRST n ROWS ONLY` 子句。 仅在 SELECT() 时有效。 该方法应该配合 OFFSET_ROWS() 使用。（于 3.5.2 加入）                                                                                      |
| `DELETE_FROM(String)`                                                                                                                                                                                                            | 开始新的 delete 语句，并指定删除表的表名。通常它后面都会跟着一个 WHERE 子句！                                                                                                                              |
| `INSERT_INTO(String)`                                                                                                                                                                                                            | 开始新的 insert 语句，并指定插入数据表的表名。后面应该会跟着一个或多个 VALUES() 调用，或 INTO_COLUMNS() 和 INTO_VALUES() 调用。                                                                            |
| ` SET(String)``SET(String...) `                                                                                                                                                                                                  | 对 update 语句追加 "set" 属性的列表                                                                                                                                                                        |
| `UPDATE(String)`                                                                                                                                                                                                                 | 开始新的 update 语句，并指定更新表的表名。后面都会跟着一个或多个 SET() 调用，通常也会有一个 WHERE() 调用。                                                                                                 |
| `VALUES(String, String)`                                                                                                                                                                                                         | 追加数据值到 insert 语句中。第一个参数是数据插入的列名，第二个参数则是数据值。                                                                                                                             |
| `INTO_COLUMNS(String...)`                                                                                                                                                                                                        | 追加插入列子句到 insert 语句中。应与 INTO_VALUES() 一同使用。                                                                                                                                              |
| `INTO_VALUES(String...)`                                                                                                                                                                                                         | 追加插入值子句到 insert 语句中。应与 INTO_COLUMNS() 一同使用。                                                                                                                                             |
| `ADD_ROW()`                                                                                                                                                                                                                      | 添加新的一行数据，以便执行批量插入。（于 3.5.2 引入）                                                                                                                                                      |

从版本 3.4.2 开始，你可以像下面这样使用可变长度参数：

```java
public String selectPersonSql() {
  return new SQL()
    .SELECT("P.ID", "A.USERNAME", "A.PASSWORD", "P.FULL_NAME", "D.DEPARTMENT_NAME", "C.COMPANY_NAME")
    .FROM("PERSON P", "ACCOUNT A")
    .INNER_JOIN("DEPARTMENT D on D.ID = P.DEPARTMENT_ID", "COMPANY C on D.COMPANY_ID = C.ID")
    .WHERE("P.ID = A.ID", "P.FULL_NAME like #{name}")
    .ORDER_BY("P.ID", "P.FULL_NAME")
    .toString();
}
public String updatePersonSql() {
  return new SQL().UPDATE("PERSON").SET("FULL_NAME = #{fullName}", "DATE_OF_BIRTH = #{dateOfBirth}").WHERE("ID = #{id}").toString();
}
public String insertPersonSql() {
  return new SQL()
    .INSERT_INTO("PERSON")
    .INTO_COLUMNS("ID", "FULL_NAME")
    .INTO_VALUES("#{id}", "#{fullName}")
    .toString();
}

```

`>3.5.2`构建批量插入语句：

```java
public String insertPersonsSql() {
  // INSERT INTO PERSON (ID, FULL_NAME)
  //     VALUES (#{mainPerson.id}, #{mainPerson.fullName}) , (#{subPerson.id}, #{subPerson.fullName})
  return new SQL()
    .INSERT_INTO("PERSON")
    .INTO_COLUMNS("ID", "FULL_NAME")
    .INTO_VALUES("#{mainPerson.id}", "#{mainPerson.fullName}")
    .ADD_ROW()
    .INTO_VALUES("#{subPerson.id}", "#{subPerson.fullName}")
    .toString();
}
```

`>3.5.2`限制返回结果数的 SELECT 语句

```java
public String selectPersonsWithOffsetLimitSql() {
  // SELECT id, name FROM PERSON
  //     LIMIT #{limit} OFFSET #{offset}
  return new SQL()
    .SELECT("id", "name")
    .FROM("PERSON")
    .LIMIT("#{limit}")
    .OFFSET("#{offset}")
    .toString();
}

public String selectPersonsWithFetchFirstSql() {
  // SELECT id, name FROM PERSON
  //     OFFSET #{offset} ROWS FETCH FIRST #{limit} ROWS ONLY
  return new SQL()
    .SELECT("id", "name")
    .FROM("PERSON")
    .OFFSET_ROWS("#{offset}")
    .FETCH_FIRST_ROWS_ONLY("#{limit}")
    .toString();
}
```

### 基于注解的 sql 开发

#### 读取数据库自动生成的主键

```
@Insert("insert into table3 (id, name) values(#{nameId}, #{name})")
@SelectKey(statement="call next value for TestSequence", keyProperty="nameId", before=true, resultType=int.class)
int insertTable3(Name name);
```

```
@Insert("insert into table2 (name) values(#{name})")
@SelectKey(statement="call identity()", keyProperty="nameId", before=false, resultType=int.class)
int insertTable2(Name name);
```

#### 自动刷新

```
@Flush
List<BatchResult> flush();
```

#### 命名结果集

```java
@Results(id = "userResult", value = {
  @Result(property = "id", column = "uid", id = true),
  @Result(property = "firstName", column = "first_name"),
  @Result(property = "lastName", column = "last_name")
})
@Select("select * from users where id = #{id}")
User getUserById(Integer id);

@Results(id = "companyResults")
@ConstructorArgs({
  @Arg(column = "cid", javaType = Integer.class, id = true),
  @Arg(column = "name", javaType = String.class)
})
@Select("select * from company where id = #{id}")
Company getCompanyById(Integer id);
```

#### 使用 sqlprovider

```java
@SelectProvider(type = UserSqlBuilder.class, method = "buildGetUsersByName")
List<User> getUsersByName(String name);

class UserSqlBuilder {
  public static String buildGetUsersByName(final String name) {
    return new SQL(){{
      SELECT("*");
      FROM("users");
      if (name != null) {
        WHERE("name like #{value} || '%'");
      }
      ORDER_BY("id");
    }}.toString();
  }
}
```

```java
@SelectProvider(type = UserSqlBuilder.class, method = "buildGetUsersByName")
List<User> getUsersByName(String name);

class UserSqlBuilder {
  public static String buildGetUsersByName(final String name) {
    return new SQL(){{
      SELECT("*");
      FROM("users");
      if (name != null) {
        WHERE("name like #{value} || '%'");
      }
      ORDER_BY("id");
    }}.toString();
  }
}
```

```java
@SelectProvider(type = UserSqlBuilder.class, method = "buildGetUsersByName")
List<User> getUsersByName(
    @Param("name") String name, @Param("orderByColumn") String orderByColumn);

class UserSqlBuilder {

  // 如果不使用 @Param，就应该定义与 mapper 方法相同的参数
  public static String buildGetUsersByName(
      final String name, final String orderByColumn) {
    return new SQL(){{
      SELECT("*");
      FROM("users");
      WHERE("name like #{name} || '%'");
      ORDER_BY(orderByColumn);
    }}.toString();
  }

  // 如果使用 @Param，就可以只定义需要使用的参数
  public static String buildGetUsersByName(@Param("orderByColumn") final String orderByColumn) {
    return new SQL(){{
      SELECT("*");
      FROM("users");
      WHERE("name like #{name} || '%'");
      ORDER_BY(orderByColumn);
    }}.toString();
  }
}
```

以下例子展示了 `ProviderMethodResolver`（3.5.1 后可用）的默认实现使用方法：

```java
@SelectProvider(UserSqlProvider.class)
List<User> getUsersByName(String name);

// 在你的 provider 类中实现 ProviderMethodResolver 接口
class UserSqlProvider implements ProviderMethodResolver {
  // 默认实现中，会将映射器方法的调用解析到实现的同名方法上
  public static String getUsersByName(final String name) {
    return new SQL(){{
      SELECT("*");
      FROM("users");
      if (name != null) {
        WHERE("name like #{value} || '%'");
      }
      ORDER_BY("id");
    }}.toString();
  }
}
```

```java
@Select(value = "SELECT SYS_GUID() FROM dual", databaseId = "oracle") // Use this statement if DatabaseIdProvider provide "oracle"
@Select(value = "SELECT uuid_generate_v4()", databaseId = "postgres") // Use this statement if DatabaseIdProvider provide "postgres"
@Select("SELECT RANDOM_UUID()") // Use this statement if the DatabaseIdProvider not configured or not matches databaseId
String generateId();
```

## 日志

### 自动查找实现

Mybatis 通过使用内置的日志工厂提供日志功能,内置日志工厂将会把日志工作委托给下面的实现之一

- SLF4J
- Apache Commons Logging
- Log4j 2
- Log4j
- JDK logging

MyBatis 内置日志工厂会基于运行时检测信息选择日志委托实现。它会（按上面罗列的顺序）使用第一个查找到的实现。当没有找到这些实现时，将会禁用日志功能

### 手动配置

```xml
<configuration>
  <settings>
    ...
    <setting name="logImpl" value="LOG4J"/>
    ...
  </settings>
</configuration>

```

可选的值有：SLF4J、LOG4J、LOG4J2、JDK_LOGGING、COMMONS_LOGGING、STDOUT_LOGGING、NO_LOGGING，或者是实现了 `org.apache.ibatis.logging.Log` 接口，且构造方法以字符串为参数的类完全限定名。

你也可以调用以下任一方法来选择日志实现：

```properties
org.apache.ibatis.logging.LogFactory.useSlf4jLogging();
org.apache.ibatis.logging.LogFactory.useLog4JLogging();
org.apache.ibatis.logging.LogFactory.useJdkLogging();
org.apache.ibatis.logging.LogFactory.useCommonsLogging();
org.apache.ibatis.logging.LogFactory.useStdOutLogging();
```

### 日志控制

只打印特定语句的日志。以下配置将只打印语句 `selectBlog` 的日志

```properties
log4j.logger.org.mybatis.example.BlogMapper.selectBlog=TRACE
log4j.logger.org.mybatis.example=TRACE
log4j.logger.org.mybatis.example=DEBUG -- 只查看sql语句 不查看返回结果集
```

而要记录具体语句的日志，可以这样做：

```
log4j.logger.org.mybatis.example.BlogMapper.selectBlog=TRACE
```

<!-- endtoc -->

{% endraw %}

