# 概述

ConcurrentHashMap是conccurrent家族中的一个类，由于它可以高效地支持并发操作，以及被广泛使用，经典的开源框架[spring](http://lib.csdn.net/base/javaee)的底层[数据结构](http://lib.csdn.net/base/datastructure)就是使用ConcurrentHashMap实现的。与同是线程安全的老大哥HashTable相比，它已经更胜一筹，因此它的锁更加细化，而不是像HashTable一样为几乎每个方法都添加了synchronized锁，这样的锁无疑会影响到性能。





# 重要的属性

## sizeCtl

**申明**

```java
private transient volatile int sizeCtl;
```

- 负数代表正在进行初始化或扩容操作
    - -1代表正在初始化
    - -N 表示有N-1个线程正在进行扩容操作

- 正数或0代表hash表还没有被初始化，这个数值表示初始化或下一次进行扩容的大小，这一点类似于扩容阈值的概念。还后面可以看到，它的值始终是当前ConcurrentHashMap容量的0.75倍，这与loadfactor是对应的。

## table

```java
transient volatile Node<K,V>[] table;  
```

盛装Node元素的数组 它的大小是2的整数次幂



# 使用 *CounterCells* 记录数据容量

* ConcurrentHashMap是采用CounterCell数组来记录元素个数的，像一般的集合记录集合大小，直接定义一个size的成员变量即可，当出现改变的时候只要更新这个变量就行。

**为什么ConcurrentHashMap要用这种形式来处理呢？** 

* 问题还是处在并发上，*ConcurrentHashMap*是并发集合，如果用一个成员变量来统计元素个数的话，为了保证并发情况下共享变量的的安全，势必会需要通过加锁或者自旋来实现
* 如果竞争比较激烈的情况下，size的设置上会出现比较大的冲突反而影响了性能，所以在ConcurrentHashMap采用了分片的方法来记录大小



# 三个核心方法

```java
 @SuppressWarnings("unchecked")
//获得在i位置上的Node节点,保证获取得是最新的改动
static final <K,V> Node<K,V> tabAt(Node<K,V>[] tab, int i) {
    return (Node<K,V>)U.getObjectVolatile(tab, ((long)i << ASHIFT) + ABASE);
}
//利用CAS算法设置i位置上的Node节点。之所以能实现并发是因为他指定了原来这个节点的值是多少
//在CAS算法中，会比较内存中的值与你指定的这个值是否相等，如果相等才接受你的修改，否则拒绝你的修改
//因此当前线程中的值并不是最新的值，这种修改可能会覆盖掉其他线程的修改结果  有点类似于SVN
static final <K,V> boolean casTabAt(Node<K,V>[] tab, int i,
                                    Node<K,V> c, Node<K,V> v) {
    return U.compareAndSwapObject(tab, ((long)i << ASHIFT) + ABASE, c, v);
}
//利用volatile方法设置节点位置的值
static final <K,V> void setTabAt(Node<K,V>[] tab, int i, Node<K,V> v) {
    U.putObjectVolatile(tab, ((long)i << ASHIFT) + ABASE, v);
}
```





# 扩容

## initTable

> 懒加载初始化

CAS 设置  *sizeCtl* 为 -1

* 初始化竞争成功
    * 设置 sizeCtl为-1，表示已占据
    * 新建数组
    * 修改table引用
    * 变更 sizeCtl为 sc，即3/4的当前容量
* 初始化竞争失败（判断 sizeCtl<0）
    * 自旋

```java
private final Node<K,V>[] initTable() {
    Node<K,V>[] tab; int sc;
    while ((tab = table) == null || tab.length == 0) {
        if ((sc = sizeCtl) < 0)
            Thread.yield(); // lost initialization race; just spin
        else if (U.compareAndSwapInt(this, SIZECTL, sc, -1)) {
            try {
                if ((tab = table) == null || tab.length == 0) {
                    int n = (sc > 0) ? sc : DEFAULT_CAPACITY;
                    @SuppressWarnings("unchecked")
                    Node<K,V>[] nt = (Node<K,V>[])new Node<?,?>[n];
                    table = tab = nt;
                    sc = n - (n >>> 2);
                }
            } finally {
                sizeCtl = sc;
            }
            break;
        }
    }
    return tab;
}
```



## addCount

* 此方法 用于维护 map的 size大小
* 二是用于 判断是否需要扩容

```java
private final void addCount(long x, int check) {
	......//舍去size维护 code
    if (check >= 0) {
        Node<K,V>[] tab, nt; int n, sc;
        while (s >= (long)(sc = sizeCtl) && (tab = table) != null &&
               (n = tab.length) < MAXIMUM_CAPACITY) { //如果 当前元素个数 大于 sc,且数组不为0，且数组大小没有超过最大值
            int rs = resizeStamp(n); //取得扩容标志：  16的容量，返回28，且第16位为1，保证 右移16位后为负数，则该值为 65536+28 = 65564
            if (sc < 0) { //sc小于0，表示当前正在扩容
                if ((sc >>> RESIZE_STAMP_SHIFT) != rs || sc == rs + 1 ||
                    sc == rs + MAX_RESIZERS || (nt = nextTable) == null ||
                    transferIndex <= 0)
                    break; //进入此路的情况是：已经完成了扩容，且更新了 table变量，但还未来得及更新 ctl字段
                if (U.compareAndSwapInt(this, SIZECTL, sc, sc + 1)) //加入扩容
                    transfer(tab, nt);
            }
            else if (U.compareAndSwapInt(this, SIZECTL, sc,
                                         (rs << RESIZE_STAMP_SHIFT) + 2)) //还未扩容，则将 sc直接 覆盖为 (rs << RESIZE_STAMP_SHIFT) + 2，+2： 1表示初始化，1表示有一个线程在扩容
                transfer(tab, null);
            s = sumCount(); //更新大小
        }
    }
}
```

## transfer

扩容是ConcurrentHashMap的精华之一，扩容操作的核心在于数据的转移

在单线程环境下数据的转移很简单，无非就是把旧数组中的数据迁移到新的数组。

但是这在多线程环境下，在扩容的时候其他线程也可能正在添加元素，这时又触发了扩容怎么办？

可能大家想到的第一个解决方案是加互斥锁，把转移过程锁住，虽然是可行的解决方案，但是会带来较大的性能开销。

因为互斥锁会导致所有访问临界区的线程陷入到阻塞状态，持有锁的线程耗时越长，其他竞争线程就会一直被阻塞，导致吞吐量较低。

而且还可能导致死锁。 

而ConcurrentHashMap并没有直接加锁，而是采用CAS实现无锁的并发同步策略

最精华的部分是它可以利用多线程来进行协同扩容 简单来说，

* **它把Node数组当作多个线程之间共享的任务队列**

* **然后通过维护一个指针来划分每个线程锁负责的区间**
* **每个线程通过区间逆向遍历来实现扩容**
* 一个已经迁移完的bucket会被替换为一个ForwardingNode节点，标记当前bucket已经被其他线程迁移完了。接下来分析一下它的源码实现

1、fwd:这个类是个标识类，用于指向新表用的，其他线程遇到这个类会主动跳过这个类，因为这个类要么就是扩容迁移正在进行，要么就是已经完成扩容迁移，也就是这个类要保证线程安全，再进行操作。

2、advance:这个变量是用于提示代码是否进行推进处理，也就是当前桶处理完，处理下一个桶的标识

3、finishing:这个变量用于提示扩容是否结束用的

```java
private final void transfer(Node<K,V>[] tab, Node<K,V>[] nextTab) {
        int n = tab.length, stride;
//将 (n>>>3相当于 n/8) 然后除以 CPU核心数。如果得到的结果小于 16，那么就使用 16
 
    // 这里的目的是让每个 CPU 处理的桶一样多，避免出现转移任务不均匀的现象，如果桶较少的话，默认一个 CPU（一个线程）处理 16 个桶，也就是长度为16的时候，扩容的时候只会有一个线程来扩容
    if ((stride = (NCPU > 1) ? (n >>> 3) / NCPU : n) < MIN_TRANSFER_STRIDE)
        stride = MIN_TRANSFER_STRIDE; // subdivide range
    
    //nextTab未初始化，nextTab是用来扩容的node数组
    if (nextTab == null) {            // initiating
        try {
 
            //新建一个n<<1原始table大小的nextTab,也就是32
            @SuppressWarnings("unchecked")
            Node<K,V>[] nt = (Node<K,V>[])new Node<?,?>[n << 1];
 
            //赋值给nextTab
            nextTab = nt;
        } catch (Throwable ex) {      // try to cope with OOME
 
            //扩容失败，sizeCtl使用int的最大值
            sizeCtl = Integer.MAX_VALUE;
            return;
        }
 
        //更新成员变量
        nextTable = nextTab;
 
        //更新转移下标，表示转移时的下标
        transferIndex = n;
    }
 
    //新的tab的长度
    int nextn = nextTab.length;
 
    // 创建一个 fwd 节点，表示一个正在被迁移的Node，并且它的hash值为-1(MOVED)，也就是前面我们在讲putval方法的时候，会有一个判断MOVED的逻辑。它的作用是用来占位，表示原数组中位置i处的节点完成迁移以后，就会在i位置设置一个fwd来告诉其他线程这个位置已经处理过了，具体后续还会在讲
    ForwardingNode<K,V> fwd = new ForwardingNode<K,V>(nextTab);
 
    // 首次推进为 true，如果等于 true，说明需要再次推进一个下标（i--），反之，如果是 false，那么就不能推进下标，需要将当前的下标处理完毕才能继续推进
    boolean advance = true;
 
    //判断是否已经扩容完成，完成就return，退出循环
    boolean finishing = false; // to ensure sweep before committing nextTab
 
    //通过for自循环处理每个槽位中的链表元素，默认advace为真，通过CAS设置transferIndex属性值，并初始化i和bound值，i指当前处理的槽位序号，bound指需要处理的槽位边界，先处理槽位15的节点；
    for (int i = 0, bound = 0;;) {
 
        // 这个循环使用CAS不断尝试为当前线程分配任务
 
        // 直到分配成功或任务队列已经被全部分配完毕
 
        // 如果当前线程已经被分配过bucket区域
 
        // 那么会通过--i指向下一个待处理bucket然后退出该循环
        Node<K,V> f; int fh;
        while (advance) {
            int nextIndex, nextBound;
 
            //--i表示下一个待处理的bucket，如果它>=bound,表示当前线程已经分配过bucket区域
            if (--i >= bound || finishing)
                advance = false;
 
            //表示所有bucket已经被分配完毕 给nextIndex赋予初始值 = 16
            else if ((nextIndex = transferIndex) <= 0) {
                i = -1;
                advance = false;
            }
            //通过cas来修改TRANSFERINDEX,为当前线程分配任务，处理的节点区间为(nextBound,nextIndex)->(0,15)
            else if (U.compareAndSwapInt
                     (this, TRANSFERINDEX, nextIndex,
                      nextBound = (nextIndex > stride ?
                                   nextIndex - stride : 0))) {
 
                //0
                bound = nextBound;
 
                //15
                i = nextIndex - 1;
                advance = false;
            }
        }
 
        //i<0说明已经遍历完旧的数组，也就是当前线程已经处理完所有负责的bucket
        if (i < 0 || i >= n || i + n >= nextn) {
            int sc;
 
            //如果完成了扩容
            if (finishing) {
 
                //删除成员变量
                nextTable = null;
 
                //更新table数组
                table = nextTab;
 
                //更新阈值(32*0.75=24)
                sizeCtl = (n << 1) - (n >>> 1);
                return;
            }
 
            // sizeCtl 在迁移前会设置为 (rs << RESIZE_STAMP_SHIFT) + 2 (详细介绍点击这里)
 
            // 然后，每增加一个线程参与迁移就会将 sizeCtl 加 1，
 
            // 这里使用 CAS 操作对 sizeCtl 的低16位进行减 1，代表做完了属于自己的任务
            if (U.compareAndSwapInt(this, SIZECTL, sc = sizeCtl, sc - 1)) {
 
                //第一个扩容的线程，执行transfer方法之前，会设置 sizeCtl = (resizeStamp(n) << RESIZE_STAMP_SHIFT) + 2)
 
                //后续帮其扩容的线程，执行transfer方法之前，会设置 sizeCtl = sizeCtl+1
 
                //每一个退出transfer的方法的线程，退出之前，会设置 sizeCtl = sizeCtl-1
 
                //那么最后一个线程退出时：必然有
                //sc == (resizeStamp(n) << RESIZE_STAMP_SHIFT) + 2)，即 (sc - 2) == resizeStamp(n) << RESIZE_STAMP_SHIFT
 
                // 如果 sc - 2 不等于标识符左移 16 位。如果他们相等了，说明没有线程在帮助他们扩容了。也就是说，扩容结束了。
                if ((sc - 2) != resizeStamp(n) << RESIZE_STAMP_SHIFT)
                    return;
 
                // 如果相等，扩容结束了，更新 finising 变量
                finishing = advance = true;
 
                // 再次循环检查一下整张表
                i = n; // recheck before commit
            }
        }
 
        // 如果位置 i 处是空的，没有任何节点，那么放入刚刚初始化的 ForwardingNode ”空节点“
        else if ((f = tabAt(tab, i)) == null)
            advance = casTabAt(tab, i, null, fwd);
 
        //表示该位置已经完成了迁移，也就是如果线程A已经处理过这个节点，那么线程B处理这个节点时，hash值一定为MOVED
        else if ((fh = f.hash) == MOVED)
            advance = true; // already processed
        else { //真正开始迁移
            synchronized (f) {
                ...... //省略
            }
        }
    }
}
```

## 数据迁移

**链表迁移优化**

* 如果在链表 尾端存在 类似的数据，那么尾端的三个0 可以直接搬过来，减少内存的使用，lastRun的作用

    `1->1->0->0->0`

* 

```java
 synchronized (f) {
    if (tabAt(tab, i) == f) {
        Node<K,V> ln, hn;
        if (fh >= 0) {
            int runBit = fh & n;
            Node<K,V> lastRun = f;
            for (Node<K,V> p = f.next; p != null; p = p.next) {
                int b = p.hash & n;
                if (b != runBit) {
                    runBit = b;
                    lastRun = p;
                }
            }
            if (runBit == 0) {
                ln = lastRun;
                hn = null;
            }
            else {
                hn = lastRun;
                ln = null;
            }
            for (Node<K,V> p = f; p != lastRun; p = p.next) {
                int ph = p.hash; K pk = p.key; V pv = p.val;
                if ((ph & n) == 0)
                    ln = new Node<K,V>(ph, pk, pv, ln);
                else
                    hn = new Node<K,V>(ph, pk, pv, hn);
            }
            setTabAt(nextTab, i, ln);
            setTabAt(nextTab, i + n, hn);
            setTabAt(tab, i, fwd);
            advance = true;
        }
        else if (f instanceof TreeBin) {
            TreeBin<K,V> t = (TreeBin<K,V>)f;
            TreeNode<K,V> lo = null, loTail = null;
            TreeNode<K,V> hi = null, hiTail = null;
            int lc = 0, hc = 0;
            for (Node<K,V> e = t.first; e != null; e = e.next) {
                int h = e.hash;
                TreeNode<K,V> p = new TreeNode<K,V>
                    (h, e.key, e.val, null, null);
                if ((h & n) == 0) {
                    if ((p.prev = loTail) == null)
                        lo = p;
                    else
                        loTail.next = p;
                    loTail = p;
                    ++lc;
                }
                else {
                    if ((p.prev = hiTail) == null)
                        hi = p;
                    else
                        hiTail.next = p;
                    hiTail = p;
                    ++hc;
                }
            }
            ln = (lc <= UNTREEIFY_THRESHOLD) ? untreeify(lo) :
            (hc != 0) ? new TreeBin<K,V>(lo) : t;
            hn = (hc <= UNTREEIFY_THRESHOLD) ? untreeify(hi) :
            (lc != 0) ? new TreeBin<K,V>(hi) : t;
            setTabAt(nextTab, i, ln);
            setTabAt(nextTab, i + n, hn);
            setTabAt(tab, i, fwd);
            advance = true;
        }
    }
 }
```
## helpTransfer

* 当发现 存在 ForwardingNode结点，则加入扩容行列上来

```java
final Node<K,V>[] helpTransfer(Node<K,V>[] tab, Node<K,V> f) {
    Node<K,V>[] nextTab; int sc;
    if (tab != null && (f instanceof ForwardingNode) &&
        (nextTab = ((ForwardingNode<K,V>)f).nextTable) != null) {
        int rs = resizeStamp(tab.length);
        while (nextTab == nextTable && table == tab &&
               (sc = sizeCtl) < 0) {
            //已经扩容完，但还未来得及更新SC
            if ((sc >>> RESIZE_STAMP_SHIFT) != rs || sc == rs + 1 ||
                sc == rs + MAX_RESIZERS || transferIndex <= 0)
                break;
            if (U.compareAndSwapInt(this, SIZECTL, sc, sc + 1)) {
                transfer(tab, nextTab);
                break;
            }
        }
        return nextTab;
    }
    return table;
}
```



## sizeCtl为负数的含义分析

### **组成图**

![image-20210322234319630](\images\concurrent_hash_map_sizectl_negate.png)

### **这样存储带来的好处？？**

首先在 CHM 中是支持并发扩容的，也就是说如果当前的数组需要进行扩容操作，可以由多个线程来共同负责；
第一个扩容的线程，执行 transfer 方法之前，
会设置 sizeCtl =(resizeStamp(n) << RESIZE_STAMP_SHIFT) + 2)

