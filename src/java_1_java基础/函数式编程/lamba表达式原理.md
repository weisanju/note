## 一、创建测试样例

```java
public class App {
    public static void main(String[] args) {
        Lam lam = (msg) -> "log:" + msg;
        String result = lam.print("Test");
        System.out.println(result);
    }
}
interface Lam {
    String print(String msg);
}
```



## 利用Java命令编译分析

```java
javap -p Lam.class
// 从返回值我们可以看到，因为Lam.class是App.class的内部类，所以提示我们它是App.java编译过来的，并且其内部只有一个abstract方法print()。
  Compiled from "App.java"
interface Lam {
  public abstract java.lang.String print(java.lang.String);
}


javap -p App.class
//在APP中生成了一个静态方法：lambda$main$0
Compiled from "App.java"
public class App {
  public App();
  public static void main(java.lang.String[]);
  private static java.lang.String lambda$main$0(java.lang.String);
}


```

## 静态方法如何实现的

```java
//查看详细实现过程，这里只关注 lambda$main$0 方法
➜  classes javap -c -p  App.class    
Compiled from "App.java"
public class App {
  public App();
    Code:
       0: aload_0
       1: invokespecial #1                  // Method java/lang/Object."<init>":()V
       4: return

  public static void main(java.lang.String[]);
    Code:
       0: invokedynamic #2,  0              // InvokeDynamic #0:print:()LLam;
       5: astore_1
       6: aload_1
       7: ldc           #3                  // String Test
       9: invokeinterface #4,  2            // InterfaceMethod Lam.print:(Ljava/lang/String;)Ljava/lang/String;
      14: astore_2
      15: getstatic     #5                  // Field java/lang/System.out:Ljava/io/PrintStream;
      18: aload_2
      19: invokevirtual #6                  // Method java/io/PrintStream.println:(Ljava/lang/String;)V
      22: return

  private static java.lang.String lambda$main$0(java.lang.String);
    Code:
       0: new           #7                  // class java/lang/StringBuilder
       3: dup
       4: invokespecial #8                  // Method java/lang/StringBuilder."<init>":()V
       7: ldc           #9                  // String log:
       9: invokevirtual #10                 // Method java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder;
      12: aload_0
      13: invokevirtual #10                 // Method java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder;
      16: invokevirtual #11                 // Method java/lang/StringBuilder.toString:()Ljava/lang/String;
      19: areturn
}
```





## 如何调用生成的静态方法

这个*lambda$main$0(String)*方法是怎么被调用的呢？我们通过对java命令指定选项查看底层详细的编译过程：

**调用 AppMain方法、并导出中途生成的代理类**

```shell
java -Djdk.internal.lambda.dumpProxyClasses App
```

我们能够看到多出来一个*App$$Lambda$1.class*文件，打开文件：

```
javap -p App$$Lambda$1.class
```

我们再通过javap -c查看一下它内部详细信息：

```java
javap -c App$$Lambda$1.class
```

```java
final class App$$Lambda$1 implements Lam {
  public java.lang.String print(java.lang.String);
    Code:
       0: aload_1
       1: invokestatic  #18                 // Method App.lambda$main$0:(Ljava/lang/String;)Ljava/lang/String;
       4: areturn
}
```

由此我们可以看出，App$$Lambda$1.class的print()方法执行了App.lambda$main$0()，因此，我们就可以得出结论：

1. Java在编译时，首先，在App内将Lambda表达式抽取出来作为一个static方法*lambda$main$0(String)*；
2. 然后，对Lam.class做了默认实现*App$$Lambda$1.class*，并在内部*print*()方法中调用了App内的static方法：*lambda$main$0()*；
3. 接下来，执行App的*main*()方法时，就会对lambda表达式利用实现类的*print*()方法运行；
4. 最后，将结果返回，并打印。

