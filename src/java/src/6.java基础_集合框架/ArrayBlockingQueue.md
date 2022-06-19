# BlockingQueue接口

## 声明

```java
public interface BlockingQueue<E> extends Queue<E> {
```

## 概述

**额外功能**

blockingQueue是 一个队列，提供额外的功能

* 当向队列取元素时，会阻塞直到队列存在元素
* 当向队列存元素时，会阻塞直到队列有空余

**形式**

BlockingQueue方法有四种形式，它们以不同的方式处理操作，这些操作无法立即满足，但将来可能会满足

* 第一种抛异常
* 第二种 有返回值
* 第三种 阻塞，直到操作可执行
* 第四种 超时阻塞

|         | Throws Exception | SpecialValue | Blocks | TimesOut           |
| ------- | ---------------- | ------------ | ------ | ------------------ |
| Insert  | add(e)           | offer(e)     | put(e) | offer(e,time,unit) |
| remove  | remove()         | poll         | take() | poll(time,unit)    |
| examine | element()        | peek()       |        |                    |

**NULL值**

blockingQueue 不能 插入*NULL* ，所有插入*NULL*的操作 抛NPE，空值在内部视为  失败

**容量限制**

BlockingQueue可能受容量限制。在任何给定时间，它可能具有剩余容量，超过该容量就不能放置其他元素而不会阻塞。
没有任何内部容量约束的BlockingQueue始终报告Integer.MAX_VALUE的剩余容量。

**集合关系**

BlockingQueue实现被设计为主要用于生产者-消费者队列，但额外也支持 集合接口，也可以随机 移除 一个数据 `remove(x)`，这样的操作效率不会很高，建议很少使用，例如 当消息队列中，某个消息被取消

**线程安全**

BlockingQueue实现是线程安全的。所有排队方法 都由内部锁实现并发控制

批量操作不保证 原子性，除非在实现中另行指定，否则批量Collection操作addAll，containsAll，retainAll和removeAll不一定是原子执行的。
因此，例如，仅在c中添加一些元素之后，addAll（c）可能会失败（引发异常）。

**关闭操作**

BlockingQueue本质上不支持任何类型的“关闭”或“关闭”操作，以指示将不再添加任何项目。
此类功能的需求和使用往往取决于实现。
例如，一种常见的策略是让生产者插入特殊的**流尾对象或有毒对象**（ special end-of-stream or poison objects）当消费者采取这种方法时会对其进行相应的解释。

## 接口

**新增**

```java
boolean add(E e);
boolean offer(E e);
void put(E e) throws InterruptedException;
boolean offer(E e, long timeout, TimeUnit unit) throws InterruptedException;
```

**移除**

```java
E take() throws InterruptedException;
E poll(long timeout, TimeUnit unit) throws InterruptedException;
boolean remove(Object o);
```

**批量移除**

```java
//将队列中的元素 倒入 指定集合
int drainTo(Collection<? super E> c);
//maxElements 指定最大要倒的集合
int drainTo(Collection<? super E> c, int maxElements);
```



# ArrayBlockingQueue

## 初始化

**空初始化**

* 立即初始化容量
* 使用 可重入锁
* 两个条件：*notEmpty*，*notFull*

```java
public ArrayBlockingQueue(int capacity, boolean fair) {
    if (capacity <= 0)
        throw new IllegalArgumentException();
    this.items = new Object[capacity];
    lock = new ReentrantLock(fair);
    notEmpty = lock.newCondition();
    notFull =  lock.newCondition();
}
```

**复制初始化**

* 使用者 自行确保容量 不能小于 *Collection*

```java
public ArrayBlockingQueue(int capacity, boolean fair,
                          Collection<? extends E> c) {
    this(capacity, fair);

    final ReentrantLock lock = this.lock;
    lock.lock(); // Lock only for visibility, not mutual exclusion ,出于可见性，上锁。不是为了互斥
    try {
        int i = 0;
        try {
            for (E e : c) {
                checkNotNull(e);
                items[i++] = e;
            }
        } catch (ArrayIndexOutOfBoundsException ex) {
            throw new IllegalArgumentException();
        }
        count = i;
        putIndex = (i == capacity) ? 0 : i;
    } finally {
        lock.unlock();
    }
}
```

## 重要方法

### 入队列

* 确保 已经获取到锁
* 确保 当前位置 为空
* 放入元素
* 容量自增
* *putIndex* 自增
* 通知 消费者队列

```java
private void enqueue(E x) {
    // assert lock.getHoldCount() == 1;
    // assert items[putIndex] == null;
    final Object[] items = this.items;
    items[putIndex] = x;
    if (++putIndex == items.length)
        putIndex = 0;
    count++;
    notEmpty.signal();
}
```

### **出队列**

* 确保 已经获取到锁
* 确保 当前位置 不为空
* 根据 *takeIndex* 取数据
* 原位置置空
* 容量自减
* 通知 生产者队列

