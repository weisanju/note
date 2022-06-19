# DispatcherServlet

* Spring MVC, 与其他Web框架一样,基于  central `Servlet`的控制器模式， the `DispatcherServlet`, 提供了一个共享的 请求处理算法，实际工作交给 其他配置配置的组件执行，这个模型是可弹性的，可划分工作流的

* The `DispatcherServlet`, as any `Servlet`, 需要被申明，要么通过 Java配置指定，要么通过 web.xml 申明，然后, the `DispatcherServlet` 

    使用spring注解，发现实际处理请求的 组件, 例如 视图解析器, 异常处理以及其他

* 下面的Java配置注册，并实例化一个 `DispatcherServlet` ，被*ServletContainer* 自动 发现(see [Servlet Config](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-container-config)):

```java
public class MyWebApplicationInitializer implements WebApplicationInitializer {
    @Override
    public void onStartup(ServletContext servletContext) {
        // Load Spring web application configuration,实例化容器
        AnnotationConfigWebApplicationContext context = new AnnotationConfigWebApplicationContext();
        context.register(AppConfig.class);

        // Create and register the DispatcherServlet
        DispatcherServlet servlet = new DispatcherServlet(context); //注册servlet
        ServletRegistration.Dynamic registration = servletContext.addServlet("app", servlet);
        registration.setLoadOnStartup(1);
        registration.addMapping("/app/*");
    }
}
```

在 *Servlet3.0* 环境中，编程式配置 *ServletContext* 所要实现的接口 （与 *web.xml* 配置 相反）

这个 *SPI* 的实现类都会被 *SpringServletContainerInitializer* 自动检测到



# ContextHierarchy

## 介绍

* *DispatcherServlet* 需要 *WebApplicationContext* 作为配置来源
* 一般 一个 *WebApplicationContext*  对应一个 *Servlet* 
* 多个Servlet 对应 多个 *WebApplicationContext*  ，且共享 一个 *RootWebApplicationContext* ，共享一些基础设施的 Bean对象，例如 数据访问，通用业务服务

![servletContext继承图](../images/mvc-context-hierarchy.png)



## 配置*ContextHierarchy*

```java
public class MyWebAppInitializer extends AbstractAnnotationConfigDispatcherServletInitializer {

    @Override
    protected Class<?>[] getRootConfigClasses() {
        return new Class<?>[] { RootConfig.class };
    }

    @Override
    protected Class<?>[] getServletConfigClasses() {
        return new Class<?>[] { App1Config.class };
    }

    @Override
    protected String[] getServletMappings() {
        return new String[] { "/app1/*" };
    }
}
```

**相对应的XML配置**

```xml
<web-app>

    <listener>
        <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
    </listener>

    <context-param>
        <param-name>contextConfigLocation</param-name>
        <param-value>/WEB-INF/root-context.xml</param-value>
    </context-param>

    <servlet>
        <servlet-name>app1</servlet-name>
        <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
        <init-param>
            <param-name>contextConfigLocation</param-name>
            <param-value>/WEB-INF/app1-context.xml</param-value>
        </init-param>
        <load-on-startup>1</load-on-startup>
    </servlet>

    <servlet-mapping>
        <servlet-name>app1</servlet-name>
        <url-pattern>/app1/*</url-pattern>
    </servlet-mapping>

</web-app>

```



# Special Bean Types

* `DispatcherServlet` 委托各个不同的bean处理 不同的请求，渲染合适的响应
* `special beans` 意思是 spring管理的 对象
* 以下表格列出的 被 `DispatcherServlet` 检测到的   special beans 

