## 简介

1. Node.js 是一个开源和跨平台的 JavaScript 运行时环境。 它几乎是任何类型项目的流行工具！

2. Node.js 在浏览器之外运行 V8 JavaScript 引擎（Google Chrome 的内核）。 这使得 Node.js 的性能非常好。

3. Node.js 应用程序在单个进程中运行，无需为每个请求创建新的线程

4. Node.js 在其标准库中提供了一组异步的 I/O 原语，以防止 JavaScript 代码阻塞

5. 通常，Node.js 中的库是使用非阻塞范式编写的，使得阻塞行为成为异常而不是常态。



当 Node.js 执行 I/O 操作时（比如从网络读取、访问数据库或文件系统），Node.js 将在响应返回时恢复操作（而不是阻塞线程和浪费 CPU 周期等待）。



这允许 Node.js 使用单个服务器处理数千个并发连接，而不会引入管理线程并发（这可能是错误的重要来源）的负担。

## Node.js 应用程序的示例

```js
JScopyconst http = require('http')
const hostname = '127.0.0.1'const port = 3000
const server = http.createServer((req, res) => {  res.statusCode = 200  res.setHeader('Content-Type', 'text/plain')
 res.end('Hello World\n')}
                                )
server.listen(port, hostname, () => {  console.log(`Server running at http://${hostname}:${port}/`)})
```

要运行此代码片段，则将其另存为 `server.js` 文件并在终端中运行 `node server.js`。

1. 此代码首先引入 Node.js [`http` 模块](http://nodejs.cn/api/http.html)。
2. `http` 的 `createServer()` 方法创建新的 HTTP 服务器并返回。
3. 服务器设置为监听指定的端口和主机名。 当服务器准备好时，则回调函数被调用，在此示例中会通知我们服务器正在运行。
4. 每当接收到新请求时，都会调用 [`request` 事件](http://nodejs.cn/api/http.html#http_event_request)，其提供两个对象：请求（[`http.IncomingMessage`](http://nodejs.cn/api/http.html#http_class_http_incomingmessage) 对象）和响应（[`http.ServerResponse`](http://nodejs.cn/api/http.html#http_class_http_serverresponse) 对象）。
5. 每当接收到新请求时，都会调用 [`request` 事件](http://nodejs.cn/api/http.html#http_event_request)，其提供两个对象：请求（[`http.IncomingMessage`](http://nodejs.cn/api/http.html#http_class_http_incomingmessage) 对象）和响应（[`http.ServerResponse`](http://nodejs.cn/api/http.html#http_class_http_serverresponse) 对象）。



## 安装

使用 NVM 动态切换 nodejs版本

[windows版本](https://github.com/coreybutler/nvm-windows/releases)



