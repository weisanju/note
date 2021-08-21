# 概述

`SynchronousQueue`类实现了[`BlockingQueue`](https://links.jianshu.com/go?to=http%3A%2F%2Ftutorials.jenkov.com%2Fjava-util-concurrent%2Fblockingqueue.html)接口。

`SynchronousQueue`是一个内部只能包含一个元素的队列。插入元素到队列的线程被阻塞，直到另一个线程从队列中获取了队列中存储的元素。同样，如果线程尝试获取元素并且当前不存在任何元素，则该线程将被阻塞，直到线程将元素插入队列。

将这个类称为队列有点不是很形象，这更像是一个点。

# 源码分析

`SynchronousQueue`的内部实现了两个类，一个是`TransferStack`类，使用LIFO顺序存储元素，这个类用于非公平模式；还有一个类是`TransferQueue`，使用FIFI顺序存储元素，这个类用于公平模式。这两个类继承自"Nonblocking Concurrent Objects with Condition Synchronization"算法，此算法是由W. N. Scherer III 和 M. L. Scott提出的，关于此算法的理论内容在这个网站中：[http://www.cs.rochester.edu/u/scott/synchronization/pseudocode/duals.html](https://links.jianshu.com/go?to=http%3A%2F%2Fwww.cs.rochester.edu%2Fu%2Fscott%2Fsynchronization%2Fpseudocode%2Fduals.html)。两个类的性能差不多，FIFO通常用于在竞争下支持更高的吞吐量，而LIFO在一般的应用中保证更高的线程局部性。

队列（或者栈）的节点在任何时间要么是"data"模式 —— 通过put操作提供的元素的模式，要么是"request"模式 —— 通过take操作取出元素的模式，要么为空。还有一个模式是"fulfill"模式，当队列有一个data节点时，请求从队列中获取一个元素就会构造一个"fulfill"模式的节点，反之亦然。这个类最有趣的特性在于任何操作都能够计算出现在队列头节点处于什么模式，然后根据它进行操作而无需使用锁。

队列和栈都继承了抽象类`Transferer`，这个类只定义了一个方法`transfer`，此方法可以既可以执行put也可以执行take操作。这两个操作被统一到了一个方法中，因为在`dual`数据结构中，put和take操作是对称的，所以相近的所有结点都可以被结合。使用`transfer`方法是从长远来看的，它相比分为两个几乎重复的部分来说更加容易理解。



## Transferer抽象类

```java
abstract static class Transferer<E> {
    /**
     * 执行put或者take操作/
     * 如果参数e非空，这个元素将被交给一个消费线程；如果为null，
     * 则请求返回一个被生产者提交的元素。
     * 如果返回的结果非空，那么元素被提交了或被接受了；如果为null，
     * 这个操作可能因为超时或者中断失败了。调用者可以通过检查
     * Thread.interrupted来区分到底是因为什么元素失败。
     */
    abstract E transfer(E e, boolean timed, long nanos);
}
```

## TransferStack

### 数据结构定义

```java
static final class TransferStack<E> extends Transferer<E> {
/* Modes for SNodes, ORed together in node fields */
/** 表示一个未满足的消费者 */
static final int REQUEST    = 0;
/** 表示一个未满足的生产者 */
static final int DATA       = 1;
/** Node is fulfilling another unfulfilled DATA or REQUEST */
static final int FULFILLING = 2;

static boolean isFulfilling(int m) { return (m & FULFILLING) != 0; }

/** Node class for TransferStacks. */
static final class SNode {
    volatile SNode next;        // 栈中的下一个结点
    volatile SNode match;       // 匹配此结点的结点
    volatile Thread waiter;     // 控制 park/unpark
    Object item;                // 数据
    int mode;//结点模式
```
### Stack的核心方法

使用put操作时参数e不为空，而使用take操作时参数e为null，而`timed`和`nanos`指定是否使用超时。

**此方法主要有三个 *Action***

* 当栈为空或者， 栈顶有模式相同的元素：则直接入栈顶
* 当 栈顶存在不同模式的元素，则 尝试 *fullFiller* 待插入的元素，入栈顶，并尝试匹配下一个结点 一起出栈
* 当栈顶 存在 *fullfiller* 元素 则帮助其 出栈

```java
E transfer(E e, boolean timed, long nanos) {
    /*
     * 基础算法，循环尝试下面三种操作中的一个：
     *
     * 1. 如果头节点为空或者已经包含了相同模式的结点，尝试将结点
     *    增加到栈中并且等待匹配。如果被取消，返回null
     *
     * 2. 如果头节点是一个模式不同的结点，尝试将一个`fulfilling`结点加入
     *    到栈中，匹配相应的等待结点，然后一起从栈中弹出，
     *    并且返回匹配的元素。匹配和弹出操作可能无法进行，
     *    由于其他线程正在执行操作3
     *
     * 3. 如果栈顶已经有了一个`fulfilling`结点，帮助它完成
     *    它的匹配和弹出操作，然后继续。
     */

    SNode s = null; // constructed/reused as needed
    // 传入参数为null代表请求获取一个元素，否则表示插入元素
    int mode = (e == null) ? REQUEST : DATA;

    for (;;) {
        SNode h = head;
        // 如果头节点为空或者和当前模式相同
        if (h == null || h.mode == mode) {  // empty or same-mode
            // 设置超时时间为 0，立刻返回
            if (timed && nanos <= 0L) {     // can't wait
                if (h != null && h.isCancelled())
                    casHead(h, h.next);     // pop cancelled node
                else
                    return null;
            // 构造一个结点并且设为头节点
            } else if (casHead(h, s = snode(s, e, h, mode))) {
                // 等待满足
                SNode m = awaitFulfill(s, timed, nanos);
                if (m == s) {               // wait was cancelled
                    clean(s);
                    return null;
                }
                if ((h = head) != null && h.next == s)
                    casHead(h, s.next);     // help s's fulfiller
                return (E) ((mode == REQUEST) ? m.item : s.item);
            }
        // 检查头节点是否为FULFILLIING
        } else if (!isFulfilling(h.mode)) { // try to fulfill
            if (h.isCancelled())            // already cancelled
                casHead(h, h.next);         // pop and retry
            // 更新头节点为自己
            else if (casHead(h, s=snode(s, e, h, FULFILLING|mode))) {
                // 循环直到匹配成功
                for (;;) { // loop until matched or waiters disappear
                    SNode m = s.next;       // m is s's match
                    if (m == null) {        // all waiters are gone
                        casHead(s, null);   // pop fulfill node
                        s = null;           // use new node next time
                        break;              // restart main loop
                    }
                    SNode mn = m.next;
                    if (m.tryMatch(s)) {
                        casHead(s, mn);     // pop both s and m
                        return (E) ((mode == REQUEST) ? m.item : s.item);
                    } else                  // lost match
                        s.casNext(m, mn);   // help unlink
                }
            }
        // 帮助满足的结点匹配
        } else {                            // help a fulfiller
            SNode m = h.next;               // m is h's match
            if (m == null)                  // waiter is gone
                casHead(h, null);           // pop fulfilling node
            else {
                SNode mn = m.next;
                if (m.tryMatch(h))          // help match
                    casHead(h, mn);         // pop both h and m
                else                        // lost match
                    h.casNext(m, mn);       // help unlink
            }
        }
    }
}
```

### 同模式自旋等待

>  遇到同模式的结点后，入栈自旋一段时间后等待

```java
SNode awaitFulfill(SNode s, boolean timed, long nanos) {
    final long deadline = timed ? System.nanoTime() + nanos : 0L;
    Thread w = Thread.currentThread();
    int spins = (shouldSpin(s) ?
                 (timed ? maxTimedSpins : maxUntimedSpins) : 0); //计算需要自旋的次数
    for (;;) {
        if (w.isInterrupted()) //中断后取消自己
            s.tryCancel();
        SNode m = s.match;
        if (m != null) // 被唤醒之后，匹配不为空，说明匹配成功
            return m;
        if (timed) {
            nanos = deadline - System.nanoTime();
            if (nanos <= 0L) {
                s.tryCancel(); //超时后取消自己
                continue;
            }
        }
        if (spins > 0)
            spins = shouldSpin(s) ? (spins-1) : 0;//当自己是头结点，或者已经是fullFiller了，保持自旋
        else if (s.waiter == null) //自旋完毕，开始等待
            s.waiter = w; // establish waiter so can park next iter
        else if (!timed)
            LockSupport.park(this);
        else if (nanos > spinForTimeoutThreshold)
            LockSupport.parkNanos(this, nanos);
    }
}
```

### **示意图**

![img](\images\synchronous_queue_stack_example.png)

## TransferQueue

### 数据结构定义

```java
static final class TransferQueue<E> extends Transferer<E> {
    /** Head of queue */
    transient volatile QNode head; //队列头结点
    /** Tail of queue */
    transient volatile QNode tail; //队列尾结点
    /**
         * Reference to a cancelled node that might not yet have been
         * unlinked from queue because it was the last inserted node
         * when it was cancelled.
         */
    transient volatile QNode cleanMe;
    /** Node class for TransferQueue. */
    static final class QNode {
        volatile QNode next;          // next node in queue 下节点
        volatile Object item;         // CAS'ed to or from null，数据域
        volatile Thread waiter;       // to control park/unpark
        final boolean isData; //是否包含数
```

### 核心方法

* 如果 队列为空 或者 与队尾模式相同，则入队尾 自旋等待
* 如果  与队头模式匹配成功，则尝试 队头出队列，并唤醒匹配的等待结点

```java
E transfer(E e, boolean timed, long nanos) {
    /* Basic algorithm is to loop trying to take either of
     * two actions:
     *
     * 1. If queue apparently empty or holding same-mode nodes,
     *    try to add node to queue of waiters, wait to be
     *    fulfilled (or cancelled) and return matching item.
     *
     * 2. If queue apparently contains waiting items, and this
     *    call is of complementary mode, try to fulfill by CAS'ing
     *    item field of waiting node and dequeuing it, and then
     *    returning matching item.
     *
     * In each case, along the way, check for and try to help
     * advance head and tail on behalf of other stalled/slow
     * threads.
     *
     * The loop starts off with a null check guarding against
     * seeing uninitialized head or tail values. This never
     * happens in current SynchronousQueue, but could if
     * callers held non-volatile/final ref to the
     * transferer. The check is here anyway because it places
     * null checks at top of loop, which is usually faster
     * than having them implicitly interspersed.
     */

    QNode s = null; // constructed/reused as needed
    boolean isData = (e != null);

    for (;;) {
        QNode t = tail;
        QNode h = head;
        if (t == null || h == null)         // saw uninitialized value
            continue;                       // spin

        // 如果队列为空或者模式与头节点相同
        if (h == t || t.isData == isData) { // empty or same-mode
            QNode tn = t.next;
            // 如果有其他线程修改了tail，进入下一循环重读
            if (t != tail)                  // inconsistent read    还未开始竞争，就输了：已经将tail更新好了，则退出，开始下一轮
                continue;
            // 如果有其他线程修改了tail，尝试cas更新尾节点，进入下一循环重读
            if (tn != null) {               // lagging tail   还未开始竞争，就输了：已经更新了next，还未开始更新tail引用，则帮忙更新下，然后退出开始下一轮
                advanceTail(t, tn);
                continue;
            }
            // 超时返回
            if (timed && nanos <= 0L)       // can't wait
                return null;
            // 构建一个新节点
            if (s == null)
                s = new QNode(e, isData);
            // 尝试CAS设置尾节点的next字段指向自己
            // 如果失败，重试
            if (!t.casNext(null, s))        // failed to link in，正式开始竞争，竞争失败，则退出
                continue;
      
            // cas设置当前节点为尾节点
            advanceTail(t, s);              // swing tail and wait  竞争成功设置 尾结点
            // 等待匹配的节点
            Object x = awaitFulfill(s, e, timed, nanos); //自旋等待
            // 如果被取消，删除自己，返回null
            if (x == s) {                   // wait was cancelled 由于中断被唤醒了，则取消
                clean(t, s);
                return null;
            }

            // 如果此节点没有被模式匹配的线程出队:即将 头节点 从上一个指向自己，
            // 那么自己进行出队操作
            if (!s.isOffList()) {           // not already unlinked  唤醒自己的结点还没来得扫尾，则自己开始扫尾
                advanceHead(t, s);          // unlink if head 此时头节点理应是t， 更新为s 指向自己，即出队列，此出发生在 队列已经成功匹配，且已经唤醒了配对的线程，也就是当前线程，但还未来得及 出队列故 出队列，小优化：不是必要
                if (x != null)              // and forget fields：取消对 item的引用，加快GC
                    s.item = s;
                s.waiter = null;//取消对 线程对象的引用，加快GC
            }
            return (x != null) ? (E)x : e;

        } else {                            // complementary-mode
            QNode m = h.next;               // node to fulfill
            // 数据不一致，重读
            if (t != tail || m == null || h != head)
                continue;                   // inconsistent read

            Object x = m.item;
            if (isData == (x != null) ||    // m already fulfilled     m已经匹配成功了
                x == m ||                   // m cancelled             m被取消了
                !m.casItem(x, e)) {         // lost CAS                CAS竞争失败
                // 上面三个条件无论哪一个满足，都证明m已经失效无用了，
                // 需要将其出队
                advanceHead(h, m);          // dequeue and retry
                continue;
            }

            // 成功匹配，依然需要将节点出队
            advanceHead(h, m);              // successfully fulfilled
            // 唤醒匹配节点，如果它被阻塞了
            LockSupport.unpark(m.waiter);
            return (x != null) ? (E)x : e;
        }
    }
}

Object awaitFulfill(QNode s, E e, boolean timed, long nanos) {
    /* Same idea as TransferStack.awaitFulfill */
    final long deadline = timed ? System.nanoTime() + nanos : 0L;
    Thread w = Thread.currentThread();
    int spins = (head.next == s)
        ? (timed ? MAX_TIMED_SPINS : MAX_UNTIMED_SPINS)
        : 0;
    for (;;) {
        if (w.isInterrupted())
            s.tryCancel(e);
        Object x = s.item;
        // item被修改后返回
        // 如果put操作在此等待，item会被更新为null
        // 如果take操作再次等待，item会由null变为一个值
        if (x != e)
            return x;
        if (timed) {
            nanos = deadline - System.nanoTime();
            if (nanos <= 0L) {
                s.tryCancel(e);
                continue;
            }
        }
        if (spins > 0) {
            --spins;
            Thread.onSpinWait();
        }
        else if (s.waiter == null)
            s.waiter = w;
        else if (!timed)
            LockSupport.park(this);
        else if (nanos > SPIN_FOR_TIMEOUT_THRESHOLD)
            LockSupport.parkNanos(this, nanos);
    }
}
```

### 等待

* 先自旋一段时间判断 是否已经匹配，匹配成功 则 返回
* 否则 超时等待
* 如果被打断 则取消自身结点

```java
Object awaitFulfill(QNode s, E e, boolean timed, long nanos) {
    /* Same idea as TransferStack.awaitFulfill */
    final long deadline = timed ? System.nanoTime() + nanos : 0L;
    Thread w = Thread.currentThread();
    int spins = ((head.next == s) ?
                 (timed ? maxTimedSpins : maxUntimedSpins) : 0);
    for (;;) {
        if (w.isInterrupted())
            s.tryCancel(e);
        Object x = s.item;
        if (x != e)
            return x;
        if (timed) {
            nanos = deadline - System.nanoTime();
            if (nanos <= 0L) {
                s.tryCancel(e);
                continue;
            }
        }
        if (spins > 0)
            --spins;
        else if (s.waiter == null)
            s.waiter = w;
        else if (!timed)
            LockSupport.park(this);
        else if (nanos > spinForTimeoutThreshold)
            LockSupport.parkNanos(this, nanos);
    }
}
```

### **示意图**

**PUT -> PUT -> TAKE**

![img](\images\synchronous_queue_queue_example_put_take.png)

**TAKE -> TAKE -> PUT**

![img](\images\synchronous_queue_queue_example_take_put.png)



## 公共方法

### 初始化

* 公平、非公平

```java
public SynchronousQueue(boolean fair) {
    transferer = fair ? new TransferQueue<E>() : new TransferStack<E>();
}
```

### Queue的方法

```java
//非阻塞 取数据，失败 返回*NULL*,成功 返回数据
public E poll() {
    return transferer.transfer(null, true, 0);
}
```

```java
//同步队列 不存放任何数据，所以返回NULL
public E peek() {
    return null;
}
```

### BlockingQueued的方法

**PUT**

* 不允许空
* 阻塞获取数据 直到数据返回
* 如果返回*NULL* 则表明是被中断了，则手动中断

```java
public void put(E e) throws InterruptedException {
    if (e == null) throw new NullPointerException();
    if (transferer.transfer(e, false, 0) == null) {
        Thread.interrupted();
        throw new InterruptedException();
    }
}
```

**Offer**

* 超时等待
* 超时则返回false
* 中断抛异常

```java
public boolean offer(E e, long timeout, TimeUnit unit)
    throws InterruptedException {
    if (e == null) throw new NullPointerException();
    if (transferer.transfer(e, true, unit.toNanos(timeout)) != null)
        return true;
    if (!Thread.interrupted())
        return false;
    throw new InterruptedException();
}
```

**Take**

* 阻塞获取数据
* 为空则认为是中断，抛异常

```java
public E take() throws InterruptedException {
    E e = transferer.transfer(null, false, 0);
    if (e != null)
        return e;
    Thread.interrupted();
    throw new InterruptedException();
}
```

**Poll**

* 超时取数据
* 获取数据成功或者超时 则返回 e
* 否则 抛异常

```java
public E poll(long timeout, TimeUnit unit) throws InterruptedException {
    E e = transferer.transfer(null, true, unit.toNanos(timeout));
    if (e != null || !Thread.interrupted())
        return e;
    throw new InterruptedException();
}
```

## DrainTo

* 非阻塞循环 快速取数据
* 能取多少是多少，直到无法取到

```java
public int drainTo(Collection<? super E> c) {
    if (c == null)
        throw new NullPointerException();
    if (c == this)
        throw new IllegalArgumentException();
    int n = 0;
    for (E e; (e = poll()) != null;) {
        c.add(e);
        ++n;
    }
    return n;
}
```



# 总结

## SynchronousQueue的实现原理是怎样的？

* 使用栈 或者队列 实现 公平与非公平
* 利用 模式匹配 统一 存或取操作，如果队尾 或 栈顶 是 同种 模式则 自旋一定次数进入等待，如果不是同种模式则 匹配成功
    * 如果是队列 则 直接将队头结点 出队列，并唤醒等待在该结点的 线程
    * 如果是栈，则入栈一个 匹配结点，然后将两个结点 出栈



## TransferQueue与TransferStack的异同

### 核心不同点

* 一个FIFO，一个FILO 

### **共同点**

* 都使用 *NULL* 值返回 表明 没有取到数据
* 都使用 匹配 模式，实现1对1的存取
* 都通过 将指针 指向自己  表明 结点的取消状态

### 不同处

* 队列实际匹配时，不会将待匹配的结点入队列，而是直接 将已匹配的结点出队列
* 栈 在 模式匹配时，会将两个结点都入栈



## TransferStack与Queue如何 减少多线程间的竞争

* 通过设置 标记位 通知正 在自旋的线程已 完成匹配
* *Stack*：当检测到 其他结点 在 进行匹配操作时 会 其他线程会帮助 匹配的那对元素 出栈 后 在进行自己的 入栈
* Queue：多个线程在队头 竞争一个资源
    * 还未开始竞争，其他线程已经 到手了，则默默退出
    * 竞争失败后，会默默帮对方做 好善后工作：例如更新 队头结点
    * 竞争成功后，唤醒等待在该结点的线程