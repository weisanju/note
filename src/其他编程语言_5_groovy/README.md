# Groovy 概述

Groovy是一种基于Java平台的面向对象语言

Groovy中有以下特点:

- 同时支持静态和动态类型。
- 支持运算符重载。
- 本地语法列表和关联数组。
- 对正则表达式的本地支持。
- 各种标记语言，如XML和HTML原生支持。
- Groovy对于Java开发人员来说很简单，因为Java和Groovy的语法非常相似。
- 您可以使用现有的Java库。
- Groovy扩展了java.lang.Object。





# 与Java的不同之处

## 默认 imports

所有这些包和类都是默认导入的，您不必使用显式import语句来使用它们：

```java
java.io.*
java.lang.*
java.math.BigDecimal
java.math.BigInteger
java.net.*
java.util.*
groovy.lang.*
groovy.util.*
```

## 多方法运行时选择

1. Java的方法调用是在编译时期 根据变量的类型 决定调用哪个方法
2. 而 groovy 是 在运行时 根据运行时类型选择调用方法

```java
int method(String arg) {
    return 1;
}
int method(Object arg) {
    return 2;
}
Object o = "Object";
int result = method(o);
```

```sh
#In Java, you would have:
assertEquals(2, result);
# Whereas in Groovy: 
assertEquals(1, result);
```

## 数组初始化

```java
// java
int [] array = {1，2，3}

// groovy
int [] array = [1,2,3]
```

## 包可见性

默认情况下，java的成员变量 可见性 是 包私有的

*groovy* 是 私有的，但会自动生成 *get set*

```groovy
class Person {
    String name
}

class Person {
    @PackageScope String name
}
```

## ARM blocks

> 自动资源管理 块

**Java7的自动资源管理块**

```java
Path file = Paths.get("/path/to/file");
Charset charset = Charset.forName("UTF-8");
try (BufferedReader reader = Files.newBufferedReader(file, charset)) {
    String line;
    while ((line = reader.readLine()) != null) {
        System.out.println(line);
    }

} catch (IOException e) {
    e.printStackTrace();
}
```

**groovy提供 各种闭包方法实现自动资源管理**

```groovy
new File('/path/to/file').eachLine('UTF-8') {
   println it
}

new File('/path/to/file').withReader('UTF-8') { reader ->
   reader.eachLine {
       println it
   }
}
```

## 内部类

**静态内部类**

```sh
class A {
    static class B {}
}

new A.B()
```

**匿名内部类**

```groovy
import java.util.concurrent.CountDownLatch
import java.util.concurrent.TimeUnit

CountDownLatch called = new CountDownLatch(1)

Timer timer = new Timer()
timer.schedule(new TimerTask() {
    void run() {
        called.countDown()
    }
}, 0)

assert called.await(10, TimeUnit.SECONDS)
```

**非静态内部类**

```java
//java
public class Y {
    public class X {}
    public X foo() {
        return new X();
    }
    public static X createX(Y y) {
        return y.new X();
    }
}
```



```groovy
//grovvy
public class Y {
    public class X {}
    public X foo() {
        return new X()
    }
    public static X createX(Y y) {
        return new X(y)
    }
}
```





## Lambda表达式和方法引用

```java
Runnable run = () -> System.out.println("Run");  // Java
list.forEach(System.out::println);

Runnable run = { println 'run' }
list.each { println it } // or list.each(this.&println)
```

## GStrings

1. 由于双引号字符串字面量被解释为`GString`，Groovy将在GString和String之间自动转换

2. Groovy中的单引号用于String，双引号结果是String或GString，**取决于文字中是否有插值**
3. 只有在赋给char类型的变量时，Groovy会自动将单字符String转换为char
4. 当调用类型为char的参数的方法时，我们需要显式转换或确保该值已预先转换。

```.dart
assert 'c'.getClass()==String
assert "c".getClass()==String
assert "c${1}".getClass() in GString
```



Groovy支持两种类型的转换，在转换为char的情况下，在转换multi-char 时存在微妙的差别。 Groovy风格的转换是更宽松的，将采取第一个字符，而C风格的转换将失败，异常。



```java
// for single char strings, both are the same
assert ((char) "c").class==Character
assert ("c" as char).class==Character

// for multi char strings they are not
try {
  ((char) 'cx') == 'c'
  assert false: 'will fail - not castable'
} catch(GroovyCastException e) {
}
assert ('cx' as char) == 'c'
assert 'cx'.asType(char) == 'c'
```

## 原始类型和包装类型