| Bean type                                                    | Explanation                                                  |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| `HandlerMapping`                                             | 通过一系列 拦截器 将请求映射给 *handler* <br />不通的 *handleMapping* 映射逻辑各有不同<br />有两个主要的 实现 <br />`RequestMappingHandlerMapping` (支持@RequestMapping注解) <br />`SimpleUrlHandlerMapping` (维护 显示的 URLPattern To Handler的 注册) |
| `HandlerAdapter`                                             | Help the `DispatcherServlet` to invoke a handler mapped to a request, |
| [`HandlerExceptionResolver`](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-exceptionhandlers) | 异常处理策略, 通常把 异常 映射到 handler，HTML error views, or other targets. See [Exceptions](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-exceptionhandlers). |
| [`ViewResolver`](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-viewresolver) | Resolve logical `String`-based view names returned from a handler to an actual `View` with which to render to the response. See [View Resolution](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-viewresolver) and [View Technologies](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-view). |
| [`LocaleResolver`](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-localeresolver), [LocaleContextResolver](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-timezone) | 解决客户端国际化问题                                         |
| [`ThemeResolver`](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-themeresolver) | Resolve themes your web application can use — for example, to offer personalized layouts. See [Themes](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-themeresolver). |
| [`MultipartResolver`](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-multipart) | Abstraction for parsing a multi-part request (for example, browser form file upload) with the help of some multipart parsing library. See [Multipart Resolver](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-multipart). |
| [`FlashMapManager`](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-flash-attributes) | Store and retrieve the “input” and the “output” `FlashMap` that can be used to pass attributes from one request to another, usually across a redirect. See [Flash Attributes](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-flash-attributes). |

核心功能包括 handler映射，handler方法执行，handler异常处理，handler返回视图解析，

其他功能包括 客户端国际化，主题，文件上传，跨请求数据共享





# Web MVC Config

> 配置 sepcial beans

可以在容器中申明 以上提到的 *special beans* ,`DispatcherServlet`  会检查容器中的 *special bean* 如果不存在则使用声明在 `DispatcherServlet.properties` 的默认实现，



# Servlet Config

*Servlet3.0*+ 环境中，你可以选择以 编程方式申明 Servlet容器，或者 结合 *web.xml*，以下配置 注册了一个 `DispatcherServlet`

```java
import org.springframework.web.WebApplicationInitializer;

public class MyWebApplicationInitializer implements WebApplicationInitializer {

    @Override
    public void onStartup(ServletContext container) {
        XmlWebApplicationContext appContext = new XmlWebApplicationContext();
        appContext.setConfigLocation("/WEB-INF/spring/dispatcher-config.xml");
        
        ServletRegistration.Dynamic registration = container.addServlet("dispatcher", new DispatcherServlet(appContext));
        registration.setLoadOnStartup(1);
        registration.addMapping("/");
    }
}
```

`WebApplicationInitializer` 由springMVC提供的接口，确保你的实现能够被自动 使用，用来初始化 任何 *Servlet3* 容器，

抽象基类  `AbstractDispatcherServletInitializer` 使之更简单的 注册一个 `DispatcherServlet`  ，只要指定 servletMapping,和 `DispatcherServlet`  的 配置文件的位置，当然更推荐用以下方式 配置

**基于Java的Servlet配置**

```java
public class MyWebAppInitializer extends AbstractAnnotationConfigDispatcherServletInitializer {

    @Override
    protected Class<?>[] getRootConfigClasses() {
        return null;
    }

    @Override
    protected Class<?>[] getServletConfigClasses() {
        return new Class<?>[] { MyWebConfig.class };
    }

    @Override
    protected String[] getServletMappings() {
        return new String[] { "/" };
    }
}
```

**使用XML配置的方式**

```java
public class MyWebAppInitializer extends AbstractDispatcherServletInitializer {

    @Override
    protected WebApplicationContext createRootApplicationContext() {
        return null;
    }

    @Override
    protected WebApplicationContext createServletApplicationContext() {
        XmlWebApplicationContext cxt = new XmlWebApplicationContext();
        cxt.setConfigLocation("/WEB-INF/spring/dispatcher-config.xml");
        return cxt;
    }

    @Override
    protected String[] getServletMappings() {
        return new String[] { "/" };
    }
}
```

**给*Servlet*注册 *Filter***

```java
public class MyWebAppInitializer extends AbstractDispatcherServletInitializer {

    // ...

    @Override
    protected Filter[] getServletFilters() {
        return new Filter[] {
            new HiddenHttpMethodFilter(), new CharacterEncodingFilter() };
    }
}
```

每一个*Filter* 会使用 具体类名进行命名，自动映射到 `DispatcherServlet`

