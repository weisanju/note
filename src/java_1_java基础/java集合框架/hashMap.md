# 前言

* java.util.Map接口常用的实现类：HashMap、TreeMap、HashTable、SortedMap。这些实现中最常用的是HashMap。
* HashMap是存放键值对key-value的散列表，它的底层数据结构是数组+链表+红黑树。
* ![hashMap结构图](/images/hashMap_structure.png)





# 核心参数

* *DEFAULT_INITIAL_CAPACITY* 为16，默认大小
* *MAXIMUM_CAPACITY* 为 *1 << 30* ，最大大小
* *TREEIFY_THRESHOLD* 8，树形化阈值，当链表的个数大于8 ，才从链表转 红黑树
* *UNTREEIFY_THRESHOLD* 6 去树形话阈值，从红黑树转 链表的阈值
* *MIN_TREEIFY_CAPACITY* 当 容器的大小大于 64 才会  在 链表过长时 转 红黑树
* *DEFAULT_LOAD_FACTOR* 负载因子 0.75 ，当元素个数/容器大小 超过 0.75时，会扩容





# **HashMap的Node对象**

Node对象定义了4个变量：

- hash：key的hash值
- key：需要存储的键值对的key值
- value：需要存储的键值对的value值
- next：指向下一个元素的指针地址，如果不是链表或者树next为null





# 对键取Hash方法

```java
//将hashCode的 低16位与高16位 做异或
static final int hash(Object key) {
    int h;
    return (key == null) ? 0 : (h = key.hashCode()) ^ (h >>> 16);
}
```



# PUT方法

## PUT方法流程图

![image-20210215151635333](..\..\images\hashMap_put_value.png)



## PUT_VALUE CODE

```java
    final V putVal(int hash, K key, V value, boolean onlyIfAbsent,
                   boolean evict) {
        Node<K,V>[] tab; Node<K,V> p; int n, i;
        //判断table是否初始化，如果为空则 扩容
        if ((tab = table) == null || (n = tab.length) == 0)
            n = (tab = resize()).length;
        //如果没有冲突则直接插入
        if ((p = tab[i = (n - 1) & hash]) == null)
            tab[i] = newNode(hash, key, value, null);
        //如果存在冲突则
        else {
            Node<K,V> e; K k;
            //当key跟 数组的第一个结点一样，则只更新 value
            if (p.hash == hash &&
                ((k = p.key) == key || (key != null && key.equals(k))))
                e = p;
            //当结点为 TreeNode 则插入红黑树
            else if (p instanceof TreeNode)
                e = ((TreeNode<K,V>)p).putTreeVal(this, tab, hash, key, value);
            //当结点为 普通Node，则插入普通结点，如果已经存在结点，则只更新value
            //当超过了树形化阈值，则树形化
            else {
                for (int binCount = 0; ; ++binCount) {
                    if ((e = p.next) == null) {
                        p.next = newNode(hash, key, value, null);
                        if (binCount >= TREEIFY_THRESHOLD - 1) // -1 for 1st
                            treeifyBin(tab, hash);
                        break;
                    }
                    if (e.hash == hash &&
                        ((k = e.key) == key || (key != null && key.equals(k))))
                        break;
                    p = e;
                }
            }
            if (e != null) { // existing mapping for key
                V oldValue = e.value;
                if (!onlyIfAbsent || oldValue == null)
                    e.value = value;
                afterNodeAccess(e);
                return oldValue;
            }
        }
        //如果大小超过了 容量阈值，则扩容
        ++modCount;
        if (++size > threshold)
            resize();
        afterNodeInsertion(evict);
        return null;
    }
```

# 树形化

略，参照红黑树

## 结点转换

* 首先将 普通node结点转换为 TreeNode结点
* 然后从将 链表树形化

```java

final void treeifyBin(Node<K,V>[] tab, int hash) {
    int n, index; Node<K,V> e;
    if (tab == null || (n = tab.length) < MIN_TREEIFY_CAPACITY)
        resize();
    
    //链表结点 转树形结点
    else if ((e = tab[index = (n - 1) & hash]) != null) {
        TreeNode<K,V> hd = null, tl = null;
        do {
            TreeNode<K,V> p = replacementTreeNode(e, null);
            if (tl == null)
                hd = p;
            else {
                p.prev = tl;
                tl.next = p;
            }
            tl = p;
        } while ((e = e.next) != null);
        if ((tab[index] = hd) != null)
            hd.treeify(tab);
    }
}
```

## 树形化

* 遍历 结点树链表，依次平衡插入

**key值比较逻辑**

* hash 大于 根节点的 放右边，小于根节点的hash放 左边
* 如果hash相等，则判断 key是否实现了 Compareable类，如果实现了则 调用 compareable方法
* 如果hash相等，也没有实现 compareable,则 调用全局系统的 判定方法

