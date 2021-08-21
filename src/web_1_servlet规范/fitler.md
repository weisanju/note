# 什么是过滤器

它可以改变 HTTP 请求的内容，响应，及 header 信息。过滤器通常不产生
响应或像 servlet 那样对请求作出响应，而是修改或调整到资源的请求，修改或调整来自资源的响应。过滤器可以作用于动态或静态内容。



# 过滤器组件示例

■ 验证过滤器
■ 日志记录和审计过滤器
■ 图像转换过滤器
■ 数据压缩过滤器
■ 加密过滤器
■ 词法（Tokenizing）过滤器
■ 触发资源访问事件过滤器
■ 转换 XML 内容的 XSL/T 过滤器
■ MIME-类型链过滤器
■ 缓存过滤器



# Filter概念

Filter 在部署描述符中通过<filter>元素声明。
一个过滤器或一组过滤器可以通过在部署描述符中定义<filter-mapping>来为调用配置

**过滤器的 doFilter 方法通常会被实现为如下或如下形式的子集**

* 该方法检查请求的头。
* 该方法可以用自定义的ServletRequest或HttpServletRequest实现包装请求对象为了修改请求的头或数据
* 该方法可以用自定义的ServletResponse 或 HttpServletResponse实现包装传入doFilter方法的响应对象用
    于修改响应的头或数据。

**重试机制**

过滤器可能抛出一个异常以表示处理过程中出错了。如果过滤器在 doFilter 处理过程中抛出
**UnavailableException，容器必须停止处理剩下的过滤器链。 如果异常没有标识为永久的( isPermanent 属性)，它或许选择稍候重试整个链。**



当链中的最后的过滤器被调用，下一个实体访问的是链最后的目标 servlet 或资源

在容器能把服务中的过滤器实例移除之前，容器必须先调用过滤器的 destroy 方法以便过滤器释放资源并执行其他的清理工作。



# 包装请求和响应

过滤器的核心概念是包装请求或响应，以便它可以覆盖行为执行过滤任务。在这个模型中，开发人员不仅
可以覆盖请求和响应对象上已有的方法，也能提供新的API以适用于对过滤器链中剩下的过滤器或目标web
资源做特殊的过滤任务。例如，开发人员可能希望用更高级别的输出对象如 output stream 或 writer 来扩展
响应对象，如 API，允许 DOM 对象写回客户端。
为了支持这种风格的过滤器，容器必须支持如下要求。当过滤器调用容器的过滤器链实现的 doFilter 方法
时，容器必须确保请求和响应对象传到过滤器链中的下一个实体，或如果过滤器是链中最后一个，将传入
目标 web 资源，且与调用过滤器传入 doFilter 方法的对象是一样的。
当 调 用 者 包 装 请 求 或 响 应 对 象 时 ， 对 包 装 对 象 的 要 求 同 样 适 用 于 从 servlet 或 过 滤 器 到
RequestDispatcher.forward 或 RequestDispatcher.include 的调用。在这种情况下，调用 servlet 看到的请求和
响应对象与调用 servlet 或过滤器传入的包装对象必须是一样的。



# 过滤器环境

可以使用部署描述符中的<init-params>元素把一组初始化参数关联到过滤器。这些参数的名字和值在过滤
器运行期间可以使用过滤器的FilterConfig对象的getInitParameter和getInitParameterNames方法得到。另外，
FilterConfig 提供访问 Web 应用的 ServletContext 用于加载资源，记录日志，在 ServletContext 的属性列表存
储状态。链中最后的过滤器和目标 servlet 或资源必须执行在同一个调用线程。





#  在 在 Web 应用中配置过滤器

@WebFilter 

■ filter-name: 用于映射过滤器到 servlet 或 URL
■ filter-class: 由容器用于表示过滤器类型
■ init-params: 过滤器的初始化参数



容器必须为部署描述符中定义的
每个过滤器声明实例化一个 Java 类实例。因此，如果开发人员对同一个过滤器类声明了两次，**则容器将实例化两个相同的过滤器类的实例**。

**定义**

```xml
<filter>
<filter-name>Image Filter</filter-name>
<filter-class>com.acme.ImageFilter</filter-class>
</filter>
```

**指定servlet过滤**

```xml
<filter-mapping>
<filter-name>Multipe Mappings Filter</filter-name>
<url-pattern>/foo/*</url-pattern>
<servlet-name>Servlet1</servlet-name>
<servlet-name>Servlet2</servlet-name>
<url-pattern>/bar/*</url-pattern>
</filter-mapping>
```

**servlet url**

```xml
<filter-mapping>
<filter-name>Logging Filter</filter-name>
<url-pattern>/*</url-pattern>
</filter-mapping>
```

1. 首先， <url-pattern>按照在部署描述符中的出现顺序匹配过滤器映射。
2. 接下来，<servlet-name>按照在部署描述符中的出现顺序匹配过滤器映射。

# 过滤器和 RequestDispatcher

Java Servlet 规范自从 2.4 新版本以来，能够在请求分派器 forward()和 include()调用情况下配置可被调用的
过滤器。
通过在部署描述符中使用新的<dispatcher>元素，开发人员可以为 filter-mapping 指定是否想要过滤器应用到
请求，当：
1. 请求直接来自客户端。
可以由一个带有 REQUEST 值的<dispatcher>元素，或者没有任何<dispatcher>元素来表示。
2.使用表示匹配<url-pattern> 或 <servlet-name>的 Web 组件的请求分派器的 forward()调用情况下处理请求。
可以由一个带有 FORWARD 值的<dispatcher>元素表示。
3.使用表示匹配<url-pattern> 或 <servlet-name>的 Web 组件的请求分派器的 include()调用情况下处理请求。
50
可以由一个带有 INCLUDE 值的<dispatcher>元素表示。
4. 使用第 106 页“错误处理”指定的错误页面机制处理匹配<url-pattern>的错误资源的请求。
可以由一个带有 ERROR 值的<dispatcher>元素表示。
5. 使用第 10 页指定的“异步处理”中的异步上下文分派机制对 web 组件使用 dispatch 调用处理请求。
可以由一个带有 ASYNC 值的<dispatcher>元素表示。
6. 或之上 1，2，3，4 或 5 的任何组合。

```xml
<filter-mapping>
<filter-name>Logging Filter</filter-name>
<url-pattern>/products/*</url-pattern>
</filter-mapping>
```

客户端以/products/...开始的请求将导致 Logging Filter 被调用，但不是在以路径/products/...开始的请求分派
器调用情况下。LoggingFilter 将在初始请求分派和恢复请求时被调用。如下代码：

```xml
<filter-mapping>
<filter-name>Logging Filter</filter-name>
<servlet-name>ProductServlet</servlet-name>
<dispatcher>INCLUDE</dispatcher>
</filter-mapping>
```



