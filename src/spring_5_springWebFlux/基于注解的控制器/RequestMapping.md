## Request Mapping

`@RequestMapping` 注解用来 映射请求到 控制器方法

它有很多参数进行匹配

* by URL
* HTTP method
* request parameters
* headers
* media types

在类级别用于 共享映射

在方法级别 用于 确定一个 指定的 endpoint mapping

基于不同的Http方法的`@RequestMapping` 的快捷方式

- `@GetMapping`
- `@PostMapping`
- `@PutMapping`
- `@DeleteMapping`
- `@PatchMapping`



前面的注解是  [Custom Annotations](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-requestmapping-composed) ，因为 大多数方法都需要映射到一个指定的 Http方法，而不是 直接使用 `@RequestMapping`匹配所有Http方法，同时 `@RequestMapping`需要 在类级别 上表示 共享的 映射

```java
@RestController
@RequestMapping("/persons")
class PersonController {

    @GetMapping("/{id}")
    public Person getPerson(@PathVariable Long id) {
        // ...
    }

    @PostMapping
    @ResponseStatus(HttpStatus.CREATED)
    public void add(@RequestBody Person person) {
        // ...
    }
}
```

### URI Patterns

可以使用通配符

| Pattern         | Description                                                  | Example                                                      |
| :-------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| `?`             | 匹配一个字符                                                 | `"/pages/t?st.html"` matches `"/pages/test.html"` and `"/pages/t3st.html"` |
| `*`             | Matches zero or more characters within a path segment        | `"/resources/*.png"` matches `"/resources/file.png"``"/projects/*/versions"` matches `"/projects/spring/versions"` but does not match `"/projects/spring/boot/versions"` |
| `**`            | Matches zero or more path segments until the end of the path | `"/resources/**"` matches `"/resources/file.png"` and `"/resources/images/file.png"``"/resources/**/file.png"` is invalid as `**` is only allowed at the end of the path. |
| `{name}`        | Matches a path segment and captures it as a variable named "name" | `"/projects/{project}/versions"` matches `"/projects/spring/versions"` and captures `project=spring` |
| `{name:[a-z]+}` | Matches the regexp `"[a-z]+"` as a path variable named "name" | `"/projects/{project:[a-z]+}/versions"` matches `"/projects/spring/versions"` but not `"/projects/spring1/versions"` |
| `{*path}`       | Matches zero or more path segments until the end of the path and captures it as a variable named "path" | `"/resources/{*file}"` matches `"/resources/images/file.png"` and captures `file=/images/file` |

捕获的URI可以通过  `@PathVariable` 变量访问

```java
@GetMapping("/owners/{ownerId}/pets/{petId}")
public Pet findPet(@PathVariable Long ownerId, @PathVariable Long petId) {
    // ...
}

//类级别上的 变量捕获
@Controller
@RequestMapping("/owners/{ownerId}") 
public class OwnerController {

    @GetMapping("/pets/{petId}") 
    public Pet findPet(@PathVariable Long ownerId, @PathVariable Long petId) {
        // ...
    }
}
```

* URI 变量 自动转换成合适的类型，或者抛出`TypeMismatchException` ，默认支持简单类型（`int`, `long`, `Date`）可以注册其他类型的支持See [Type Conversion](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-typeconversion) and [`DataBinder`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-ann-initbinder)

* URI variables 可以显示命名（`@PathVariable("customId")`） 但是可以忽略，如果使用 Java8 `-parameters` 选项 编译代码

* `{*varName}` 语法 申明了 一个 URI 变量，匹配一个或多个 剩余路径片段 例如 `/resources/{*path}`  匹配  `/resources/` 下所有资源的路径
* `{varName:regex}`  申明了 URI 变量 使用正则表达式示例如下

```java
@GetMapping("/{name:[a-z-]+}-{version:\\d\\.\\d\\.\\d}{ext:\\.[a-z]+}")
public void handle(@PathVariable String version, @PathVariable String ext) {
    // ...
}
```

URI path支持 `${}` 占位符 ，在启动时 使用 `PropertyPlaceHolderConfigurer` 通过 local、system、environment等其他 资源属性

Spring WebFlux  使用`PathPattern`  `PathPatternParser`  来进行 路径匹配，这些类在 `spring-web` 模块，主要是用来在web应用 运行时 对 HTTP URL path进行大量的路径匹配

Spring WebFlux 不支持 后缀路径匹配





### Pattern Comparison

