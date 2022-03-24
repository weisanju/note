# 会话

> 超文本传输协议（HTTP）被设计为一种无状态协议。为构建有效的 Web 应用，必须与来自一个特定的客户端的请求彼此是相互关联。随时间的推移，演变了许多会话跟踪机制，这些机制直接使用对程序员而言是困难或麻烦的。

该规范定义了一个简单的 HttpSession 接口，允许 servlet 容器使用几种方法来跟踪用户会话，而不会使应用开发人员陷入到这些方法的细节中。



# 会话跟踪机制

## Cookies

* 通过 HTTP cookie 的会话跟踪是最常用的会话跟踪机制，且所有 servlet 容器都应该支持

* 容器向客户端发送一个 cookie，客户端后续到服务器的请求都将返回该 cookie，明确地将请求与会话关联。会话跟踪 cookie 的标准名字必须是 JSESSIONID，所有 3.0 兼容的容器必须支持。容器也允许通过容器指定的配置自定义会话跟踪 cookie 的名字

    见 *SessionCookieConfig*

* 所有 servlet 容器必须提供能够配置容器是否标记会话跟踪 cookie 为 HttpOnly

* 如果 web 应用为其会话跟踪 cookie 配置了一个自定义的名字，则如果会话 id 编码到 URL 中那么相同的自定义名字也将用于 URI 参数的名字（假如 URL 重写已开启）。

## SSL 会话

安全套接字层，在 HTTPS 使用的加密技术，有一种内置机制允许多个来自客户端的请求被明确识别为同一会话。Servlet 容器可以很容易地使用该数据来定义会话。

# URL 重写

URL 重写是会话跟踪的最低标准。当客户端不接受 cookie 时，服务器可使用 URL 重写作为会话跟踪的基
础。URL 重写涉及添加数据、会话 ID、容器解析 URL 路径从而请求与会话相关联。
会话 ID 必须被编码为 URL 字符串中的一个路径参数。参数的名字必须是 jsessionid。下面是一个 URL 包
含编码的路径信息的例子：

```
http://www.myserver.com/catalog/index.html;jsessionid=1234
```

URL 重写在日志、书签、referer header、缓存的 HTML、URL 工具条中暴露会话标识。在支持 cookie 或 SSL
会话的情况下，不应该使用 URL 重写作为会话跟踪机制。

URL 重写在日志、书签、referer header、缓存的 HTML、URL 工具条中暴露会话标识。在支持 cookie 或 SSL
会话的情况下，不应该使用 URL 重写作为会话跟踪机制。



## 创建会话

如果以下之一是 true，会话被认为是“新”的：
■ 客户端还不知道会话
■ 客户端选择不加入会话。

直到客户端“加入”到 HTTP 会话之前它都被认为是新的

与每个会话相关联是一个包含唯一标识符的字符串，也被称为会话 ID。会话 ID 的值能通过调用
*javax.servlet.http.HttpSession.getId()* 获 取 ， 且 能 在 创 建 后 通 过 调 用
*javax.servlet.http.HttpServletRequest.changeSessionId()*改变



# 会话范围

HttpSession 对象必须被限定在应用（或 servlet 上下文）级别。底层的机制，如使用 cookie 建立会话，不同的上下文可以是相同，但所引用的对象，包括包括该对象中的属性，决不能在容器上下文之间共享。
用一个例子来说明该要求： 如果 servlet 使用 RequestDispatcher 来调用另一个 Web 应用的 servlet，任何创建的会话和被调用 servlet 所见的必须不同于来自调用会话所见的。
此外，一个上下文的会话在请求进入那个上下文时必须是可恢复的，不管是直接访问它们关联的上下文还
是在请求目标分派时创建的会话。

# 绑定 Session 属性

servlet 可以按名称绑定对象属性到 HttpSession 实现，任何绑定到会话的对象可用于任意其他的 Servlet，其
属于同一个 ServletContext 且处理属于相同会话中的请求。
一 些 对 象 可 能 需 要 在 它 们 被 放 进 会 话 或 从 会 话 中 移 除 时 得 到 通 知 。 这 些 信 息 可 以 从
HttpSessionBindingListener 接口实现的对象中获取。这个接口定义了以下方法，用于标识一个对象被绑定到
会话或从会话解除绑定时。
■ valueBound
■ valueUnbound
在对象对 HttpSession 接口的 getAttribute 方法可用之前 valueBound 方法必须被调用。在对象对 HttpSession
接口的 getAttribute 方法不可用之后 valueUnbound 方法必须被调用。

# 会话超时

在 HTTP 协议中，当客户端不再处于活动状态时没有显示的终止信号。这意味着当客户端不再处于活跃状
态时可以使用的唯一机制是超时时间。
Servlet 容器定义了默认的会话超时时间，且可以通过 HttpSession 接口的 getMaxInactiveInterval 方法获取。
开发人员可以使用 HttpSession 接口的 setMaxInactiveInterval 方法改变超时时间。这些方法的超时时间以秒
为单位。根据定义，如果超时时间设置为 0 或更小的值，会话将永不过期。会话不会生效，直到所有 servlet
使用的会话已经退出其 service 方法。一旦会话已失效,新的请求必须不能看到该会话。





# 最后访问时间

HttpSession 接口的 getLastAccessedTime 方法允许 servlet 确定在当前请求之前的会话的最后访问时间。当会
话中的请求是 servlet 容器第一个处理的时该会话被认为是访问了







