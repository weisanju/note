# Overview

Log4j 2 API 提供应用程序应编码到的接口，并提供实现者创建日志记录实现所需的适配器组件。

尽管 Log4j 2 在 API 和实现之间被分解，但这样做的主要目的不是允许多个实现，尽管这当然是可能的，

**但要明确定义在“正常”应用程序代码中可以安全使用哪些类和方法**

## Hello World!

```java
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
 
public class HelloWorld {
    private static final Logger logger = LogManager.getLogger("HelloWorld");
    public static void main(String[] args) {
        logger.info("Hello, World!");
    }
}
```

## Substituting Parameters

```java
logger.debug("Logging in user {} with birthday {}", user.getName(), user.getBirthdayCalendar());
```

## Formatting Parameters

如果 toString() 不是您想要的，格式化程序记录器将格式化由您决定。

为了便于格式化，您可以使用与 Java 的 Formatter 相同的格式字符串。

```java
public static Logger logger = LogManager.getFormatterLogger("Foo");
 
logger.debug("Logging in user %s with birthday %s", user.getName(), user.getBirthdayCalendar());
logger.debug("Logging in user %1$s with birthday %2$tm %2$te,%2$tY", user.getName(), user.getBirthdayCalendar());
logger.debug("Integer.MAX_VALUE = %,d", Integer.MAX_VALUE);
logger.debug("Long.MAX_VALUE = %,d", Long.MAX_VALUE);
```

要使用格式化程序 Logger，您必须调用 LogManager getFormatterLogger 方法之一。此示例的输出显示 Calendar toString() 与自定义格式相比更为冗长：

## Mixing Loggers with Formatter Loggers

格式化程序记录器对输出格式进行细粒度控制，但缺点是必须指定正确的类型（例如，为 %d 格式参数传递除十进制整数以外的任何内容都会导致异常）。

如果您的主要用途是使用 {} 样式的参数，但偶尔需要对输出格式进行细粒度控制，则可以使用 printf 方法：

```java
public static Logger logger = LogManager.getLogger("Foo");
 
logger.debug("Opening connection to {}...", someDataSource);
logger.printf(Level.INFO, "Logging in user %1$s with birthday %2$tm %2$te,%2$tY", user.getName(), user.getBirthdayCalendar());
```

## Java 8 lambda support for lazy logging

在 2.4 版中，Logger 接口添加了对 lambda 表达式的支持。这允许客户端代码延迟记录消息，而无需显式检查请求的日志级别是否已启用。

```java
// pre-Java 8 style optimization: explicitly check the log level
// to make sure the expensiveOperation() method is only called if necessary
if (logger.isTraceEnabled()) {
    logger.trace("Some long-running operation returned {}", expensiveOperation());
}
```

```java
// Java-8 style optimization: no need to explicitly check the log level:
// the lambda expression is not evaluated if the TRACE level is not enabled
logger.trace("Some long-running operation returned {}", () -> expensiveOperation());
```

## Logger Names

大多数日志实现使用分层方案来匹配日志名称和日志配置。

在此方案中，记录器名称层次结构由“.”表示。

记录器名称中的字符，其方式与用于 Java 包名称的层次结构非常相似。

例如，org.apache.logging.appender 和 org.apache.logging.filter 都将 org.apache.logging 作为它们的父级。

在大多数情况下，应用程序通过将当前类的名称传递给 LogManager.getLogger(...) 来命名它们的记录器。

因为这种用法非常普遍，所以 Log4j 2 提供了当 logger name 参数被省略或为 null 时的默认值。

例如，在下面的所有示例中，Logger 的名称都为“org.apache.test.MyTest”。

```java
package org.apache.test; 
public class MyTest {    
    private static final Logger logger = LogManager.getLogger(MyTest.class);
}
package org.apache.test; 
public class MyTest {    
    private static final Logger logger = LogManager.getLogger(MyTest.class.getName());
}
package org.apache.test; 
public class MyTest {    
    private static final Logger logger = LogManager.getLogger();
}
```

