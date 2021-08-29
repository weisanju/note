# Logging

**CommonsLoggingAPI**

SpringBoot 使用 [Commons Logging](https://commons.apache.org/logging) API 记录所有内部日志 ，但是允许多种底层内部实现

**默认提供控制台输出**

提供  Java Util Logging、Log4J2 和 Logback 默认配置，在每种情况下，记录器都预先配置为使用控制台输出，也可以使用可选的文件输出。

默认情况下，如果您使用“Starters”，则使用 Logback 进行日志记录，还包括适当的 Logback 路由，以确保使用 Java Util Logging、Commons Logging、Log4J 或 SLF4J 的依赖库都能正常工作。





# Log Format

Spring Boot 的默认日志输出类似于以下示例：

```
019-03-05 10:57:51.112  INFO 45469 --- [           main] org.apache.catalina.core.StandardEngine  : Starting Servlet Engine: Apache Tomcat/7.0.52
2019-03-05 10:57:51.253  INFO 45469 --- [ost-startStop-1] o.a.c.c.C.[Tomcat].[localhost].[/]       : Initializing Spring embedded WebApplicationContext
2019-03-05 10:57:51.253  INFO 45469 --- [ost-startStop-1] o.s.web.context.ContextLoader            : Root WebApplicationContext: initialization completed in 1358 ms
2019-03-05 10:57:51.698  INFO 45469 --- [ost-startStop-1] o.s.b.c.e.ServletRegistrationBean        : Mapping servlet: 'dispatcherServlet' to [/]
2019-03-05 10:57:51.702  INFO 45469 --- [ost-startStop-1] o.s.b.c.embedded.FilterRegistrationBean  : Mapping filter: 'hiddenHttpMethodFilter' to: [/*]
```

**格式为**

- Date and Time: Millisecond precision and easily sortable.
- Log Level: `ERROR`, `WARN`, `INFO`, `DEBUG`, or `TRACE`.
- Process ID.
- A `---` separator ：分割日志头和日志内容
- Thread name: 括在方括号中（控制台输出可能会被截断）。
- Logger name: 通常是源类名称（通常缩写）。
- The log message.



# Console Output

默认日志配置在写入消息时将消息回显到控制台。

默认情况下，会记录 ERROR 级别、WARN 级别和 INFO 级别的消息。

您还可以通过使用 --debug 标志启动应用程序来启用 “debug” 模式。

```shell
$ java -jar myapp.jar --debug
```

You can also specify `debug=true` in your `application.properties`.

或者，您可以通过使用 --trace 标志（或 application.properties 中的 trace=true）启动应用程序来启用“跟踪”模式。

## Color-coded Output

如果您的终端支持 ANSI，则使用颜色输出来提高可读性。

您可以将 spring.output.ansi.enabled 设置为支持的值以覆盖自动检测。

颜色编码是使用 %clr 转换字配置的。

在最简单的形式中，转换器根据日志级别为输出着色，如以下示例所示：

```
%clr(%5p)
```



| Level   | Color  |
| :------ | :----- |
| `FATAL` | Red    |
| `ERROR` | Red    |
| `WARN`  | Yellow |
| `INFO`  | Green  |
| `DEBUG` | Green  |
| `TRACE` | Green  |

或者，您可以通过将其作为转换选项提供来指定应使用的颜色或样式。

例如，要使文本变黄，请使用以下设置：

```
%clr(%d{yyyy-MM-dd HH:mm:ss.SSS}){yellow}
```

支持以下颜色和样式：

- `blue`
- `cyan`
- `faint`
- `green`
- `magenta`
- `red`
- `yellow`

# File Output

默认情况下，Spring Boot 只记录到控制台，不写入日志文件。

如果您想在控制台输出之外写入日志文件，您需要设置 logging.file.name 或 logging.file.path 属性（例如，在您的 application.properties 中）。

下表显示了 logging.* 属性如何一起使用：

| `logging.file.name` | `logging.file.path` | Example    | Description                                                  |
| :------------------ | :------------------ | :--------- | :----------------------------------------------------------- |
| *(none)*            | *(none)*            |            | 仅控制台记录。                                               |
| Specific file       | *(none)*            | `my.log`   | 写入指定的日志文件。名称可以是确切位置或相对于当前目录。     |
| *(none)*            | Specific directory  | `/var/log` | 将 `spring.log` 写入指定目录。名称可以是确切位置或相对于当前目录。 |

日志文件在达到 10 MB 时会轮换，并且与控制台输出一样，默认情况下会记录 ERROR 级别、WARN 级别和 INFO 级别的消息。



日志属性独立于实际的日志基础设施。

因此，特定的配置键（例如 Logback 的 logback.configurationFile）不受 spring Boot 管理。





# File Rotation

如果您使用的是 Logback，则可以使用 application.properties 或 application.yaml 文件微调日志轮换设置。

对于所有其他日志系统，您需要自己直接配置轮换设置（例如，如果您使用 Log4J2，那么您可以添加一个 log4j.xml 文件）。

支持以下轮换策略属性：

| Name                                                   | Description                                |
| :----------------------------------------------------- | :----------------------------------------- |
| `logging.logback.rollingpolicy.file-name-pattern`      | 用于创建日志存档的文件名模式。             |
| `logging.logback.rollingpolicy.clean-history-on-start` | 如果在应用程序启动时应该进行日志归档清理。 |
| `logging.logback.rollingpolicy.max-file-size`          | 归档前日志文件的最大大小。                 |
| `logging.logback.rollingpolicy.total-size-cap`         | 在删除之前可以使用的最大大小日志存档数量。 |
| `logging.logback.rollingpolicy.max-history`            | 保留日志存档的天数（默认为 7）             |

# Log Levels

所有支持的日志系统都可以通过使用 logging.level.= 在 Spring Environment（例如，在 application.properties）中设置记录器级别，其中级别是 TRACE, DEBUG, INFO, WARN, ERROR, FATAL, or OFF 之一

可以使用 logging.level.root 配置根记录器

```yaml
logging:
  level:
    root: "warn"
    org.springframework.web: "debug"
    org.hibernate: "error"

```

**也可以使用环境变量设置日志记录级别。**

例如， LOGGING_LEVEL_ORG_SPRINGFRAMEWORK_WEB=DEBUG 会将 org.springframework.web 设置为 DEBUG。

上述方法仅适用于包级日志记录。

由于宽松绑定总是将环境变量转换为小写，因此不可能以这种方式为单个类配置日志记录



如果需要为类配置日志记录，可以使用 SPRING_APPLICATION_JSON 变量。



# Log Groups

能够将相关的记录器组合在一起以便可以同时配置它们通常很有用。

例如，您通常可能会更改所有与 Tomcat 相关的记录器的日志记录级别，但您无法轻松记住顶级包。

为了解决这个问题，Spring Boot 允许您在 Spring 环境中定义日志记录组。

例如，下面是如何通过将“tomcat”组添加到 application.properties 来定义它：

```yaml
logging:
  group:
    tomcat: "org.apache.catalina,org.apache.coyote,org.apache.tomcat"
```

定义后，您可以使用一行更改组中所有记录器的级别：

```yaml
logging:
  level:
    tomcat: "trace"
```

Spring Boot 包括以下可以开箱即用的预定义日志记录组：



| Name | Loggers                                                      |
| :--- | :----------------------------------------------------------- |
| web  | `org.springframework.core.codec`, `org.springframework.http`, `org.springframework.web`, `org.springframework.boot.actuate.endpoint.web`, `org.springframework.boot.web.servlet.ServletContextInitializerBeans` |
| sql  | `org.springframework.jdbc.core`, `org.hibernate.SQL`, `org.jooq.tools.LoggerListener` |

# Using a Log Shutdown Hook

为了在您的应用程序终止时释放日志资源，提供了一个关闭挂钩,当 JVM 退出时将触发日志系统清理。

```yaml
logging:
  register-shutdown-hook: false

```



# Custom Log Configuration

可以通过在类路径中包含适当的库来激活各种日志系统

并且可以通过在类路径的根目录或以下 Spring 环境变量属性指定的位置提供合适的配置文件来进一步定制：logging.config。



您可以通过使用 **org.springframework.boot.logging.LoggingSystem** 系统属性来强制 Spring Boot 使用特定的日志记录系统。

该值应该是 LoggingSystem 实现的完全限定类名。

您还可以使用 none 值完全禁用 Spring Boot 的日志记录配置。



**注意**

1. 由于日志记录是在创建 ApplicationContext 之前初始化的，因此无法从 Spring @Configuration 文件中的 @PropertySources 控制日志记录。

2. 更改日志系统或完全禁用它的唯一方法是通过系统属性。





**根据您的日志系统，加载以下文件：**

| Logging System          | Customization                                                |
| :---------------------- | :----------------------------------------------------------- |
| Logback                 | `logback-spring.xml`, `logback-spring.groovy`, `logback.xml`, or `logback.groovy` |
| Log4j2                  | `log4j2-spring.xml` or `log4j2.xml`                          |
| JDK (Java Util Logging) | `logging.properties`                                         |

如果可能，我们建议您将 -spring 变体用于日志记录配置（例如，logback-spring.xml 而不是 logback.xml）。

如果使用标准配置位置，Spring 无法完全控制日志初始化。

**警告**

Java Util Logging 存在已知的类加载问题，这些问题会导致从“可执行 jar”运行时出现问题。

我们建议您在从“可执行 jar”运行时尽可能避免使用它。



为了帮助定制，一些其他属性从 Spring Environment 转移到 System properties，如下表所述：

| Spring Environment                  | System Property                 | Comments                                                     |
| :---------------------------------- | :------------------------------ | :----------------------------------------------------------- |
| `logging.exception-conversion-word` | `LOG_EXCEPTION_CONVERSION_WORD` | 记录异常时使用的转换词。                                     |
| `logging.file.name`                 | `LOG_FILE`                      | 如果定义，则在默认日志配置中使用。                           |
| `logging.file.path`                 | `LOG_PATH`                      | 如果定义，则在默认日志配置中使用。                           |
| `logging.pattern.console`           | `CONSOLE_LOG_PATTERN`           | The log pattern to use on the console (stdout).              |
| `logging.pattern.dateformat`        | `LOG_DATEFORMAT_PATTERN`        | Appender pattern for log date format.                        |
| `logging.charset.console`           | `CONSOLE_LOG_CHARSET`           | The charset to use for console logging.                      |
| `logging.pattern.file`              | `FILE_LOG_PATTERN`              | The log pattern to use in a file (if `LOG_FILE` is enabled). |
| `logging.charset.file`              | `FILE_LOG_CHARSET`              | The charset to use for file logging (if `LOG_FILE` is enabled). |
| `logging.pattern.level`             | `LOG_LEVEL_PATTERN`             | The format to use when rendering the log level (default `%5p`). |
| `PID`                               | `PID`                           | The current process ID (discovered if possible and when not already defined as an OS environment variable). |





如果您使用的是 Logback，以下属性也会被转移：

| Spring Environment                                     | System Property                                | Comments                                                     |
| :----------------------------------------------------- | :--------------------------------------------- | :----------------------------------------------------------- |
| `logging.logback.rollingpolicy.file-name-pattern`      | `LOGBACK_ROLLINGPOLICY_FILE_NAME_PATTERN`      | Pattern for rolled-over log file names (default `${LOG_FILE}.%d{yyyy-MM-dd}.%i.gz`). |
| `logging.logback.rollingpolicy.clean-history-on-start` | `LOGBACK_ROLLINGPOLICY_CLEAN_HISTORY_ON_START` | Whether to clean the archive log files on startup.           |
| `logging.logback.rollingpolicy.max-file-size`          | `LOGBACK_ROLLINGPOLICY_MAX_FILE_SIZE`          | Maximum log file size.                                       |
| `logging.logback.rollingpolicy.total-size-cap`         | `LOGBACK_ROLLINGPOLICY_TOTAL_SIZE_CAP`         | Total size of log backups to be kept.                        |
| `logging.logback.rollingpolicy.max-history`            | `LOGBACK_ROLLINGPOLICY_MAX_HISTORY`            | Maximum number of archive log files to keep.                 |

所有支持的日志系统在解析其配置文件时都可以查询系统属性。示例参见 spring-boot.jar 中的默认配置：

- [Logback](https://github.com/spring-projects/spring-boot/tree/v2.5.4/spring-boot-project/spring-boot/src/main/resources/org/springframework/boot/logging/logback/defaults.xml)
- [Log4j 2](https://github.com/spring-projects/spring-boot/tree/v2.5.4/spring-boot-project/spring-boot/src/main/resources/org/springframework/boot/logging/log4j2/log4j2.xml)

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="WARN">
	<Properties>
		<Property name="LOG_EXCEPTION_CONVERSION_WORD">%xwEx</Property>
		<Property name="LOG_LEVEL_PATTERN">%5p</Property>
		<Property name="LOG_DATEFORMAT_PATTERN">yyyy-MM-dd HH:mm:ss.SSS</Property>
		<Property name="CONSOLE_LOG_PATTERN">%clr{%d{${sys:LOG_DATEFORMAT_PATTERN}}}{faint} %clr{${sys:LOG_LEVEL_PATTERN}} %clr{%pid}{magenta} %clr{---}{faint} %clr{[%15.15t]}{faint} %clr{%-40.40c{1.}}{cyan} %clr{:}{faint} %m%n${sys:LOG_EXCEPTION_CONVERSION_WORD}</Property>
		<Property name="FILE_LOG_PATTERN">%d{${LOG_DATEFORMAT_PATTERN}} ${LOG_LEVEL_PATTERN} %pid --- [%t] %-40.40c{1.} : %m%n${sys:LOG_EXCEPTION_CONVERSION_WORD}</Property>
	</Properties>
	<Appenders>
		<Console name="Console" target="SYSTEM_OUT" follow="true">
			<PatternLayout pattern="${sys:CONSOLE_LOG_PATTERN}" charset="${sys:CONSOLE_LOG_CHARSET}"/>
		</Console>
	</Appenders>
	<Loggers>
		<Logger name="org.apache.catalina.startup.DigesterFactory" level="error" />
		<Logger name="org.apache.catalina.util.LifecycleBase" level="error" />
		<Logger name="org.apache.coyote.http11.Http11NioProtocol" level="warn" />
		<Logger name="org.apache.sshd.common.util.SecurityUtils" level="warn"/>
		<Logger name="org.apache.tomcat.util.net.NioSelectorPool" level="warn" />
		<Logger name="org.eclipse.jetty.util.component.AbstractLifeCycle" level="error" />
		<Logger name="org.hibernate.validator.internal.util.Version" level="warn" />
		<Logger name="org.springframework.boot.actuate.endpoint.jmx" level="warn"/>
		<Root level="info">
			<AppenderRef ref="Console" />
		</Root>
	</Loggers>
</Configuration
```



- [Java Util logging](https://github.com/spring-projects/spring-boot/tree/v2.5.4/spring-boot-project/spring-boot/src/main/resources/org/springframework/boot/logging/java/logging-file.properties)





**提示**

如果你想在日志属性中使用占位符，你应该使用 Spring Boot 的语法而不是底层框架的语法。

值得注意的是，如果您使用 Logback，您应该使用 : 作为属性名称与其默认值之间的分隔符，而不是使用 :-。

您可以通过仅覆盖 LOG_LEVEL_PATTERN（或带有 Logback 的 logging.pattern.level ）将 MDC 和其他临时内容添加到日志行。

例如，如果您使用 logging.pattern.level=user:%X{user} %5p，则默认日志格式包含“user”的 MDC 条目（如果存在），如以下示例所示。





# 源码分析



## 如何初始化日志系统

通过SpringAppliccationListener 事件监听器：监听spring的启动事件：以初始化日志系统

```java
@Override
public void onApplicationEvent(ApplicationEvent event) {
    //spring刚启动时，执行 日志系统的载入与前初始化
   if (event instanceof ApplicationStartingEvent) {
      onApplicationStartingEvent((ApplicationStartingEvent) event);
   }
    //spring环境变量准备好时：日志系统的初始化
   else if (event instanceof ApplicationEnvironmentPreparedEvent) {
      onApplicationEnvironmentPreparedEvent((ApplicationEnvironmentPreparedEvent) event);
   }
    //spring启动好 之后：往容器中注入 日志相关bean springBootLoggingSystem、springBootLogFile
   else if (event instanceof ApplicationPreparedEvent) {
      onApplicationPreparedEvent((ApplicationPreparedEvent) event);
   }
    //spring容器关闭之后，调用日志系统的关闭
   else if (event instanceof ContextClosedEvent
         && ((ContextClosedEvent) event).getApplicationContext().getParent() == null) {
      onContextClosedEvent();
   }
    //spring容器启动 失败后，调用清理
   else if (event instanceof ApplicationFailedEvent) {
      onApplicationFailedEvent();
   }
}
```



## 日志系统的载入与前初始化

**载入日志 系统**

```java
public static LoggingSystem get(ClassLoader classLoader) {
    //如果指定了 `org.springframework.boot.logging.LoggingSystem`系统环境变量,则使用指定的
   String loggingSystem = System.getProperty(SYSTEM_PROPERTY);
   if (StringUtils.hasLength(loggingSystem)) {
      if (NONE.equals(loggingSystem)) {
         return new NoOpLoggingSystem();
      }
      return get(classLoader, loggingSystem);
   }
   //否则 按默认顺序检测特定类
   return SYSTEMS.entrySet().stream().filter((entry) -> ClassUtils.isPresent(entry.getKey(), classLoader))
         .map((entry) -> get(classLoader, entry.getValue())).findFirst()
         .orElseThrow(() -> new IllegalStateException("No suitable logging system located"));
}
	static {
		Map<String, String> systems = new LinkedHashMap<>();
		systems.put("ch.qos.logback.core.Appender", "org.springframework.boot.logging.logback.LogbackLoggingSystem");
		systems.put("org.apache.logging.log4j.core.impl.Log4jContextFactory",
				"org.springframework.boot.logging.log4j2.Log4J2LoggingSystem");
		systems.put("java.util.logging.LogManager", "org.springframework.boot.logging.java.JavaLoggingSystem");
		SYSTEMS = Collections.unmodifiableMap(systems);
	}
```

**前初始化**

```java
//sl4j前初始化：桥接 JUL与 sl4j
private void configureJdkLoggingBridgeHandler() {
   try {
      if (isBridgeJulIntoSlf4j()) {
         removeJdkLoggingBridgeHandler();
         SLF4JBridgeHandler.install();
      }
   }
   catch (Throwable ex) {
      // Ignore. No java.util.logging bridge is installed.
   }
}

//log4j前初始化：禁用一切日志打印
public void beforeInitialize() {
    LoggerContext loggerContext = getLoggerContext();
    if (isAlreadyInitialized(loggerContext)) {
        return;
    }
    super.beforeInitialize();
    loggerContext.getConfiguration().addFilter(FILTER);
}
```

## 日志系统的初始化

1. 转换 日志配置到 系统环境变量中
2. 将 日志位置的配置 加载到环境变量 中
3. 日志组的获取与处理
4. c设置初始化日志级别
5. 注册jvm关闭回调

```java
protected void initialize(ConfigurableEnvironment environment, ClassLoader classLoader) {
    //转换 日志配置到 系统环境变量中：并对配置进行环境变量替换
   new LoggingSystemProperties(environment).apply();
   //获取指定 的logFile
   this.logFile = LogFile.get(environment);
    //将 日志位置的配置 加载到环境变量 中
   if (this.logFile != null) {
      this.logFile.applyToSystemProperties();
   }
    //日志组的处理
   this.loggerGroups = new LoggerGroups(DEFAULT_GROUP_LOGGERS);
    //早期日志级别的 设置：兼容 命令行选项 --debug --trace
   initializeEarlyLoggingLevel(environment);
    //初始化日志系统
   initializeSystem(environment, this.loggingSystem, this.logFile);
    /设置初始化日志级别
   initializeFinalLoggingLevels(environment, this.loggingSystem);
    //注册jvm关闭回调
   registerShutdownHookIfNecessary(environment, this.loggingSystem);
}
```









## 初始化 实际日志系统实现

> 以log4j为例

**配置Or约定**

```java
public void initialize(LoggingInitializationContext initializationContext, String configLocation, LogFile logFile) {
    //如果有配置日志路径
   if (StringUtils.hasLength(configLocation)) {
       //使用特定配置文件初始化
      initializeWithSpecificConfig(initializationContext, configLocation, logFile);
      return;
   }
    //按照约定查找配置文件
   initializeWithConventions(initializationContext, logFile);
}
```

**按照约定查找配置文件**

> 以log4j为例

添加 `log4j2.properties` `log4j2-test.properties` 等等

```java
//org.springframework.boot.logging.log4j2.Log4J2LoggingSystem#getCurrentlySupportedConfigLocations
private String[] getCurrentlySupportedConfigLocations() {
   List<String> supportedConfigLocations = new ArrayList<>();
   addTestFiles(supportedConfigLocations);
   supportedConfigLocations.add("log4j2.properties");
   if (isClassAvailable("com.fasterxml.jackson.dataformat.yaml.YAMLParser")) {
      Collections.addAll(supportedConfigLocations, "log4j2.yaml", "log4j2.yml");
   }
   if (isClassAvailable("com.fasterxml.jackson.databind.ObjectMapper")) {
      Collections.addAll(supportedConfigLocations, "log4j2.json", "log4j2.jsn");
   }
   supportedConfigLocations.add("log4j2.xml");
   return StringUtils.toStringArray(supportedConfigLocations);
}
```

**如果有多个返回配置中的第一个资源存在的**

```java
//org.springframework.boot.logging.AbstractLoggingSystem#findConfig
private String findConfig(String[] locations) {
   for (String location : locations) {
      ClassPathResource resource = new ClassPathResource(location, this.classLoader);
      if (resource.exists()) {
         return "classpath:" + location;
      }
   }
   return null;
}
```

