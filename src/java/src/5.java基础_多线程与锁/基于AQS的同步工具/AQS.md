# AQS框架

## AQS数据结构图

![](../../../images/aqs_clh_queen.png)

它维护了一个volatile int state（代表共享资源）和一个FIFO线程等待队列（多线程争用资源被阻塞时会进入此队列）。

## 资源访问的两种方式

**AQS定义两种资源共享方式**：

* Exclusive（独占，只有一个线程能执行，如ReentrantLock）和

* Share（共享，多个线程可同时执行，如Semaphore/CountDownLatch）。

**自定义同步器实现**

> 不同的自定义同步器争用共享资源的方式也不同。**自定义同步器在实现时只需要实现共享资源state的获取与释放方式即可**, 至于具体线程等待队列的维护（ 如获取资源失败入队/唤醒出队等），AQS已经在顶层实现好了。自定义同步器实现时主要实现以下几种方法：

- isHeldExclusively()：该线程是否正在独占资源。只有用到condition才需要去实现它。
- tryAcquire(int)：独占方式。尝试获取资源，成功则返回true，失败则返回false。
- tryRelease(int)：独占方式。尝试释放资源，成功则返回true，失败则返回false。
- tryAcquireShared(int)：共享方式。尝试获取资源。负数表示失败；0表示成功，但没有剩余可用资源；正数表示成功，且有剩余资源。
- tryReleaseShared(int)：共享方式。尝试释放资源，如果释放后允许唤醒后续等待结点返回true，否则返回false。

一般来说，自定义同步器要么是独占方法，要么是共享方式，他们也只需实现tryAcquire-tryRelease、tryAcquireShared-tryReleaseShared中的一种即可。但AQS也支持自定义同步器同时实现独占和共享两种方式，如ReentrantReadWriteLock。

**示例**

**ReentrantLock**

```
state初始化为0，表示未锁定状态。A线程lock()时，会调用tryAcquire()独占该锁并将state+1。此后，其他线程再tryAcquire()时就会失败，直到A线程unlock()到state=0（即释放锁）为止，其它线程才有机会获取该锁。当然，释放锁之前，A线程自己是可以重复获取此锁的（state会累加），这就是可重入的概念。但要注意，获取多少次就要释放多么次，这样才能保证state是能回到零态的。
```

**CountDownLatch**	

```
任务分为N个子线程去执行，state也初始化为N（注意N要与线程个数一致）。这N个子线程是并行执行的，每个子线程执行完后countDown()一次，state会CAS减1。等到所有子线程都执行完后(即state=0)，会unpark()主调用线程，然后主调用线程就会从await()函数返回，继续后余动作。
```





# 源码解析

## 结点状态

> Node结点是对每一个等待获取资源的线程的封装，其包含了需要同步的线程本身及其等待状态，如是否被阻塞、是否等待唤醒、是否已经被取消等。

变量 *waitStatus* 则表示当前Node结点的等待状态，共有5种取值CANCELLED、SIGNAL、CONDITION、PROPAGATE、0。

- **CANCELLED**(1)：表示当前结点已取消调度。当timeout或被中断（响应中断的情况下），会触发变更为此状态，进入该状态后的结点将不会再变化。
- **SIGNAL**(-1)：表示后继结点在等待当前结点唤醒。后继结点入队时，会将前继结点的状态更新为SIGNAL。
- **CONDITION**(-2)：表示结点等待在Condition上，当其他线程调用了Condition的signal()方法后，CONDITION状态的结点将**从等待队列转移到同步队列中**，等待获取同步锁。
- **PROPAGATE**(-3)：共享模式下，前继结点不仅会唤醒其后继结点，同时也可能会唤醒后继的后继结点。
- **0**：新结点入队时的默认状态。

注意，**负值表示结点处于有效等待状态，而正值表示结点已被取消。所以源码中很多地方用>0、<0来判断结点的状态是否正常**。