# Log Builder

Log4j 传统上与日志语句一起使用，例如

```java
logger.error("Unable to process request due to {}", code, exception);
```

这导致了一些关于异常是否应该作为消息的参数或 Log4j 是否应该将其作为 throwable 处理的混淆。

为了使日志记录更清晰，API 中添加了一个构建器模式。

使用构建器语法，上述内容将被处理为：

```java
 logger.atError().withThrowable(exception).log("Unable to process request due to {}", code);
```

现在，当调用任何 atTrace、atDebug、atInfo、atWarn、atError、atFatal、always 或 atLevel(Level) 方法时，Logger 类将返回一个 LogBuilder。

**然后 logBuilder 允许在记录事件之前将标记、Throwable 和/或位置添加到事件中。**

**对 log 方法的调用总是导致日志事件被最终确定和发送。**

带有标记、Throwable 和位置的日志记录语句如下所示：

```java
logger.atInfo().withMarker(marker).withLocation().withThrowable(exception).log("Login for user {} failed", userId);
```

在 LogBuilder 上提供 location 方法有两个明显的优势：

1. Logging 包装器可以使用它来提供 Log4j 使用的位置信息。
2. 使用不带参数的位置方法时捕获位置信息的开销比需要时必须计算位置信息要好得多。 Log4j 可以简单地在固定索引处请求堆栈跟踪条目，而不必遍历堆栈跟踪来确定调用类。

正如预期的那样，当使用 LogBuilder 并调用 withLocation() 方法时，当输出中使用位置信息时，日志记录速度要快得多，但如果不使用，则速度要慢得多。





# Flow Tracing

Logger 类提供了对跟踪应用程序的执行路径非常有用的日志记录方法。

这些方法生成可以与其他调试日志分开过滤的日志事件。

* 无需调试会话即可帮助开发中的问题诊断

* 帮助无法进行调试的生产中的问题

* 诊断有助于教育新开发人员学习应用程序。

最常用的方法是 traceEntry() 和 traceExit() 方法。 记录方法进入与出去



# Markers

日志框架的主要目的之一是提供仅在需要时生成调试和诊断信息的方法，并允许过滤该信息，以免系统或需要使用的个人不堪重负

例如，应用程序希望将其进入、退出和其他操作与正在执行的 SQL 语句分开记录，并希望能够将查询与更新分开记录。实现此目的的一种方法如下所示：

**使用标记过滤器 过滤日志**

```java
public class MyApp {
 
    private Logger logger = LogManager.getLogger(MyApp.class.getName());
    private static final Marker SQL_MARKER = MarkerManager.getMarker("SQL");
    private static final Marker UPDATE_MARKER = MarkerManager.getMarker("SQL_UPDATE").setParents(SQL_MARKER);
    private static final Marker QUERY_MARKER = MarkerManager.getMarker("SQL_QUERY").setParents(SQL_MARKER);
 
    public String doQuery(String table) {
        logger.traceEntry();
 
        logger.debug(QUERY_MARKER, "SELECT * FROM {}", table);
 
        String result = ... 
 
        return logger.traceExit(result);
    }
 
    public String doUpdate(String table, Map<String, String> params) {
        logger.traceEntry();
 
        if (logger.isDebugEnabled()) {
            logger.debug(UPDATE_MARKER, "UPDATE {} SET {}", table, formatCols());
        }
	
        String result = ... 
 
        return logger.traceExit(result);
    }
 
    private String formatCols(Map<String, String> cols) {
        StringBuilder sb = new StringBuilder();
        boolean first = true;
        for (Map.Entry<String, String> entry : cols.entrySet()) {
            if (!first) {
                sb.append(", ");
            }
            sb.append(entry.getKey()).append("=").append(entry.getValue());
            first = false;
        }
        return sb.toString();
    }
}
```