```java
        final void treeify(Node<K,V>[] tab) {
            TreeNode<K,V> root = null;
            for (TreeNode<K,V> x = this, next; x != null; x = next) {
                next = (TreeNode<K,V>)x.next;
                x.left = x.right = null;
                //树立根结点
                if (root == null) {
                    x.parent = null;
                    x.red = false;
                    root = x;
                }
                else {
                    K k = x.key;
                    int h = x.hash;
                    Class<?> kc = null;
                    //大于根节点的放在 
                    for (TreeNode<K,V> p = root;;) {
                        int dir, ph;
                        K pk = p.key;
                        if ((ph = p.hash) > h)
                            dir = -1;
                        else if (ph < h)
                            dir = 1;
                        else if ((kc == null &&
                                  (kc = comparableClassFor(k)) == null) ||
                                 (dir = compareComparables(kc, k, pk)) == 0)
                            dir = tieBreakOrder(k, pk);

                        TreeNode<K,V> xp = p;
                        if ((p = (dir <= 0) ? p.left : p.right) == null) {
                            x.parent = xp;
                            if (dir <= 0)
                                xp.left = x;
                            else
                                xp.right = x;
                            root = balanceInsertion(root, x);
                            break;
                        }
                    }
                }
            }
            moveRootToFront(tab, root);
        }
```

## 平衡插入

略，具体实现见 [红黑树](..\..\数据结构与算法\树\红黑树.md)







# 重新扩容

* 如果容量 超过了 允许的最大的个数 *MAXIMUM_CAPACITY = 1 << 30* 则退出

* 将容量翻倍 ，阈值翻倍

* 遍历旧 容器的 所有结点

    * 如果 链条中结点只有一个 则直接使用 新容量  重新计算 位置

    * 如果是 链条结点 则将链条数据分为 两类，一类是需要改变位置的结点，一类是不需要改变位置结点

        * *(e.hash & oldCap) == 0* 判断 hash值的高位 是否为0
            * 如果为0 则说明 hash值< *oldCap* 在新数组中的位置 不用变，
            * 如果为1 则说明 hash值> *oldCap* 需要重新改变

    * 如果是 红黑树  将数据分为 两类，一类是需要改变位置的结点，一类是不需要改变位置结点

        * *(e.hash & oldCap) == 0* 判断 hash值的高位 是否为0

            * 如果为0 则说明 hash值< *oldCap* 在新数组中的位置 不用变，
            * 如果为1 则说明 hash值> *oldCap* 需要重新改变

        * 如果链条长度 超过了树形话的阈值，则树形话

            ```
            loHead.treeify(tab);
            ```

            



## 重新扩容主方法

```java
 final Node<K,V>[] resize() {
        Node<K,V>[] oldTab = table;
        int oldCap = (oldTab == null) ? 0 : oldTab.length;
        int oldThr = threshold;
        int newCap, newThr = 0;
        if (oldCap > 0) {
            if (oldCap >= MAXIMUM_CAPACITY) {
                threshold = Integer.MAX_VALUE;
                return oldTab;
            }
            else if ((newCap = oldCap << 1) < MAXIMUM_CAPACITY &&
                     oldCap >= DEFAULT_INITIAL_CAPACITY)
                newThr = oldThr << 1; // double threshold
        }
        else if (oldThr > 0) // initial capacity was placed in threshold
            newCap = oldThr;
        else {               // zero initial threshold signifies using defaults
            newCap = DEFAULT_INITIAL_CAPACITY;
            newThr = (int)(DEFAULT_LOAD_FACTOR * DEFAULT_INITIAL_CAPACITY);
        }
        if (newThr == 0) {
            float ft = (float)newCap * loadFactor;
            newThr = (newCap < MAXIMUM_CAPACITY && ft < (float)MAXIMUM_CAPACITY ?
                      (int)ft : Integer.MAX_VALUE);
        }
        threshold = newThr;
        @SuppressWarnings({"rawtypes","unchecked"})
        Node<K,V>[] newTab = (Node<K,V>[])new Node[newCap];
        table = newTab;
        if (oldTab != null) {
            for (int j = 0; j < oldCap; ++j) {
                Node<K,V> e;
                if ((e = oldTab[j]) != null) {
                    oldTab[j] = null;
                    if (e.next == null)
                        newTab[e.hash & (newCap - 1)] = e;
                    else if (e instanceof TreeNode)
                        ((TreeNode<K,V>)e).split(this, newTab, j, oldCap);
                    else { // preserve order
                        Node<K,V> loHead = null, loTail = null;
                        Node<K,V> hiHead = null, hiTail = null;
                        Node<K,V> next;
                        do {
                            next = e.next;
                            if ((e.hash & oldCap) == 0) {
                                if (loTail == null)
                                    loHead = e;
                                else
                                    loTail.next = e;
                                loTail = e;
                            }
                            else {
                                if (hiTail == null)
                                    hiHead = e;
                                else
                                    hiTail.next = e;
                                hiTail = e;
                            }
                        } while ((e = next) != null);
                        if (loTail != null) {
                            loTail.next = null;
                            newTab[j] = loHead;
                        }
                        if (hiTail != null) {
                            hiTail.next = null;
                            newTab[j + oldCap] = hiHead;
                        }
                    }
                }
            }
        }
        return newTab;
    }
```

