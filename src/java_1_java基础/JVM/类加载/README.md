# 总结

## 如果要自定义类加载器，可以放在*classpath* 中吗

不能，默认情况下，Application ClassLoader 会加载 classpath下的  所有类，如果 需要自定义类加载器加载类 则需要将 该class文件放置到**其他路径下**



## Class与ClassLoader的getResourceAsStream区别

不仅ClassLoader中有getResourceAsStream(String name)方法，Class下也有getResourceAsStream(String name)方法，它们两个方法的区别在于：

1、Class的getResourceAsStream(String name)方法，参数不以"/"开头则默认从此类对应的.class文件所在的packge下取资源，以"/"开头则从CLASSPATH下获取

2、ClassLoader的getResourceAsStream(String name)方法，默认就是从CLASSPATH下获取资源，参数不可以以"/"开头

## **为什么要自定义类加载器**

主流的 Java Web 服务器，比如 Tomcat,都实现了自定义的类加载器（一般都不止一个）。因为一个功能健全的 Web 服务器，要解决如下几个问题：

- 部署在同一个服务器上的两个 Web 应用程序所使用的 Java 类库可以实现相互隔离(不同应用使用不同版本的 同名 Java 类)
- 部署在同一个服务器上的两个 Web 应用程序所使用的 Java 类库可以相互共享
- 支持热部署



## **如何判定类是否相同**

- 类的全名是否相同
- 还要看加载此类的类加载器是否一样(被不同的类加载器加载之后所得到的类，也是不同的)

```
两个不同的类加载器 ClassLoaderA和 ClassLoaderB分别读取了这个 Sample.class文件，并定义出两个 java.lang.Class类的实例来表示这个类。这两个实例是不相同的。对于 Java 虚拟机来说，它们是不同的类。试图对这两个类的对象进行相互赋值，会抛出运行时异常 ClassCastException
```



## tomcat等web容器的类加载有何不同

* 优先加载 web应用提供的类，而找不到时，才使用 容器提供的类

* 但时java核心类，交给 启动类加载器，或者扩展类加载器启动

这样既保证 应用之间的 不通版本的类可以隔离，又保证相同版本的类可以共享



## 为什么要使用 *Class.forName*加载 sql驱动类

* 高版本的不需要手动加载了

[参考链接](https://www.toutiao.com/i6674120935265534467/)