## 获取独占锁入口

### 方法体

> *acquire* 方法是在独占模式下线程获取共享资源的顶层入口。如果获取到资源，线程直接返回，否则进入等待队列，直到获取到资源为止，且整个过程忽略中断的影响。这也正是lock()的语义，当然不仅仅只限于lock()。获取到资源后，线程就可以去执行其临界区代码了。下面是acquire()的源码：

```java
    public final void acquire(int arg) {
        if (!tryAcquire(arg) &&
            acquireQueued(addWaiter(Node.EXCLUSIVE), arg))
            selfInterrupt();
    }
```

函数流程如下：

1. *tryAcquire()* 尝试直接去获取资源，如果成功则直接返回（这里体现了**非公平锁**，每个线程获取锁时会尝试直接抢占加塞一次，而CLH队列中可能还有别的线程在等待）；
2. *addWaiter()* 将该线程加入等待队列的尾部，并标记为独占模式；
3. *acquireQueued()* 使线程阻塞在等待队列中获取资源，一直获取到资源后才返回。如果在整个等待过程中被中断过，则返回true，否则返回false。
4. 如果线程在等待过程中被中断过，它是不响应的。只是获取资源后才再进行自我中断selfInterrupt()，将中断补上。

### *tryAcquire(int)*

* 此方法尝试去获取独占资源。如果获取成功，则直接返回true，否则直接返回false

* 具体资源的获取交由自定义同步器去实现了（通过state的get/set/CAS）至于能不能重入，能不能加塞，交由实现决定
* 之所以没有定义成abstract，是因为独占模式下只用实现tryAcquire-tryRelease，而共享模式下只用实现tryAcquireShared-tryReleaseShared

### *addWaiter(Node)*

> 此方法用于将当前线程加入到等待队列的队尾，并返回当前线程所在的结点

```java
private Node addWaiter(Node mode) {
    //以给定模式构造结点。mode有两种：EXCLUSIVE（独占）和SHARED（共享）
    Node node = new Node(Thread.currentThread(), mode);

    //尝试快速方式直接放到队尾。
    Node pred = tail;
    if (pred != null) {
        node.prev = pred;
        if (compareAndSetTail(pred, node)) {
            pred.next = node;
            return node;
        }
    }

    //上一步失败则通过enq入队。
    enq(node);
    return node;
}
//循环取 tail，设置值
    private Node enq(final Node node) {
        for (;;) {
            Node t = tail;
            if (t == null) { // Must initialize
                if (compareAndSetHead(new Node()))
                    tail = head;
            } else {
                node.prev = t;
                if (compareAndSetTail(t, node)) {
                    t.next = node;
                    return t;
                }
            }
        }
    }
```

### *acquireQueued(Node, int)*

> 获取锁失败，入队列，进入等待状态休息，直到其他线程彻底释放资源后唤醒自己

```java
    final boolean acquireQueued(final Node node, int arg) {
        boolean failed = true;
        try {
            boolean interrupted = false;
            for (;;) {//自旋
                final Node p = node.predecessor();//拿到前驱
                //如果前驱是head，即该结点已成老二，那么便有资格去尝试获取资源（可能是老大释放完资源唤醒自己的，当然也可能被interrupt了）。
                if (p == head && tryAcquire(arg)) {
                    setHead(node); //拿到资源后，将head指向该结点。所以head所指的结点，就是当前获取到资源的那个结点或null。
                    p.next = null; // help GC，setHead中node.prev已置为null，此处再将head.next置为null，就是为了方便GC回收以前的head结点。也就意味着之前拿完资源的结点出队了！
                    failed = false; //成功获取资源标识
                    return interrupted; //返回等待过程中是否被中断过
                }
                //如果自己可以休息了，就通过park()进入waiting状态，直到被unpark()。如果不可中断的情况下被中断了，那么会从park()中醒过来，发现拿不到资源，从而继续进入park()等待。
                if (shouldParkAfterFailedAcquire(p, node) &&
                    parkAndCheckInterrupt())
                    interrupted = true;
            }
        } finally {
            if (failed) // 如果等待过程中没有成功获取资源（如timeout，或者可中断的情况下被中断了），那么取消结点在队列中的等待。
                cancelAcquire(node);
        }
    }
```



