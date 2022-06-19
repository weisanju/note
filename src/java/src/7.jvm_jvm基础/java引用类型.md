# JAVA 引用类型

> 在 java 的引用体系中，存在着强引用，软引用，弱引用，虚引用，这 4 种引用类型。关于这四种引用类型，可以查看[强引用、弱引用、软引用、虚引用](https://www.jianshu.com/p/1fc5d1cbb2d4)

| 引用类型                 | 被垃圾回收时间 | 用途         | 生存时间           |
| ------------------------ | -------------- | ------------ | ------------------ |
| 强引用                   | 从来不会       | 对象一般状态 | JVM 停止运行时终止 |
| 软引用(SoftReference)    | 内存不足时     | 对象缓存     | 内存不足时被回收   |
| 弱引用(WeakReference)    | 垃圾回收时     | 对象缓存     | GC 运行后后终止    |
| 虚引用(PhantomReference) | Unkonw         | Unkonw       | Unkonw             |

# 强引用

- 强引用是使用最普遍的引用。如果一个对象具有强引用，那垃圾回收器绝不会回收它

# 软引用

- 如果一个对象只具有软引用，则内存空间足够，垃圾回收器就不会回收它；如果内存空间不足了，就会回收这些对象的内存。只要垃圾回收器没有回收它，该对象就可以被程序使用。软引用可用来实现内存敏感的高速缓存。
- 软引用可以和一个引用队列（ReferenceQueue）联合使用，如果软引用所引用的对象被垃圾回收器回收，[Java](https://link.jianshu.com/?t=http://lib.csdn.net/base/javase)虚拟机就会把这个软引用加入到与之关联的引用队列中

# 弱引用

- 弱引用与软引用的区别在于：只具有弱引用的对象拥有更短暂的生命周期。在垃圾回收器线程扫描它所管辖的内存区域的过程中，一旦发现了只具有弱引用的对象，不管当前内存空间足够与否，都会回收它的内存。不过，由于垃圾回收器是一个优先级很低的线程，因此不一定会很快发现那些只具有弱引用的对象。
- 如果这个对象是偶尔的使用，并且希望在使用时随时就能获取到，但又不想影响此对象的垃圾收集，那么你应该用 Weak Reference 来记住此对象
- 弱引用可以和一个引用队列（ReferenceQueue）联合使用，如果弱引用所引用的对象被垃圾回收，Java 虚拟机就会把这个弱引用加入到与之关联的引用队列中。

**为什么引入？**

> 考虑下面的场景：现在有一个 Product 类代表一种产品，这个类被设计为不可扩展的，而此时我们想要为每个产品增加一个编号。一种解决方案是使用 HashMap<Product, Integer>。于是问题来了，如果我们已经不再需要一个 Product 对象存在于内存中（比如已经卖出了这件产品），假设指向它的引用为 productA，我们这时会给 productA 赋值为 null，然而这时 productA 过去指向的 Product 对象并不会被回收，因为它显然还被 HashMap 引用着。所以这种情况下，我们想要真正的回收一个 Product 对象，仅仅把它的强引用赋值为 null 是不够的，还要把相应的条目从 HashMap 中移除。显然“从 HashMap 中移除不再需要的条目”这个工作我们不想自己完成，我们希望告诉垃圾收集器：在只有 HashMap 中的 key 在引用着 Product 对象的情况下，就可以回收相应 Product 对象了。显然，根据前面弱引用的定义，使用弱引用能帮助我们达成这个目的。我们只需要用一个指向 Product 对象的弱引用对象来作为 HashMap 中的 key 就可以了。

# 虚引用

如果一个对象仅持有虚引用，那么它就和没有任何引用一样，在任何时候都可能被垃圾回收器回收。

虚引用主要用来跟踪对象被垃圾回收器回收的活动。虚引用与软引用和弱引用的一个区别在于：虚引用必须和引用队列 （ReferenceQueue）联合使用。当垃圾回收器准备回收一个对象时，如果发现它还有虚引用，就会在回收对象的内存之前，把这个虚引用加入到与之 关联的引用队列中。

# 引用图

![20201025080109](https://i.loli.net/2020/10/25/fpJ3rFgQKP5hosC.png)

# 引用队列

> ReferenceQueue, 当对象被 GC 时, 通知用户线程

- 对于`软引用`和`弱引用`，我们希望当一个对象被 gc 掉的时候通知用户线程，进行额外的处理时，就需要使用引用队列了。ReferenceQueue 即这样的一个对象，当一个 obj 被 gc 掉之后，其相应的包装类，即 ref 对象会被放入 queue 中。我们可以从 queue 中获取到相应的对象信息，同时进行额外的处理。比如反向操作，数据清理等。

**在 weakHashMap 中引用队列的使用**

继承 AbstractMap、Map 接口。和 HashMap 一样都是散列表，存储的是 key-value,键和值都可以为 null。
-

**清除代码**

> 每次对map的操作都会从 ReferenceQueue 获取失效的key, 然后从map中删除。调用该方法

```java
for (Object x; (x = queue.poll()) != null; ) {
    synchronized (queue) {
        @SuppressWarnings("unchecked")
            Entry<K,V> e = (Entry<K,V>) x;
        int i = indexFor(e.hash, table.length);

        Entry<K,V> prev = table[i];
        Entry<K,V> p = prev;
        while (p != null) {
            Entry<K,V> next = p.next;
            if (p == e) {
                if (prev == e)
                    table[i] = next;
                else
                    prev.next = next;
                // Must not null out e.next;
                // stale entries may be in use by a HashIterator
                e.value = null; // Help GC
                size--;
                break;
            }
            prev = p;
            p = next;
        }
    }
}
```

**weakHashMap总结**

* WeakHashMap 使用(数据 + 链表 ） 存储结构。

* WeakHashMap中的key 是弱引用，垃圾回收时会被回收。
* 使用场景： 作为缓存

