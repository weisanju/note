# 创建代理

## 核心源代码

```java
public Object getProxy(@Nullable ClassLoader classLoader) {

   try {
      Class<?> rootClass = this.advised.getTargetClass();
      Assert.state(rootClass != null, "Target class must be available for creating a CGLIB proxy");

      Class<?> proxySuperClass = rootClass;
      //如果是代理类
      if (rootClass.getName().contains(ClassUtils.CGLIB_CLASS_SEPARATOR)) {
          //获取父类
         proxySuperClass = rootClass.getSuperclass();
         Class<?>[] additionalInterfaces = rootClass.getInterfaces();
         for (Class<?> additionalInterface : additionalInterfaces) {
            this.advised.addInterface(additionalInterface);
         }
      }

      // Validate the class, writing log messages as necessary.
      validateClassIfNecessary(proxySuperClass, classLoader);

      // Configure CGLIB Enhancer...
      Enhancer enhancer = createEnhancer();
      if (classLoader != null) {
         enhancer.setClassLoader(classLoader);
         if (classLoader instanceof SmartClassLoader &&
               ((SmartClassLoader) classLoader).isClassReloadable(proxySuperClass)) {
            enhancer.setUseCache(false);
         }
      }
      //设置代理目标类
      enhancer.setSuperclass(proxySuperClass);
      //设置接口
      enhancer.setInterfaces(AopProxyUtils.completeProxiedInterfaces(this.advised));
      enhancer.setNamingPolicy(SpringNamingPolicy.INSTANCE);
      enhancer.setStrategy(new ClassLoaderAwareGeneratorStrategy(classLoader));
       //获取回调
      Callback[] callbacks = getCallbacks(rootClass);
      Class<?>[] types = new Class<?>[callbacks.length];
      //缓存回调类
      for (int x = 0; x < types.length; x++) {
         types[x] = callbacks[x].getClass();
      }
       //设置 callbackFilter
      // fixedInterceptorMap only populated at this point, after getCallbacks call above
      enhancer.setCallbackFilter(new ProxyCallbackFilter(
            this.advised.getConfigurationOnlyCopy(), this.fixedInterceptorMap, this.fixedInterceptorOffset));
       //设置回调的类型
      enhancer.setCallbackTypes(types);
       
      // Generate the proxy class and create a proxy instance.
      return createProxyClassAndInstance(enhancer, callbacks);
   }
   catch (CodeGenerationException | IllegalArgumentException ex) {
      throw new AopConfigException("Could not generate CGLIB subclass of " + this.advised.getTargetClass() +
            ": Common causes of this problem include using a final class or a non-visible class",
            ex);
   }
   catch (Throwable ex) {
      // TargetSource.getTarget() failed
      throw new AopConfigException("Unexpected AOP exception", ex);
   }
}
```

## 获取回调

