# 什么是跨域资源共享(CORS)

* 跨域资源共享,实际上叫跨源 资源共享(Cross-Origin-Resource-share),是一种 用来控制跨域访问的机制

* 它使用额外的Http头来告诉浏览器,让运行在一个源(origin)上的web应用准许访问来自不同源的服务器上指定的资源
* 源的定义为: 域名:协议:端口.这三者一致则认为同源,当一个资源试图访问 不同源的资源时源会发起一个**跨域 HTTP 请求**。

* 出于安全原因，浏览器限制从脚本内发起的跨源HTTP请求

  * 例如，XMLHttpRequest和Fetch API遵循同源策略
  *  这意味着使用这些API的Web应用程序只能从加载应用程序的同一个域请求HTTP资源，除非响应报文包含了正确CORS响应头。

  >  并不一定是浏览器限制了发起跨站请求，也可能是跨站请求可以正常发起，但是返回结果被浏览器拦截了。



# 功能概述

* 跨域资源共享标准新增了一组HTTP首部字段,允许服务器申明哪些源 通过浏览器 有权限访问哪些资源
* 另外,规范要求,对那些可能对服务器数据产生副作用的HTTP请求方法,浏览器必须首先使用 [`OPTIONS`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Methods/OPTIONS) 方法发起一个预检请求（preflight request），从而获知服务端是否允许该跨域请求。
  * 特别是 [`GET`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Methods/GET) 以外的 HTTP 请求
  * 或者搭配某些 MIME 类型的 [`POST`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Methods/POST) 请求
* 服务器确认允许之后，才发起实际的 HTTP 请求。在预检请求的返回中，服务器端也可以通知客户端，是否需要携带身份凭证（包括 [Cookies ](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Cookies)和 HTTP 认证相关数据）。
* CORS请求失败会产生错误，但是为了安全，在JavaScript代码层面是无法获知到底具体是哪里出了问题。你只能查看浏览器的控制台以得知具体是哪里出现了错误。



# 访问控制场景

这里，我们使用三个场景来解释跨域资源共享机制的工作原理。这些例子都使用 [`XMLHttpRequest`](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest) 对象。

