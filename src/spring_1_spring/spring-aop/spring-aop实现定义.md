# Advisor

持有 AOP advice 和 确定 advice的 applicability  的 过滤器（例如 *pointcut*）的 基本接口

这个接口不是供 Spring 用户使用的，而是为了支持不同类型的通知的 通用性。

Advisor 接口允许支持不同类型的建议，例如前后建议，不需要使用拦截来实现。

```
//返回相应的 *Advice*
Advice getAdvice();
```



# PointCut

1. spring poinCut抽象
2. 定义了 类 过滤，方法过滤 的 逻辑



# PointcutAdvisor

1. 既持有 advisor又 持有 pointCut
2. pointcut 负责 对 判断方法是否 应该切入
3. advisor 负责进行如何切入



# Advice

advice负责具体如何进行 切入策略

例如 前置、后置、环绕、等等

在spring中 advice由 *MethodInterceptor*   完成具体逻辑



# JointPoint

在 AOP 术语中 ，此接口表示通用运行连接点 

运行时 连接点  是发生 在  静态连接点（程序上的位置） 上的事件

例如 方法调用 是  方法上（静态连接点）的运行时连接点

可以使用 getStaticPart() 方法一般检索给定连接点的静态部分

在拦截框架的上下文中，运行时连接点是对可访问对象（方法、构造函数、字段）(连接点的静态部分)的访问的具体化，

它被传递给安装在静态连接点上的拦截器。



# TaskList

1. spring事务  代理弄清楚
2. shiro 注解 代理弄清楚
3. 异步 代理 弄清楚
4. 缓存 代理 弄清楚
5. spring如何管理多层代理
6. spring是如何懒加载的
7. 弄清楚 *org.springframework.aop.framework.CglibAopProxy.ProxyCallbackFilter*





# ProxyConfig

## optimize

设置代理 是否应该执行 积极的优化

 The exact meaning of "aggressive optimizations" will differ between proxies, but there is usually some tradeoff. Default is "false".

“积极优化”的确切含义因代理而异，但通常会有一些权衡。默认为 false

例如，优化通常意味着在创建代理后通知更改不会生效,因此，默认情况下禁用优化。

如果其他设置阻止优化，则可以忽略“true”的优化值

An optimize value of "true" may be ignored if other settings preclude optimization: for example, 如果“exposeProxy”设置为“true”与 **optimize**  不兼容。

## opaque

设置是否应防止此配置创建的代理被强制转换为 **Advised** 以查询代理状态

默认是*false* 意味着任何AOP 代理 能够被转换成 *Advised*

## proxyTargetClass

设置是否直接代理目标类，而不是只代理特定的接口。默认为“false”。

设置为“true”以强制代理 TargetSource 的公开目标类。

如果该目标类是一个接口，则会为给定的接口创建一个 JDK 代理

如果该目标类是任何其他类，则将为给定类创建 CGLIB 代理。

根据具体代理工厂的配置，如果没有指定接口（并且没有激活接口自动检测），也将应用代理目标类行为。

## exposeProxy

Set whether the proxy should be exposed by the AOP framework as a ThreadLocal for retrieval via the AopContext class.

设置代理是否应由 AOP 框架公开为 ThreadLocal 以通过 AopContext 类进行检索。

如果一个被通知的对象需要对自己调用另一个被通知的方法，这很有用。

 (If it uses this, the invocation will not be advised).

默认为“false”，以避免不必要的额外拦截

这意味着不保证 AopContext 访问将在建议对象的任何方法中一致地工作。

## frozen

设置是否应冻结此配置。

当配置被冻结时，不能进行任何建议更改

这对于优化很有用，当我们不希望调用者在转换为 Advised 后能够操作配置时很有用。

# ProxyProcessorSupport

具有代理处理器通用功能的基类 特别是 

1. classLoader管理
2. 类的接口查找 算法



# AdvisorAdapter

接口允许扩展到 Spring AOP 框架以允许处理新的 *Advisor* 和 *Advice* type。

实现对象可以从自定义通知类型创建 AOP Alliance  拦截器，使这些通知类型能够在 Spring AOP 框架中使用，该框架在幕后使用拦截。

大多数 Spring 用户不需要实现这个接口；仅当您需要向 Spring 引入更多 Advisor 或 Advice 类型时才这样做。

## **接口方法列表**



```java
// 是否支持Advice
boolean supportsAdvice(Advice advice);
// 返回一个 AOP Alliance MethodInterceptor，将给定建议的行为暴露给基于拦截的 AOP 框架。不要担心Advisor 中包含的任何切入点； AOP 框架将负责检查切入点。
MethodInterceptor getInterceptor(Advisor advisor);
```



# AdvisorAdapterRegistry

> advisorAdapter 注册器接口





# AbstractAutoProxyCreator

**org.springframework.beans.factory.config.BeanPostProcessor** 的一个实现

使用 AOP 代理包装每个符合条件的 bean，在调用 bean 本身之前委托给指定的拦截器。



此类区分“通用”拦截器（为它创建的所有代理共享），以及“特定”拦截器（每个 bean 实例唯一）

他们不需要任何通用的拦截器。如果有，则使用interceptorNames 属性设置它们

与 org.springframework.aop.framework.ProxyFactoryBean 一样，使用当前工厂中的拦截器名称而不是 bean 引用来允许正确处理原型顾问和拦截器：例如，支持有状态的混合。

`interceptorNames`条目支持任何advice类型。

如果有大量 bean 需要用类似的代理包装，即委托给相同的拦截器，这种自动代理特别有用

委托给相同的拦截器。代替 x 个目标 bean 的 x 个重复代理定义，您可以向 bean 工厂注册一个这样的后处理器以实现相同的效果。

子类可以应用任何策略来决定是否要代理 bean，例如按类型、按名称、按定义详细信息等。

它们还可以返回应仅应用于特定 bean 实例的其他拦截器

一个简单的具体实现是 BeanNameAutoProxyCreator，它通过给定的名称识别要代理的 bean。

可以使用任意数量的 TargetSourceCreator 实现来创建自定义目标源

例如，池化原型对象。

Auto-proxying will occur even if there is no advice, as long as a TargetSourceCreator specifies a custom TargetSource. If there are no TargetSourceCreators set, or if none matches, a SingletonTargetSource will be used by default to wrap the target bean instance.



