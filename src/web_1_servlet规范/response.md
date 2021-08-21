# 缓冲

*ServletResponse* 接口的如下方法允许 servlet 访问和设置缓冲信息：

```
getBufferSize 如果没有使用缓冲，该方法必须返回一个 int 值 0
setBufferSize 
isCommitted 是否有任何响应字节已经返回到客户端
reset 当响应没有提交时，reset 方法清空缓冲区的数据,。头信息，状态码也要被清空
resetBuffer 将清空缓冲区中的内容，但不清空请求头和状态码
如果响应已经提交并且 reset 或 resetBuffer 方法已被调用，则必须抛出 IllegalStateException，响应及它关联的缓冲区将保持不变
flushBuffer 当使用缓冲区时，容器必须立即刷出填满的缓冲区内容到客户端。如果这是最早发送到客户端的数据，且认为响应被提交了
```



# 非阻塞 IO

非阻塞 IO 仅对在 Servlet 和 Filter（“异步处理”）中的异步请求处理和升级处理
（“升级处理”）有效。否则，当调用 ServletInputStream.setReadListener 或
ServletOutputStream.setWriteListener 方法时将抛出 IllegalStateException。



*WriteListener*

*ServletOutputStream*

```
boolean isReady(). 如果往 ServletOutputStream 写会成功，则该方法返回 true，其他情况会返回 false。
如果该方法返回 true，可以在 ServletOutputStream 上执行写操作。如果没有后续的数据能写到
ServletOutputStream，那么直到底层的数据被刷出之前该方法将一直返回 false。且在此时容器将调用
WriteListener 的 onWritePossible 方法。随后调用该方法将返回 true。
■ void setWriteListener(WriteListener listener). 关联 WriteListener 和当且的 ServletOutputStream，当
ServletOutputStream 可以写入数据时容器会调用 WriteListener 的回调方法。注册了 WriteListener 将开
始非阻塞 IO。此时再切换到传统的阻塞 IO 是非法的。
```



# 简便方法

HttpServletResponse 提供了如下简便方法：
■ sendRedirect
■ sendError

sendRedirect 方法将设置适当的 header 和内容体,将客户端重定向到另一个地址。使用相对 URL 路径调用该方法是合法的，但是底层的容器必须将传回到客户端的相对地址转换为全路径 URL。无论出于什么原因，如 果 给 定 的 URL 是 不 完 整 的 ， 且 不 能 转 换 为 一 个 有 效 的 URL ， 那 么 该 方 法 必 须 抛 出IllegalArgumentException。
sendError 方法将设置适当的 header 和内容体用于返回给客户端返回错误消息。可以 sendError 方法提供一个可选的 String 参数用于指定错误的内容体。
如果响应已经提交并终止，这两个方法将对提交的响应产生负作用。

**这两个方法调用后 servlet 将不会产生到客户端的后续的输出**。这两个方法调用后如果有数据继续写到响应，这些数据被忽略。 如果数据已经写到响应的缓冲区，但没有返回到客户端（例如，响应没有提交），**则响应缓冲区中的数据必须被清空并使用这两个方法设置的数据替换。**

如果响应已提交，这两个方法必须抛出 IllegalStateException。