## 对红黑树进行重新hash

> ```
> ((TreeNode<K,V>)e).split(this, newTab, j, oldCap);
> ```

```java
        final void split(HashMap<K,V> map, Node<K,V>[] tab, int index, int bit) {
            TreeNode<K,V> b = this;
            // Relink into lo and hi lists, preserving order
            TreeNode<K,V> loHead = null, loTail = null;
            TreeNode<K,V> hiHead = null, hiTail = null;
            int lc = 0, hc = 0;
            for (TreeNode<K,V> e = b, next; e != null; e = next) {
                next = (TreeNode<K,V>)e.next;
                e.next = null;
                if ((e.hash & bit) == 0) {
                    if ((e.prev = loTail) == null)
                        loHead = e;
                    else
                        loTail.next = e;
                    loTail = e;
                    ++lc;
                }
                else {
                    if ((e.prev = hiTail) == null)
                        hiHead = e;
                    else
                        hiTail.next = e;
                    hiTail = e;
                    ++hc;
                }
            }

            if (loHead != null) {
                if (lc <= UNTREEIFY_THRESHOLD)
                    tab[index] = loHead.untreeify(map);
                else {
                    tab[index] = loHead;
                    if (hiHead != null) // (else is already treeified)
                        loHead.treeify(tab);
                }
            }
            if (hiHead != null) {
                if (hc <= UNTREEIFY_THRESHOLD)
                    tab[index + bit] = hiHead.untreeify(map);
                else {
                    tab[index + bit] = hiHead;
                    if (loHead != null)
                        hiHead.treeify(tab);
                }
            }
        }
```



# 移除结点

* 根据 *hash* 索引数组 

```java
p = tab[index = (n - 1) & hash]
```

* 查找结点
    * 通过 链条查找结点
    * 通过 红黑树查找结点
* 找到结点之后
    * 如果是链条 则 断开该节点的链条
    * 如果是红黑树，则 调用红黑树的删除

```java
final Node<K,V> removeNode(int hash, Object key, Object value,
                           boolean matchValue, boolean movable) {
    Node<K,V>[] tab; Node<K,V> p; int n, index;
    if ((tab = table) != null && (n = tab.length) > 0 &&
        (p = tab[index = (n - 1) & hash]) != null) {
        Node<K,V> node = null, e; K k; V v;
        if (p.hash == hash &&
            ((k = p.key) == key || (key != null && key.equals(k))))
            node = p;
        else if ((e = p.next) != null) {
            if (p instanceof TreeNode)
                node = ((TreeNode<K,V>)p).getTreeNode(hash, key);
            else {
                do {
                    if (e.hash == hash &&
                        ((k = e.key) == key ||
                         (key != null && key.equals(k)))) {
                        node = e;
                        break;
                    }
                    p = e;
                } while ((e = e.next) != null);
            }
        }
        if (node != null && (!matchValue || (v = node.value) == value ||
                             (value != null && value.equals(v)))) {
            if (node instanceof TreeNode)
                ((TreeNode<K,V>)node).removeTreeNode(this, tab, movable);
            else if (node == p)
                tab[index] = node.next;
            else
                p.next = node.next;
            ++modCount;
            --size;
            afterNodeRemoval(node);
            return node;
        }
    }
    return null;
}
```



# 面试总结

## hashMap 的底层结构

数组+链表+红黑树，通过数组存储 *hash* 散列后的位置，使用链条 解决 hash冲突，当冲突足够多时，为了提高查询效率 使用 红黑树

## *hashMap* 容量大小为什么是 2的倍数

* hash散列 时 是使用 key的 hash值 与   `hash(key) & (capacity-1)`   能够充分的散列 

* 在扩容时减少 数据移动

  * 数据索引的位置 跟 *hash* 与 *capacity-1* 有关
  * 扩容后  hash不变   要么 变化为 *index+oldCapcity*
  
    

## hashMap 的key值 允许为空吗？

允许，因为无论 null Key取hash的值为0

hashTab 在插入null Key时直接报错



## HashMap与 HashTab的区别

* **数据结构不同** 数组加链表

* 没有 容量懒加载

* key值不能为空
* 容量 不用满足 2的幂方
* 求 hash散列方式略微不同

**最重要的**

**所有 新增，删除，扩容方法 都加了锁**

