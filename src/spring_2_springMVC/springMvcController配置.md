# spring配置文件写法

```xml
<context:component-scan base-package="com.lzzcms" >
<context:exclude-filter type="annotation" expression="org.springframework.stereotype.Controller"></context:exclude-filter>
</context:component-scan>
```



# springMVC配置文件写法

```xml
<context:component-scan base-package="com.lzzcms" use-default-filters="false">
<context:include-filter type="annotation"  expression="org.springframework.stereotype.Controller"></context:include-filter>
</context:component-scan>
```

默认情况下 context:component-scan  会扫描 一下注解

```
@Component, @Repository, @Service,
@Controller, @RestController, @ControllerAdvice, and @Configuration
```