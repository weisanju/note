`<mvc:annotation-driven>`

* 会自动注册 *RequestMappingHandlerMapping* 与 *RequestMappingHandlerAdapter* 两个Bean,这是Spring MVC为@Controller分发请求所必需的

* 并且提供了数据绑定支持，@*NumberFormatannotation*支持，@*DateTimeFormat*支持,@Valid支持

* 读写XML的支持（JAXB）

    ```
    JAXB能够使用Jackson对JAXB注解的支持实现(jackson-module-jaxb-annotations)，既方便生成XML，也方便生成JSON，这样一来可以更好的标志可以转换为JSON对象的JAVA类。JAXB允许JAVA人员将JAVA类映射为XML表示方式，常用的注解包括：@XmlRootElement,@XmlElement等等。
    
        JAXB（Java Architecture for XML Binding) 是一个业界的标准，是一项可以根据XML Schema产生Java类的技术。该过程中，JAXB也提供了将XML实例文档反向生成Java对象树的方法，并能将Java对象树的内容重新写到XML实例文档。从另一方面来讲，JAXB提供了快速而简便的方法将XML模式绑定到Java表示，从而使得Java开发者在Java应用程序中能方便地结合XML数据和处理函数。
    ```

* 读写JSON的支持（默认Jackson）等功能。

使用该注解后的springmvc-config.xml:

```xml
<!--  spring 可以自动去扫描 base-package下面的包或子包下面的Java文件，如果扫描到有Spring的相关注解的类，则把这些类注册为Spring的bean -->
<!--设置配置方案 -->
<context:component-scan base-package="org.fkit.controller"/>
<mvc:annotation-driven/>

<!--使用默认的Servlet来响应静态文件-->

<mvc:default-servlet-handler/>

<!-- 视图解析器 -->
<bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
<!-- 前缀 -->
<property name="prefix">
<value>/WEB-INF/content/</value>
</property>
<!-- 后缀 -->
<property name="suffix">
<value>.jsp</value>
</property>
</bean>
```