### *shouldParkAfterFailedAcquire*

> 此方法主要用于检查状态，看看自己是否真的可以去休息了，要是 队列前边的线程都放弃了 那么当前线程可以尝试 竞争下

**如果前驱结点的状态不是SIGNAL，那么自己就不能安心去休息，需要去找个安心的休息点，同时可以再尝试下看有没有机会轮到自己拿号。**

```java
private static boolean shouldParkAfterFailedAcquire(Node pred, Node node) {
    int ws = pred.waitStatus;//拿到前驱的状态
    if (ws == Node.SIGNAL)
        //如果已经告诉前驱拿完号后通知自己一下，那就可以安心休息了
        return true;
    if (ws > 0) {
        /*
         * 如果前驱放弃了，那就一直往前找，直到找到最近一个正常等待的状态，并排在它的后边。
         * 注意：那些放弃的结点，由于被自己“加塞”到它们前边，它们相当于形成一个无引用链，稍后就会被保安大叔赶走了(GC回收)！
         * 回收已经处于  CANCELLED 状态的 等待线程结点
         */
        do {
            node.prev = pred = pred.prev;
        } while (pred.waitStatus > 0);
        pred.next = node;
    } else {
         //如果前驱正常，那就把前驱的状态设置成SIGNAL，告诉它拿完号后通知自己一下。有可能失败，可能前驱刚刚释放锁完毕
        compareAndSetWaitStatus(pred, ws, Node.SIGNAL);
    }
    return false;
}
```

### *parkAndCheckInterrupt()*

> 如果线程找好安全休息点后，那就可以安心去休息了。此方法就是让线程去休息，真正进入等待状态。

```java
private final boolean parkAndCheckInterrupt() {
     LockSupport.park(this);//调用park()使线程进入waiting状态
     return Thread.interrupted();//如果被唤醒，查看自己是不是被中断的。
}
```

**park()会让当前线程进入waiting状态。在此状态下，有两种途径可以唤醒该线程：1）被unpark()；2）被interrupt()。**



### 小结

#### 入队列流程

1. 结点进入队尾后，检查状态，找到安全休息点；
2. 调用park()进入waiting状态，等待unpark()或interrupt()唤醒自己；
3. 被唤醒后，看自己是不是有资格能拿到号。如果拿到，head指向当前结点，并返回从入队到拿到号的整个过程中是否被中断过；如果没拿到，继续流程1。

#### **获取独占锁总体流程**

* 调用自定义同步器的tryAcquire()尝试直接去获取资源，如果成功则直接返回；
* 没成功，则addWaiter()将该线程加入等待队列的尾部，并标记为独占模式；
* acquireQueued()使线程在等待队列中休息，有机会时（轮到自己，会被unpark()）会去尝试获取资源。获取到资源后才返回。
* 如果在整个等待过程中被中断过，它是不响应的。只是获取资源后才再进行自我中断selfInterrupt()，将中断补上。

**流程图**

![](../../../images/aqs_clh_acquire.png)

## 释放独占锁

> 此方法是独占模式下线程释放共享资源的顶层入口。它会释放指定量的资源，如果彻底释放了（即state=0）,它会唤醒等待队列里的其他线程来获取资源。

```java
public final boolean release(int arg) {
    if (tryRelease(arg)) {
        Node h = head;//找到头结点
        if (h != null && h.waitStatus != 0)
            unparkSuccessor(h);//唤醒等待队列里的下一个线程
        return true;
    }
    return false;
}
```

### *unparkSuccessor*

