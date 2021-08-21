# ServletContext作用范围

 ServletContext 是非分布式的且仅存在于一个 JVM 中

> 每个 JVM 的每个 Web 应用将有一个 ServletContext 实例



# 初始化参数

> 容器启动的初始化参数

*getInitParameter*
*getInitParameterNames*





# 配置方法

* 从 Servlet 3.0 开始,可以以编程方式定义 Servlet、Filter 和它们映射到的url 模式,

    **这些方法只能从 *ServletContextListener* 实现的 *contexInitialized* 方法或者**
    ***ServletContainerInitializer* 实现的 *onStartup* 方法进行的应用初始化过程中调用**。 

* 也可以查找关联到 Servlet 或 Filter 的一个 Registration 对象实例，或者到 Servlet 或 Filter 的所有 Registration对象的 map。
* 如果 ServletContext 传到了 ServletContextListener 的 contextInitialized 方法，但该 ServletContextListener 即没 有在 web.xml 或 web-fragment.xml 中声明也没有使用@WebListener 注解，则在 ServletContext 中定义的用于Servlet、Filter 和 Listener 的编程式配置的所有方法必须抛出 UnsupportedOperationException



## 编程式添加和配置 Servlet

```java
addServlet(String servletName, String className)
addServlet(String servletName, Servlet servlet)
addServlet(String servletName, Class <? extends Servlet> servletClass)
<T extends Servlet> T createServlet(Class<T> clazz)
ServletRegistration getServletRegistration(String servletName)
Map<String, ? extends ServletRegistration> getServletRegistrations()
```

## 编程式添加和配置 Filter

```java
addFilter(String filterName, String className)
addFilter(String filterName, Filter filter)
addFilter(String filterName, Class <? extends Filter> filterClass)
<T extends Filter> T createFilter(Class<T> clazz)
FilterRegistration getFilterRegistration(String filterName)
Map<String, ? extends FilterRegistration> getFilterRegistrations()
```

## 编程式添加和配置 Listener

**listener接口**

```java
javax.servlet.ServletContextAttributeListener
javax.servlet.ServletRequestListener
javax.servlet.ServletRequestAttributeListener
javax.servlet.http.HttpSessionListener
javax.servlet.http.HttpSessionAttributeListener
javax.servlet.http.HttpSessionIdListener
```

**配置API**

```
<T extends EventListener> void addListener(T t)
void addListener(String className)
<T extends EventListener> void addListener(T t)
void addListener(Class <? extends EventListener> listenerClass)
<T extends EventListener> void createListener(Class<T> clazz)
```

**编程式添加*servlet,filter,listener* 时注解请求处理**

```
@ServletSecurity、@RunAs、@DeclareRoles、@MultipartConfig。
这些注解 需要确保在手动 添加时,已经被处理到,
// 使用DI 去自动处理
```



# 上下文属性

setAttribute
getAttribute
getAttributeNames
removeAttribute

# 资源

ServletContext 接口提供了直接访问 Web 应用中静态内容层次结构的文件的方法，包括 HTML，GIF 和 JPEG
文件：
*getResource*
*getResourceAsStream*
getResource 和 getResourceAsStream 方法需要一个以“/”开头的 String 字符串作为参数，

* **给定的资源路径是相对于上下文的根**，

* 或者相对于 web 应用的 **WEB-INF/lib 目录下的 JAR 文件中的 META-INF/resources目录**。

    这两个方法首先根据请求的资源查找 web 应用上下文的根，然后查找所有 WEB-INF/lib 目录下的 JAR
    文件。**查找 WEB-INF/lib 目录中 JAR 文件的顺序是不确定的**。这种层次结构的文件可以存在于服务器的文
    件系统，Web 应用的归档文件，远程服务器，或在其他位置。

* **可以使用 getResourcePaths(String path)方法访问 Web 应用中的资源的完整列表。**

# 多主机和 Servlet 上下文

ServletContext 接口的 getVirtualServerName 方法允许访问 ServletContext 部署在的逻辑主机的配置名字。该
方法必须对所有部署在逻辑主机上的所有 servlet context 返回同一个名字。且该方法返回的名字必须是明确
的、每个逻辑主机稳定的、和适合用于关联服务器配置信息和逻辑主机





