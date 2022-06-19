# DispatcherHandler

SpringWebFlux 类似于 SpringMVC 围绕 前端 控制器模式，中央 WebHandler ：*DispatcherHandler* 为请求处理 提供共享算法，实际工作由 其他组件完成

`DispatcherHandler`  从Spring配置中 发现它 所需要的组件

DispatcherHandler 也可以申明为 *WebHandler* bean名。由  [`WebHttpHandlerBuilder`](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/web/server/adapter/WebHttpHandlerBuilder.html) 发现，这个类是用来 构建 WebHandler的请求处理链

WebFlux的Spring配置 一般包含

- `DispatcherHandler` with the bean name `webHandler`
- `WebFilter` and `WebExceptionHandler` beans
- [`DispatcherHandler` special beans](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-special-bean-types)
- Others

这些配置由 `WebHttpHandlerBuilder` 去构建 处理链

Java

```java
ApplicationContext context = ...
HttpHandler handler = WebHttpHandlerBuilder.applicationContext(context).build();
```

返回的 HttpHandler用于 [server adapter](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-httphandler)

## Special Bean Types

`DispatcherHandler`  将 请求处理 响应渲染 委托给 特殊bean（指spring管理的对象，并且是实现WebFlux协定）

以下是 *DispatcherHandler* 会自动检测的bean

### `HandlerMapping`

将请求映射到具体的 Handler

映射规则 基于 不同 *HandlerMapping*的实现

* annotated controllers
* simple URL pattern mappings
* and others

主要的实现是

* 基于@RequestMapping的   *RequestMappingHandlerMapping* 
* 函数式端点：*RouterFunctionMapping*
* 简单的URL匹配：：*SimpleUrlHandlerMapping*

### HandlerAdapter

帮助 *DispatcherHandler* 执行 *handler* 

例如：执行基于注解的控制器，需要解析注解：此类的主要目的是隐藏调用 handler细节

### HandlerResultHandler

从Handler 调用的结果，并最终确定响应See [Result Handling](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-resulthandling).

## Processing



*DispatcherHandler* 按以下方式处理请求

- 每一个  `HandlerMapping`  会请求去 匹配 handler，使用第一个匹配到的
- 找到 handler后，*HandlerAdapter* 会去调用 该 handler，并返回 `HandlerResult`
- `HandlerResultHandler`  会处理 `HandlerResult` 返回值，完成处理过程，要么直接 写数据到响应、要么使用 视图渲染



## Result Handling

通过 `HandlerAdapter` 调用handler的返回值 被封装为 HandlerResult，包含额外上下文，传给第一个 能支持 该result的 `HandlerResultHandler` 

下表是`HandlerResultHandler`  的实现

| Result Handler Type           | Return Values                                                | Default Order       |
| :---------------------------- | :----------------------------------------------------------- | :------------------ |
| `ResponseEntityResultHandler` | `ResponseEntity`, typically from `@Controller` instances.    | 0                   |
| `ServerResponseResultHandler` | `ServerResponse`, typically from functional endpoints.       | 0                   |
| `ResponseBodyResultHandler`   | Handle return values from `@ResponseBody` methods or `@RestController` classes. | 100                 |
| `ViewResolutionResultHandler` | `CharSequence`, [`View`](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/web/reactive/result/view/View.html), [Model](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/ui/Model.html), `Map`, [Rendering](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/web/reactive/result/view/Rendering.html), or any other `Object` is treated as a model attribute.See also [View Resolution](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-viewresolution). | `Integer.MAX_VALUE` |



## Exceptions

`HandlerResult`  提供错误处理 的 函数，基于 handler-specific  机制

错误函数会在以下情况被调用

- handler处理发生异常
- The handling of the handler return value through a `HandlerResultHandler` fails.

error function 可以改变 响应（例如 error status）只要在 从 handler返回的 响应式类型  产生任何数据项 之前 发出错误信号

这也是 为什么 `@ExceptionHandler`  方法 支持 在 @Controller类中受支持

Spring MVC  也同样支持，基于 `HandlerExceptionResolver`. 

记住：不能使用 `@ControllerAdvice`  处理异常，因为这发生在 handler 被选中前

See also [Managing Exceptions](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-controller-exceptions) in the “Annotated Controller” section or [Exceptions](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-exception-handler) in the WebHandler API section.

## View Resolution

视图解析 使得可以 向浏览器 渲染 HTML 模板，无需使用某一个特定的 模板机制

在 SpringWebFlux中，视图解析 是通过 专有 [HandlerResultHandler](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-resulthandling) ，使用  `ViewResolver`  实例 将  string 映射成 View实例

### Handling

传给 `ViewResolutionResultHandler` 的 `HandlerResult`  包含 从 handler 的返回值，包含 在 request handling 中添加进来的 属性的 model，返回值会按以下步骤处理

- `String`, `CharSequence`: 逻辑视图页面，通过配置的 `ViewResolver` 实例 解析成View实例
- `void`: 基于request path选择默认页面, 将请求URI，去除前导斜线和后缀斜线, 解析成 `View`. 如果未提供视图名也会按此逻辑处理(例如：返回ModelAndAttribute) 或者 返回异步值(例如：`Mono` 为空)
- [Rendering](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/web/reactive/result/view/Rendering.html): 视图解析API
- `Model`, `Map`: 添加到 model中的额外 model attributes
- Any other: 任何其他返回值 (除了简单属性，依据[BeanUtils#isSimpleProperty](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/beans/BeanUtils.html#isSimpleProperty-java.lang.Class-)) 被当作 属性 被添加进 Model当作 ，属性名 使用 [conventions](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/core/Conventions.html) 从 类名中取，除非 有`@ModelAttribute` 注解

The model 可以包含 异步 响应式 类型，

在渲染之前`AbstractView`  将 model attributes 解析成 具体的值，并更新模型，单值响应式类型被解析成单值或无值，多值会被解析成`List<T>`

配置视图解析 `ViewResolutionResultHandler` 添加到Spring配置中

[WebFlux Config](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-config-view-resolvers) 提供专门的视图解析配置API

## Redirecting

在 视图名的 特殊`redirect:` 前缀可以 重定向

`UrlBasedViewResolver` （及其子类） 可以识别，余下的是重定向的URL

效果跟 controller返回 *RedirectView* 或者 `Rendering.redirectTo("abc").build()`

但控制器本身可以 按逻辑视图名操作

视图名 如`redirect:/some/resource` ，相当于当前 应用的重定向

`redirect:https://example.com/arbitrary/path` 绝对路径的重定向

## Content Negotiation

`ViewResolutionResultHandler` 支持内容协商.

它比较 请求的媒体类型 和 每一个 视图支持的媒体类型 ，第一个支持的 被选中

为了支持 JSON XML媒体类型 SpringWebFlux 提供了 *HttpMessageWriterView* ，这是一个特殊的视图，通过  [HttpMessageWriter](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-codecs).  渲染

一般来说，你可以通过[WebFlux Configuration](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-config-view-resolvers)  配置 这些作为 默认视图

默认视图 如果支持请求的媒体类型，它总是第一个被选中











