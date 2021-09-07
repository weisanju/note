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



即使没有   **advice**，只要 TargetSourceCreator 指定自定义 TargetSource，就会发生自动代理。

如果没有设置 TargetSourceCreators，或者没有匹配项，默认情况下将使用 SingletonTargetSource 来包装目标 bean 实例。



# 代理过程中出现的对象

## AopProxyFactory

AopProxy代理工厂类，用于生成代理对象AopProxy

## AopProxy

代表一个AopProxy代理对象，可以通过这个对象构造代理对象实例。

```java
public interface AopProxy {
 Object getProxy();
 Object getProxy(ClassLoader classLoader);
}
```

## Advised接口

代表被Advice增强的对象，包括添加advisor的方法、添加advice等的方法。



## ProxyConfig类

一个代理对象的配置信息，包括代理的各种属性，如基于接口还是基于类构造代理。



## AdvisedSupport类

对Advised的构建提供支持，Advised的实现类以及ProxyConfig的子类。



## ProxyCreatorSupport

AdvisedSupport的子类，创建代理对象的支持类，内部包含AopProxyFactory工厂成员，可直接使用工厂成员创建Proxy。

## ProxyFactory类

ProxyCreatorSupport的子类，用于生成代理对象实例的工厂类



## Advisor接口

代表一个增强器提供者的对象，内部包含getAdvice方法获取增强器。



## AdvisorChainFactory

获取增强器链的工厂接口。提供方法返回所有增强器，以数组返回。

## Pointcut接口

切入点，用于匹配类与方法，满足切入点的条件是才插入advice。相关接口：ClassFilter、MethodMatcher。	



# Spring如何选用底层代理

```java
public class DefaultAopProxyFactory implements AopProxyFactory, Serializable {
	@Override
	public AopProxy createAopProxy(AdvisedSupport config) throws AopConfigException {
		if (config.isOptimize() || config.isProxyTargetClass() || hasNoUserSuppliedProxyInterfaces(config)) {
		// 如果是需要优化的代理，或者标记代理目标类，或者代理配置中没有需要代理的接口
			Class<?> targetClass = config.getTargetClass();
			if (targetClass == null) {
				throw new AopConfigException("TargetSource cannot determine target class: " +
						"Either an interface or a target is required for proxy creation.");
			}
			if (targetClass.isInterface() || Proxy.isProxyClass(targetClass)) {
			 // 如果目标类是接口，或者已经是Jdk的动态代理类，则创建jdk动态代理
				return new JdkDynamicAopProxy(config);
			}
			// 否则创建Cglib动态代理
			return new ObjenesisCglibAopProxy(config);
		}
		else {
		 // 如果声明创建Jdk动态代理则返回Jdk动态代理
		 return new JdkDynamicAopProxy(config);
		}
	}
}
```



# jdk动态代理

## 实例化

实例化 *JdkDynamicAopProxy* 时  获取所有需要代理的接口

```java
public JdkDynamicAopProxy(AdvisedSupport config) throws AopConfigException {
   Assert.notNull(config, "AdvisedSupport must not be null");
   if (config.getAdvisorCount() == 0 && config.getTargetSource() == AdvisedSupport.EMPTY_TARGET_SOURCE) {
      throw new AopConfigException("No advisors and no TargetSource specified");
   }
   this.advised = config;
   this.proxiedInterfaces = AopProxyUtils.completeProxiedInterfaces(this.advised, true);
   findDefinedEqualsAndHashCodeMethods(this.proxiedInterfaces);
}
```

## **获取实例对象**

```java

public Object getProxy(ClassLoader classLoader) {
	if (logger.isDebugEnabled()) {
		logger.debug("Creating JDK dynamic proxy: target source is " + this.advised.getTargetSource());
	}
	// 获取所有需要代理的接口
	Class<?>[] proxiedInterfaces = AopProxyUtils.completeProxiedInterfaces(this.advised, true);
	findDefinedEqualsAndHashCodeMethods(proxiedInterfaces);
	// 返回代理对象的实例
	return Proxy.newProxyInstance(classLoader, proxiedInterfaces, this);
}
```

## 代理对象入口方法

自己作为InvocationHandler注册，看他的invoke方法

