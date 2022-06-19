# 前言

用于 源节点 与目标节点 的连接

在JAVA NIO 中负责 缓冲区 中数据的传输

Channel本身不存储数据、需要配合缓冲区进行传输









# Channel实现类

Channel的实现：覆盖了TCP，UDP，文件IO

* 文件通道：FileChannel
* 数据包通道：DatagramChannel
* 客户端socket：SocketChannel
* 服务端socket：ServerSocketChannel





# 获取通道

## 针对通道的类提供了 *getChannel()* 方法

### 本地IO

**FileInputSream/FileOutputStream**/**RandomAccessFile**

## 网络 IO

**Socket/ServerSocket/DatagramSocket**

## jdk1.7 NIO2的改进

1. 针对各个通道提供了 静态 *open* 方法
2. Files工具类 有一个 **newByteChannel()**



# 通道之间的数据传输

## transferTo

> 将此通道在中的数据 传输到 目标通道 中

```java
public abstract long transferTo(long position, long count,
                                WritableByteChannel target)
```

> 将目标通道的数据 写入到此通道中

```java
public abstract long transferFrom(ReadableByteChannel src,
                                  long position, long count)
```

# 分散（Scatter）与聚集（Gather）

## 分散读取

分散读取（Scattering Reads）是指将缓冲区的数据 按顺序读取 到各个 缓冲区去



## 聚集写入

**gathering writer**  是指将各个 缓冲区的数据  按顺序写入到 Chaanel中

















