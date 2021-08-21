# 静态资源处理

## 静态资源映射规则

SpringBoot中，SpringMVC的web配置都在 *WebMvcAutoConfiguration* 这个配置类里面；

*WebMvcAutoConfiguration.EnableWebMvcConfiguration#addResourceHandlers* 方法可以添加

```java
protected void addResourceHandlers(ResourceHandlerRegistry registry) {
    super.addResourceHandlers(registry);
    if (!this.resourceProperties.isAddMappings()) {
        logger.debug("Default resource handling disabled");
        return;
    }
    ServletContext servletContext = getServletContext();
    addResourceHandler(registry, "/webjars/**", "classpath:/META-INF/resources/webjars/");
    addResourceHandler(registry, this.mvcProperties.getStaticPathPattern(), (registration) -> {
        registration.addResourceLocations(this.resourceProperties.getStaticLocations());
        if (servletContext != null) {
            registration.addResourceLocations(new ServletContextResource(servletContext, SERVLET_LOCATION));
        }
    });
}
```

### webJar

比如所有的 /webjars/** ， 都需要去 classpath:/META-INF/resources/webjars/ 找对应的资源；

Webjars本质就是以jar包的方式引入我们的静态资源 ， 我们以前要导入一个静态资源文件，直接导入即可。

使用SpringBoot需要使用Webjars，网站：[https://www.webjars.org](https://www.webjars.org/)

### 静态目录

```java
private static final String[] CLASSPATH_RESOURCE_LOCATIONS = { 
    "classpath:/META-INF/resources/",
  "classpath:/resources/", 
    "classpath:/static/", 
    "classpath:/public/" 
};
```

# 自定义静态资源路径

spring.resources.static-locations=classpath:/coding/,classpath:/kuang/
**一旦自己定义了静态文件夹的路径，原来的自动配置就都会失效了！**



`



