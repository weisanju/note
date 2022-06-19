# JavaUnsafe类与CAS操作

## 前言

最近看java源码发现有多处地方都使用到了Unsafe类,于是在网上查阅资料教程.以下是个人总结

## Unsafe简介

Unsafe两大功能:

1. 直接通过内存地址 修改对象,获取对象引用
2. 使用硬件指令 实现 原子操作 (CAS compare and swap)

Unsafe的使用:

1. Unsafe是典型的单例模式,通过  `public static Unsafe getUnsafe()`获取实例

2. 且 该方法被 `@CallerSensitive`所注解, 表明只能由系统类加载器加载的类所调用

3. 为了在测试代码中使用Unsafe,可以通过反射获取该类的静态字段的实例

   ```java
   Field f= Unsafe.class.getDeclaredField("theUnsafe");
   f.setAccessible(true);
   Unsafe u = (Unsafe) f.get(null);
   ```

   

## Unsafe API

### 获取偏移

1. 获取成员变量在 对象中的偏移

   `public native long objectFieldOffset(Field f);`

2. 获取静态成员所在 的类,返回`Class`对象

   `public native Object staticFieldBase(Field f);`

3. 获取静态成员在 类中的偏移

   `public native long staticFieldOffset(Field f);`

4. 获取数组首个元素 在数组对象中的偏移

   `public native int arrayBaseOffset(Class arrayClass);`

5. 获取每个数组元素所占空间

   `public native int arrayIndexScale(Class arrayClass);`



### 根据 对象+偏移  获取或设置 对象中字段的引用或值

1. 获取 对象var1内部中偏移为var2的 XXX类型字段的 值或引用

   ```java
   public native byte getXxxx(Object var1, long var2);
   例如
      public native byte getByte(Object var1, long var2);
      public native int getInt(Object var1, long var2);
      public native double getDouble(long var1);
      public native boolean getBoolean(Object var1, long var2);
      public native Object getObject(Object var1, long var2);
   ......
   ```

   

2. 设置对象var1内部中偏移为var2的 XXX类型字段的值 为var4

   ```java
    public native void putBoolean(Object var1, long var2, boolean var4);
    public native void putByte(Object var1, long var2, byte var4);
    public native void putInt(Object var1, long var2, int var4);
    public native void putObject(Object var1, long var2, Object var4);
   ......
   ```

3. 带`volatile`语义的`get,put`:表示多线程之间的变量可见,一个线程修改一个变量之后,另一个线程立刻能看到

   ```JAVA
   public native void putBooleanVolatile(Object var1, long var2, boolean var4);
   public native int getIntVolatile(Object var1, long var2);
   public native long getLongVolatile(Object var1, long var2);
   ......
   ```

### 本地内存操作

1. 分配指定大小的一块本地内存 (同C语言中的 malloc)

   `public native long allocateMemory(long bytes);`

2. 重新分配内存(同C语言中的 realloc)

   `public native long reallocateMemory(long address, long bytes);`

3. 将给定的内存块  的所有字节 `bytes` 设置成固定的值 `value` (通过 `object + offset` 确定内存的基址)(同C语言中的 memset)

   `public native void setMemory(Object o, long offset, long bytes, byte value);`

4. 复制内存块,`内存块 srcBasc+srcOffset + bytes - > destBase+destOffset + bytes`  (同C语言中的 memcpy)

   `public native void copyMemory(Object srcBase, long srcOffset, Object destBase, long destOffset,long bytes);`

5. 释放通过Allocate分配的本地内存(同C语言中的 free)

   ` public native void freeMemory(long address);`

6. 获取和设置本地内存中的值,va1表示本地内存绝对地址,var3表示要设置的值

   ```java
   public native short getShort(long var1);
   public native int getInt(long var1);
   public native void putShort(long var1, short var3);
   public native void putInt(long var1, int var3);
   ```

   

### CAS操作

`java.util.concurrent 包中无锁化的实现就是调用了CAS以下原子操作`

0. CAS语义
   1. 将 由var1+var2确定的地址的值  从var4 修改成 var5 
   2. 如果旧值不为 var4,则直接退出
   3. 多个线程修改同一个变量时, 只会有一个线程修改成功,其他线程不会被挂起,而是告知失败
   4. 这是一种 乐观锁的语义, 每个线程都假设自己的操作能成功,与之相对应的synchronized的悲观锁语义,每次修改操作必须 只能有一个线程独占资源