当多个模式 都匹配 到URL时，必须比较出一个 最佳比配，使用 *PathPattern.SPECIFICITY_COMPARATOR* 已经完成，这回查找最精确的 patterns

对于每一个 pattern，都会计算出一个分数，URI变量和 通配符的个数，URI变量 分数低于 通配符，更低的 分数 获胜，分数一致的 更长的获胜

Catch-all patterns (for example, `**`, `{*varName}`)  不会计算分数，最会最后一个考虑，如果两个都是 catch-all 则长的 会被选择





### Consumable Media Types

你可以更加精确 请求 通过 `Content-Type` 

Java

```java
@PostMapping(path = "/pets", consumes = "application/json")
public void addPet(@RequestBody Pet pet) {
    // ...
}
```

consumers属性 支持内容协商表达式：

* `!text/plain`  匹配任何内容除了 `text/plain`
* 可以在类级别 定义 *consumes*
* 方法级别的 consumes覆盖 类级别的  consumes

`MediaType` 提供 通用使用的常量 例如：`APPLICATION_JSON_VALUE` and `APPLICATION_XML_VALUE`.

### Producible Media Types

基于 Accept请求头 可以列出 controller 方法 产生的 内容类型   更加精确请求，

Java

```java
@GetMapping(path = "/pets/{petId}", produces = "application/json")
@ResponseBody
public Pet getPet(@PathVariable String petId) {
    // ...
}
```

* 媒体类型可以 指定 字符集

* 支持 否定表达式 例如：`!text/plain`
* 可以在类级别声明 `produces` 属性，方法级别的 produces属性 会 覆盖类级别的
* `MediaType`  提供 常量访问，e.g. `APPLICATION_JSON_VALUE`, `APPLICATION_XML_VALUE`.

### Parameters and Headers

支持查询参数条件匹配

* 使用`myParam` 测试参数存在
* 使用 `!myParam` 测试 条件不存在
* 使用 `myParam=myValue` 测试等于某一个具体值

Java

```java
@GetMapping(path = "/pets/{petId}", params = "myParam=myValue") 
public void findPet(@PathVariable String petId) {
    // ...
}
```



**测试请求体中的头**

Java

```java
@GetMapping(path = "/pets", headers = "myHeader=myValue") 
public void findPet(@PathVariable String petId) {
    // ...
}
```

### HTTP HEAD, OPTIONS

`@GetMapping` and `@RequestMapping(method=HttpMethod.GET)` 透明的 支持 HTTP HEAD方法

response wrapper 应用于 `HttpHandler` server adapter，确保 `Content-Length` 头 被设置 ，且字节数没有计算 实际响应。

默认的 HTTP OPTIONS  通过 设置 Allow 头 来处理，由该URL匹配的  方法所支持的请求方式

* 对于 没有指定请求方法的`@RequestMapping` ，Allow头 设置为 `GET,HEAD,POST,PUT,PATCH,DELETE,OPTIONS`
* 控制器方法 应该始终指定 请求方法： `@GetMapping`, `@PostMapping`





### Custom Annotations

Spring WebFlux 支持 request mapping的  组合注解 （ [composed annotations](https://docs.spring.io/spring-framework/docs/current/reference/html/core.html#beans-meta-annotations) ）的使用

自定义注解本身 使用 `@RequestMapping`  注解，并在注解中 重新声明 RequetMapping 的属性

`@GetMapping`, `@PostMapping`, `@PutMapping`, `@DeleteMapping`, and `@PatchMapping` 就是组合注解的实例



Spring WebFlux 同样支持 自定义请求 映射属性 和 自定以请求匹配逻辑

这是更高级的选项：需要继承 `RequestMappingHandlerMapping`  覆盖`getCustomMethodCondition`  方法，这里你可以检查 自定义属性，并且返回 自己的`RequestCondition`

### Explicit Registrations

可以编程式注册 handler 方法，这可以用于动态注册 或者 高级 案例

例如同一个 Handler类 的不同实例 处理不同的 URLs

Java

```java
@Configuration
public class MyConfig {

    @Autowired
    public void setHandlerMapping(RequestMappingHandlerMapping mapping, UserHandler handler) 
            throws NoSuchMethodException {
//准备 request mapping 元数据
        RequestMappingInfo info = RequestMappingInfo
                .paths("/user/{id}").methods(RequestMethod.GET).build(); 
//获取handler方法
        Method method = UserHandler.class.getMethod("getUser", Long.class); 
//注册
        mapping.registerMapping(info, handler, method); 
    }

}
```

## 