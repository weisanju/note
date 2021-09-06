# callbacks简介

这里的callback可以认为是cglib用于生成字节码的实现手段，cglib一共实现了6种callback，用于对代理类目标进行不同手段的代理

分别为：

- FixedValue
- InvocationHandler
- LazyLoader
- MethodInterceptor
- Dispatcher
- NoOp
- ProxyRefDispatcher





# Dispatcher

实现Dispatcher接口，要求实现loadObject方法，返回期望的代理类。

值的一提的是，loadobject方法在每次调用被拦截方法的时候都会被调用一次

手动返回指定的被代理类的，每次调用都会返回一个新的



```java
public final void methodForDispatcher() {
    Dispatcher var10000 = this.CGLIB$CALLBACK_3;
    if (var10000 == null) {
        CGLIB$BIND_CALLBACKS(this);
        var10000 = this.CGLIB$CALLBACK_3;
    }
	//每次都调用一次loadObject，获取对象，并调用对象的相应方法
    //这样的实现，相当于loadObject可以很灵活的返回相应的实现类或者子类
    ((CallbackBean)var10000.loadObject()).methodForDispatcher();
}
```



# LazyLoader

1. 与Dispatcher 类似，但是只会返回一次代理类实例
2. 懒加载 

```java
public final void select_with_lazyLoader() {
    ((MyDao)this.CGLIB$LOAD_PRIVATE_6()).select_with_lazyLoader();
}
private final synchronized Object CGLIB$LOAD_PRIVATE_6() {
    Object var10000 = this.CGLIB$LAZY_LOADER_6;
    if (var10000 == null) {
        LazyLoader var10001 = this.CGLIB$CALLBACK_6;
        if (var10001 == null) {
            CGLIB$BIND_CALLBACKS(this);
            var10001 = this.CGLIB$CALLBACK_6;
        }
		//如果为空才调用代理类
        var10000 = this.CGLIB$LAZY_LOADER_6 = var10001.loadObject();
    }

    return var10000;
}
```



# ProxyRefDispatcher

与Dispatcher 类似，每次调用都会传入代理类的引用进来

```java
public final void select_with_proxyRef() {
    ProxyRefDispatcher var10000 = this.CGLIB$CALLBACK_7;
    if (var10000 == null) {
        CGLIB$BIND_CALLBACKS(this);
        var10000 = this.CGLIB$CALLBACK_7;
    }

    ((MyDao)var10000.loadObject(this)).select_with_proxyRef();
}
```



# FixedValue

1. 该callback同样要求实现一个loadobject方法
2. 每次调用方法时直接返回该值，并强制转换

```java
public final String select_with_fixedValue() {
    FixedValue var10000 = this.CGLIB$CALLBACK_3;
    if (var10000 == null) {
        CGLIB$BIND_CALLBACKS(this);
        var10000 = this.CGLIB$CALLBACK_3;
    }

    return (String)var10000.loadObject();
}
```



# Noop

1. 没有重写方法
2. 直接调用父类的方法

# InvocationHandler

1. 直接调用 InvocationHandler

```java
    public final Object select_with_invocationHandler(String var1) {
        try {
            InvocationHandler var10000 = this.CGLIB$CALLBACK_5;
            if (var10000 == null) {
                CGLIB$BIND_CALLBACKS(this);
                var10000 = this.CGLIB$CALLBACK_5;
            }

            return var10000.invoke(this, CGLIB$select_with_invocationHandler$5, new Object[]{var1});
        } catch (Error | RuntimeException var2) {
            throw var2;
        } catch (Throwable var3) {
            throw new UndeclaredThrowableException(var3);
        }
    }
```

# MethodInterceptor

使用 *fastclass* 机制 调用父类方法

1. var1是 代理对象引用
2. var2是 调用方法
3. var3是 参数对象
4. var4是 fastClass机制的类调用

```java
    public final void select() {
        MethodInterceptor var10000 = this.CGLIB$CALLBACK_0;
        if (var10000 == null) {
            CGLIB$BIND_CALLBACKS(this);
            var10000 = this.CGLIB$CALLBACK_0;
        }

        if (var10000 != null) {
            var10000.intercept(this, CGLIB$select$1$Method, CGLIB$emptyArgs, CGLIB$select$1$Proxy);
        } else {
            super.select();
        }
    }
```

