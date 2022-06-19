# Condition接口方法

## 介绍

* *Condition* 对象 将 对象监视锁方法（*wait*，*notify*，*noyifyAll* ） 分解成多个对象，通过与任意Lock实现结合使用，使得每个锁实例对象有多个等待集

* 条件（也称为条件队列或条件变量）为一个线程挂起（“等待”），直到另一线程通知某些状态条件现在可能为真提供了一种方法。
* condition的关键特性： 它自动释放关联的锁并挂起当前线程，就像Object.wait一样。
* condition与锁实例 紧密关联，通过  *newCondition* 创建*Condition**
* *Condition* 子类实现能提供 比 对象锁方法 （ *wait*，*notify*，*noyifyAll* ） 更多的功能，例如 有序通知，没有必要 只能在拥有锁的情况下通知，
* 不要使用 Condition对象身上的 对象锁

## 实现注意事项

> 当条件等待时，允许 *spurious wakeup*情况发生，

实现可以自由地消除虚假唤醒的可能性，但是建议应用程序程序员始终假定它们会发生，因此总是在循环中等待。

三种形式的条件等待 （interruptible, non-interruptible, and timed）应实现各异



## 举例说明

假设我们有一个有界缓冲区，它支持put和take方法。

当缓冲区为空， take方法 会阻塞

当缓冲区为满，put方法会阻塞

这样可以使用两个 Condition实例

**Code**

> 该实例，将put与take 分离，分别做两个等待条件
>
> java.util.concurrent.ArrayBlockingQueue  就是如此的实现思想

```java
   class BoundedBuffer {
     final Lock lock = new ReentrantLock();
     final Condition notFull  = lock.newCondition(); 
     final Condition notEmpty = lock.newCondition(); 
  
     final Object[] items = new Object[100];
     int putptr, takeptr, count;
  
     public void put(Object x) throws InterruptedException {
       lock.lock();
       try {
         while (count == items.length)
           notFull.await();
         items[putptr] = x;
         if (++putptr == items.length) putptr = 0;
         ++count;
         notEmpty.signal();
       } finally {
         lock.unlock();
       }
     }
  
     public Object take() throws InterruptedException {
       lock.lock();
       try {
         while (count == 0)
           notEmpty.await();
         Object x = items[takeptr];
         if (++takeptr == items.length) takeptr = 0;
         --count;
         notFull.signal();
         return x;
       } finally {
         lock.unlock();
       }
     }
   }
```

## 接口方法

### Await

**方法申明**

```java
void await() throws InterruptedException;
```

#### **方法语义**

* 与此条件相关联的锁被原子释放，当前线程处于休眠状态，直到发生以下四种情况之一：
    * 其他线程 调用了 该 condition的 *signal* 方法，且碰巧当前线程被选中 唤醒
    * 其他线程 调用了 *signalAll* 方法
    * 其他线程 打断了 该线程，且 该Condition支持 中断
    * 虚假唤醒 
* 只有线程重新竞争到锁了，之后代码才会继续执行

#### **同类型方法**

**不可中断等待**

> 不支持中断

```
void awaitUninterruptibly();
```

**超时等待**

> 可不可中断，看各自的实现

```
boolean awaitUntil(Date deadline) throws InterruptedException;
boolean await(long time, TimeUnit unit) throws InterruptedException;
long awaitNanos(long nanosTimeout) throws InterruptedException;
```



### Signal

> 唤醒等待在该条件的线程

**方法申明**

```
void signal();
void signalAll();
```

**方法语义**

* 唤醒原则 决定于实现
* 唤醒一个，随机，或者公平，或者 全部唤醒







# *ConditionObject* 

> 唯一的 具体实现类，

## **类声明**

```java
public abstract class AbstractQueuedSynchronizer
    extends AbstractOwnableSynchronizer
    implements java.io.Serializable {
	//与AQS 同步器 实例，相关联，一个AQS实例 可以 拥有 多个ConditionObject
    public class ConditionObject implements Condition, java.io.Serializable {}
}
```



## 条件等待

### AWAIT方法

* 是将当前线程 入 条件队列，并等待唤醒，或者中断
* 被*signal* 唤醒后，将其 从条件队列  移出 到 同步队列等待
* 被中断时 也将其 从条件队列  移出 到 同步队列等待
* 获取锁成功之后 清理 条件队列中cancelled结点