因为Groovy使用Objects来做每一件事，它对原始的引用[自动包装](https://links.jianshu.com/go?to=http%3A%2F%2Fdocs.groovy-lang.org%2Flatest%2Fhtml%2Fdocumentation%2Fcore-object-orientation.html)。 因此，它不遵循Java的扩展优先于装箱。 这里有一个使用int的例子

```cpp
int i
m(i)

//这是Java将调用的方法，因为扩展优先于装箱。
void m(long l) {
  println "in m(long)"
}

//这是Groovy实际调用的方法，因为所有的基本引用都使用它们的包装类。
void m(Integer i) {
  println "in m(Integer)"
}
```

## ==的行为

在Java中==表示对象的原始类型或标识的相等性。 在Groovy ==翻译为a.compareTo(b)== 0，如果他们是可比较的，否则a.equals(b)。 如果要检查身份，有is方法，例如a.is(b)

或者使用 === 判断对象是否完全相等



## Conversions

Java 会自动进行扩大和缩小转换。

|                   | ** Converts to** |          |           |          |         |          |           |            |
| ----------------- | ---------------- | -------- | --------- | -------- | ------- | -------- | --------- | ---------- |
| **Converts from** | **boolean**      | **byte** | **short** | **char** | **int** | **long** | **float** | **double** |
| **boolean**       | -                | N        | N         | N        | N       | N        | N         | N          |
| **byte**          | N                | -        | Y         | C        | Y       | Y        | Y         | Y          |
| **short**         | N                | C        | -         | C        | Y       | Y        | Y         | Y          |
| **char**          | N                | C        | C         | -        | Y       | Y        | Y         | Y          |
| **int**           | N                | C        | C         | C        | -       | Y        | T         | Y          |
| **long**          | N                | C        | C         | C        | C       | -        | T         | T          |
| **float**         | N                | C        | C         | C        | C       | C        | -         | Y          |
| **double**        | N                | C        | C         | C        | C       | C        | C         | -          |

'Y' 表示 Java 可以进行的转换，'C' 表示存在显式强制转换时 Java 可以进行的转换，'T' 表示 Java 可以进行的转换但数据被截断，'N' 表示 Java 不可以进行的转换

**Groovy 在这方面做了很大的扩展。**



|                   | Converts to |             |          |          |           |           |          |               |         |             |          |          |                |           |           |            |            |                |
| ----------------- | ----------- | ----------- | -------- | -------- | --------- | --------- | -------- | ------------- | ------- | ----------- | -------- | -------- | -------------- | --------- | --------- | ---------- | ---------- | -------------- |
| **Converts from** | **boolean** | **Boolean** | **byte** | **Byte** | **short** | **Short** | **char** | **Character** | **int** | **Integer** | **long** | **Long** | **BigInteger** | **float** | **Float** | **double** | **Double** | **BigDecimal** |
| **boolean**       | -           | B           | N        | N        | N         | N         | N        | N             | N       | N           | N        | N        | N              | N         | N         | N          | N          | N              |
| **Boolean**       | B           | -           | N        | N        | N         | N         | N        | N             | N       | N           | N        | N        | N              | N         | N         | N          | N          | N              |
| **byte**          | T           | T           | -        | B        | Y         | Y         | Y        | D             | Y       | Y           | Y        | Y        | Y              | Y         | Y         | Y          | Y          | Y              |
| **Byte**          | T           | T           | B        | -        | Y         | Y         | Y        | D             | Y       | Y           | Y        | Y        | Y              | Y         | Y         | Y          | Y          | Y              |
| **short**         | T           | T           | D        | D        | -         | B         | Y        | D             | Y       | Y           | Y        | Y        | Y              | Y         | Y         | Y          | Y          | Y              |
| **Short**         | T           | T           | D        | T        | B         | -         | Y        | D             | Y       | Y           | Y        | Y        | Y              | Y         | Y         | Y          | Y          | Y              |
| **char**          | T           | T           | Y        | D        | Y         | D         | -        | D             | Y       | D           | Y        | D        | D              | Y         | D         | Y          | D          | D              |
| **Character**     | T           | T           | D        | D        | D         | D         | D        | -             | D       | D           | D        | D        | D              | D         | D         | D          | D          | D              |
| **int**           | T           | T           | D        | D        | D         | D         | Y        | D             | -       | B           | Y        | Y        | Y              | Y         | Y         | Y          | Y          | Y              |
| **Integer**       | T           | T           | D        | D        | D         | D         | Y        | D             | B       | -           | Y        | Y        | Y              | Y         | Y         | Y          | Y          | Y              |
| **long**          | T           | T           | D        | D        | D         | D         | Y        | D             | D       | D           | -        | B        | Y              | T         | T         | T          | T          | Y              |
| **Long**          | T           | T           | D        | D        | D         | T         | Y        | D             | D       | T           | B        | -        | Y              | T         | T         | T          | T          | Y              |
| **BigInteger**    | T           | T           | D        | D        | D         | D         | D        | D             | D       | D           | D        | D        | -              | D         | D         | D          | D          | T              |
| **float**         | T           | T           | D        | D        | D         | D         | T        | D             | D       | D           | D        | D        | D              | -         | B         | Y          | Y          | Y              |
| **Float**         | T           | T           | D        | T        | D         | T         | T        | D             | D       | T           | D        | T        | D              | B         | -         | Y          | Y          | Y              |
| **double**        | T           | T           | D        | D        | D         | D         | T        | D             | D       | D           | D        | D        | D              | D         | D         | -          | B          | Y              |
| **Double**        | T           | T           | D        | T        | D         | T         | T        | D             | D       | T           | D        | T        | D              | D         | T         | B          | -          | Y              |
| **BigDecimal**    | T           | T           | D        | D        | D         | D         | D        | D             | D       | D           | D        | D        | D              | T         | D         | T          | D          | -              |

'Y' 表示 Groovy 可以进行的转换，'D' 表示动态编译或显式转换时 Groovy 可以进行的转换，'T' 表示 Groovy 可以进行的转换但数据被截断，'B' 表示装箱/拆箱操作

, 'N' 表示 Groovy 无法进行的转换

* 在转换为布尔值/布尔值时，截断使用 [Groovy Truth](https://docs.groovy-lang.org/latest/html/documentation/core-semantics.html#Groovy-Truth)

* 从数字转换为字符将 Number.intvalue() 转换为 char
* 当从 Float 或 Double 转换时，Groovy 使用 Number.doubleValue() 构造 BigInteger 和 BigDecimal，否则它使用 toString() 构造，其他转换的行为由 java.lang.Number 定义。





## Extra keywords

- `as`
- `def`
- `in`
- `trait`
- `it` // within closures

Groovy 不如 Java 严格，因为它允许某些关键字出现在 Java 中不合法的地方，例如

以下是有效的：var var = [def: 1, as: 2, in: 3, trait: 4]。

尽管如此，即使编译器可能满意，也不鼓励您在可能引起混淆的地方使用上述关键字。











