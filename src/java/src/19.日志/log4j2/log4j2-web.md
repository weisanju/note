# Using Log4j 2 in Web Applications

在 Java EE Web 应用程序中使用 Log4j 或任何其他日志记录框架时，您必须特别小心。

当容器关闭或取消部署 Web 应用程序时，正确清理日志资源（关闭数据库连接、关闭文件等）非常重要。

由于 Web 应用程序中类加载器的性质，Log4j 资源无法通过正常方式清理。

当 Web 应用程序部署时 Log4j 必须“启动”，当 Web 应用程序取消部署时必须“关闭”。

它的工作方式取决于您的应用程序是 Servlet 3.0 或更新版本还是 Servlet 2.5 Web 应用程序。



无论哪种情况，您都需要将 log4j-web 模块添加到您的部署中，如 Maven、Ivy 和 Gradle Artifacts 手册页中所述。

为避免出现问题，当包含 log4j-web jar 时，将自动禁用 Log4j 关闭挂钩。



# Configuration

Log4j 允许 在 web.xml 中 使用 log4jConfiguration  context parameter 指定配置文件。 

Log4j 将通过以下方式搜索配置文件

1. 如果提供了位置，它将作为 servlet 上下文资源进行搜索。例如，如果 log4jConfiguration 包含“logging.xml”，那么 Log4j 将在 Web 应用程序的根目录中查找具有该名称的文件。

2. 如果没有定义位置，Log4j 将在 WEB-INF 目录中搜索以“log4j2”开头的文件。如果找到多个文件，并且存在以“log4j2-name”开头的文件（其中 name 是 Web 应用程序的名称），则将使用该文件。否则将使用第一个文件。
3. 使用类路径和文件 URL 的“正常”搜索序列将用于定位配置文件。
4. 使用类路径和文件 URL 的“正常”搜索序列将用于定位配置文件。

# Servlet 3.0 and Newer Web Applications

Log4j 2 在 Servlet 3.0 和更新的 Web 应用程序中“正常工作”。

它能够在应用程序部署时自动启动并在应用程序取消部署时自动关闭。

由于 ServletContainerInitializer API 添加到 Servlet 3.0，相关的 Filter 和 ServletContextListener 类可以在 web 应用程序启动时动态注册。



重要的提示！

出于性能原因，容器通常会忽略某些已知不包含 TLD 或 ServletContainerInitializers 的 JAR，并且不扫描它们以查找 web 片段和初始化程序。

重要的是，Tomcat 7 <7.0.43 会忽略所有名为 log4j*.jar 的 JAR 文件，这会阻止此功能工作。

这已在 Tomcat 7.0.43、Tomcat 8 及更高版本中修复。



Log4j 2 Web JAR 文件是一个 Web 片段，配置为在应用程序中的任何其他 Web 片段之前排序。

它包含一个容器自动发现和初始化的 ServletContainerInitializer (Log4jServletContainerInitializer)。

这会将 Log4jServletContextListener 和 Log4jServletFilter 添加到 ServletContext。

这些类正确初始化和取消初始化 Log4j 配置。



对于某些用户来说，自动启动 Log4j 是有问题的或不可取的。

您可以使用 isLog4jAutoInitializationDisabled 上下文参数轻松禁用此功能。

只需使用值“true”将其添加到您的部署描述符中即可禁用自动初始化。

您必须在 web.xml 中定义上下文参数。

如果以编程方式设置，Log4j 检测设置为时已晚。

```xml
  <context-param>
        <param-name>isLog4jAutoInitializationDisabled</param-name>
        <param-value>true</param-value>
    </context-param>
```

禁用自动初始化后，您必须像初始化 Servlet 2.5 Web 应用程序一样初始化 Log4j。

您必须以这种初始化发生在任何其他应用程序代码（例如 Spring Framework 启动代码）执行之前的方式执行此操作。

您可以使用 log4jContextName、log4jConfiguration 和/或 isLog4jContextSelectorNamed 上下文参数自定义侦听器和过滤器的行为。

在下面的上下文参数部分阅读更多相关信息。

除非您使用 isLog4jAutoInitializationDisabled 禁用自动初始化，否则您不得在部署描述符 (web.xml) 或 Servlet 3.0 或更高版本应用程序中的另一个初始化程序或侦听器中手动配置 Log4jServletContextListener 或 Log4jServletFilter。

这样做将导致启动错误和未指定的错误行为。



# Servlet 2.5 Web Applications



Servlet 2.5 Web 应用程序是版本属性值为“2.5”的任何 。 

version 属性是唯一重要的东西；

即使 Web 应用程序运行在 Servlet 3.0 或更新的容器中，如果版本属性为“2.5”，它也是 Servlet 2.5 Web 应用程序。

请注意，Log4j 2 不支持 Servlet 2.4 和更旧的 Web 应用程序。



如果您在 Servlet 2.5 Web 应用程序中使用 Log4j，或者您已使用 isLog4jAutoInitializationDisabled 上下文参数禁用自动初始化，则必须在部署描述符中或以编程方式配置 Log4jServletContextListener 和 Log4jServletFilter。

过滤器应匹配任何类型的所有请求。

监听器应该是应用程序中定义的第一个监听器，过滤器应该是应用程序中定义和映射的第一个过滤器。

这可以使用以下 web.xml 代码轻松完成：



```xml
    <listener>
        <listener-class>org.apache.logging.log4j.web.Log4jServletContextListener</listener-class>
    </listener>
 
    <filter>
        <filter-name>log4jServletFilter</filter-name>
        <filter-class>org.apache.logging.log4j.web.Log4jServletFilter</filter-class>
    </filter>
    <filter-mapping>
        <filter-name>log4jServletFilter</filter-name>
        <url-pattern>/*</url-pattern>
        <dispatcher>REQUEST</dispatcher>
        <dispatcher>FORWARD</dispatcher>
        <dispatcher>INCLUDE</dispatcher>
        <dispatcher>ERROR</dispatcher>
        <dispatcher>ASYNC</dispatcher><!-- Servlet 3.0 w/ disabled auto-initialization only; not supported in 2.5 -->
    </filter-mapping>
```



您可以使用 log4jContextName、log4jConfiguration 和/或 isLog4jContextSelectorNamed 上下文参数自定义侦听器和过滤器的行为。

在下面的上下文参数部分阅读更多相关信息



# Context Parameters