# Event Logging

EventLogger 类提供了一种简单的机制来记录应用程序中发生的事件。

虽然 EventLogger 作为启动应由审计日志系统处理的事件的一种方式很有用，但它本身并没有实现审计日志系统所需的任何功能，例如保证交付。

在典型的 Web 应用程序中使用 EventLogger 的推荐方法是使用与请求的整个生命周期相关的数据填充 ThreadContext Map，例如用户的 id、用户的 IP 地址、产品名称等。这很容易

在 servlet 过滤器中完成，其中也可以在请求结束时清除 ThreadContext Map。

当需要记录的事件发生时，应创建并填充 StructuredDataMessage。

然后调用 EventLogger.logEvent(msg)，其中 msg 是对 StructuredDataMessage 的引用。

```java
public class RequestFilter implements Filter {
    private FilterConfig filterConfig;
    private static String TZ_NAME = "timezoneOffset";
 
    public void init(FilterConfig filterConfig) throws ServletException {
        this.filterConfig = filterConfig;
    }
 
    /**
     * Sample filter that populates the MDC on every request.
     */
    public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain filterChain)
            throws IOException, ServletException {
        HttpServletRequest request = (HttpServletRequest)servletRequest;
        HttpServletResponse response = (HttpServletResponse)servletResponse;
        ThreadContext.put("ipAddress", request.getRemoteAddr());
        HttpSession session = request.getSession(false);
        TimeZone timeZone = null;
        if (session != null) {
            // Something should set this after authentication completes
            String loginId = (String)session.getAttribute("LoginId");
            if (loginId != null) {
                ThreadContext.put("loginId", loginId);
            }
            // This assumes there is some javascript on the user's page to create the cookie.
            if (session.getAttribute(TZ_NAME) == null) {
                if (request.getCookies() != null) {
                    for (Cookie cookie : request.getCookies()) {
                        if (TZ_NAME.equals(cookie.getName())) {
                            int tzOffsetMinutes = Integer.parseInt(cookie.getValue());
                            timeZone = TimeZone.getTimeZone("GMT");
                            timeZone.setRawOffset((int)(tzOffsetMinutes * DateUtils.MILLIS_PER_MINUTE));
                            request.getSession().setAttribute(TZ_NAME, tzOffsetMinutes);
                            cookie.setMaxAge(0);
                            response.addCookie(cookie);
                        }
                    }
                }
            }
        }
        ThreadContext.put("hostname", servletRequest.getServerName());
        ThreadContext.put("productName", filterConfig.getInitParameter("ProductName"));
        ThreadContext.put("locale", servletRequest.getLocale().getDisplayName());
        if (timeZone == null) {
            timeZone = TimeZone.getDefault();
        }
        ThreadContext.put("timezone", timeZone.getDisplayName());
        filterChain.doFilter(servletRequest, servletResponse);
        ThreadContext.clear();
    }
 
    public void destroy() {
    }
}
```

```java
import org.apache.logging.log4j.StructuredDataMessage;
import org.apache.logging.log4j.EventLogger;
 
import java.util.Date;
import java.util.UUID;
 
public class MyApp {
 
    public String doFundsTransfer(Account toAccount, Account fromAccount, long amount) {
        toAccount.deposit(amount);
        fromAccount.withdraw(amount);
        String confirm = UUID.randomUUID().toString();
        StructuredDataMessage msg = new StructuredDataMessage(confirm, null, "transfer");
        msg.put("toAccount", toAccount);
        msg.put("fromAccount", fromAccount);
        msg.put("amount", amount);
        EventLogger.logEvent(msg);
        return confirm;
    }
}
```

# Messages

尽管 Log4j 2 提供了接受字符串和对象的 Logger 方法，但所有这些最终都在 Message 对象中捕获，然后与日志事件关联。

应用程序可以自由地构建自己的消息并将它们传递给记录器。

