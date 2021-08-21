# 命令

## 发布

```shell
publish channel message
```

## 订阅

```
subscribe channel [channel ...]
```

使用订阅命令，需要主要几点：

* 客户端执行订阅指令之后，就会进入订阅状态，之后就只能接收 **subscribe**、**psubscribe**、**unsubscribe**、**punsubscribe** 这四个命令。

* 第二，新订阅的客户端，是**无法收到这个频道之前的消息**，这是因为 Redis 并不会对发布的消息持久化的。

## 模式匹配的订阅方式

```
psubscribe pay.*
punsubscribe pay.*
```



# 基于 Jedis 开发发布/订阅

**发布**

```java
HostAndPort hostAndPort1 = new HostAndPort("192.168.3.16",7000);
HostAndPort hostAndPort2 = new HostAndPort("192.168.3.16",7001);
HostAndPort hostAndPort3 = new HostAndPort("192.168.3.16",7002);
HostAndPort hostAndPort4 = new HostAndPort("192.168.3.16",7003);
HostAndPort hostAndPort5 = new HostAndPort("192.168.3.16",7004);
HostAndPort hostAndPort6 = new HostAndPort("192.168.3.16",7005);
HashSet<HostAndPort> objects = new HashSet<>();
objects.add(hostAndPort1);
objects.add(hostAndPort2);
objects.add(hostAndPort3);
objects.add(hostAndPort4);
objects.add(hostAndPort5);
objects.add(hostAndPort6);

JedisCluster jedisCluster = new JedisCluster(objects);

jedisCluster.publish("xjq","yes");
```

**订阅**

```java
jedisCluster.subscribe(new JedisPubSub() {
    @Override
    public void onMessage(String channel, String message) {
        System.out.printf("I have rececive a message from %s,message is %s%n",channel,message);
    }
},"xjq");
```

# Redis 发布订阅实际应用

### Redis Sentinel 节点发现

**Redis Sentinel** 节点主要使用发布订阅机制，实现新节点的发现，以及交换主节点的之间的状态

如下所示，每一个 **Sentinel** 节点将会定时向 `_sentinel_:hello` 频道发送消息，并且每个 **Sentinel** 都会订阅这个节点。

![](/images/redis-sentinel-pub-sub.jpg)

这样一旦有节点往这个频道发送消息，其他节点就可以立刻收到消息。

这样一旦有的新节点加入，它往这个频道发送消息，其他节点收到之后，判断本地列表并没有这个节点，于是就可以当做新的节点加入本地节点列表。

除此之外，每次往这个频道发送消息内容可以包含节点的状态信息，这样可以作为后面 **Sentinel** 领导者选举的依据。

以上都是对于 Redis 服务端来讲，对于客户端来讲，我们也可以用到发布订阅机制。

当 **Redis Sentinel** 进行主节点故障转移，这个过程各个阶段会通过发布订阅对外提供。

对于我们客户端来讲，比较关心切换之后的主节点，这样我们及时切换主节点的连接（旧节点此时已故障，不能再接受操作指令），

客户端可以订阅 `+switch-master`频道，一旦 **Redis Sentinel** 结束了对主节点的故障转移就会发布主节点的的消息。



## redission 分布式锁

就是采用服务通知的机制实现分布式锁的唤醒

当线程加锁失败之后，线程将会订阅 `redisson_lock__channel_xxx`（xx 代表锁的名称） 频道，使用异步线程监听消息，然后利用 Java 中 `Semaphore` 使当前线程进入阻塞

一旦其他客户端进行解锁，redission 就会往这个`redisson_lock__channel_xxx` 发送解锁消息。

等异步线程收到消息，将会调用 `Semaphore` 释放信号量，从而让当前被阻塞的线程唤醒去加锁