[JavaScript 代码片段](http://arunranga.com/examples/access-control/)

关于服务端对跨域资源共享的支持的讨论，请参见这篇文章： [Server-Side_Access_Control (CORS)](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Server-Side_Access_Control)。

## 简单请求

某些跨域请求不会触发 [CORS 预检请求](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Access_control_CORS#Preflighted_requests).本文称这样的请求为“简单请求”，请注意，该术语并不属于 [Fetch](https://fetch.spec.whatwg.org/) （其中定义了 CORS）规范。若请求满足所有下述条件，则该请求可视为“简单请求”：

* 使用下列方法之一：
  * [`GET`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Methods/GET)
  * [`HEAD`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Methods/HEAD)
  * [`POST`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Methods/POST)

* 除了被浏览器自动设置的首部字段（例如 [`Connection`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Headers/Connection) ，[`User-Agent`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Headers/User-Agent)）和在 Fetch 规范中定义为 [禁用首部名称](https://fetch.spec.whatwg.org/#forbidden-header-name) 的其他首部，允许人为设置的字段为 Fetch 规范定义的 [对 CORS 安全的首部字段集合](https://fetch.spec.whatwg.org/#cors-safelisted-request-header)。该集合为：
  - [`Accept`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Headers/Accept)
  - [`Accept-Language`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Headers/Accept-Language)
  - [`Content-Language`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Headers/Content-Language)
  - [`Content-Type`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Headers/Content-Type) （需要注意额外的限制）
  - `DPR`
  - `Downlink`
  - `Save-Data`
  - `Viewport-Width`
  - `Width`
* [`Content-Type`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Headers/Content-Type) 的值仅限于下列三者之一：
  - `text/plain`
  - `multipart/form-data`
  - `application/x-www-form-urlencoded`
* 请求中的任意[`XMLHttpRequestUpload`](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequestUpload) 对象均没有注册任何事件监听器；[`XMLHttpRequestUpload`](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequestUpload) 对象可以使用 [`XMLHttpRequest.upload`](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest/upload) 属性访问。

* 请求中没有使用 [`ReadableStream`](https://developer.mozilla.org/zh-CN/docs/Web/API/ReadableStream) 对象。

**一次跨域的 `简单请求`示例**

```shell
GET /resources/public-data/ HTTP/1.1
Host: bar.other
User-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.5; en-US; rv:1.9.1b3pre) Gecko/20081130 Minefield/3.1b3pre
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language: en-us,en;q=0.5
Accept-Encoding: gzip,deflate
Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7
Connection: keep-alive
Referer: http://foo.example/examples/access-control/simpleXSInvocation.html
Origin: http://foo.example


HTTP/1.1 200 OK
Date: Mon, 01 Dec 2008 00:23:53 GMT
Server: Apache/2.0.61 
Access-Control-Allow-Origin: *
Keep-Alive: timeout=2, max=100
Connection: Keep-Alive
Transfer-Encoding: chunked
Content-Type: application/xml

[XML Data]
响应中携带了响应首部字段 Access-Control-Allow-Origin（第 16 行）。使用 Origin 和 Access-Control-Allow-Origin 就能完成最简单的访问控制。
本例中，服务端返回的 Access-Control-Allow-Origin: * 表明，该资源可以被任意外域访问。如果服务端仅允许来自 http://foo.example 的访问，该首部字段的内容如下：

Access-Control-Allow-Origin: http://foo.example

现在，除了 http://foo.example，其它外域均不能访问该资源
```



## 预检请求

​	与前述简单请求不同，“需预检的请求”要求必须首先使用 [`OPTIONS`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Methods/OPTIONS)  方法发起一个预检请求到服务器,以获知服务器是否允许该实际请求

​	"预检请求“的使用，可以避免跨域请求对服务器的用户数据产生未预期的影响。



### 示例

**如下是一个需要执行预检请求的 HTTP 请求：**

```js
// 上面的代码使用 POST 请求发送一个 XML 文档,该请求包含了一个自定义的请求首部字段（X-PINGOTHER: pingpong）,另外，该请求的 Content-Type 为 application/xml。因此，该请求需要首先发起“预检请求”。
var invocation = new XMLHttpRequest();
var url = 'http://bar.other/resources/post-here/';
var body = '<?xml version="1.0"?><person><name>Arun</name></person>';
    
function callOtherDomain(){
  if(invocation)
    {
      invocation.open('POST', url, true);
      invocation.setRequestHeader('X-PINGOTHER', 'pingpong');
      invocation.setRequestHeader('Content-Type', 'application/xml');
      invocation.onreadystatechange = handler;
      invocation.send(body); 
    }
}

......
```

**预检报文**

```html
OPTIONS /resources/post-here/ HTTP/1.1
Host: bar.other
User-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.5; en-US; rv:1.9.1b3pre) Gecko/20081130 Minefield/3.1b3pre
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language: en-us,en;q=0.5
Accept-Encoding: gzip,deflate
Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7
Connection: keep-alive
Origin: http://foo.example
Access-Control-Request-Method: POST
Access-Control-Request-Headers: X-PINGOTHER, Content-Type


HTTP/1.1 200 OK
Date: Mon, 01 Dec 2008 01:15:39 GMT
Server: Apache/2.0.61 (Unix)
Access-Control-Allow-Origin: http://foo.example
Access-Control-Allow-Methods: POST, GET, OPTIONS
Access-Control-Allow-Headers: X-PINGOTHER, Content-Type
Access-Control-Max-Age: 86400
Vary: Accept-Encoding, Origin
Content-Encoding: gzip
Content-Length: 0
Keep-Alive: timeout=2, max=100
Connection: Keep-Alive
Content-Type: text/plain
```

**预检完成后发送实际请求**

```
POST /resources/post-here/ HTTP/1.1
Host: bar.other
User-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.5; en-US; rv:1.9.1b3pre) Gecko/20081130 Minefield/3.1b3pre
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language: en-us,en;q=0.5
Accept-Encoding: gzip,deflate
Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7
Connection: keep-alive
X-PINGOTHER: pingpong
Content-Type: text/xml; charset=UTF-8
Referer: http://foo.example/examples/preflightInvocation.html
Content-Length: 55
Origin: http://foo.example
Pragma: no-cache
Cache-Control: no-cache

<?xml version="1.0"?><person><name>Arun</name></person>


HTTP/1.1 200 OK
Date: Mon, 01 Dec 2008 01:15:40 GMT
Server: Apache/2.0.61 (Unix)
Access-Control-Allow-Origin: http://foo.example
Vary: Accept-Encoding, Origin
Content-Encoding: gzip
Content-Length: 235
Keep-Alive: timeout=2, max=99
Connection: Keep-Alive
Content-Type: text/plain

[Some GZIP'd payload]
```

### 预检请求携带的字段

​	浏览器检测到，从 JavaScript 中发起的请求需要被预检。

​	从上面的报文中，我们看到，第 1~12 行发送了一个使用 `OPTIONS 方法的“`预检请求`”。` 

​	OPTIONS 是 HTTP/1.1 协议中定义的方法，用以从服务器获取更多信息。该方法不会对服务器资源产生影响。 预检请求中同时携带了下面两个首部字段：

```
//告知服务器 实际请求的方法
Access-Control-Request-Method: POST
//告知服务器器:将携带两个自定义首部字段
Access-Control-Request-Headers: X-PINGOTHER, Content-Type

服务器据此决定，该实际请求是否被允许。
```

### 预检请求响应的字段

```
#表明服务器允许客户端的来源
Access-Control-Allow-Origin: http://foo.example
#表明服务器允许客户端使用的方法,该字段与 HTTP/1.1 Allow: response header 类似,但仅限于在需要访问控制的场景中使用。
Access-Control-Allow-Methods: POST, GET, OPTIONS
#表明服务器允许请求中携带字段
Access-Control-Allow-Headers: X-PINGOTHER, Content-Type
#表明该响应的有效时间为 86400 秒,也就是 24 小时。在有效时间内，浏览器无须为同一请求再次发起预检请求。请注意，浏览器自身维护了一个最大有效时间，如果该首部字段的值超过了最大有效时间，将不会生效。
Access-Control-Max-Age: 86400
```

### 预检请求与重定向

大多数浏览器不支持针对于预检请求的重定向。如果一个预检请求发生了重定向，浏览器将报告错误：

> The request was redirected to 'https://example.com/foo', which is disallowed for cross-origin requests that require preflight

> Request requires preflight, which is disallowed to follow cross-origin redirect

CORS 最初要求该行为，不过[在后续的修订中废弃了这一要求](https://github.com/whatwg/fetch/commit/0d9a4db8bc02251cc9e391543bb3c1322fb882f2)。

在浏览器的实现跟上规范之前，有两种方式规避上述报错行为：

- 在服务端去掉对预检请求的重定向；
- 将实际请求变成一个简单请求。

如果上面两种方式难以做到，我们仍有其他办法：

* 发出一个简单请求（使用  [Response.url](https://developer.mozilla.org/en-US/docs/Web/API/Response/url) 或 [XHR.responseURL](https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest/responseURL)）以判断真正的预检请求会返回什么地址。
* 发出另一个请求（真正的请求），使用在上一步通过[Response.url](https://developer.mozilla.org/en-US/docs/Web/API/Response/url) 或 [XMLHttpRequest.responseURL](https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest/responseURL)获得的URL。

不过，如果请求是由于存在 Authorization 字段而引发了预检请求，则这一方法将无法使用。这种情况只能由服务端进行更改。



### 附带身份凭证的请求

[`XMLHttpRequest`](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest) 或 [Fetch](https://developer.mozilla.org/en-US/docs/Web/API/Fetch_API) 与 CORS 的一个有趣的特性是，可以基于  [HTTP cookies](https://developer.mozilla.org/en-US/docs/Web/HTTP/Cookies) 和 HTTP 认证信息发送身份凭证,一般而言，对于跨域 [`XMLHttpRequest`](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest) 或 [Fetch](https://developer.mozilla.org/en-US/docs/Web/API/Fetch_API) 请求，浏览器**不会**发送身份凭证信息。如果要发送凭证信息，需要设置 `XMLHttpRequest `的某个特殊标志位。



```
//本例中，http://foo.example 的某脚本向 http://bar.other 发起一个GET 请求，并设置 Cookies：
var invocation = new XMLHttpRequest();
var url = 'http://bar.other/resources/credentialed-content/';
    
function callOtherDomain(){
  if(invocation) {
    invocation.open('GET', url, true);
    invocation.withCredentials = true;
    invocation.onreadystatechange = handler;
    invocation.send(); 
  }
}
但是，如果服务器端的响应中未携带 Access-Control-Allow-Credentials: true ，浏览器将不会把响应内容返回给请求的发送者。

GET /resources/access-control-with-credentials/ HTTP/1.1
Host: bar.other
User-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.5; en-US; rv:1.9.1b3pre) Gecko/20081130 Minefield/3.1b3pre
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language: en-us,en;q=0.5
Accept-Encoding: gzip,deflate
Connection: keep-alive
Referer: http://foo.example/examples/credential.html
Origin: http://foo.example
Cookie: pageAccess=2


HTTP/1.1 200 OK
Date: Mon, 01 Dec 2008 01:34:52 GMT
Server: Apache/2
Access-Control-Allow-Origin: http://foo.example
Access-Control-Allow-Credentials: true
Cache-Control: no-cache
Pragma: no-cache
Set-Cookie: pageAccess=3; expires=Wed, 31-Dec-2008 01:34:53 GMT
Vary: Accept-Encoding, Origin
Content-Encoding: gzip
Content-Length: 106
Keep-Alive: timeout=2, max=100
Connection: Keep-Alive
Content-Type: text/plain


[text/plain payload]
```



**附带身份凭证的请求与通配符**

对于附带身份凭证的请求，服务器不得设置 `Access-Control-Allow-Origin` 的值为“`*`”。

这是因为请求的首部中携带了 `Cookie` 信息，如果 `Access-Control-Allow-Origin` 的值为“`*`”，请求将会失败。而将 `Access-Control-Allow-Origin` 的值设置为 `http://foo.example`，则请求将成功执行。

另外，响应首部中也携带了 Set-Cookie 字段，尝试对 Cookie 进行修改。如果操作失败，将会抛出异常。

## 第三方 cookies

注意在 CORS 响应中设置的 cookies 适用一般性第三方 cookie 策略。在上面的例子中，页面是在 `foo.example` 加载，但是第 20 行的 cookie 是被 `bar.other` 发送的，如果用户设置其浏览器拒绝所有第三方 cookies，那么将不会被保存。



## 访问控制的请求首部字段

## 访问控制的响应首部字段





# [SpringMVC实现跨域请求](https://www.jianshu.com/p/9203e9b14465)

## Servlet Filter 实现 CORS

## @CrossOrigin 注解实现 CORS 跨域

## 全局 CORS 配置

基于 JavaConfig

基于 XML 配置文件

