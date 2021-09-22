# 前言

基于代理的 自动注册类 都继承于 *AdviceModeImportSelector* 抽象类



# 源码

```java
public final String[] selectImports(AnnotationMetadata importingClassMetadata) {
    //获取实际泛型类
   Class<?> annType = GenericTypeResolver.resolveTypeArgument(getClass(), AdviceModeImportSelector.class);
   Assert.state(annType != null, "Unresolvable type argument for AdviceModeImportSelector");

    //获取泛型属性对象
   AnnotationAttributes attributes = AnnotationConfigUtils.attributesFor(importingClassMetadata, annType);
   if (attributes == null) {
      throw new IllegalArgumentException(String.format(
            "@%s is not present on importing class '%s' as expected",
            annType.getSimpleName(), importingClassMetadata.getClassName()));
   }
	//获取代理模式
   AdviceMode adviceMode = attributes.getEnum(getAdviceModeAttributeName());
    //子类根据不同代理模式：返回不同注入类
   String[] imports = selectImports(adviceMode);
   if (imports == null) {
      throw new IllegalArgumentException("Unknown AdviceMode: " + adviceMode);
   }
   return imports;
}
```



# 启用异步调用

**类选中逻辑**

```java
public String[] selectImports(AdviceMode adviceMode) {
   switch (adviceMode) {
      case PROXY:
           //返回 异步配置Configuration类
         return new String[] {ProxyAsyncConfiguration.class.getName()};
      case ASPECTJ:
           //返回AspectJ代理类
         return new String[] {ASYNC_EXECUTION_ASPECT_CONFIGURATION_CLASS_NAME};
      default:
         return null;
   }
}
```

**类注入逻辑**

```java
//ProxyAsyncConfiguration
@Configuration
@Role(BeanDefinition.ROLE_INFRASTRUCTURE)
public class ProxyAsyncConfiguration extends AbstractAsyncConfiguration {

	@Bean(name = TaskManagementConfigUtils.ASYNC_ANNOTATION_PROCESSOR_BEAN_NAME)
	@Role(BeanDefinition.ROLE_INFRASTRUCTURE)
    //注入 异步注解后置处理器
	public AsyncAnnotationBeanPostProcessor asyncAdvisor() {
		Assert.notNull(this.enableAsync, "@EnableAsync annotation metadata was not injected");
		AsyncAnnotationBeanPostProcessor bpp = new AsyncAnnotationBeanPostProcessor();
		bpp.configure(this.executor, this.exceptionHandler);
		Class<? extends Annotation> customAsyncAnnotation = this.enableAsync.getClass("annotation");
		if (customAsyncAnnotation != AnnotationUtils.getDefaultValue(EnableAsync.class, "annotation")) {
			bpp.setAsyncAnnotationType(customAsyncAnnotation);
		}
		bpp.setProxyTargetClass(this.enableAsync.getBoolean("proxyTargetClass"));
		bpp.setOrder(this.enableAsync.<Integer>getNumber("order"));
		return bpp;
	}
}

//抽象类：
@Configuration
public abstract class AbstractAsyncConfiguration implements ImportAware {

	@Nullable
	protected AnnotationAttributes enableAsync;

	@Nullable
	protected Supplier<Executor> executor;

	@Nullable
	protected Supplier<AsyncUncaughtExceptionHandler> exceptionHandler;


	@Override
	public void setImportMetadata(AnnotationMetadata importMetadata) {
		this.enableAsync = AnnotationAttributes.fromMap(
				importMetadata.getAnnotationAttributes(EnableAsync.class.getName(), false));
		if (this.enableAsync == null) {
			throw new IllegalArgumentException(
					"@EnableAsync is not present on importing class " + importMetadata.getClassName());
		}
	}

	/**
	 * Collect any {@link AsyncConfigurer} beans through autowiring.
	 */
	@Autowired(required = false)
    //注入异步配置
	void setConfigurers(Collection<AsyncConfigurer> configurers) {
		if (CollectionUtils.isEmpty(configurers)) {
			return;
		}
		if (configurers.size() > 1) {
			throw new IllegalStateException("Only one AsyncConfigurer may exist");
		}
		AsyncConfigurer configurer = configurers.iterator().next();
		this.executor = configurer::getAsyncExecutor;
		this.exceptionHandler = configurer::getAsyncUncaughtExceptionHandler;
	}
}
```

# 启用缓存

**类选中逻辑**

```java
//org.springframework.cache.annotation.CachingConfigurationSelector

	@Override
	public String[] selectImports(AdviceMode adviceMode) {
		switch (adviceMode) {
			case PROXY: //基于spring的代理
				return getProxyImports();
			case ASPECTJ://基于aspectj的代理
				return getAspectJImports();
			default:
				return null;
		}
	}

//spring代理
	private String[] getProxyImports() {
		List<String> result = new ArrayList<>(3);
        //注入 自动代理注册器：注册自动代理的类 org.springframework.aop.framework.autoproxy.InfrastructureAdvisorAutoProxyCreator
		result.add(AutoProxyRegistrar.class.getName());
        //注入缓存代理配置
		result.add(ProxyCachingConfiguration.class.getName());
		if (jsr107Present && jcacheImplPresent) {
			result.add(PROXY_JCACHE_CONFIGURATION_CLASS);
		}
		return StringUtils.toStringArray(result);
	}
```

