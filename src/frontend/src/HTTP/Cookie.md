# 什么是Cookie

​	HTTP Cookie（也叫Web Cookie或浏览器Cookie）是服务器发送到用户浏览器并保存在本地的一小块数据,它会在浏览器下次向同一服务器再发起请求时被携带并发送到服务器上

​	通常，它用于告知服务端两个请求是否来自同一浏览器，如保持用户的登录状态,Cookie使基于[无状态](https://developer.mozilla.org/en-US/docs/Web/HTTP/Overview#HTTP_is_stateless_but_not_sessionless)的HTTP协议记录稳定的状态信息成为了可能。

# cookie的使用场景

- 会话状态管理（如用户登录状态、购物车、游戏分数或其它需要记录的信息）
- 个性化设置（如用户自定义设置、主题等）
- 浏览器行为跟踪（如跟踪分析用户行为等）



> Cookie曾一度用于客户端数据的存储，因当时并没有其它合适的存储办法而作为唯一的存储手段，但现在随着现代浏览器开始支持各种各样的存储方式，Cookie渐渐被淘汰。**由于服务器指定Cookie后，浏览器的每次请求都会携带Cookie数据**，会带来额外的性能开销



# 创建Cookie

当服务器收到HTTP请求时，服务器可以在**响应头**里面添加一个[`Set-Cookie`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Headers/Set-Cookie)选项。浏览器收到响应后通常会保存下Cookie，之后对该服务器每一次请求中都通过[`Cookie`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Headers/Cookie)请求头部将Cookie信息发送给服务器。另外，Cookie的过期时间、域、路径、有效期、适用站点都可以根据需要来指定。

```
Set-Cookie: <cookie名>=<cookie值>
```

```html
HTTP/1.0 200 OK
Content-type: text/html
Set-Cookie: yummy_cookie=choco
Set-Cookie: tasty_cookie=strawberry

[页面内容]
```

现在，对该服务器发起的每一次新请求，浏览器都会将之前保存的Cookie信息通过[`Cookie`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Headers/Cookie)请求头部再发送给服务器。

```html
GET /sample_page.html HTTP/1.1
Host: www.example.org
Cookie: yummy_cookie=choco; tasty_cookie=strawberry
```



# Cookie种类

## 会话期Cookie

会话期Cookie是最简单的Cookie：浏览器关闭之后它会被自动删除,也就是说它仅在会话期内有效,会话期Cookie不需要指定过期时间（`Expires`）或者有效期（Max-Age）。需要注意的是，有些浏览器提供了会话恢复功能，这种情况下即使关闭了浏览器，会话期Cookie也会被保留下来，就好像浏览器从来没有关闭一样。

## 持久性Cookie

和关闭浏览器便失效的会话期Cookie不同，持久性Cookie可以指定一个特定的过期时间（`Expires`）或有效期（`Max-Age`）。

```html
Set-Cookie: id=a3fWa; Expires=Wed, 21 Oct 2015 07:28:00 GMT;
```

> 当Cookie的过期时间被设定时，设定的日期和时间只与客户端相关，而不是服务端。

​	

# Cookie的`Secure` 和`HttpOnly` 标记

​	标记为 `Secure` 的Cookie只应通过被HTTPS协议加密过的请求发送给服务端,但即便设置了 `Secure` 标记，敏感信息也不应该通过Cookie传输，因为Cookie有其固有的不安全性，`Secure `标记也无法提供确实的安全保障。从 Chrome 52 和 Firefox 52 开始，不安全的站点（`http:`）无法使用Cookie的 `Secure` 标记。

​	为避免跨域脚本 ([XSS](https://developer.mozilla.org/en-US/docs/Glossary/XSS)) 攻击，通过JavaScript的 [`Document.cookie`](https://developer.mozilla.org/zh-CN/docs/Web/API/Document/cookie) API无法访问带有 `HttpOnly` 标记的Cookie

它们只应该发送给服务端,如果包含服务端 Session 信息的 Cookie 不想被客户端 JavaScript 脚本调用，那么就应该为其设置 `HttpOnly` 标记。

```html
Set-Cookie: id=a3fWa; Expires=Wed, 21 Oct 2015 07:28:00 GMT; Secure; HttpOnly
```

# Cookie的作用域

* `Domain` 和 `Path` 标识定义了Cookie的*作用域：*即Cookie应该发送给哪些URL。
* `Domain` 标识指定了哪些主机可以接受Cookie。如果不指定，默认为[当前文档的主机](https://developer.mozilla.org/en-US/docs/Web/API/Document/location)（**不包含子域名**）。如果指定了`Domain`，则一般包含子域名。
* 例如，如果设置 `Domain=mozilla.org`，则Cookie也包含在子域名中（如`developer.mozilla.org`）。
* `Path` 标识指定了主机下的哪些路径可以接受Cookie（该URL路径必须存在于请求URL中）。以字符 `%x2F` ("/") 作为路径分隔符，子路径也会被匹配。
* 那么cookie的作用域：**cookie的作用域是domain本身以及domain下的所有子域名**

```
例如，设置 Path=/docs，则以下地址都会匹配：

/docs
/docs/Web/
/docs/Web/HTTP
```

# `SameSite` Cookie

允许服务器要求某个cookie在跨站请求时不会被发送，从而可以阻止跨站请求伪造攻击（[CSRF](https://developer.mozilla.org/zh-CN/docs/Glossary/CSRF)）。

```js
Set-Cookie: key=value; SameSite=Strict
```

SameSite可以有下面三种值：

* **None**

  浏览器会在同站请求、跨站请求下继续发送cookies，不区分大小写。

* **`Strict`**

  浏览器将只在访问相同站点时发送cookie。（在原有Cookies的限制条件上的加强，如上文“Cookie的作用域” 所述）

* Lax

  在新版本浏览器中，为默认选项，Same-site cookies 将会为一些跨站子请求保留，如图片加载或者frames的调用，但只有当用户从外部站点导航到URL时才会发送。如link链接

# JavaScript通过Document.cookie访问Cookie

通过[`Document.cookie`](https://developer.mozilla.org/zh-CN/docs/Web/API/Document/cookie)属性可创建新的Cookie，也可通过该属性访问非`HttpOnly`标记的Cookie。

# 安全

当机器处于不安全环境时，切记*不能*通过HTTP Cookie存储、传输敏感信息。

## 会话劫持和XSS

在Web应用中，Cookie常用来标记用户或授权会话。因此，如果Web应用的Cookie被窃取，可能导致授权用户的会话受到攻击。常用的窃取Cookie的方法有利用社会工程学攻击和利用应用程序漏洞进行[XSS](https://developer.mozilla.org/en-US/docs/Glossary/XSS)攻击。

## 跨站请求伪造（CSRF）

比如在不安全聊天室或论坛上的一张图片，它实际上是一个给你银行服务器发送提现的请求：

```html
<img src="http://bank.example.com/withdraw?account=bob&amount=1000000&for=mallory">
```

当你打开含有了这张图片的HTML页面时，如果你之前已经登录了你的银行帐号并且Cookie仍然有效（还没有其它验证步骤）,你银行里的钱很可能会被自动转走。有一些方法可以阻止此类事件的发生：

- 对用户输入进行过滤来阻止[XSS](https://developer.mozilla.org/en-US/docs/Glossary/XSS)；
- 任何敏感操作都需要确认；
- 用于敏感信息的Cookie只能拥有较短的生命周期；



# 第三方Cookie

* 每个Cookie都会有与之关联的域（Domain）,如果Cookie的域和页面的域相同，那么我们称这个Cookie为*第一方Cookie*（*first-party cookie*）
* 如果Cookie的域和页面的域不同，则称之为*第三方Cookie*（*third-party cookie*.）
* 第一方的Cookie也只会发送给设置它们的服务器。通过第三方组件发送的第三方Cookie主要用于广告和网络追踪

## 如何让浏览器发送第三方cookie

当前域名只能设置当前域名以及他的子域名

例如:  

```
zydya.com 域名设置的cookie 只能 发送给
blog.zyday.com	one.blog.zyday.com
自身及其子域名
```



# Set-Cookie

被用来由服务器端向客户端发送 cookie。

```
Set-Cookie: <cookie-name>=<cookie-value> 
Set-Cookie: <cookie-name>=<cookie-value>; Expires=<date>
Set-Cookie: <cookie-name>=<cookie-value>; Max-Age=<non-zero-digit>
Set-Cookie: <cookie-name>=<cookie-value>; Domain=<domain-value>
Set-Cookie: <cookie-name>=<cookie-value>; Path=<path-value>
Set-Cookie: <cookie-name>=<cookie-value>; Secure
Set-Cookie: <cookie-name>=<cookie-value>; HttpOnly

Set-Cookie: <cookie-name>=<cookie-value>; SameSite=Strict
Set-Cookie: <cookie-name>=<cookie-value>; SameSite=Lax

// Multiple directives are also possible, for example:
Set-Cookie: <cookie-name>=<cookie-value>; Domain=<domain-value>; Secure; HttpOnly
```

## `<cookie-name>=<cookie-value>`

一个 cookie 开始于一个名称/值对：

* `<cookie-name>` 可以是除了控制字符 (CTLs)、空格 (spaces) 或制表符 (tab)之外的任何 US-ASCII 字符。同时不能包含以下分隔字符： ( ) < > @ , ; : \ " / [ ] ? = { }.
* `<cookie-value>` 是可选的，如果存在的话，那么需要包含在双引号里面。支持除了控制字符（CTLs）、空格（whitespace）、双引号（double quotes）、逗号（comma）、分号（semicolon）以及反斜线（backslash）之外的任意 US-ASCII 字符。**关于编码**：许多应用会对 cookie 值按照URL编码（URL encoding）规则进行编码，但是按照 RFC 规范，这不是必须的。不过满足规范中对于 <cookie-value> 所允许使用的字符的要求是有用的。
* **`__Secure-` 前缀**：以 __Secure- 为前缀的 cookie（其中连接符是前缀的一部分），必须与 secure 属性一同设置，同时必须应用于安全页面（即使用 HTTPS 访问的页面）。
* **`__Host-` 前缀：** 以 __Host- 为前缀的 cookie，必须与 secure 属性一同设置，必须应用于安全页面（即使用 HTTPS 访问的页面），必须不能设置 domain 属性 （也就不会发送给子域），同时 path 属性的值必须为“/”。

## `Expires=<date> `

可选

cookie 的最长有效时间，形式为符合 HTTP-date 规范的时间戳。参考 [`Date`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Headers/Date) 可以获取详细信息。如果没有设置这个属性，那么表示这是一个**会话期 cookie** 。一个会话结束于客户端被关闭时，这意味着会话期 cookie 在彼时会被移除。然而，很多Web浏览器支持会话恢复功能，这个功能可以使浏览器保留所有的tab标签，然后在重新打开浏览器的时候将其还原。与此同时，cookie 也会恢复，就跟从来没有关闭浏览器一样。

## `Max-Age=<non-zero-digit>` 

可选

在 cookie 失效之前需要经过的秒数。秒数为 0 或 -1 将会使 cookie 直接过期。一些老的浏览器（ie6、ie7 和 ie8）不支持这个属性。对于其他浏览器来说，假如二者 （指 `Expires` 和`Max-Age`） 均存在，那么 Max-Age 优先级更高。

## `**Domain=<domain-value>**` 

可选

指定 cookie 可以送达的主机名。假如没有指定，那么默认值为当前文档访问地址中的主机部分（但是不包含子域名）。与之前的规范不同的是，域名之前的点号会被忽略。假如指定了域名，那么相当于各个子域名也包含在内了。

## **Secure** 

可选

一个带有安全属性的 cookie 只有在请求使用SSL和HTTPS协议的时候才会被发送到服务器。然而，保密或敏感信息永远不要在 HTTP cookie 中存储或传输，因为整个机制从本质上来说都是不安全的，比如前述协议并不意味着所有的信息都是经过加密的。

> 非安全站点（http:）已经不能再在 cookie 中设置 secure 指令了

## HttpOnly 

可选

设置了 HttpOnly 属性的 cookie 不能使用 JavaScript 经由  [`Document.cookie`](https://developer.mozilla.org/zh-CN/docs/Web/API/Document/cookie) 属性、[`XMLHttpRequest`](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest) 和  [`Request`](https://developer.mozilla.org/zh-CN/docs/Web/API/Request) APIs 进行访问，以防范跨站脚本攻击（[XSS](https://developer.mozilla.org/en-US/docs/Glossary/XSS)）。



# 示例

## 会话期 cookie

```html
Set-Cookie: sessionid=38afes7a8; HttpOnly; Path=/
```

## 持久化 cookie

```html
Set-Cookie: id=a3fWa; Expires=Wed, 21 Oct 2015 07:28:00 GMT; Secure; HttpOnly
```

## 非法域

属于特定域的 cookie，假如域名不能涵盖原始服务器的域名，那么[应该被用户代理拒绝](https://tools.ietf.org/html/rfc6265#section-4.1.2.3)。下面这个 cookie 假如是被域名为 originalcompany.com 的服务器设置的，那么将会遭到用户Http客户端的拒绝(Http连接报错)：

```html
Set-Cookie: qwerty=219ffwef9w0f; Domain=somecompany.co.uk; Path=/; Expires=Wed, 30 Aug 2019 00:00:00 GMT
```

### Cookie 前缀

* 名称中包含 __Secure- 或 __Host- 前缀的 cookie，只可以应用在使用了安全连接（HTTPS）的域中，需要同时设置 secure 指令。

* 假如 cookie 以 __Host- 为前缀，那么 path 属性的值必须为 "/" （表示整个站点）,且不能含有 domain 属性。
* 对于不支持 cookie 前缀的客户端，无法保证这些附加的条件成立，所以 cookie 总是被接受的。

```html
// 当响应来自于一个安全域（HTTPS）的时候，二者都可以被客户端接受
Set-Cookie: __Secure-ID=123; Secure; Domain=example.com
Set-Cookie: __Host-ID=123; Secure; Path=/

// 缺少 Secure 指令，会被拒绝
Set-Cookie: __Secure-id=1

// 缺少 Path=/ 指令，会被拒绝
Set-Cookie: __Host-id=1; Secure

// 由于设置了 domain 属性，会被拒绝
Set-Cookie: __Host-id=1; Secure; Path=/; domain=example.com
```





# 参考

[MDN-Cookie](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Cookies)

