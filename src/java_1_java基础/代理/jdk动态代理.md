# 什么是JAVA动态代理？

JAVA动态代理与静态代理相对，静态代理是在编译期就已经确定代理类和真实类的关系，并且生成代理类的。而动态代理是在运行期利用JVM的反射机制生成代理类

这里是直接生成类的字节码，然后通过类加载器载入JAVA虚拟机执行

现在主流的JAVA动态代理技术的实现有两种：

一种是JDK自带的，就是我们所说的JDK动态代理，

另一种是开源社区的一个开源项目CGLIB





# 什么是JDK动态代理？

JDK动态代理的实现是在运行时，根据一组接口定义，使用Proxy、InvocationHandler等工具类去生成一个代理类和代理类实例。



![](../../images/jdkproxy.webp)

1. 类名的生成规则是前缀"$Proxy"加上一个序列数
2. 这个类继承Proxy，实现一系列的接口Intf1,Intf2...IntfN
3. 既然要实现接口，那么就要实现接口的各个方法,JDK动态代理类是如何实现这些接口方法的具体逻辑,答案就在InvocationHandler上
4. $Proxy0对外只提供一个构造函数，这个构造函数接受一个InvocationHandler实例h，这个构造函数的逻辑非常简单，就是调用父类的构造函数
5. 将参数h赋值给对象字段h。最终就是把所有的方法实现都分派到InvocationHandler实例h的invoke方法上。
6. 所以JDK动态代理的接口方法实现逻辑是完全由InvocationHandler实例的invoke方法决定的。









# 保存JDK动态代理字节码的两种方式

## 设置系统属性

```java
public Object getProxy() {
	System.getProperties().put("sun.misc.ProxyGenerator.saveGeneratedFiles", "true"); //设置系统属性
	return Proxy.newProxyInstance(target.getClass().getClassLoader(),
			target.getClass().getInterfaces(), this); 
}
```

会 自动将 proxy 写入 `${workplace}/com/sun/proxy` 路径下

## 保存ProxyGenerator生成的字节流数组

```java
byte[] bytes = ProxyGenerator.generateProxyClass("MyClass.class", Dog.class.getInterfaces());
Path path = Paths.get(Dog.class.getResource("").toURI());

FileChannel open = FileChannel.open(path.resolve("MyClass.class"), StandardOpenOption.WRITE,StandardOpenOption.CREATE);
        open.write(ByteBuffer.wrap(bytes));

```

# 字节码分析

1. m0是 *hashCode*、m1是 *equals*、m2是 *toString* 方法
2. 其余 m* 是接口的方法
3. 所有接口方法都交由 InvocationHandler处理

```java
//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by FernFlower decompiler)
//

package com.sun.proxy;

import com.weisanju.ioStudy.proxy.Animal;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.lang.reflect.UndeclaredThrowableException;

public final class $Proxy0 extends Proxy implements Animal {
    private static Method m1;
    private static Method m2;
    private static Method m4;
    private static Method m3;
    private static Method m0;

    public $Proxy0(InvocationHandler var1) throws  {
        super(var1);
    }

    public final boolean equals(Object var1) throws  {
        try {
            return (Boolean)super.h.invoke(this, m1, new Object[]{var1});
        } catch (RuntimeException | Error var3) {
            throw var3;
        } catch (Throwable var4) {
            throw new UndeclaredThrowableException(var4);
        }
    }

    public final String toString() throws  {
        try {
            return (String)super.h.invoke(this, m2, (Object[])null);
        } catch (RuntimeException | Error var2) {
            throw var2;
        } catch (Throwable var3) {
            throw new UndeclaredThrowableException(var3);
        }
    }

    public final void eat(String var1) throws  {
        try {
            super.h.invoke(this, m4, new Object[]{var1});
        } catch (RuntimeException | Error var3) {
            throw var3;
        } catch (Throwable var4) {
            throw new UndeclaredThrowableException(var4);
        }
    }

    public final void say() throws  {
        try {
            super.h.invoke(this, m3, (Object[])null);
        } catch (RuntimeException | Error var2) {
            throw var2;
        } catch (Throwable var3) {
            throw new UndeclaredThrowableException(var3);
        }
    }

    public final int hashCode() throws  {
        try {
            return (Integer)super.h.invoke(this, m0, (Object[])null);
        } catch (RuntimeException | Error var2) {
            throw var2;
        } catch (Throwable var3) {
            throw new UndeclaredThrowableException(var3);
        }
    }

    static {
        try {
            m1 = Class.forName("java.lang.Object").getMethod("equals", Class.forName("java.lang.Object"));
            m2 = Class.forName("java.lang.Object").getMethod("toString");
            m4 = Class.forName("com.weisanju.ioStudy.proxy.Animal").getMethod("eat", Class.forName("java.lang.String"));
            m3 = Class.forName("com.weisanju.ioStudy.proxy.Animal").getMethod("say");
            m0 = Class.forName("java.lang.Object").getMethod("hashCode");
        } catch (NoSuchMethodException var2) {
            throw new NoSuchMethodError(var2.getMessage());
        } catch (ClassNotFoundException var3) {
            throw new NoClassDefFoundError(var3.getMessage());
        }
    }
}
```

# 源码解析

## newProxyInstance

> 该代码段主要是 获取代理类，并根据代理类 使用 InvocationHandler  实例化

1. 根据 接口获取 代理类 （如果有缓存则使用缓存，如果没有缓存则 生成）
2. 获取代理类的 构造函数：*invocationHandler* 的那个
3. 使用 InvocationHandler 根据构造函数实例化 对象

