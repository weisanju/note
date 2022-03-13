# JCache (JSR-107) Annotations

* 从4.1版开始，Spring的缓存抽象完全支持JCache标准注解：@CacheResult，@CachePut，@CacheRemove和@CacheRemoveAll以及@ CacheDefaults，@ CacheKey和@CacheValue

* 内部实现使用Spring的缓存抽象，并提供符合规范的默认CacheResolver和KeyGenerator实现
* 换句话说，如果您已经在使用Spring的缓存抽象，则可以切换到这些标准注释，而无需更改缓存存储（或配置）。

#  Feature Summary

下表描述了Spring注释与JSR-107副本之间的主要区别：

| Spring                         | JSR-107           | Remark                                                       |
| :----------------------------- | :---------------- | :----------------------------------------------------------- |
| `@Cacheable`                   | `@CacheResult`    | Fairly similar. `@CacheResult` can cache specific exceptions and force the execution of the method regardless of the content of the cache. |
| `@CachePut`                    | `@CachePut`       | While Spring updates the cache with the result of the method invocation, JCache requires that it be passed it as an argument that is annotated with `@CacheValue`. Due to this difference, JCache allows updating the cache before or after the actual method invocation. |
| `@CacheEvict`                  | `@CacheRemove`    | Fairly similar. `@CacheRemove` supports conditional eviction when the method invocation results in an exception. |
| `@CacheEvict(allEntries=true)` | `@CacheRemoveAll` | See `@CacheRemove`.                                          |
| `@CacheConfig`                 | `@CacheDefaults`  | Lets you configure the same concepts, in a similar fashion.  |

JCache具有javax.cache.annotation.CacheResolver的概念，该概念与Spring的CacheResolver接口相同，只是JCache仅支持单个缓存。
默认情况下，一个简单的实现根据 注解中声明的名称检索要使用的缓存。
应该注意的是，如果注释中未指定缓存名称，则会自动生成一个默认值。

CacheResolver实例由CacheResolverFactory检索。可以为每个缓存操作自定义工厂，如以下示例所示：

```java
@CacheResult(cacheNames="books", cacheResolverFactory=MyCacheResolverFactory.class) 
public Book findBook(ISBN isbn)
```

*key* 是由javax.cache.annotation.CacheKeyGenerator生成的，其作用与Spring的KeyGenerator相同。
默认情况下，将考虑所有方法参数，除非至少一个参数用@CacheKey注释。这类似于Spring的自定义密钥生成声明。
例如，以下是相同的操作，一个使用Spring的抽象，另一个使用JCache：

```java
@Cacheable(cacheNames="books", key="#isbn")
public Book findBook(ISBN isbn, boolean checkWarehouse, boolean includeUsed)

@CacheResult(cacheName="books")
public Book findBook(@CacheKey ISBN isbn, boolean checkWarehouse, boolean includeUsed)
```

**您还可以在操作上指定CacheKeyResolver，类似于指定CacheResolverFactory的方式。**



**可以缓存异常**

JCache可以管理带注解的方法引发的异常。这样可以防止更新缓存，但是也可以将异常缓存为失败的指示，而不必再次调用该方法。
假定如果ISBN的结构无效，则引发InvalidIsbnNotFoundException。
这是一个永久性的失败（使用这样的参数无法检索任何书籍）。
以下内容缓存了该异常，以便使用相同的无效ISBN进行的进一步调用直接引发该缓存的异常，而不是再次调用该方法：

```java
@CacheResult(cacheName="books", exceptionCacheName="failures"
            cachedExceptions = InvalidIsbnNotFoundException.class)
public Book findBook(ISBN isbn)
```

# Enabling JSR-107 Support

除了启用Spring的声明性注释支持外，您无需执行任何其他操作即可启用JSR-107支持。
如果类路径中同时存在JSR-107 API和spring-context-support模块，则@EnableCaching和cache：annotation-driven元素都会自动启用JCache支持。



