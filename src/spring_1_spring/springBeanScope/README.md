# 简述

1. **scope** 用来限定 容器中bean对象的存活时间
2. 即对象在进入相应 scope时 容器会自动装配这些对象、在容器不再处于该scope后，容器通常会销毁这些对象



# 预定义的Scope

## singleton

1. singleton是容器默认的scope

2. 在Spring的IoC容器中只存在一个实例

#### singleton的bean具有的特性

- **对象实例数量**：singleton类型的bean定义，在一个容器中只存在一个共享实例，所有对该类型bean的依赖都引用这一单一实例
- **对象存活时间**：singleton类型bean定义，从容器启动，到它第一次被请求而实例化开始，只要容器不销毁或退出，该类型的单一实例就会一直存活



## prototype

**每次得到的对象都是重新装配的**

## request、session、global session

这三个scope类型是Spring2.0之后新增加的，它们不像上面两个那么通用，它们只适用于Web应用程序，通常是与XmlWebApplicationContext共同使用

### request

1. 在Spring容器中，即XmlWebApplicationContext会为每个HTTP请求创建一个全新的Request-Processor对象供当前请求使用，当请求结束后，该对象实生命周期就结束

2. 当同时有10个HTTP请求进来的时候，容器会分别针对这10个请求返回10个全新的RequestProcessor对象实例，且它们之间互不干扰。

### session

放到session中的最普遍的信息就是用户登录信息，Spring容器会为每个独立的session创建属于它们自己全新的UserPreferences对象实例。与request相比，除了拥有session scope的bean比request scope的bean可能更长的存活时间，其他没什么差别

### global session

global session只有应用在基于portlet的Web应用程序中才有意义，它映射到portlet的global范围的session。

如果在普通的基于servlet的Web应用中使了用这个类型的scope，容器会将其作为普通的session类型的scope来对待

##### 什么是portlet

Portlets是一种Web组件－就像servlets－是专为将合成页面里的内容聚集在一起而设计的。通常请求一个portal页面会引发多个portlets被调用。每个portlet都会生成标记段，并与别的portlets生成的标记段组合在一起嵌入到portal页面的标记内。

# 源码分析

## Scope接口

**GET接口**

> 从底层依赖 查找对象，如果查找不到则 使用 Spring自动装配Bean

```java
Object get(String name, ObjectFactory<?> objectFactory);
```

**REMOVE接口**

1. 从底层作用域中删除具有给定名称的对象。
2. 如果没有找到对象，则返回 null，否则返回移除的对象。
3. 实现者 还应  移除指定对象的注册销毁回调（如果有）。如果没有，它不需要执行注册的销毁回调，因为对象将被调用者销毁（如果合适）。
4. 这是一个可选的 实现操作 也可以抛出  **UnsupportedOperationException**


```java
Object remove(String name);
```

**注册回调接口**

1. 注册一个回调，以在销毁指定的 scope 内的对象时执行

2. 注意：这是一个可选操作。此方法只会为配置了 实际销毁动作的 作用域bean调用

   ```
   DisposableBean, destroy-method, DestructionAwareBeanPostProcessor
   ```

3. 实现应该尽最大努力在适当的时间执行给定的回调，如果底层运行时环境根本不支持这样的回调，则必须忽略回调并记录相应的警告



```java
//bean的名称、销毁要执行的回调
void registerDestructionCallback(String name, Runnable callback);
```



**解析给定key的上下文对象**（如果有）。

例如 key “request” 的 HttpServletRequest 对象。

```java
Object resolveContextualObject(String key);
```



返回当前基础**scope**的对话 ID（如果有）。

1. 对话 ID 的确切含义取决于底层存储机制。
2. 在session scope 对象的情况下，对话ID 通常等于（或派生自）session ID；
3. 对于位于整个会话中的自定义对话，指定 当前对话的ID 将是合适的。
4. 如果底层存储机制没有明显的此类 ID 候选者，则 返回 null 是完全有效的。

```java
String getConversationId();
```

## AbstractRequestAttributesScope

1. scope抽象实现类，基于Web的 Request

2. **RequestScope** 与 **SessionScope ** 都是依赖于 **RequestAttributes** 实现的Scope

   

## Scope是如何生效的





