# SPI是什么

SPI全称Service Provider Interface，是Java提供的一套用来被第三方实现或者扩展的API，它可以用来启用框架扩展和替换组件。

![](/images/spi.png)



# 使用场景

* 数据库驱动加载接口实现类的加载  JDBC加载不同类型数据库的驱动
* 日志门面接口实现类加载  SLF4J加载不同提供商的日志实现类
* Spring ，Spring中大量使用了SPI,比如：对servlet3.0规范对ServletContainerInitializer的实现、自动类型转换Type Conversion SPI(Converter SPI、Formatter SPI)等



# 使用规范

实现SPI，就需要按照SPI本身定义 的规范来进行配置，SPI规范如下：

* 需要在classpath下创建一个目录，该目录命名必须是：META-INF/services
* 在该目录下创建一个文件，该文件需要满足以下几个条件
    * 文件名必须是扩展的接口的全路径名称
    * 文件内部描述的是该扩展接口的所有实现类
    * 文件的编码格式是UTF-8
* SPI的实现类**必须携带一个不带参数的构造方法**



# 示例

![](/images/spi_sqldriver.png)





# 总结

## SPI是如何进行类加载的

* 通过规定 类的定义与 类的注册 方式来动态加载类

```java
public static <S> ServiceLoader<S> load(Class<S> service) {
    ClassLoader cl = Thread.currentThread().getContextClassLoader();
    return ServiceLoader.load(service, cl);
}
```

## SPI为什么会破坏双亲委派机制

因为 Java核心库定义了一系列 核心SPI接口，这些接口类是由 系统类加载器加载的，而系统类加载器 无法加载 实现类，所以需要使用 线程上下文的类加载器

## SPI的优缺点

**优点**

相比使用提供接口jar包，供第三方服务模块实现接口的方式，SPI的方式使得源框架，不必关心接口的实现类的路径，可以不用通过下面的方式获取接口实现类：

- 代码硬编码import 导入实现类
- 指定类全路径反射获取：例如在JDBC4.0之前，JDBC中获取数据库驱动类需要通过**Class.forName("com.mysql.jdbc.Driver")**，类似语句先动态加载数据库相关的驱动，然后再进行获取连接等的操作
- 第三方服务模块把接口实现类实例注册到指定地方，源框架从该处访问实例

**缺点**：

- 虽然ServiceLoader也算是使用的延迟加载，但是基本只能通过遍历全部获取，也就是接口的实现类全部加载并实例化一遍。如果你并不想用某些实现类，它也被加载并实例化了，这就造成了浪费。获取某个实现类的方式不够灵活，只能通过Iterator形式获取，不能根据某个参数来获取对应的实现类。
- 并发多线程使用ServiceLoader类的实例是不安全的。
