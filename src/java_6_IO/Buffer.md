# Buffer

缓冲区，用于数据读写





## buffer的基本实现类

- ByteBuffer
- CharBuffer
- DoubleBuffer
- FloatBuffer
- IntBuffer
- LongBuffer
- ShortBuffer
- MappedByteBuffer



# 三大核心属性

**capacity**: 容量，缓冲区最大存储数据的容量。一旦声明不能改变

**limit**:  界限，缓冲区 可以 操作数据的 大小

**postion**: 位置，表示缓冲区 正在 操作数据 的位置

**mark**：标记上次的位置

**mark<=position** <= **limit** <=  **capacity**



## 基本操作

**put**：存入数据到缓冲区中

**get**：读取缓存区的数据

**flip**：缓冲区读写翻转

**rewind**:  **将position设回0,limit保持不变**

**clear**: position将被设回0，limit被设置成 capacity的值

**compact**:将所有未读的数据拷贝到Buffer起始处然后,将position设到最后一个未读元素正后面

**mark/reset**:标记一个position ，恢复 position

**equals**:buffer的类型相同，个数相同，每个byte相同



# 基本操作解析

## 当进行读写混合操作之后 position指针时如何 运转的

**问题描述**

*capacity* 是10 put 5个字符串、get 1个字符串、put1个字符串 切换到读模式 此时的 *position* 与limit是多少

**猜想**

读写模式切换时 

**position = limit+1**

**limit =  position**

**结论**

**读写翻转时：默认只继承上次模式可用的数据，并不会识别缓冲区本身的所有大小**

例如：10个字节的缓冲区：写了5个字节，切读模式，那么只有5个字节可用，

这五个字节 在切换到 写模式时，由于未读0个：则没有写入空间

## 缓冲区满了或空了之后发生什么情况

## 现象

**从满的缓冲区PUT**

会报 **BufferOverflowException**

**从空的缓冲区GET**

指针往前

**从满的缓存区GET**

会报 **BufferOverflowException**

## 结论

1. Buffer内部 不会 特定 区分读写模式
2. 只要 指针越过 *Limit* 就会 报 **BufferOverflowException**
3. 而 *GET* *PUT* 都会 把 指针往前移



## Buffer Flip

> 将 position设置成 0，limit设置成 *position*

1. *flip* 没有任何 涉及到读写相关的逻辑
2. 我们可以把 position ~ limit设置成 当前 工作窗口
3. 0~position的位置就是 当前操作的 产出段，翻转的就是 当前操作的产出
4. 而对于 写入 读取操作：写入的产出就是 一个个数据、读取的产出就是 为写入腾空间

```java
public final Buffer flip() {
    limit = position;
    position = 0;
    mark = -1;
    return this;
}
```

## Rewind

> 将*position* 设置成 0 ，mark重置

将当前操作的产出 重置为0

```java
public final Buffer rewind() {
    position = 0;
    mark = -1;
    return this;
}
```

## remaining

> 剩余多少可供消费的空间  limit - position

## clear

> 清空产出，

```
    public final Buffer clear() {
        position = 0;
        limit = capacity;
        mark = -1;
        return this;
    }
```



## mark/reset

```java
public final Buffer mark() {
        mark = position;
        return this;
}
public final Buffer reset() {
    int m = mark;
    if (m < 0)
        throw new InvalidMarkException();
    position = m;
    return this;
}
```

## position(int newPosition)

> 修改新的position

新的position不能大于  工作窗口

```java
public final Buffer position(int newPosition) {
    if ((newPosition > limit) || (newPosition < 0))
        throw new IllegalArgumentException();
    position = newPosition;
    if (mark > position) mark = -1;
    return this;
}
```



# 直接缓冲区与非直接缓冲区

## 解释

非直接缓冲区：*allocate* 分配缓冲区，将缓冲建立在JVM内存中

直接缓冲区：及那个 缓冲区建立在 操作系统的物理内存中

## 为什么出现直接缓冲区的需求

**传统方式 读取文件** 

需要经历  **物理磁盘 -> 内核缓存 -> JVM缓存 -> 应用程序空间**

**直接缓冲区**

需要经历  **物理磁盘 -> 直接缓冲区 -> 应用程序空间**



## 建立直接缓冲区的办法

1. **allocateDirect**
2. **FileChannel.map()**



## example

```java
//20.002、16.028、16.879、 一次系统调用
@Test
public void main() throws IOException {
    long l = System.currentTimeMillis();

    FileChannel file = FileChannel.open(Paths.get("D:\\", "myimage.tar"), StandardOpenOption.READ);
    //直接映射的缓冲区
    MappedByteBuffer map = file.map(FileChannel.MapMode.READ_ONLY, 0, file.size());

    //copy到C:
    FileChannel open = FileChannel.open(Paths.get("Z:\\", "myimage.tar"), StandardOpenOption.WRITE,StandardOpenOption.CREATE);
    open.write(map);
    System.out.println((System.currentTimeMillis() - l)/1000D);
}
// 482s、18.717
@Test
public void testNormal() throws IOException {
    long l = System.currentTimeMillis();
    FileChannel file = FileChannel.open(Paths.get("D:\\", "myimage.tar"), StandardOpenOption.READ);
    FileChannel open = FileChannel.open(Paths.get("Z:\\", "myimage.tar"), StandardOpenOption.WRITE,StandardOpenOption.CREATE);
    ByteBuffer allocate = ByteBuffer.allocate((int) file.size());
    while (file.read(allocate)>0) {
        allocate.flip();
        open.write(allocate);
        allocate.clear();
    }

    System.out.println((System.currentTimeMillis() - l)/1000D);
}
```

