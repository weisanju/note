# Selector（选择器）介绍

**Selector** 一般称 为**选择器** ，当然你也可以翻译为 **多路复用器** 。它是Java NIO核心组件中的一个，用于检查一个或多个NIO Channel（通道）的状态是否处于可读、可写。如此可以实现单线程管理多个channels,也就是可以管理多个网络链接。



**使用Selector的好处在于：** 使用更少的线程来就可以来处理通道了， 相比使用多个线程，避免了线程上下文切换带来的开销。







# Selector（选择器）的使用方法介绍

## 通过调用Selector.open()方法创建一个Selector对象

如下：

```java
Selector selector = Selector.open();
```

## 注册Channel到Selector

**Channel必须是非阻塞的**。

所以FileChannel不适用Selector，因为FileChannel不能切换为非阻塞模式，更准确的来说是因为FileChannel没有继承SelectableChannel。Socket channel可以正常使用。

**SelectableChannel抽象类** 有一个 **configureBlocking（）** 方法用于使通道处于阻塞模式或非阻塞模式。





```java
channel.configureBlocking(false);
SelectionKey key = channel.register(selector, Selectionkey.OP_READ);
```



# 对IO事件分类

**register()** 方法的第二个参数。这是一个“ **interest集合** ”，意思是在**通过Selector监听Channel时对什么事件感兴趣**。可以监听四种不同类型的事件：

- **Connect**
- **Accept**
- **Read**
- **Write**



通道触发了一个事件意思是该事件已经就绪。比如某个Channel成功连接到另一个服务器称为“ **连接就绪** ”。一个Server Socket Channel准备好接收新进入的连接称为“ **接收就绪** ”。一个有数据可读的通道可以说是“ **读就绪** ”。等待写数据的通道可以说是“ **写就绪** ”。

这四种事件用SelectionKey的四个常量来表示：

```java
SelectionKey.OP_CONNECT
SelectionKey.OP_ACCEPT
SelectionKey.OP_READ
SelectionKey.OP_WRITE
```



如果你对不止一种事件感兴趣，使用或运算符即可，如下：

```java
int interestSet = SelectionKey.OP_READ | SelectionKey.OP_WRITE;
```

# SelectionKey介绍

一个SelectionKey键表示了一个特定的通道对象和一个特定的选择器对象之间的注册关系。

```java
key.attachment(); //返回SelectionKey的attachment，attachment可以在注册channel的时候指定。
key.channel(); // 返回该SelectionKey对应的channel。
key.selector(); // 返回该SelectionKey对应的Selector。
key.interestOps(); //返回代表需要Selector监控的IO操作的bit mask
key.readyOps(); // 返回一个bit mask，代表在相应channel上可以进行的IO操作。
```

**key.interestOps():**



我们可以通过以下方法来判断Selector是否对Channel的某种事件感兴趣

```java
int interestSet = selectionKey.interestOps(); 
boolean isInterestedInAccept = (interestSet & SelectionKey.OP_ACCEPT) == SelectionKey.OP_ACCEPT；
boolean isInterestedInConnect = interestSet & SelectionKey.OP_CONNECT;
boolean isInterestedInRead = interestSet & SelectionKey.OP_READ;
boolean isInterestedInWrite = interestSet & SelectionKey.OP_WRITE;
```

**key.readyOps()**

ready 集合是通道已经准备就绪的操作的集合。JAVA中定义以下几个方法用来检查这些操作是否就绪.

```java
//创建ready集合的方法
int readySet = selectionKey.readyOps();
//检查这些操作是否就绪的方法
key.isAcceptable();//是否可读，是返回 true
boolean isWritable()：//是否可写，是返回 true
boolean isConnectable()：//是否可连接，是返回 true
boolean isAcceptable()：//是否可接收，是返回 true
```

```
Channel channel = key.channel();
Selector selector = key.selector();
key.attachment();
```

还可以在用register()方法向Selector注册Channel的时候附加对象。如：

```
SelectionKey key = channel.register(selector, SelectionKey.OP_READ, theObject);

```

# 从Selector中选择channel(Selecting Channels via a Selector)

**Selector维护的三种类型SelectionKey集合：**

