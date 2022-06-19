# 介绍

* 同步工具类 旨在 允许 所有线程 等待直到 他们 达到了 某个 障碍点

* CyclicBarriers在涉及固定大小的线程的程序中很有用，该线程方有时必须互相等待
* The barrier is called cyclic  是因为 它可以在释放等待线程后重新使用。

* *CyclicBarrier* 支持可选的Runnable命令，该命令在障碍中的最后一个线程到达之后 但在释放任何线程之前，每个障碍点运行一次。
    this barrier action 对于在任何线程继续之前  更新共享状态很有用。



# AWAIT

阻塞 直到 所有线程都 阻塞在 该 *barrier* 

**从AWAIT唤醒**

如果当前线程 不是 最后一个 调用 *AWAIT* ，那么该线程会被 休眠，不回被调度。直到以下事情发生

* 最后一个线程到达
* 当前线程被打断了
* 位于等待的  其他线程之一 被打断了
* 等待中的 线程 超时了
* 其他线程调用 *reset*

**中断异常抛出**

* 如果当前线程 进入这个方法之前被 中断了

* 或者在等待 的过程中  被中断了 则 先清除中断状态 抛出 *InterruptedException*

**BrokenBarrierException抛出**

* 如果 调用 *reset* 时 有线程 在等待，
* 或者 有线程调用 *await* 或者 *barrier* is *broken* 

**BarrierAction**

如果一个线程 被中断了，则其他线程 将会 抛出 *BrokenBarrierException*

如果一个线程是最后调用 *await* 而且 提供了 非空的  *BarrierAction* ，它会首先执行 *BarrierAction* 然后在唤醒其他线程

如果执行 BarrierAction 抛异常，则异常会被 抛出到当前线程，barrier也被置位 *broken*

**非超时等待**

```java
public int await() throws InterruptedException, BrokenBarrierException {
    try {
        return dowait(false, 0L);
    } catch (TimeoutException toe) {
        throw new Error(toe); // cannot happen
    }
}
```

```java
private int dowait(boolean timed, long nanos)
    throws InterruptedException, BrokenBarrierException,
           TimeoutException {
    final ReentrantLock lock = this.lock;
    lock.lock();
    try {
        //获取当前的 barrier代
        final Generation g = generation;

        //如果已经 破损，则抛出破损异常
        if (g.broken)
            throw new BrokenBarrierException();

        //如果当前线程被中断了，则 将 barrier 置位 broken，并抛出中断异常
        if (Thread.interrupted()) {
            breakBarrier();
            throw new InterruptedException();
        }
		//对 count自减
        int index = --count;
        
        //最后一个 arrive 则执行 BarrierAction
        if (index == 0) {  // tripped
            boolean ranAction = false;
            try {
                final Runnable command = barrierCommand;
                if (command != null)
                    command.run();
                ranAction = true; //更新换代 barrier generation
                nextGeneration();
                return 0;
            } finally { //如果抛异常了，则 将 barrier置位 broken
                if (!ranAction)
                    breakBarrier();
            }
        }
		//如果不是最后一个 arrive的线程，则 自选等待
        // loop until tripped, broken, interrupted, or timed out
        for (;;) {
            try {
                if (!timed)
                    trip.await();
                else if (nanos > 0L)
                    nanos = trip.awaitNanos(nanos);
            } catch (InterruptedException ie) {
                //被中断了，且还没有 broken 则 broken
                if (g == generation && ! g.broken) {
                    breakBarrier();
                    throw ie;
                } else {
                    //被中断了，但已经更新换代了，则可以认为 继续执行
                    // We're about to finish waiting even if we had not
                    // been interrupted, so this interrupt is deemed to
                    // "belong" to subsequent execution.
                    Thread.currentThread().interrupt();
                }
            }

            if (g.broken)
                throw new BrokenBarrierException();
			
            //如果更新换代了，则说明是被最后一个 arriver 唤醒的，则返回index,到达的索引
            if (g != generation)
                return index;

            //如果超时了，则 置 breakBarrier 为 broken
            if (timed && nanos <= 0L) {
                breakBarrier();
                throw new TimeoutException();
            }
        }
    } finally {
        lock.unlock();
    }
}
```



# 重置

```java
    public void reset() {
        final ReentrantLock lock = this.lock;
        lock.lock();
        try {
            //将 barrier 置位 broken
            breakBarrier();   // break the current generation
            //开始下一个 generation
            nextGeneration(); // start a new generation
        } finally {
            lock.unlock();
        }
    }
```



# 打破Barrier

* 代数置位 *true*
* 重置 count
* 唤醒其他 阻塞线程

```java
private void breakBarrier() {
    generation.broken = true;
    count = parties;
    trip.signalAll();
}
```



# 更新换代

* 通知其他 线程
* 重置 count
* 更新 generation 引用

```java
private void nextGeneration() {
    // signal completion of last generation
    trip.signalAll();
    // set up next generation
    count = parties;
    generation = new Generation();
}
```

