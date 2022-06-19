# *Queue*

> **队列**

## 接口申明

```java
public interface Queue<E> extends Collection<E> {
}
```

## 插入

### 插入1

```java
boolean add(E e);
```

### 插入2

```java
boolean offer(E e);
```



## 移除

### 移除队头元素1

```java
E remove();
```

### 移除对头元素2

* 与1相比，如果为空则返回*NULL*，而1会 抛异常

```java
E poll();
```



## 取队头元素

### 取队头元素1

```java
E element();
```

### 取队头元素2

* 与1相比，如果为空则返回*NULL*，而1会 抛异常

```java
E peek();
```



# *Deque*

**双向队列**

## 声明

```java
public interface Deque<E> extends Queue<E> {}
```

## 插入元素

```java
//头部插入
void addFirst(E e);
//尾部插入
void addLast(E e);
//头部插入
boolean offerFirst(E e);
//尾部插入
boolean offerLast(E e);
```

## 移除元素

```java
E removeFirst();
E removeLast();

//以下移除空队列不会报错
E pollFirst();
E pollLast();

//移除第一个出现的对象
boolean removeFirstOccurrence(Object o);

//移除最后一个出现的对象
boolean removeLastOccurrence(Object o);
```

## 获取元素

```java
E getFirst();
E getLast();
//以下取空队列不会报错
E peekFirst();
E peekLast();
```

## 栈方法

```java
//头部压栈
void push(E e);

//头部出栈
E pop();
```