```java
private Callback[] getCallbacks(Class<?> rootClass) throws Exception {
   // Parameters used for optimization choices...
    //是否暴露代理类
   boolean exposeProxy = this.advised.isExposeProxy();
    //是否冻结配置
   boolean isFrozen = this.advised.isFrozen();
    //对象是否是可变的
   boolean isStatic = this.advised.getTargetSource().isStatic();

    
   // Choose an "aop" interceptor (used for AOP calls). aop链式调用
   Callback aopInterceptor = new DynamicAdvisedInterceptor(this.advised);

   // Choose a "straight to target" interceptor. (used for calls that are
   // unadvised but can return this). May be required to expose the proxy.
   Callback targetInterceptor; //直接调用目标方法
   if (exposeProxy) {
      targetInterceptor = (isStatic ?
            new StaticUnadvisedExposedInterceptor(this.advised.getTargetSource().getTarget()) :
            new DynamicUnadvisedExposedInterceptor(this.advised.getTargetSource()));
   }
   else {
      targetInterceptor = (isStatic ?
            new StaticUnadvisedInterceptor(this.advised.getTargetSource().getTarget()) :
            new DynamicUnadvisedInterceptor(this.advised.getTargetSource()));
   }

   // Choose a "direct to target" dispatcher (used for
   // unadvised calls to static targets that cannot return this). 直接调用实际 Target的方法
   Callback targetDispatcher = (isStatic ?
         new StaticDispatcher(this.advised.getTargetSource().getTarget()) : new SerializableNoOp());

   Callback[] mainCallbacks = new Callback[] {
         aopInterceptor,  // for normal advice  普通AOP调用  0
         targetInterceptor,  // invoke target without considering advice, if optimized  调用  1
         new SerializableNoOp(),  // no override for methods mapped to this 调用父类的方法   2
         targetDispatcher,//直接 调用target的方法  3
       this.advisedDispatcher, //调用 advised对象中的方法 4
         new EqualsInterceptor(this.advised),  //equals  5
         new HashCodeInterceptor(this.advised) //hashcode  6
   };

   Callback[] callbacks;

   // If the target is a static one and the advice chain is frozen,
   // then we can make some optimizations by sending the AOP calls
   // direct to the target using the fixed chain for that method.
   if (isStatic && isFrozen) {
       //获取所有 公共方法，每个方法创建FixedChainStaticTargetInterceptor拦截器
      Method[] methods = rootClass.getMethods();
      Callback[] fixedCallbacks = new Callback[methods.length];
      this.fixedInterceptorMap = CollectionUtils.newHashMap(methods.length);

      // TODO: small memory optimization here (can skip creation for methods with no advice)
      for (int x = 0; x < methods.length; x++) {
         Method method = methods[x];
         List<Object> chain = this.advised.getInterceptorsAndDynamicInterceptionAdvice(method, rootClass);
         fixedCallbacks[x] = new FixedChainStaticTargetInterceptor(
               chain, this.advised.getTargetSource().getTarget(), this.advised.getTargetClass());
         this.fixedInterceptorMap.put(method, x);
      }

      // Now copy both the callbacks from mainCallbacks
      // and fixedCallbacks into the callbacks array.
      callbacks = new Callback[mainCallbacks.length + fixedCallbacks.length];
      System.arraycopy(mainCallbacks, 0, callbacks, 0, mainCallbacks.length);
      System.arraycopy(fixedCallbacks, 0, callbacks, mainCallbacks.length, fixedCallbacks.length);
      this.fixedInterceptorOffset = mainCallbacks.length;
   }
   else {
      callbacks = mainCallbacks;
   }
   return callbacks;
}
```



## 确定每个方法的拦截器

