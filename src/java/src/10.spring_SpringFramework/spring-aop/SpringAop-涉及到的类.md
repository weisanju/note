# ProxyConfig

一个代理对象的配置信息，包括代理的各种属性，如基于接口还是基于类构造代理。

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

> 继承于 *ProxyConfig*

具有代理处理器通用功能的基类 特别是 

1. classLoader管理
2. 类的接口查找算法



# AdvisorAdapter

实现对象可以从自定义通知类型创建 AOP Alliance  拦截器，使这些通知类型能够在 Spring AOP 框架中使用，该框架在幕后使用拦截。

大多数 Spring 用户不需要实现这个接口；仅当您需要向 Spring 引入更多 Advisor 或 Advice 类型时才这样做。

```java
// 是否支持Advice
boolean supportsAdvice(Advice advice);
// 返回一个 AOP Alliance MethodInterceptor，将给定建议的行为暴露给基于拦截的 AOP 框架。不要担心Advisor 中包含的任何切入点； AOP 框架将负责检查切入点。
MethodInterceptor getInterceptor(Advisor advisor);
```

# AdvisorAdapterRegistry

> advisorAdapter 注册器接口



# AbstractAutoProxyCreator

使用 AOP 代理包装每个符合条件的 bean，在调用 bean 本身之前委托给指定的拦截器。

此类区分“通用”拦截器（为它创建的所有代理共享），以及“特定”拦截器（每个 bean 实例唯一）

他们不需要任何通用的拦截器。如果有，则使用interceptorNames 属性设置它们

与 org.springframework.aop.framework.ProxyFactoryBean 一样，使用当前工厂中的拦截器名称而不是 bean 引用来允许正确处理原型顾问和拦截器：例如，支持有状态的混合。

`interceptorNames`条目支持任何advice类型。

如果有大量 bean 需要用类似的代理包装，即委托给相同的拦截器，这种自动代理特别有用委托给相同的拦截器。

代替 x 个目标 bean 的 x 个重复代理定义，您可以向 bean 工厂注册一个这样的后处理器以实现相同的效果。

子类可以应用任何策略来决定是否要代理 bean，例如按类型、按名称、按定义详细信息等。

它们还可以返回应仅应用于特定 bean 实例的其他拦截器

一个简单的具体实现是 BeanNameAutoProxyCreator，它通过给定的名称识别要代理的 bean。

可以使用任意数量的 TargetSourceCreator 实现来创建自定义目标源

例如，池化原型对象。

即使没有   **advice**，只要 TargetSourceCreator 指定自定义 TargetSource，就会发生自动代理。

如果没有设置 TargetSourceCreators，或者没有匹配项，默认情况下将使用 SingletonTargetSource 来包装目标 bean 实例。



# AopProxyFactory

AopProxy代理工厂类，用于生成代理对象AopProxy

# AopProxy

代表一个AopProxy代理对象，可以通过这个对象构造代理对象实例。

```java
public interface AopProxy {
 Object getProxy();
 Object getProxy(ClassLoader classLoader);
}
```

# Advised接口

代表被Advice增强的对象，包括添加advisor的方法、添加advice等的方法。



# AdvisedSupport类

对Advised的构建提供支持，Advised的实现类以及ProxyConfig的子类。

1. 提供Advisor的 增删改查

2. 提供 *AdvisorChainFactory* 从 Advisor中获取所有 *Advice*

**AdvisedSupport对象的作用**

1. 获取拦截器

2. 提供targetSource

3. 提供Advisor的憎删改查

4. 代理对象的配置

5. 透明与不透明

6. 暴露代理



# ProxyCreatorSupport

AdvisedSupport的子类，创建代理对象的支持类，内部包含**AopProxyFactory**工厂成员，可直接使用工厂成员创建Proxy。

# ProxyFactory类

ProxyCreatorSupport的子类，**用于生成代理对象实例的工厂类**

相比 ProxyCreatorSupport 提供了 *getProxy* 的方法

# Advisor接口

代表一个增强器提供者的对象，内部包含getAdvice方法获取增强器。



# AdvisorChainFactory

获取增强器链的工厂接口。提供方法返回所有增强器，以数组返回。

# Pointcut接口

切入点，用于匹配类与方法，满足切入点的条件是才插入advice。相关接口：ClassFilter、MethodMatcher。



# AnnotationMatchingPointcut

基于注解的*PointCut*

有三个成员变量

1. 基于类的注解 Class：用于类匹配
2. 基于方法的注解 Class：用于方法匹配
3. 是由启用继承





# AbstractBeanFactoryPointcutAdvisor

基于*Beanfactory* 的 *PointcutAdvisor*,从 Beanfactory获取Advice

成员变量

*adviceBeanName*

*beanFactory*

*advice*

```java
public Advice getAdvice() {
   Advice advice = this.advice;
   if (advice != null) {
      return advice;
   }

   Assert.state(this.adviceBeanName != null, "'adviceBeanName' must be specified");
   Assert.state(this.beanFactory != null, "BeanFactory must be set to resolve 'adviceBeanName'");
	//单例的化从 Bean工厂获取后 缓存
   if (this.beanFactory.isSingleton(this.adviceBeanName)) {
      // Rely on singleton semantics provided by the factory.
      advice = this.beanFactory.getBean(this.adviceBeanName, Advice.class);
      this.advice = advice;
      return advice;
   }
   else {
      // No singleton guarantees from the factory -> let's lock locally but
      // reuse the factory's singleton lock, just in case a lazy dependency
      // of our advice bean happens to trigger the singleton lock implicitly...
       //懒加载、加锁
      synchronized (this.adviceMonitor) {
         advice = this.advice;
         if (advice == null) {
            advice = this.beanFactory.getBean(this.adviceBeanName, Advice.class);
            this.advice = advice;
         }
         return advice;
      }
   }
}
```





