# **测试题**

```java
@Test
public void test2() {
	Integer i1 = 64;
	int i2 = 64;

	Integer i3 = Integer.valueOf(64);
	Integer i4 = new Integer(64);

	Integer i5 = 256;
	Integer i6 = Integer.valueOf(256);

	System.out.println("A：" + (i1 == i2));
	System.out.println("B：" + (i1 == i3));
	System.out.println("C：" + (i3 == i4));
	System.out.println("D：" + (i2 == i4));
	System.out.println("E：" + (i3.equals(i4)));
	System.out.println("F：" + (i5 == i6));
}
```

# **答案**

```j
A：true
B：true
C：false
D：true
E：true
F：false
```

# **现象**

- 如果==两端有一个是基础类型(int)，则会发生自动拆箱操作，这时比较的是值。
- 如果==两端都是包装类型(Integer)，则不会自动拆箱，首先会面临缓存问题
  - 自动装箱与 Integer.valueOf(64) 会面临缓存问题, 缓存范围 在 -128~127
  - new Integer 会生成一个新对象

# **总结**

引用数据类型 使用 **equals** 比较



# 扩展

不同数值类型比较

```java
public class MainTesat {
    public static void main(String[] args) {
       Long a = 2L;
       Integer b = 2;

       System.out.println(b.equals(a));
    }
}

//false
```

**总结**

不同 数值类型 的equals 比较 会返回false