# spring#注解与$的区别

1. #{} springEL表达式
2. 主要有以下作用
   1. 获取bean的某些属性 `@Value("#dataSource.url")`
   2. 调用bean的某个方法
   3. 表示常量 @Value("#{1}") @Value("#{'springEL'}")
3. ${} 可以获取对应属性文件的值

# spring区分开发与生产配置文件

1. 在spring配置文件中配置

```xml
<beans profile="local">
	<context:property-placeholder location="classpath:/config/" />
</beans>

<beans profile="test">
	<context:property-placeholder location="classpath:/config/" />
</beans>
```

2. 在web.xml配置默认值

```xml
<context-parm>
	<parm-name>spring.profiles.default</parm-name>
    <parm-value>local</parm-value>
</context-parm>
```

3. 启动时指定配置

```
JAVA_OPTIONS="-Dspring.profiles.active=test"
```