```java
//org.springframework.aop.framework.JdkDynamicAopProxy#invokes
public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
		Object oldProxy = null;
		boolean setProxyContext = false;

		TargetSource targetSource = this.advised.targetSource;
		Object target = null;

		try {
	//	没有声明equals方法，调用equals方法时，委托调用。
			if (!this.equalsDefined && AopUtils.isEqualsMethod(method)) {
				// The target does not implement the equals(Object) method itself.
				return equals(args[0]);
			}
			// 没有声明hashCode方法，调用hashCode方法时，委托调用。
			else if (!this.hashCodeDefined && AopUtils.isHashCodeMethod(method)) {
				// The target does not implement the hashCode() method itself.
				return hashCode();
			}
			// 如果调用的方法是DecoratingProxy中的方法，因为其中只有一个getDecoratedClass方法，这里直接返回被装饰的Class即可
			else if (method.getDeclaringClass() == DecoratingProxy.class) {
				// There is only getDecoratedClass() declared -> dispatch to proxy config.
				return AopProxyUtils.ultimateTargetClass(this.advised);
			}
			// 代理不是不透明的，且是接口中声明的方法，且是Advised或其父接口的方法，则直接调用构造时传入的advised对象的相应方法
			else if (!this.advised.opaque && method.getDeclaringClass().isInterface() &&
					method.getDeclaringClass().isAssignableFrom(Advised.class)) {
				// Service invocations on ProxyConfig with the proxy config...
				return AopUtils.invokeJoinpointUsingReflection(this.advised, method, args);
			}

			Object retVal;
 // 如果暴露代理，则用AopContext保存当前代理对象。用于多级代理时获取当前的代理对象，一个有效应用是同类中调用方法，代理拦截器会无效。可以使用AopContext.currentProxy()获得代理对象并调用。
			if (this.advised.exposeProxy) {
				// Make invocation available if necessary.
				oldProxy = AopContext.setCurrentProxy(proxy);
				setProxyContext = true;
			}

			// Get as late as possible to minimize the time we "own" the target,
			// in case it comes from a pool.
			target = targetSource.getTarget();
			Class<?> targetClass = (target != null ? target.getClass() : null);

		// 这里是关键，获得拦截链chain，是通过advised对象，即config对象获得的。
			List<Object> chain = this.advised.getInterceptorsAndDynamicInterceptionAdvice(method, targetClass);

			// Check whether we have any advice. If we don't, we can fallback on direct
			// reflective invocation of the target, and avoid creating a MethodInvocation.
			if (chain.isEmpty()) {
				// We can skip creating a MethodInvocation: just invoke the target directly
				// Note that the final invoker must be an InvokerInterceptor so we know it does
				// nothing but a reflective operation on the target, and no hot swapping or fancy proxying.
				Object[] argsToUse = AopProxyUtils.adaptArgumentsIfNecessary(method, args);			// 如果链是空，则直接调用被代理对象的方法
				retVal = AopUtils.invokeJoinpointUsingReflection(target, method, argsToUse);
			}
			else {
			// 否则创建一个MethodInvocation对象，用于链式调用拦截器链chain中的拦截器。
				// We need to create a method invocation...
				MethodInvocation invocation =
						new ReflectiveMethodInvocation(proxy, target, method, args, targetClass, chain);
				// 开始执行链式调用，得到返回结果
				retVal = invocation.proceed();
			}

			// Massage return value if necessary.
			Class<?> returnType = method.getReturnType();
			if (retVal != null && retVal == target &&
					returnType != Object.class && returnType.isInstance(proxy) &&
					!RawTargetAccess.class.isAssignableFrom(method.getDeclaringClass())) {
				// Special case: it returned "this" and the return type of the method
				// is type-compatible. Note that we can't help if the target sets
				// a reference to itself in another returned object.
					// 处理返回值
			// 如果返回结果是this，即原始对象，且方法所在类没有标记为RawTargetAccess(不是RawTargetAccess的实现类或者子接口)，则返回代理对象。
				retVal = proxy;
			}
			else if (retVal == null && returnType != Void.TYPE && returnType.isPrimitive()) {
				throw new AopInvocationException(
						"Null return value from advice does not match primitive return type for: " + method);
			}
			return retVal;
		}
		finally {
			if (target != null && !targetSource.isStatic()) {
				// Must have come from TargetSource.
				targetSource.releaseTarget(target);
			}
			if (setProxyContext) {
				// Restore old proxy.
				AopContext.setCurrentProxy(oldProxy);
			}
		}
	}
```

## 执行调用链

