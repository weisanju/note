# 介绍

* 计数信号量，从概念上讲，信号量维护一组许可证。

* 每一个*acquire* 消费一个许可，每一个*release* 增加一个许可

* 当许可不够时，*acquire* 会被阻塞

* 信号量一般使用于 限制 指定数量线程 能够访问某些资源

**example**

```java
class Pool {
   private static final int MAX_AVAILABLE = 100;
   private final Semaphore available = new Semaphore(MAX_AVAILABLE, true);

   public Object getItem() throws InterruptedException {
     available.acquire();
     return getNextAvailableItem();
   }

   public void putItem(Object x) {
     if (markAsUnused(x))
       available.release();
   }

   // Not a particularly efficient data structure; just for demo

   protected Object[] items = ... whatever kinds of items being managed
   protected boolean[] used = new boolean[MAX_AVAILABLE];

   protected synchronized Object getNextAvailableItem() {
     for (int i = 0; i < MAX_AVAILABLE; ++i) {
       if (!used[i]) {
          used[i] = true;
          return items[i];
       }
     }
     return null; // not reached
   }

   protected synchronized boolean markAsUnused(Object item) {
     for (int i = 0; i < MAX_AVAILABLE; ++i) {
       if (item == items[i]) {
          if (used[i]) {
            used[i] = false;
            return true;
          } else
            return false;
       }
     }
     return false;
   }
 }
```



* 信号量的获取可以保证公平与非公平

* *untimed* *try* 操作不保证公平
* 一般 信号量 使用公平的方式 初始化，以确保不会造成 饥饿，当使用 信号量作为另类的 同步器，建议使用 非公平，以提升吞吐量
* 同样 可以 获取 多个 或者释放多个资源



# 构造

> 初始化，信号量 类似生产消费者模型

```java
public Semaphore(int permits, boolean fair) {
    sync = fair ? new FairSync(permits) : new NonfairSync(permits);
}
```



# Acquire操作

> 可中断，阻塞 获取

* 尝试获取指定量的信号量
* 获取成功则返回 剩余量
* 获取失败则 入队列

```java
// Semphre 信号量调用
public void acquire() throws InterruptedException {
sync.acquireSharedInterruptibly(1);
}
// sync同步器调用
public final void acquireSharedInterruptibly(int arg)
    throws InterruptedException {
    if (Thread.interrupted())
        throw new InterruptedException();
    if (tryAcquireShared(arg) < 0)
        doAcquireSharedInterruptibly(arg);
}
//NonfairSync 非公平的 资源获取
protected int tryAcquireShared(int acquires) {
    return nonfairTryAcquireShared(acquires);
}

final int nonfairTryAcquireShared(int acquires) {
    for (;;) {
        int available = getState();
        int remaining = available - acquires;
        if (remaining < 0 ||
            compareAndSetState(available, remaining))
            return remaining;
    }
}
```



# *ReducePermists*

**扣减可用的资源**

```java
        final void reducePermits(int reductions) {
            for (;;) {
                int current = getState();
                int next = current - reductions;
                if (next > current) // underflow
                    throw new Error("Permit count underflow");
                if (compareAndSetState(current, next))
                    return;
            }
        }
```

# *Release*

**释放一个许可证**

```java
public void release() {
    sync.releaseShared(1);
}
//通用 释放共享锁
public final boolean releaseShared(int arg) {
    if (tryReleaseShared(arg)) {
        doReleaseShared();
        return true;
    }
    return false;
}
```

**子类实现state更新**

> release既 给 增加 state，只要不超过 Integer.MaxVALUE 就能通过

```java
protected final boolean tryReleaseShared(int releases) {
    for (;;) {
        int current = getState();
        int next = current + releases;
        if (next < current) // overflow
            throw new Error("Maximum permit count exceeded");
        if (compareAndSetState(current, next))
            return true;
    }
}
```



# *DrainPermits*

**立即将许可证置为0**

```java
final int drainPermits() {
    for (;;) {
        int current = getState();
        if (current == 0 || compareAndSetState(current, 0))
            return current;
    }
}
```





