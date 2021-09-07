*AnnotationAwareAspectJAutoProxyCreator*

# 启用

> ```
> @EnableAspectJAutoProxy(proxyTargetClass = true)
> ```

## **注册调用链**

```
AbstractApplicationContext#refresh //重启容器
    AbstractApplicationContext#obtainFreshBeanFactory //刷新 BeanFactory
    AbstractApplicationContext#invokeBeanFactoryPostProcessors // 回调 BeanFactoryPostProcessor
        PostProcessorRegistrationDelegate#invokeBeanDefinitionRegistryPostProcessors //注册bean定义
            ConfigurationClassPostProcessor#postProcessBeanDefinitionRegistry //根据配置类注册bean定义
                ConfigurationClassBeanDefinitionReader#loadBeanDefinitions //根据配置类解析bean定义
                	ConfigurationClassBeanDefinitionReader#loadBeanDefinitionsForConfigurationClass //根据配置类解析bean定义
                		ConfigurationClassBeanDefinitionReader#loadBeanDefinitionsFromRegistrars //从配置类上的 注册员 获取bean定义
                			ImportBeanDefinitionRegistrar#registerBeanDefinitions //通用注册员
                			AspectJAutoProxyRegistrar#registerBeanDefinitions //AOP注册员
```

## **注册方法**

### **主注册方法**

1. 注册 AOP切面Bean定义
2. 配置AOP切面 *proxyTargetClass* 与 *exposeProxy* 属性

```java
AopConfigUtils.registerAspectJAnnotationAutoProxyCreatorIfNecessary(registry);

AnnotationAttributes enableAspectJAutoProxy =
      AnnotationConfigUtils.attributesFor(importingClassMetadata, EnableAspectJAutoProxy.class);
if (enableAspectJAutoProxy != null) {
   if (enableAspectJAutoProxy.getBoolean("proxyTargetClass")) {
      AopConfigUtils.forceAutoProxyCreatorToUseClassProxying(registry);
   }
   if (enableAspectJAutoProxy.getBoolean("exposeProxy")) {
      AopConfigUtils.forceAutoProxyCreatorToExposeProxy(registry);
   }
}
```

### **注册 AOP切面Bean定义**

**调用链**

```
AspectJAutoProxyRegistrar#registerBeanDefinitions
	AopConfigUtils#registerOrEscalateApcAsRequired
```

**主方法**

```java
private static BeanDefinition registerOrEscalateApcAsRequired(
      Class<?> cls, BeanDefinitionRegistry registry, @Nullable Object source) {

   Assert.notNull(registry, "BeanDefinitionRegistry must not be null");

   if (registry.containsBeanDefinition(AUTO_PROXY_CREATOR_BEAN_NAME)) {
       ......
   }

   RootBeanDefinition beanDefinition = new RootBeanDefinition(cls);
   beanDefinition.setSource(source);
   beanDefinition.getPropertyValues().add("order", Ordered.HIGHEST_PRECEDENCE); //顺序
   beanDefinition.setRole(BeanDefinition.ROLE_INFRASTRUCTURE); //基础设施bean
   registry.registerBeanDefinition(AUTO_PROXY_CREATOR_BEAN_NAME, beanDefinition);
   return beanDefinition;
}
```

**注册实际Bean定义**

*org.springframework.aop.aspectj.annotation.AnnotationAwareAspectJAutoProxyCreator*



# 初始化BeanPostProcessor

```java
AbstractApplicationContext#refresh //重启容器
    AbstractApplicationContext#obtainFreshBeanFactory //刷新 BeanFactory
    AbstractApplicationContext#invokeBeanFactoryPostProcessors // 回调 BaanFactoryPostProcessor，用来注册bean定义
    AbstractApplicationContext#registerBeanPostProcessors //注册客户端BeanPostProcessor，根据bean定义实例化 BeanPostProcessor
    PostProcessorRegistrationDelegate#registerBeanPostProcessors(beanFactory,AbstractApplicationContext applicationContext) //实例化并获取BeanPostProcessor
    PostProcessorRegistrationDelegate#registerBeanPostProcessors(beanFactory,List<BeanPostProcessor> postProcessors) //注册客户端BeanPostProcessor
```



# 初始化切面类

懒加载

在所有 单例

