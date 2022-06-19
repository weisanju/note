# List接口

## 查询操作

```java
int size();
boolean isEmpty();
boolean contains(Object o);
Iterator<E> iterator();
Object[] toArray();
<T> T[] toArray(T[] a);
```

## 修改操作

```java
boolean add(E e);
boolean remove(Object o);
boolean containsAll(Collection<?> c);
boolean addAll(Collection<? extends E> c);
boolean addAll(int index, Collection<? extends E> c);
boolean removeAll(Collection<?> c);
boolean retainAll(Collection<?> c);
default void replaceAll(UnaryOperator<E> operator) {
    Objects.requireNonNull(operator);
    final ListIterator<E> li = this.listIterator();
    while (li.hasNext()) {
    	li.set(operator.apply(li.next()));
    }
}
void clear();
```



## 排序

* 先取出数组
* 后调用 数组静态排序方法
* 然后设置回当前 *ListIterator*

```java
default void sort(Comparator<? super E> c) {
    Object[] a = this.toArray();
    Arrays.sort(a, (Comparator) c);
    ListIterator<E> i = this.listIterator();
    for (Object e : a) {
        i.next();
        i.set((E) e);
    }
}
```



## 基于索引的操作

```java
E get(int index);
E set(int index, E element);
void add(int index, E element);
E remove(int index);
int indexOf(Object o);
int lastIndexOf(Object o);
```



## 基于索引的迭代器

```java
ListIterator<E> listIterator();
ListIterator<E> listIterator(int index);
```



## 基于索引的视图

```java
List<E> subList(int fromIndex, int toIndex);
```



## 总结

* List接口 增加了 基于索引的方法
    * 包括基于索引的增删查改
    * 以及基于索引的 视图
* 同时提供了  基于 list迭代器  的两个默认实现 *sort* 与 *replaceAll* 



# *AbstractList*

## 类声明

```java
public abstract class AbstractList<E> extends AbstractCollection<E> implements List<E> {
	
}
```

## 未实现方法

```java
public void add(int index, E element) {}
abstract public E get(int index);
public E set(int index, E element) {}
public E remove(int index) {}
```

## 搜索

**根据对象本身查找对象索引**

* 依赖 *ListIterator* 实现
* 从头往后查找

```java
public int indexOf(Object o) {
    ListIterator<E> it = listIterator();
    if (o==null) {
        while (it.hasNext())
            if (it.next()==null)
                return it.previousIndex();
    } else {
        while (it.hasNext())
            if (o.equals(it.next()))
                return it.previousIndex();
    }
    return -1;
}
```

**查找最后一次出现的**

* 依赖 *ListIterator* 实现
* 从后往前查找

```java
public int lastIndexOf(Object o) {
    ListIterator<E> it = listIterator(size());
    if (o==null) {
        while (it.hasPrevious())
            if (it.previous()==null)
                return it.nextIndex();
    } else {
        while (it.hasPrevious())
            if (o.equals(it.previous()))
                return it.nextIndex();
    }
    return -1;
}
```

## 移除

* 依赖 *ListIterator* 实现
* 从前往后 移除范围内的元素

```java
public void clear() {
    removeRange(0, size());
}

protected void removeRange(int fromIndex, int toIndex) {
    ListIterator<E> it = listIterator(fromIndex);
    for (int i=0, n=toIndex-fromIndex; i<n; i++) {
        it.next();
        it.remove();
    }
}
```

## 比较

* 快速比较内存地址

    如果内存地址相等 则返回 True

* 类型判断 判断是否是*List*

* 依赖 *ListIterator*

* 空值与空值相等

```java
public boolean equals(Object o) {
    if (o == this)
        return true;
    if (!(o instanceof List))
        return false;

    ListIterator<E> e1 = listIterator();
    ListIterator<?> e2 = ((List<?>) o).listIterator();
    while (e1.hasNext() && e2.hasNext()) {
        E o1 = e1.next();
        Object o2 = e2.next();
        if (!(o1==null ? o2==null : o1.equals(o2)))
            return false;
    }
    return !(e1.hasNext() || e2.hasNext());
}
```

## *hashCode*