The `isAsyncSupported` protected method of `AbstractDispatcherServletInitializer`提供了一个 地方 在 `DispatcherServlet` 和所有映射到它身上的 *filters* 异步支持

默认为 *true*

如果你需要 更加定制化，则 `createDispatcherServlet`  方法



# Processing

The `DispatcherServlet` 请求处理过程

- 查询 `WebApplicationContext`  ，并绑定在 请求中，作为一个属性，供*Controller* 和其他元素使用，默认 绑定的Key是   `DispatcherServlet.WEB_APPLICATION_CONTEXT_ATTRIBUTE` 
- 给请求绑定 *locale resolver*  ，以便 让 元素 在处理过程中， 解析 locale
- theme resolver 绑定主题解析器 给请求，  能构让 视图呈现不通的布局样式
- 如果你指定了 multipart file resolver, 就会检查请求是否 是*multipart*. 如果是*multipart* ，将会以`MultipartHttpServletRequest` 包装. See [Multipart Resolver](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-multipart) for further information about multipart handling.
- 查找 指定的 *Handler*. 如果找到 ，跟执行链相关的 handler（预处理器，后处理器，控制器等）会被执行以渲染 视图响应，对于基础注解的控制器，响应能够被在 `HandlerAdapter` 中渲染，而不需要返回一个视图
- 如果 *model* 返回，则渲染视图，如果没有model返回，则不渲染视图，因为请求可能已经被 处理了



`HandlerExceptionResolver`  是为了处理异常，

`DispatcherServlet` 同样支持 指定返回的  `last-modification-date` 通过Servlet API提供

 确定特定请求的 上次修改日期 很简单，`DispatcherServlet`查找合适的 *handler*时，会判断它是否 实现了`LastModified` 接口，如果实现了则返回给 客户端

你能够自定义 独立的`DispatcherServlet`  实例，通过 向 `web.xml` servlet申明中，添加  Servlet initialization parameters

以下是支持的参数：

