# 简介

通过复用以前获取的资源,可提高性能,减轻服务端的压力



# 分类

缓存种类有很多,网关缓存、CDN、反向代理缓存和负载均衡器等部署在服务器上的缓存方式,大致上分两类

私有缓存:浏览器缓存

共享缓存:架设的web代理



# 缓存操作目标

只能缓存GET 请求,普遍的缓存案例

- 一个检索请求的成功响应: 对于 [`GET`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Methods/GET)请求，响应状态码为：[`200`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Status/200)，则表示为成功。一个包含例如HTML文档，图片，或者文件的响应。
- 永久重定向: 响应状态码：[`301`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Status/301)。
- 错误响应: 响应状态码：[`404`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Status/404) 的一个页面。
- 不完全的响应: 响应状态码 [`206`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Status/206)，只返回局部的信息。
- 除了 [`GET`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Methods/GET) 请求外，如果匹配到作为一个已被定义的cache键名的响应。

# 缓存控制

*Cache-control*取值

*no-store*:缓存中不得存储任何关于客户端请求和服务端响应的内容

*no-cache*:每次有请求发出时,会将带有本地缓存相关的验证字段发到服务器,服务器会验证缓存是否过期,若未过期(返回304),则缓存才使用本地缓存副本

私有与公共缓存

*private*|*public*:控制中间人是否能缓存 目标用户的页面

过期机制

```html
Cache-Control: max-age=31536000
```

* 表示资源能够被缓存的最大时间
* max-age是距离请求发起的时间的秒数
* 针对应用中那些不会改变的文件，通常可以手动设置一定的时长以保证缓存有效，例如图片、css、js等静态资源

*must-revalidate*

* 使用一个陈旧的资源时，必须先验证它的状态

# Pragma 头

* [`Pragma`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Headers/Pragma) 是**HTTP/1.0**标准中定义的一个header属性，

* 请求中包含Pragma的效果跟在头信息中定义Cache-Control: no-cache相同，但是HTTP的响应头没有明确定义这个属性，
* 所以它不能拿来完全替代HTTP/1.1中定义的Cache-control头

# 新鲜度

![HTTP时序图](C:\Users\weisanju\Desktop\实用图\HTTPStaleness.png)

# 计算缓存寿命

1. 首先 查 `Cache-control: max-age=N`的头

2. 然后 expires属性,比较Expires的值和头里面[Date](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Headers/Date)属性的值来判断是否缓存还有效
3. [Last-Modified](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Headers/Last-Modified)信息:缓存的寿命就等于头里面Date的值减去Last-Modified的值除以10（注：根据rfc2626其实也就是乘以10%）。

```
expirationTime = responseTime + freshnessLifetime - currentAge
```



# 改进资源

revving

* 给 不频繁更新的文件在URL后面（通常是文件名后面）会加上版本号
* 加上版本号后的资源就被视作一个完全新的独立的资源，同时拥有一年甚至更长的缓存过期时长
* 所有引用这个资源的地方都需要更新链接,通常会采用自动化构建工具在实际工作中完成这些琐碎的工作
* 加在加速文件后面的版本号不一定是一个正式的版本号字符串，如1.1.3这样或者其他固定自增的版本数。它可以是任何防止缓存碰撞的标记例如hash或者时间戳。

# 缓存验证

* 用户点击刷新按钮时会开始缓存验证
* 如果缓存的响应头信息里含有"Cache-control: must-revalidate”的定义
* 另外，在浏览器偏好设置里设置Advanced->Cache为强制验证缓存也能达到相同的效果。

*ETags*

* 强校验器,是服务器给 UA的 一个不透明的值, UA可以在后续的 *if-none-match* 头来验证缓存

 *last-modified*

* 弱校验器: 只能精确到秒,客户端后续可以在请求中带上 *if-modified-since* 来验证缓存
* 客户端向 服务端 发起校验时,服务端会返回 200,或者304 *not modified* ,304响应头也可以同时更新缓存文档的过期时间

# *Vary*响应

[`Vary`](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Headers/Vary) HTTP 响应头 决定了对于后续的请求头，如何判断是请求一个新的资源还是使用缓存的文件。

![HttpVary图](C:\Users\weisanju\Desktop\实用图\HTTPVary.png)



* vary头有利于内容服务的动态多样性
* 除了 请求的URI 不同 导致缓存失效以外,还有 区分移动端,桌面端缓存,区分不通编码类型的缓存,区别不通浏览器的缓存