```java
//org.springframework.aop.framework.CglibAopProxy.ProxyCallbackFilter#accept
		public int accept(Method method) {
            //finanl方法 直接 调用父类的
			if (AopUtils.isFinalizeMethod(method)) {
				logger.trace("Found finalize() method - using NO_OVERRIDE");
				return NO_OVERRIDE;
			}
            //用户透明的、接口、而且是在 Advised类申明的方法
			if (!this.advised.isOpaque() && method.getDeclaringClass().isInterface() &&
					method.getDeclaringClass().isAssignableFrom(Advised.class)) {
				if (logger.isTraceEnabled()) {
					logger.trace("Method is declared on Advised interface: " + method);
				}
                //调用 Advised类中的方法
				return DISPATCH_ADVISED;
			}
			// We must always proxy equals, to direct calls to this.调用 Equals方法
			if (AopUtils.isEqualsMethod(method)) {
				if (logger.isTraceEnabled()) {
					logger.trace("Found 'equals' method: " + method);
				}
				return INVOKE_EQUALS;
			}
			// We must always calculate hashCode based on the proxy.  基于proxy计算hashcode
			if (AopUtils.isHashCodeMethod(method)) {
				if (logger.isTraceEnabled()) {
					logger.trace("Found 'hashCode' method: " + method);
				}
				return INVOKE_HASHCODE;
			}
            //非增强方法的处理
            
			Class<?> targetClass = this.advised.getTargetClass();
			// Proxy is not yet available, but that shouldn't matter.
            //获取调用链条
			List<?> chain = this.advised.getInterceptorsAndDynamicInterceptionAdvice(method, targetClass);
			boolean haveAdvice = !chain.isEmpty();
			boolean exposeProxy = this.advised.isExposeProxy();
			boolean isStatic = this.advised.getTargetSource().isStatic();
			boolean isFrozen = this.advised.isFrozen();
            //存在调用链、或者非冻结的配置使用 aopInterceptor(该配置在调用时  实时匹配调用)
			if (haveAdvice || !isFrozen) {
				// If exposing the proxy, then AOP_PROXY must be used.
				if (exposeProxy) { //如果需要暴露代理 
					if (logger.isTraceEnabled()) {
						logger.trace("Must expose proxy on advised method: " + method);
					}
                    //
					return AOP_PROXY;
				}
				// Check to see if we have fixed interceptor to serve this method.
				// Else use the AOP_PROXY. 如果是 静态的 、且 配置 冻结 了 而且包含了 此method
				if (isStatic && isFrozen && this.fixedInterceptorMap.containsKey(method)) {
					if (logger.isTraceEnabled()) {
						logger.trace("Method has advice and optimizations are enabled: " + method);
					}  //则调用该出索引的Method
					// We know that we are optimizing so we can use the FixedStaticChainInterceptors.
					int index = this.fixedInterceptorMap.get(method);
					return (index + this.fixedInterceptorOffset);
				}
				else {
					if (logger.isTraceEnabled()) {
						logger.trace("Unable to apply any optimizations to advised method: " + method);
					}
                    //否则 还是使用 动态
					return AOP_PROXY;
				}
			}
			else {
				// See if the return type of the method is outside the class hierarchy of the target type.
                // 查看方法的返回类型是否在目标类型的类层次结构之外。
				// 如果在之外，则不需要对返回值进行类型转换，直接静态dispatcher.
				// 如果 代理被暴露, 必须使用拦截器. 
                // 如果目标不是静态的，那么我们不能使用调度器，因为 目标需要在调用后显式释放。
				if (exposeProxy || !isStatic) {
					return INVOKE_TARGET;
				}
				Class<?> returnType = method.getReturnType();
				if (targetClass != null && returnType.isAssignableFrom(targetClass)) {
					if (logger.isTraceEnabled()) {
						logger.trace("Method return type is assignable from target type and " +
								"may therefore return 'this' - using INVOKE_TARGET: " + method);
					}
					return INVOKE_TARGET;
				}
				else {
					if (logger.isTraceEnabled()) {
						logger.trace("Method return type ensures 'this' cannot be returned - " +
								"using DISPATCH_TARGET: " + method);
					}
					return DISPATCH_TARGET;
				}
			}
		}
```

# 运行逻辑

## final不覆盖

如果是final方法则 NO_OVERRIDE，对应于 *NoOp* 即：不覆盖方法

## 如果是Advised接口的方法

则直接转发到 this.advised对象上



## Equals HashCode 调用代理类的方法

## 如果没有Advice且 不需要暴露代理且是静态 而且不需要返回this

直接转发到 *target*的 方法上



## 如果没有Advice且 需要暴露代理或者非静态或者返回值为this

则调用 *INVOKE_TARGET*

## 存在 Advice,且（静态的、冻结配置了的、且存在拦截方法）

使用静态拦截器：在代理的时刻：已经确定好哪些拦截器

## 存在 Advice,且（需要暴露代理、或者非静态或者没有冻结配置、或者还不存在拦截方法）

使用动态拦截器：在调用时匹配拦截方法

**动态拦截逻辑**

