# 简介

本部分介绍了Spring框架中可用于URI的各种选项。



# UriComponents

UriComponentsBuilder有助于从具有变量的URI模板中构建URI，如以下示例所示：

```java
UriComponents uriComponents = UriComponentsBuilder
        .fromUriString("https://example.com/hotels/{hotel}")  
        .queryParam("q", "{q}")  
        .encode() 
        .build(); 

URI uri = uriComponents.expand("Westin", "123").toUri();  
```

可以将前面的示例合并为一个链，并通过buildAndExpand进行缩短，如以下示例所示：

```
URI uri = UriComponentsBuilder
        .fromUriString("https://example.com/hotels/{hotel}")
        .queryParam("q", "{q}")
        .encode()
        .buildAndExpand("Westin", "123")
        .toUri();
```

您可以通过直接转到URI（这意味着编码）来进一步缩短它，如以下示例所示：

```java
URI uri = UriComponentsBuilder
        .fromUriString("https://example.com/hotels/{hotel}")
        .queryParam("q", "{q}")
        .build("Westin", "123");
```

您可以使用完整的URI模板进一步缩短它，如以下示例所示：

```java
URI uri = UriComponentsBuilder
        .fromUriString("https://example.com/hotels/{hotel}?q={q}")
        .build("Westin", "123");
```

# UriBuilder

* UriComponentsBuilder实现UriBuilder。
* 您可以依次使用UriBuilderFactory创建UriBuilder。 
* UriBuilderFactory和UriBuilder一起提供了一种可插入的机制，
* 可以基于共享配置（例如基本URL，编码首选项和其他详细信息）从URI模板构建URI。
* 您可以使用UriBuilderFactory配置RestTemplate和WebClient以自定义URI的准备。 
* DefaultUriBuilderFactory是UriBuilderFactory的默认实现，该实现在内部使用UriComponentsBuilder并公开共享的配置选项。
    以下示例显示如何配置RestTemplate：

```java
// import org.springframework.web.util.DefaultUriBuilderFactory.EncodingMode;

String baseUrl = "https://example.org";
DefaultUriBuilderFactory factory = new DefaultUriBuilderFactory(baseUrl);
factory.setEncodingMode(EncodingMode.TEMPLATE_AND_VALUES);

RestTemplate restTemplate = new RestTemplate();
restTemplate.setUriTemplateHandler(factory);
```

The following example configures a `WebClient`:

```java
// import org.springframework.web.util.DefaultUriBuilderFactory.EncodingMode;

String baseUrl = "https://example.org";
DefaultUriBuilderFactory factory = new DefaultUriBuilderFactory(baseUrl);
factory.setEncodingMode(EncodingMode.TEMPLATE_AND_VALUES);

WebClient client = WebClient.builder().uriBuilderFactory(factory).build();
```

此外，您也可以直接使用DefaultUriBuilderFactory。
它类似于使用UriComponentsBuilder，但不是静态工厂方法，它是一个包含配置和首选项的实际实例，如以下示例所示：



```java
String baseUrl = "https://example.com";
DefaultUriBuilderFactory uriBuilderFactory = new DefaultUriBuilderFactory(baseUrl);

URI uri = uriBuilderFactory.uriString("/hotels/{hotel}")
        .queryParam("q", "{q}")
        .build("Westin", "123");
```

# URI Encoding

`UriComponentsBuilder` exposes encoding options at two levels:

- [UriComponentsBuilder#encode()](https://docs.spring.io/spring-framework/docs/5.3.3/javadoc-api/org/springframework/web/util/UriComponentsBuilder.html#encode--): Pre-encodes the URI template first and then strictly encodes URI variables when expanded.
- [UriComponents#encode()](https://docs.spring.io/spring-framework/docs/5.3.3/javadoc-api/org/springframework/web/util/UriComponents.html#encode--): Encodes URI components *after* URI variables are expanded.

这两个选项都使用转义的八位字节替换非ASCII和非法字符。
但是，第一个选项还会替换出现在URI变量中的具有保留含义的字符。

思考 “;" 这在路径上是合法的，但具有保留的含义。
第一个选项代替“;”  URI变量中带有“％3B”，但URI模板中没有。 相比之下，第二个选项永远不会替换“;”，因为它是路径中的合法字符。



