# SpringWebMVC

Spring Web 模型-视图-控制（MVC）框架是围绕 *DispatcherServlet* 设计的

1. DispatchServlet 收到 Http请求后，根据 *HandlerMapping* 来选择并且调用适当的*控制器*
2. 控制器接受请求，调用适当的service，service设置适当的模型数据，返回视图名到DispatchServlet
3. *DispatcherServlet* 会从 *ViewResolver* 获取帮助，为请求检取定义视图
4. 一旦确定视图，*DispatcherServlet* 将把模型数据传递给视图

HandlerMapping、Controller 和 ViewResolver 是 *WebApplicationContext* 的一部分

## HandlerMapping映射配置

DispatchServlet配置在web.xml 中

```
<servlet>
      <servlet-name>HelloWeb</servlet-name>
      <servlet-class>
         org.springframework.web.servlet.DispatcherServlet
      </servlet-class>
      <load-on-startup>1</load-on-startup>
</servlet>
```

该框架将尝试加载位于该应用程序的 *WebContent/WEB-INF* 目录中文件名为 `[servlet-name]-servlet.xml` 的应用程序内容

想要改变默认位置,不使用默认名,配置在web.xml中

```xml
<context-param>
   <param-name>contextConfigLocation</param-name>
   <param-value>/WEB-INF/HelloWeb-servlet.xml</param-value>
</context-param>
<listener>
   <listener-class>
      org.springframework.web.context.ContextLoaderListener
   </listener-class>
</listener>
```

**HelloWeb-servlet.xml** 文件的请求配置

```xml
   <context:component-scan base-package="com.tutorialspoint" />

   <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
      <property name="prefix" value="/WEB-INF/jsp/" />
      <property name="suffix" value=".jsp" />
   </bean>
```

 **HelloWeb-servlet.xml** 文件的一些要点：

* 用于创建 bean 定义，重新定义在全局范围内具有相同名称的任何已定义的 bean
* context:component-scan: 激活 Spring MVC 注释扫描功能，该功能允许使用注释，如 @Controller 和 @RequestMapping
* *InternalResourceViewResolver* :将使用定义的规则来解决视图名称
* 按照上述定义的规则，一个名称为 **hello** 的逻辑视图将发送给位于 `/WEB-INF/jsp/hello.jsp` 中实现的视图

## 定义控制器

**@Controller** 注释表明一个特定类是一个控制器的作用。

**@RequestMapping** 注释用于映射 URL 到整个类或一个特定的处理方法。

## 创建 JSP 视图

Spring MVC 支持许多类型的视图

包括 JSP、HTML、PDF、Excel 工作表、XML、Velocity 模板、XSLT、JSON、Atom 和 RSS 提要、JasperReports 



@ModelAttribute：解析表单变量

重定向：return "redirect:finalPage";

## Web 静态页面

 标签被用来映射静态页面

 `<mvc:resources mapping="/pages/**" location="/WEB-INF/pages/" />`

## 异常处理

```xml
<bean class="org.springframework.web.servlet.handler.
      SimpleMappingExceptionResolver">
   <property name="exceptionMappings">
      <props>
         <prop key="com.tutorialspoint.SpringException">
            ExceptionPage
         </prop>
      </props>
   </property>
   <property name="defaultErrorView" value="error"/>
</bean>

```

${exception}  可以访问异常对象