* 后续帮其扩容的线程，执行 transfer 方法之前，会设置 sizeCtl = sizeCtl+1
* 每一个退出 transfer 的方法的线程，退出之前，会设置 sizeCtl = sizeCtl-1
    那么最后一个线程退出时：必然有 sc == (resizeStamp(n) << RESIZE_STAMP_SHIFT) + 2)，
    如果 sc - 2 不等于标识符左移 16 位。

如果他们相等了，说明没有线程在帮助他们扩容了。也就是说，扩容结束了。

可以保证每次扩容都生成唯一的生成戳， 每次新的扩容，都有一个不同的 n（n是map的size），这个生成戳就是根据 n 来计算出来的一个数字， n 不同，这个数字也不同

### **第一个线程尝试扩容的时候，为什么是+2 ？？**

因为 1 表示初始化，2 表示一个线程在执行扩容，而且对 sizeCtl 的操作都是基于位运算的，
所以不会关心它本身的数值是多少，只关心它在二进制上的数值，而 sc + 1 会在
低 16 位上加 1。

### **多线程扩容要注意的问题？**



# PUTVAL

* 如果还未初始化，则初始化
* 如果当前 位置空闲，则直接插入结点
* 如果当前位置 为 *MOVED* 则说明有 扩容存在，则参与扩容
* 对当前结点加锁
    * 如果是链表 则往链表后插入
    * 如果是红黑树，则按红黑树插入
