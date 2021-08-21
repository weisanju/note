# 比较

```java
public static boolean equals(Object a, Object b)
```

```java
public static boolean deepEquals(Object a, Object b)
```



# **取Hash**

```java
public static int hashCode(Object o)
    
public static int hash(Object... values)
```



# ToString()

```java
public static String toString(Object o)
    
public static String toString(Object o, String nullDefault)

```



# 比较

```java
public static <T> int compare(T a, T b, Comparator<? super T> c)
```





# 非空判断

```java
public static <T> T requireNonNull(T obj)
public static <T> T requireNonNull(T obj, String message)
public static boolean isNull(Object obj)
public static boolean nonNull(Object obj)
public static <T> T requireNonNull(T obj, Supplier<String> messageSupplier)
```