尽管看起来比将消息格式和参数直接传递给事件更昂贵，但测试表明，使用现代 JVM，创建和销毁事件的成本很小，尤其是当复杂的任务封装在消息而不是应用程序中时。

**此外，当使用接受字符串和参数的方法时，只有在任何配置的全局过滤器或 Logger 的日志级别允许处理消息时，才会创建底层 Message 对象。**

```
考虑一个应用程序，它有一个包含 {"Name" = "John Doe", "Address" = "123 Main St.", "Phone" = "(999) 555-1212"} 的 Map 对象和一个具有

返回“jdoe”的 getId 方法。

开发人员想要添加返回“用户 John Doe 已使用 id jdoe 登录”的信息性消息。

实现这一点的方法是：
logger.info("User {} has logged in using id {}", map.get("Name"), user.getId());

```

虽然这本身没有任何问题，但随着对象的复杂性和所需输出的增加，这种技术变得更难使用。

**作为替代方案，使用 Messages 允许：**

```java
logger.info(new LoggedInMessage(map, user));
```

在此替代方案中，格式化委托给 LoggedInMessage 对象的 getFormattedMessage 方法。

尽管在此替代方案中创建了一个新对象，但在格式化 LoggedInMessage 之前，不会调用传递给 LoggedInMessage 的对象上的任何方法。

当对象的 toString 方法不产生您希望出现在日志中的信息时，这尤其有用。

Messages 的另一个优点是它们简化了编写布局。在其他日志框架中，布局必须单独遍历参数并根据遇到的对象确定要执行的操作。**对于消息，布局可以选择将格式委托给消息或根据遇到的消息类型执行其格式。**

借用前面说明标记以识别正在记录的 SQL 语句的示例，还可以利用消息。首先，定义消息。



```java
public class SQLMessage implements Message {
  public enum SQLType {
      UPDATE,
      QUERY
  };
 
  private final SQLType type;
  private final String table;
  private final Map<String, String> cols;
 
  public SQLMessage(SQLType type, String table) {
      this(type, table, null);
  }
 
  public SQLMessage(SQLType type, String table, Map<String, String> cols) {
      this.type = type;
      this.table = table;
      this.cols = cols;
  }
 
  public String getFormattedMessage() {
      switch (type) {
          case UPDATE:
            return createUpdateString();
            break;
          case QUERY:
            return createQueryString();
            break;
          default;
      }
  }
 
  public String getMessageFormat() {
      return type + " " + table;
  }
 
  public Object getParameters() {
      return cols;
  }
 
  private String createUpdateString() {
  }
 
  private String createQueryString() {
  }
 
  private String formatCols(Map<String, String> cols) {
      StringBuilder sb = new StringBuilder();
      boolean first = true;
      for (Map.Entry<String, String> entry : cols.entrySet()) {
          if (!first) {
              sb.append(", ");
          }
          sb.append(entry.getKey()).append("=").append(entry.getValue());
          first = false;
      }
      return sb.toString();
  }
}
```

**接下来我们可以在我们的应用程序中使用消息。**

```java
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;
import java.util.Map;
 
public class MyApp {
 
    private Logger logger = LogManager.getLogger(MyApp.class.getName());
    private static final Marker SQL_MARKER = MarkerManager.getMarker("SQL");
    private static final Marker UPDATE_MARKER = MarkerManager.getMarker("SQL_UPDATE", SQL_MARKER);
    private static final Marker QUERY_MARKER = MarkerManager.getMarker("SQL_QUERY", SQL_MARKER);
 
    public String doQuery(String table) {
        logger.entry(param);
 
        logger.debug(QUERY_MARKER, new SQLMessage(SQLMessage.SQLType.QUERY, table));
 
        return logger.exit();
    }
 
    public String doUpdate(String table, Map<String, String> params) {
        logger.entry(param);
 
        logger.debug(UPDATE_MARKER, new SQLMessage(SQLMessage.SQLType.UPDATE, table, parmas);
 
        return logger.exit();
    }
}
```



