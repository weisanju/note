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



## AsyncAnnotationAdvisor

完成 PointCut的构建，与代理逻辑的异步核心Advisor类的构建

1. PointCut 由 *AnnotationMatchingPointcut* 完成对注解的 匹配
2. 多个PointCut使用 *ComposablePointcut* 支持多注解组合
3. Advice使用 *AnnotationAsyncExecutionInterceptor* 完成核心代理逻辑

```java
	public AsyncAnnotationAdvisor(
			@Nullable Supplier<Executor> executor, @Nullable Supplier<AsyncUncaughtExceptionHandler> exceptionHandler) {

		Set<Class<? extends Annotation>> asyncAnnotationTypes = new LinkedHashSet<>(2);
		asyncAnnotationTypes.add(Async.class);
		try {
			asyncAnnotationTypes.add((Class<? extends Annotation>)
					ClassUtils.forName("javax.ejb.Asynchronous", AsyncAnnotationAdvisor.class.getClassLoader()));
		}
		catch (ClassNotFoundException ex) {
			// If EJB 3.1 API not present, simply ignore.
		}
		this.advice = buildAdvice(executor, exceptionHandler);
		this.pointcut = buildPointcut(asyncAnnotationTypes);
	}

```

**构建pointcut**

```java
protected Pointcut buildPointcut(Set<Class<? extends Annotation>> asyncAnnotationTypes) {
	ComposablePointcut result = null;
	for (Class<? extends Annotation> asyncAnnotationType : asyncAnnotationTypes) {
		Pointcut cpc = new AnnotationMatchingPointcut(asyncAnnotationType, true);
		Pointcut mpc = new AnnotationMatchingPointcut(null, asyncAnnotationType, true);
		if (result == null) {
			result = new ComposablePointcut(cpc);
		}
		else {
			result.union(cpc);
		}
		result = result.union(mpc);
	}
	return (result != null ? result : Pointcut.TRUE);
}
```

**构建Advice**

```java
protected Advice buildAdvice(
      @Nullable Supplier<Executor> executor, @Nullable Supplier<AsyncUncaughtExceptionHandler> exceptionHandler) {

   AnnotationAsyncExecutionInterceptor interceptor = new AnnotationAsyncExecutionInterceptor(null);
   interceptor.configure(executor, exceptionHandler);
   return interceptor;
}
```



### AsyncExecutionAspectSupport

1. AnnotationAsyncExecutionInterceptor的基类

2. 帮助 *AnnotationAsyncExecutionInterceptor* 进行

   1. 线程池查找：默认线程池查找、缓存线程池对象
   2. 任务提交
   3. 错误处理

   获取线程池名称的方法没有线程，因为不通子类的获取线程池的名称 途径可能不一致

   1. 基于注解的线程池 是通过 注解上获取线程池名称的

**获取默认线程池逻辑**

```java
	protected Executor getDefaultExecutor(@Nullable BeanFactory beanFactory) {
		if (beanFactory != null) {
			try {
				// Search for TaskExecutor bean... not plain Executor since that would
				// match with ScheduledExecutorService as well, which is unusable for
				// our purposes here. TaskExecutor is more clearly designed for it.
				return beanFactory.getBean(TaskExecutor.class);
			}
			catch (NoUniqueBeanDefinitionException ex) {
				logger.debug("Could not find unique TaskExecutor bean", ex);
				try {
					return beanFactory.getBean(DEFAULT_TASK_EXECUTOR_BEAN_NAME, Executor.class);
				}
				catch (NoSuchBeanDefinitionException ex2) {
					if (logger.isInfoEnabled()) {
						logger.info("More than one TaskExecutor bean found within the context, and none is named " +
								"'taskExecutor'. Mark one of them as primary or name it 'taskExecutor' (possibly " +
								"as an alias) in order to use it for async processing: " + ex.getBeanNamesFound());
					}
				}
			}
			catch (NoSuchBeanDefinitionException ex) {
				logger.debug("Could not find default TaskExecutor bean", ex);
				try {
					return beanFactory.getBean(DEFAULT_TASK_EXECUTOR_BEAN_NAME, Executor.class);
				}
				catch (NoSuchBeanDefinitionException ex2) {
					logger.info("No task executor bean found for async processing: " +
							"no bean of type TaskExecutor and no bean named 'taskExecutor' either");
				}
				// Giving up -> either using local default executor or none at all...
			}
		}
		return null;
	}
```