> 此方法用于唤醒等待队列中下一个线程,用unpark()唤醒等待队列中最前边的那个未放弃线程

```java
private void unparkSuccessor(Node node) {
    //这里，node一般为当前线程所在的结点。
    int ws = node.waitStatus;
    if (ws < 0)//置零当前线程所在的结点状态，允许失败。
        compareAndSetWaitStatus(node, ws, 0);

    Node s = node.next;//找到下一个需要唤醒的结点s
    if (s == null || s.waitStatus > 0) {//如果为空或已取消
        s = null;
        for (Node t = tail; t != null && t != node; t = t.prev) // 从后向前找。
            if (t.waitStatus <= 0)//从这里可以看出，<=0的结点，都是还有效的结点。
                s = t;
    }
    if (s != null)
        LockSupport.unpark(s.thread);//唤醒
}
```



## 取消结点

> 因为中断和超时导致的 结点取消

### Code

```java
private void cancelAcquire(Node node) {
    // Ignore if node doesn't exist
    if (node == null)
        return;

    node.thread = null;

    // Skip cancelled predecessors
    Node pred = node.prev;
    while (pred.waitStatus > 0)
        node.prev = pred = pred.prev;

    // predNext is the apparent node to unsplice. CASes below will
    // fail if not, in which case, we lost race vs another cancel
    // or signal, so no further action is necessary.
    Node predNext = pred.next;

    // Can use unconditional write instead of CAS here.
    // After this atomic step, other Nodes can skip past us.
    // Before, we are free of interference from other threads.
    node.waitStatus = Node.CANCELLED;

    // If we are the tail, remove ourselves.
    if (node == tail && compareAndSetTail(node, pred)) {
        compareAndSetNext(pred, predNext, null);
    } else {
        // If successor needs signal, try to set pred's next-link
        // so it will get one. Otherwise wake it up to propagate.
        int ws;
        if (pred != head &&
            ((ws = pred.waitStatus) == Node.SIGNAL ||
             (ws <= 0 && compareAndSetWaitStatus(pred, ws, Node.SIGNAL))) &&
            pred.thread != null) {
            Node next = node.next;
            if (next != null && next.waitStatus <= 0)
                compareAndSetNext(pred, predNext, next);
        } else {
            unparkSuccessor(node);
        }

        node.next = node; // help GC
    }
}
```



### **流程图**

![](../../../images/aqs_canceled.png)



### **无效结点的移除逻辑**

**分两步**

* 第一步 将 当前结点 前驱的 后继 指向 当前结点的后继

**伪代码**

```c
Node prev = current.prev;
Node next = current.next;
prev.next = next;
```

这一步可能会产生冲突

如果前驱被取消，则唤醒后继

如果后继被取消，不做任何处理



* 第二步 每个结点各自 维护自己 的*prev* 指针

**伪代码**

**在调用取消结点时**

```java
private void cancelAcquire(Node node){
    ...
    Node pred = node.prev;
	while (pred.waitStatus > 0)
		node.prev = pred = pred.prev;
    ...
}

```

**结点入队列 结点被唤醒时，且没有获取到锁**

```java
//判断是否应该阻塞
shouldParkAfterFailedAcquire(){
    ...
    do {
	node.prev = pred = pred.prev;
	} while (pred.waitStatus > 0);
	pred.next = node;
    ...
}
//入队列
acquireQueued(){
    ...
    for (;;) {
        final Node p = node.predecessor();
        if (p == head && tryAcquire(arg)) {
            setHead(node);
            p.next = null; // help GC
            failed = false;
            return interrupted;
        }
        if (shouldParkAfterFailedAcquire(p, node) &&
            parkAndCheckInterrupt())
            interrupted = true;
        }
    ...
}

```





## 获取共享锁入口

> 它会获取指定量的资源，获取成功则直接返回，获取失败则进入等待队列，直到获取到资源为止.整个过程忽略中断

