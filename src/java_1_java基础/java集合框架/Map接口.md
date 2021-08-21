# 概述

* 包含 key values 键值对的 集合
* 不能包含重复*key*
* 每个key只能映射一个值
* 该接口代替Dictionary类
* Map提供了 三个 集合视图：keys，values，entrys
* 迭代顺序不做保证
* 如果使用 可变对象 作为 *key*，则必须格外小心，如果对象中 影响 equals比较的键变了，那么map的行为可能会未知
* 禁止Map 自身作为 *Key*
* 所有 通用Map的 实现类 必须 提供 二个标准的 构造函数
    * 无参构造
    * 指定Map类型的 构造，（用作Map拷贝目的）







# 查询操作

## 大小

```
int size();
```

## 是否为空

```
boolean isEmpty();
```

## 是否包含key

```
boolean containsKey(Object key);
```

## 是否包含值

```
boolean containsValue(Object value);
```

## 根据Key获取值

```
V get(Object key);
```

# 修改操作

## 放入元素

```
V put(K key, V value);
```

## 移除元素

```
V remove(Object key);
```

# 批量操作

## 放入Map

```
void putAll(Map<? extends K, ? extends V> m);
```

## 清空Map

```
void clear();
```

# 视图

## 键集合

```
Set<K> keySet();
```

## Values集合

```
Collection<V> values();
```

## Entry集合

```
Set<Map.Entry<K, V>> entrySet();
```



# 默认方法

## **获取key,不存在则取默认值**

```java
default V getOrDefault(Object key, V defaultValue) {
    V v;
    return (((v = get(key)) != null) || containsKey(key))
        ? v
        : defaultValue;
}
```

## 迭代

```java
default void forEach(BiConsumer<? super K, ? super V> action) {
    Objects.requireNonNull(action);
    for (Map.Entry<K, V> entry : entrySet()) {
        K k;
        V v;
        try {
            k = entry.getKey();
            v = entry.getValue();
        } catch(IllegalStateException ise) {
            // this usually means the entry is no longer in the map.
            throw new ConcurrentModificationException(ise);
        }
        action.accept(k, v);
    }
}
```

## 批量更新Key

```java
default void replaceAll(BiFunction<? super K, ? super V, ? extends V> function) {
    Objects.requireNonNull(function);
    for (Map.Entry<K, V> entry : entrySet()) {
        K k;
        V v;
        try {
            k = entry.getKey();
            v = entry.getValue();
        } catch(IllegalStateException ise) {
            // this usually means the entry is no longer in the map.
            throw new ConcurrentModificationException(ise);
        }

        // ise thrown from function is not a cme.
        v = function.apply(k, v);

        try {
            entry.setValue(v);
        } catch(IllegalStateException ise) {
            // this usually means the entry is no longer in the map.
            throw new ConcurrentModificationException(ise);
        }
    }
}
```

## 不存在则更新

```java
default V putIfAbsent(K key, V value) {
    V v = get(key);
    if (v == null) {
        v = put(key, value);
    }

    return v;
}
```

## 根据KeyValue移除

```java
default boolean remove(Object key, Object value) {
    Object curValue = get(key);
    if (!Objects.equals(curValue, value) ||
        (curValue == null && !containsKey(key))) {
        return false;
    }
    remove(key);
    return true;
}
```



## 替换指定KeyValue的元素的值

```
default boolean replace(K key, V oldValue, V newValue) {
    Object curValue = get(key);
    if (!Objects.equals(curValue, oldValue) ||
        (curValue == null && !containsKey(key))) {
        return false;
    }
    put(key, newValue);
    return true;
}
```

## 替换指定Key的值

```java
default V replace(K key, V value) {
    V curValue;
    if (((curValue = get(key)) != null) || containsKey(key)) {
        curValue = put(key, value);
    }
    return curValue;
}
```

## 不存在则放入元素1

```java
default V computeIfAbsent(K key,
        Function<? super K, ? extends V> mappingFunction) {
    Objects.requireNonNull(mappingFunction);
    V v;
    if ((v = get(key)) == null) {
        V newValue;
        if ((newValue = mappingFunction.apply(key)) != null) {
            put(key, newValue);
            return newValue;
        }
    }

    return v;
}
```

## 不存在则放入元素2

```java
default V computeIfPresent(K key,
        BiFunction<? super K, ? super V, ? extends V> remappingFunction) {
    Objects.requireNonNull(remappingFunction);
    V oldValue;
    if ((oldValue = get(key)) != null) {
        V newValue = remappingFunction.apply(key, oldValue);
        if (newValue != null) {
            put(key, newValue);
            return newValue;
        } else {
            remove(key);
            return null;
        }
    } else {
        return null;
    }
}
```



## 根据Mapping返回的结果更新 或移除 key

*  key，与oldValue 映射为 null，则移除 该 key
* 如果映射不为 null，则放入元素

```java
default V compute(K key,
        BiFunction<? super K, ? super V, ? extends V> remappingFunction) {
    Objects.requireNonNull(remappingFunction);
    V oldValue = get(key);

    V newValue = remappingFunction.apply(key, oldValue);
    if (newValue == null) {
        // delete mapping
        if (oldValue != null || containsKey(key)) {
            // something to remove
            remove(key);
            return null;
        } else {
            // nothing to do. Leave things as they were.
            return null;
        }
    } else {
        // add or replace old mapping
        put(key, newValue);
        return newValue;
    }
}
```

## 合并

如果旧值 为*NULL*，则直接用新值

如果旧值 不为 NULL，则两者的值进行合并

合并的返回值 为 NULL 则移除 该key

不为NULL 则  放入key

```java
default V merge(K key, V value,
        BiFunction<? super V, ? super V, ? extends V> remappingFunction) {
    Objects.requireNonNull(remappingFunction);
    Objects.requireNonNull(value);
    V oldValue = get(key);
    V newValue = (oldValue == null) ? value :
               remappingFunction.apply(oldValue, value);
    if(newValue == null) {
        remove(key);
    } else {
        put(key, newValue);
    }
    return newValue;
}
```

