# Cache Abstraction

从3.1版开始，Spring框架提供了对将缓存透明添加到现有Spring应用程序的支持。与事务支持类似，缓存抽象允许对各种缓存解决方案的一致使用，而对代码的影响最小。

从Spring 4.1开始，通过支持JSR-107注释和更多自定义选项，对缓存抽象进行了显着扩展。



# 了解缓存抽象

## **Cache vs Buffer**

* 术语“缓冲区”和“缓存”倾向于互换使用。但是请注意，它们代表不同的事物。
* 一般，缓冲区用作快速实体和慢速实体之间的数据的中间临时存储。由于一方必须等待另一方（这会影响性能）
* 缓冲区通过允许立即移动整个数据块而不是小块数据来缓解这种情况。数据只能从缓冲区写入和读取一次。此外，缓冲区对于至少一个知道缓冲区的一方是可见的。
* 另一方面，根据定义，缓存是隐藏的，任何一方都不知道发生了缓存。它还可以提高性能，但是可以通过快速读取多次相同数据来实现。
* 本质上，缓存抽象 是对 方法的缓存，减少方法的执行次数。对于方法的调用者来说是透明的
* 要保证 方法是无状态的 

## spring缓存抽象

> spring对 缓存的抽象 是通过对 *org.springframework.cache.Cache* ` and `  *org.springframework.cache.CacheManager* 

**Spring提供了该抽象的一些实现：**

* JDK `java.util.concurrent.ConcurrentMap` based caches, 
* [Ehcache 2.x](https://www.ehcache.org/), 
* Gemfire cache, 
* [Caffeine](https://github.com/ben-manes/caffeine/wiki),
*  JSR-107 compliant caches (such as Ehcache 3.x). 

**多进程环境的缓存**

* 如果您具有多进程环境（即，一个应用程序部署在多个节点上），则需要相应地配置缓存提供程序。根据您的用例，在几个节点上复制相同数据就足够了。
    但是，如果在应用程序过程中更改数据，则可能需要启用其他传播机制。

**多线程环境的缓存**

缓存某一个对象时， 直接等 同于 典型的  缓存交互程序中的  “如果找不到，然后继续执行 之后并放入”的 代码块。

没有应用锁，几个线程可能会尝试同时加载同一项目。驱逐同样如此。如果多个线程试图同时更新或逐出数据，则可能使用了旧数据。

**要使用缓存抽象，您需要注意两个方面：**

- Caching declaration（申明缓存）: Identify the methods that need to be cached and their policy.
- Cache configuration（配置缓存）: The backing cache where the data is stored and from which it is read.