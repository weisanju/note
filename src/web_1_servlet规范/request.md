# Http协议参数

**POST参数可用时**

1。该请求是一个 HTTP或 HTTPS请求。
2。HTTP方法是 POST。
3。内容类型是 application/x-www-form-urlencoded
4。该 servlet已经对 request 对象的任意 getParameter方法进行了初始调用



**容器从 URI查询字符串和 POST 数据中填充参数**。

**参数以一系列的名-值对的形式保存**。

**任何给定的参数的名称可存在多个参数值**。
**ServletRequest 接口**的下列方法可访问这些参数：

* getParameter
* getParameterNames
* getParameterValues
* getParameterMap

# 文件上传

> 当数据以 multipart/form-data的格式发送时，servlet 容器支持文件上传

**坑点1**

* 会有两种输入流 *FileInputStream* *ByteArrayInputStream*

* *InputStream* 每调用一次都会 包装一遍,导致*ByteArrayInputStream*维护的变量失效

    *DiskFileItem.getInputStream*

    ```
    {
            if (!isInMemory()) {
                return new FileInputStream(dfos.getFile());
            }
    
            if (cachedContent == null) {
                cachedContent = dfos.getData();
            }
            return new ByteArrayInputStream(cachedContent);
        }
    ```

    

**注解 *MultipartConfig***

* 使得servlet支持 文件上传
* *location* 指定临时文件位置
* *maxFileSize* 指定文件最大大小
* *maxRequestSize* 指定请求头最大大小
* *fileSizeThreshold* 指定文件 多少被写进磁盘



# 属性

■ getAttribute
■ getAttributeNames
■ setAttribute

只有一个属性值可与一个属性名称相关联。以前缀 java.和 javax.开头的属性名称是本规范的保留定义。
同样地，以前缀 sun.和 com.sun.，oracle 和 com.oracle 开头的属性名是 Oracle Corporation 的保留定
义。建议属性集中所有属性的命名与 Java 编程语言的规范 1 为包命名建议的反向域名约定一致





# 头

■ getHeader
■ getHeaders
■ getHeaderNames





# 请求路径

**Context Path ** 项目路径

**Servlet Path** *servletpath*

**PathInfo**

*requestURI = contextPath + servletPath + pathInfo*

```
/catalog/lawn/index.html 
	ContextPath: /catalog
	ServletPath: /lawn
	PathInfo: /index.html
/catalog/garden/implements/ 
	ContextPath: /catalog
	ServletPath: /garden
	PathInfo: /implements/
/catalog/help/feedback.jsp 
	ContextPath: /catalog
	ServletPath: /help/feedback.jsp
	PathInfo: null
```



**路径转换方法**
在 API中有两个方便的方法，允许开发者获得与某个特定的路径等价的文件系统路径。这些方法是：

* ServletContext.getRealPath

* HttpServletRequest.getPathTranslated
    getRealPath 方法需要一个字符串参数，并返回一个字符串形式的路径，这个路径对应一个在本地文件系统
    上的文件。getPathTranslated 方法推断出请求的 pathInfo 的实际路径（译者注：把 URL 中 servlet 名称之后，
    查询字符串之前的路径信息转化成实际的路径）。
    这些方法在 servlet 容器无法确定一个有效的文件路径 的情况下，如 Web 应用程序从归档中，在不能访问
    本地的远程文件系统上，或在一个数据库中执行时，这些方法必须返回null。JAR文件中META-INF/resources
    目录下的资源，只有当调用 getRealPath()方法时才认为容器已经从包含它的 JAR 文件中解压，在这种情况
    下，必须返回解压缩后位置

# 非阻塞IO

> 非阻塞 IO 仅对在 Servlet 和 Filter（“异步处理”）中的异步请求处理和升级处理
> （“升级处理”）有效

*ReadListener*

*onDataAvailable*

*onAllDataRead*

*onError*





# Cookie

HttpOnly cookie 暗示客户端它们不会暴
露给客户端脚本代码





# SSL属性

**协议属性**

| 属性         | 属性名称                             | Java 类型 |
| ------------ | ------------------------------------ | --------- |
| 密码套件     | javax.servlet.request.cipher_suite   | String    |
| 算法的位大小 | javax.servlet.request.key_size       | Integer   |
| SSL 会话 id  | javax.servlet.request.ssl_session_id | String    |

如果有一个与请求相关的 SSL 证书，它必须由 servlet 容器以 java.security.cert.X509Certificate 类型的对象数组暴露给 servlet 程序员并可通过一个 javax.servlet.request.X509Certificate 类型的 ServletRequest 属性访问。
这个数组的顺序是按照信任的升序顺序。证书链中的第一个证书是由客户端设置的，第二个是用来验证第
一个的，等等。





# 国际化

getLocale
getLocales

 getLocale 方法将返回客户端要接受内容的首选语言环境。要了解更多关于 Accept-Language 头必须被解
释为确定客户端首选语言的信息，请参阅 RFC 2616（HTTP/1.1）14.4 节。
getLocales 方法将返回一个 Locale 对象的枚举，从首选语言环境开始顺序递减，这些语言环境是可被客户
接受的语言环境。
如果客户端没有指定首选语言环境，getLocale方法返回的语言环境必须是 servlet容器默认的语言环境，而
getLocales方法必须返回只包含一个默认语言环境的 Local 元素的枚举。







# 请求数据编码

目前，许多浏览器不随着 Content-Type 头一起发送字符编码限定符，而是根据读取 HTTP 请求确定字符编码。如果客户端请求没有指定请求默认的字符编码，容器用来创建请求读取器和解析 POST 数据的编码必须是“ISO-8859-1”。然而，为了向开发人员说明客户端没有指定请求默认的字符编码，在这种情况下，客户端发送字符编码失败，容器从 getCharacterEncoding方法返回 null。
如果客户端没有设置字符编码，并使用不同的编码来编码请求数据，而不是使用上面描述的默认的字符编
码 ， 那 么 可 能 会 发 生 破 坏 。 为 了 弥 补 这 种 情 况 ， ServletRequest 接 口 添 加 了 一 个 新 的 方 法
*setCharacterEncoding(String enc)*。

开发人员可以通过调用此方法来覆盖由容器提供的字符编码。**必须在解析任何 post 数据或从请求读取任何输入之前调用此方法**。

此方法一旦调用，将不会影响已经读取的数据的编码。

