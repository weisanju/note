# 集合类继承图

![](/images/collection_structure.png)





# *Collection*接口

## 查询操作

*int size();*，*boolean isEmpty();*，*boolean contains(Object o)*



## 数组与集合转换

**返回Object数组**

返回一个全新的数组，可以任意修改

```java
Object[] toArray();
```

**返回指定类型的数组**

* 如果数组大小正合适，则将元素 填充至该指定数组，否则，返回全新数组
* 如果数组元素有多的，则第一个多的元素会被置为 *NULL*，以示区分
* 这个方法能精确控制 数组返回的 运行时类型



```java
<T> T[] toArray(T[] a);
//一般使用
String[] y = x.toArray(new String[0]);
```



## 修改操作

```
boolean add(E e);
boolean remove(Object o);
```



## 批量操作

```java
//判断当前集合  是否 包含 指定集合
boolean containsAll(Collection<?> c);
//添加到集合中
boolean addAll(Collection<? extends E> c);
//通过迭代器，迭代，移除
boolean removeAll(Collection<?> c);
    default boolean removeIf(Predicate<? super E> filter) {
        Objects.requireNonNull(filter);
        boolean removed = false;
        final Iterator<E> each = iterator();
        while (each.hasNext()) {
            if (filter.test(each.next())) {
                each.remove();
                removed = true;
            }
        }
        return removed;
    }

//取交集，只保留 当前集合与 指定集合 都含有的元素
boolean retainAll(Collection<?> c);
//清空集合
void clear();
```

## 排序与比较

```java
boolean equals(Object o);
int hashCode();
```





# AbstractCollection

```java
//类声明
public abstract class AbstractCollection<E> implements Collection<E> {
}
```

## 是否包含

```java
//包含
public boolean contains(Object o) {
    Iterator<E> it = iterator();
    if (o==null) { //如果对象是NULL，则判断集合中有没有NULL值
        while (it.hasNext())
            if (it.next()==null)
                return true;
    } else { //如果不是NULL，则equals判断
        while (it.hasNext())
            if (o.equals(it.next()))
                return true;
    }
    return false;
}
```



## 转数组

* 新建 *Object* 数组
* 如果迭代器返回少数据
    * 则将数组调整至实际大小
* 如果迭代器返回多数据
    * 则将数组 扩容至 实际迭代器返回的个数
    * 扩容速率是`(n/2+1)` 
    * 超过 INT的最大值，则会 抛出 *OutOfMemoryError*

```java
public Object[] toArray() {
    // Estimate size of array; be prepared to see more or fewer elements，先以集合大小作为数组大小
    Object[] r = new Object[size()];
    Iterator<E> it = iterator();
    for (int i = 0; i < r.length; i++) {
        if (! it.hasNext()) // fewer elements than expected
            return Arrays.copyOf(r, i);
        r[i] = it.next();
    }
    return it.hasNext() ? finishToArray(r, it) : r;
}

private static <T> T[] finishToArray(T[] r, Iterator<?> it) {
    int i = r.length;
    while (it.hasNext()) {
        int cap = r.length;
        if (i == cap) {
            int newCap = cap + (cap >> 1) + 1;
            // overflow-conscious code
            if (newCap - MAX_ARRAY_SIZE > 0)
                newCap = hugeCapacity(cap + 1);
            r = Arrays.copyOf(r, newCap);
        }
        r[i++] = (T)it.next();
    }
    // trim if overallocated
    return (i == r.length) ? r : Arrays.copyOf(r, i);
}

private static int hugeCapacity(int minCapacity) {
    if (minCapacity < 0) // overflow
        throw new OutOfMemoryError
        ("Required array size too large");
    return (minCapacity > MAX_ARRAY_SIZE) ?
        Integer.MAX_VALUE :
    MAX_ARRAY_SIZE;
}
```

## 指定运行时类型转数组

* 如果数组大小 足够，则使用传入的数组，不够则 反射实例化一个
* 当 迭代器返回的 元素个数不够时
    * 如果使用的是 传入的数组 则 置为NULL
    * 如果使用的不是 传入的数组，但时 传入数组大小 大于 实际迭代器返回的元素个数，则返回 传入的元素数组
    * 如果使用的不是 传入的数组，且传入数组大小 小于 实际迭代器返回的元素个数，则重新调整数组大小并返回
* 当迭代器返回的 比预期多的 元素时
    * 重新调整数组大小

```java
public <T> T[] toArray(T[] a) {
    // Estimate size of array; be prepared to see more or fewer elements
    int size = size();
    T[] r = a.length >= size ? a :
              (T[])java.lang.reflect.Array
              .newInstance(a.getClass().getComponentType(), size);
    Iterator<E> it = iterator();

    for (int i = 0; i < r.length; i++) {
        if (! it.hasNext()) { // fewer elements than expected
            if (a == r) {
                r[i] = null; // null-terminate
            } else if (a.length < i) {
                return Arrays.copyOf(r, i);
            } else {
                System.arraycopy(r, 0, a, 0, i);
                if (a.length > i) {
                    a[i] = null;
                }
            }
            return a;
        }
        r[i] = (T)it.next();
    }
    // more elements than expected
    return it.hasNext() ? finishToArray(r, it) : r;
}
```

## Remove

* 如果是 NULL 则移除 第一个为NULL的
* 如果不是NULL，则移除 相等的为NULL的

```java
public boolean remove(Object o) {
    Iterator<E> it = iterator();
    if (o==null) {
        while (it.hasNext()) {
            if (it.next()==null) {
                it.remove();
                return true;
            }
        }
    } else {
        while (it.hasNext()) {
            if (o.equals(it.next())) {
                it.remove();
                return true;
            }
        }
    }
    return false;
}
```

## *containsAll*

循环调用*Contains*

```java
public boolean containsAll(Collection<?> c) {
    for (Object e : c)
        if (!contains(e))
            return false;
    return true;
}
```

## addAll

循环调用 *add*

```java
public boolean addAll(Collection<? extends E> c) {
    boolean modified = false;
    for (E e : c)
        if (add(e))
            modified = true;
    return modified;
}
```

## *removeAll*

循环调用 迭代器的remove

```java
public boolean removeAll(Collection<?> c) {
    Objects.requireNonNull(c);
    boolean modified = false;
    Iterator<?> it = iterator();
    while (it.hasNext()) {
        if (c.contains(it.next())) {
            it.remove();
            modified = true;
        }
    }
    return modified;
}
```

## retainAll

```java
public boolean retainAll(Collection<?> c) {
    Objects.requireNonNull(c);
    boolean modified = false;
    Iterator<E> it = iterator();
    while (it.hasNext()) {
        if (!c.contains(it.next())) {
            it.remove();
            modified = true;
        }
    }
    return modified;
}
```

## clear

```java
public void clear() {
    Iterator<E> it = iterator();
    while (it.hasNext()) {
        it.next();
        it.remove();
    }
}
```

## 总结

利用 迭代器 实现了 contains与 remove的语义 ，add的语义还未实现





