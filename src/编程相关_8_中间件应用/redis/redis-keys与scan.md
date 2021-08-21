# 问题描述

Shiro 并没有直接提供 Redis 存储 Session 组件，所以使用 Github 一个开源组件 [shiro-redis](https://github.com/alexxiyang/shiro-redis)

由于 Shiro 框架需要定期验证 Session 是否有效，于是 Shiro 底层将会调用 `SessionDAO#getActiveSessions` 获取所有的 Session 信息。

底层使用用`keys` 命令查找 Redis 所有存储的 `Session` key，效率较慢

```java
public Set<byte[]> keys(byte[] pattern){
    checkAndInit();
    Set<byte[]> keys = null;
    Jedis jedis = jedisPool.getResource();
    try{
        keys = jedis.keys(pattern);
    }finally{
        jedis.close();
    }
    return keys;
}
```

在最新版本中，`shiro-redis` 采用 `scan`命令代替 `keys`,从而修复这个问题。

```java
public Set<byte[]> keys(byte[] pattern) {
    Set<byte[]> keys = null;
    Jedis jedis = jedisPool.getResource();

    try{
        keys = new HashSet<byte[]>();
        ScanParams params = new ScanParams();
        params.count(count);
        params.match(pattern);
        byte[] cursor = ScanParams.SCAN_POINTER_START_BINARY;
        ScanResult<byte[]> scanResult;
        do{
            scanResult = jedis.scan(cursor,params);
            keys.addAll(scanResult.getResult());
            cursor = scanResult.getCursorAsBytes();
        }while(scanResult.getStringCursor().compareTo(ScanParams.SCAN_POINTER_START) > 0);
    }finally{
        jedis.close();
    }
    return keys;

}
```



为什么`keys` 指令会导致其他命令执行变慢？

为什么`Keys` 指令查询会这么慢？

为什么`Scan` 指令就没有问题？

# Redis 执行命令的原理

![](/images/redis-execute-cmd.jpg)

由于 Redis 单线程执行命令，只能顺序从队列取出任务开始执行。

只要 3 这个过程执行命令速度过慢，队列其他任务不得不进行等待，这对外部客户端看来，Redis 好像就被阻塞一样，一直得不到响应。

# KEYS 原理

Redis 底层使用字典这种结构，这个结构与 Java HashMap 底层比较类似。

`keys`命令需要返回所有的符合给定模式 `pattern` 的 Redis 中键，为了实现这个目的，Redis 不得不遍历字典中 `ht[0]`哈希表底层数组，这个时间复杂度为 **O(N)**（N 为 Redis 中 key 所有的数量）。

# SCAN 原理

最后我们来看下第三个问题，为什么`scan` 指令就没有问题？

这是因为 `scan`命令采用一种黑科技-**基于游标的迭代器**。

每次调用 `scan` 命令，Redis 都会向用户返回一个新的游标以及一定数量的 key。下次再想继续获取剩余的 key，需要将这个游标传入 scan 命令， 以此来延续之前的迭代过程。

简单来讲，`scan` 命令使用分页查询 redis 。

下面是一个 scan 命令的迭代过程示例：

`scan` 命令使用游标这种方式，巧妙将一次全量查询拆分成多次，降低查询复杂度。



虽然 `scan` 命令时间复杂度与 `keys`一样，都是 **O(N)**，但是由于 `scan` 命令只需要返回少量的 key，所以执行速度会很快。

最后，虽然`scan` 命令解决 `keys`不足，但是同时也引入其他一些缺陷：

- 同一个元素可能会被返回多次，这就需要我们应用程序增加处理重复元素功能。
- 如果一个元素在迭代过程增加到 redis，或者说在迭代过程被删除，那个这个元素会被返回，也可能不会。



除了 `scan`以外，redis 还有其他几个用于增量迭代命令：

- `sscan`:用于迭代当前数据库中的数据库键，用于解决 `smembers` 可能产生阻塞问题
- `hscan`命令用于迭代哈希键中的键值对，用于解决 `hgetall` 可能产生阻塞问题。
- `zscan`:命令用于迭代有序集合中的元素（包括元素成员和元素分值），用于产生 `zrange` 可能产生阻塞问题。



# 总结

Redis 使用单线程执行操作命令，所有客户端发送过来命令，Redis 都会现放入队列，然后从队列中顺序取出执行相应的命令。

所以不要在生产执行 `keys`、`smembers`、`hgetall`、`zrange`这类可能造成阻塞的指令，如果真需要执行，可以使用相应的`scan` 命令渐进式遍历，可以有效防止阻塞问题。



