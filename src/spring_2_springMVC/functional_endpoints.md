# 简介

* Spring Web MVC包含WebMvc.fn，这是一个轻量级的函数编程模型，其中的函数用于路由和处理请求，而接口则是为不变性而设计的。
    它是基于注解的编程模型的替代方案，但可以在同一DispatcherServlet上运行。

* 在WebMvc.fn中，HTTP请求由HandlerFunction处理：该函数接受ServerRequest并返回ServerResponse。
    请求和响应对象都具有不可变的约定，这些约定为JDK 8提供了对HTTP请求和响应的友好访问。 
    *HandlerFunction* 等效于基于注解的编程模型中@RequestMapping方法的主体。

* 即将到来的请求通过RouterFunction路由到处理程序函数：此函数接受ServerRequest并返回可选的HandlerFunction（即Optional ）。
    当路由器功能匹配时，返回处理程序功能。
    否则为空的Optional。 
    RouterFunction等效于@RequestMapping批注，但主要区别在于路由器功能不仅提供数据，还提供行为。

RouterFunctions.route（）提供了一个路由器构建器，可简化路由器的创建过程，如以下示例所示：

```java
import static org.springframework.http.MediaType.APPLICATION_JSON;
import static org.springframework.web.servlet.function.RequestPredicates.*;
import static org.springframework.web.servlet.function.RouterFunctions.route;

PersonRepository repository = ...
PersonHandler handler = new PersonHandler(repository);

RouterFunction<ServerResponse> route = route()
    .GET("/person/{id}", accept(APPLICATION_JSON), handler::getPerson)
    .GET("/person", accept(APPLICATION_JSON), handler::listPeople)
    .POST("/person", handler::createPerson)
    .build();


public class PersonHandler {

    // ...

    public ServerResponse listPeople(ServerRequest request) {
        // ...
    }

    public ServerResponse createPerson(ServerRequest request) {
        // ...
    }

    public ServerResponse getPerson(ServerRequest request) {
        // ...
    }
}
```

# HandlerFunction

ServerRequest和ServerResponse是不可变的接口，它们提供JDK 8友好的HTTP请求和响应访问，包括请求头，正文，方法和状态代码。

## ServerRequest

ServerRequest提供对HTTP方法，URI，标头和查询参数的访问，而通过body方法提供对 请求体的访问。

**下面的示例将请求正文提取为String：**

```java
String string = request.body(String.class);
```

**以下示例将主体提取到List ，其中Person对象从序列化形式（例如JSON或XML）解码：**

```java
List<Person> people = request.body(new ParameterizedTypeReference<List<Person>>() {});
```

**访问请求参数**

```java
MultiValueMap<String, String> params = request.params();
```

## ServerResponse

ServerResponse提供对HTTP响应的访问，并且由于它是不可变的，因此您可以使用构建方法来创建它。
您可以使用构建器来设置响应状态，添加响应标题或提供正文。
以下示例使用JSON内容创建200（确定）响应：

```java
Person person = ...
ServerResponse.ok().contentType(MediaType.APPLICATION_JSON).body(person);
```

**下面的示例演示如何构建一个具有Location标头且没有正文的201（已创建）响应：**

```java
URI location = ...
ServerResponse.created(location).build();
```

您还可以将异步结果用作主体，形式为CompletableFuture，Publisher或ReactiveAdapterRegistry支持的任何其他类型。
例如：

```java
Mono<Person> person = webClient.get().retrieve().bodyToMono(Person.class);
ServerResponse.ok().contentType(MediaType.APPLICATION_JSON).body(person);
```

如果不仅正文，而且状态或标头都基于异步类型，则可以在ServerResponse上使用静态异步方法，该方法接受CompletableFuture ，Publisher 或ReactiveAdapterRegistry支持的任何其他异步类型

```
Mono<ServerResponse> asyncResponse = webClient.get().retrieve().bodyToMono(Person.class)
  .map(p -> ServerResponse.ok().header("Name", p.name()).body(p));
ServerResponse.async(asyncResponse);
```