* 将每个元素的hashCode累加
* 确保了，*equals* 相等 则*hashCode* 不相等

```java
public int hashCode() {
    int hashCode = 1;
    for (E e : this)
        hashCode = 31*hashCode + (e==null ? 0 : e.hashCode());
    return hashCode;
}
```

## 普通迭代器子类

### **变量释义**

***lastRet***

上一次访问的 索引位置，如果没有访问或者 元素被删除了，则置为-1

***cursor***

下一次访问的索引

***expectedModCount***

当生成迭代器时，记录当前集合被修改的次数

```java
private class Itr implements Iterator<E> {
    /**
     * Index of element to be returned by subsequent call to next.
     */
    int cursor = 0;

    /**
     * Index of element returned by most recent call to next or
     * previous.  Reset to -1 if this element is deleted by a call
     * to remove.
     */
    int lastRet = -1;

    /**
     * The modCount value that the iterator believes that the backing
     * List should have.  If this expectation is violated, the iterator
     * has detected concurrent modification.
     */
    int expectedModCount = modCount;

    //是否有下一个
    public boolean hasNext() {
        return cursor != size();
    }
    //取下一个，记录 lastRet，移动cursor
    public E next() {
        checkForComodification();
        try {
            int i = cursor;
            E next = get(i);
            lastRet = i;
            cursor = i + 1;
            return next;
        } catch (IndexOutOfBoundsException e) {
            checkForComodification();
            throw new NoSuchElementException();
        }
    }

    //根据索引，移除最近的元素，并更新 modCount
    public void remove() {
        if (lastRet < 0)
            throw new IllegalStateException();
        checkForComodification();

        try {
            AbstractList.this.remove(lastRet);
            if (lastRet < cursor)
                cursor--;
            lastRet = -1;
            expectedModCount = modCount;
        } catch (IndexOutOfBoundsException e) {
            throw new ConcurrentModificationException();
        }
    }

    final void checkForComodification() {
        if (modCount != expectedModCount)
            throw new ConcurrentModificationException();
    }
}
```

## ListIterator迭代器

List迭代器 与  普通迭代器相比，多了以下几个特性

* 可以 往前迭代
* 且可以获取索引，
* 可以修改当前位置的值，可以在最后插入值

**接口定义**

```java
public interface ListIterator<E> extends Iterator<E> {

	boolean hasNext();
    boolean hasPrevious();
   
    
	E next();

	E previous();
	int nextIndex();
	int previousIndex();
	
	
	void remove();
	
	void set(E e);
	void add(E e);
}
```

**实现**

* *ListItr* 继承于 *Itr*
* 可以指定 *cursor*
* 迭代器的头为0
* *previos* 实现为 *cursor-1* 取得

```java
private class ListItr extends Itr implements ListIterator<E> {
    ListItr(int index) {
        cursor = index;
    }

    public boolean hasPrevious() {
        return cursor != 0;
    }

    public E previous() {
        checkForComodification();
        try {
            int i = cursor - 1;
            E previous = get(i);
            lastRet = cursor = i;
            return previous;
        } catch (IndexOutOfBoundsException e) {
            checkForComodification();
            throw new NoSuchElementException();
        }
    }

    public int nextIndex() {
        return cursor;
    }

    public int previousIndex() {
        return cursor-1;
    }

    public void set(E e) {
        if (lastRet < 0)
            throw new IllegalStateException();
        checkForComodification();

        try {
            AbstractList.this.set(lastRet, e);
            expectedModCount = modCount;
        } catch (IndexOutOfBoundsException ex) {
            throw new ConcurrentModificationException();
        }
    }

    public void add(E e) {
        checkForComodification();

        try {
            int i = cursor;
            AbstractList.this.add(i, e);
            lastRet = -1;
            cursor = i + 1;
            expectedModCount = modCount;
        } catch (IndexOutOfBoundsException ex) {
            throw new ConcurrentModificationException();
        }
    }
}
```





## 总结

* *AbstractList* 提供了 基于 List迭代器，根据 对象查询索引的实现
* 提供了 基于 List迭代器的 范围删除的实现
* 实现了 equals 与 hashCode的实现