* 桶中有两种结点，一种是 链表结点，一种是 TreeBin结点，TreeBin结点负责 维护红黑树的 形成，插入删除

```java
final V putVal(K key, V value, boolean onlyIfAbsent) {
    if (key == null || value == null) throw new NullPointerException();
    int hash = spread(key.hashCode());
    int binCount = 0;
    for (Node<K,V>[] tab = table;;) {
        Node<K,V> f; int n, i, fh;
        if (tab == null || (n = tab.length) == 0)
            tab = initTable();
        else if ((f = tabAt(tab, i = (n - 1) & hash)) == null) {
            if (casTabAt(tab, i, null,
                         new Node<K,V>(hash, key, value, null)))
                break;                   // no lock when adding to empty bin
        }
        else if ((fh = f.hash) == MOVED)
            tab = helpTransfer(tab, f);
        else {
            V oldVal = null;
            synchronized (f) {
                if (tabAt(tab, i) == f) {
                    if (fh >= 0) {
                        binCount = 1;
                        for (Node<K,V> e = f;; ++binCount) {
                            K ek;
                            if (e.hash == hash &&
                                ((ek = e.key) == key ||
                                 (ek != null && key.equals(ek)))) {
                                oldVal = e.val;
                                if (!onlyIfAbsent)
                                    e.val = value;
                                break;
                            }
                            Node<K,V> pred = e;
                            if ((e = e.next) == null) {
                                pred.next = new Node<K,V>(hash, key,
                                                          value, null);
                                break;
                            }
                        }
                    }
                    else if (f instanceof TreeBin) {
                        Node<K,V> p;
                        binCount = 2;
                        if ((p = ((TreeBin<K,V>)f).putTreeVal(hash, key,
                                                       value)) != null) {
                            oldVal = p.val;
                            if (!onlyIfAbsent)
                                p.val = value;
                        }
                    }
                }
            }
            if (binCount != 0) {
                if (binCount >= TREEIFY_THRESHOLD)
                    treeifyBin(tab, i);
                if (oldVal != null)
                    return oldVal;
                break;
            }
        }
    }
    addCount(1L, binCount);
    return null;
}
```



