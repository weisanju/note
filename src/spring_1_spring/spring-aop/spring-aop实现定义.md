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



