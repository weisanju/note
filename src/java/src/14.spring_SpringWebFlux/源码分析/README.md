

# 服务层

该层是各个 HTTP 服务器的 启动方法，服务就是从该层启动

## Netty

```java
HttpHandler handler = ...
ReactorHttpHandlerAdapter adapter = new ReactorHttpHandlerAdapter(handler);
HttpServer.create().host(host).port(port).handle(adapter).bind().block();
```

## Tomcat

```java
HttpHandler handler = ...
Servlet servlet = new TomcatHttpHandlerAdapter(handler);

Tomcat server = new Tomcat();
File base = new File(System.getProperty("java.io.tmpdir"));
Context rootContext = server.addContext("", base.getAbsolutePath());
Tomcat.addServlet(rootContext, "main", servlet);
rootContext.addServletMappingDecoded("/", "main");
server.setHost(host);
server.setPort(port);
server.start();
```



# 适配层

* 利用服务库的API实现响应式
* 不同 的 服务层中 有不同形式的 handler去处理请求与响应

## Netty

**需要的handler签名**

```java
BiFunction<HttpServerRequest, HttpServerResponse, Mono<Void>>
```

## Tomcat

```java
Servlet
```



# WebFlux处理层

WebFlux将处理逻辑转交给 *HttpHandler*

## *HttpWebHandlerAdapter*

* 将请求逻辑转统一定向到WebHandler
* 并提供 spring容器，会话管理，请求、国际化，编解码等的初步支持

## WebHandler

* 处理会话、容器、国际化、编解码等
* 将路由与 业务处理逻辑交给其他层



## 路由与业务处理

### *RouterFunctionWebHandler*

基于 *RouterFunction* 的路由

基于 *HandlerFunction* 的业务处理

### *DispatcherHandler*

**路由查找**

*HandlerMapping*  ：不同的路由方式 映射到 不同的 handler

* *RouterFunctionMapping*：基于 *RouterFunction* 的路由

* *AbstractUrlHandlerMapping*：基于Url匹配的路由

* *AbstractHandlerMethodMapping*：基于RequestMapping方法的路由

**业务处理**

由于不同的 *HandlerMapping*  返回不同的 handler，所以定义了 *HandlerAdapter* 去定义了如何调用 *handler*

* *RequestMappingHandlerAdapter*：处理 *AbstractHandlerMethodMapping*产生的 HandlerMethod
* *HandlerFunctionAdapter*：处理 *RouterFunctionMapping* 产生的 *HandlerFunction*



