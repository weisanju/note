# 代理

## 优势

使用代理有 2 个优势：

- 可以隐藏委托类的实现
- 可以实现客户与委托类之间的解耦, 在不修改委托类代码的情况下能够做一些额外的处理

## 场景

* 在 Java 中我们有很多场景需要使用代理类, 比如远程 RPC 调用的时候我们就是通过代理类去实现的, 还有 Spring 的 AOP 切面中我们也是为切面生成了一个代理类等等。 
* 代理类主要分为静态代理、JDK 动态代理和 CGLIB 动态代理，它们各有优缺点，没有最好的, 存在就是有意义的，在不同的场景下它们会有不同的用武之地。







# Java 静态代理

* 首先, 定义接口和接口的实现类, 然后定义接口的代理对象, 将接口的实例注入到代理对象中, 然后通过代理对象去调用真正的实现类，实现过程非常简单也比较
* 静态代理的代理关系在编译期间就已经确定了的。它适合于代理类较少且确定的情况。它可实现在怒修改委托类代码的情况下做一些额外的处理，比如包装礼盒，实现客户类与委托类的解耦。缺点是只适用委托方法少的情况下, 试想一下如果委托类有几百上千个方法, 岂不是很难受, 要在代理类中写一堆的代理方法。这个需求动态代理可以搞定



```java
// 委托接口
public interface IHelloService {

    /**
     * 定义接口方法
     * @param userName
     * @return
     */
    String sayHello(String userName);

}
// 委托类实现
public class HelloService implements IHelloService {

    @Override
    public String sayHello(String userName) {
        System.out.println("helloService" + userName);
        return "HelloService" + userName;
    }
}

// 代理类
public class StaticProxyHello implements IHelloService {

    private IHelloService helloService = new HelloService();

    @Override
    public String sayHello(String userName) {
        /** 代理对象可以在此处包装一下*/
        System.out.println("代理对象包装礼盒...");
        return helloService.sayHello(userName);
    }
}
// 测试静态代理类
public class MainStatic {
    public static void main(String[] args) {
        StaticProxyHello staticProxyHello = new StaticProxyHello();
        staticProxyHello.sayHello("isole");
    }
}
```

# 动态代理

代理类在程序运行时创建的代理方式被成为 `动态代理`。在了解动态代理之前, 我们先简回顾一下 JVM 的类加载机制中的加载阶段要做的三件事情 ( 附 Java 中的类加载器 )

1. 通过一个类的全名或其它途径来获取这个类的二进制字节流
2. 将这个字节流所代表的静态存储结构转化为方法区的运行时数据结构
3. 在内存中生成一个代表这个类的 Class 对象, 作为方法区中对这个类访问的入口



而我们要说的动态代理，主要就发生在第一个阶段, 这个阶段类的二进制字节流的来源可以有很多, 比如 zip 包、网络、`运行时计算生成`、其它文件生成 (JSP)、数据库获取。其中运行时计算生成就是我们所说的动态代理技术，在 Proxy 类中, 就是运用了 ProxyGenerator.generateProxyClass 来为特定接口生成形式为 `*$Proxy` 的代理类的二进制字节流。所谓的动态代理就是想办法根据接口或者目标对象计算出`代理类`的字节码然后加载进 JVM 中。实际计算的情况会很复杂，我们借助一些诸如 JDK 动态代理实现、CGLIB 第三方库来完成的

另一方面为了让生成的代理类与目标对象 (就是委托类) 保持一致, 我们有 2 种做法：通过接口的 JDK 动态代理 和通过继承类的 CGLIB 动态代理。

## JDK动态代理

```java
public interface InvocationHandler {
    /**
     * 调用处理
     * @param proxy 代理类对象
     * @param methon 标识具体调用的是代理类的哪个方法
     * @param args 代理类方法的参数
     */
    public Object invoke(Object proxy, Method method, Object[] args)
        throws Throwable;
}
```



```java
// 委托类接口
public interface IHelloService {

    /**
     * 方法1
     * @param userName
     * @return
     */
    String sayHello(String userName);

    /**
     * 方法2
     * @param userName
     * @return
     */
    String sayByeBye(String userName);

}
// 委托类
public class HelloService implements IHelloService {

    @Override
    public String sayHello(String userName) {
        System.out.println(userName + " hello");
        return userName + " hello";
    }

    @Override
    public String sayByeBye(String userName) {
        System.out.println(userName + " ByeBye");
        return userName + " ByeBye";
    }
}
// 中间类
public class JavaProxyInvocationHandler implements InvocationHandler {

    /**
     * 中间类持有委托类对象的引用,这里会构成一种静态代理关系
     */
    private Object obj ;

    /**
     * 有参构造器,传入委托类的对象
     * @param obj 委托类的对象
     */
    public JavaProxyInvocationHandler(Object obj){
        this.obj = obj;

    }

    /**
     * 动态生成代理类对象,Proxy.newProxyInstance
     * @return 返回代理类的实例
     */
    public Object newProxyInstance() {
        return Proxy.newProxyInstance(
                //指定代理对象的类加载器
                obj.getClass().getClassLoader(),
                //代理对象需要实现的接口，可以同时指定多个接口
                obj.getClass().getInterfaces(),
                //方法调用的实际处理者，代理对象的方法调用都会转发到这里
                this);
    }


    /**
     *
     * @param proxy 代理对象
     * @param method 代理方法
     * @param args 方法的参数
     * @return
     * @throws Throwable
     */
    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        System.out.println("invoke before");
        Object result = method.invoke(obj, args);
        System.out.println("invoke after");
        return result;
    }
}
// 测试动态代理类
public class MainJavaProxy {
    public static void main(String[] args) {
        JavaProxyInvocationHandler proxyInvocationHandler = new JavaProxyInvocationHandler(new HelloService());
        IHelloService helloService = (IHelloService) proxyInvocationHandler.newProxyInstance();
        helloService.sayByeBye("paopao");
        helloService.sayHello("yupao");
    }

}
```



## CGLIB动态代理

```xml
<dependency>
    <groupId>cglib</groupId>
    <artifactId>cglib</artifactId>
    <version>3.3.0s</version>
</dependency>
```

CGLIB 创建动态代理类的模式是:

1. 查找目标类上的所有非 final 的 public 类型的方法 (final 的不能被重写)
2. 将这些方法的定义转成字节码
3. 将组成的字节码转换成相应的代理的 Class 对象然后通过反射获得代理类的实例对象
4. 实现 MethodInterceptor 接口, 用来处理对代理类上所有方法的请求