```java
        public final void await() throws InterruptedException {
            //如果是中断的，则抛异常
            if (Thread.interrupted())
                throw new InterruptedException();
            Node node = addConditionWaiter();//在 该条件队列 新增一个等待结点
            int savedState = fullyRelease(node); //释放当前线程所占有的 锁资源
            int interruptMode = 0;
            while (!isOnSyncQueue(node)) { 判断当前 线程结点 是否在AQS同步队列
                LockSupport.park(this);//如果不在 则阻塞
                if ((interruptMode = checkInterruptWhileWaiting(node)) != 0)
                    //当线程被唤醒后，判断 是由于 中断唤醒的，还是 signal 唤醒的，如果是由于中断唤醒的则，将结点 从 条件队列转移到同步队列
                    break;
            }
            //尝试在同步队列等待获取锁，如果被中断了，且之前未发生中断，或者 signal先发生于中断
            if (acquireQueued(node, savedState) && interruptMode != THROW_IE)
                interruptMode = REINTERRUPT;
            if (node.nextWaiter != null) // clean up if cancelled ，清理cancelled结点
                unlinkCancelledWaiters();
            if (interruptMode != 0) //重放中断
                reportInterruptAfterWait(interruptMode);
        }
```

### 往条件队列尾部插入结点

```java
        private Node addConditionWaiter() {
            Node t = lastWaiter;
            // If lastWaiter is cancelled, clean out. 清理cancelled结点
            if (t != null && t.waitStatus != Node.CONDITION) {
                unlinkCancelledWaiters();
                t = lastWaiter;
            }
            Node node = new Node(Thread.currentThread(), Node.CONDITION);
            if (t == null)
                firstWaiter = node;
            else
                t.nextWaiter = node;
            lastWaiter = node;
            return node;
        }
```

### 释放当前线程持有锁

* 如果释放失败，当前线程抛异常，并将结点置为 CANCELLED
* 其他等待在该条件队列的结点 会自动清理 CANCELLED结点

```java
    final int fullyRelease(Node node) {
        boolean failed = true;
        try {
            int savedState = getState();
            if (release(savedState)) {
                failed = false;
                return savedState;
            } else {
                throw new IllegalMonitorStateException();
            }
        } finally {
            
            if (failed)
                node.waitStatus = Node.CANCELLED;
        }
    }
```

### 判断是否在同步队列

**在条件队列满足的情况**

* 状态 为 CONDITION肯定位于 条件队列
* prev指针为空，肯定是位于 条件队列 （同步队列CAS操作维护的是*prev* 指针，所以一定不可能为空）
* next指针 不为*null* 一定是在同步队列
* 以上判断均不通过则
    * 从尾到头 遍历结点 找到了则 在同步队列 找不到则不在

```java
    final boolean isOnSyncQueue(Node node) {
        if (node.waitStatus == Node.CONDITION || node.prev == null)
            return false;
        if (node.next != null) // If has successor, it must be on queue
            return true;
        /*
         * node.prev can be non-null, but not yet on queue because
         * the CAS to place it on queue can fail. So we have to
         * traverse from tail to make sure it actually made it.  It
         * will always be near the tail in calls to this method, and
         * unless the CAS failed (which is unlikely), it will be
         * there, so we hardly ever traverse much.
         */
        return findNodeFromTail(node);
    }

    private boolean findNodeFromTail(Node node) {
        Node t = tail;
        for (;;) {
            if (t == node)
                return true;
            if (t == null)
                return false;
            t = t.prev;
        }
    }
```

### 唤醒判断

> 条件队列等待的结点被唤醒后，可能有两种情况，一种是被 中断唤醒，一种是 被 signal唤醒

* 判断是否存在中断
    * 存在中断
        * 如果CAS没有失败，中断前没有发生 *signal* 则直接入同步队列，更新 结点状态为0
        * 如果CAS失败了，则中断前发生了 *signal*，则等待 自旋等待 其入同步队列
    * 不存在中断则返回0

