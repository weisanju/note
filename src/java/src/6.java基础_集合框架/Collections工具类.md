# Collections中API的分类

* 排序操作
* 查找替换
* checkedxxx 检查集合
* emptyxxx 返回空集合
* synchronizedXXX 同步集合
* unmodifiableXXX 不可变集合



# 排序操作

**static <T>boolean addAIl(Collection <? super T>c,T... elements)**

将所有指定元素添加到指定的collection中

**static void reverse(List list)**

反转指定List集合中元素的顺序

**static void shuffle(List list)**

对List集合中的元素进行随机排序（模拟玩扑克中的“洗牌”）

**static void sort(List list)**

根据元素的自然顺序对List集合中的元素进行排序

**static void swap(List list,int i，int j)**

将指定List集合中i处元素和j处元素进行交换



# **查找替换**

**static int binaryScarch ( List list,Object key)**

使用二分法搜索指定对象在List集合中的索引，查找的 List集合中的元素必须是有序的

**static Object max(Collection col)**

返回给定集合中最大的元素

**static Object min (Collection col)**

返回给定集合中最小的元素

**static boolean replaccAll (List list，Object oldVal,Object newVal)**

用一个新的newVal替换List集合中所有的旧值oldVal



# checkedxxx 检查集合

返回类型检查的集合，在对类型进行set 或者add的时候会做类型检查

```java
用法：

ArrayList list = Lists.newArrayList();

list.add(new Player("香菜"));

// 返回的safeList 在add时会进行类型检查

List safeList = Collections.checkedList(list, Player.class);

//此时会对类型进行检查，不是Player类型，抛出异常 java.lang.ClassCastException:
```





# emptyxxx 返回空的集合

返回一个空集合，不能添加，不能删除



# synchronizedxxx 同步集合

对集合进行了二次包装，在内部加了一把锁



# unmodifiableXxx 不可变集合

传入的集合返回后不可以改变。

