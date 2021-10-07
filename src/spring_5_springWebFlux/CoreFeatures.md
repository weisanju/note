# Reactive Core

对于响应式 web应用，spring-web模块 包含以下 几个 基础支持

- 对于服务端请求处理，有两个级别的支持
  - [HttpHandler](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-httphandler): 使用非阻塞IO处理Http请求的基本协定  还有 Reactive Streams back pressure, along with adapters for Reactor Netty, Undertow, Tomcat, Jetty, and any Servlet 3.1+ container.
  - [`WebHandler` API](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-web-handler-api): 稍微高一点的API, 用于请求处理的通用 Web API,具体的编程模型 在这个基础上 构建（例如：基于注解、函数式）
- 对于客户端, `ClientHttpConnector` 是一个基本的 非阻塞的 执行 Http请求的约定 并且 Reactive Streams back pressure, along with adapters for [Reactor Netty](https://github.com/reactor/reactor-netty), reactive [Jetty HttpClient](https://github.com/jetty-project/jetty-reactive-httpclient) and [Apache HttpComponents](https://hc.apache.org/). 
  - 更高级的 API： [WebClient](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-client) 基于 此协定
- 对于客户端 服务端 都可用的是, [codecs](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-codecs) for serialization and deserialization of HTTP request and response content.





# `HttpHandler`

[HttpHandler](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/http/server/reactive/HttpHandler.html)  是一个简单的 接口，只有一个方法处理 请求和响应

以下表格 表明支持的 *HttpHandler*

| Server name           | Server API used                                              | Reactive Streams support                                     |
| :-------------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| Netty                 | Netty API                                                    | [Reactor Netty](https://github.com/reactor/reactor-netty)    |
| Undertow              | Undertow API                                                 | spring-web: Undertow to Reactive Streams bridge              |
| Tomcat                | Servlet 3.1 non-blocking I/O; Tomcat API to read and write ByteBuffers vs byte[] | spring-web: Servlet 3.1 non-blocking I/O to Reactive Streams bridge |
| Jetty                 | Servlet 3.1 non-blocking I/O; Jetty API to write ByteBuffers vs byte[] | spring-web: Servlet 3.1 non-blocking I/O to Reactive Streams bridge |
| Servlet 3.1 container | Servlet 3.1 non-blocking I/O                                 | spring-web: Servlet 3.1 non-blocking I/O to Reactive Streams bridge |

以下表格描述了 依赖以及版本

| Server name   | Group id                | Artifact name               |
| :------------ | :---------------------- | :-------------------------- |
| Reactor Netty | io.projectreactor.netty | reactor-netty               |
| Undertow      | io.undertow             | undertow-core               |
| Tomcat        | org.apache.tomcat.embed | tomcat-embed-core           |
| Jetty         | org.eclipse.jetty       | jetty-server, jetty-servlet |



## CodeExample

### ReactorNetty Code

```java
HttpHandler handler = ...
ReactorHttpHandlerAdapter adapter = new ReactorHttpHandlerAdapter(handler);
HttpServer.create().host(host).port(port).handle(adapter).bind().block();
```

### Tomcat

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

### **Servlet 3.1+ Container**

To deploy as a WAR to any Servlet 3.1+ container, you can extend and include [AbstractReactiveWebInitializer](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/web/server/adapter/AbstractReactiveWebInitializer.html) in the WAR. That class wraps an HttpHandler with ServletHttpHandlerAdapter and registers that as a Servlet.



# `WebHandler` 

## webhandler作用

*WebHandler* 主要是为了 提供  通用 web-APi，通过 多个 [WebExceptionHandler](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/web/server/WebExceptionHandler.html) 多个  [`WebFilter`](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/web/server/WebFilter.html) 和一个  [`WebHandler`](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/web/server/WebHandler.html) 组件

这些链条可以通过  `WebHttpHandlerBuilder` 中设置 *ApplicationContext* ，会自动检测 或者 手动注册



HttpHandler主要是为了处理 不同 HttpServer的抽象使用

WebHandler旨在 提供 web应用中 更广泛的使用

* *session with attribute* 
* *request attribute*
* 本地化：*Resolved `Locale` or `Principal` for the request.*
* 对表单的解析、缓存、访问
* multipart data的抽象
* and more..

## Special bean types

下表列出了 WebHttpHandlerBuilder 能够自动检测容器中的bean类型的列表，或者能够直接注册

| Bean name                    | Bean type                    | Count | Description                                                  |
| :--------------------------- | :--------------------------- | :---- | :----------------------------------------------------------- |
| <any>                        | `WebExceptionHandler`        | 0..N  | Provide handling for exceptions from the chain of `WebFilter` instances and the target `WebHandler`. For more details, see [Exceptions](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-exception-handler). |
| <any>                        | `WebFilter`                  | 0..N  | Apply interception style logic to before and after the rest of the filter chain and the target `WebHandler`. For more details, see [Filters](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-filters). |
| `webHandler`                 | `WebHandler`                 | 1     | The handler for the request.                                 |
| `webSessionManager`          | `WebSessionManager`          | 0..1  | The manager for `WebSession` instances exposed through a method on `ServerWebExchange`. `DefaultWebSessionManager` by default. |
| `serverCodecConfigurer`      | `ServerCodecConfigurer`      | 0..1  | For access to `HttpMessageReader` instances for parsing form data and multipart data that is then exposed through methods on `ServerWebExchange`. `ServerCodecConfigurer.create()` by default. |
| `localeContextResolver`      | `LocaleContextResolver`      | 0..1  | The resolver for `LocaleContext` exposed through a method on `ServerWebExchange`. `AcceptHeaderLocaleContextResolver` by default. |
| `forwardedHeaderTransformer` | `ForwardedHeaderTransformer` | 0..1  | For processing forwarded type headers, either by extracting and removing them or by removing them only. Not used by default. |

## Form Data

`ServerWebExchange` 暴露了下面方法 以访问 FormData:

```java
Mono<MultiValueMap<String, String>> getFormData();
```

The `DefaultServerWebExchange` uses the configured `HttpMessageReader` to parse form data (`application/x-www-form-urlencoded`) into a `MultiValueMap`.

 By default, `FormHttpMessageReader` is configured for use by the `ServerCodecConfigurer` bean (see the [Web Handler API](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-web-handler-api)).

## Multipart Data

`ServerWebExchange` exposes the following method for accessing multipart data:

```java
Mono<MultiValueMap<String, Part>> getMultipartData();
```

**默认使用 *HttpMessageReader***

The `DefaultServerWebExchange` uses the configured `HttpMessageReader<MultiValueMap<String, Part>>` to parse `multipart/form-data` content into a `MultiValueMap`. By default, this is the `DefaultPartHttpMessageReader`, which does not have any third-party dependencies. 

**使用SynchronossPartHttpMessageReader**

Alternatively, the `SynchronossPartHttpMessageReader` can be used, which is based on the [Synchronoss NIO Multipart](https://github.com/synchronoss/nio-multipart) library. Both are configured through the `ServerCodecConfigurer` bean (see the [Web Handler API](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-web-handler-api)).

**使用 StreamingFashion**

To parse multipart data in streaming fashion, you can use the `Flux<Part>` returned from an `HttpMessageReader<Part>` instead. For example, in an annotated controller, use of `@RequestPart` implies `Map`-like access to individual parts by name and, hence, requires parsing multipart data in full. By contrast, you can use `@RequestBody` to decode the content to `Flux<Part>` without collecting to a `MultiValueMap`.

## Forwarded Headers

**代理改变服务器信息**

由于请求会走代理，host、port、scheme、可能会变。这导致 客户端无法连接正确的服务端

**原始信息**

[RFC 7239](https://tools.ietf.org/html/rfc7239)  定义了  `Forwarded` HTTP header，代理可以提供 原始请求的信息，也有非标准的：including `X-Forwarded-Host`, `X-Forwarded-Port`, `X-Forwarded-Proto`, `X-Forwarded-Ssl`, and `X-Forwarded-Prefix`.



**将Forwarded 头 替代为 request中信息**

* `ForwardedHeaderTransformer`  基于 转发的headers 修改 host、port、scheme

* 移除这些转发的头
* 如果申明为 *forwardedHeaderTransformer* bean 则会自动检测

对于 forwarded headers 这是处于安全问题考虑的，因为 应用程序 无法知道 请求头是被代理添加的还是 被 可疑的客户端

这也就是为什么 一个 边界区域的代理 应配置 移除外部不受信任的转发流量

你可以设置为  `removeOnly=true` ：这意味着 只删除不适用

**向下兼容**

从5.1 *ForwardedHeaderFilter* 已过时，被 *ForwardedHeaderTransformer* 取代，在 exchange创建之前 以尽早处理  forwarded headers

如果配置了 *ForwardedHeaderFilter* 会把它拿出来、然后 使用 *ForwardedHeaderTransformer*

# Filters

在 WebHandler的API中、使用 *WebFilter* 应用 拦截器样式逻辑，在 处理链中的 前、后、剩余部分 以及 目标 *webHandler*

当使用  [WebFlux Config](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-config), 时 通过 容器自动注册 或者 手动、使用 @Order 注解 或者 实现 Ordered接口 来表明 先后顺序

**CORS**

Spring WebFlux provides fine-grained support for CORS configuration through annotations on controllers. However, when you use it with Spring Security, 

Spring WebFlux 提供了 细粒度的 CORS 配置支持 通过 controllers上的注解

当使用Spring Security 时呢？，我们建议使用 内置的`CorsFilter`  必须在 Spring Security’s 其他过滤器之前

See the section on [CORS](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-cors) and the [webflux-cors.html](https://docs.spring.io/spring-framework/docs/current/reference/html/webflux-cors.html#webflux-cors-webfilter) for more details.



# Exceptions

In the [`WebHandler` API](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-web-handler-api), you can use a `WebExceptionHandler` to handle exceptions from the chain of `WebFilter` instances and the target `WebHandler`. When using the [WebFlux Config](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-config), registering a `WebExceptionHandler` is as simple as declaring it as a Spring bean and (optionally) expressing precedence by using `@Order` on the bean declaration or by implementing `Ordered`.

The following table describes the available `WebExceptionHandler` implementations:

| Exception Handler                       | Description                                                  |
| :-------------------------------------- | :----------------------------------------------------------- |
| `ResponseStatusExceptionHandler`        | Provides handling for exceptions of type [`ResponseStatusException`](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/web/server/ResponseStatusException.html) by setting the response to the HTTP status code of the exception. |
| `WebFluxResponseStatusExceptionHandler` | Extension of `ResponseStatusExceptionHandler` that can also determine the HTTP status code of a `@ResponseStatus` annotation on any exception.This handler is declared in the [WebFlux Config](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-config). |







# Codecs

## 介绍

The `spring-web` and `spring-core` 模块提供 序列化与反序列化，从字节码到高级对象之间、通过非阻塞IO、使用Reactive Streams back pressure.

以下是详情

- [`Encoder`](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/core/codec/Encoder.html) and [`Decoder`](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/core/codec/Decoder.html) 是独立于 HTTP 对内容进行编码和解码的低级协定
- [`HttpMessageReader`](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/http/codec/HttpMessageReader.html) and [`HttpMessageWriter`](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/http/codec/HttpMessageWriter.html) 是编码和解码 HTTP 消息内容的协定
- An `Encoder` can be wrapped with `EncoderHttpMessageWriter` to adapt it for use in a web application, while a `Decoder` can be wrapped with `DecoderHttpMessageReader`.
- [`DataBuffer`](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/core/io/buffer/DataBuffer.html) abstracts different byte buffer representations (e.g. Netty `ByteBuf`, `java.nio.ByteBuffer`, etc.) and is what all codecs work on. See [Data Buffers and Codecs](https://docs.spring.io/spring-framework/docs/current/reference/html/core.html#databuffers) in the "Spring Core" section for more on this topic.

**spring默认实现**

The `spring-core` 模块提供 `byte[]`, `ByteBuffer`, `DataBuffer`, `Resource`, and `String`  encoder、decoder实现

 The `spring-web` module provides Jackson JSON, Jackson Smile, JAXB2, Protocol Buffers and other encoders and decoders along with web-only HTTP message reader and writer implementations for form data, multipart content, server-sent events, and others.

`ClientCodecConfigurer` and `ServerCodecConfigurer` are typically used to configure and customize the codecs to use in an application. See the section on configuring [HTTP message codecs](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-config-message-codecs).

## Jackson JSON

当 jackson library 存在时  JSON and binary JSON 支持

 `Jackson2Decoder` 工作

* Jackson’s 异步非阻塞的 parser 用于 byte chunks 聚合 成 `TokenBuffer`，代表一个 JSON Object
* 每一个 TokenBuffer 传给 ObjectMapper 用来船舰 高等级的对象
* When decoding to a multi-value publisher (e.g. `Flux`), each `TokenBuffer` is passed to the `ObjectMapper` as soon as enough bytes are received for a fully formed object. The input content can be a JSON array, or any [line-delimited JSON](https://en.wikipedia.org/wiki/JSON_streaming) format such as NDJSON, JSON Lines, or JSON Text Sequences.

`Jackson2Encoder` 工作如下:

- For a single value publisher (e.g. `Mono`), simply serialize it through the `ObjectMapper`.
- For a multi-value publisher with `application/json`, by default collect the values with `Flux#collectToList()` and then serialize the resulting collection.
- For a multi-value publisher with a streaming media type such as `application/x-ndjson` or `application/stream+x-jackson-smile`, encode, write, and flush each value individually using a [line-delimited JSON](https://en.wikipedia.org/wiki/JSON_streaming) format. Other streaming media types may be registered with the encoder.
- For SSE the `Jackson2Encoder` is invoked per event and the output is flushed to ensure delivery without delay.

`Jackson2Encoder` and `Jackson2Decoder` 不支持 *string*

## Form Data

`FormHttpMessageReader` and `FormHttpMessageWriter`  支持 `application/x-www-form-urlencoded`  的编码与解码

在服务器端，表单内容通常需要在多个位置访问， `ServerWebExchange` 提供专门的 `getFormData()`  方法 ，使用 *FormHttpMessageReader* 解析 重复访问会缓存结果 See [Form Data](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-form-data) in the [`WebHandler` API](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-web-handler-api) section.

一旦调用 *getFormData()* 那么无法从 request body中 读取原始内容，因此，应用程序应该始终通过 *ServerWebExchange* 一致的访问 缓存的 form data而不是从 原始请求体读取数据

## Multipart

`MultipartHttpMessageReader` and `MultipartHttpMessageWriter`  支持 编码与解码  *multipart/form-data* 内容

`MultipartHttpMessageReader`  将实际 解析 Flux<Part\> 的工作 交给 另一个 *HttpMessageReader*，然后简单的 将其收集成 *MultiValueMap*

默认实现为 ：`DefaultPartHttpMessageReader` ，通过 *ServerCodecConfigurer* 可以改变，详见：[javadoc of `DefaultPartHttpMessageReader`](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/http/codec/multipart/DefaultPartHttpMessageReader.html)

在服务端，可能需要从多个位置访问 multipart， `ServerWebExchange`提供了 专门的：`getMultipartData()`  方法，通过 `MultipartHttpMessageReader`  解析数据，然后缓存，See [Multipart Data](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-multipart) in the [`WebHandler` API](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-web-handler-api) section.

一旦调用`getMultipartData()` 那么无法再从 request body中 读取原始内容，所以 应该始终使用  `getMultipartData()` 或者 依赖 *SynchronossPartHttpMessageReader* ，一次性访问 `Flux<Part>`



## Limits

`Decoder` and `HttpMessageReader` 的实现 了 要么部分 缓存 要么 全部 缓存 输入流。这是可以配置的，限制在内存中缓冲的最大字节数。

在默写情况，缓冲发生 因为 输入被聚合 表示为单个对象，例如：`@RequestBody byte[]`, `x-www-form-urlencoded` 

缓冲也可能 发生在 流中，拆分输入流时（例如：分割的文本、JOSN对象流），这个限制适用于 一个对象关联的字节数

使用 `maxInMemorySize` 属性来配置  `Decoder` or `HttpMessageReader`

在服务端 `ServerCodecConfigurer` 提供 统一设置所有 codecs的该limit的地方，, see [HTTP message codecs](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-config-message-codecs)

在客户端 通过 [WebClient.Builder](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-client-builder-maxinmemorysize). 配置

对于 [Multipart parsing](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-codecs-multipart)   `maxInMemorySize` 属性限制了 非文件 parts的大小、对于文件 它决定 了 文件是否要写入到磁盘。对于写入到磁盘，还有一个额外的参数：`maxDiskUsagePerPart` 限制 每个 part 使用的磁盘 上限，`maxParts` 限制了 在一个 multipart 请求中 最多 parts部件总数



## Streaming

对于 HTTP 流式的响应，例如：（ `text/event-stream`, `application/x-ndjson`）

定期发送数据很重要，尽早的可靠的 检测断开的客户端，这样的发送可以只是 * comment-only* 空 SSE 事件 或者 其他 no-op 数据，作为 heartbeat

## `DataBuffer`

`DataBuffer`  在 WebFlux中表示 a byte buffer ，详见： [Data Buffers and Codecs](https://docs.spring.io/spring-framework/docs/current/reference/html/core.html#databuffers). 

The key point to understand is that on some servers like Netty, byte buffers are pooled and reference counted,

要知道的关键点是：在 Netty等服务器上，byte buffer 是池化 并且引用计算的，当被消费完之后，需要被释放 避免内存泄漏

WebFlux 程序 大体上 不需要 关系这类问题，除非 直接 消费或者产生 data buffers，相反 一般都依赖 codecs 转换 除非创建自定义的 codecs

详见： [Data Buffers and Codecs](https://docs.spring.io/spring-framework/docs/current/reference/html/core.html#databuffers), especially the section on [Using DataBuffer](https://docs.spring.io/spring-framework/docs/current/reference/html/core.html#databuffers-using).

# Logging

`DEBUG`  ：在 SpringWebFlux中 `DEBUG`  级别的日志  旨在 紧凑、最小、可读。关注 重要信息 

`TRACE`： 与 `DEBUG` 遵循一样的原则 但是可以用来调式 任何问题

## Log Id

在 WebFlux中，一个请求可能跨多个线程，线程ID 记录 特定请求 的日志消息不是很有用。这也是为什么 WebFlux默认使用 *request-specific* id

在服务端，logID 存储在 `ServerWebExchange` 属性，[`LOG_ID_ATTRIBUTE`](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/web/server/ServerWebExchange.html#LOG_ID_ATTRIBUTE)

`ServerWebExchange#getLogPrefix()` 获取完整格式化的 ID 前缀

在客户端，logId 存储在`ClientRequest`   [`LOG_ID_ATTRIBUTE`](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/web/reactive/function/client/ClientRequest.html#LOG_ID_ATTRIBUTE) 

`ClientRequest#logPrefix()` 获取完整格式化的 ID 前缀

## Sensitive Data

`DEBUG` and `TRACE`  可以记录 敏感信息，这也是为什么表单参数 跟请求头 默认被掩盖，需要显示启用

Java

```java
@Configuration
@EnableWebFlux
class MyConfig implements WebFluxConfigurer {

    @Override
    public void configureHttpMessageCodecs(ServerCodecConfigurer configurer) {
        configurer.defaultCodecs().enableLoggingRequestDetails(true);
    }
}
```



The following example shows how to do so for client-side requests:

Java

```java
Consumer<ClientCodecConfigurer> consumer = configurer ->
        configurer.defaultCodecs().enableLoggingRequestDetails(true);

WebClient webClient = WebClient.builder()
        .exchangeStrategies(strategies -> strategies.codecs(consumer))
        .build();
```

## Appenders

日志库 例如 SLF4J and Log4J2  提供异步 日志器 避免阻塞，这也有缺点，比如 可能会 丢失消息，因为没法对日志 入队列，它们最好的选择就是使用 reactive 

## Custom codecs

应用程序 能注册 自定义的 codecs ，以支持 额外的 请求类型，或者 默认codecs 不支持的 特定行为

想 保持跟 首选项对齐的配置

Java

```java
WebClient webClient = WebClient.builder()
        .codecs(configurer -> {
                CustomDecoder decoder = new CustomDecoder();
                configurer.customCodecs().registerWithDefaultConfig(decoder);
        })
        .build();
```



