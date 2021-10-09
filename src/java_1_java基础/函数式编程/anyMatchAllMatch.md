Stream中 *anyMatch* 跟 *allMatch* 对于空集合的 作用





anyMatch：只要有一个满足即可退出

**等价代码**

```java
    public static <T>boolean anyMatch(List<T> target, Predicate<T> predicate){
        for (T t : target) {
            if(predicate.test(t)){
                return true;
            }
        }
        return false;
    }
```

*allMatch*: 只要有一个不满足条件，则返回false

**等价代码**

```java
    public static <T>boolean allMatch(List<T> target, Predicate<T> predicate){
        for (T t : target) {
            if(!predicate.test(t)){
                return false;
            }
        }
        return true;
    }
```