| Parameter                        | Explanation                                                  |
| :------------------------------- | :----------------------------------------------------------- |
| `contextClass`                   | spring容器的类，必须要实现 `ConfigurableWebApplicationContext`，由该 *Servlet*实例化<br />默认使用 *XmlWebApplicationContext* |
| `contextConfigLocation           | XML配置路径<br />逗号分隔支持的多个Context. 重复定义的bean，最近优先 |
| `namespace`                      | Namespace of the `WebApplicationContext`. Defaults to `[servlet-name]-servlet`. |
| `throwExceptionIfNoHandlerFound` | 如果没有handler找到，要不要抛  `NoHandlerFoundException` <br />这个异常可以被 `HandlerExceptionResolver`捕获<br />(例如, by using an `@ExceptionHandler` controller method) <br />默认false, <br /> 如果false,`DispatcherServlet` sets the response status to 404 (NOT_FOUND) without raising an exception.<br />Note that, if [default servlet handling](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-default-servlet-handler) is also configured, unresolved requests are always forwarded to the default servlet and a 404 is never raised. |



# Interception

所有 `HandlerMapping` 的实现都支持 拦截器，例如 身份检查。

拦截器实现 `HandlerInterceptor`   `org.springframework.web.servlet` 包下。

- `preHandle(..)`: Before the actual handler is run
    - 返回true才能继续执行
    - 返回true，`DispatcherServlet` 认为 拦截器本身已处理了请求，然后渲染合适的视图
- `postHandle(..)`: After the handler is run
- `afterCompletion(..)`: After the complete request has finished

See [Interceptors](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-config-interceptors) in the section on MVC configuration for examples of how to configure interceptors. 

你可以在 各自`HandlerMapping`  实现里 使用 setters 直接注册 

**注意** `postHandle` 和  `@ResponseBody` and `ResponseEntity` 方法一起使用用处不大，因为响应已经在 *posthandler* 执行前 被 写入，提交

，无法对 响应做改变。例如添加额外的头，对于这种场景，建议使用  [Controller Advice](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-ann-controller-advice) ，要么实现 `ResponseBodyAdvice`  要么申明为 *ControllerAdvice* 要么 直接配置在 `RequestMappingHandlerAdapter`





# Exceptions

## 异常实现类

如果在执行请求处理过程中发生异常，`DispatcherServlet` 将它 交给 `HandlerExceptionResolver`  bean做处理，

以下表列出了 `HandlerExceptionResolver` implementations:

| `HandlerExceptionResolver`                                   | Description                                                  |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| `SimpleMappingExceptionResolver`                             | 异常类与 错误视图的映射处理器                                |
| [`DefaultHandlerExceptionResolver`](https://docs.spring.io/spring-framework/docs/5.3.3/javadoc-api/org/springframework/web/servlet/mvc/support/DefaultHandlerExceptionResolver.html) | Resolves exceptions raised by Spring MVC and maps them to HTTP status codes. See also alternative `ResponseEntityExceptionHandler` and [REST API exceptions](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-ann-rest-exceptions). |
| `ResponseStatusExceptionResolver`                            | Resolves exceptions with the `@ResponseStatus` annotation and maps them to HTTP status codes based on the value in the annotation. |
| `ExceptionHandlerExceptionResolver`                          | Resolves exceptions by invoking an `@ExceptionHandler` method in a `@Controller` or a `@ControllerAdvice` class. See [@ExceptionHandler methods](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-ann-exceptionhandler). |

## Chain of Resolvers

You can form an exception resolver chain by declaring multiple `HandlerExceptionResolver` beans in your Spring configuration

您可以通过在Spring配置中声明多个`HandlerExceptionResolver` bean来形成异常解析器链。必要时可以设置顺序，顺序越高，链中的位置越后

`HandlerExceptionResolver`接口的返回值

- a `ModelAndView` that points to an error view.
- An empty `ModelAndView` if the exception was handled within the resolver.
- `null` if the exception remains unresolved, for subsequent resolvers to try, and, if the exception remains at the end, it is allowed to bubble up to the Servlet container.

The [MVC Config](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-config) automatically declares built-in resolvers for default Spring MVC exceptions, for `@ResponseStatus` annotated exceptions, and for support of `@ExceptionHandler` methods. You can customize that list or replace it.



## Container Error Page

If an exception remains unresolved by any `HandlerExceptionResolver` and is,

如果任何 异常处理 都没有解决异常，则让它继续传播，Servlet containers能够渲染 默认的HTML错误视图，自定义容器默认错误页，可以在 *web.xml* 指定错误页

```xml
<error-page>
    <location>/error</location>
</error-page>
```

Given the preceding example, when an exception bubbles up or the response has an error status, 

the Servlet container makes an ERROR dispatch within the container to the configured URL (for example, `/error`). This is then processed by the `DispatcherServlet`, possibly mapping it to a `@Controller`, which could be implemented to return an error view name with a model or to render a JSON response, as the following example shows:

基于前面的示例，当异常冒出或响应具有错误状态时，Servlet容器在容器内向配置的URL（例如，/ error）进行ERROR调度。
然后由DispatcherServlet处理，可能将其映射到@Controller，可以实现返回错误视图名称或呈现JSON响应，如以下示例所示：

```java
@RestController
public class ErrorController {

