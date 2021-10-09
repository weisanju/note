# WebFlux Config

使用 `@EnableWebFlux`  注解启用 WebFlux

```java
@Configuration
@EnableWebFlux
public class WebConfig {
}
```

以上例子注册了一系列 的 SpringWebFlux [infrastructure beans](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-special-bean-types) ，自动应用 可用的依赖：JSON、XML

# WebFlux config API

实现`WebFluxConfigurer` 去配置

```java
@Configuration
@EnableWebFlux
public class WebConfig implements WebFluxConfigurer {
    // Implement configuration methods...
}
```

## Conversion, formatting

默认情况下，各种数据类型的 格式化都已安装。也支持字段的 `@NumberFormat` `@DateTimeFormat`

**注册Formatter**

```java
@Configuration
@EnableWebFlux
public class WebConfig implements WebFluxConfigurer {
    @Override
    public void addFormatters(FormatterRegistry registry) {
        // ...
    }
}
```

默认情况下，Spring WebFlux 在解析或格式化日期格式 考虑 到 request Locale

自定义日期时间格式化

```java
@Configuration
@EnableWebFlux
public class WebConfig implements WebFluxConfigurer {

    @Override
    public void addFormatters(FormatterRegistry registry) {
        DateTimeFormatterRegistrar registrar = new DateTimeFormatterRegistrar();
        registrar.setUseIsoFormat(true);
        registrar.registerFormatters(registry);
    }
}
```

