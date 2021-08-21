# 介绍

可重入互斥锁，实现了与 隐式锁 *synchronized* 同样的 行为 与 语义，但扩展了其功能

## **锁逻辑**

* 可重入锁  被上次成功上锁，但目前还未解锁 的线程拥有
* 如果 锁还未被任何线程占有，则当线程上锁时能成功获取锁
* 如果当前线程已经 占有改锁，重复加锁会记录加锁的次数，次数通过 *isHeldByCurrentThread, and getHoldCount.*获取

## 公平与非公平

**公平锁**

遵循先来先获取锁的原则，倾向于分配给等待时间最长的线程

使用公平锁会降低系统整体吞吐量，但减少 饥饿锁的现象

锁的公平获取不保证 线程的公平调度

*untimed tryLock()* 不会遵循公平与非公平，当锁可用时则 立即获取锁



## 建议使用方式

> 使用 try finally 保证锁会被释放

```java
 class X {
   private final ReentrantLock lock = new ReentrantLock();
   // ...

   public void m() {
     lock.lock();  // block until condition holds
     try {
       // ... method body
     } finally {
       lock.unlock()
     }
   }
 }
```



# 可重入锁

> Lock接口方法 全部委托于 *Sync* 类来实现

* 主要有两个同步器 *NonfairSync* *FairSync*

## *NonfairSync*

> 非公平锁同步器

### 独占锁获取逻辑

> 先尝试获取锁，体现了非公平的方式，如果没有获取成功则 入队列等待

```java
        final void lock() {
            if (compareAndSetState(0, 1))
                setExclusiveOwnerThread(Thread.currentThread());
            else
                acquire(1);
        }
```

### 独占锁释放逻辑

>  直接以独占锁的方式释放锁

```java
        protected final boolean tryRelease(int releases) {
            int c = getState() - releases; //拥有锁的线程不是 当前线程则报错
            if (Thread.currentThread() != getExclusiveOwnerThread())
                throw new IllegalMonitorStateException();
            boolean free = false;
            if (c == 0) {
                free = true;
                setExclusiveOwnerThread(null);
            }
            setState(c);
            return free;
        }
```

### 尝试获取锁

> 尝试获取锁，只能是非公平方式

```java
    public boolean tryLock() {
        return sync.nonfairTryAcquire(1);
    }


    final boolean nonfairTryAcquire(int acquires) {
        	//获取当前线程，获取当前线程状态
            final Thread current = Thread.currentThread();
            int c = getState();
            if (c == 0) { //锁可用
                if (compareAndSetState(0, acquires)) {  //尝试获取锁
                    setExclusiveOwnerThread(current);
                    return true;
                }
            }
            else if (current == getExclusiveOwnerThread()) { //获取锁失败，判断是否是同一线程
                int nextc = c + acquires;
                if (nextc < 0) // overflow
                    throw new Error("Maximum lock count exceeded");
                setState(nextc);
                return true;
            }
            return false;
        }
```

## FairSync

### **公平方式获取独占锁**

* 一开始不尝试获取锁，直接进入队列排队
* 如果没有继任者，尝试获取锁，如果获取锁成功，则返回true

```java
final void lock() {
    acquire(1);
}

protected final boolean tryAcquire(int acquires) {
    final Thread current = Thread.currentThread();
    int c = getState();
    if (c == 0) {
        if (!hasQueuedPredecessors() &&
            compareAndSetState(0, acquires)) {
            setExclusiveOwnerThread(current);
            return true;
        }
    }
    else if (current == getExclusiveOwnerThread()) {
        int nextc = c + acquires;
        if (nextc < 0)
            throw new Error("Maximum lock count exceeded");
        setState(nextc);
        return true;
    }
    return false;
}
```





### 判断是否可以获取锁

> **查询是否有任何线程在等待获取比当前线程更长的时间**

* 不存在排队队列
* 或者 head的下一个结点 不是 自己 `getFirstQueuedThread（）！= Thread.currentThread（）&& hasQueuedThreads（）`

请注意，由于中断和超时引起的取消可能随时发生，因此返回true不能保证某些其他线程将在当前线程之前获取。

```java
public final boolean hasQueuedPredecessors() {
    // The correctness of this depends on head being initialized
    // before tail and on head.next being accurate if the current
    // thread is first in queue.
    Node t = tail; // Read fields in reverse initialization order
    Node h = head;
    Node s;
    return h != t &&
        ((s = h.next) == null || s.thread != Thread.currentThread());
}
```

### 释放锁

**同非公平锁一样**