## 其他消息类

### FormattedMessage

使用 MessageFormatMessage 对其进行格式化

### LocalizedMessage

提供 LocalizedMessage 主要是为了提供与 Log4j 1.x 的兼容性。通常，本地化的最佳方法是让客户端 UI 在客户端的语言环境中呈现事件。

#### LoggerNameAwareMessage

此方法将在事件构造期间调用，以便 Message 具有在格式化消息时用于记录事件的 Logger 的名称。

### MapMessage

MapMessage 包含字符串键和值的映射。 

MapMessage 实现 FormattedMessage 并接受“XML”、“JSON”或“JAVA”的格式说明符，在这种情况下，Map 将被格式化为 XML、JSON 或 java.util.AbstractMap.toString() 所记录的格式。

否则，地图将被格式化为“key1=value1 key2=value2 ...”。

### 一些 Appender 专门使用 MapMessage 对象：

1. 当 JMS Appender 配置了 MessageLayout 时，它会将 Log4j MapMessage 转换为 JMS javax.jms.MapMessage。
2. 当 JDBC Appender 配置了 MessageLayout 时，它会将 Log4j MapMessage 转换为 SQL INSERT 语句中的值。
3. 当 MongoDB3 Appender 或 MongoDB4 Appender 配置了 MessageLayout 时，它会将 Log4j MapMessage 转换为 MongoDB 对象中的字段。

当 Appender 是 MessageLayout-aware 时，Log4j 发送到目标的对象不是 Log4j 日志事件，而是自定义对象。

#### MessageFormatMessage

