# 循环依赖发生的时机

1. createBeanInstance实例化
2. populateBean 填充属性
3. InitializeBean 初始化

在1,2过程中会发生

# 如何解决

Spring 为了解决单例的循环依赖问题，使用了 **三级缓存**

```java
/** 一级缓存：用于存放完全初始化好的 bean **/
private final Map<String, Object> singletonObjects = new ConcurrentHashMap<String, Object>(256);

/** 二级缓存：存放原始的 bean 对象（尚未填充属性），用于解决循环依赖 */
private final Map<String, Object> earlySingletonObjects = new HashMap<String, Object>(16);

/** 三级级缓存：存放 bean 工厂对象，用于解决循环依赖 */
private final Map<String, ObjectFactory<?>> singletonFactories = new HashMap<String, ObjectFactory<?>>(16);
```

过程

- A 创建过程中需要 B，于是 **A 将自己放到三级缓里面** ，去实例化 B
- B 实例化的时候发现需要 A，于是 B 先查一级缓存，没有，再查二级缓存，还是没有，再查三级缓存，找到了！
- **然后把三级缓存里面的这个 A 放到二级缓存里面，并删除三级缓存里面的 A**
- B 顺利初始化完毕，**将自己放到一级缓存里面**（此时B里面的A依然是创建中状态）
- 然后回来接着创建 A，此时 B 已经创建结束，直接从一级缓存里面拿到 B ，然后完成创建，**并将自己放到一级缓存里面**



# 源码分析

```java
protected Object getSingleton(String beanName, boolean allowEarlyReference) {
   // Quick check for existing instance without full singleton lock
   Object singletonObject = this.singletonObjects.get(beanName); //从一级缓存中取对象
   if (singletonObject == null && isSingletonCurrentlyInCreation(beanName)) { //没有取到，且当前对象正在创建中
      singletonObject = this.earlySingletonObjects.get(beanName); //从二级缓存取
      if (singletonObject == null && allowEarlyReference) { //从二级缓存没取到，且需要早期暴露
         synchronized (this.singletonObjects) {
            // Consistent creation of early reference within full singleton lock
            singletonObject = this.singletonObjects.get(beanName);
            if (singletonObject == null) {
               singletonObject = this.earlySingletonObjects.get(beanName);
               if (singletonObject == null) { //则从三级缓存中取 ObjectFactory 获取实例早期对象，并从三级缓存转移到二级缓存
                  ObjectFactory<?> singletonFactory = this.singletonFactories.get(beanName);
                  if (singletonFactory != null) {
                     singletonObject = singletonFactory.getObject();
                     this.earlySingletonObjects.put(beanName, singletonObject);
                     this.singletonFactories.remove(beanName);
                  }
               }
            }
         }
      }
   }
   return singletonObject;
}
```



# 总结

## 为什么要设计三级缓存

一般来说，二级缓存就够用了，但是 Spring 提供了 *InstantiationAwareBeanPostProcessor* 与 *SmartInstantiationAwareBeanPostProcessor* 后处理器

用于提前初始化，所以在 循环依赖过程中，如果提早暴露 引用给 其他人，则 需要对 先将 未 调用过*InstantiationAwareBeanPostProcessor*  的 bean放入 三缓

调用过 *InstantiationAwareBeanPostProcessor*  的 bean 但正在创建中的 放入 二缓，创建完成的放入一缓
