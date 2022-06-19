The `spring-web` module 提供了以下 有用的 过滤器

- Form Data
- Forwarded Headers
- Shallow ETag
- CORS

# Form Data

Servlet API要求 ServletRequest.getParameter *（）方法来仅支持HTTP POST的表单字段访问。

 `spring-web` 模块 提供了 `FormContentFilter`  过滤器 去 拦截  content type 为 *application/x-www-form-urlencoded* 的 *HTTP PUT, PATCH, and DELETE* 请求

将 *ServletRequest* 包装使得 *ServletRequest.getParameter*()* 可用于获取表单数据



# Forwarded Headers

一个请求可能会 经过多层代理，很难获取用户的真正IP

[RFC 7239](https://tools.ietf.org/html/rfc7239) 定义了 the `Forwarded` HTTP header ，代理服务器可以提供 原始 请求的信息

还有非标准的 头，包括`X-Forwarded-Host`, `X-Forwarded-Port`, `X-Forwarded-Proto`, `X-Forwarded-Ssl`, and `X-Forwarded-Prefix`

`ForwardedHeaderFilter`  是一个 *Servlet* *Filter* ，基于  `Forwarded` headers 改变 host, port, and scheme 

它必须位于 所有 过滤器的前面

对于转发的标头，出于安全方面的考虑，因为应用程序无法知道标头是由代理添加的，还是由恶意客户端添加的

这就是为什么代理 应该 在 信任边界处的  删除来自外部的不受信任的“转发”标头的原因。

`ForwardedHeaderFilter` with `removeOnly=true`  会移除不受信任的 转发 头，

为了支持 asynchronous requests ，此过滤器应映射为DispatcherType.ASYNC和DispatcherType.ERROR。

如果使用 spring 框架的 `AbstractAnnotationConfigDispatcherServletInitializer`  ，所有 filters 自动注册 所有 dispatch types

However if registering the filter via `web.xml` or in Spring Boot via a `FilterRegistrationBean` be sure to include `DispatcherType.ASYNC` and `DispatcherType.ERROR` in addition to `DispatcherType.REQUEST`.



# Shallow ETag

* The `ShallowEtagHeaderFilter` filter creates a “shallow” ETag by caching the content written to the response and computing an MD5 hash from it. 

* 下一次请求，同样的计算 ，计算返回的值的 MD5 跟之前缓存的是否一样，则返回304
* 这个策略节省 了网络带宽，但没有节省 CPU 
* 另外一种  策略基于  控制器 级别，可以避免计算 See [HTTP Caching](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-caching).

这个 过滤器 有一个 `writeWeakETag` 参数  that configures the filter to write weak ETags similar to the following: `W/"02a2d595e6ed9a0b24f027f2b63b134d6"` (as defined in [RFC 7232 Section 2.3](https://tools.ietf.org/html/rfc7232#section-2.3)).

In order to support [asynchronous requests](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-ann-async) this filter must be mapped with `DispatcherType.ASYNC` so that the filter can delay and successfully generate an ETag to the end of the last async dispatch. If using Spring Framework’s `AbstractAnnotationConfigDispatcherServletInitializer` (see [Servlet Config](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-container-config)) all filters are automatically registered for all dispatch types. However if registering the filter via `web.xml` or in Spring Boot via a `FilterRegistrationBean` be sure to include `DispatcherType.ASYNC`.

# CORS

Spring MVC provides fine-grained support for CORS configuration through annotations on controllers. However, when used with Spring Security, we advise relying on the built-in `CorsFilter` that must be ordered ahead of Spring Security’s chain of filters.

See the sections on [CORS](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-cors) and the [CORS Filter](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-cors-filter) for more details.