# org.aopalliance的拦截体系

1. 该包是AOP组织下的公用包，用于AOP中方法增强和调用。相当于一个jsr标准
2. 只有接口和异常，只有接口和异常





# 包结构图

![](../../images/aopalliance-structure.png)



# 解析

AOP主要有两大概念

* **Advice**

* **JointPoint**

*Advice* 定义了如何 切面，没有定义相关行为

*JointPoint* 定义了运行时 具体切面的位置



1. 在实际编程中 *Advice* 具象化为  *Interceptor* 拦截器 分为 *ConstrutorInterceptor* 与 *MethodIntercceptor*都是基于方法的拦截
2. 而*JointPoint* 具象为 *Invocation*，即每次方法的拦截，就会相应拦截的实例 ，这里将其抽象为 *invocation* 调用，调用也区分 构造函数调用与 普通方法调用
