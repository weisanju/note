# Channel

* 所有的 IO 在NIO 中都从一个Channel 开始
* Channel的实现：覆盖了TCP，UDP，文件IO
  * 文件通道：FileChannel
  * 数据包通道：DatagramChannel
  * 客户端socket：SocketChannel
  * 服务端socket：ServerSocketChannel

# Buffer

## buffer的基本实现类

- ByteBuffer
- CharBuffer
- DoubleBuffer
- FloatBuffer
- IntBuffer
- LongBuffer
- ShortBuffer
- MappedByteBuffer

## 基本操作

put

get

flip

rewind:将position设回0,limit保持不变

clear:position将被设回0，limit被设置成 capacity的值

compact:将所有未读的数据拷贝到Buffer起始处然后,将position设到最后一个未读元素正后面

mark/reset:标记一个position ，恢复 position

equals:buffer的类型相同，个数相同，每个byte相同



## NIO的buffer结构

capacity,position和limit

capacity容器

position

# Selector

Selector允许单线程处理多个 Channel