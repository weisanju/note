# 前言

Spring对各种实例对象的抽象







# BeanDefinition

bean定义



# AnnotatedTypeMetadata

以不一定需要类加载的形式定义对特定类型（类或方法）注解的访问。



# MergedAnnotation

从合并的注解集返回中 返回单个合并的注解。在属性值可能从不同的源值中"合并"的注释上呈现视图。



# MergedAnnotations

提供对注解合并 集合访问



# AliasRegistry

别名注册

给定一个名称，并将相关的别名与之绑定



# BeanDefinitionRegistry

1. 注册bean定义

2. 给定bean名称，并绑定与之相关的bean定义
3. 一般实现者是：DefaultListableBeanFactory and GenericApplicationContext





# BeanMetadataElement

携带配置源对象的 bean元数据 元素 实现



# BeanDefinitionHolder

持有 bean定义，bean名，bean别名的引用



# BeanNameGenerator

根据bean定义 bean定义注册器 生成名称



# SingletonBeanRegistry

共享bean实例的注册

由BeanFactory 实现 主要用来 以统一的方式暴露他们的单例管理工具

Register the given existing object as singleton in the bean registry, under the given bean name.
The given instance is supposed to be fully initialized; 

the registry will not perform any initialization callbacks (in particular, it won't call InitializingBean's afterPropertiesSet method). 

The given instance will not receive any destruction callbacks (like DisposableBean's destroy method) either.

When running within a full BeanFactory: Register a bean definition instead of an existing instance if your bean is supposed to receive initialization and/or destruction callbacks.
Typically invoked during registry configuration, but can also be used for runtime registration of singletons. As a consequence, a registry implementation should synchronize singleton access; it will have to do this anyway if it supports a BeanFactory's lazy initialization of singletons.
Params:
beanName – the name of the bean
singletonObject – the existing singleton object
