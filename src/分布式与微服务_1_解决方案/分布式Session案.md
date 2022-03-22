# 分布式 Session

## 粘性 Session

* 场景

  将用户锁定到某一个服务器上，用户第一次请求时，负载均衡器将用户的请求转发到了 A 服务器上，如果负载均衡器设置了粘性 Session 的话，那么用户以后的每次请求都会转发到 A 服务器上，相当于把用户和 A 服务器粘到了一块，这就是粘性 Session 机制。

* 优点

  简单，不需要对 Session 做任何处理。

* 缺点

  缺乏容错性，如果当前访问的服务器发生故障，用户被转移到第二个服务器上时，他的 Session 信息都将失效。

* 适用场景

  - 发生故障对客户产生的影响较小；
  - 服务器发生故障是低概率事件。

## 服务器 Session 复制

* 任何一个服务器上的 Session 发生改变，该节点会把这个 Session 的所有内容序列化，然后广播给所有其它节点，不管其他服务器需不需要 Session，以此来保证 Session 同步

* 优点

  * 可容错，各个服务器间 Session 能够实时响应。

* 缺点

  会对网络负荷造成一定压力，如果 Session 量大的话可能会造成网络堵塞，拖慢服务器性能

* 实现方式

1. 设置 Tomcat 的 server.xml 开启 tomcat 集群功能。
2. 在应用里增加信息：通知应用当前处于集群环境中，支持分布式，即在 web.xml 中添加`<distributable/>` 选项。

## Session 共享机制

使用分布式缓存方案比如 Memcached、Redis，但是要求 Memcached 或 Redis 必须是集群。

* 粘性 Session 共享机制

  一个用户的 Session 会绑定到一个 Tomcat 上。Memcached 只是起到备份作用。

* 非粘性 Session 共享机制

  Tomcat 本身不存储 Session，而是存入 Memcached 中。Memcached 集群构建主从复制架构。

* 实现方式

  用开源的 msm 插件解决 Tomcat 之间的 Session 共享：Memcached_Session_Manager（MSM）

## Session 持久化到数据库

拿出一个数据库，专门用来存储 Session 信息。保证 Session 的持久化。

* 优点

  服务器出现问题，Session 不会丢失

* 缺点

  如果网站的访问量很大，把 Session 存储到数据库中，会对数据库造成很大压力，还需要增加额外的开销维护数据库。

## Terracotta 实现 Session 复制

Terracotta 的基本原理是对于集群间共享的数据，当在一个节点发生变化的时候，Terracotta 只把变化的部分发送给 Terracotta 服务器，然后由服务器把它转发给真正需要这个数据的节点。它是服务器 Session 复制的优化。