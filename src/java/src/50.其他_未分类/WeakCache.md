# 前言

WeakCache<K,P,V>中，K代表key值，P代表参数，V代表存储的值。

此类用于缓存{（key，sub-key）-->value}键值对

 Keys and values are 弱引用 sub-keys are 强引用. 键直接传递给 get 方法，该方法也接受一个参数

sub-keys 是使用 *subKeyFactory*  从键计算出来的，使用传递给构造函数的 valueFactory 函数从 keys and parameter 中计算 values，键可以为空并通过标识进行比较，而 subKeyFactory 返回的子键或 valueFactory 返回的值不能为空

sub-keys使用它们的 equals 方法比较

当 keys的 WeakReferences 被清除时,每次get、containsValue 、 size methods   调用时都会从缓存中懒惰地删除条目

清除对单个值的 WeakReferences 不会导致删除，但这些条目在逻辑上被视为不存在，并根据请求对其键/子键触发重新评估 valueFactory。





# 实现

```java
//缓存Map
private final ConcurrentMap<Object, ConcurrentMap<Object, Supplier<V>>> map = new ConcurrentHashMap<>();
// 用于 size、containsValue，快速查找
private final ConcurrentMap<Supplier<V>, Boolean> reverseMap = new ConcurrentHashMap<>();
```





# 构造方法

```java
public WeakCache(BiFunction<K, P, ?> subKeyFactory, BiFunction<K, P, V> valueFactory) {
	this.subKeyFactory = Objects.requireNonNull(subKeyFactory);
	this.valueFactory = Objects.requireNonNull(valueFactory);
}
```

这样我们就可以通过subKeyFactory ，valueFactory 获取对应的子键与值。

# 静态内部类

## CacheValue

静态内部类，实际上就是用于存储一个值的对象

```java
private interface Value<V> extends Supplier<V> {}

@FunctionalInterface
public interface Supplier<T> {
    T get();
}

```

## hashCode与identityHashCode

在Object类中的hashCode可以获取相应对象的hashCode，而这个identityHashCode也是可以获取对象的hashCode，那么两这有什么不同吗？从源码看两者都是本地方法（native），实际上获取时的结果是与hashCode无异的，但是这里的hashCode指的是原有的Object中的hashCode的方法，如果进行了重写就可能会有不同了，所以为了得到原有的Object中的hashCode的值，identityHashCode会比较方便。


## LookupValue

静态内部类，为了便于对CacheValue中的值进行判断，建立了LookupValue，也实现了Value接口，是CacheValue运算时的替代，实现方式也很相似。

```java
    private static final class LookupValue<V> implements Value<V> {
        private final V value;

        LookupValue(V value) {
            this.value = value;
        }

        @Override
        public V get() {
            return value;
        }

        @Override
        public int hashCode() {
            return System.identityHashCode(value); // compare by identity
        }

        @Override
        public boolean equals(Object obj) {
            return obj == this ||
                   obj instanceof Value &&
                   this.value == ((Value<?>) obj).get();  // compare by identity
        }
    }					
```

## CacheKey

```java
private static final class CacheKey<K> extends WeakReference<K> {

    // a replacement for null keys
    private static final Object NULL_KEY = new Object();

    static <K> Object valueOf(K key, ReferenceQueue<K> refQueue) {
        return key == null
               // null key means we can't weakly reference it,
               // so we use a NULL_KEY singleton as cache key
               ? NULL_KEY
               // non-null key requires wrapping with a WeakReference
               : new CacheKey<>(key, refQueue);
    }

    private final int hash;

    private CacheKey(K key, ReferenceQueue<K> refQueue) {
        super(key, refQueue);
        this.hash = System.identityHashCode(key);  // compare by identity
    }

    @Override
    public int hashCode() {
        return hash;
    }

    @Override
    public boolean equals(Object obj) {
        K key;
        return obj == this ||
               obj != null &&
               obj.getClass() == this.getClass() &&
               // cleared CacheKey is only equal to itself
               (key = this.get()) != null &&
               // compare key by identity
               key == ((CacheKey<K>) obj).get();
    }

    void expungeFrom(ConcurrentMap<?, ? extends ConcurrentMap<?, ?>> map,
                     ConcurrentMap<?, Boolean> reverseMap) {
        // removing just by key is always safe here because after a CacheKey
        // is cleared and enqueue-ed it is only equal to itself
        // (see equals method)...
        ConcurrentMap<?, ?> valuesMap = map.remove(this);
        // remove also from reverseMap if needed
        if (valuesMap != null) {
            for (Object cacheValue : valuesMap.values()) {
                reverseMap.remove(cacheValue);
            }
        }
    }
}
```



# GET方法



