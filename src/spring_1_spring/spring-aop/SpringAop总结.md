# Spring是如何选择底层字节码操作框架的？

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

# SpringAOP进行代理的两大类

## 继承*AbstractAdvisingBeanPostProcessor*

编程式手动进行代理

## 继承 *AbstractAutoProxyCreator*

自动从容器中获取 *Adviosr* 进行代理



# SpringTargetSource是如何工作的

## 介绍

TargetSource 用于获取 *AOP invocation* 的 当前“目标”

如果 TargetSource 是“静态的”，它将始终返回相同的目标，从而允许在 AOP 框架中进行优化。

动态目标源可以支持池化、热插拔等。

```java
//返回目标类
Class<?> getTargetClass();
//每次调用是否会返回同一个类，如果为false 则AOP不会进行缓存，为true,AOP会进行缓存
boolean isStatic();
//获取目标类
Object getTarget() throws Exception;
// Release the given target object obtained from the getTarget() method, if any.
void releaseTarget(Object target) throws Exception;	
```

普通场景下 包装了 被代理对象