```java
//org.springframework.aop.framework.ReflectiveMethodInvocation#proceed
	public Object proceed() throws Throwable {
		// We start with an index of -1 and increment early.
		if (this.currentInterceptorIndex == this.interceptorsAndDynamicMethodMatchers.size() - 1) {
            // 链全部执行完，再次调用proceed时，返回原始对象方法调用执行结果。递归的终止。
			return invokeJoinpoint();
		}
// 用currentInterceptorIndex记录当前的interceptor位置，初值-1，先++再获取。当再拦截器中调用invocation.proceed()时，递归进入此方法，索引向下移位，获取下一个拦截器。
		Object interceptorOrInterceptionAdvice =
				this.interceptorsAndDynamicMethodMatchers.get(++this.currentInterceptorIndex);
         // 如果是InterceptorAndDynamicMethodMatcher则再执行一次动态匹配
		if (interceptorOrInterceptionAdvice instanceof InterceptorAndDynamicMethodMatcher) {
			// Evaluate dynamic method matcher here: static part will already have
			// been evaluated and found to match.
			InterceptorAndDynamicMethodMatcher dm =
					(InterceptorAndDynamicMethodMatcher) interceptorOrInterceptionAdvice;
			Class<?> targetClass = (this.targetClass != null ? this.targetClass : this.method.getDeclaringClass());
			if (dm.methodMatcher.matches(this.method, targetClass, this.arguments)) {
                 // 匹配成功，执行
				return dm.interceptor.invoke(this);
			}
			else {
				// Dynamic matching failed.
				// Skip this interceptor and invoke the next in the chain.
                // 匹配失败，跳过该拦截器，递归调用本方法，执行下一个拦截器。
				return proceed();
			}
		}
		else {
			// It's an interceptor, so we just invoke it: The pointcut will have
			// been evaluated statically before this object was constructed.
            // 如果是interceptor，则直接调用invoke。把自己作为invocation，以便在invoke方法中，调用invocation.proceed()来执行递归。或者invoke中也可以不执行invocation.proceed()，强制结束递归，返回指定对象作为结果。
			return ((MethodInterceptor) interceptorOrInterceptionAdvice).invoke(this);
		}
	}
```

## 获取拦截器

```java
//org.springframework.aop.framework.DefaultAdvisorChainFactory#getInterceptorsAndDynamicInterceptionAdvice


	public List<Object> getInterceptorsAndDynamicInterceptionAdvice(
			Advised config, Method method, @Nullable Class<?> targetClass) {

		// This is somewhat tricky... We have to process introductions first,
		// but we need to preserve order in the ultimate list.
		AdvisorAdapterRegistry registry = GlobalAdvisorAdapterRegistry.getInstance();
        
        //获取 Advised对象中的 advisors
		Advisor[] advisors = config.getAdvisors();
		List<Object> interceptorList = new ArrayList<>(advisors.length);
		Class<?> actualClass = (targetClass != null ? targetClass : method.getDeclaringClass());
		Boolean hasIntroductions = null;

     	//从 advisor手上 获取 Advice
		for (Advisor advisor : advisors) {
            //如果是带有切面的，则执行切面条件匹配
			if (advisor instanceof PointcutAdvisor) {
				// Add it conditionally.
				PointcutAdvisor pointcutAdvisor = (PointcutAdvisor) advisor;
                //如果 Class匹配则 继续进行方法匹配
				if (config.isPreFiltered() || pointcutAdvisor.getPointcut().getClassFilter().matches(actualClass)) {
					MethodMatcher mm = pointcutAdvisor.getPointcut().getMethodMatcher();
					boolean match;
					if (mm instanceof IntroductionAwareMethodMatcher) {
						if (hasIntroductions == null) {
							hasIntroductions = hasMatchingIntroductions(advisors, actualClass);
						}
						match = ((IntroductionAwareMethodMatcher) mm).matches(method, actualClass, hasIntroductions);
					}
					else {
						match = mm.matches(method, actualClass);
					}
					if (match) {
                        //匹配成功且 还包含运行时匹配的：则创建 运行时匹配对象
                        //这里从 advisor中获取 advice,包含从spring的 interceptor转换为 aop的interceptor
						MethodInterceptor[] interceptors = registry.getInterceptors(advisor);
						if (mm.isRuntime()) {
							// Creating a new object instance in the getInterceptors() method
							// isn't a problem as we normally cache created chains.
							for (MethodInterceptor interceptor : interceptors) {
								interceptorList.add(new InterceptorAndDynamicMethodMatcher(interceptor, mm));
							}
						}
						else {
							interceptorList.addAll(Arrays.asList(interceptors));
						}
					}
				}
			}
			else if (advisor instanceof IntroductionAdvisor) {
				IntroductionAdvisor ia = (IntroductionAdvisor) advisor;
				if (config.isPreFiltered() || ia.getClassFilter().matches(actualClass)) {
					Interceptor[] interceptors = registry.getInterceptors(advisor);
					interceptorList.addAll(Arrays.asList(interceptors));
				}
			}
			else {
				Interceptor[] interceptors = registry.getInterceptors(advisor);
				interceptorList.addAll(Arrays.asList(interceptors));
			}
		}

		return interceptorList;
	}
```



## Advised对象中的 Advisor如何来的

## Advised对象中的 targetSource如何来的



## AdvisedSupport对象的作用

### 获取拦截器

### *提供targetSource*

### 查找需要代理的接口

### 代理对象的配置

#### 透明与不透明

#### 暴露代理