```java
//DynamicAdvisedInterceptor#intercept
public Object intercept(Object proxy, Method method, Object[] args, MethodProxy methodProxy) throws Throwable {
   Object oldProxy = null;
   boolean setProxyContext = false;
   Object target = null;
    //获取目标类
   TargetSource targetSource = this.advised.getTargetSource();
   try {
       //暴露代理类
      if (this.advised.exposeProxy) {
         // Make invocation available if necessary.
         oldProxy = AopContext.setCurrentProxy(proxy);
         setProxyContext = true;
      }
      // Get as late as possible to minimize the time we "own" the target, in case it comes from a pool...
       //获取目标
      target = targetSource.getTarget();
       //获取目标class
      Class<?> targetClass = (target != null ? target.getClass() : null);
       //获取动态拦截器：从spring容器中获取所有Advisor对象并对 method,class一一匹配
      List<Object> chain = this.advised.getInterceptorsAndDynamicInterceptionAdvice(method, targetClass);
      Object retVal;
      // Check whether we only have one InvokerInterceptor: that is,
      // no real advice, but just reflective invocation of the target.
       //没有代理类，直接调用目标方法
      if (chain.isEmpty() && Modifier.isPublic(method.getModifiers())) {
         // We can skip creating a MethodInvocation: just invoke the target directly.
         // Note that the final invoker must be an InvokerInterceptor, so we know
         // it does nothing but a reflective operation on the target, and no hot
         // swapping or fancy proxying.
         Object[] argsToUse = AopProxyUtils.adaptArgumentsIfNecessary(method, args);
         retVal = methodProxy.invoke(target, argsToUse);
      }
      else {
          //生成责任链调用类，进行链式调用
         // We need to create a method invocation...
         retVal = new CglibMethodInvocation(proxy, target, method, args, targetClass, chain, methodProxy).proceed();
      }
       //处理返回值
      retVal = processReturnType(proxy, target, method, retVal);
      return retVal;
   }
   finally {
       //释放target
      if (target != null && !targetSource.isStatic()) {
         targetSource.releaseTarget(target);
      }
       //还原代理对象
      if (setProxyContext) {
         // Restore old proxy.
         AopContext.setCurrentProxy(oldProxy);
      }
   }
}
```

**链式调用**

```java
private static class CglibMethodInvocation extends ReflectiveMethodInvocation {

   @Nullable
   private final MethodProxy methodProxy;

   public CglibMethodInvocation(Object proxy, @Nullable Object target, Method method,
         Object[] arguments, @Nullable Class<?> targetClass,
         List<Object> interceptorsAndDynamicMethodMatchers, MethodProxy methodProxy) {

      super(proxy, target, method, arguments, targetClass, interceptorsAndDynamicMethodMatchers);

      // Only use method proxy for public methods not derived from java.lang.Object
      this.methodProxy = (Modifier.isPublic(method.getModifiers()) &&
            method.getDeclaringClass() != Object.class && !AopUtils.isEqualsMethod(method) &&
            !AopUtils.isHashCodeMethod(method) && !AopUtils.isToStringMethod(method) ?
            methodProxy : null);
   }

   @Override
   @Nullable
   public Object proceed() throws Throwable {
      try {
         return super.proceed();
      }
      catch (RuntimeException ex) {
         throw ex;
      }
      catch (Exception ex) {
         if (ReflectionUtils.declaresException(getMethod(), ex.getClass()) ||
               KotlinDetector.isKotlinType(getMethod().getDeclaringClass())) {
            // Propagate original exception if declared on the target method
            // (with callers expecting it). Always propagate it for Kotlin code
            // since checked exceptions do not have to be explicitly declared there.
            throw ex;
         }
         else {
            // Checked exception thrown in the interceptor but not declared on the
            // target method signature -> apply an UndeclaredThrowableException,
            // aligned with standard JDK dynamic proxy behavior.
            throw new UndeclaredThrowableException(ex);
         }
      }
   }

   /**
    * 直接目标方法调用：通过 方法名称确定调用哪个方法：有略微的性能提升
    * Gives a marginal performance improvement versus using reflection to
    * invoke the target when invoking public methods.
    */
   @Override
   protected Object invokeJoinpoint() throws Throwable {
       //跳过Object的方法
      if (this.methodProxy != null) {
         return this.methodProxy.invoke(this.target, this.arguments);
      }
      else {
		//object方法使用反射调用
         return super.invokeJoinpoint();
      }
   }
}
```