1. 设置 通过 var1+var2确定的内存基址的int类型变量,将值原子的从 var4 变成 var5,成功true,失败false

   ```java
   替换int值:public final native boolean compareAndSwapInt(Object var1, long var2, int var4, int var5);
   替换引用:public final native boolean compareAndSwapObject(Object var1, long var2, Object var4, Object var5);
   ```

2. 基于上面操作的包装方法: 得到对象 中某个int字段的值 通过(var1+var2), 并给该值加上 var4,返回相加前的值

   ```Java
   典型实现
   public final int getAndAddInt(Object var1, long var2, int var4) {
           int var5;
           do {
               var5 = this.getIntVolatile(var1, var2);
           } while(!this.compareAndSwapInt(var1, var2, var5, var5 + var4));
           return var5;
       }
   ```

### Pack/Unpack

1. 阻塞和释放任一线程对象

2. 内部实现通过 信号量的方式,信号量值为1,pack 消耗值, unpack增加值

3. 在 `LockSupport `类包装使用

   

## Example

```
//测试对象
public class UnsafeEntity {
    private  int a;
    private  int c;
    private  int d;
    private  static  int b = 1;
    getter......
    setter......
}
```

```java

//测试代码
package com.weisanju;
import sun.misc.Unsafe;
import java.lang.reflect.Field;

public class UnsafeTest {
    public static void main(String[] args) throws Exception {
        Field f= Unsafe.class.getDeclaredField("theUnsafe");
        f.setAccessible(true);
        Unsafe u = (Unsafe) f.get(null);
        //获取成员变量 的偏移
        long a = u.objectFieldOffset(UnsafeEntity.class.getDeclaredField("a"));
        long c = u.objectFieldOffset(UnsafeEntity.class.getDeclaredField("c"));
        long d = u.objectFieldOffset(UnsafeEntity.class.getDeclaredField("d"));
        System.out.println("成员字段a:"+a);
        System.out.println("成员字段c:"+c);
        System.out.println("成员字段d:"+d);

        //设置对象字段的值
        UnsafeEntity testa = new UnsafeEntity();
        testa.setA(666);
        System.out.println("设置前:"+u.getInt(testa, a));
        u.putInt(testa,a,777);
        System.out.println("设置后:"+u.getInt(testa, a));

        //获取静态字段所在的类的对象
        System.out.println(u.staticFieldBase(UnsafeEntity.class.getDeclaredField("b")));
        //获取静态字段的偏移
        long b = u.staticFieldOffset(UnsafeEntity.class.getDeclaredField("b"));
        System.out.println("静态字段b:"+b);

        //静态字段的设置, 注意由于静态字段,存储于方法区,所以起始对象为类的字节码
        System.out.println("设置前:"+u.getInt(UnsafeEntity.class, b));
        u.putInt(UnsafeEntity.class,b,11);
        System.out.println("设置后:"+u.getInt(UnsafeEntity.class, b));


        //普通 数组的使用
        int arr[] = {1,2,3,4,5,6,7,8};
        //head为头地址偏移
        long head = u.arrayBaseOffset(int[].class);
        //step为数组元素所占空间
        long step = u.arrayIndexScale(int[].class);
        // 获取 与设置 arr[7] 的值
        int index = 7;
        System.out.println(u.getInt(arr, head + step * index));
        u.putInt(arr,head+step*index,666);
        System.out.println(arr[index]);

        //对象数组的使用
        UnsafeEntity arrObj[] = new UnsafeEntity[10];
        //head为头地址偏移
        head = u.arrayBaseOffset(UnsafeEntity[].class);
        //step为数组元素所占空间
        step = u.arrayIndexScale(UnsafeEntity[].class);
        // 获取 与设置 arr[7] 的值
        index = 7;
        arrObj[index] = new UnsafeEntity();
        System.out.println(u.getObject(arrObj, head + step * index));
        u.putObject(arrObj,head+step*index,new UnsafeEntity());
        System.out.println(arrObj[index]);
    }
}
```

```
输出结果
成员字段a:12
成员字段c:16
成员字段d:20
设置前:666
设置后:777
class com.weisanju.UnsafeEntity
静态字段b:104
设置前:1
设置后:11
8
666
com.weisanju.UnsafeEntity@1540e19d
com.weisanju.UnsafeEntity@677327b6
```



## 总结

1. Unsafe为从cpu底层指令 层面 为多线程提供了无锁化设计,以及直接操作内存地址的能力,Java中 Atomic原子类,netty,concurrent包等底层都封装了 该对象
2. 当然 极大的效率,也必然意外着 极大的不安全, 如果错误给一块内存区赋值,程序不会有任何反应,这就给程序带来极大的安全隐患
3. 当然了解Unsafe类 能够便于我们更好的阅读 Java底层源码

