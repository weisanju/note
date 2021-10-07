## 1. Spring WebFlux

 Spring WebFlux 是 5.0 之后加入的，它支持完全的 非阻塞、支持  [Reactive Streams](https://www.reactive-streams.org/)  背压、可以运行在 Netty、Undertow、以及 Servlet3.1+以上的 容器

[spring-webmvc](https://github.com/spring-projects/spring-framework/tree/main/spring-webmvc) and  [spring-webflux](https://github.com/spring-projects/spring-framework/tree/main/spring-webflux) 在 spring框架中并存、都是可选的、也可以都选，例如：使用 Spring MVC controllers 和 reactive WebClient



### 1.1. Overview

Why was Spring WebFlux created?

1. 非阻塞 web端  使用小部分线程 处理并发，能够以更小的硬件资源为代价 实现扩展 这部分的需求，Servlet3.1提供了非阻塞的API，但是使用它违背 rest 风格的Servlet API，这就是  需要一个  通用API 作为 任何非阻塞运行时的底层 
2. 另一个原因是 函数式编程：它 让非阻塞应用 和 流API 能够以 声明式的方式组合异步逻辑



#### 1.1.1. Define “Reactive”

我们提到 非阻塞、函数式。响应式是什么意思呢?

术语，`reactive` 引用自 编程模型 。围绕对变化的响应而构建：网络组件响应 IO事件、UI控制器 响应鼠标事件，非阻塞是响应式的，因为 我们正处于  一种 针对 操作完成 或  数据可用 进行响应的模型

还有另外一个跟响应式 关联的重要的机制：非阻塞背压，在同步的命令式代码中，阻塞的调用作为 一种自然的背压的形式，让调用者强制等待

在非阻塞代码中，控制事件的速率使得 生产者不会 压倒 目的地，Reactive Streams 是 一个小的规范，处理背压的方式是 让 订阅者 控制 产生元素的速率

> 如果一个生产者 没有减速： ReactorStream的机制是 建立一种机制跟边界，决定是否要 缓存、丢弃、或者 失败





#### 1.1.2. Reactive API

Reactive Stream 在互操作性上有着 很重要的作用，但是这是 库或者基础设施 组件所需要关注的 地方

对于应用程序来说，用处不是很大。因为它 太低级了。应用程序需要 更高级的 更丰富的API

Spring WebFlux 选用 Reactor 作为响应式框架



#### 1.1.3. Programming Models

**spring-web** 模块 包含了响应式基础模块，是构建 Spring WebFlux 的基础。包括 

* HTTP abstractions
* Reactive Streams [adapters](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-httphandler) for supported servers
* [codecs](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-codecs)
* a core [`WebHandler` API](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-web-handler-api) comparable to the Servlet API but with non-blocking contracts

在上述基础之上，提供两种基础模型

- [Annotated Controllers](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-controller): 和Spring-web模块中的 注解一致，SpringMVC 跟 WebFlux 控制器 支持 响应式的 返回值类型（Reactor and RxJava），因此也难以将两者分开，一个显著的区别是：WebFlux 支持 reactive `@RequestBody`  参数
- [Functional Endpoints](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-fn): 基于Lambda、轻量级、函数式编程模型，与基于注解的最大区别是：应用程序 负责从头到尾 的请求处理

#### 1.1.4. Applicability

Spring MVC or WebFlux?

两者协同工作，互补

![spring mvc and webflux venn](https://docs.spring.io/spring-framework/docs/current/reference/html/images/spring-mvc-and-webflux-venn.png)

要使用 MVC或者 WebFlux，请考虑以下几点

- 正在使用MVC、且工作得很好，不用换
- 如果你在对 非阻塞的 响应式web栈 进行技术选型
  - SpringWebFlux提供 servers可选（Netty, Tomcat, Jetty, Undertow, and Servlet 3.1+ containers）
  - 提供编程模式 可选（基于注解的、基于函数式）
  - 提供 响应式库 可选(Reactor, RxJava, or other)
- 检查应用程序的依赖：如果有 使用 阻塞式的API（JPA、JDBC）或者网络APIs，最好使用 SpringMVC，从技术上讲，Reactor在单独的线程执行 阻塞调用 是可行的，但没有充分利用非阻塞

- 建议先尝试使用 *WebClient*

  

#### 1.1.5. Servers

**支持servers**

Spring WebFlux 支持 Tomcat、jetty，Servlet3.1+ 容器，同样非Servlet运行环境例如 Netty、Undertow

所有servers 适配于 低级[common API](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-httphandler) ，这样 高级 [programming models](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-programming-models)  能够 跨 serves使用

**启停**

Spring WebFlux 没有对 启停服务 提供内置支持 但是很容器 从 Spring配置 和 [WebFlux infrastructure](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-config)   中 使用少量代码组装 并运行

Spring Boot has a WebFlux starter  自动完成这些步骤

**对Servlet3.1的依赖**

Spring MVC 依赖于 Servlet阻塞API  可以让应用程序  直接使用 ServletAPI

Spring WebFlux 依赖 Servlet3.1 非阻塞IO ，在一个较低级别的 适配器中 使用的，不能让应用程序直接使用





#### 1.1.6. Performance

性能有许多方面，Reactive 和非阻塞 不会 让应用程序运行更快，反而需要更多的工作，略微提供了处理事件。

核心的 收益是 ：以较少的线程和更低的内存 去缩放，使得应用程序在负载下更具弹性



#### **1.1.7. Configuring**

The Spring Framework does not provide support for starting and stopping [servers](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-server-choice). 

Spring不提供 servers的启停，配置一个 server的 线程模型你需要 使用 特定server的 配置API或者使用SpringBoot

检查SpringBoot的每个server的 配置选项

可以直接配置 [WebClient](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-client-builder)





# 杂项问题

## **springBoot是如何判断 webflux还是 webMVC，还是其他**

### **从 *classPath* 判断**

* *org.springframework.web.reactive.DispatcherHandler* 对应 webflux的核心类
* *org.springframework.web.servlet.DispatcherServlet* webMVC的核心类
* *org.glassfish.jersey.servlet.ServletContainer* jersey的核心类

### **如果三个都存在**

* 则判断 *javax.servlet.Servlet* *org.springframework.web.context.ConfigurableWebApplicationContext* 是否存在类路径
* 都存在则 使用 *SERVLET* 

```java
//org.springframework.boot.WebApplicationType#deduceFromClasspath
    static WebApplicationType deduceFromClasspath() {
		if (ClassUtils.isPresent(WEBFLUX_INDICATOR_CLASS, null) && !ClassUtils.isPresent(WEBMVC_INDICATOR_CLASS, null)
				&& !ClassUtils.isPresent(JERSEY_INDICATOR_CLASS, null)) {
			return WebApplicationType.REACTIVE;
		}
		for (String className : SERVLET_INDICATOR_CLASSES) {
			if (!ClassUtils.isPresent(className, null)) {
				return WebApplicationType.NONE;
			}
		}
		return WebApplicationType.SERVLET;
	}
```

### 根据 不同的 *WebApplicationType* 创建不同的 *SpringContext*

```java
//org.springframework.boot.ApplicationContextFactory
ApplicationContextFactory DEFAULT = (webApplicationType) -> {
   try {
      switch (webApplicationType) {
      case SERVLET:
         return new AnnotationConfigServletWebServerApplicationContext();
      case REACTIVE:
         return new AnnotationConfigReactiveWebServerApplicationContext();
      default:
         return new AnnotationConfigApplicationContext();
      }
   }
   catch (Exception ex) {
      throw new IllegalStateException("Unable create a default ApplicationContext instance, "
            + "you may need a custom ApplicationContextFactory", ex);
   }
};
```