当使用 `FormatterRegistrar`  时，详见 [`FormatterRegistrar` SPI](https://docs.spring.io/spring-framework/docs/current/reference/html/core.html#format-FormatterRegistrar-SPI)   以及 `FormattingConversionServiceFactoryBean`  



## Validation

默认情况下：如果 [Bean Validation](https://docs.spring.io/spring-framework/docs/current/reference/html/core.html#validation-beanvalidation-overview)  在 类路径上（例如：the Hibernate Validator），`LocalValidatorFactoryBean`  会注册为 全局 [validator](https://docs.spring.io/spring-framework/docs/current/reference/html/core.html#validator) 供 @Controller方法参数上的  `@Valid` and `@Validated`  使用

**可以自定义 Validator实例**

```java
@Configuration
@EnableWebFlux
public class WebConfig implements WebFluxConfigurer {

    @Override
    public Validator getValidator(); {
        // ...
    }

}
```

**应用本地`Validator`**  

```java
@Controller
public class MyController {

    @InitBinder
    protected void initBinder(WebDataBinder binder) {
        binder.addValidators(new FooValidator());
    }
}
```

如果需要`LocalValidatorFactoryBean`  注入，创建一个 bean 使用 `@Primary`  标注，避免 在 MVC config 中冲突



## Content Type Resolvers

可以配置 Spring WebFlux如何 判断 请求的 媒体类型

默认情况下，会检查 `Accept` 头，但是你可以 启用 基于 查询参数的 策略

```java
@Configuration
@EnableWebFlux
public class WebConfig implements WebFluxConfigurer {

    @Override
    public void configureContentTypeResolver(RequestedContentTypeResolverBuilder builder) {
        // ...
    }
}
```

## HTTP message codecs

**自定义 request 和 response body 被读写**

```java
@Configuration
@EnableWebFlux
public class WebConfig implements WebFluxConfigurer {

    @Override
    public void configureHttpMessageCodecs(ServerCodecConfigurer configurer) {
        configurer.defaultCodecs().maxInMemorySize(512 * 1024);
    }
}
```

`ServerCodecConfigurer` 提供默认的 读写器，可以添加，自定义修改默认的配置，对于

For Jackson JSON and XML, consider using [`Jackson2ObjectMapperBuilder`](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/http/converter/json/Jackson2ObjectMapperBuilder.html), which customizes Jackson’s default properties with the following ones:

- [`DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES`](https://fasterxml.github.io/jackson-databind/javadoc/2.6/com/fasterxml/jackson/databind/DeserializationFeature.html#FAIL_ON_UNKNOWN_PROPERTIES) is disabled.
- [`MapperFeature.DEFAULT_VIEW_INCLUSION`](https://fasterxml.github.io/jackson-databind/javadoc/2.6/com/fasterxml/jackson/databind/MapperFeature.html#DEFAULT_VIEW_INCLUSION) is disabled.

It also automatically registers the following well-known modules if they are detected on the classpath:

- [`jackson-datatype-joda`](https://github.com/FasterXML/jackson-datatype-joda): Support for Joda-Time types.
- [`jackson-datatype-jsr310`](https://github.com/FasterXML/jackson-datatype-jsr310): Support for Java 8 Date and Time API types.
- [`jackson-datatype-jdk8`](https://github.com/FasterXML/jackson-datatype-jdk8): Support for other Java 8 types, such as `Optional`.
- [`jackson-module-kotlin`](https://github.com/FasterXML/jackson-module-kotlin): Support for Kotlin classes and data classes.

## View Resolvers

**视图解析**

```java
@Configuration
@EnableWebFlux
public class WebConfig implements WebFluxConfigurer {

    @Override
    public void configureViewResolvers(ViewResolverRegistry registry) {
        // ...
    }
}
```



The `ViewResolverRegistry`  有配置模板库的快捷方式

```java
@Configuration
@EnableWebFlux
public class WebConfig implements WebFluxConfigurer {


    @Override
    public void configureViewResolvers(ViewResolverRegistry registry) {
        registry.freeMarker();
    }

    // Configure Freemarker...

    @Bean
    public FreeMarkerConfigurer freeMarkerConfigurer() {
        FreeMarkerConfigurer configurer = new FreeMarkerConfigurer();
        configurer.setTemplateLoaderPath("classpath:/templates");
        return configurer;
    }
}
```

配置自定义的视图

```java
@Configuration
@EnableWebFlux
public class WebConfig implements WebFluxConfigurer {


    @Override
    public void configureViewResolvers(ViewResolverRegistry registry) {
        ViewResolver resolver = ... ;
        registry.viewResolver(resolver);
    }
}
```



为了支持 [Content Negotiation](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-multiple-representations)  ，渲染除了 HTML的 其他格式，可以配置多个基于 `HttpMessageWriterView`的默认视图，它从 `spring-web` 中接收多种可用的  [Codecs](https://docs.spring.io/spring-framework/docs/current/reference/html/web-reactive.html#webflux-codecs) 

```java
@Configuration
@EnableWebFlux
public class WebConfig implements WebFluxConfigurer {

    @Override
    public void configureViewResolvers(ViewResolverRegistry registry) {
        registry.freeMarker();
        Jackson2JsonEncoder encoder = new Jackson2JsonEncoder();
        registry.defaultViews(new HttpMessageWriterView(encoder));
    }
    // ...
}
```



## Static Resources

基于location的 静态资源服务的便捷方法，下面例子中，`/resources` 使用相对路径查找静态资源，相对于 *classPath* 下,资源在一年到期，以确保浏览器缓存的最大使用 和减少HTTP请求次数，`Last-Modified` 请求头 会校验，如果存在则返回304

```java
@Configuration
@EnableWebFlux
public class WebConfig implements WebFluxConfigurer {

    @Override
    public void addResourceHandlers(ResourceHandlerRegistry registry) {
        registry.addResourceHandler("/resources/**")
            .addResourceLocations("/public", "classpath:/static/")
            .setCacheControl(CacheControl.maxAge(365, TimeUnit.DAYS));
    }

}
```



资源处理器 同样支持 [`ResourceResolver`](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/web/reactive/resource/ResourceResolver.html) 与  [`ResourceTransformer`](https://docs.spring.io/spring-framework/docs/5.3.10/javadoc-api/org/springframework/web/reactive/resource/ResourceTransformer.html)  链式处理

可以 使用`VersionResourceResolver` ， 对版本化资源，可以使用 资源内容的MD5、固定应用程序版本等

`ContentVersionStrategy` (基于内容的MD5) 是一个好的选择，但是有一个明显的缺点（无法处理，使用 module loader的 js）

```java
@Configuration
@EnableWebFlux
public class WebConfig implements WebFluxConfigurer {

    @Override
    public void addResourceHandlers(ResourceHandlerRegistry registry) {
        registry.addResourceHandler("/resources/**")
                .addResourceLocations("/public/")
                .resourceChain(true)
                .addResolver(new VersionResourceResolver().addContentVersionStrategy("/**"));
    }
}
```

You can use `ResourceUrlProvider` to rewrite URLs and apply the full chain of resolvers and transformers (for example, to insert versions). The WebFlux configuration provides a `ResourceUrlProvider` so that it can be injected into others.



Unlike Spring MVC, at present, in WebFlux, there is no way to transparently rewrite static resource URLs, since there are no view technologies that can make use of a non-blocking chain of resolvers and transformers. When serving only local resources, the workaround is to use `ResourceUrlProvider` directly (for example, through a custom element) and block.

Note that, when using both `EncodedResourceResolver` (for example, Gzip, Brotli encoded) and `VersionedResourceResolver`, they must be registered in that order, to ensure content-based versions are always computed reliably based on the unencoded file.

[WebJars](https://www.webjars.org/documentation) are also supported through the `WebJarsResourceResolver` which is automatically registered when the `org.webjars:webjars-locator-core` library is present on the classpath. The resolver can re-write URLs to include the version of the jar and can also match against incoming URLs without versions — for example, from `/jquery/jquery.min.js` to `/jquery/1.2.0/jquery.min.js`.

## Path Matching

**自定义路径匹配**

```java
@Configuration
@EnableWebFlux
public class WebConfig implements WebFluxConfigurer {

    @Override
    public void configurePathMatch(PathMatchConfigurer configurer) {
        configurer
            .setUseCaseSensitiveMatch(true)
            .setUseTrailingSlashMatch(false)
            .addPathPrefix("/api",
                    HandlerTypePredicate.forAnnotation(RestController.class));
    }
}
```





## WebSocketService

WebFlux 定义了 WebSocketHandlerAdapter  提供对 WebSocket handlers 的支持，意味着所有，要处理网络套接字握手请求，所有做的是将 `WebSocketHandler`  映射到 URL 通过 `SimpleUrlHandlerMapping`

```java
@Configuration
@EnableWebFlux
public class WebConfig implements WebFluxConfigurer {

    @Override
    public WebSocketService getWebSocketService() {
        TomcatRequestUpgradeStrategy strategy = new TomcatRequestUpgradeStrategy();
        strategy.setMaxSessionIdleTimeout(0L);
        return new HandshakeWebSocketService(strategy);
    }
}
```

## Advanced Configuration Mode

`@EnableWebFlux` imports `DelegatingWebFluxConfiguration` that:

- Provides default Spring configuration for WebFlux applications
- detects and delegates to `WebFluxConfigurer` implementations to customize that configuration.

**继承DelegatingWebFluxConfiguration**

```java
@Configuration
public class WebConfig extends DelegatingWebFluxConfiguration {
    // ...
}
```

## HTTP/2

HTTP/2 is supported with Reactor Netty, Tomcat, Jetty, and Undertow. However, there are considerations related to server configuration. For more details, see the [HTTP/2 wiki page](https://github.com/spring-projects/spring-framework/wiki/HTTP-2-support).

