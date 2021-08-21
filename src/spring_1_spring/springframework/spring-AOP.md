# AOP 术语

在我们开始使用 AOP 工作之前，让我们熟悉一下 AOP 概念和术语。这些术语并不特定于 Spring，而是与 AOP 有关的。

## AOP概念

| 项            | 描述                                                         |
| ------------- | ------------------------------------------------------------ |
| Aspect        | 以业务逻辑  划分各个切点的 逻辑实体类,例如事务控制是一个切面, 日志打印也是一个切面 |
| Join point    | 被拦截的方法                                                 |
| Advice        | 告诉AOP 在什么时候调用 切入的方法                            |
| Pointcut      | 被拦截的方法 以一个表达式定义                                |
| Introduction  | 引用允许你添加新方法或属性到现有的类中。                     |
| Target object | 被拦截方法的对象                                             |
| Weaving       | Weaving 把方面连接到其它的应用程序类型或者对象上，并创建一个被通知的对象。这些可以在编译时，类加载时和运行时完成。 |

## 通知类型

| 通知           | 描述                                                         |
| -------------- | ------------------------------------------------------------ |
| 前置通知       | 在一个方法执行之前，执行通知。                               |
| 后置通知       | 在一个方法执行之后，不考虑其结果，执行通知。                 |
| 返回后通知     | 在一个方法执行之后，只有在方法成功完成时，才能执行通知。     |
| 抛出异常后通知 | 在一个方法执行之后，只有在方法退出抛出异常时，才能执行通知。 |
| 环绕通知       | 在建议方法调用之前和之后，执行通知。                         |



# spring AOP的目标

springAOP的目的不是与AspectJ竞争，提供最全面的AOP实现,而是提供与SpringIOC的无缝集成

# AOP代理方式

* 默认使用 JDK动态代理,也提供了 基于 CGLIB的 动态代理
* JDK动态代理 是基于接口的 代理
* CGLIB 基于 继承的代理



# 基于注解的AOP的步骤

## 启用AOP代理功能

```java
@Configuration
@EnableAspectJAutoProxy
public class AppConfig {

}
```

```xml
<aop:aspectj-autoproxy/>
```

## 申明一个切面

```java
package org.xyz;
import org.aspectj.lang.annotation.Aspect;

@Aspect
public class NotVeryUsefulAspect {

}
```

```xml
<bean id="myAspect" class="org.xyz.NotVeryUsefulAspect">
    <!-- configure properties of the aspect here -->
</bean>
```

**注意事项**

* 可以在xml中申明 切面类,也可以通过 @Aspect与 @Component 申明切面类
* 切面类本身不能称为代理的目标对象

## 申明切点

```java
@Pointcut("execution(* transfer(..))") // the pointcut expression
private void anyOldTransfer() {} // the pointcut signature
```

