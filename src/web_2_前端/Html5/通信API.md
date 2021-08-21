# 通信API

## 跨文档消息传输

1. 什么是跨文档消息传输?

   1. H5提供在网页文档之间互相接收与发送消息的功能
   2. 只要获取到网页所在窗口对象的实例,不仅同源(域+端口号) 的web网页之间可以互相通信
   3. 甚至可以跨域通信

2. 流程

   1. 从其他窗口接收消息

      `window.addEventListener("message",function(){...},false);`

   2. 使用window对象发送消息

      `window.postMessage(message,targetOrigin)`

      targetOrigin:为对象窗口的URL地址例如:http://localhost:8080

      

## webSocket通信

1. 使用方式

   1. 创建对象

      `var webSocket = new WebSocket("ws://localhost:8005")`

   2. 发送数据

      `websocket.send(String)`

   3. 主动关闭

      `websocket.close()`

2. 事件

   1. onmessage
   2. onopen
   3. onclose

3. readyState的状态

   1. CONNECTING(0):正在连接

   2. OPEN(1):已建立连接

   3. CLOSING(数值为2):正在关闭连接

   4. CLOSED(数值为3):已关闭连接

      