```java
    public static Object newProxyInstance(ClassLoader loader,
                                          Class<?>[] interfaces,
                                          InvocationHandler h)
        throws IllegalArgumentException
    {
        //检验h不为空，h为空抛异常
        Objects.requireNonNull(h);
        //接口的类对象拷贝一份
        final Class<?>[] intfs = interfaces.clone();
        //进行一些安全性检查
        final SecurityManager sm = System.getSecurityManager();
        if (sm != null) {
            checkProxyAccess(Reflection.getCallerClass(), loader, intfs);
        }

        /*
         * Look up or generate the designated proxy class.
         *  查询（在缓存中已经有）或生成指定的代理类的class对象。
         * 根据接口生成生成指定代理类
         */
        Class<?> cl = getProxyClass0(loader, intfs);

        /*
         * Invoke its constructor with the designated invocation handler.
         */
        try {
            //权限检查
            if (sm != null) {
                checkNewProxyPermission(Reflection.getCallerClass(), cl);
            }
            //得到代理类对象的构造函数，这个构造函数的参数由constructorParams指定
            //参数constructorParames为常量值：private static final Class<?>[] constructorParams = { InvocationHandler.class };
            //获取构造函数
            final Constructor<?> cons = cl.getConstructor(constructorParams);
            final InvocationHandler ih = h;
            if (!Modifier.isPublic(cl.getModifiers())) {
                AccessController.doPrivileged(new PrivilegedAction<Void>() {
                    public Void run() {
                        cons.setAccessible(true);
                        return null;
                    }
                });
            }
            //实例化
            //这里生成代理对象，传入的参数new Object[]{h}后面讲
            return cons.newInstance(new Object[]{h});
        } catch (IllegalAccessException|InstantiationException e) {
            throw new InternalError(e.toString(), e);
        } catch (InvocationTargetException e) {
            Throwable t = e.getCause();
            if (t instanceof RuntimeException) {
                throw (RuntimeException) t;
            } else {
                throw new InternalError(t.toString(), t);
            }
        } catch (NoSuchMethodException e) {
            throw new InternalError(e.toString(), e);
        }
    }
```

## getProxyClass0

这里使用的 *WeakCache* 二级弱缓存

1. key是 classLoader：使用的是 弱引用队列
2. 二级key是 代理的接口们 强引用
3. value 弱引用



**value计算逻辑**

1. 负责校验接口 是否存在 可加载、是否是接口、是否重复
2. 维护 proxy的类名
3. 使用接口 调用 生成字节码的类

```java
public Class<?> apply(ClassLoader loader, Class<?>[] interfaces) {

    Map<Class<?>, Boolean> interfaceSet = new IdentityHashMap<>(interfaces.length);
    for (Class<?> intf : interfaces) {
        /*
         * Verify that the class loader resolves the name of this
         * interface to the same Class object.
         */
        Class<?> interfaceClass = null;
        try {
            interfaceClass = Class.forName(intf.getName(), false, loader);
        } catch (ClassNotFoundException e) {
        }
        if (interfaceClass != intf) {
            throw new IllegalArgumentException(
                intf + " is not visible from class loader");
        }
        /*
         * Verify that the Class object actually represents an
         * interface.
         */
        if (!interfaceClass.isInterface()) {
            throw new IllegalArgumentException(
                interfaceClass.getName() + " is not an interface");
        }
        /*
         * Verify that this interface is not a duplicate.
         */
        if (interfaceSet.put(interfaceClass, Boolean.TRUE) != null) {
            throw new IllegalArgumentException(
                "repeated interface: " + interfaceClass.getName());
        }
    }

    String proxyPkg = null;     // package to define proxy class in
    int accessFlags = Modifier.PUBLIC | Modifier.FINAL;

    /*
     * Record the package of a non-public proxy interface so that the
     * proxy class will be defined in the same package.  Verify that
     * all non-public proxy interfaces are in the same package.
     */
    for (Class<?> intf : interfaces) {
        int flags = intf.getModifiers();
        if (!Modifier.isPublic(flags)) {
            accessFlags = Modifier.FINAL;
            String name = intf.getName();
            int n = name.lastIndexOf('.');
            String pkg = ((n == -1) ? "" : name.substring(0, n + 1));
            if (proxyPkg == null) {
                proxyPkg = pkg;
            } else if (!pkg.equals(proxyPkg)) {
                throw new IllegalArgumentException(
                    "non-public interfaces from different packages");
            }
        }
    }

    if (proxyPkg == null) {
        // if no non-public proxy interfaces, use com.sun.proxy package
        proxyPkg = ReflectUtil.PROXY_PACKAGE + ".";
    }

    /*
     * Choose a name for the proxy class to generate.
     */
    long num = nextUniqueNumber.getAndIncrement();
    String proxyName = proxyPkg + proxyClassNamePrefix + num;

    /*
     * Generate the specified proxy class.
     */
    byte[] proxyClassFile = ProxyGenerator.generateProxyClass(
        proxyName, interfaces, accessFlags);
    try {
        return defineClass0(loader, proxyName,
                            proxyClassFile, 0, proxyClassFile.length);
    } catch (ClassFormatError e) {
        /*
         * A ClassFormatError here means that (barring bugs in the
         * proxy class generation code) there was some other
         * invalid aspect of the arguments supplied to the proxy
         * class creation (such as virtual machine limitations
         * exceeded).
         */
        throw new IllegalArgumentException(e.toString());
    }
}
```

