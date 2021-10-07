## Handler Methods

`@RequestMapping`  的handler 方法 可以有 灵活的 申明

### Method Arguments

Reactive types 支持 需要 阻塞IO才能解析的 参数（例如 读请求体），这会在 Description 列中标记

Reactive types 不应该 期望 不需要 阻塞的 参数

JDK 1.8’s `java.util.Optional`  支持 方法参数 上的注解  `required` 属性，并设置为`required=false`

| Controller method argument                                   | Description                                                  |      |
| :----------------------------------------------------------- | :----------------------------------------------------------- | ---- |
| `ServerWebExchange`                                          | Access to the full `ServerWebExchange` — container for the HTTP request and response, request and session attributes, `checkNotModified` methods, and others. |      |
| `ServerHttpRequest`, `ServerHttpResponse`                    | Access to the HTTP request or response.                      |      |
| `WebSession`                                                 | Access to the session. This does not force the start of a new session unless attributes are added. Supports reactive types. |      |
| `java.security.Principal`                                    | The currently authenticated user — possibly a specific `Principal` implementation class if known. Supports reactive types. |      |
| `org.springframework.http.HttpMethod`                        | The HTTP method of the request.                              |      |
| `java.util.Locale`                                           | The current request locale, determined by the most specific `LocaleResolver` available — in effect, the configured `LocaleResolver`/`LocaleContextResolver`. |      |
| `java.util.TimeZone` + `java.time.ZoneId`                    | The time zone associated with the current request, as determined by a `LocaleContextResolver`. |      |
| `@PathVariable`                                              | For access to URI template variables. See [URI Patterns](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-requestmapping-uri-templates). |      |
| `@MatrixVariable`                                            | For access to name-value pairs in URI path segments. See [Matrix Variables](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-matrix-variables). |      |
| `@RequestParam`                                              | For access to Servlet request parameters. Parameter values are converted to the declared method argument type. See [`@RequestParam`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-requestparam).Note that use of `@RequestParam` is optional — for example, to set its attributes. See “Any other argument” later in this table. |      |
| `@RequestHeader`                                             | For access to request headers. Header values are converted to the declared method argument type. See [`@RequestHeader`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-requestheader). |      |
| `@CookieValue`                                               | For access to cookies. Cookie values are converted to the declared method argument type. See [`@CookieValue`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-cookievalue). |      |
| `@RequestBody`                                               | For access to the HTTP request body. Body content is converted to the declared method argument type by using `HttpMessageReader` instances. Supports reactive types. See [`@RequestBody`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-requestbody). |      |
| `HttpEntity<B>`                                              | For access to request headers and body. The body is converted with `HttpMessageReader` instances. Supports reactive types. See [`HttpEntity`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-httpentity). |      |
| `@RequestPart`                                               | For access to a part in a `multipart/form-data` request. Supports reactive types. See [Multipart Content](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-multipart-forms) and [Multipart Data](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-multipart). |      |
| `java.util.Map`, `org.springframework.ui.Model`, and `org.springframework.ui.ModelMap`. | For access to the model that is used in HTML controllers and is exposed to templates as part of view rendering. |      |
| `@ModelAttribute`                                            | For access to an existing attribute in the model (instantiated if not present) with data binding and validation applied. See [`@ModelAttribute`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-modelattrib-method-args) as well as [`Model`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-modelattrib-methods) and [`DataBinder`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-initbinder).Note that use of `@ModelAttribute` is optional — for example, to set its attributes. See “Any other argument” later in this table. |      |
| `Errors`, `BindingResult`                                    | For access to errors from validation and data binding for a command object, i.e. a `@ModelAttribute` argument. An `Errors`, or `BindingResult` argument must be declared immediately after the validated method argument. |      |
| `SessionStatus` + class-level `@SessionAttributes`           | For marking form processing complete, which triggers cleanup of session attributes declared through a class-level `@SessionAttributes` annotation. See [`@SessionAttributes`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-sessionattributes) for more details. |      |
| `UriComponentsBuilder`                                       | For preparing a URL relative to the current request’s host, port, scheme, and context path. See [URI Links](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-uri-building). |      |
| `@SessionAttribute`                                          | For access to any session attribute — in contrast to model attributes stored in the session as a result of a class-level `@SessionAttributes` declaration. See [`@SessionAttribute`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-sessionattribute) for more details. |      |
| `@RequestAttribute`                                          | For access to request attributes. See [`@RequestAttribute`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-requestattrib) for more details. |      |
| Any other argument                                           | If a method argument is not matched to any of the above, it is, by default, resolved as a `@RequestParam` if it is a simple type, as determined by [BeanUtils#isSimpleProperty](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/beans/BeanUtils.html#isSimpleProperty-java.lang.Class-), or as a `@ModelAttribute`, otherwise. |      |

### Return Values

返回值支持的类型

| Controller method return value                               | Description                                                  |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| `@ResponseBody`                                              | 使用 *HttpMessageWriter* 实例 编码，并写入到响应，详见：[`@ResponseBody`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-responsebody) |
| `HttpEntity<B>`, `ResponseEntity<B>`                         | 包括 Http头,请求体使用`HttpMessageWriter` 实例 编码，并写入到响应，详见 [`ResponseEntity`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-responseentity). |
| `HttpHeaders`                                                | 只返回头，不返回响应                                         |
| `String`                                                     | 使用  `ViewResolver`  解析的 视图名                          |
| `View`                                                       | 视图实例                                                     |
| `java.util.Map`, `org.springframework.ui.Model`              | 要添加到  隐式模型中的属性                                   |
| `@ModelAttribute`                                            | 要添加到 隐式 模型中的属性                                   |
| `Rendering`                                                  | An API for model and view rendering scenarios.               |
| `void`                                                       | 返回 Void的方法 可能是 异步的（eg： `Mono<Void>`）， 返回值类型（可能是 null返回值）<br />1. 被认为是 已经将 响应 处理完成 ，如果有一个 `ServerHttpResponse`, a `ServerWebExchange`  参数<br />2. 或者存在  `@ResponseStatus` 注解<br />3. 如果 controller 对 *ETag* *lastModified timestamp* 进行 检查 |
| `Flux<ServerSentEvent>`, `Observable<ServerSentEvent>`, or other reactive type | Emit server-sent events. The `ServerSentEvent` wrapper can be omitted when only data needs to be written (however, `text/event-stream` must be requested or declared in the mapping through the `produces` attribute). |
| Any other return value                                       | 如果以上返回值都不匹配，如果是空的或者 是Sting 则默认的作为视图名 or 否则会加入到 Model中作为 attributes,触发 它是简单类型 依据[BeanUtils#isSimpleProperty](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/beans/BeanUtils.html#isSimpleProperty-java.lang.Class-) |

### Type Conversion

基于 string的请求输入 的 注解控制器方法参数 （例如：`@RequestParam`，`@RequestHeader`，`@PathVariable`, `@MatrixVariable`, and `@CookieValue`）

需要类型转换，会自动进行类型转换，基于 配置的 converters

通过 自定义 `WebDataBinder` 配置类型转换(see [`DataBinder`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-initbinder)) ，或者 使用`FormattingConversionService`  注册 `Formatters` 

### Matrix Variables

[RFC 3986](https://tools.ietf.org/html/rfc3986#section-3.3) discusses name-value pairs in path segments

In Spring WebFlux, 我们把它叫做：*matrix variables* based on an [“old post”](https://www.w3.org/DesignIssues/MatrixURIs.html) by Tim Berners-Lee 

但它们也可以称为 URI 路劲参数，Matrix variables 可以出现在任何 路径参数中，每一个变量被 分号分隔，多值用逗号分隔，也可以重复变量名

```
`"/cars;color=red,green;year=2012"` `"color=red;color=green;color=blue"`
```

跟SpringMVC 不一样的是：matrix variables 不会影响请求映射

```java
// GET /pets/42;q=11;r=22

@GetMapping("/pets/{petId}")
public void findPet(@PathVariable String petId, @MatrixVariable int q) {

    // petId == 42
    // q == 11
}
```

每一个路径片段都可以有 matrix variable,这时候需要消除歧义

```java
// GET /owners/42;q=11/pets/21;q=22

@GetMapping("/owners/{ownerId}/pets/{petId}")
public void findPet(
        @MatrixVariable(name="q", pathVar="ownerId") int q1,
        @MatrixVariable(name="q", pathVar="petId") int q2) {

    // q1 == 11
    // q2 == 22
}
```

**默认值**

```java
// GET /pets/42

@GetMapping("/pets/{petId}")
public void findPet(@MatrixVariable(required=false, defaultValue="1") int q) {

    // q == 1
}
```



**获取某个 路径片段的所有 matrix variable**

```java
// GET /owners/42;q=11;r=12/pets/21;q=22;s=23

@GetMapping("/owners/{ownerId}/pets/{petId}")
public void findPet(
        @MatrixVariable MultiValueMap<String, String> matrixVars,
        @MatrixVariable(pathVar="petId") MultiValueMap<String, String> petMatrixVars) {

    // matrixVars: ["q" : [11,22], "r" : 12, "s" : 23]
    // petMatrixVars: ["q" : 22, "s" : 23]
}
```



### @RequestParam

使用 `@RequestParam`  注解绑定 查询参数

```java
@Controller
@RequestMapping("/pets")
public class EditPetForm {
    // ...
    @GetMapping
    public String setupForm(@RequestParam("petId") int petId, Model model) { 
        Pet pet = this.clinic.loadPet(petId);
        model.addAttribute("pet", pet);
        return "petForm";
    }
}
```



The Servlet API `request parameter` 概念 将 查询参数、表单、multiparts  合成一个，在 WebFlux中，每一个是通过 `ServerWebExchange` 独立访问的

`@RequestParam` 只绑定 查询参数，你可以使用数据绑定 将 查询参数、表单、multiparts 绑定到 [command object](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-modelattrib-method-args).

使用 `@RequestParam`  注解的方法参数 默认是 必须，支持 `java.util.Optional`  wrapper

会自动应用类型转换，See [Type Conversion](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-typeconversion).

当 `@RequestParam`  注解声明在 `Map<String, String>` or `MultiValueMap<String, String>`  参数上

map会填充所有 查询参数

注意：`@RequestParam`   是可选的，例如：为了设置它的属性。

默认的，任何参数，且是一个 简单值类型、依据[BeanUtils#isSimpleProperty](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/beans/BeanUtils.html#isSimpleProperty-java.lang.Class-)  不被任何参数解析器 解析，会被当做 使用了 `@RequestParam`一样

### @RequestHeader

在控制器中  使用 `@RequestHeader` 可以绑定 请求头

```
Host                    localhost:8080
Accept                  text/html,application/xhtml+xml,application/xml;q=0.9
Accept-Language         fr,en-gb;q=0.7,en;q=0.3
Accept-Encoding         gzip,deflate
Accept-Charset          ISO-8859-1,utf-8;q=0.7,*;q=0.7
Keep-Alive              300
```



获取`Accept-Encoding` `Keep-Alive` 

```java
@GetMapping("/demo")
public void handle(
        @RequestHeader("Accept-Encoding") String encoding, 
        @RequestHeader("Keep-Alive") long keepAlive) { 
    //...
}
```



* 会自动应用类型转换，See [Type Conversion](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-typeconversion).
* `@RequestHeader` 注解在 `Map<String, String>`, `MultiValueMap<String, String>`, or `HttpHeaders`  上，那么会填充所有 header

例如 `@RequestHeader("Accept")` 可以注解在 `String`、`List<String>`、 `String[]` 

### @CookieValue

使用  `@CookieValue`  绑定 HTTP cookie到 参数方法上

```
JSESSIONID=415A4AC178C59DACE0B2C9CA727CDD84
```

```java
@GetMapping("/demo")
public void handle(@CookieValue("JSESSIONID") String cookie) { 
    //...
}
```

自动应用类型转换 See [Type Conversion](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-typeconversion)



### @ModelAttribute

在参数方法上 使用 `@ModelAttribute` 注解 ，对 model的属性访问，如果它不在的话 实例化它

The model attribute 同样也和 查询参数、表单字段重叠

这称为 数据绑定、使你避免 处理 解析、转换单独的查询参数以及表单字段

```java
@PostMapping("/owners/{ownerId}/pets/{petId}/edit")
public String processSubmit(@ModelAttribute Pet pet) { } 
```

`Pet` 实例 会按以下方式解析

- 从已添加到 [`Model`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-modelattrib-methods)  的属性
- 来自 HTTP session  通过[`@SessionAttributes`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-sessionattributes)
- 从默认构造器的调用
- 来自 *primary constructor* 的调用，带着 匹配 查询参数或表单字段的 参数
- 参数名 通过 JavaBean的 *@ConstructorProperties* 判断，或者通过 字节码中 运行时的 参数名保留



获取到 model attribute  的实例后，开始应用数据绑定，`WebExchangeDataBinder` 类在目标对象上 匹配 查询参数和表单字段名

匹配字段会在 类型转换后 填充,对于校验 详见： [Validation](https://docs.spring.io/spring-framework/docs/current/reference/html/core.html#validation)，对于 自定义数据绑定，详见：[`DataBinder`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-initbinder).

数据绑定会造成错误，默认情况下，`WebExchangeBindException` 会被抛出，但是为了在控制器方法 检查异常，可以在  `@ModelAttribute` 下一个参数中声明  `BindingResult` 0

```java
@PostMapping("/owners/{ownerId}/pets/{petId}/edit")
public String processSubmit(@ModelAttribute("pet") Pet pet, BindingResult result) { 
    if (result.hasErrors()) {
        return "petForm";
    }
    // ...
}
```

可以通过 添加  `javax.validation.Valid`   或者 spring的注解`@Validated`  注解 在数据绑定后 自动应用 校验，详见  (see also [Bean Validation](https://docs.spring.io/spring-framework/docs/current/reference/html/core.html#validation-beanvalidation) and [Spring validation](https://docs.spring.io/spring-framework/docs/current/reference/html/core.html#validation))



```java
@PostMapping("/owners/{ownerId}/pets/{petId}/edit")
public String processSubmit(@Valid @ModelAttribute("pet") Pet pet, BindingResult result) { 
    if (result.hasErrors()) {
        return "petForm";
    }
    // ...
}
```



Spring WebFlux 支持 在 model中的 响应式类型  例如：`Mono<Account>` ，可以声明 存在或不存在 响应式类型的包装类的 `@ModelAttribute` 注解的参数

如果使用 `BindingResult`  参数，必须 声明`@ModelAttribute`  是非响应式的

Java

```java
@PostMapping("/owners/{ownerId}/pets/{petId}/edit")
public Mono<String> processSubmit(@Valid @ModelAttribute("pet") Mono<Pet> petMono) {
    return petMono
        .flatMap(pet -> {
            // ...
        })
        .onErrorResume(ex -> {
            // ...
        });
}
```

### @SessionAttributes

`@SessionAttributes` 是用来在 请求间 保存 model属性的，将属性存储在 `WebSession`，这是个类级别的注解，声明在某个控制器中，这通常列出model属性的 名称 或者模型属性的  类型，它们被透明的 存储在 *session* 中以供下次请求使用

Java

```java
@Controller
@SessionAttributes("pet") 
public class EditPetForm {
    // ...
}
```

在第一次请求中，模型属性 `pet` 会被添加到 模型中、自动保存在 `WebSession`，它保持直到 另一个控制器 方法 使用 `SessionStatus`  方法参数来 清除存储

```java
@Controller
@SessionAttributes("pet") 
public class EditPetForm {

    // ...

    @PostMapping("/pets/{id}")
    public String handle(Pet pet, BindingResult errors, SessionStatus status) { 
        if (errors.hasErrors()) {
            // ...
        }
            status.setComplete();
            // ...
        }
    }
}
```

### @SessionAttribute

如果想访问 预先存在的 session 属性（由全局管理），可以使用 `@SessionAttribute`  属性管理

```java
@GetMapping("/")
public String handle(@SessionAttribute User user) { 
    // ...
}
```

如果想添加 或者删除session属性，注入WebSession 到方法参数中

### @RequestAttribute

使用 `@RequestAttribute`  访问 request attirbutes 中的属性

```java
@GetMapping("/")
public String handle(@RequestAttribute Client client) { 
    // ...
}
```

### Multipart Content

上述提到的 [Multipart Data](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-multipart), `ServerWebExchange`  提供了 对 multipart 内容的访问



在控制器中 最佳的处理文件上传的方式是 通过数据绑定到  [command object](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-modelattrib-method-args)

Java

```java
class MyForm {

    private String name;

    private MultipartFile file;

    // ...

}

@Controller
public class FileUploadController {

    @PostMapping("/form")
    public String handleFormUpload(MyForm form, BindingResult errors) {
        // ...
    }

}
```

在 RESTFUL 场景 从非浏览器客户端 中提交 multipart 请求

```
POST /someUrl
Content-Type: multipart/mixed

--edt7Tfrdusa7r3lNQc79vXuhIIMlatb7PQg7Vp
Content-Disposition: form-data; name="meta-data"
Content-Type: application/json; charset=UTF-8
Content-Transfer-Encoding: 8bit

{
    "name": "value"
}
--edt7Tfrdusa7r3lNQc79vXuhIIMlatb7PQg7Vp
Content-Disposition: form-data; name="file-data"; filename="file.properties"
Content-Type: text/xml
Content-Transfer-Encoding: 8bit
... File Data ...
```



使用 `@RequestPart` 访问 单独部件

```java
@PostMapping("/")
public String handle(@RequestPart("meta-data") Part metadata, 
        @RequestPart("file-data") FilePart file) { 
    // ...
}
```



反序列化 原始部件内容（例如：to JSON），你可以申明一个具体的 对象，而不是 *Part*

Java

```java
@PostMapping("/")
public String handle(@RequestPart("meta-data") MetaData metadata) { 
    // ...
}
```



你可以将  `@RequestPart`  与 `javax.validation.Valid` or Spring’s `@Validated`  注解组合，这会 引入 Standard Bean Validation 

校验报错 会导致 `WebExchangeBindException`   导致 BAD_REQUEST（400）

异常包含`BindingResult`  ，可以通过申明为 Mono<MetaData\> ，在`Mono` 中 进行错误的处理

```java
@PostMapping("/")
public String handle(@Valid @RequestPart("meta-data") Mono<MetaData> metadata) {
    // use one of the onError* operators...
}
```



声明为 `MultiValueMap` 可以使用 `@RequestBody`  以访问所有部件

```java
@PostMapping("/")
public String handle(@RequestBody Mono<MultiValueMap<String, Part>> parts) { 
    // ...
}
```



以流式序列的访问，可以申明 `@RequestBody`  以及`Flux<Part>`

```java
@PostMapping("/")
public String handle(@RequestBody Flux<Part> parts) { 
    // ...
}
```

### @RequestBody

使用 `@RequestBody`  注解 通过 [HttpMessageReader](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-codecs).  读取 请求头 反序列化 成 对象

```java
@PostMapping("/accounts")
public void handle(@RequestBody Account account) {
    // ...
}
```

和SpringMVC不同的是，WebMVC中，`@RequestBody` 注解的方法参数支持 响应式类型，完全支持 非阻塞读

```java
@PostMapping("/accounts")
public void handle(@RequestBody Mono<Account> account) {
    // ...
}
```

可以使用  [WebFlux Config](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-config) 中的 [HTTP message codecs](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-config-message-codecs)  选项去配置

你可以将  `@RequestBody`  与 `javax.validation.Valid` or Spring’s `@Validated`  注解组合，这会 引入 Standard Bean Validation 

校验报错 会导致 `WebExchangeBindException`   导致 BAD_REQUEST（400）

异常包含`BindingResult`  ，可以通过申明为 Mono<MetaData\> ，在`Mono` 中 进行错误的处理

```java
@PostMapping("/accounts")
public void handle(@Valid @RequestBody Mono<Account> account) {
    // use one of the onError* operators...
}
```

### HttpEntity

`HttpEntity` 或多或少 与  [`@RequestBody`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-requestbody)  相同 ，但是基于 一个容器对象，暴露请求体和请求头

```java
@PostMapping("/accounts")
public void handle(HttpEntity<Account> entity) {
    // ...
}
```

### @ResponseBody

使用`@ResponseBody`  注解在 方法上 使得 方法的返回值 使用 [HttpMessageWriter](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-codecs) 进行序列化

```java
@GetMapping("/accounts/{id}")
@ResponseBody
public Account handle() {
    // ...
}
```



* `@ResponseBody` 支持类级别上，被所有控制器的方法继承
* `@RestController` 只不过是 一个 注解了`@Controller` and `@ResponseBody`的元注解
* `@ResponseBody`  支持 响应式 类型这意味着 你可以 返回Reactor类型

更多额外的 细节详见： see [Streaming](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-codecs-streaming) and [JSON rendering](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-codecs-jackson)

可以使用 JSON serialization views 综合 `@ResponseBody`  方法See [Jackson JSON](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-jackson) for details.

### ResponseEntity

`ResponseEntity` 与[`@ResponseBody`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-responsebody)  类似但是 由 状态 和 头

Java

```java
@GetMapping("/something")
public ResponseEntity<String> handle() {
    String body = ... ;
    String etag = ... ;
    return ResponseEntity.ok().eTag(etag).build(body);
}
```

WebFlux supports using a single value [reactive type](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-reactive-libraries) to produce the `ResponseEntity` asynchronously, and/or single and multi-value reactive types for the body. This allows a variety of async responses with `ResponseEntity` as follows:

WebFlux 支持 使用 单值 响应式类型 异步产生ResponseEntity 

* `ResponseEntity<Mono<T>>` or `ResponseEntity<Flux<T>>`  使得 响应 状态跟 请求头立即 可用，但是 请求体 是稍后异步 提供的，如果body是单值 则 使用Mono、如果 多值使用 Flux

- `Mono<ResponseEntity<T>>`  异步提供 响应状态、请求头、请求体，这允许 响应状态 和头 因异步请求的结果而异
- `Mono<ResponseEntity<Mono<T>>>` or `Mono<ResponseEntity<Flux<T>>>`  这又是另一种可能，虽然是不太常见的选择，异步的提供请求头、请求体，之后在异步的响应请求内容

### Jackson JSON

Spring offers support for the Jackson JSON library.

#### JSON Views

Spring WebFlux 提供内置 [Jackson’s Serialization Views](https://www.baeldung.com/jackson-json-view-annotation) 的支持，允许 渲染对象中字段的某个子集

To use it with `@ResponseBody` or `ResponseEntity` controller methods, 

需要 配合`@ResponseBody` or `ResponseEntity` 使用，使用  `@JsonView` 注解 激活 序列化视图

Java

```java
@RestController
public class UserController {

    @GetMapping("/user")
    @JsonView(User.WithoutPasswordView.class)
    public User getUser() {
        return new User("eric", "7!jd#h23");
    }
}

public class User {

    public interface WithoutPasswordView {};
    public interface WithPasswordView extends WithoutPasswordView {};

    private String username;
    private String password;

    public User() {
    }

    public User(String username, String password) {
        this.username = username;
        this.password = password;
    }

    @JsonView(WithoutPasswordView.class)
    public String getUsername() {
        return this.username;
    }

    @JsonView(WithPasswordView.class)
    public String getPassword() {
        return this.password;
    }
}
```

`@JsonView` 允许数组的视图类，但是 每个 控制器方法 指定一个

如果需要多个视图 ，请组合接口 

### Model

可以使用 `@ModelAttribute` 注解

- 方法参数：在 `@RequestMapping`方法参数中  从model 中 创建或访问 对象，然后 通过`WebDataBinder` 绑定到 请求中
- 方法上：在 `@Controller` or `@ControllerAdvice` 类中， 作为方法级别的注解，优先于  `@RequestMapping`  方法的调用 帮助 初始化 model
- 返回值：在一个`@RequestMapping` 方法中，将其返回值 标记为 model 属性

这节讨论 `@ModelAttribute`  方法，或者 上述列表中的第二项

控制器可以有 任意数量的 `@ModelAttribute` 方法

所有这些方法都 在 `@RequestMapping` 方法之前调用

通过 `@ControllerAdvice` `@ModelAttribute` 方法 可以 跨控制器 共享

详见：[Controller Advice](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-controller-advice) 章节

`@ModelAttribute` 方法有很灵活的  方法声明

它们与 `@RequestMapping` 方法一样 支持同样多的 参数 除了 `@ModelAttribute`  本身任何其他 跟 *request body* 相关的参数

```java
@ModelAttribute
public void populateModel(@RequestParam String number, Model model) {
    model.addAttribute(accountRepository.findAccount(number));
    // add more ...
}
```

**将返回值 加入到 Model中**

```java
@ModelAttribute
public Account addAccount(@RequestParam String number) {
    return accountRepository.findAccount(number);
}
```

当没有显示指定名称时，会基于类型选择默认名称，详见： [`Conventions`](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/core/Conventions.html)

可以通过 `addAttribute`  显示指定名称 或者 在返回值中加入  `@ModelAttribute`  注解指定



Spring WebFlux 支持 显示的 响应式类型

```java
@ModelAttribute
public void addAccount(@RequestParam String number) {
    Mono<Account> accountMono = accountRepository.findAccount(number);
    model.addAttribute("account", accountMono);
}

@PostMapping("/accounts")
public String handle(@ModelAttribute Account account, BindingResult errors) {
    // ...
}
```





另外 ，任何 模型属性 ，只要有响应式类型包装 ，在 视图渲染前 会被 解析成实际值

可以在 `@RequestMapping` 方法上注解 `@RequestMapping`，这会让 返回值 被当作 模型属性

这个不用特别指定，因为 这是 HTML 控制器的默认行为，除非 返回值 是 string类型：这个会被解析成视图

`@ModelAttribute` 也可以自定义 属性名

Java

```java
@GetMapping("/accounts/{id}")
@ModelAttribute("myAccount")
public Account handle() {
    // ...
    return account;
}
```

### DataBinder

`@Controller`  或者  `@ControllerAdvice`  类 可以有 `@InitBinder` 方法，用来初始化 `WebDataBinder` 实例

这个实例是用来以下：

- 绑定请求参数到模型中去.
- 将基于 string的请求值 转换成 控制器方法参数对象
- 在渲染模板时，将 model对象 转换为 string



`@InitBinder` 方法 可以注解 特定于 控制器的 `java.beans.PropertyEditor`  或者 Spring `Converter` and `Formatter` 组件

另外：可以使用[WebFlux Java configuration](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-config-conversion)  在全局共享的`FormattingConversionService`中注册 `Converter` and `Formatter`

`@InitBinder` 支持 跟  `@RequestMapping` 方法 许多相同的参数，除了 `@ModelAttribute`参数

典型的，`WebDataBinder`  参数用来注册 ，返回 VOID

Java

```java
@Controller
public class FormController {

    @InitBinder 
    public void initBinder(WebDataBinder binder) {
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
        dateFormat.setLenient(false);
        binder.registerCustomEditor(Date.class, new CustomDateEditor(dateFormat, false));
    }
    // ...
}
```

另外可以通过 `FormattingConversionService` 注册   `Formatter` based 

Java

```java
@Controller
public class FormController {

    @InitBinder
    protected void initBinder(WebDataBinder binder) {
        binder.addCustomFormatter(new DateFormatter("yyyy-MM-dd")); 
    }

    // ...
}
```

### Managing Exceptions

`@Controller` and [@ControllerAdvice](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-controller-advice)  类可以有  `@ExceptionHandler` 方法来处理 控制器方法的异常

Java

```java
@Controller
public class SimpleController {

    // ...

    @ExceptionHandler 
    public ResponseEntity<String> handle(IOException ex) {
        // ...
    }
}
```



异常可以 与 正在传播的 顶层 异常相匹配 （也就是：一个直接的 IOException 被抛出） 或者是 包装异常的直接 异常（例如： `IOException`包装在 `IllegalStateException`的内部）

对于异常类型匹配，最好 声明 目标异常作为 方法参数

也可以在 注解中声明 异常的类型

通常建议：

1. 在参数中 声明的越具体越好
2. 在注解中 声明 primary root exception
3. 在  @ControllerAdvice中 按照 优先级的顺序 声明 异常处理方法，See [the MVC section](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-ann-exceptionhandler) for details

An `@ExceptionHandler` 方法 支持 与`@RequestMapping` method  同样的方法参数、返回值，除了 equest body- 和 `@ModelAttribute` 关联的 方法参数

在SpringWebFlux中 `@ExceptionHandler`  方法支持 由  `@RequestMapping`  方法的 `HandlerAdapter` 提供

See [`DispatcherHandler`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-dispatcher-handler)

#### REST API exceptions

REST services  的 通用要求是 包含 错误明细在 响应中

Spring Framework 不会自动做这些工作，因为 错误的明细 是特定于应用程序的

`@RestController`  可以使用 `@ExceptionHandler`  方法 ，返回 `ResponseEntity`  值 用来设置 状态 和响应体

可以生命在 `@ControllerAdvice`  以全局 处理 

注意：Spring WebFlux 没有 SpringMVC中 类似的 *ResponseEntityExceptionHandler*  因为WebFlux 只抛出 ResponseStatusException 或者其 子类，不需要转换成 HTTP状态码



### Controller Advice

典型说来，`@ExceptionHandler`, `@InitBinder`, and `@ModelAttribute` 方法  应用于 它们所声明的控制器类（或者类继承结构）内部，如果你想跨控制器全局使用 可以把它们声明在 `@ControllerAdvice` or `@RestControllerAdvice`

`@ControllerAdvice`  被 `@Component` 注解了，这意味着 这些类可以被 注册到 Spring bean容器中

`@RestControllerAdvice`  是一个组合注解，由  `@ControllerAdvice` and `@ResponseBody` 注解 注解 这也意味着 `@ExceptionHandler` 方法 渲染数据到 响应体，通过 message conversion



在启动时， `@RequestMapping` and `@ExceptionHandler` 方法的 基础设施类 检测 Spring beans 中 带有 `@ControllerAdvice` 注解的 bean,在运行时 应用



Global (来自`@ControllerAdvice`)  `@ExceptionHandler`  方法  应用于 本地 之后（来自  `@Controller`）

据约定， 全局的 `@ModelAttribute` and `@InitBinder`  应用于 本地之前

By default, `@ControllerAdvice` methods apply to every request (that is, all controllers), but you can narrow that down to a subset of controllers by using attributes on the annotation, as the following example shows:

默认情况下，`@ControllerAdvice` 方法应用于每个请求（即所有控制器类），但是可以缩小 控制器的范围

```java
// Target all Controllers annotated with @RestController
@ControllerAdvice(annotations = RestController.class)
public class ExampleAdvice1 {}

// Target all Controllers within specific packages
@ControllerAdvice("org.example.controllers")
public class ExampleAdvice2 {}

// Target all Controllers assignable to specific classes
@ControllerAdvice(assignableTypes = {ControllerInterface.class, AbstractController.class})
public class ExampleAdvice3 {}
```

上述案例的  选择 是在运行时 解析的，可能会轻微的影响性能，详见 [`@ControllerAdvice`](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/web/bind/annotation/ControllerAdvice.html)