```java
AbstractBeanFactory#doGetBean
    AbstractAutowireCapableBeanFactory#createBean
        AbstractAutowireCapableBeanFactory#resolveBeforeInstantiation
        AbstractAutowireCapableBeanFactory#doCreateBean
            AbstractAutowireCapableBeanFactory#createBeanInstance
            AbstractAutowireCapableBeanFactory#populateBean
            AbstractAutowireCapableBeanFactory#initializeBean
                AbstractAutowireCapableBeanFactory#invokeAwareMethods //调用 Aware方法
                AbstractAutowireCapableBeanFactory#applyBeanPostProcessorsBeforeInitialization //调用 postProcess
                    BeanPostProcessor#postProcessBeforeInitialization //beforeInitialization
                AbstractAutowireCapableBeanFactory#invokeInitMethods //调用初始化方法
                    InitializingBean#afterPropertiesSet //InitializingBean类
                    AbstractAutowireCapableBeanFactory#invokeCustomInitMethod //自定义初始化方法
                AbstractAutowireCapableBeanFactory#applyBeanPostProcessorsAfterInitialization //在此处懒加载切面
					AbstractAutoProxyCreator#postProcessAfterInitialization //在此处懒加载切面
						AbstractAutoProxyCreator#wrapIfNecessary
						AbstractAutoProxyCreator#shouldSkip
							AbstractAdvisorAutoProxyCreator#findCandidateAdvisors//此处查找 Advisor
							BeanFactoryAspectJAdvisorsBuilder#buildAspectJAdvisors //根据注解 构建Advisor
								AspectJAdvisorFactory#isAspect //判断是否带有 Apsect注解
								AspectJAdvisorFactory#getAdvisors //根据 class生成Advisor
```



# 拦截增强

## 方法调用栈

```java
AbstractBeanFactory#doGetBean
    AbstractAutowireCapableBeanFactory#createBean
        AbstractAutowireCapableBeanFactory#resolveBeforeInstantiation
        AbstractAutowireCapableBeanFactory#doCreateBean
            AbstractAutowireCapableBeanFactory#createBeanInstance
            AbstractAutowireCapableBeanFactory#populateBean
            AbstractAutowireCapableBeanFactory#initializeBean
                AbstractAutowireCapableBeanFactory#invokeAwareMethods //调用 Aware方法
                AbstractAutowireCapableBeanFactory#applyBeanPostProcessorsBeforeInitialization //调用 postProcess
                    BeanPostProcessor#postProcessBeforeInitialization //beforeInitialization
                AbstractAutowireCapableBeanFactory#invokeInitMethods //调用初始化方法
                    InitializingBean#afterPropertiesSet //InitializingBean类
                    AbstractAutowireCapableBeanFactory#invokeCustomInitMethod //自定义初始化方法
                AbstractAutowireCapableBeanFactory#applyBeanPostProcessorsAfterInitialization //在此处懒加载切面
					AbstractAutoProxyCreator#postProcessAfterInitialization //在此处懒加载切面
						AbstractAutoProxyCreator#wrapIfNecessary
						AbstractAutoProxyCreator#getAdvicesAndAdvisorsForBean //获取与 目标Bean相匹配的 Advisor
						AbstractAutoProxyCreator#createProxy //创建代理
```





```java
	public Object postProcessAfterInitialization(@Nullable Object bean, String beanName) {
		if (bean != null) {
			Object cacheKey = getCacheKey(bean.getClass(), beanName);
			if (this.earlyProxyReferences.remove(cacheKey) != bean) {
				return wrapIfNecessary(bean, beanName, cacheKey);
			}
		}
		return bean;
	}
```



## 核心代理方法

```java
protected Object wrapIfNecessary(Object bean, String beanName, Object cacheKey) {
   if (StringUtils.hasLength(beanName) && this.targetSourcedBeans.contains(beanName)) {
      return bean;
   }
   if (Boolean.FALSE.equals(this.advisedBeans.get(cacheKey))) {
      return bean;
   }
   if (isInfrastructureClass(bean.getClass()) || shouldSkip(bean.getClass(), beanName)) {
      this.advisedBeans.put(cacheKey, Boolean.FALSE);
      return bean;
   }

   // Create proxy if we have advice.
   Object[] specificInterceptors = getAdvicesAndAdvisorsForBean(bean.getClass(), beanName, null);
   if (specificInterceptors != DO_NOT_PROXY) {
      this.advisedBeans.put(cacheKey, Boolean.TRUE);
      Object proxy = createProxy(
            bean.getClass(), beanName, specificInterceptors, new SingletonTargetSource(bean));
      this.proxyTypes.put(cacheKey, proxy.getClass());
      return proxy;
   }

   this.advisedBeans.put(cacheKey, Boolean.FALSE);
   return bean;
}
```

## 创建AopProxyFactory

> 使用该工厂 创建代理类

如果目标类是接口 或者 是代理类

则启用Jdk动态代理

否则启动ObjenesisCglib代理

```java
public AopProxy createAopProxy(AdvisedSupport config) throws AopConfigException {
   if (config.isOptimize() || config.isProxyTargetClass() || hasNoUserSuppliedProxyInterfaces(config)) {
      Class<?> targetClass = config.getTargetClass();
      if (targetClass == null) {
         throw new AopConfigException("TargetSource cannot determine target class: " +
               "Either an interface or a target is required for proxy creation.");
      }
      if (targetClass.isInterface() || Proxy.isProxyClass(targetClass)) {
         return new JdkDynamicAopProxy(config);
      }
      return new ObjenesisCglibAopProxy(config);
   }
   else {
      return new JdkDynamicAopProxy(config);
   }
}
```









