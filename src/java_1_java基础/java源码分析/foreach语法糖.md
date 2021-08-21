foreach是 Java中的 语法糖



# 数组

如果遇到数组，则将 其 编译成普通 for循环

**编译前**

```java
        String[] a = new String[]{"1","2"};

        for (String s : a) {
            System.out.println(s);
        }
```

**编译后**

```java
        String[] a = new String[]{"1", "2"};
        String[] var8 = a;
        int var4 = a.length;

        for(int var5 = 0; var5 < var4; ++var5) {
            String s = var8[var5];
            System.out.println(s);
        }
```



# 迭代器

如果遇到集合等其他实现了迭代器的接口，则编译成 迭代器 迭代

```java
        List<Integer> integers = Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8);
        for (Integer integer : integers) {
            System.out.println(integer);
        }
```

```java
        List<Integer> integers = Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8);
        Iterator var2 = integers.iterator();

        while(var2.hasNext()) {
            Integer integer = (Integer)var2.next();
            System.out.println(integer);
   s     }
```