```java
public V get(K key, P parameter) {
    Objects.requireNonNull(parameter);
// 清除 引用队列 过期的 key
    expungeStaleEntries();


    Object cacheKey = CacheKey.valueOf(key, refQueue);

    //来加载设置并获取一级缓存
    // lazily install the 2nd level valuesMap for the particular cacheKey
    ConcurrentMap<Object, Supplier<V>> valuesMap = map.get(cacheKey);
    if (valuesMap == null) {
        ConcurrentMap<Object, Supplier<V>> oldValuesMap
            = map.putIfAbsent(cacheKey,
                              valuesMap = new ConcurrentHashMap<>());
        if (oldValuesMap != null) {
            valuesMap = oldValuesMap;
        }
    }

    // create subKey and retrieve the possible Supplier<V> stored by that
    // subKey from valuesMap
    Object subKey = Objects.requireNonNull(subKeyFactory.apply(key, parameter));
    //计算二级key,获取二级缓存
    Supplier<V> supplier = valuesMap.get(subKey);
    Factory factory = null;

    while (true) {
        //二级缓存在
        if (supplier != null) {
            // supplier might be a Factory or a CacheValue<V> instance
            //获取实际值
            V value = supplier.get();
            //二级缓存不为空，直接返回
            if (value != null) {
                return value;
            }
        }
        //没有supplier、
        //或者 value被回收了
        // else no supplier in cache
        // or a supplier that returned null (could be a cleared CacheValue
        // or a Factory that wasn't successful in installing the CacheValue)

        //没有factory新建一个 FactorySupplier
        // lazily construct a Factory
        if (factory == null) {
            factory = new Factory(key, parameter, subKey, valuesMap);
        }
        
        //没有supplier
        if (supplier == null) {
            //放入supplier
            supplier = valuesMap.putIfAbsent(subKey, factory);
            //返回空 则证明成功按爪那个
            if (supplier == null) {
                // successfully installed Factory
                supplier = factory;
            }
            //不反回空，则说明同一时刻  有其他线程放入了 supplier,则继续下一轮循环
            // else retry with winning supplier
        } else {
            //存在supplier，但是值没了：放入新值
            if (valuesMap.replace(subKey, supplier, factory)) {
                // successfully replaced
                // cleared CacheEntry / unsuccessful Factory
                // with our Factory
                supplier = factory;
            } else {
                //放入失败:同一时刻有人在尝试操作此键：获取它并操作
                // retry with current supplier
                supplier = valuesMap.get(subKey);
            }
        }
    }
}
```

# 清除Key的无效引用

将无效弱引用队列的值拿出来，一一从一级缓存中移除



```java
private void expungeStaleEntries() {
    CacheKey<K> cacheKey;
    while ((cacheKey = (CacheKey<K>)refQueue.poll()) != null) {
        cacheKey.expungeFrom(map, reverseMap);
    }
}


void expungeFrom(ConcurrentMap<?, ? extends ConcurrentMap<?, ?>> map,
                 ConcurrentMap<?, Boolean> reverseMap) {
    // removing just by key is always safe here because after a CacheKey
    // is cleared and enqueue-ed it is only equal to itself
    // (see equals method)...
    ConcurrentMap<?, ?> valuesMap = map.remove(this);
    // remove also from reverseMap if needed
    if (valuesMap != null) {
        for (Object cacheValue : valuesMap.values()) {
            reverseMap.remove(cacheValue);
        }
    }
}
```

# 计算Value

java.lang.reflect.WeakCache.Factory#get

```java
public synchronized V get() { // serialize access
    // re-check
    Supplier<V> supplier = valuesMap.get(subKey);
    if (supplier != this) {
        //返回null表明：1.被CacheValue替代了  2. 值 被GC可能将其回收了,其他线程新了一个Supplier
        // something changed while we were waiting:
        // might be that we were replaced by a CacheValue
        // or were removed because of failure ->
        // return null to signal WeakCache.get() to retry
        // the loop
        return null;
    }
    // else still us (supplier == this)

    // create new value
    V value = null;
    try {
        //计算值
        value = Objects.requireNonNull(valueFactory.apply(key, parameter));
    } finally {
        if (value == null) { // remove us on failure
            valuesMap.remove(subKey, this);
        }
    }
    // the only path to reach here is with non-null value
    assert value != null;

    // wrap value with CacheValue (WeakReference)
    CacheValue<V> cacheValue = new CacheValue<>(value);

    // put into reverseMap：放入索引Map
    reverseMap.put(cacheValue, Boolean.TRUE);

    //替换 当前factory为 CacheValue
    // try replacing us with CacheValue (this should always succeed)
    if (!valuesMap.replace(subKey, this, cacheValue)) {
        throw new AssertionError("Should not reach here");
    }

    // successfully replaced us with new CacheValue -> return the value
    // wrapped by it
    return value;
}
```

