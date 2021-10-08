# Functional Endpoints

Spring WebFlux 包含 WebFlux.fn，轻量级的函数式编程模型，函数 是用来 作 路由 跟 请求处理，协定旨在不可变，是 基于注解的 编程模型的另一个可用的方式



# Overview

在WebFlux.fn中，使用 `HandlerFunction`处理 HTTP请求，以 *ServerRequest* 为参数，返回 延迟的 *ServerResponse* ，例如 `Mono<ServerResponse>`

* 请求跟响应都 有不变的 协定，以 jdk8友好的 方式 访问 请求跟响应
* `HandlerFunction` 类似注解中的  `@RequestMapping` 方法，即将到来的请求通过 *RouterFunction* 映射到 handler function
* `RouterFunctions.route()`  提供 router的构建

```java
import static org.springframework.http.MediaType.APPLICATION_JSON;
import static org.springframework.web.reactive.function.server.RequestPredicates.*;
import static org.springframework.web.reactive.function.server.RouterFunctions.route;

PersonRepository repository = ...
PersonHandler handler = new PersonHandler(repository);

RouterFunction<ServerResponse> route = route()
    .GET("/person/{id}", accept(APPLICATION_JSON), handler::getPerson)
    .GET("/person", accept(APPLICATION_JSON), handler::listPeople)
    .POST("/person", handler::createPerson)
    .build();

public class PersonHandler {

    // ...

    public Mono<ServerResponse> listPeople(ServerRequest request) {
        // ...
    }

    public Mono<ServerResponse> createPerson(ServerRequest request) {
        // ...
    }

    public Mono<ServerResponse> getPerson(ServerRequest request) {
        // ...
    }
}
```

运行 `RouterFunction` 的一个方法是：将它转换为 `HttpHandler` ，通过 内置的 server adapters 安装它们

- `RouterFunctions.toHttpHandler(RouterFunction)`
- `RouterFunctions.toHttpHandler(RouterFunction, HandlerStrategies)`





# HandlerFunction

`ServerRequest` and `ServerResponse`  是不可变接口，提供 jdk8友好访问 HTTP的请求和响应

请求跟响应 都提供了 响应式流  的背压，请求Body被表示为 `Flux` or `Mono` 响应被表示为 任何 Reactive Streams `Publisher`

## ServerRequest

`ServerRequest` 提供对 HTTP方法 URI 请求头、查询参数的访问，访问请求体通过  *body* 方法

**body转`Mono<String>`**

```java
Mono<String> string = request.bodyToMono(String.class);
```

**Person对象通过 反序列化的方式（JSON或者 XML**）

```java
Flux<Person> people = request.bodyToFlux(Person.class);
```

**提供自定义函数解析**

```java
Mono<String> string = request.body(BodyExtractors.toMono(String.class));
Flux<Person> people = request.body(BodyExtractors.toFlux(Person.class));
```

**访问表单格式**

```java
Mono<MultiValueMap<String, String>> map = request.formData();
```

**访问multi-part**

```java
Mono<MultiValueMap<String, Part>> map = request.multipartData();
```

**响应式流 一次访问一个**

```java
Flux<Part> parts = request.body(BodyExtractors.toParts());
```

## ServerResponse

`ServerResponse`  提供对 HTTP 响应的 访问。因为是不可变的，可以使用 *build* 构建

可以设置请求状态、请求头、请求体

```java
Mono<Person> person = ...
ServerResponse.ok().contentType(MediaType.APPLICATION_JSON).body(person, Person.class);
```

**201（CREATED）with a location**

```java
URI location = ...
ServerResponse.created(location).build();
```

**提供 *hint* 参数以 自定义 body如何序列化如何反序列化**

```java
ServerResponse.ok().hint(Jackson2CodecSupport.JSON_VIEW_HINT, MyJacksonView.class).body(...);
```

## Handler Classes

**使用 Lambada表达式定义HandlerFunction**

```java
HandlerFunction<ServerResponse> helloWorld =
  request -> ServerResponse.ok().bodyValue("Hello World");
```

如果有多个hander function，可以使用 一个 handler class 将 多个方法组合起来，类似于 controller的作用



**使用 Person repository构建响应式流**