切入点表达式详见: [AspectJ Programming Guide](https://www.eclipse.org/aspectj/doc/released/progguide/index.html) 



# aspectJ语法

## 语法定义

```
MethodPattern = 
  [ModifiersPattern] TypePattern 
        [TypePattern . ] IdPattern (TypePattern | ".." , ... ) 
        [ throws ThrowsPattern ]
ConstructorPattern = 
  [ModifiersPattern ] 
        [TypePattern . ] new (TypePattern | ".." , ...) 
        [ throws ThrowsPattern ]
FieldPattern = 
  [ModifiersPattern] TypePattern [TypePattern . ] IdPattern
ThrowsPattern = 
  [ ! ] TypePattern , ...
TypePattern = 
    IdPattern [ + ] [ [] ... ]
    | ! TypePattern
    | TypePattern && TypePattern
    | TypePattern || TypePattern
    | ( TypePattern )  
IdPattern =
  Sequence of characters, possibly with special * and .. wildcards
ModifiersPattern =
  [ ! ] JavaModifier  ...
```

| 指示符                            | 说明                                                         |
| --------------------------------- | ------------------------------------------------------------ |
| execution(*`MethodPattern`*)      | 匹配 执行方法 的签名                                         |
| execution(*`ConstructorPattern`*) | 匹配构造函数                                                 |
| within(*`TypePattern`*)           | Picks out each join point <br />where the executing code is defined in a type matched by *`TypePattern`*. |
| this(*`Type`* or *`Id`*)          | 生成的代理对象 是 Type 或者 id 的一个实例,无法从静态实例对象中匹配 |
| target(*`Type`* or *`Id`*)        | 被代理的对象                                                 |

## pattern说明

| pattern              | 说明                    |
| -------------------- | ----------------------- |
| *MethodPattern*      | 方法表达式,匹配方法     |
| *ConstructorPattern* | 构造器表达式,匹配构造器 |
| *TypePattern*        | 类型表达式, 匹配某个类  |
| *Type*               | 全限定类名              |

## 指示符定义

### *within(TypePattern)*

匹配包路径,

| 模式                                        | 描述                                                         |
| ------------------------------------------- | ------------------------------------------------------------ |
| `within(com.learn.all..*)`                  | com.learn.all包及子包下的任何方法执行                        |
| `within(com.learn.service..IHelloService+)` | com.learn.service包或所有子包下IHelloService类型及子类型的任何方法 |
| `within(@com.learn..Secure *)`              | 持有com.learn..Secure注解的类的任何方法，注解必须是在目标对象上声明，对在接口上声明的不起作用 |

### *execution(MethodPattern)*

* 拦截一个方法 (spriingAOP中主要用到的)
* 语法

```
execution(modifiers-pattern? ret-type-pattern declaring-type-pattern?name-pattern(param-pattern)throws-pattern?)
//说明
访问修饰符 返回值类型 包路径 方法名( 参数名 ) 抛出异常的返回值
//示例
execution(public Integer com.weisanju.xjq.MethodName(*,String)
```

* 注意点
  * 除了返回值类型, 方法名,参数名 这三者,必选 其他的都可选
  * 星号 表示一个或多个字符
  * param-pattern 有以下几种模式
    * `()`  表示空参
    * `(*)` 表示一个任意类型的参数 的方法
    * `(*,String)` 表示两个参数 ,第一个是任意类型,第二个是 string类型
    * `(..)` 表示任意多个参数
  * this,target的作用
    * 对 前面被PCD选中的类 进一步的筛选
    * 取得该对象作为入参

### *args(typepattern)*

指定参数类型

### *this(type)*

用于返回 当前代理对象的 当前类型或者 父类或接口实现

### *target(type)*

返回 当前被代理的对象

### bean(beanId)

拦截指定bean的名称的方法



### @within

拦截 带有特定注解的类  的方法

### @target

拦截 目标对象持有指定注解的 方法

### @args

拦截参数持有指定注解的 方法

### @annotation

拦截方法持有指定注解的 方法



### @declareParent

为某一个类引入一个新的接口 以及接口的实现

```
    @DeclareParents(value = "com.weisanju.aop.test.TestClass",defaultImpl = DoSomethingImpl.class )
    private DoSomething doSomething;
    
    DoSomething 必须为接口
    DoSomethingImpl 必须为 DoSomething的实现类
    value为 类型匹配符
```

* 在 AOP切面与 引入点 都作用于一个类 
* 则 引入的实现 是在 代理的对象 上, 即 this 而非target

## 类型匹配的通配符

* *：匹配任何数量字符；

*  ..：匹配任何数量字符的重复，如在类型模式中匹配任何数量子包；而在方法参数模式中匹配任何数量参数。

*  +：匹配指定类型的子类型；仅能作为后缀放在类型模式后边。

## 切入点表达式组合

AspectJ使用 且（&&）、或（||）、非（！）来组合切入点表达式。



# 通知参数

## JoinPoint

```java
package org.aspectj.lang;  
import org.aspectj.lang.reflect.SourceLocation;  
public interface JoinPoint {  
    String toString();         //连接点所在位置的相关信息  
    String toShortString();     //连接点所在位置的简短相关信息  
    String toLongString();     //连接点所在位置的全部相关信息  
    Object getThis();         //返回AOP代理对象  
    Object getTarget();       //返回目标对象  
    Object[] getArgs();       //返回被通知方法参数列表  
    Signature getSignature();  //返回当前连接点签名  
    SourceLocation getSourceLocation();//返回连接点方法所在类文件中的位置  
    String getKind();        //连接点类型  
    StaticPart getStaticPart(); //返回连接点静态部分  
}  	
```

## ProceedingJoinPoint

> 用于环绕通知，使用proceed()方法来执行目标方法

```java
public interface ProceedingJoinPoint extends JoinPoint {  
    public Object proceed() throws Throwable;  
    public Object proceed(Object[] args) throws Throwable;  
}
```

## JoinPoint.StaticPart

>  提供访问连接点的静态部分，如被通知方法签名、连接点类型等

```java
public interface StaticPart {  
Signature getSignature();    //返回当前连接点签名  
String getKind();          //连接点类型  
    int getId();               //唯一标识  
String toString();         //连接点所在位置的相关信息  
    String toShortString();     //连接点所在位置的简短相关信息  
String toLongString();     //连接点所在位置的全部相关信息  
}  
```

## 参数传递

* 在Spring AOP中，除了execution和bean指示符不能传递参数给通知方法，其他指示符都可以将匹配的**相应参数或对象自动传递给通知方法。**
* JoinPoint、ProceedingJoinPoint或JoinPoint.StaticPart类型,这些类型对象会自动传入的，但必须作为第一个参数；

```
@Before(args(param) && target(bean) && @annotation(secure)",   
        argNames="jp,param,bean,secure")  
public void before5(JoinPoint jp, String param,
 IPointcutService pointcutService, Secure secure) {  
……  
}  
```

## 参数传递的实例

```java
package com.weisanju.aop.aspect;

import com.weisanju.aop.annotaion.TimeElapse;
import com.weisanju.aop.entity.Person;
import com.weisanju.aop.interfaces.DoSomething;
import com.weisanju.aop.interfaces.impl.DoSomethingImpl;
import com.weisanju.aop.test.TestClass;
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.*;
import org.springframework.stereotype.Component;

import java.text.MessageFormat;
import java.util.stream.Collectors;
import java.util.stream.Stream;

@Aspect
@Component
public class MyAspect {
    @DeclareParents(value = "com.weisanju.aop.test.TestClass",defaultImpl = DoSomethingImpl.class )
    private DoSomething doSomething;
    @Pointcut("execution(* *log(..))")
    public void log(){}

    @Around("log()")
    public void doLog(ProceedingJoinPoint joinPoint){
        MessageFormat messageFormat = new MessageFormat("方法名%s,方法参数%s");
        System.out.print("方法开始执行:");
        Object[] args = joinPoint.getArgs();
        String format = messageFormat.format(joinPoint.getSignature().getName(), Stream.of(args).map(Object::toString).collect(Collectors.joining()));
        System.out.println(format);

        try {
            //joinPoint.proceed();
        } catch (Throwable throwable) {
            throwable.printStackTrace();
        }
        System.out.println("方法执行完毕");
    }

    @Around(" @annotation(unit)  &&  args(person) && this(proxied) && target(target)")
    public String timeCount(ProceedingJoinPoint joinPoint, TimeElapse unit, Person person, DoSomething proxied, TestClass target){
        String result = "这是默认值";
        long start =System.currentTimeMillis();

        try {
            target.runElapse();
            System.out.println("哈哈原来是这样用的");

            proxied.doSomething();
            result= (String) joinPoint.proceed();
        } catch (Throwable throwable) {
            throwable.printStackTrace();
        }
        System.out.println("总耗时:"+(System.currentTimeMillis() - start)/unit.value());
        return result+person.toString();
    }
}

```



# 基于XML的配置



1. 定义一个config
2. 定义一个 切面
3. 定义一个切入点
4. 定义拦截的方法

```xml
<aop:config>
   <aop:aspect id="myAspect" ref="aBean">
      <aop:pointcut id="businessService" expression="execution(* com.xyz.myapp.service.*.*(..))"/>
      <aop:before pointcut-ref="businessService" 
         method="doRequiredTask"/>
      <!-- an after advice definition -->
      <aop:after pointcut-ref="businessService" 
         method="doRequiredTask"/>
      <!-- an after-returning advice definition -->
      <!--The doRequiredTask method must have parameter named retVal -->
      <aop:after-returning pointcut-ref="businessService"
         returning="retVal"
         method="doRequiredTask"/>
      <!-- an after-throwing advice definition -->
      <!--The doRequiredTask method must have parameter named ex -->
      <aop:after-throwing pointcut-ref="businessService"
         throwing="ex"
         method="doRequiredTask"/>
      <!-- an around advice definition -->
      <aop:around pointcut-ref="businessService" 
         method="doRequiredTask"/>
   ...
   </aop:aspect>
</aop:config>
```



