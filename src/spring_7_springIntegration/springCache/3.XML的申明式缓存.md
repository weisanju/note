# 基于声明式XML的缓存

如果不能使用注释（可能是由于无法访问源代码或没有外部代码），则可以使用XML进行声明式缓存。因此，您可以在外部指定目标方法和缓存指令，而不是注释用于缓存的方法（类似于声明式事务管理建议）。
上一节中的示例可以转换为以下示例：

```java
<!-- the service we want to make cacheable -->
<bean id="bookService" class="x.y.service.DefaultBookService"/>

<!-- cache definitions -->
<cache:advice id="cacheAdvice" cache-manager="cacheManager">
    <cache:caching cache="books">
        <cache:cacheable method="findBook" key="#isbn"/>
        <cache:cache-evict method="loadBooks" all-entries="true"/>
    </cache:caching>
</cache:advice>

<!-- apply the cacheable behavior to all BookService interfaces -->
<aop:config>
    <aop:advisor advice-ref="cacheAdvice" pointcut="execution(* x.y.BookService.*(..))"/>
</aop:config>

<!-- cache manager definition omitted -->
```

