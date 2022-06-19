# 介绍

* 与使用 *synchronized* 方法和语句相比，锁实现提供了更扩展的锁操作。

* 锁是 在多线程环境中，对共享资源 的访问控制
* 尽管 *synchronized*关键字提供的 作用域使得编程更为简单，但有时候需要更为 弹性的方式使用锁 例如 `链式锁`

```
获取 nodeA的锁，获取nodeB的锁，然后释放A，
获取C的锁，释放B，获取D的锁
```

* 灵活性的提高带来了额外的工作量，例如不会 像 *synchronized* 自动释放锁



**锁实现提供了 比 *synchronized* 更多的功能**

* 非阻塞获取锁 *tryLock()*
* 可中断的方式获取锁 *lockInterruptibly* ，*tryLock(long, TimeUnit)*
* 非可重入
* 死锁检测
* 公平与公平



**内存同步语义**

所有Lock实现都必须强制执行与内置监视器锁相同的内存同步语义

三种获取锁的形式（可中断，不可中断，超时）根据 不同的实现而不同

实现需要清楚地记录每个锁定方法提供的语义和保证。



# Lock接口方法

## LOCK

**签名**

```
void lock();
```

* 获取锁，如果锁不可用，则当前线程进入休眠状态，不会被CPU进行线程调度
* 子类实现 要求 能够进行 错误检测，例如死锁，错误检测需要 标识出文档

## TryLock

**签名**

```
boolean tryLock();
```

* 当锁可用时 上锁，并返回*True*
* 当锁不可用时，返回*false*

*用法*

```java
 Lock lock = ...;
 if (lock.tryLock()) {
   try {
     // manipulate protected state
   } finally {
     lock.unlock();
   }
 } else {
   // perform alternative actions
 }
```



## Timed TryLock

**签名**

```java
boolean tryLock(long time, TimeUnit unit) throws InterruptedException;
```

* 如果锁在给定的等待时间内是空闲的，并且当前线程尚未中断，则获取该锁。
* 如果锁可用，则此方法立即返回true值。如果该锁不可用，当前线程处于休眠状态，直到发生以下三种情况之一：
    * 当前线程获取锁，返回true
    * 其他线程中断了此线程（获取锁支持中断） 抛出 *InterruptedException* 并清除中断标志
    * 指定时间到了 返回*false*



## Unlock

**签名**

```
void unlock();
```



## Condition

> **新建与 锁实例绑定的  条件**
>
> *synchronized* 与 wait,notify,notifyAll 的关系 等同于 *Condition* 与 Lock的关系

**签名**

```
Condition newCondition();
```

具体 条件类 见 [Condition接口](Condition条件等待.md)