```java
public final void acquireShared(int arg) {
    if (tryAcquireShared(arg) < 0)
        doAcquireShared(arg);
}
```

### *tryAcquireShared*

* 该方法由子类实现
* **获取指定量的共享锁**

* 返回值负值代表获取失败；0代表获取成功，但没有剩余资源；正数表示获取成功，还有剩余资源

### *doAcquireShared*

```java
private void doAcquireShared(int arg) {
    final Node node = addWaiter(Node.SHARED); //加入队列尾部
    boolean failed = true; //是否成功标志
    try {
        boolean interrupted = false; //等待过程中是否被中断过的标志
        for (;;) {
            final Node p = node.predecessor(); //前驱
            if (p == head) { //如果到head的下一个，因为head是拿到资源的线程，此时node被唤醒，很可能是head用完资源来唤醒自己的
                int r = tryAcquireShared(arg);  //尝试获取资源
                if (r >= 0) { //获取资源成功
                    setHeadAndPropagate(node, r);  //将head指向自己，还有剩余资源可以再唤醒之后的线程
                    p.next = null; // help GC
                    if (interrupted) //如果等待过程中被打断过，此时将中断补上。
                        selfInterrupt();
                    failed = false;
                    return;
                }
            }
            //判断状态，寻找安全点，进入waiting状态，等着被unpark()或interrupt()
            if (shouldParkAfterFailedAcquire(p, node) &&
                parkAndCheckInterrupt())
                interrupted = true;
        }
    } finally {
        if (failed)
            cancelAcquire(node);
    }
}
```

### *setHeadAndPropagate*

```java
private void setHeadAndPropagate(Node node, int propagate) {
    Node h = head; // Record old head for check below,记录唤醒当前结点的前驱结点，也就是头结点
    setHead(node);
    /*
     * Try to signal next queued node if:
     *   Propagation was indicated by caller,
     *     or was recorded (as h.waitStatus either before
     *     or after setHead) by a previous operation
     *     (note: this uses sign-check of waitStatus because
     *      PROPAGATE status may transition to SIGNAL.)
     * and
     *   The next node is waiting in shared mode,
     *     or we don't know, because it appears null
     *
     * The conservatism in both of these checks may cause
     * unnecessary wake-ups, but only when there are multiple
     * racing acquires/releases, so most need signals now or soon
     * anyway.
     */
    if (propagate > 0 || h == null || h.waitStatus < 0 ||
        (h = head) == null || h.waitStatus < 0) {
        Node s = node.next;
        if (s == null || s.isShared())
            doReleaseShared();
    }
}
```
* 如果 propagate 大于0，说明资源仍有余量 可以唤醒

* 如果 *propagate = 0*，但 头节点的状态 小于0，说明此时处于 *PROPAGATE*  状态 或者   *SIGNAL* 状态 也可以释放资源

    

## 释放共享锁入口

> 它会释放指定量的资源，如果成功释放且允许唤醒等待线程，它会唤醒等待队列里的其他线程来获取资源

```java
public final boolean releaseShared(int arg) {
    if (tryReleaseShared(arg)) {//尝试释放资源
        doReleaseShared();//唤醒后继结点
        return true;
    }
    return false;
}
```

* 一句话总结：**释放掉资源后，唤醒后继**。

* 跟独占模式下的release()区别：独占模式下的tryRelease()在完全释放掉资源（state=0）后，才会返回true去唤醒其他线程，这主要是基于独占下可重入的考量；
* 例如，资源总量是13，A（5）和B（7）分别获取到资源并发运行，C（4）来时只剩1个资源就需要等待。A在运行过程中释放掉2个资源量，然后tryReleaseShared(2)返回true唤醒C，C一看只有3个仍不够继续等待；随后B又释放2个，tryReleaseShared(2)返回true唤醒C，C一看有5个够自己用了，然后C就可以跟A和B一起运行。而ReentrantReadWriteLock读锁的tryReleaseShared()只有在完全释放掉资源（state=0）才返回true，所以自定义同步器可以根据需要决定tryReleaseShared()的返回值。

