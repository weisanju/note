# 介绍

* 用来创建 工具锁，以及其他同步类的基本线程阻塞原语

* 此类与使用它的每个线程关联一个许可

* 如果许可 可用的化，调用 *park*立即返回，否则会阻塞

* *unpark* 会产生一个 许可，许可不会 积累，最多一个

* *unpark* 与 *park* 提供 了 比 *Thread.suspend* and *Thread.resume* 更 加有效的 线程 阻塞方式，支持可中断，可超时

* 同时也支持 *blocker object* 参数，线程被阻塞时会记录该对象，以允许监视和诊断工具确定线程被阻塞的原因。（此类工具可以使用方法getBlocker（Thread）访问 *blocker object*）

* 强烈建议使用 带 *blocker* 形式，而不要使用没有此参数的原始形式。

* 这些方法旨在用作创建高级同步实用程序的工具,本身对大多数并发控制应用程序都不有用。

* 一般使用以下形式

    ```java
     while (!canProceed()) { ... LockSupport.park(this); }
    ```

    任何中间对 *park* 或 *unpark*d的调用将会 影响 预期效果

* 先进先出 不可重入的示意图

    ```java
     class FIFOMutex {
       private final AtomicBoolean locked = new AtomicBoolean(false);
       private final Queue<Thread> waiters
         = new ConcurrentLinkedQueue<Thread>();
    
       public void lock() {
         boolean wasInterrupted = false;
         Thread current = Thread.currentThread();
         waiters.add(current);
    
         // Block while not first in queue or cannot acquire lock
         while (waiters.peek() != current ||
                !locked.compareAndSet(false, true)) {
           LockSupport.park(this);
           if (Thread.interrupted()) // ignore interrupts while waiting
             wasInterrupted = true;
         }
    
         waiters.remove();
         if (wasInterrupted)          // reassert interrupt status on exit
           current.interrupt();
       }
    
       public void unlock() {
         locked.set(false);
         LockSupport.unpark(waiters.peek());
       }
     }
    ```

# Park

**方法声明**

```java
    public static void park() {
        UNSAFE.park(false, 0L);
    }
```

线程会阻塞在此方法，并且不会被调度，直到以下情况发生

* 其他线程调用了 *unPark* 
* 线程被中断，不会抛异常
* 虚假调用

需要调用者自己检查 被唤醒原因，例如 是否被中断



# 超时Park

```java
public static void parkNanos(long nanos) {
    if (nanos > 0)
        UNSAFE.park(false, nanos);
}
```



# *DeadLine Park*

> 从公元 Epoch 开始的绝对时间

```java
    public static void parkUntil(long deadline) {
        UNSAFE.park(true, deadline);
    }
```





# Unpark

* 调用之后 会对 信号量+1，那么下次 *Park* 将会 不阻塞，能够精确的 发送 可靠信号

```java
public static void unpark(Thread thread) {
    if (thread != null)
        UNSAFE.unpark(thread);
}
```