    @RequestMapping(path = "/error")
    public Map<String, Object> handle(HttpServletRequest request) {
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("status", request.getAttribute("javax.servlet.error.status_code"));
        map.put("reason", request.getAttribute("javax.servlet.error.message"));
        return map;
    }
}
```

>  The Servlet API does not provide a way to create error page mappings in Java. You can, however, use both a `WebApplicationInitializer` and a minimal `web.xml`.

# View Resolution

## ViewResolver

`ViewResolver` 提供 view 名称与 实际 view的映射关系

在移交给特定的视图渲染技术之前。  `View` 主要是用来准备数据 

以下是 `ViewResolver`的继承结构

| ViewResolver                     | Description                                                  |
| :------------------------------- | :----------------------------------------------------------- |
| `AbstractCachingViewResolver`    | 视图缓存                                                     |
| `UrlBasedViewResolver`           | 将URL直接映射到 视图.                                        |
| `InternalResourceViewResolver`   | Convenient subclass of `UrlBasedViewResolver` that supports `InternalResourceView` (in effect, Servlets and JSPs) and subclasses such as `JstlView` and `TilesView`. You can specify the view class for all views generated by this resolver by using `setViewClass(..)`. |
| `FreeMarkerViewResolver`         | Convenient subclass of `UrlBasedViewResolver` that supports `FreeMarkerView` and custom subclasses of them. |
| `ContentNegotiatingViewResolver` | Implementation of the `ViewResolver` interface that resolves a view based on the request file name or `Accept` header. See [Content Negotiation](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-multiple-representations). |
| `BeanNameViewResolver`           | Implementation of the `ViewResolver` interface that interprets a view name as a bean name in the current application context. This is a very flexible variant which allows for mixing and matching different view types based on distinct view names. Each such `View` can be defined as a bean e.g. in XML or in configuration classes. |





# Locale

spring大部分 架构支持国际化，springMVC也支持

`DispatcherServlet` 根据 客户端的 *locale* 自动解析消息，通过 `LocaleResolver`  实现

当请求过来时，`DispatcherServlet` 查找 *locale resolver*  ，一旦找到一个，则尝试设置 *locale*

使用  `RequestContext.getLocale()` 可也始终获得  locale resolve 解析的 *locale*

另外，为了自动 locale解析，可以给 *handlerMapping* 添加 locale解析器，这适用于 根据 请求参数 改变*locale*



以下类是常见的 locale解析实现， 定义在 org.springframework.web.servlet.i18n 包下，

- Time Zone

In addition to obtaining the client’s locale, it is often useful to know its time zone. The `LocaleContextResolver` interface offers an extension to `LocaleResolver` that lets resolvers provide a richer `LocaleContext`, which may include time zone information.

When available, the user’s `TimeZone` can be obtained by using the `RequestContext.getTimeZone()` method. Time zone information is automatically used by any Date/Time `Converter` and `Formatter` objects that are registered with Spring’s `ConversionService`.

- Header Resolver

解析客户端传过来的  `accept-language` 请求头，不支持时区信息

- Cookie Resolver

    - 检查 *Cookie* 钟可能会存在的   `Locale` or `TimeZone`  

    ```xml
    <bean id="localeResolver" class="org.springframework.web.servlet.i18n.CookieLocaleResolver">
    
        <property name="cookieName" value="clientlanguage"/>
    
        <!-- in seconds. If set to -1, the cookie is not persisted (deleted when browser shuts down) -->
        <property name="cookieMaxAge" value="100000"/>
    
    </bean>
    ```

    | Property       | Default                   | Description                                                  |
    | :------------- | :------------------------ | :----------------------------------------------------------- |
    | `cookieName`   | classname + LOCALE        | The name of the cookie                                       |
    | `cookieMaxAge` | Servlet container default | The maximum time a cookie persists on the client. If `-1` is specified, the cookie will not be persisted. It is available only until the client shuts down the browser. |
    | `cookiePath`   | /                         | Limits the visibility of the cookie to a certain part of your site. When `cookiePath` is specified, the cookie is visible only to that path and the paths below it. |



- Session Resolver

从*session* 中取 *Locale and TimeZone* ，将本地请求的语言环境，设置在 *Session*

- Locale Interceptor

可以给 任何`HandlerMapping`  定义 应用 该 拦截器，

它检查请求中的参数，并改变*locale* ，以下是实例

```xml
<bean id="localeChangeInterceptor"
        class="org.springframework.web.servlet.i18n.LocaleChangeInterceptor">
    <property name="paramName" value="siteLanguage"/>
</bean>

<bean id="localeResolver"
        class="org.springframework.web.servlet.i18n.CookieLocaleResolver"/>

<bean id="urlMapping"
        class="org.springframework.web.servlet.handler.SimpleUrlHandlerMapping">
    <property name="interceptors">
        <list>
            <ref bean="localeChangeInterceptor"/>
        </list>
    </property>
    <property name="mappings">
        <value>/**/*.view=someController</value>
    </property>
