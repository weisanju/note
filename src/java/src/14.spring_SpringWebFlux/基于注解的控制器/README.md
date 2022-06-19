# Annotated Controllers

Spring WebFlux 提供 基于注解的编程模型，使用`@Controller` and `@RestController`  组件表示 请求映射、请求输入、处理异常以及其他，基于注解的控制器 方法申明很灵活，没必要继承 基类或者实现接口

Java

```java
@RestController
public class HelloController {

    @GetMapping("/hello")
    public String handle() {
        return "Hello WebFlux";
    }
}
```