### *doReleaseShared*

**分析**

每次循环中重新读取一次head，配合*if(h == head) break;*，循环检测到head没有变化时就会退出循环

head变化一定是因为：acquire thread被唤醒，之后它成功获取锁，然后setHead设置了新head。



```
所以设置这种中间状态的head的status为PROPAGATE，让其status又变成负数，这样可能被 被唤醒线程
（因为正常来讲，被唤醒线程的前驱，也就是head会被设置为0的，所以被唤醒线程发现head不为0，就会知道自己应该去唤醒自己的后继了） 检测到。
如果状态为PROPAGATE，直接判断head是否变化。
两个continue保证了进入那两个分支后，只有当CAS操作成功后，才可能去执行if(h == head) break;，才可能退出循环。
if(h == head) break;保证了，只要在某个循环的过程中有线程刚获取了锁且设置了新head，就会再次循环。目的当然是为了再次执行unparkSuccessor(h)，即唤醒队列中第一个等待的线程。
```

**代码**

* 如果头节点为 SIGNAL，说明后继结点在等候，则先将自身结点置为0，然后唤醒后继
* 如果头结点为 0，说明h的后继所代表的线程已经被唤醒或即将被唤醒，这种状态是一个中间状态，



```java
private void doReleaseShared() {
    for (;;) {
        Node h = head; //循环中重新读取一次head
        if (h != null && h != tail) { //判断队列是否至少有两个node，如果队列从来没有初始化过（head为null），或者head就是tail，那么中间逻辑直接不走
            int ws = h.waitStatus;
            if (ws == Node.SIGNAL) { //如果状态为SIGNAL,说明h的后继是需要被通知的
                if (!compareAndSetWaitStatus(h, Node.SIGNAL, 0))//只要head成功得从SIGNAL修改为0，那么head的后继的代表线程肯定会被唤醒了。
                    continue;
                unparkSuccessor(h);//唤醒后继
            }
            // 如果状态为0，说明h的后继所代表的线程已经被唤醒或即将被唤醒，并且这个中间状态即将消失,要么由于acquire thread获取锁失败再次设置head为 SIGNAL并再次阻塞,要么由于acquire thread获取锁成功而将自己（head后继）设置为新head并且只要head后继不是队尾，那么新head肯定为SIGNAL。
            else if (ws == 0 &&
                     !compareAndSetWaitStatus(h, 0, Node.PROPAGATE))
                continue;
        }
        if (h == head)// head发生变化
           break;
    }
}
```





# 个人理解

## AQS为什么要用双向队列？

* 如果 不需要支持 中断或超时，则可以使用 单向队列，所有线程入队列之后，只可能会被阻塞直到获取锁时被唤醒

* 但如果需要支持中断或超时，则会造成链条中 出现许多 无效结点，如果使用单向链表，很难凭借无锁设计 实现原子的 结点的移除，而且被取消的结点 查找自己的前驱比较 麻烦
* **AQS 使用双向队列 处理 无效结点的移除**  
* **移除无效结点** 重要原则：断开其他结点对当前结点的引用 ： 前驱的引用，断开后继的引用, 而 prev结点由各个结点各自维护

**使用双向队列的优点**

**减少数据竞争** 取消结点时，在 维护 前驱的 next引用时，不影响 prev 引用的 使用，在决定下一个被唤醒者时， 只将 next结点作为一种优化路径，next为空或者 已取消 则使用prev结点查找 下一个候选者。



## AQS的核心

> AQS的核心在于 如何 原子的或者 保证队列结构安全的情况 下  将  取消的 结点 及时的 移出队列



## AQS共享锁工作机制

* 成功获取 共享锁 时候，会尝试 唤醒其他线程来 抢占资源
* 释放共享锁的时候 尝试唤醒其他线程来抢占资源