```java
private E dequeue() {
    // assert lock.getHoldCount() == 1;
    // assert items[takeIndex] != null;
    final Object[] items = this.items;
    @SuppressWarnings("unchecked")
    E x = (E) items[takeIndex];
    items[takeIndex] = null;
    if (++takeIndex == items.length)
        takeIndex = 0;
    count--;
    if (itrs != null)
        itrs.elementDequeued(); //关于迭代器的维护比较复杂，在此不赘叙
    notFull.signal();
    return x;
}
```

## 新增元素

### **非阻塞新增**

容量满了立刻退出返回 *false*

```java
public boolean offer(E e) {
    checkNotNull(e);
    final ReentrantLock lock = this.lock;
    lock.lock();
    try {
        if (count == items.length)
            return false;
        else {
            enqueue(e);
            return true;
        }
    } finally {
        lock.unlock();
    }
}
```

### **阻塞新增**

* 获取锁
* 如果队列不为空，则等待
* 否则入队列

```java
public void put(E e) throws InterruptedException {
    checkNotNull(e);
    final ReentrantLock lock = this.lock;
    lock.lockInterruptibly();
    try {
        while (count == items.length)
            notFull.await();
        enqueue(e);
    } finally {
        lock.unlock();
    }
}
```

### **超时新增**

* 尝试获取锁
* **成功获取锁 之后 在开始计时**
* 锁超时之后 立刻返回 *false*

```java
public boolean offer(E e, long timeout, TimeUnit unit)
    throws InterruptedException {

    checkNotNull(e);
    long nanos = unit.toNanos(timeout);
    final ReentrantLock lock = this.lock;
    lock.lockInterruptibly();
    try {
        while (count == items.length) {
            if (nanos <= 0)
                return false;
            nanos = notFull.awaitNanos(nanos);
        }
        enqueue(e);
        return true;
    } finally {
        lock.unlock();
    }
}
```

## 移除元素

### 阻塞移除

```java
public E take() throws InterruptedException {
    final ReentrantLock lock = this.lock;
    lock.lockInterruptibly();
    try {
        while (count == 0)
            notEmpty.await();
        return dequeue();
    } finally {
        lock.unlock();
    }
}
```

### 超时移除

```java
public E poll(long timeout, TimeUnit unit) throws InterruptedException {
    long nanos = unit.toNanos(timeout);
    final ReentrantLock lock = this.lock;
    lock.lockInterruptibly();
    try {
        while (count == 0) {
            if (nanos <= 0)
                return null;
            nanos = notEmpty.awaitNanos(nanos);
        }
        return dequeue();
    } finally {
        lock.unlock();
    }
}
```

### 随机指定对象移除

* 需要将 移除对象后面的元素 全都 左移一位

```java
public boolean remove(Object o) {
    if (o == null) return false;
    final Object[] items = this.items;
    final ReentrantLock lock = this.lock;
    lock.lock();
    try {
        if (count > 0) {
            final int putIndex = this.putIndex;
            int i = takeIndex;
            do {
                if (o.equals(items[i])) {
                    removeAt(i);
                    return true;
                }
                if (++i == items.length)
                    i = 0;
            } while (i != putIndex);
        }
        return false;
    } finally {
        lock.unlock();
    }
}
```

### 批量移除数据到指定集合中

* 获取锁
* 循环 把数据加入到 新集合
* 维护*count* 变量
* 

```java
public int drainTo(Collection<? super E> c, int maxElements) {
    checkNotNull(c);
    if (c == this)
        throw new IllegalArgumentException();
    if (maxElements <= 0)
        return 0;
    final Object[] items = this.items;
    final ReentrantLock lock = this.lock;
    lock.lock();
    try {
        int n = Math.min(maxElements, count);
        int take = takeIndex;
        int i = 0;
        try {
            while (i < n) {
                @SuppressWarnings("unchecked")
                E x = (E) items[take];
                c.add(x);
                items[take] = null;
                if (++take == items.length)
                    take = 0;
                i++;
            }
            return n;
        } finally {
            // Restore invariants even if c.add() threw
            if (i > 0) {
                count -= i;
                takeIndex = take;
                if (itrs != null) {
                    if (count == 0)
                        itrs.queueIsEmpty();
                    else if (i > take)
                        itrs.takeIndexWrapped();
                }
                for (; i > 0 && lock.hasWaiters(notFull); i--)
                    notFull.signal();
            }
        }
    } finally {
        lock.unlock();
    }
}
```





# 总结

## ArrayBlockingQueue的实现原理

**同步实现方式**

* 使用可重入锁 实现，所有方法基本都加锁

**take与put的实现方式**

* 使用 两个条件锁，实现 take与 put的阻塞与通知

**存储结构**

* 使用循环队列作为存储结构
* 内部使用 数组，加上 两个 索引 takeIndex负责 取，putIndex负责存 实现的双向循环队列
* takeIndex总是指向 下一个待取的数据，而 putIndex总是指向 下一个空闲的位置



## 应用

线程池的默认底层数据结构