[MessageFormatMessage](http://logging.apache.org/log4j/2.x/log4j-api/apidocs/org/apache/logging/log4j/message/MessageFormatMessage.html) handles messages that use a [conversion format](https://docs.oracle.com/javase/7/docs/api/java/text/MessageFormat.html). While this Message has more flexibility than ParameterizedMessage, it is also about two times slower.

#### MultiformatMessage

A MultiformatMessage will have a getFormats method and a getFormattedMessage method that accepts and array of format Strings. The getFormats method may be called by a Layout to provide it information on what formatting options the Message supports. The Layout may then call getFormattedMessage with one or more for the formats. If the Message doesn't recognize the format name it will simply format the data using its default format. An example of this is the StructuredDataMessage which accepts a format String of "XML" which will cause it to format the event data as XML instead of the RFC 5424 format.

#### ObjectMessage

Formats an Object by calling its toString method. Since Log4j 2.6, Layouts trying to be low-garbage or garbage-free will call the formatTo(StringBuilder) method instead.

#### ParameterizedMessage

[ParameterizedMessage](http://logging.apache.org/log4j/2.x/log4j-api/apidocs/org/apache/logging/log4j/message/ParameterizedMessage.html) handles messages that contain "{}" in the format to represent replaceable tokens and the replacement parameters.

#### ReusableObjectMessage

In garbage-free mode, this message is used to pass logged Objects to the Layout and Appenders. Functionally equivalent to [ObjectMessage](http://logging.apache.org/log4j/2.x/manual/messages.html#ObjectMessage).

#### ReusableParameterizedMessage

In garbage-free mode, this message is used to handle messages that contain "{}" in the format to represent replaceable tokens and the replacement parameters. Functionally equivalent to [ParameterizedMessage](http://logging.apache.org/log4j/2.x/manual/messages.html#ParameterizedMessage).

#### ReusableSimpleMessage

In garbage-free mode, this message is used to pass logged Strings and CharSequences to the Layout and Appenders. Functionally equivalent to [SimpleMessage](http://logging.apache.org/log4j/2.x/manual/messages.html#SimpleMessage).

#### SimpleMessage

SimpleMessage contains a String or CharSequence that requires no formatting.

#### StringFormattedMessage

[StringFormattedMessage](http://logging.apache.org/log4j/2.x/log4j-api/apidocs/org/apache/logging/log4j/message/StringFormattedMessage.html) handles messages that use a [conversion format](https://docs.oracle.com/javase/7/docs/api/java/util/Formatter.html#syntax) that is compliant with [java.lang.String.format()](https://docs.oracle.com/javase/7/docs/api/java/lang/String.html#format(java.lang.String, java.lang.Object...)). While this Message has more flexibility than ParameterizedMessage, it is also 5 to 10 times slower.

#### StructuredDataMessage

[StructuredDataMessage](http://logging.apache.org/log4j/2.x/log4j-api/apidocs/org/apache/logging/log4j/message/StructuredDataMessage.html) allows applications to add items to a Map as well as set the id to allow a message to be formatted as a Structured Data element in accordance with [RFC 5424](http://tools.ietf.org/html/rfc5424).

#### ThreadDumpMessage

A ThreadDumpMessage, if logged, will generate stack traces for all threads. The stack traces will include any locks that are held.

#### TimestampMessage

A TimestampMessage will provide a getTimestamp method that is called during event construction. The timestamp in the Message will be used in lieu of the current timestamp.

# Thread Context

Log4j 引入了映射诊断上下文或 MDC 的概念。

Log4j 2 延续了 MDC 和 NDC 的想法，但将它们合并到一个单一的线程上下文中。

线程上下文映射相当于 MDC

线程上下文堆栈相当于 NDC。

两者间的区别是 

至于选择NDC还是MDC要看需要存储的上下文信息是堆栈式的还是key/value形式的。

NDC采用了一个类似栈的机制来push和pop上下文信息，每一个线程都独立地储存上下文信息。比如说一个servlet就可以针对每一个request创建对应的NDC，储存客户端地址等等信息。

当使用的时候，我们要尽可能确保在进入一个context的时候，把相关的信息使用NDC.push(message);在离开这个context的时候使用NDC.pop()将信息删除。另外由于设计上的一些问题，还需要保证在当前thread结束的时候使用NDC.remove()清除内存，否则会产生内存泄漏的问题。


## Fish Tagging

大多数现实世界的系统必须同时处理多个客户端。在此类系统的典型多线程实现中，不同的线程将处理不同的客户端。日志记录特别适合跟踪和调试复杂的分布式应用程序。区分一个客户端的日志输出与另一个的常用方法是为每个客户端实例化一个新的单独记录器。这促进了记录器的扩散并增加了记录的管理开销。

**一种较轻的技术是对从同一客户端交互发起的每个日志请求进行唯一标记。** 

Neil Harrison 在由 R. Martin、D. Riehle 和 F. Buschmann 编辑的“程序设计模式语言 3”一书中的“记录诊断消息的模式”一书中描述了这种方法（Addison-Wesley，1997 年）。

就像鱼可以被标记并跟踪其移动一样，使用通用标记或数据元素集标记日志事件允许跟踪事务或请求的完整流程。

我们称之为鱼标记。

log4j 提供了两种执行 Fish Tagging 的机制；

1. 线程上下文映射

2. 线程上下文堆栈

线程上下文映射允许使用键/值对添加和识别任意数量的项目。

线程上下文堆栈允许将一个或多个项目压入堆栈，然后通过它们在堆栈中的顺序或数据本身进行标识

由于key/value对更加灵活，当请求的处理过程中可能会添加数据项或者数据项超过一两个时，推荐使用Thread Context Map。

```java
ThreadContext.push(UUID.randomUUID().toString()); // Add the fishtag;
 
logger.debug("Message 1");
.
.
.
logger.debug("Message 2");
.
.
ThreadContext.pop();
```

```java
ThreadContext.put("id", UUID.randomUUID().toString()); // Add the fishtag;
ThreadContext.put("ipAddress", request.getRemoteAddr());
ThreadContext.put("loginId", session.getAttribute("loginId"));
ThreadContext.put("hostName", request.getServerName());
.
logger.debug("Message 1");
.
.
logger.debug("Message 2");
.
.
ThreadContext.clear();
```

## CloseableThreadContext

放入堆栈需要清除数据，CloseableThreadContext 实现了 AutoCloseable 接口。

这允许将项目推送到堆栈或放入映射中，并在调用 close() 方法时删除 - 或者作为 try-with-resources 的一部分自动删除。

```java
// Add to the ThreadContext stack for this try block only;
try (final CloseableThreadContext.Instance ctc = CloseableThreadContext.push(UUID.randomUUID().toString())) {
 
    logger.debug("Message 1");
.
.
    logger.debug("Message 2");
.
.
}
// Add to the ThreadContext map for this try block only;
try (final CloseableThreadContext.Instance ctc = CloseableThreadContext.put("id", UUID.randomUUID().toString())
                                                                .put("loginId", session.getAttribute("loginId"))) {
 
    logger.debug("Message 1");
.
.
    logger.debug("Message 2");
.
.
}
```

如果您使用线程池，则可以使用 putAll(final Map values) 和/或 pushAll(List messages) 方法初始化 CloseableThreadContext ；

开启线程池时 使用putAll pushAll传值

```java
for( final Session session : sessions ) {
    try (final CloseableThreadContext.Instance ctc = CloseableThreadContext.put("loginId", session.getAttribute("loginId"))) {
        logger.debug("Starting background thread for user");
        final Map<String, String> values = ThreadContext.getImmutableContext();
        final List<String> messages = ThreadContext.getImmutableStack().asList();
        executor.submit(new Runnable() {
        public void run() {
            try (final CloseableThreadContext.Instance ctc = CloseableThreadContext.putAll(values).pushAll(messages)) {
                logger.debug("Processing for user started");
                .
                logger.debug("Processing for user completed");
            }
        });
    }
}
```

Map 可以配置为使用 [InheritableThreadLocal](http://docs.oracle.com/javase/6/docs/api/java/lang/InheritableThreadLocal.html).以这种方式配置时，Map 的内容将传递给子线程。

## 配置

#### Configuration

- Set the system property `disableThreadContextMap` to `true` to disable the Thread Context Map.
- Set the system property `disableThreadContextStack` to `true` to disable the Thread Context Stack.
- Set the system property `disableThreadContext` to `true` to disable both the Thread Context Map and Stack.
- Set the system property `log4j2.isThreadContextMapInheritable` to `true` to enable child threads to inherit the Thread Context Map.



## Including the ThreadContext when writing logs

The [PatternLayout](http://logging.apache.org/log4j/2.x/log4j-core/apidocs/org/apache/logging/log4j/core/layout/PatternLayout.html) provides mechanisms to print the contents of the [ThreadContext](http://logging.apache.org/log4j/2.x/log4j-api/apidocs/org/apache/logging/log4j/ThreadContext.html) Map and Stack.

- Use %X by itself to include the full contents of the Map.
- Use %X{key} to include the specified key.
- Use %x to include the full contents of the [Stack](http://docs.oracle.com/javase/6/docs/api/java/util/Stack.html).

#### Custom context data injectors for non thread-local context data

With the ThreadContext logging statements can be tagged so log entries that were related in some way can be linked via these tags. The limitation is that this only works for logging done on the same application thread (or child threads when configured).

Some applications have a thread model that delegates work to other threads, and in such models, tagging attributes that are put into a thread-local map in one thread are not visible in the other threads and logging done in the other threads will not show these attributes.

Log4j 2.7 adds a flexible mechanism to tag logging statements with context data coming from other sources than the ThreadContext. See the manual page on [extending Log4j](http://logging.apache.org/log4j/2.x/manual/extending.html#Custom_ContextDataInjector) for details.







