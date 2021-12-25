# 前言

## **Cglib是什么**

Cglib是一个强大的、高性能的**代码生成包**，它广泛被许多AOP框架使用，为他们**提供方法的拦截**。下图是我网上找到的一张Cglib与一些框架和语言的关系：

![cglib-structure](../images/cglib-structure.gif)

- 最底层的是字节码Bytecode，字节码是Java为了保证“一次编译、到处运行”而产生的一种虚拟指令格式，例如iload_0、iconst_1、if_icmpne、dup等
- 位于字节码之上的是ASM，这是一种直接操作字节码的框架，应用ASM需要对Java字节码、Class结构比较熟悉
- 位于ASM之上的是CGLIB、Groovy、BeanShell，后两种并不是Java体系中的内容而是脚本语言，它们通过ASM框架生成字节码变相执行Java代码，这说明**在JVM中执行程序并不一定非要写Java代码----只要你能生成Java字节码，JVM并不关心字节码的来源**，当然通过Java代码生成的JVM字节码是通过编译器直接生成的，算是最“正统”的JVM字节码
- 位于CGLIB、Groovy、BeanShell之上的就是Hibernate、Spring AOP这些框架了，这一层大家都比较熟悉
- 最上层的是Applications，即具体应用，一般都是一个Web项目或者本地跑一个程序

# CGLIB类库结构

## **使用Cglib代码对类做代理**

下面演示一下Cglib代码示例----对类做代理。首先定义一个Dao类，里面有一个select()方法和一个update()方法：

**源方法**

```java
public class Dao {
  
    public void update() {
        System.out.println("PeopleDao.update()");
    }
  
    public void select() {
        System.out.println("PeopleDao.select()");
    }
}
```

**拦截代理类**

```java
public class DaoProxy implements MethodInterceptor {

    @Override
    public Object intercept(Object object, Method method, Object[] objects, MethodProxy proxy) throws Throwable {
        System.out.println("Before Method Invoke");
        proxy.invokeSuper(object, objects);
        System.out.println("After Method Invoke");
      
        return object;
    }
  
}
```

intercept方法的参数的含义为：

- Object表示要进行增强的对象
- Method表示拦截的方法
- Object[]数组表示参数列表，基本数据类型需要传入其包装类型
- MethodProxy 表示对方法的代理，invokeSuper方法表示对被代理对象方法的调用

**返回值**

*any value compatible with the signature of the proxied method. Method returning void will ignore this value*

**增强类**

```java
public class CglibTest {

    @Test
    public void testCglib() {
        DaoProxy daoProxy = new DaoProxy();
      
        Enhancer enhancer = new Enhancer();
        //设置要代理的类
        enhancer.setSuperclass(Dao.class);
        //表示设置回调即MethodInterceptor的实现类
        enhancer.setCallback(daoProxy);
        //使用create()方法生成一个代理对象
        Dao dao = (Dao)enhancer.create();
        dao.update();
        dao.select();
    }
  
}
```