# 红黑树操作

* 红黑树，即维护了 红黑树 的信息，又维护了 线性链表

## 查询

* 如果当前 root结点 有写锁存在，则 使用线性查找
* 没有写锁，则 使用红黑树查找，并设置一个 读锁
* 读完之后，如果当前 有阻塞在此处的线程则 唤醒

```java
final Node<K,V> find(int h, Object k) {
    if (k != null) {
        for (Node<K,V> e = first; e != null; ) {
            int s; K ek;
            if (((s = lockState) & (WAITER|WRITER)) != 0) {
                if (e.hash == h &&
                    ((ek = e.key) == k || (ek != null && k.equals(ek))))
                    return e;
                e = e.next;
            }
            else if (U.compareAndSwapInt(this, LOCKSTATE, s,
                                         s + READER)) {
                TreeNode<K,V> r, p;
                try {
                    p = ((r = root) == null ? null :
                         r.findTreeNode(h, k, null));
                } finally {
                    Thread w;
                    if (U.getAndAddInt(this, LOCKSTATE, -READER) ==
                        (READER|WAITER) && (w = waiter) != null)
                        LockSupport.unpark(w);
                }
                return p;
            }
        }
    }
    return null;
}
```

## 新增

* 对跟结点上锁
* 定位到叶子结点
* 红黑树上锁
    * 先尝试上写锁
    * 上锁失败，则尝试更加 耗时的锁竞争
        * 如果当前没有 人获取读写锁，则尝试 获取写锁
        * 如果当前没有 等待者，则设置 自己为 等待者，且进入 等待
        * 如果当前 锁被人占用，且等待者 也被人占用，则自旋
