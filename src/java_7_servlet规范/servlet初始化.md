# tomcat编程式启动

## 引入 tomcat内置库

```xml
<dependency>
            <groupId>org.apache.tomcat.embed</groupId>
            <artifactId>tomcat-embed-core</artifactId>
            <version>8.5.35</version>
</dependency>
<dependency>
        <groupId>org.apache.tomcat.embed</groupId>
        <artifactId>tomcat-embed-jasper</artifactId>
        <version>8.5.35</version>
</dependency>
```

## 调用Main方法

```java
public class MainApplication {
    public static void main(String[] args) throws LifecycleException {
        Tomcat tomcat = new Tomcat();
        tomcat.setPort(8080);
        tomcat.addWebapp("/","D://tomcat/");
        tomcat.start();
        tomcat.getServer().await();
    }
}
```



## 初始化其他

## SPI接口

**接口**

*javax.servlet.ServletContainerInitializer*

tomcat容器启动时，使用  *SPI* 机制 调用 其所有实现类，并执行 *onStartup* 方法

```
public void onStartup(@Nullable Set<Class<?>> webAppInitializerClasses, ServletContext servletContext)
```



## **注解**

```
@HandlesTypes(WebApplicationInitializer.class)
```

会获取该接口的所有实现类 并再调用 *onStartUp* 时 传递给客户端







# 示例

> org.springframework.web.SpringServletContainerInitializer

这是springMVC启动时的加载类

```java
	public void onStartup(@Nullable Set<Class<?>> webAppInitializerClasses, ServletContext servletContext)
			throws ServletException {

		List<WebApplicationInitializer> initializers = Collections.emptyList();
		//实例化所有WebApplicationInitializer
		if (webAppInitializerClasses != null) {
			initializers = new ArrayList<>(webAppInitializerClasses.size());
			for (Class<?> waiClass : webAppInitializerClasses) {
				// Be defensive: Some servlet containers provide us with invalid classes,
				// no matter what @HandlesTypes says...
				if (!waiClass.isInterface() && !Modifier.isAbstract(waiClass.getModifiers()) &&
						WebApplicationInitializer.class.isAssignableFrom(waiClass)) {
					try {
						initializers.add((WebApplicationInitializer)
								ReflectionUtils.accessibleConstructor(waiClass).newInstance());
					}
					catch (Throwable ex) {
						throw new ServletException("Failed to instantiate WebApplicationInitializer class", ex);
					}
				}
			}
		}

		if (initializers.isEmpty()) {
			servletContext.log("No Spring WebApplicationInitializer types detected on classpath");
			return;
		}

		servletContext.log(initializers.size() + " Spring WebApplicationInitializers detected on classpath");
		AnnotationAwareOrderComparator.sort(initializers);
		for (WebApplicationInitializer initializer : initializers) {
			initializer.onStartup(servletContext);
		}
	}
```

然后配置SPI 的 service

![](/images/spring-web-spi-metainfo.png)

**文件内容为**

```
org.springframework.web.SpringServletContainerInitializer
```