- **已注册的键的集合(Registered key set)**

  所有与选择器关联的通道所生成的键的集合称为已经注册的键的集合。并不是所有注册过的键都仍然有效。这个集合通过 **keys()** 方法返回，并且可能是空的。这个已注册的键的集合不是可以直接修改的；试图这么做的话将引发java.lang.UnsupportedOperationException。

- **已选择的键的集合(Selected key set)**

  所有与选择器关联的通道所生成的键的集合称为已经注册的键的集合。并不是所有注册过的键都仍然有效。这个集合通过 **keys()** 方法返回，并且可能是空的。这个已注册的键的集合不是可以直接修改的；试图这么做的话将引发java.lang.UnsupportedOperationException。

- **已取消的键的集合(Cancelled key set)**

  已注册的键的集合的子集，这个集合包含了 **cancel()** 方法被调用过的键(这个键已经被无效化)，但它们还没有被注销。这个集合是选择器对象的私有成员，因而无法直接访问。

  **注意：**

  1. 当键被取消（ 可以通过**isValid( )** 方法来判断）时，它将被放在相关的选择器的已取消的键的集合里。

  2. 注册不会立即被取消，但键会立即失效。
  3. 当再次调用 **select( )** 方法时（或者一个正在进行的select()调用结束时），已取消的键的集合中的被取消的键将被清理掉，并且相应的注销也将完成。通道会被注销，而新的SelectionKey将被返回。
  4. 当通道关闭时，所有相关的键会自动取消（记住，一个通道可以被注册到多个选择器上）。
  5. 当选择器关闭时，所有被注册到该选择器的通道都将被注销，并且相关的键将立即被无效化（取消）。一旦键被无效化，调用它的与选择相关的方法就将抛出CancelledKeyException。

# **select()方法介绍：**

在刚初始化的Selector对象中，这三个集合都是空的。 **通过Selector的select（）方法可以选择已经准备就绪的通道** （这些通道包含你感兴趣的的事件）。比如你对读就绪的通道感兴趣，那么select（）方法就会返回读事件已经就绪的那些通道。下面是Selector几个重载的select()方法：

- int select()：阻塞到至少有一个通道在你注册的事件上就绪了。
- int select(long timeout)：和select()一样，但最长阻塞时间为timeout毫秒。
- int selectNow()：非阻塞，只要有通道就绪就立刻返回。

## 返回值解析

**select()方法返回的int值表示有多少通道已经就绪,**

1. 是自上次调用select()方法后有多少通道变成就绪状态

2. 之前在select（）调用时进入就绪的通道不会在本次调用中被记入，

3. 而在前一次select（）调用进入就绪但现在已经不再处于就绪的通道也不会被记入

**example**

例如：首次调用select()方法，如果有一个通道变成就绪状态，返回了1，若再次调用select()方法，如果另一个通道就绪了，它会再次返回1。如果对第一个就绪的channel没有做任何操作，现在就有两个就绪的通道，但在每次select()方法调用之间，只有一个通道就绪了。

**一旦调用select()方法，并且返回值不为0时，则 可以通过调用Selector的selectedKeys()方法来访问已选择键集合** 。如下：



```java
Set selectedKeys = selector.selectedKeys();
Iterator keyIterator = selectedKeys.iterator();
while(keyIterator.hasNext()) {
    SelectionKey key = keyIterator.next();
    if(key.isAcceptable()) {
        // a connection was accepted by a ServerSocketChannel.
    } else if (key.isConnectable()) {
        // a connection was established with a remote server.
    } else if (key.isReadable()) {
        // a channel is ready for reading
    } else if (key.isWritable()) {
        // a channel is ready for writing
    }
    keyIterator.remove();
}
```



# 停止选择的方法

选择器执行选择的过程，系统底层会依次询问每个通道是否已经就绪，这个过程可能会造成调用线程进入阻塞状态,那么我们有以下三种方式可以唤醒在select



- **wakeup()方法** ：通过调用Selector对象的wakeup（）方法让处在阻塞状态的select()方法立刻返回
  该方法使得选择器上的第一个还没有返回的选择操作立即返回。如果当前没有进行中的选择操作，那么下一次对select()方法的一次调用将立即返回。
- **close()方法** ：通过close（）方法关闭Selector，
  该方法使得任何一个在选择操作中阻塞的线程都被唤醒（类似wakeup（）），同时使得注册到该Selector的所有Channel被注销，所有的键将被取消，但是Channel本身并不会关闭。



