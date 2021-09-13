# `@Import`注解

* `@Import` 是`Spring`基于 Java 注解配置的主要组成部分

* `@Import`注解提供了`@Bean`注解的功能，同时还有原来`Spring`基于 xml 配置文件里的 `<import>`  标签组织多个分散的xml文件的功能，当然在这里是组织多个分散的`@Configuration`的类



# `@Import`注解的功能

## 引入其他的`@Configuration`

```java
package com.test
//载入ConfigB类：且ConfigB优先于  ConfigA
@Import(ConfigB.class)
@Configuration
class ConfigA {
    @Bean
    @ConditionalOnMissingBean
    public ServiceInterface getServiceA() {
        return new ServiceA();
    }
}

@Configuration
class ConfigB {
    @Bean
    @ConditionalOnMissingBean
    public ServiceInterface getServiceB() {
        return new ServiceB();
    }
}
```

## 直接初始化其他类的`Bean`

`@Import`可以直接指定实体类，加载这个类定义到`context`中

```java
//就会生成ServiceB的Bean到容器上下文中
@Import(ServiceB.class)
@Configuration
class ConfigA {
    @Bean
    @ConditionalOnMissingBean
    public ServiceInterface getServiceA() {
        return new ServiceA();
    }
}
```

## 个性化加载

指定实现`ImportSelector`(以及`DefferredServiceImportSelector`)的类，用于个性化加载

Enable就是利用如此 特性实现 的

```javascript
package com.test;

@Retention(RetentionPolicy.RUNTIME)
@Documented
@Target(ElementType.TYPE)
@Import(ServiceImportSelector.class)
@interface EnableService {
    String name();
}

class ServiceImportSelector implements ImportSelector {
    @Override
    public String[] selectImports(AnnotationMetadata importingClassMetadata) {
        //这里的importingClassMetadata针对的是使用@EnableService的非注解类
        //因为`AnnotationMetadata`是`Import`注解所在的类属性，如果所在类是注解类，则延伸至应用这个注解类的非注解类为止
        Map<String , Object> map = importingClassMetadata.getAnnotationAttributes(EnableService.class.getName(), true);
        String name = (String) map.get("name");
        if (Objects.equals(name, "B")) {
            return new String[]{"com.test.ConfigB"};
        }
        return new String[0];
    }
}

package com.test;
@EnableService(name = "B")
@Configuration
class ConfigA {
    @Bean
    @ConditionalOnMissingBean
    public ServiceInterface getServiceA() {
        return new ServiceA();
    }
}
```

*DeferredImportSelector*  与selector的区别就是 是否延迟于当前注解类加载





## 个性化定制bean

```java
package com.test;

@Retention(RetentionPolicy.RUNTIME)
@Documented
@Target(ElementType.TYPE)
@Import(ServiceImportBeanDefinitionRegistrar.class)
@interface EnableService {
    String name();
}

class ServiceImportBeanDefinitionRegistrar implements ImportBeanDefinitionRegistrar {
    @Override
    public void registerBeanDefinitions(AnnotationMetadata importingClassMetadata, BeanDefinitionRegistry registry) {
        Map<String, Object> map = importingClassMetadata.getAnnotationAttributes(EnableService.class.getName(), true);
        String name = (String) map.get("name");
        BeanDefinitionBuilder beanDefinitionBuilder = BeanDefinitionBuilder.rootBeanDefinition(ServiceC.class)
                //增加构造参数
                .addConstructorArgValue(name);
        //注册Bean
        registry.registerBeanDefinition("serviceC", beanDefinitionBuilder.getBeanDefinition());
    }
}
```

# 源码分析

