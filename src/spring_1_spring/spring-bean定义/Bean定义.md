# BeanDefinition

BeanDefinition 描述了一个 bean 实例，它具有属性值、构造函数参数值以及由具体实现提供的更多信息。

这只是一个最小的接口：主要目的是允许 BeanFactoryPostProcessor 内省和修改属性值和其他 bean 元数据。



## parentName

父 *definition* 



## beanClassName

 bean 类名称

类名可以在 bean factory 后处理期间修改，通常用它的解析变体替换原始类名



## *scope*

bean的作用域



## lazyInit

懒加载



## dependsOn

依赖bean名称

## autowireCandidate

设置此 bean 是否是自动装配到其他 bean 的候选者。

此标志旨在仅影响基于类型的自动装配

它不会影响按名称的显式引用，即使指定的 bean 没有被标记为自动装配候选者，也会被解析

因此，如果名称匹配，按名称自动装配仍然会注入一个 bean



## primary

设置此 bean 是否是主要的自动装配候选者。

如果这个值对于多个匹配候选中的一个 bean 正好是真的，它将作为一个 *tie-breaker.*



## factoryBeanName

指定要使用的工厂 bean（如果有）



## factoryMethodName

此方法将使用构造函数参数调用

该方法将在指定的工厂 bean（如果有）上调用，或者作为本地 bean 类上的静态方法调用。



## ConstructorArgumentValues

返回的实例可以在 bean factory 后处理期间进行修改



## MutablePropertyValues

返回要应用于 bean 的新实例的属性值

返回的实例可以在 bean factory 后处理期间进行修改



## initMethodName

## destroyMethodName

## role

BeanDefinition 的角色提示。角色提示为框架和工具提供了特定 BeanDefinition 的角色和重要性的指示。

## description

设置此 bean 定义的人类可读描述。

## ResolvableType

根据 bean 类或其他特定元数据，返回此 bean 定义的可解析类型。

这通常在运行时合并的 bean 定义上完全解决，但不一定在配置时定义实例上解决。

## singleton

单例

## abstract

抽象

## ResourceDescription

返回此 bean 定义来自的资源的描述（为了在出现错误时显示上下文）。

## OriginatingBeanDefinition

返回原始 BeanDefinition，如果没有则返回 null。允许检索装饰的 bean 定义（如果有）。请注意，此方法返回直接发起者。遍历创建者链以找到用户定义的原始 BeanDefinition。



