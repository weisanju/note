# 介绍

* 一种同步工具类  旨在 允许一个或多个线程等待，直到在其他线程中执行的一组操作完成为止

* *await* 方法 会阻塞 直到 调用 *countDown* 使得 *count* 数量为0，所有线程 将会执行接下来的操作

* 这是一次性的 现象，如果需要 重置 *count* 请使用 *CyclicBarrier*

* 简单使用

    **example1**

    ```java
    class Driver { // ...
       void main() throws InterruptedException {
           //启动信号量
         CountDownLatch startSignal = new CountDownLatch(1);
         CountDownLatch doneSignal = new CountDownLatch(N);
    
         for (int i = 0; i < N; ++i) // create and start threads
           new Thread(new Worker(startSignal, doneSignal)).start();
    
         doSomethingElse();            // don't let run yet
          //启动线程
         startSignal.countDown();      // let all threads proceed
         doSomethingElse();
           //等待线程
         doneSignal.await();           // wait for all to finish
       }
     }
    
     class Worker implements Runnable {
       private final CountDownLatch startSignal;
       private final CountDownLatch doneSignal;
       Worker(CountDownLatch startSignal, CountDownLatch doneSignal) {
         this.startSignal = startSignal;
         this.doneSignal = doneSignal;
       }
       public void run() {
         try {
             //所有线程等待启动
           startSignal.await();
           doWork();
           doneSignal.countDown();
         } catch (InterruptedException ex) {} // return;
       }
    
       void doWork() { ... }
     }
    ```

    **example2**

    ```java
     class Driver2 { // ...
       void main() throws InterruptedException {
         CountDownLatch doneSignal = new CountDownLatch(N);
         Executor e = ...
    
         for (int i = 0; i < N; ++i) // create and start threads
           e.execute(new WorkerRunnable(doneSignal, i));
    
         doneSignal.await();           // wait for all to finish
       }
     }
    
     class WorkerRunnable implements Runnable {
       private final CountDownLatch doneSignal;
       private final int i;
       WorkerRunnable(CountDownLatch doneSignal, int i) {
         this.doneSignal = doneSignal;
         this.i = i;
       }
       public void run() {
         try {
           doWork(i);
           doneSignal.countDown();
         } catch (InterruptedException ex) {} // return;
       }
    
       void doWork() { ... }
     }
    ```

* **Memory consistency effects**: Until the count reaches zero, actions in a thread prior to calling countDown() happen-before actions following a successful return from a corresponding await() in another thread.



# AWAIT

**等待**

```java
public void await() throws InterruptedException {
    sync.acquireSharedInterruptibly(1);
}
//AQS尝试获取 锁，获取锁失败则阻塞在同步队列中等待
public final void acquireSharedInterruptibly(int arg)
    throws InterruptedException {
    if (Thread.interrupted())
        throw new InterruptedException();
    if (tryAcquireShared(arg) < 0)
        doAcquireSharedInterruptibly(arg);
}
```

**尝试获取锁资源**

**只要 状态不为0，则一直等待**

```java
protected int tryAcquireShared(int acquires) {
    return (getState() == 0) ? 1 : -1;
}
```



# CountDown

* 自旋释放
* 如果 状态量为0 ，则 唤醒所有等待 在 该同步队列的 线程

```java
public void countDown() {
    sync.releaseShared(1);
}

protected boolean tryReleaseShared(int releases) {
    // Decrement count; signal when transition to zero
    for (;;) {
        int c = getState();
        if (c == 0)
            return false;
        int nextc = c-1;
        if (compareAndSetState(c, nextc))
            return nextc == 0;
    }
}
```



# 超时AWAIT

```java
    public boolean await(long timeout, TimeUnit unit)
        throws InterruptedException {
        return sync.tryAcquireSharedNanos(1, unit.toNanos(timeout));
    }
```

