# NIO

1. JavaNIO（NewIO，或者Non-Block IO）是从 java1.4版本引入的 新的IOAPI 可以替代标准JavaIOAPI的
2. NIO 与原来的IO有同样的作用和目的，但是使用方式完全不同
3. NIO 是 面向缓冲区、基于Channel 的IO操作
4. NIO 以更高效的方式进行 流的读写操作







# 核心对象

## Buffer

缓冲区，用于数据读写

## Channel

用于缓冲区数据的传递

## Selector

用于 单线程管理 多个 **Channel** 连接



# 各种IO模型

| BIO          | NIO                 | AIO                             |
| ------------ | ------------------- | ------------------------------- |
| Socket       | SocketChannel       | AsynchronousSocketChannel       |
| ServerSocket | ServerSocketChannel | AsynchronousServerSocketChannel |



# JavaAIO(NIO2.0)

1. 真正的 异步非阻塞

2. 服务器实现模式为一个有效请求一个线程
3. 客户端的IO请求 都是由 os完成 在通知服务器应用 去启动先线程 处理



java.nio.channels 包下增加了 下面四个异步通道

```
AsynchronousSocketChannel
AsynchronousSerrverSocketChannel
AsynchronousFileChannel
AsynchronousDatagramChannel
```