```java
import static org.springframework.http.MediaType.APPLICATION_JSON;
import static org.springframework.web.reactive.function.server.ServerResponse.ok;

public class PersonHandler {

    private final PersonRepository repository;

    public PersonHandler(PersonRepository repository) {
        this.repository = repository;
    }

    public Mono<ServerResponse> listPeople(ServerRequest request) { 
        Flux<Person> people = repository.allPeople();
        return ok().contentType(APPLICATION_JSON).body(people, Person.class);
    }

    public Mono<ServerResponse> createPerson(ServerRequest request) { 
        Mono<Person> person = request.bodyToMono(Person.class);
        return ok().build(repository.savePerson(person));
    }

    public Mono<ServerResponse> getPerson(ServerRequest request) { 
        int personId = Integer.valueOf(request.pathVariable("id"));
        return repository.getPerson(personId)
            .flatMap(person -> ok().contentType(APPLICATION_JSON).bodyValue(person))
            .switchIfEmpty(ServerResponse.notFound().build());
    }
}
```





## Validation

functional endpoint 可以使用  Spring’s [validation facilities](https://docs.spring.io/spring-framework/docs/current/reference/html/core.html#validation)  以校验请求体

```java
public class PersonHandler {

    private final Validator validator = new PersonValidator(); 

    // ...

    public Mono<ServerResponse> createPerson(ServerRequest request) {
        Mono<Person> person = request.bodyToMono(Person.class).doOnNext(this::validate); 
        return ok().build(repository.savePerson(person));
    }

    private void validate(Person person) {
        Errors errors = new BeanPropertyBindingResult(person, "person");
        validator.validate(person, errors);
        if (errors.hasErrors()) {
            throw new ServerWebInputException(errors.toString()); 
        }
    }
}
```

Handlers 同样 使用 the standard bean validation API (JSR-303) ，通过 创建注入 全局 *Validator* 实例，基于`LocalValidatorFactoryBean`，See [Spring Validation](https://docs.spring.io/spring-framework/docs/current/reference/html/core.html#validation-beanvalidation)



# `RouterFunction`

使用  `RouterFunctions`工具类 创建 *RouterFunction*,`RouterFunctions.route()` 创建 链式构造器，`RouterFunctions.route(RequestPredicate, HandlerFunction)`直接创建 router

## Predicates

`RequestPredicates`工具类 提供基于request path, HTTP method, content-type等 常用的实现

```java
RouterFunction<ServerResponse> route = RouterFunctions.route()
    .GET("/hello-world", accept(MediaType.TEXT_PLAIN),
        request -> ServerResponse.ok().bodyValue("Hello World")).build();
```

可以组合 多个 request predicates 

- `RequestPredicate.and(RequestPredicate)` — both must match.
- `RequestPredicate.or(RequestPredicate)` — either can match.

来自 `RequestPredicates`  大多数 predicates 是组合的

例如：`RequestPredicates.GET(String)`  组合于 `RequestPredicates.method(HttpMethod)` `RequestPredicates.path(String)`

## Routes

Router functions  按顺序解析的：如果第一个 不匹配 解析第二个，因此 精确的 routes应该在 广泛的 routes 前面

将 RouterFunction 注册为 Spring beans 也是很重要的，与基于注解的 行为不同之处是：最佳匹配是自动计算的，

还有其他组合 多个 router functions的方法

- `add(RouterFunction)` on the `RouterFunctions.route()` builder
- `RouterFunction.and(RouterFunction)`
- `RouterFunction.andRoute(RequestPredicate, HandlerFunction)` — shortcut for `RouterFunction.and()` with nested `RouterFunctions.route()`.



**以下案例是 四个routes组合**

```java
import static org.springframework.http.MediaType.APPLICATION_JSON;
import static org.springframework.web.reactive.function.server.RequestPredicates.*;

PersonRepository repository = ...
PersonHandler handler = new PersonHandler(repository);

RouterFunction<ServerResponse> otherRoute = ...

RouterFunction<ServerResponse> route = route()
    //`GET /person/{id}` with an `Accept` header that matches JSON is routed to `PersonHandler.getPerson`
    .GET("/person/{id}", accept(APPLICATION_JSON), handler::getPerson) 
    //GET /person` with an `Accept` header that matches JSON is routed to `PersonHandler.listPeople
    .GET("/person", accept(APPLICATION_JSON), handler::listPeople) 
    //POST /person` with no additional predicates is mapped to `PersonHandler.createPerson
    .POST("/person", handler::createPerson) 
    //`otherRoute` is a router function that is created elsewhere, and added to the route built.
    .add(otherRoute) 
    .build();
```



## Nested Routes



一组 functions共享 一个predicate，例如 共享前缀

在上述例子中 就是 共享 `/person` 路径

当使用注解时，使用 `@RequestMapping`  注解在 类上来共享 `/person` 路径

在 WebFlux.fn  path predicates  可以 通过 router function  构造器共享

```java
RouterFunction<ServerResponse> route = route()
    .path("/person", builder -> builder 
        .GET("/{id}", accept(APPLICATION_JSON), handler::getPerson)
        .GET(accept(APPLICATION_JSON), handler::listPeople)
        .POST("/person", handler::createPerson))
    .build();
```

嵌套路由 表示外层的 predicate 在内层是共享的



基于 路径的嵌套式很普遍的  可以使用  `builder#nest` 方法

```java
RouterFunction<ServerResponse> route = route()
    .path("/person", b1 -> b1
        .nest(accept(APPLICATION_JSON), b2 -> b2
            .GET("/{id}", handler::getPerson)
            .GET(handler::listPeople))
        .POST("/person", handler::createPerson))
    .build();
```

## Running a Server

如何使用 router function 运行HTTP服务器，一个简单的 方法是 将 一个 router function 转换成 http handler

- `RouterFunctions.toHttpHandler(RouterFunction)`
- `RouterFunctions.toHttpHandler(RouterFunction, HandlerStrategies)`

将 返回的   `HttpHandler` 与 一系列 server adapters 配合使用，在SpringBooter中 使用 [`DispatcherHandler`](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-dispatcher-handler) ，它使用spring 配置化 声明 需要的组件

WebFlux Java configuration 声明了 以下基础设施组件

- `RouterFunctionMapping`: 检测 `RouterFunction<?>`  bean对象， [orders them](https://docs.spring.io/spring-framework/docs/current/reference/html/core.html#beans-factory-ordered), 通过`RouterFunction.andOther` 组合
- `HandlerFunctionAdapter`:  让  `DispatcherHandler`  执行 `HandlerFunction`  的简单适配器 
- `ServerResponseResultHandler`: 处理返回结果

```java
@Configuration
@EnableWebFlux
public class WebConfig implements WebFluxConfigurer {

    @Bean
    public RouterFunction<?> routerFunctionA() {
        // ...
    }

    @Bean
    public RouterFunction<?> routerFunctionB() {
        // ...
    }

    // ...

    @Override
    public void configureHttpMessageCodecs(ServerCodecConfigurer configurer) {
        // configure message conversion...
    }

    @Override
    public void addCorsMappings(CorsRegistry registry) {
        // configure CORS...
    }

    @Override
    public void configureViewResolvers(ViewResolverRegistry registry) {
        // configure view resolution for HTML rendering...
    }
}
```

# Filtering Handler Functions



可以在 builder 上使用  `before`, `after`, or `filter` 创建过滤器

也可以使用注解`@ControllerAdvice`、 `ServletFilter` 这些过滤器会 应用于所有 路由

```java
RouterFunction<ServerResponse> route = route()
    .path("/person", b1 -> b1
        .nest(accept(APPLICATION_JSON), b2 -> b2
            .GET("/{id}", handler::getPerson)
            .GET(handler::listPeople)
            .before(request -> ServerRequest.from(request) 
                .header("X-RequestHeader", "Value")
                .build()))
        .POST("/person", handler::createPerson))
    .after((request, response) -> logResponse(response)) 
    .build();
```

`before`过滤器 只应用于 两个 GET routes

 `after` 过滤器 应用于所有 路由



```java
SecurityManager securityManager = ...

RouterFunction<ServerResponse> route = route()
    .path("/person", b1 -> b1
        .nest(accept(APPLICATION_JSON), b2 -> b2
            .GET("/{id}", handler::getPerson)
            .GET(handler::listPeople))
        .POST("/person", handler::createPerson))
    .filter((request, next) -> {
        if (securityManager.allowAccessTo(request.path())) {
            return next.handle(request);
        }
        else {
            return ServerResponse.status(UNAUTHORIZED).build();
        }
    })
    .build();
```



Besides using the `filter` method on the router function builder, it is possible to apply a filter to an existing router function via `RouterFunction.filter(HandlerFilterFunction)`.

除了 在 builder上 时使用 过滤器方法，可以在已存在的  router function上应用 过滤器 通过 `RouterFunction.filter(HandlerFilterFunction)`

通过  [`CorsWebFilter`](https://docs.spring.io/spring-framework/docs/current/reference/html/webflux-cors.html#webflux-cors-webfilter) 支持 CORS