可以通过ServerResponse上的静态sse方法提供服务器发送的事件。[Server-Sent Events](https://www.w3.org/TR/eventsource/) 
该方法提供的构建器允许您将字符串或其他对象作为JSON发送。例如：

```java
public RouterFunction<ServerResponse> sse() {
    return route(GET("/sse"), request -> ServerResponse.sse(sseBuilder -> {
                // Save the sseBuilder object somewhere..
            }));
}

// In some other thread, sending a String
sseBuilder.send("Hello world");

// Or an object, which will be transformed into JSON
Person person = ...
sseBuilder.send(person);

// Customize the event by using the other methods
sseBuilder.id("42")
        .event("sse event")
        .data(person);

// and done at some point
sseBuilder.complete();
```

## Handler Classes

我们可以将处理程序函数编写为lambda，如以下示例所示：

```java
HandlerFunction<ServerResponse> helloWorld =
  request -> ServerResponse.ok().body("Hello World");
```

这很方便，但是在应用程序中我们需要多个功能，并且多个内联lambda可能会变得凌乱。因此，将相关的处理程序功能分组到一个处理程序类中很有用，该类的作用与基于 注解的应用程序中的@Controller相似。
例如，以下类公开了反应型Person存储库：

```java
import static org.springframework.http.MediaType.APPLICATION_JSON;
import static org.springframework.web.reactive.function.server.ServerResponse.ok;

public class PersonHandler {

    private final PersonRepository repository;

    public PersonHandler(PersonRepository repository) {
        this.repository = repository;
    }

    public ServerResponse listPeople(ServerRequest request) { 
        List<Person> people = repository.allPeople();
        return ok().contentType(APPLICATION_JSON).body(people);
    }

    public ServerResponse createPerson(ServerRequest request) throws Exception { 
        Person person = request.body(Person.class);
        repository.savePerson(person);
        return ok().build();
    }

    public ServerResponse getPerson(ServerRequest request) { 
        int personId = Integer.parseInt(request.pathVariable("id"));
        Person person = repository.getPerson(personId);
        if (person != null) {
            return ok().contentType(APPLICATION_JSON).body(person);
        }
        else {
            return ServerResponse.notFound().build();
        }
    }

}
```

## Validation

功能端点可以使用Spring的验证工具将验证应用于请求正文。例如，给定Person的自定义Spring Validator实现：

```java
public class PersonHandler {

    private final Validator validator = new PersonValidator(); 

    // ...

    public ServerResponse createPerson(ServerRequest request) {
        Person person = request.body(Person.class);
        validate(person); 
        repository.savePerson(person);
        return ok().build();
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

# RouterFunction

* 路由器功能用于将请求路由到相应的HandlerFunction。
* 通常，您不是自己编写路由器功能，而是使用RouterFunctions实用工具类上的方法创建一个。 
    RouterFunctions.route（）（无参数）为您提供了一个流畅的生成器来创建路由器功能，而RouterFunctions.route（RequestPredicate，HandlerFunction）提供了直接创建路由器的方法。

* 除了基于HTTP方法的映射外，路由构建器还提供了一种在映射到请求时引入其他谓词的方法。
    对于每个HTTP方法，都有一个以RequestPredicate作为参数的重载变体，尽管可以表达其他约束。
* 您可以编写自己的RequestPredicate，但是RequestPredicates实用程序类根据请求路径，HTTP方法，内容类型等提供常用的实现。
    以下示例使用请求谓词基于Accept头创建约束：

```java
RouterFunction<ServerResponse> route = RouterFunctions.route()
    .GET("/hello-world", accept(MediaType.TEXT_PLAIN),
        request -> ServerResponse.ok().body("Hello World")).build();
```



## 谓词

您可以使用以下命令将多个请求谓词组合在一起

- `RequestPredicate.and(RequestPredicate)` — both must match.
- `RequestPredicate.or(RequestPredicate)` — either can match.

RequestPredicates中的许多谓词都是组成的。
例如，RequestPredicates.GET（String）由RequestPredicates.method（HttpMethod）和RequestPredicates.path（String）组成。
上面显示的示例还使用了两个请求谓词，因为构建器在内部使用RequestPredicates.GET并将其与接受谓词组合在一起



* 路由器功能按顺序评估：如果第一个路由不匹配，则评估第二个路由，依此类推。
    因此，在通用路由之前声明更具体的路由是有意义的。
    请注意，此行为不同于基于注释的编程模型，在该模型中，将自动选择“最特定”的控制器方法。
* 使用路由器功能生成器时，所有定义的路由都组成一个RouterFunction，从build（）返回。
    还有其他方法可以将多个路由器功能组合在一起：
    * `add(RouterFunction)` on the `RouterFunctions.route()` builder
    * `RouterFunction.and(RouterFunction)`
    * `RouterFunction.andRoute(RequestPredicate, HandlerFunction)` — shortcut for `RouterFunction.and()` with nested `RouterFunctions.route()`.



```java
import static org.springframework.http.MediaType.APPLICATION_JSON;
import static org.springframework.web.servlet.function.RequestPredicates.*;

PersonRepository repository = ...
PersonHandler handler = new PersonHandler(repository);

RouterFunction<ServerResponse> otherRoute = ...

RouterFunction<ServerResponse> route = route()
    .GET("/person/{id}", accept(APPLICATION_JSON), handler::getPerson) 
    .GET("/person", accept(APPLICATION_JSON), handler::listPeople) 
    .POST("/person", handler::createPerson) 
    .add(otherRoute) 
    .build();
```



## Nested Routes

* 一组路由器功能通常具有共享谓词，例如共享路径。
* 在上面的示例中，共享谓词将是与/ person匹配的路径谓词，由三个路由使用。
* 使用注释时，您可以通过使用映射到/ person的类型级别@RequestMapping注释来删除此重复项。
* 在WebMvc.fn中，可以通过路由器功能构建器上的path方法共享路径谓词。例如，可以通过使用嵌套路由以以下方式改进上面示例的最后几行：

```java
RouterFunction<ServerResponse> route = route()
    .path("/person", builder -> builder 
        .GET("/{id}", accept(APPLICATION_JSON), handler::getPerson)
        .GET(accept(APPLICATION_JSON), handler::listPeople)
        .POST("/person", handler::createPerson))
    .build();
```

```java
RouterFunction<ServerResponse> route = route()
    .path("/person", b1 -> b1
        .nest(accept(APPLICATION_JSON), b2 -> b2
            .GET("/{id}", handler::getPerson)
            .GET(handler::listPeople))
        .POST("/person", handler::createPerson))
    .build();
```

# Running a Server

通常，您可以通过MVC Config在基于DispatcherHandler的设置中运行路由器功能，该配置使用Spring配置来声明处理请求所需的组件。 
MVC Java配置声明以下基础结构组件以支持 functional endpoints

- `RouterFunctionMapping`:在spring配置中 检测到一个或多个 `RouterFunction<?>` beans ,通过 `RouterFunction.andOther`,组合 并将请求路由到组成的“RouterFunction”。
- `HandlerFunctionAdapter`:  lets `DispatcherHandler` invoke a `HandlerFunction` that was mapped to a request.



前面的组件使 *functional endpoints* 适合于DispatcherServlet请求处理生命周期，并且（可能）与带注释的控制器并排运行。
这也是Spring Boot Web启动程序如何启用 *functional endpoints* 的方式。

以下示例显示了WebFlux Java配置：

```java
@Configuration
@EnableMvc
public class WebConfig implements WebMvcConfigurer {

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
    public void configureMessageConverters(List<HttpMessageConverter<?>> converters) {
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

* 您可以使用路由功能构建器上的before，after或filter方法来过滤处理程序函数。

* 使用注解，可以通过使用@ ControllerAdvice，ServletFilter或同时使用两者来实现类似的功能。
* 该过滤器将应用于构建器构建的所有路由。 这意味着在嵌套路由中定义的过滤器不适用于“顶级”路由。例如，考虑以下示例：

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

The `before` filter that adds a custom request header is only applied to the two GET routes.

The `after` filter that logs the response is applied to all routes, including the nested ones.

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

除了在路由器功能构建器上使用filter方法之外，还可以通过RouterFunction.filter（HandlerFilterFunction）将过滤器应用于现有路由器功能。



