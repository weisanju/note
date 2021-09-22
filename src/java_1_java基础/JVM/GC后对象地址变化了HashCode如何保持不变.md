### HashCode的约定

在java.lang.Object的JavaDoc注释上对hashCode方法有三项约定

1. 当一个对象equals方法所使用的字段不变时，多次调用hashCode方法的值应保持不变
2. 如果两个对象equals(Object o)方法是相等的，则hashCode方法值必须相等
3. 如果两个对象equals(Object o)方法是不相等，则hashCode方法值不要求相等，但在这种情况下尽量确保hashCode不同，以提升性能。



# 验证

```java
public static void main(String[] args) {
    Object obj = new Object();
    //获取当前地址
    long address = VM.current().addressOf(obj);
    //取hashCode
    long hashCode = obj.hashCode();
    System.out.println("before GC : The memory address is " + address);
    System.out.println("before GC : The hash code is " + hashCode);

    new Object();
    new Object();
    new Object();

    System.gc();

    long afterAddress = VM.current().addressOf(obj);
    long afterHashCode = obj.hashCode();
    System.out.println("after GC : The memory address is " + afterAddress);
    System.out.println("after GC : The hash code is " + afterHashCode);
    System.out.println("---------------------");

    System.out.println("memory address = " + (address == afterAddress));
    System.out.println("hash code = " + (hashCode == afterHashCode));
}
```

# hashCode不变的原理

**存储到对象头**

原来的hashcode值被存储在了某个地方，以备再用。对此以Hotspot为例，最直接的实现方式就是在对象的header区域中划分出来一部分（32位机器上是占用25位，64位机器上占用31）用来存储hashcode值。但这种方式会添加额外信息到对象中，而在大多数情况下hashCode方法并不会被调用，这就造成空间浪费

**懒计算**

当hashCode方法未被调用时，object header中用来存储hashcode的位置为0，只有当hashCode方法（本质上是System#identityHashCode）首次被调用时，才会计算对应的hashcode值，并存储到object header中。当再次被调用时，则直接获取计算好hashcode即可。

# hashcode生成的方式

不同的JVM对hashcode值的生成方式不同。Open JDK中提供了6中生成hash值的方法。

- 0：随机数生成器（A randomly generated number.）；
- 1：通过对象内存地址的函数生成（A function of memory address of the object.）；
- 2：硬编码1（用于敏感度测试）（A hardcoded 1 (used for sensitivity testing.)）；
- 3：通过序列（A sequence.）；
- 4：对象的内存地址，强制转换为int。（The memory address of the object, cast to int.）
- 5：线程状态与xorshift结合（Thread state combined with xorshift）；

其中在OpenJDK6、7中使用的是随机数生成器的（第0种）方式，OpenJDK8、9则采用第5种作为默认的生成方式。所以，单纯从OpenJDK的实现来说，其实hashcode的生成与对象内存地址没有什么关系。而Object类中hashCode方法上的注释，很有可能是早期版本中使用到了第4种方式。



# hashCode与identityHashCode

**重写hashCode**

上面我们多次提到hashCode方法，还提到identityHashCode方法，如果单纯以Object类中的hashCode方法来说，它与System类中提供了的identityHashCode方法是一致的

但在实践中我们往往会重写hashCode方法，此时object header中存储的hashcode值便有两种情况，一个是父类Object的，一个是实现类的。

**实时调用**

在OpenJDK中，header中存储的是通过System#identityHashCode获得的hashcode，而重写的hashCode方法的子类的hashcode则是通过实时调用其实现方法获得的。



**获取固定HashCode**

```text
System.identityHashCode(person)
```



# 验证JVM hashCode懒存储

```java
public static void main(String[] args) {
    Object obj = new Object();

    System.out.println(ClassLayout.parseInstance(obj).toPrintable());
    System.out.println(Integer.toHexString(obj.hashCode()));

    System.out.println(ClassLayout.parseInstance(obj).toPrintable());

}
```