**任务提交逻辑**

```java
protected Object doSubmit(Callable<Object> task, AsyncTaskExecutor executor, Class<?> returnType) {
   if (CompletableFuture.class.isAssignableFrom(returnType)) {
      return CompletableFuture.supplyAsync(() -> {
         try {
            return task.call();
         }
         catch (Throwable ex) {
            throw new CompletionException(ex);
         }
      }, executor);
   }
   else if (ListenableFuture.class.isAssignableFrom(returnType)) {
      return ((AsyncListenableTaskExecutor) executor).submitListenable(task);
   }
   else if (Future.class.isAssignableFrom(returnType)) {
      return executor.submit(task);
   }
   else {
      executor.submit(task);
      return null;
   }
}
```

**错误处理逻辑**

```java
protected void handleError(Throwable ex, Method method, Object... params) throws Exception {
   if (Future.class.isAssignableFrom(method.getReturnType())) {
      ReflectionUtils.rethrowException(ex);
   }
   else {
      // Could not transmit the exception to the caller with default executor
      try {
         this.exceptionHandler.obtain().handleUncaughtException(ex, method, params);
      }
      catch (Throwable ex2) {
         logger.warn("Exception handler for async method '" + method.toGenericString() +
               "' threw unexpected exception itself", ex2);
      }
   }
}
```

**线程池对象缓存**

```java
protected AsyncTaskExecutor determineAsyncExecutor(Method method) {
   AsyncTaskExecutor executor = this.executors.get(method);
   if (executor == null) {
      Executor targetExecutor;
      String qualifier = getExecutorQualifier(method);
      if (StringUtils.hasLength(qualifier)) {
         targetExecutor = findQualifiedExecutor(this.beanFactory, qualifier);
      }
      else {
         targetExecutor = this.defaultExecutor.get();
      }
      if (targetExecutor == null) {
         return null;
      }
      executor = (targetExecutor instanceof AsyncListenableTaskExecutor ?
            (AsyncListenableTaskExecutor) targetExecutor : new TaskExecutorAdapter(targetExecutor));
      this.executors.put(method, executor);
   }
   return executor;
}
```



### AnnotationAsyncExecutionInterceptor

继承 *AsyncExecutionAspectSupport*

继承 *MethodInterceptor* 完成方法的拦截

**该类主要完成拦截逻辑**

**拦截逻辑**

```java
public Object invoke(final MethodInvocation invocation) throws Throwable {
   Class<?> targetClass = (invocation.getThis() != null ? AopUtils.getTargetClass(invocation.getThis()) : null);
   Method specificMethod = ClassUtils.getMostSpecificMethod(invocation.getMethod(), targetClass);
   final Method userDeclaredMethod = BridgeMethodResolver.findBridgedMethod(specificMethod);
	//在运行时：确认要使用的线程池
   AsyncTaskExecutor executor = determineAsyncExecutor(userDeclaredMethod);
   if (executor == null) {
      throw new IllegalStateException(
            "No executor specified and no default executor set on AsyncExecutionInterceptor either");
   }

    //执行异步任务
   Callable<Object> task = () -> {
      try {
         Object result = invocation.proceed();
         if (result instanceof Future) {
            return ((Future<?>) result).get();
         }
      }
      catch (ExecutionException ex) {
         handleError(ex.getCause(), userDeclaredMethod, invocation.getArguments());
      }
      catch (Throwable ex) {
         handleError(ex, userDeclaredMethod, invocation.getArguments());
      }
      return null;
   };
	//提交，根据不同的返回值，任务提交也会返回不同的返回值
   return doSubmit(task, executor, invocation.getMethod().getReturnType());
}
```

### AnnotationAsyncExecutionInterceptor

继承 *AsyncExecutionInterceptor*

该类主要完成线程池名称的解析

```java
protected String getExecutorQualifier(Method method) {
	// Maintainer's note: changes made here should also be made in
	// AnnotationAsyncExecutionAspect#getExecutorQualifier
	Async async = AnnotatedElementUtils.findMergedAnnotation(method, Async.class);
	if (async == null) {
		async = AnnotatedElementUtils.findMergedAnnotation(method.getDeclaringClass(), Async.class);
	}
	return (async != null ? async.value() : null);
}
```

