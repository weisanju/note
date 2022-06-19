# 总结

1. 所有 手动编程式代理 继承 *AbstractBeanFactoryAwareAdvisingPostProcessor*
2. 并在实现中 赋值  *advisor* 

# **ProxyProcessorSupport**

继承自 *ProxyConfig* 在此基础上添加了 类加载器的管理

# AbstractAdvisingBeanPostProcessor

继承自*ProxyProcessorSupport* 的抽象类，主要完成以下功能

1. 如果是*Advised*对象则  把当前*Advisor*加入  到其中
2. 判断 目标bean是否 应该被代理
3. 应该代理的话，则 使用*ProxyFactory* 进行代理

**源码**

```java
	public Object postProcessAfterInitialization(Object bean, String beanName) {
		if (this.advisor == null || bean instanceof AopInfrastructureBean) {
			// Ignore AOP infrastructure such as scoped proxies.
			return bean;
		}
		//如果是 advisedbean  将 其加入到 advised中
		if (bean instanceof Advised) {
			Advised advised = (Advised) bean;
			if (!advised.isFrozen() && isEligible(AopUtils.getTargetClass(bean))) {
				// Add our local Advisor to the existing proxy's Advisor chain...
				if (this.beforeExistingAdvisors) {
					advised.addAdvisor(0, this.advisor);
				}
				else {
					advised.addAdvisor(this.advisor);
				}
				return bean;
			}
		}
		//bean是否应该进行代理
		if (isEligible(bean, beanName)) {
			ProxyFactory proxyFactory = prepareProxyFactory(bean, beanName);
            //如果是基于接口的代理 则解析接口
			if (!proxyFactory.isProxyTargetClass()) {
				evaluateProxyInterfaces(bean.getClass(), proxyFactory);
			}
			proxyFactory.addAdvisor(this.advisor);
			customizeProxyFactory(proxyFactory);

			// Use original ClassLoader if bean class not locally loaded in overriding class loader
			ClassLoader classLoader = getProxyClassLoader();
			if (classLoader instanceof SmartClassLoader && classLoader != bean.getClass().getClassLoader()) {
				classLoader = ((SmartClassLoader) classLoader).getOriginalClassLoader();
			}
			return proxyFactory.getProxy(classLoader);
		}

		// No proxy needed.
		return bean;
	}
```

# AbstractBeanFactoryAwareAdvisingPostProcessor

持有 *BeanFactory* 的引用，主要完成了以下功能

1. 将targetClass 存入相应Bean定义
2. 判断是否应该 对类进行 代理或者对接口进行代理

```java
	protected ProxyFactory prepareProxyFactory(Object bean, String beanName) {
		if (this.beanFactory != null) {
			AutoProxyUtils.exposeTargetClass(this.beanFactory, beanName, bean.getClass());
		}

		ProxyFactory proxyFactory = super.prepareProxyFactory(bean, beanName);
		if (!proxyFactory.isProxyTargetClass() && this.beanFactory != null &&
				AutoProxyUtils.shouldProxyTargetClass(this.beanFactory, beanName)) {
			proxyFactory.setProxyTargetClass(true);
		}
		return proxyFactory;
	}
```

# 示例

## AsyncAnnotationBeanPostProcessor

Bean 后处理器，通过将相应的 *AsyncAnnotationAdvisor* 添加到公开的代理（现有的 AOP 代理或新生成的实现所有目标的代理接口）。

可以提供负责异步执行的 TaskExecutor 以及指示应该异步调用方法的注释类型。如果未指定注释类型，则此后处理器将检测 Spring 的 @Async 注释以及 EJB 3.1 javax.ejb.Asynchronous 注释。

对于具有 void 返回类型的方法，调用者无法访问异步方法调用期间抛出的任何异常。可以指定 AsyncUncaughtExceptionHandler 来处理这些情况。

默认情况下，底层异步*Advisor*在现有 *Advisor* 之前应用, 以便在调用链中尽早切换到异步执行。



该类主要完成 Advisor的构建

构建 *AsyncAnnotationAdvisor* 所需的

1. *Executors*
2. *BeanFactory*
3. *exceptionHandler*
4. 且默认是在所有Advisor之前执行
