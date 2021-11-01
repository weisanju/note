# SpringApplication

`SpringApplication`  类提供了 启动 spring 应用程序 便利的 方式，从 main方法启动

通常调用 `SpringApplication.run`  方法

```java
@SpringBootApplication
public class MyApplication {
    public static void main(String[] args) {
        SpringApplication.run(MyApplication.class, args);
    }
}
```

**日志打印**

默认情况下 打印 *INFO* 级别日志， 包括相关的启动细节，例如 启动应用的用户，更改日志级别，详见：[Log Levels](https://docs.spring.io/spring-boot/docs/current/reference/html/features.html#features.logging.log-levels)

启动信息日志可以通过  `spring.main.log-startup-info`  关闭，这也会关闭application’s active profiles的打印

在启动时 加入 额外日志，可以在 `SpringApplication`    覆盖 `logStartupInfo(boolean)` 方法



# Startup Failure

如果启动失败，注册 `FailureAnalyzers` 可以 专用的错误消息，和修复问题的 精确 动作

例如 8080端口占用会显示以下信息

```
***************************
APPLICATION FAILED TO START
***************************
Description:
Embedded servlet container failed to start. Port 8080 was already in use.
Action:
Identify and stop the process that's listening on port 8080 or configure this application to listen on another port.
```

Spring Boot 提供很多 `FailureAnalyzer` 实现，自定义：[add your own](https://docs.spring.io/spring-boot/docs/current/reference/html/howto.html#howto.application.failure-analyzer)

如果没有analyzers 能 处理异常，可以展示详细信息，需要启用 debug 属性 [enable the `debug` property](https://docs.spring.io/spring-boot/docs/current/reference/html/features.html#features.external-config) or [enable `DEBUG` logging](https://docs.spring.io/spring-boot/docs/current/reference/html/features.html#features.logging.log-levels) 

`org.springframework.boot.autoconfigure.logging.ConditionEvaluationReportLoggingListener`

```shell
$ java -jar myproject-0.0.1-SNAPSHOT.jar --debug
```

# Lazy Initialization

`SpringApplication` 允许懒加载，懒加载可以减少启动时间，也会延迟错误的发现，确保JVM有足够内存容纳bean，最好在启动懒加载前调整队大小

在 `SpringApplicationBuilder`  中使用 `lazyInitialization` 可以编程式启用，或者调用`SpringApplication` 的 `setLazyInitialization` 

可以使用 `spring.main.lazy-initialization`  启用

如果您想禁用 某些bean 的懒加载，对其他启用懒加载，可以使用 `@Lazy(false)` 显示设置

# Customizing the Banner

* 启动时会读取   *classpath* 下的`banner.txt`，通过 `spring.banner.location` 指定

* `spring.banner.charset` 指定字符集
* 通过设置 `spring.banner.image.location`  可以添加图片 `banner.gif`, `banner.jpg`, or `banner.png`  

`banner.txt` 中可以使用 以下占位符

| Variable                                                     | Description                                                  |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| `${application.version}`                                     | `MANIFEST.MF`中定义的版本号，例如, `Implementation-Version: 1.0` is printed as `1.0` |
| `${application.formatted-version}`                           | `MANIFEST.MF`中定义的版本号： (surrounded with brackets and prefixed with `v`). For example `(v1.0)`. |
| `${spring-boot.version}`                                     | spring-boot版本号                                            |
| `${spring-boot.formatted-version}`                           | 格式化的版本号 (surrounded with brackets and prefixed with `v`). For example `(v2.5.5)`. |
| `${Ansi.NAME}` (or `${AnsiColor.NAME}`, `${AnsiBackground.NAME}`, `${AnsiStyle.NAME}`) | Where `NAME` is the name of an ANSI escape code. See [`AnsiPropertySource`](https://github.com/spring-projects/spring-boot/tree/v2.5.5/spring-boot-project/spring-boot/src/main/java/org/springframework/boot/ansi/AnsiPropertySource.java) for details. |
| `${application.title}`                                       | `MANIFEST.MF`中定义的 标题：For example `Implementation-Title: MyApp` is printed as `MyApp`. |

`SpringApplication.setBanner(…)`  方法 可以编程式设置 *banner*，实现自己的 `org.springframework.boot.Banner` 

`spring.main.banner-mode`  属性决定  *banner* 是否要 打印到 控制台

*banner* 被注册为 单例，名称叫：`springBootBanner`

`${application.version}`  和 `${application.formatted-version}` 只在 使用Spring Boot 可用。

如果使用 解压的jar，使用 `java -cp <classpath> <mainclass>`. 启动的则不能使用变量

建议使用 `java org.springframework.boot.loader.JarLauncher`.  启动 解压的jar 这会初始化  `application.*`  banner变量 



# Customizing SpringApplication

可以自定义 `SpringApplication` 

```java
@SpringBootApplication
public class MyApplication {
    public static void main(String[] args) {
        SpringApplication application = new SpringApplication(MyApplication.class);
        application.setBannerMode(Banner.Mode.OFF);
        application.run(args);
    }
}
```

构造参数 需要传递 bean的 配置源，大多数情况下是 `@Configuration` 配置类，也可以直接引用 `@Component` 类

外部化配置,详见：*[Externalized Configuration](https://docs.spring.io/spring-boot/docs/current/reference/html/features.html#features.external-config)* 

# Fluent Builder API

使用 `SpringApplicationBuilder` 链式构建 带层级的 *SpringApplication*

```java
  new SpringApplicationBuilder()
        .sources(Parent.class)
        .child(Application.class)
        .bannerMode(Banner.Mode.OFF)
        .run(args);
```

See the [`SpringApplicationBuilder` Javadoc](https://docs.spring.io/spring-boot/docs/2.5.5/api/org/springframework/boot/builder/SpringApplicationBuilder.html) for full details



# Application Availability



应用程序可以提供不同架构不同平台的信息，Spring Boot 提供开箱即用的 支持，包括常用的  *liveness* 跟 *readiness*  的可用状态

使用 Spring Boot’s actuator 支持这些状态的展示，此外，通过注入 `ApplicationAvailability`  接口到 bean中，获取 可用状态

## Liveness State

应用程序 的 Liveness  状态  告诉 其内部 状态 是否 允许它 正确工作，或者如果当前失败，自行恢复

A broken “Liveness” state  意味着 应用程序处于无法恢复的状态，基础架构平台 应该 重启应用程序

一般，Liveness 状态 不应该 基于 外部检查，例如：[Health checks](https://docs.spring.io/spring-boot/docs/current/reference/html/actuator.html#actuator.endpoints.health)

如果确实存在，失败的外部系统（database，webAPI，an external cache）会触发平台大规模重新启动和 级联故障

Spring Boot 应用的 内部状态 主要由 `ApplicationContext`表示

1. 如果 应用上下文 已经成功启动了，Spring Boot 则认为 该程序是 有效状态
2. 当 上下文刷新了，应用则认为是 活的

详见 [Spring Boot application lifecycle and related Application Events](https://docs.spring.io/spring-boot/docs/current/reference/html/features.html#features.spring-application.application-events-and-listeners).

## Readiness State

应用的 *Readiness* 状态 表明：应用程序是否已经准备好 处理流量，失败的 *Readiness* 表明：不应将流量路由到应用程序

这通常 发生在启动时刻，`CommandLineRunner`  和 `ApplicationRunner`  组件正被处理，或者 应用程序认为 它太忙 无法处理更多流量

一旦 应用程序和命令行运行者 被调用，应用程序就被认为准备就绪，详见： [Spring Boot application lifecycle and related Application Events](https://docs.spring.io/spring-boot/docs/current/reference/html/features.html#features.spring-application.application-events-and-listeners).

在 启动就要被执行的 任务，应该通过  `CommandLineRunner`   和 `ApplicationRunner` 组件执行，而不是使用  spring组件中的 生命周期的回调 （例如：`@PostConstruct`）执行

## Managing the Application Availability State

应用程序组件 可以 在任意时间获取 当前 可用性，通过注入 `ApplicationAvailability`  接口，在其上调用

更普遍的，应用程序 将会 监听 状态 变更，或者 更新应用程序的  状态

例如：可以将 应用的 *Readiness* 状态  导出到文件中， Kubernetes 即可 通过 *exec Probe* 查看

```java
@Component
public class MyReadinessStateExporter {

    @EventListener
    public void onStateChange(AvailabilityChangeEvent<ReadinessState> event) {
        switch (event.getState()) {
        case ACCEPTING_TRAFFIC:
            // create file /tmp/healthy
            break;
        case REFUSING_TRAFFIC:
            // remove file /tmp/healthy
            break;
        }
    }
}
```

当应用程序中断且无法恢复时，我们还可以更新应用程序状态：

```java
@Component
public class MyLocalCacheVerifier {

    private final ApplicationEventPublisher eventPublisher;

    public MyLocalCacheVerifier(ApplicationEventPublisher eventPublisher) {
        this.eventPublisher = eventPublisher;
    }

    public void checkLocalCache() {
        try {
            // ...
        }
        catch (CacheCompletelyBrokenException ex) {
            AvailabilityChangeEvent.publish(this.eventPublisher, ex, LivenessState.BROKEN);
        }
    }

}
```

Spring Boot provides [Kubernetes HTTP probes for "Liveness" and "Readiness" with Actuator Health Endpoints](https://docs.spring.io/spring-boot/docs/current/reference/html/actuator.html#actuator.endpoints.kubernetes-probes). You can get more guidance about [deploying Spring Boot applications on Kubernetes in the dedicated section](https://docs.spring.io/spring-boot/docs/current/reference/html/deployment.html#deployment.cloud.kubernetes).

# Application Events and Listeners

事件跟监听机制，除了常见的 SpringFramework 事件，例如：[`ContextRefreshedEvent`](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/context/event/ContextRefreshedEvent.html) ，`SpringApplication`  也会产生额外的事件

一些事件 实在 `ApplicationContext` 创建之前 触发的，不能通过@Bean 注册监听器，可以通过  `SpringApplication.addListeners(…)`  或者 `SpringApplicationBuilder.listeners(…)`  方法  注册监听器

如果想要其自动注册，则可以将 `META-INF/spring.factories`  加入到 classpath下，写法如下

`org.springframework.context.ApplicationListener=com.example.project.MyListener`



应用事件按以下顺序发送

1. An `ApplicationStartingEvent` is sent at the start of a run but before any processing, except for the registration of listeners and initializers.
2. An `ApplicationEnvironmentPreparedEvent` is sent when the `Environment` to be used in the context is known but before the context is created.
3. An `ApplicationContextInitializedEvent` is sent when the `ApplicationContext` is prepared and ApplicationContextInitializers have been called but before any bean definitions are loaded.
4. An `ApplicationPreparedEvent` is sent just before the refresh is started but after bean definitions have been loaded.
5. An `ApplicationStartedEvent` is sent after the context has been refreshed but before any application and command-line runners have been called.
6. An `AvailabilityChangeEvent` is sent right after with `LivenessState.CORRECT` to indicate that the application is considered as live.
7. An `ApplicationReadyEvent` is sent after any [application and command-line runners](https://docs.spring.io/spring-boot/docs/current/reference/html/features.html#features.spring-application.command-line-runner) have been called.
8. An `AvailabilityChangeEvent` is sent right after with `ReadinessState.ACCEPTING_TRAFFIC` to indicate that the application is ready to service requests.
9. An `ApplicationFailedEvent` is sent if there is an exception on startup.

The above list only includes `SpringApplicationEvent`s that are tied to a `SpringApplication`. In addition to these, the following events are also published after `ApplicationPreparedEvent` and before `ApplicationStartedEvent`:

- A `WebServerInitializedEvent` is sent after the `WebServer` is ready. `ServletWebServerInitializedEvent` and `ReactiveWebServerInitializedEvent` are the servlet and reactive variants respectively.
- A `ContextRefreshedEvent` is sent when an `ApplicationContext` is refreshed.

You often need not use application events, but it can be handy to know that they exist. Internally, Spring Boot uses events to handle a variety of tasks.

Event listeners should not run potentially lengthy tasks as they execute in the same thread by default. Consider using [application and command-line runners](https://docs.spring.io/spring-boot/docs/current/reference/html/features.html#features.spring-application.command-line-runner) instead.

Application events are sent by using Spring Framework’s event publishing mechanism. Part of this mechanism ensures that an event published to the listeners in a child context is also published to the listeners in any ancestor contexts. As a result of this, if your application uses a hierarchy of `SpringApplication` instances, a listener may receive multiple instances of the same type of application event.

To allow your listener to distinguish between an event for its context and an event for a descendant context, it should request that its application context is injected and then compare the injected context with the context of the event. The context can be injected by implementing `ApplicationContextAware` or, if the listener is a bean, by using `@Autowired`.

# Web Environment

`SpringApplication`  会自行 尝试创建   正确类型的  `ApplicationContext` ，用于确定`WebApplicationType`  的算法如下

- SpringMVC 在，则使用 `AnnotationConfigServletWebServerApplicationContext` 
- Spring MVC 不在，Spring WebFlux 在，则使用 `AnnotationConfigReactiveWebServerApplicationContext` 
- 否则使用 `AnnotationConfigApplicationContext`

通过 `setWebApplicationType(WebApplicationType)`设置web应用类型

可以完全控制 web的类型，通过：`ApplicationContext#setApplicationContextClass(…)`  

当使用 junit 单元测试时，可以 调用  `setWebApplicationType(WebApplicationType.NONE)` 



# Accessing Application Arguments

> 访问应用参数

你想要访问 传给`SpringApplication.run(…)` 的参数，可以注入  `org.springframework.boot.ApplicationArguments`  bean，`ApplicationArguments` 接口提供 原始的  `String[]`，或者访问 解析的`option` and `non-option`  参数

```java
@Component
public class MyBean {
    public MyBean(ApplicationArguments args) {
        boolean debug = args.containsOption("debug");
        List<String> files = args.getNonOptionArgs();
        if (debug) {
            System.out.println(files);
        }
        // if run with "--debug logfile.txt" prints ["logfile.txt"]
    }

}
```

SpringBoot同样 使用Spring `Environment`  注册 `CommandLinePropertySource`  ，这允许你 使用 `@Value` 注入应用程序参数



# Using the ApplicationRunner or CommandLineRunner

`ApplicationRunner` or `CommandLineRunner`  的代码 在 `SpringApplication.run(…)`  启动完之后，就执行

，此接口非常适合在应用程序启动后（但在开始接受流量之前）运行的任务。

`CommandLineRunner`  提供string数组，`ApplicationRunner` 是 `ApplicationArguments` 对象

```java
@Component
public class MyCommandLineRunner implements CommandLineRunner {
    @Override
    public void run(String... args) {
        // Do something...
    }
}
```

多个 bean对象实现了该接口 ，可以实现 `org.springframework.core.Ordered`接口，或者 `org.springframework.core.annotation.Order` 注解 实现排序

# Application Exit

每个 `SpringApplication` 应用 都与JVM 注册一个 关闭挂钩，确保 `ApplicationContext`  能优雅关闭，所有 Spring生命周期回调都可以使用（例如：DisposableBean 接口，或者 @PreDestroy 注解） ，另外可以使用 *ExitCodeGenerator* 接口 

```java
@SpringBootApplication
public class MyApplication {
    @Bean
    public ExitCodeGenerator exitCodeGenerator() {
        return () -> 42;
    }
    public static void main(String[] args) {
        System.exit(SpringApplication.exit(SpringApplication.run(MyApplication.class, args)));
    }
}
```



# Admin Features

可以通过 `spring.application.admin.enabled`属性开启 应用程序管理员功能

This exposes the [`SpringApplicationAdminMXBean`](https://github.com/spring-projects/spring-boot/tree/v2.5.5/spring-boot-project/spring-boot/src/main/java/org/springframework/boot/admin/SpringApplicationAdminMXBean.java) on the platform `MBeanServer`. 可以使用这个远程管理服务

如果您想知道应用程序正在运行哪个 HTTP 端口，使用 `local.server.port` 获取端口名

# Application Startup tracking

During the application startup, the `SpringApplication` and the `ApplicationContext` perform many tasks related to the application lifecycle, the beans lifecycle or even processing application events. 

在应用程序启动期间，`SpringApplication` 和  `ApplicationContext` 执行与 应用生命周期、bean生命周期 或者 处理应用事件 相关的许多任务，

使用 [`ApplicationStartup`](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/core/metrics/ApplicationStartup.html), Spring Framework 允许你 使用 [StartupStep](https://docs.spring.io/spring-framework/docs/5.3.10/reference/html/core.html#context-functionality-startup) 对象 记录应用启动序列，收集这些数据可以用于分析目的，或者只是为了更好的了解应用程序启动过程

`ApplicationStartup` 有多个实现，例如，可以使用`BufferingApplicationStartup`, 

```java
@SpringBootApplication
public class MyApplication {

    public static void main(String[] args) {
        SpringApplication application = new SpringApplication(MyApplication.class);
        application.setApplicationStartup(new BufferingApplicationStartup(2048));
        application.run(args);
    }

}
```

第一个可用的实现类是：`FlightRecorderApplicationStartup`  ，他将 spring特有的启动实现加入到 java Flight Recorder session中，用于分析应用程序并将其SpringContext 生命周期与 JVM 事件相关联（allocations, GCs, class loading）。一旦配置，可以通过 启用 Flight Recorder 记录数据

```shell
$ java -XX:StartFlightRecording:filename=recording.jfr,duration=10s -jar demo.jar
```

Spring Boot 实现了 `BufferingApplicationStartup` 变体，目的是用于输出到外部 指标系统，

Spring Boot can also be configured to expose a [`startup` endpoint](https://docs.spring.io/spring-boot/docs/2.5.5/actuator-api/htmlsingle/#startup) that provides this information as a JSON document.