```java
private int checkInterruptWhileWaiting(Node node) {
    return Thread.interrupted() ?
        (transferAfterCancelledWait(node) ? THROW_IE : REINTERRUPT) :
    0;
}

final boolean transferAfterCancelledWait(Node node) {
    if (compareAndSetWaitStatus(node, Node.CONDITION, 0)) {
        enq(node);
        return true;
    }
    /*
         * If we lost out to a signal(), then we can't proceed
         * until it finishes its enq().  Cancelling during an
         * incomplete transfer is both rare and transient, so just
         * spin.
         */
    while (!isOnSyncQueue(node))
        Thread.yield();
    return false;
}
```



## 条件唤醒

> 唤醒条件队列，需要持有锁，且以队列先进先出形式 唤醒.

**先进先出唤醒**

```java
        public final void signal() {
            //唤醒条件 需要持有锁
            if (!isHeldExclusively())
                throw new IllegalMonitorStateException();
            Node first = firstWaiter;
            //唤醒队列中的第一个
            if (first != null)
                doSignal(first);
        }
```



**头节点出队列**

```java
        private void doSignal(Node first) {
        //移除头结点，并清空引用，当头结点也为空了，尾结点也置空
            do {
                if ( (firstWaiter = first.nextWaiter) == null)
                    lastWaiter = null;
                first.nextWaiter = null;
            } while (!transferForSignal(first) &&
                     (first = firstWaiter) != null);
        }
```

**入同步队列**

```java
    final boolean transferForSignal(Node node) {
        /*
         * If cannot change waitStatus, the node has been cancelled.如果结点被取消了。则尝试唤醒下一个结点
         */
        if (!compareAndSetWaitStatus(node, Node.CONDITION, 0))
            return false;

        /*
         * Splice onto queue and try to set waitStatus of predecessor to
         * indicate that thread is (probably) waiting. If cancelled or
         * attempt to set waitStatus fails, wake up to resync (in which
         * case the waitStatus can be transiently and harmlessly wrong).
         */
        Node p = enq(node);
        int ws = p.waitStatus;
        if (ws > 0 || !compareAndSetWaitStatus(p, ws, Node.SIGNAL))
            LockSupport.unpark(node.thread);
        return true;
    }
```

**总结**

* 首先发信号需要持有锁
* 先唤醒 条件队列中的第一个，如果已取消则顺延到下一个
* 然后入同步队列，并尝试修改 前驱的 状态为 SIGNAL，修改失败可能是由于 线程中断，超时，取消等原因
* 修改失败，则唤醒 当前结点 同步前驱引用





## 唤醒全部

循环遍历 条件队列中的每一个 结点调用 上述 唤醒逻辑





## 不可中断等待

* 新增结点
* 释放锁
* 如果不在同步队列则 阻塞
* 如果 中途被中断了 则记录中断状态
* 后续被唤醒后 重放中断

```java
public final void awaitUninterruptibly() {
    Node node = addConditionWaiter();
    int savedState = fullyRelease(node);
    boolean interrupted = false;
    while (!isOnSyncQueue(node)) {
        LockSupport.park(this);
        if (Thread.interrupted())
            interrupted = true;
    }
    if (acquireQueued(node, savedState) || interrupted)
        selfInterrupt();
}
```



## 超时等待

* 新增结点
* 释放锁
* 如果不在同步队列则 阻塞指定纳秒，（当小于1000NS，则应该自旋比阻塞更快）
* 如果中间存在中断则 响应中断
* 如果 时间到期了，则入同步队列

```java
public final long awaitNanos(long nanosTimeout)
    throws InterruptedException {
    if (Thread.interrupted())
        throw new InterruptedException();
    Node node = addConditionWaiter();
    int savedState = fullyRelease(node);
    final long deadline = System.nanoTime() + nanosTimeout;
    int interruptMode = 0;
    while (!isOnSyncQueue(node)) {
        if (nanosTimeout <= 0L) {
            transferAfterCancelledWait(node);
            break;
        }
        if (nanosTimeout >= spinForTimeoutThreshold)
            LockSupport.parkNanos(this, nanosTimeout);
        if ((interruptMode = checkInterruptWhileWaiting(node)) != 0)
            break;
        nanosTimeout = deadline - System.nanoTime();
    }
    if (acquireQueued(node, savedState) && interruptMode != THROW_IE)
        interruptMode = REINTERRUPT;
    if (node.nextWaiter != null)
        unlinkCancelledWaiters();
    if (interruptMode != 0)
        reportInterruptAfterWait(interruptMode);
    return deadline - System.nanoTime();
}
```