* 进行插入操作
* 解锁：*state* 置为0



## 删除

* 对根节点上锁
* 定位到该结点
* 红黑树上锁
    * 先尝试上写锁
    * 上锁失败，则尝试更加 耗时的锁竞争
        * 如果当前没有 人获取读写锁，则尝试 获取写锁
        * 如果当前没有 等待者，则设置 自己为 等待者，且进入 等待
        * 如果当前 锁被人占用，且等待者 也被人占用，则自旋
* 进行删除操作
* 解锁：*state* 置为0





# 总结

## 如何保证并发下的数据安全性

**维护 *map* 大小**

使用分段锁，先尝试更新 baseCount，更新失败然后尝试  更新 某个 *CounterCell* 更新某个*CounterCell失败* 最后尝试 全量更新*CounterCell*

**扩容**

并发扩容，每个线程负责 迁移 部分范围的 桶的数据

**维护红黑树**

使用读写锁

## ConcurrentHashMap如何扩容

* 并发扩容，每个线程负责维护 数组的固定段
* 当其他线程 查询，新增，删除时遇到 扩容则 参与扩容





[参考链接](https://blog.csdn.net/luzhensmart/article/details/105968886)

[JDK ConcurrentHashMap的BUG集锦](https://blog.csdn.net/anlian523/article/details/107328200)

 