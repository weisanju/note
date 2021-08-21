# JDK 中 ThreadLocal 的实现

## ThreadLocal 是如何存储变量的

- 绑定在本地线程变量 中 **Thread.ThreadLocal.ThreadLocalMap**

- 每个线程可能有多个 **ThreadLocal**
  ![20201025071133](https://i.loli.net/2020/10/25/DnTV85QOvkdjR1X.png)

## ThreadLocalMap为什么要继承 **WeakReference**

* ThreadLocalMap是与线程绑定在一起的, 而ThreadLocal 又与 ThreadLocalMap存在引用 ,但两者生命周期 可能会不一致,会导致内存泄漏的风险

* 设置为 弱引用 可以在GC时 被回收

## ThreadLocalMap是如何避免内存泄漏的

* Entry是一个弱引用对象, 持有对ThreadLocal 的 弱引用

* 在 调用  

  ```
  get,set,remove 的方法时 ,都会清空 key为null 相应的 value
  ```

* 只能保证 key 的弱引用,  value无法保证, 所以在不需要 **LocalThread** 之后  应调用一次清理



# ThreadLocalMap源码分析

> 每个线程都会有一个 ThreadLocalMap，用来存放当前线程 所有的 本地线程变量，key为 ThreadLocal对象，value为存放的值

* *ThreadLocalMap* 内部是由 数组 hash实现
* hash冲突解决 使用 开放地址法中的 线性查找，往前寻找第一个空闲区域

## 初始化

**使用ThreadLocal初始化**

* 初始化容量为16
* 使用 ThreadLocal的 hashCode作为 目标散列对象
* 默认扩容阈值是 2/3

```java
ThreadLocalMap(ThreadLocal<?> firstKey, Object firstValue) {
    table = new Entry[INITIAL_CAPACITY];
    int i = firstKey.threadLocalHashCode & (INITIAL_CAPACITY - 1);
    table[i] = new Entry(firstKey, firstValue);
    size = 1;
    setThreshold(INITIAL_CAPACITY);
}
```

**使用ThreadLocalMap初始化**

* 循环取数据 放入到新的Map中
* 如果产生冲突，则 索引下放到下一个

```java
private ThreadLocalMap(ThreadLocalMap parentMap) {
    Entry[] parentTable = parentMap.table;
    int len = parentTable.length;
    setThreshold(len);
    table = new Entry[len];

    for (int j = 0; j < len; j++) {
        Entry e = parentTable[j];
        if (e != null) {
            @SuppressWarnings("unchecked")
            ThreadLocal<Object> key = (ThreadLocal<Object>) e.get();
            if (key != null) {
                Object value = key.childValue(e.value);
                Entry c = new Entry(key, value);
                int h = key.threadLocalHashCode & (len - 1);
                while (table[h] != null)
                    h = nextIndex(h, len);
                table[h] = c;
                size++;
            }
        }
    }
}
```

## 放入元素

* 根据 hashCode计算 数组中的索引

* 如果 entry是空闲的，则直接放入空闲地区

* 如果 entry不是空闲的，则说明发生hash冲突

    * 如果在冲突过程中 遇到 失效的 key，则调用 *replaceStaleEntry*
    * 如果在冲突过程中 遇到 key本身，则直接替换value

* 如果在冲突过程中 既没遇到 失效得key，也没遇到 key本身，且遇到 空闲 *slot*，

    则先调用一遍 启发式清理，如果没有清理出数据，且超过了阈值则扩容

```java
private void set(ThreadLocal<?> key, Object value) {

    // We don't use a fast path as with get() because it is at
    // least as common to use set() to create new entries as
    // it is to replace existing ones, in which case, a fast
    // path would fail more often than not.

    Entry[] tab = table;
    int len = tab.length;
    int i = key.threadLocalHashCode & (len-1);

    for (Entry e = tab[i];
         e != null;
         e = tab[i = nextIndex(i, len)]) {
        ThreadLocal<?> k = e.get();

        if (k == key) {
            e.value = value;
            return;
        }

        if (k == null) {
            replaceStaleEntry(key, value, i); //替换 当前无效引用
            return;
        }
    }

    tab[i] = new Entry(key, value);
    int sz = ++size;
    if (!cleanSomeSlots(i, sz) && sz >= threshold)
        rehash();
}
```

## 获取元素

**快慢路径**

* 如果第一个命中了则 直接返回
* 如果没有命中，则线性探测

```java
private Entry getEntry(ThreadLocal<?> key) {
    int i = key.threadLocalHashCode & (table.length - 1);
    Entry e = table[i];
    if (e != null && e.get() == key)
        return e;
    else
        return getEntryAfterMiss(key, i, e);
}

private Entry getEntryAfterMiss(ThreadLocal<?> key, int i, Entry e) {
    Entry[] tab = table;
    int len = tab.length;
    while (e != null) {
        ThreadLocal<?> k = e.get();
        if (k == key)
        return e;
        if (k == null)
        expungeStaleEntry(i);
        else
            i = nextIndex(i, len);
        e = tab[i];
        }
    return null;
}
```



## 在哈希冲突过程中遇到 无效引用

* 记录 当前无效引用的 连续非*NULL* 区间 上一个无效引用
    * 如果 后向区间 存在 key本身，则交换 key 与 staleSlot的 位置，并调用一次 线性探测清理，与 启发式清理
    * 如果 后向区间 不存在 key本身，则直接取代staleSlot的位置，如果在当前区间内还探测到其他 无效Key则 进行 线性探测清理，与 启发式清理

```java
private void replaceStaleEntry(ThreadLocal<?> key, Object value,
                               int staleSlot) {
    Entry[] tab = table;
    int len = tab.length;
    Entry e;

    // Back up to check for prior stale entry in current run.
    // We clean out whole runs at a time to avoid continual
    // incremental rehashing due to garbage collector freeing
    // up refs in 
    
    (i.e., whenever the collector runs).
    int slotToExpunge = staleSlot;
    for (int i = prevIndex(staleSlot, len);
         (e = tab[i]) != null;
         i = prevIndex(i, len))
        if (e.get() == null)
            slotToExpunge = i;

    // Find either the key or trailing null slot of run, whichever
    // occurs first
    for (int i = nextIndex(staleSlot, len);
         (e = tab[i]) != null;
         i = nextIndex(i, len)) {
        ThreadLocal<?> k = e.get();

        // If we find key, then we need to swap it
        // with the stale entry to maintain hash table order.
        // The newly stale slot, or any other stale slot
        // encountered above it, can then be sent to expungeStaleEntry
        // to remove or rehash all of the other entries in run.
        if (k == key) {
            e.value = value;

            tab[i] = tab[staleSlot];
            tab[staleSlot] = e;

            // Start expunge at preceding stale entry if it exists
            if (slotToExpunge == staleSlot)
                slotToExpunge = i;
            cleanSomeSlots(expungeStaleEntry(slotToExpunge), len);
            return;
        }

        // If we didn't find stale entry on backward scan, the
        // first stale entry seen while scanning for key is the
        // first still present in the run.
        if (k == null && slotToExpunge == staleSlot)
            slotToExpunge = i;
    }

    // If key not found, put new entry in stale slot
    tab[staleSlot].value = null;
    tab[staleSlot] = new Entry(key, value);

    // If there are any other stale entries in run, expunge them
    if (slotToExpunge != staleSlot)
        cleanSomeSlots(expungeStaleEntry(slotToExpunge), len);
}
```

## 线性清理

**输入与输出**

传入失效的key的索引，返回邻近的下一个 空闲 *slot*

**步骤**

* 清空当前 失效 *slot*
* 清空 非NULL 区间的 失效 *slot*
* 将冲突的 *slot* 尽量 往正确的索引移动

```java
private int expungeStaleEntry(int staleSlot) {
    Entry[] tab = table;
    int len = tab.length;

    // expunge entry at staleSlot
    tab[staleSlot].value = null;
    tab[staleSlot] = null;
    size--;

    // Rehash until we encounter null
    Entry e;
    int i;
    for (i = nextIndex(staleSlot, len);
         (e = tab[i]) != null;
         i = nextIndex(i, len)) {
        ThreadLocal<?> k = e.get();
        if (k == null) {
            e.value = null;
            tab[i] = null;
            size--;
        } else {
            int h = k.threadLocalHashCode & (len - 1);
            if (h != i) {
                tab[i] = null;

                // Unlike Knuth 6.4 Algorithm R, we must scan until
                // null because multiple entries could have been stale.
                while (tab[h] != null)
                    h = nextIndex(h, len);
                tab[h] = e;
            }
        }
    }
    return i;
}
```

## 启发式清理

**输入输出**

i不是 失效的key的索引即可

n为扫描的 轮数，`log2(n)`

**逻辑**

* 按 连续的 非NULL段清理，故 n的元素 最多有 n/2段

    

```java
private boolean cleanSomeSlots(int i, int n) {
    boolean removed = false;
    Entry[] tab = table;
    int len = tab.length;
    do {
        i = nextIndex(i, len);
        Entry e = tab[i];
        if (e != null && e.get() == null) {
            n = len;
            removed = true;
            i = expungeStaleEntry(i);
        }
    } while ( (n >>>= 1) != 0);
    return removed;
}
```

## 全量清理

> 循环调用线性清理

```java
private void expungeStaleEntries() {
    Entry[] tab = table;
    int len = tab.length;
    for (int j = 0; j < len; j++) {
        Entry e = tab[j];
        if (e != null && e.get() == null)
            expungeStaleEntry(j);
    }
}
```
## 扩容逻辑

* 3/4的阈值 扩容
* 扩容是 2倍率

```java
private void rehash() {
    expungeStaleEntries();

    // Use lower threshold for doubling to avoid hysteresis
    if (size >= threshold - threshold / 4)
        resize();
}

private void resize() {
    Entry[] oldTab = table;
    int oldLen = oldTab.length;
    int newLen = oldLen * 2;
    Entry[] newTab = new Entry[newLen];
    int count = 0;

    for (int j = 0; j < oldLen; ++j) {
        Entry e = oldTab[j];
        if (e != null) {
            ThreadLocal<?> k = e.get();
            if (k == null) {
                e.value = null; // Help the GC
            } else {
                int h = k.threadLocalHashCode & (newLen - 1);
                while (newTab[h] != null)
                    h = nextIndex(h, newLen);
                newTab[h] = e;
                count++;
            }
        }
    }

    setThreshold(newLen);
    size = count;
    table = newTab;
}
```

# ThreadLocal扩展

## 可继承的**ThreadLocal**

```java
ThreadLocal.ThreadLocalMap inheritableThreadLocals = null;
```

在本线程 创建子线程 会把 inheritableThreadLocals 传递 给子线程

```
InheritableThreadLocal
```

**源码**

```java
if (inheritThreadLocals && parent.inheritableThreadLocals != null)
    this.inheritableThreadLocals =
        ThreadLocal.createInheritedMap(parent.inheritableThreadLocals);
/* Stash the specified stack size in case the VM cares */
```

**示例**

```java
public static void main(String[] args) {
    ThreadLocal<String> a = new ThreadLocal<>();
    a.set("xjq");

    InheritableThreadLocal<String> b = new InheritableThreadLocal<>();
    b.set("xjq");
    new Thread(()->{
        String s = b.get();
        System.out.println(s);
    }).start();

}
```

## *TransmittableThreadLocal*

用于在异步调用间，线程池的 ThreadLocal传递









# ThreadLocal 应用

## **spring 获取被代理的对象**

**spring aop**

Spring 的事务是基于 AOP 实现的，AOP 是基于动态代理实现的。

所以 @Transactional 注解如果想要生效，那么其调用方，需要是被 Spring 动态代理后的类。

因此如果在同一个类里面，使用 this 调用被 @Transactional 注解修饰的方法时，是不会生效的。

**为什么？**

因为 this 对象是未经动态代理后的对象。

**那么我们怎么获取动态代理后的对象呢？**

其中的一个方法就是通过 AopContext 来获取。

**对应代码位置**

```java
org.springframework.aop.framework.CglibAopProxy.DynamicAdvisedInterceptor#intercept

				if (this.advised.exposeProxy) {
					// Make invocation available if necessary.
					oldProxy = AopContext.setCurrentProxy(proxy);
					setProxyContext = true;
				}
```

**对应开关**

```
@EnableAspectJAutpProxy(exposeProxy=true)
或者
<aop:aspectj-autoproxy expose-proxy='true'/>
```

## **mybatis 的分页插件，PageHelper**

**使用**

```java
PageHelper.startPage(1,10);
List<User> list = userMapper.selectInfo()
```

**为什么紧跟着的第一个 select 方法会被分页?**

PageHelper 方法使用了静态的 ThreadLocal 参数，分页参数和线程是绑定的：





# 总结

## ThreadLocal的实现原理是什么？

* 每一个线程绑定一个 *ThreadLocalMap* ，这个*Map* 以 ThreadLocal作为Key，存入的值作为 value，这样 每个线程都有作用于全局的 独立的内存空间

## ThreadLocalMap的实现原理？

* ThreadLocalMap维护了Entry环形数组，数组中元素Entry的逻辑上的key为某个ThreadLocal对象（实际上是指向该ThreadLocal对象的弱引用），value为代码中该线程往该ThreadLoacl变量实际塞入的值。

* 是一个Map，底层使用 数组实现，通过hash散列 到 数组对应的索引

* 通过线性 探测解决 hash冲突

## *ThreadLocalMap* 的key有什么特殊之处？

* 继承了弱引用，当 内存不足时，会回收此处引用的空间

**为何要这样做**

* 因为 线程的生命周期 可能明显要大于 ThreadLocal的存活周期
* 如果*ThreadLocal* 的外部引用消失了，但线程还在，就会出现 内存泄漏
* 所以使用 弱引用，如果 *ThreadLocal* 不存在外部引用了，这个*Key* 就可能会被垃圾回收掉
* 然后再下一次 对 这个 Map的 访问或修改中 *entry* 可能会就被移除

但是重要的一点：*value* 还是会产生内存泄漏

所以 *key* 的弱引用只是为了提醒 *Map* 尽快对 清理*entry* 

## ThreadLocalMap扩容机制？

启发式清理 无效引用，如果没有清理任何数据，当前大小超过了阈值，则开始 扩容

而开始扩容前，会进行一次全量的 无效引用的清理，如果此时超过了  3\4阈值，则真正开始扩容



## 应用

* spring获取当前代理对象
* mybatis分页
* 自己项目：租户Code
* org.slf4j.MDC ，链路追踪ID



[参考链接](https://github.com/Snailclimb/JavaGuide/blob/master/docs/java/multi-thread/%E4%B8%87%E5%AD%97%E8%AF%A6%E8%A7%A3ThreadLocal%E5%85%B3%E9%94%AE%E5%AD%97.md)

[InternalThreadLocalMap](https://www.toutiao.com/i6885197882979254796/)





[transmittable-thread-local](https://github.com/alibaba/transmittable-thread-local)





