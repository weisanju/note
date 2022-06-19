## Runtime and compile-time metaprogramming

> 运行时和编译时的元编程

 Groovy支持两种编程模型：运行时、编译时



### Runtime metaprogramming

运行时、元编程可以将 拦截、注入、甚至类与接口得合成推迟到运行时

为了深入理解 Groovy 的元对象协议 (MOP)，我们需要了解 Groovy 对象和 Groovy 的方法处理。

在 Groovy 中，我们使用三种对象

* POJO
* POGO
* Groovy Interceptors

Groovy 允许对所有类型的对象进行元编程，但方式不同。

- POJO - 常规 Java 对象，其类可以用 Java 或任何其他 JVM 语言编写
- POGO - 一个 Groovy 对象，其类是用 Groovy 编写的。继承了 *Object* 实现了 *groovy.lang.GroovyObject*
- Groovy Interceptor - 一个 Groovy 对象 实现了 *groovy.lang.GroovyInterceptable* 



对于每个方法调用 Groovy 检查对象是 POJO 还是 POGO

* 对于POJO，Groovy  从  *groovy.lang.MetaClassRegistry* 获取 *MetaClass* ，将方法调用委托给它。
* 对于POGO，Groovy 有更多的步骤

![](../images/GroovyInterceptions.png)

### GroovyObject interface