</bean>
```

# Themes

您可以应用Spring Web MVC框架主题来设置应用程序的整体外观

主题是 静态资源的集合，包括样式表，图片等等

## Defining a theme

首先实现 `org.springframework.ui.context.ThemeSource` 接口

`WebApplicationContext`  实现了 `ThemeSource`接口，但它的实现委托给了特定的实现，默认是  `org.springframework.ui.context.support.ResourceBundleThemeSource` 这个实现，从 *classpath* 根路径加载资源

自定义实现，需要主动往容器中 注入 一个 themeSource

当您使用`ResourceBundleThemeSource`时，将在一个简单的属性文件中定义一个主题。
属性文件列出了组成主题的资源，如以下示例所示：

```
styleSheet=/themes/cool/style.css
background=/themes/cool/img/coolBg.jpg
```

属性的键是从 视图 引用主题元素的名称。

对于  JSP 使用 `spring:theme`  标签

```html
<%@ taglib prefix="spring" uri="http://www.springframework.org/tags"%>
<html>
    <head>
        <link rel="stylesheet" href="<spring:theme code='styleSheet'/>" type="text/css"/>
    </head>
    <body style="background=<spring:theme code='background'/>">
        ...
    </body>
</html>
```

默认的，`ResourceBundleThemeSource` 使用空 前缀，从 *classpath* 加载*properties files*

## Resolving Themes

定义完之后，要使用，`DispatcherServlet`查找 名为 `themeResolver` 的bean，以下是已有的实现

| Class                  | Description                                                  |
| :--------------------- | :----------------------------------------------------------- |
| `FixedThemeResolver`   | Selects a fixed theme, set by using the `defaultThemeName` property. |
| `SessionThemeResolver` | The theme is maintained in the user’s HTTP session. It needs to be set only once for each session but is not persisted between sessions. |
| `CookieThemeResolver`  | The selected theme is stored in a cookie on the client.      |

Spring also provides a `ThemeChangeInterceptor` that lets theme changes on every request with a simple request parameter.



# Multipart Resolver

`MultipartResolver`  属于  `org.springframework.web.multipart`  包，是 解析 多部件请求包括文件上传 的 策略类

有一个实现类是 基于 [Commons FileUpload](https://jakarta.apache.org/commons/fileupload)   有一个是 基于  Servlet 3.0 multipart request parsing

要启用 多部件解析，需要声明 `MultipartResolver`  ，且bean名 为 `multipartResolver`

When a POST with content-type of `multipart/form-data` is received，`HttpServletRequest` 会被包装成`MultipartHttpServletRequest` 

##### Apache Commons `FileUpload`

使用  Apache Commons `FileUpload`，需要配置bean名为   `multipartResolver`  的  `CommonsMultipartResolver` 

需要引入 `commons-fileupload`依赖

##### Servlet 3.0

Servlet 3.0 multipart parsing needs to be enabled through Servlet container configuration. To do so:

- In Java, set a `MultipartConfigElement` on the Servlet registration.注册*MultipartConfigElement* 

```java
public class AppInitializer extends AbstractAnnotationConfigDispatcherServletInitializer {

    // ...

    @Override
    protected void customizeRegistration(ServletRegistration.Dynamic registration) {

        // Optionally also set maxFileSize, maxRequestSize, fileSizeThreshold
        registration.setMultipartConfig(new MultipartConfigElement("/tmp"));
    }

}
```

- In `web.xml`, add a `"<multipart-config>"` section to the servlet declaration.

一旦 Servlet3.0配置好之后, you can add a bean of type `StandardServletMultipartResolver` with a name of `multipartResolver`.



# Logging

Spring MVC中的DEBUG级别的日志记录旨在紧凑，最少且人性化

**Sensitive Data**

DEBUG and TRACE logging may log sensitive information. 

需要启用 `DispatcherServlet` 的 `enableLoggingRequestDetails` 属性

```java
public class MyInitializer
        extends AbstractAnnotationConfigDispatcherServletInitializer {

    @Override
    protected Class<?>[] getRootConfigClasses() {
        return ... ;
    }

    @Override
    protected Class<?>[] getServletConfigClasses() {
        return ... ;
    }

    @Override
    protected String[] getServletMappings() {
        return ... ;
    }

    @Override
    protected void customizeRegistration(ServletRegistration.Dynamic registration) {
        registration.setInitParameter("enableLoggingRequestDetails", "true");
    }

}
```

