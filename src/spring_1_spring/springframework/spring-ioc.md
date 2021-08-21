{% raw %}
# spring总结

## *beanFactory*与 *ApplicationContext* 有什么区别

*beanFactory*提供了配置框架的基本功能

* 提供 获取bean及其相关信息

而*ApplicationContext*作为它的子类 多了一些企业级的功能

* AOP集成
* 消息资源处理
* 事件发布
* 应用专用容器,例如 *WebApplicationContext*

通过元数据配置, 告诉容器如何

* 初始化 , 实例化 , 组装一个bean



## 配置bean的方式

* *xml*
* 注解
* java代码





# 配置Bean元数据

## 基于xml

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://www.springframework.org/schema/beans
        https://www.springframework.org/schema/beans/spring-beans.xsd">

    <bean id="..." class="...">  
        <!-- collaborators and configuration for this bean go here -->
    </bean>

    <bean id="..." class="...">
        <!-- collaborators and configuration for this bean go here -->
    </bean>
    <!-- more bean definitions go here -->
</beans>
```

## 导入其他XML定义

```xml
<beans>
    <import resource="services.xml"/>
    <import resource="resources/messageSource.xml"/>
    <import resource="/resources/themeSource.xml"/>

    <bean id="bean1" class="..."/>
    <bean id="bean2" class="..."/>
</beans>
```

# 容器

## spring容器类

* *ClassPathXmlApplicationContext* 基于类路径下的xml装载的容器类
* *FileSystemXmlApplicationContext* 基于文件系统下的xml装载的容器类

### 实例化容器

```java
ApplicationContext context = new ClassPathXmlApplicationContext("services.xml", "daos.xml");
//通用容器对象
ApplicationContext context = new GenericGroovyApplicationContext("services.groovy", "daos.groovy");
//加入xmlbean定义
new XmlBeanDefinitionReader(context).loadBeanDefinitions("services.xml", "daos.xml");
context.refresh();
//加入grouvybean定义
new GroovyBeanDefinitionReader(context).loadBeanDefinitions("services.groovy", "daos.groovy");
context.refresh();

// xml容器对象
ApplicationContext context = new ClassPathXmlApplicationContext("services.xml", "daos.xml");
// retrieve configured instance
PetStoreService service = context.getBean("petStore", PetStoreService.class);
```

# BeanDefintion

* bean定义描述了以何种方式创建bean

* 除了从bean定义创建bean,还可以从外部注册bean

  ```java
  getBeanFactory().registerSingleton(..)
  getBeanFactory().registerBeanDefinition(..)
  ```

### bean属性

| Property                 | Explained in…                                                |
| :----------------------- | :----------------------------------------------------------- |
| Class                    | [Instantiating Beans](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-factory-class) |
| Name                     | [Naming Beans](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-beanname) |
| Scope                    | [Bean Scopes](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-factory-scopes) |
| Constructor arguments    | [Dependency Injection](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-factory-collaborators) |
| Properties               | [Dependency Injection](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-factory-collaborators) |
| Autowiring mode          | [Autowiring Collaborators](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-factory-autowire) |
| Lazy initialization mode | [Lazy-initialized Beans](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-factory-lazy-init) |
| Initialization method    | [Initialization Callbacks](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-factory-lifecycle-initializingbean) |
| Destruction method       | [Destruction Callbacks](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-factory-lifecycle-disposablebean) |

### bean别名

id唯一,name可以多个,以 空格,逗号,分号,分隔

```xml
<alias name="myApp-dataSource" alias="subsystemA-dataSource"/>
```

### bean实例化

```xml
构造方法
<bean id="exampleBean" class="examples.ExampleBean"/>
静态工厂
<bean id="clientService"
    class="examples.ClientService"
    factory-method="createInstance"/>
实例工厂
<!-- the factory bean, which contains a method called createInstance() -->
<bean id="serviceLocator" class="examples.DefaultServiceLocator">
    <!-- inject any dependencies required by this locator bean -->
</bean>

<!-- the bean to be created via the factory bean -->
<bean id="clientService"
    factory-bean="serviceLocator"
    factory-method="createClientServiceInstance"/>

```

# 依赖注入

## 构造注入

```
构造注入-引用类型
<bean id="beanOne" class="x.y.ThingOne">
        <constructor-arg ref="beanTwo"/>
        <constructor-arg ref="beanThree"/>
</bean>
构造注入-基本类型
<bean id="exampleBean" class="examples.ExampleBean">
    <constructor-arg type="int" value="7500000"/>
    <constructor-arg type="java.lang.String" value="42"/>
</bean>

构造注入-按索引注入
<bean id="exampleBean" class="examples.ExampleBean">
    <constructor-arg index="0" value="7500000"/>
    <constructor-arg index="1" value="42"/>
</bean>
构造注入-按参数名注入-
前提是 jvm开启Debug选项
可以使用 @ConstructorProperties注解给定参数名
<bean id="exampleBean" class="examples.ExampleBean">
    <constructor-arg name="years" value="7500000"/>
    <constructor-arg name="ultimateAnswer" value="42"/>
</bean>
```

```java
    @ConstructorProperties({"years", "ultimateAnswer"})
    public ExampleBean(int years, String ultimateAnswer) {
        this.years = years;
        this.ultimateAnswer = ultimateAnswer;
    }
```



## setter方法注入

```xml
<bean id="exampleBean" class="examples.ExampleBean">
    <properties name="ddd" ref="dd"/>
</bean>
```

**推荐使用构造注入注入必须的变量, 使用setter注入不是必须的变量**



## 被注入的值的写法

### 字面量

**字符串和基本数据类型原样写**

### 集合

`<list/>`, `<set/>`, `<map/>`, and `<props/>`

```xml
<bean id="moreComplexObject" class="example.ComplexObject">
    <!-- results in a setAdminEmails(java.util.Properties) call -->
    <property name="adminEmails">
        <props>
            <prop key="administrator">administrator@example.org</prop>
            <prop key="support">support@example.org</prop>
            <prop key="development">development@example.org</prop>
        </props>
    </property>
    <!-- results in a setSomeList(java.util.List) call -->
    <property name="someList">
        <list>
            <value>a list element followed by a reference</value>
            <ref bean="myDataSource" />
        </list>
    </property>
    <!-- results in a setSomeMap(java.util.Map) call -->
    <property name="someMap">
        <map>
            <entry key="an entry" value="just some string"/>
            <entry key ="a ref" value-ref="myDataSource"/>
        </map>
    </property>
    <!-- results in a setSomeSet(java.util.Set) call -->
    <property name="someSet">
        <set>
            <value>just some string</value>
            <ref bean="myDataSource" />
        </set>
    </property>
</bean>

集合合并 merge=true
<beans>
    <bean id="parent" abstract="true" class="example.ComplexObject">
        <property name="adminEmails">
            <props>
                <prop key="administrator">administrator@example.com</prop>
                <prop key="support">support@example.com</prop>
            </props>
        </property>
    </bean>
    <bean id="child" parent="parent">
        <property name="adminEmails">
            <!-- the merge is specified on the child collection definition -->
            <props merge="true">
                <prop key="sales">sales@example.com</prop>
                <prop key="support">support@example.co.uk</prop>
            </props>
        </property>
    </bean>
<beans>
```

### 空串与null

```
等价于 ""
<bean class="ExampleBean">
    <property name="email" value=""/>
</bean>


<bean class="ExampleBean">
    <property name="email">
        <null/>
    </property>
</bean>
```



### 名称空间注入

```xml
p名称空间
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:p="http://www.springframework.org/schema/p"
    xsi:schemaLocation="http://www.springframework.org/schema/beans
    https://www.springframework.org/schema/beans/spring-beans.xsd">

    <bean id="myDataSource" class="org.apache.commons.dbcp.BasicDataSource"
        destroy-method="close"
        p:driverClassName="com.mysql.jdbc.Driver"
        p:url="jdbc:mysql://localhost:3306/mydb"
        p:username="root"
        p:password="masterkaoli"/>

</beans>
c名称空间
<!-- c-namespace index declaration -->
<bean id="beanOne" class="x.y.ThingOne" c:_0-ref="beanTwo" c:_1-ref="beanThree"
    c:_2="something@somewhere.com"/>

```

### 嵌套注入

```xml
<bean id="something" class="things.ThingOne">
    <property name="fred.bob.sammy" value="123" />
</bean>
```

### properties实例注入

```xml
<bean id="mappings"
    class="org.springframework.context.support.PropertySourcesPlaceholderConfigurer">

    <!-- typed as a java.util.Properties -->
    <property name="properties">
        <value>
            jdbc.driver.className=com.mysql.jdbc.Driver
            jdbc.url=jdbc:mysql://localhost:3306/mydb
        </value>
    </property>
</bean>
```

### *idref*

```xml
避免 值与引用混淆
<!-- in the child (descendant) context -->
<bean id="accountService" <!-- bean name is the same as the parent bean -->
    class="org.springframework.aop.framework.ProxyFactoryBean">
    <property name="target">
        <ref parent="accountService"/> <!-- notice how we refer to the parent bean -->
    </property>
    <!-- insert other configuration and dependencies as required here -->
</bean>
```

### *depends-on*

在某个bean之前实例化,用于两个不直接依赖的bean

```
<bean id="beanOne" class="ExampleBean" depends-on="manager"/>
<bean id="manager" class="ManagerBean" />
```

### Context手动获取

使用场景: 两个bean的生命周期不一致,使用自动注入只会调用一次,

可以使用 getBean(String name)来获得

```java
// a class that uses a stateful Command-style class to perform some processing
package fiona.apple;

// Spring-API imports
import org.springframework.beans.BeansException;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;

public class CommandManager implements ApplicationContextAware {

    private ApplicationContext applicationContext;

    public Object process(Map commandState) {
        // grab a new instance of the appropriate Command
        Command command = createCommand();
        // set the state on the (hopefully brand new) Command instance
        command.setState(commandState);
        return command.execute();
    }

    protected Command createCommand() {
        // notice the Spring API dependency!
        return this.applicationContext.getBean("command", Command.class);
    }

    public void setApplicationContext(
            ApplicationContext applicationContext) throws BeansException {
        this.applicationContext = applicationContext;
    }
}
```

### 抽象方法注入

```xml
<!-- a stateful bean deployed as a prototype (non-singleton) -->
<bean id="myCommand" class="fiona.apple.AsyncCommand" scope="prototype">
    <!-- inject dependencies here as required -->
</bean>

<!-- commandProcessor uses statefulCommandHelper -->
<bean id="commandManager" class="fiona.apple.CommandManager">
    <lookup-method name="createCommand" bean="myCommand"/>
</bean>
```



```java
package fiona.apple;

// no more Spring imports!

public abstract class CommandManager {

    public Object process(Object commandState) {
        // grab a new instance of the appropriate Command interface
        Command command = createCommand();
        // set the state on the (hopefully brand new) Command instance
        command.setState(commandState);
        return command.execute();
    }

    // okay... but where is the implementation of this method?
    protected abstract Command createCommand();
}

或者使用注解
@Lookup("name")
    如果不写名称则根据 方法的返回值类型查找,如果不写名字则需要写具体类名
    
​ Spring的Lookup method inject实现原理的是使用CGLIB动态生成一个类去继承CommandManager，重写createCommand方法。然后根据@Lookup中指定的bean Name或者createCommand方法的返回类型判断需要返回的bean。createCommand可以是abstract和可以不是。因为使用的是继承，所以CommandManager类和createCommand方法都不能是final的。

createCommand方法的签名需要满足如下要求

<public|protected> [abstract] <return-type> theMethodName(no-arguments);
```

### 方法替换

待替换的方法

```java
public class MyValueCalculator {

    public String computeValue(String input) {
        // some real code...
    }

    // some other methods...
}
```

重新实现的方法

```java
public class ReplacementComputeValue implements MethodReplacer {

    public Object reimplement(Object o, Method m, Object[] args) throws Throwable {
        // get the input value, work with it, and return a computed result
        String input = (String) args[0];
        ...
        return ...;
    }
}
```

配置

```
<bean id="myValueCalculator" class="x.y.z.MyValueCalculator">
    <!-- arbitrary method replacement -->
    <replaced-method name="computeValue" replacer="replacementComputeValue">
        <arg-type>String</arg-type>
    </replaced-method>
</bean>

<bean id="replacementComputeValue" class="a.b.c.ReplacementComputeValue"/>
构造参数类型主要是为了区别 重载的方法
只能配置
```

# 作用域

### 属性列表

| Scope                                                        | Description                                                  |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| [singleton](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-factory-scopes-singleton) | 单例                                                         |
| [prototype](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-factory-scopes-prototype) | 多例                                                         |
| [request](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-factory-scopes-request) | 每来一个 Http请求中 就会产生一个                             |
| [session](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-factory-scopes-session) | Httpsession                                                  |
| [application](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-factory-scopes-application) | Scopes a single bean definition to the lifecycle of a `ServletContext`. Only valid in the context of a web-aware Spring `ApplicationContext`. |
| [websocket](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/web.html#websocket-stomp-websocket-scope) | Scopes a single bean definition to the lifecycle of a `WebSocket`. Only valid in the context of a web-aware Spring `ApplicationContext`. |

### 生存周期不一致的bean 访问方式

* 单例a依赖注入 prototype的实例b时, 每次访问b, b不会变,就是直接访问,不会去scope中取
* 如果 proxyMode=ScopedProxyMode.TARGET_CLASS 或者*interface*,则 会访问b时 会生成一个代理类,里面根据 scope取值

```
package com.weisanju.javaconfig.config;

import org.springframework.context.annotation.Scope;
import org.springframework.context.annotation.ScopedProxyMode;
import org.springframework.stereotype.Component;

@Component
@Scope(proxyMode=ScopedProxyMode.NO,value = "prototype")
public class MyValueCalculator {
    public String computeValue(String input) {
        System.out.println(input);
        return input;
    }
}
在被注入的时候指定代理形式
```

### 自定义 *scope*

1. 实现 *org.springframework.beans.factory.config.Scope*接口

   基于 时间的作用域

   ```java
   /**
    * 首先自定义作用域范围类TimeScope:
    * Scope接口提供了五个方法，只有get()和remove()是必须实现，get()中写获取逻辑，
    * 如果已有存储中没有该名称的bean，则通过objectFactory.getObject()创建实例。
    */
   @Slf4j
   public class TimeScope implements Scope {
   
       private static Map<String, Map<Integer, Object>> scopeBeanMap = new HashMap<>();
   
       @Override
       public Object get(String name, ObjectFactory<?> objectFactory) {
           Integer hour = Calendar.getInstance().get(Calendar.HOUR_OF_DAY);
           // 当前是一天内的第多少分钟
           Integer minute = hour * 60 + Calendar.getInstance().get(Calendar.MINUTE);
           log.info("当前是第 {} 分钟", minute);
           Map<Integer, Object> objectMap = scopeBeanMap.get(name);
           Object object = null;
           if (Objects.isNull(objectMap)) {
               objectMap = new HashMap<>();
               object = objectFactory.getObject();
               objectMap.put(minute, object);
               scopeBeanMap.put(name, objectMap);
           } else {
               object = objectMap.get(minute);
               if (Objects.isNull(object)) {
                   object = objectFactory.getObject();
                   objectMap.put(minute, object);
                   scopeBeanMap.put(name, objectMap);
               }
           }
           return object;
       }
   
       @Override
       public Object remove(String name) {
           return scopeBeanMap.remove(name);
       }
   
       @Override
       public void registerDestructionCallback(String name, Runnable callback) {
       }
       @Override
       public Object resolveContextualObject(String key) {
           return null;
       }
       @Override
       public String getConversationId() {
           return null;
       }
   }
   ```

   

2. 注册到 *org.springframework.beans.factory.config.CustomScopeConfigurer* 上

   ```java
   @Configuration
   @Slf4j
   public class BeanScopeConfig {
       @Bean
       public CustomScopeConfigurer customScopeConfigurer() {
           CustomScopeConfigurer customScopeConfigurer = new CustomScopeConfigurer();
           Map<String, Object> map = new HashMap<>();
           map.put("timeScope", new TimeScope());
           customScopeConfigurer.setScopes(map);
           return customScopeConfigurer;
       }
       
       @Bean
       @Scope(value = "timeScope", proxyMode = ScopedProxyMode.TARGET_CLASS)
       public TimeScopeBean timeScopeBean() {
           TimeScopeBean timeScopeBean = new TimeScopeBean();
           timeScopeBean.setCurrentTime(System.currentTimeMillis());
           log.info("time scope bean");
           return timeScopeBean;
       }
   }
   ```

   

3. 使用 

   *@Scope(proxyMode=ScopedProxyMode.TARGET_CLASS,value = "thread")*

## 自定义bean的特性

### 三类回调形式

[Lifecycle Callbacks](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-factory-lifecycle) 生命周期回调

[`ApplicationContextAware` and `BeanNameAware`](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-factory-aware) bean注入的回调

[Other `Aware` Interfaces](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#aware-list) 其他的回调接口

### 生命周期回调

#### Bean生命周期回调

* 通过 实现 *InitializingBean* ,*DisposableBean*
* 推荐使用 @PostConstruct` and `@PreDestroy ,这可与 spring特定接口 松耦合
* 或者使用 bean定义 init-method` and `destroy-method 属性

* spring通过 *BeanPostProcessor* 接口 进行回调处理,如果需要自定义可以自行实现
* 被管理的bean可以实现 *Lifecycle* 接口

**Initialization Callbacks**

* 发生于 容器初始完 所有必须的属性时
* 推荐使用 * [`@PostConstruct`](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-postconstruct-and-predestroy-annotations) * 或者使用  Beandefinition 的  init-method` and `destroy-method 或者java注解的@Bean的属性,initMethod

**Destruction Callbacks**

* 类似于上面

**Default Initialization and Destroy Methods**

或者指定全局默认的 init,destroy方法

```
<beans default-init-method="init">

    <bean id="blogService" class="com.something.DefaultBlogService">
        <property name="blogDao" ref="blogDao" />
    </bean>

</beans>
// destroy-method
```

**以上各个 回调实现的组合调用顺序**

Multiple lifecycle mechanisms configured for the same bean, with different initialization methods, are called as follows:

1. Methods annotated with `@PostConstruct`
2. `afterPropertiesSet()` as defined by the `InitializingBean` callback interface
3. A custom configured `init()` method

Destroy methods are called in the same order:

1. Methods annotated with `@PreDestroy`
2. `destroy()` as defined by the `DisposableBean` callback interface
3. A custom configured `destroy()` method

#### Startup and Shutdown Callbacks

* *Lifecycle* 定义了 bean自己的 生命周期

* 容器会在收到 start stop信号后,将调用所有实现了该接口的方法,容器将委托给 *LifecycleProcessor* 去处理

* 只会在显示启动,或者显示停止时调用,要更细粒度的控制, 参照*SmartLifecycle*

  ```
  	Note that the regular org.springframework.context.Lifecycle interface is a plain contract for explicit start and stop notifications and does not imply auto-startup at context refresh time. For fine-grained control over auto-startup of a specific bean (including startup phases), consider implementing org.springframework.context.SmartLifecycle instead.
  
  Also, please note that stop notifications are not guaranteed to come before destruction. On regular shutdown, all Lifecycle beans first receive a stop notification before the general destruction callbacks are being propagated. However, on hot refresh during a context’s lifetime or on aborted refresh attempts, only destroy methods are called.
  ```

* bean 对象 之间的 start,stop决定于 *depends-on* 和显示依赖注入,对于 某一类型 与另一类型的顺序 这 在 *SmartLifecycle* 有实现

  ```java
  public interface Phased {
      int getPhase();
  }
  ```

* 当启动时, 最小的 *phase* 先启动, 关闭时 最大的 phase先关闭

* 对于普通的  “normal” `Lifecycle`  ,他们的 phase为0

* `SmartLifecycle` 的stop方法有回调,所有实现 `SmartLifecycle` 接口的 类 必须在 stop完后 回调该 stop方法

  ```
  	default void stop(Runnable callback) {
  		stop();
  		callback.run();
  	}
  ```

* processor的默认实现 在各个 bean关闭时的 默认超时时间 30s

  ```java
  <bean id="lifecycleProcessor" class="org.springframework.context.support.DefaultLifecycleProcessor">
      <!-- timeout value in milliseconds -->
      <property name="timeoutPerShutdownPhase" value="10000"/>
  </bean>
  ```

* processor还提供了  *onRefresh* 的回调 , 它会判断 `SmartLifecycle`  的isAutoStart 的标志

#### 优雅的关闭非web的容器

* springWebmvc的容器已经实现了该特性

* 在jvm那里 注册一个 钩子回调,实际上是 在jvm那里 注册一个 线程用于关闭

  ```
          ConfigurableApplicationContext ctx = new ClassPathXmlApplicationContext("beans.xml");
  
          // add a shutdown hook for the above context...
          ctx.registerShutdownHook();
          
          
  {
  		if (this.shutdownHook == null) {
  			// No shutdown hook registered yet.
  			this.shutdownHook = new Thread(SHUTDOWN_HOOK_THREAD_NAME) {
  				@Override
  				public void run() {
  					synchronized (startupShutdownMonitor) {
  						doClose();
  					}
  				}
  			};
  			Runtime.getRuntime().addShutdownHook(this.shutdownHook);
  		}
  	}
  ```

  

### `ApplicationContextAware` and `BeanNameAware`

* 用于获取 容器引用或者 bean引用,推荐使用 注解注入

* `BeanNameAware` 回调 迟于 各种属性填充前, 早于 各种初始化回调前

### Other `Aware` Interfaces

| Name                             | Injected Dependency                                          | Explained in…                                                |
| :------------------------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| `ApplicationContextAware`        | Declaring `ApplicationContext`.                              | [`ApplicationContextAware` and `BeanNameAware`](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-factory-aware) |
| `ApplicationEventPublisherAware` | Event publisher of the enclosing `ApplicationContext`.       | [Additional Capabilities of the `ApplicationContext`](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#context-introduction) |
| `BeanClassLoaderAware`           | Class loader used to load the bean classes.                  | [Instantiating Beans](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-factory-class) |
| `BeanFactoryAware`               | Declaring `BeanFactory`.                                     | [`ApplicationContextAware` and `BeanNameAware`](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-factory-aware) |
| `BeanNameAware`                  | Name of the declaring bean.                                  | [`ApplicationContextAware` and `BeanNameAware`](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-factory-aware) |
| `BootstrapContextAware`          | Resource adapter `BootstrapContext` the container runs in. Typically available only in JCA-aware `ApplicationContext` instances. | [JCA CCI](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/integration.html#cci) |
| `LoadTimeWeaverAware`            | Defined weaver for processing class definition at load time. | [Load-time Weaving with AspectJ in the Spring Framework](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#aop-aj-ltw) |
| `MessageSourceAware`             | Configured strategy for resolving messages (with support for parametrization and internationalization). | [Additional Capabilities of the `ApplicationContext`](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#context-introduction) |
| `NotificationPublisherAware`     | Spring JMX notification publisher.                           | [Notifications](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/integration.html#jmx-notifications) |
| `ResourceLoaderAware`            | Configured loader for low-level access to resources.         | [Resources](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#resources) |
| `ServletConfigAware`             | Current `ServletConfig` the container runs in. Valid only in a web-aware Spring `ApplicationContext`. | [Spring MVC](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/web.html#mvc) |
| `ServletContextAware`            | Current `ServletContext` the container runs in. Valid only in a web-aware Spring `ApplicationContext`. | [Spring MVC](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/web.html#mvc) |

## Bean Definition Inheritance

bean定义继承

* bean继承以 子类为准
* 父类可以如果不写 class,必须abstract为true
* bean之间的同名属性必须是 兼容的
* 如果abstract 定义为 true 则该bean定义为模板,不会产生实例

```xml
<bean id="inheritedTestBean" abstract="true"
        class="org.springframework.beans.TestBean">
    <property name="name" value="parent"/>
    <property name="age" value="1"/>
</bean>

<bean id="inheritsWithDifferentClass"
        class="org.springframework.beans.DerivedTestBean"
        parent="inheritedTestBean" init-method="initialize">  
    <property name="name" value="override"/>
    <!-- the age property value of 1 will be inherited from parent -->
</bean>

//不指定class
<bean id="inheritedTestBeanWithoutClass" abstract="true">
    <property name="name" value="parent"/>
    <property name="age" value="1"/>
</bean>

<bean id="inheritsWithClass" class="org.springframework.beans.DerivedTestBean"
        parent="inheritedTestBeanWithoutClass" init-method="initialize">
    <property name="name" value="override"/>
    <!-- age will inherit the value of 1 from the parent bean definition-->
</bean>
```

## Container Extension Points

容器扩展点

spring容器提供各种接口 以供开发人员扩展

### `BeanPostProcessor`

* *BeanPostProcessor*  可以设置任意个
* 通过order属性 排序
* 作用域是 容器中,容器间得另外注册
* *BeanFactoryPostProcessor* 可以改变BeanDefintion
* 回调发生在每个bean对象 创建后, *`InitializingBean.afterPropertiesSet()` or any declared `init` method*  容器初始化完,或者任何申明得初始化方法,在其他bean初始化后
* 它可以对任何 bean采取行动, 一般用于bean的代理
* 容器会根据 配置元数据(xml,或java) 注册 这些 beanpostProcessor  
* beanpostprocessor 的初始化需要早于其他bean的初始化
* 编程方式 注册
  * *ConfigurableBeanFactory.addBeanPostProcessor*通过这个 手动注册,当你有业务逻辑时
  * 不会遵守 order 顺序,注册的顺序决定 执行顺序
  * 调用发生在 自动检测bean之前
* *AOP auto-proxying* 是基于这个接口的 ,所以任何引用该类型的 类都不应该 对其 使用AOP

**Example**

*RequiredAnnotationBeanPostProcessor* 依赖注入时 确保属性的必输项都输入( 现在更推荐 构造器注入)已过期

###  `BeanFactoryPostProcessor`

Customizing Configuration Metadata with a `BeanFactoryPostProcessor`

* 用来修改bean定义本身,这种改变时不可逆的
* 通过实现 *order*接口 来配置  BeanFactoryPostProcessor间的 顺序
* 作用域时容器范围内
* 所有的postProcessor会忽略 懒加载

**Example**

`PropertySourcesPlaceholderConfigurer`

可以使用*PropertySource*替代

* 可以配置多个外部属性配置文件,用来替换  ${}表达式

* 或者手写配置文件

* 如果它失败了则 这时 容器处于 `preInstantiateSingletons()` phase of an `ApplicationContext` for a non-lazy-init bean

  预加载阶段

```xml
<bean class="org.springframework.beans.factory.config.PropertySourcesPlaceholderConfigurer">
    <property name="locations">
        <value>classpath:com/something/strategy.properties</value>
    </property>
    <property name="properties">
        <value>custom.strategy.class=com.something.DefaultStrategy</value>
    </property>
</bean>

<bean id="serviceStrategy" class="${custom.strategy.class}"/>
```

**`PropertyOverrideConfigurer`**

替换 Bean定义的 参数属性

使用标签 :*<context:property-override location="classpath:override.properties"/>*

```
person.name=大师傅似的
beanname.properteis=value
```

```
@Bean
public PropertyOverrideConfigurer propertyOverrideConfigurer(){
    PropertyOverrideConfigurer propertyOverrideConfigurer = new PropertyOverrideConfigurer();
    propertyOverrideConfigurer.setFileEncoding("UTF-8");
    propertyOverrideConfigurer.setLocation(new ClassPathResource("my.properties"));
    return propertyOverrideConfigurer;
}
```

###  `FactoryBean`

Customizing Instantiation Logic with a factoryBean

实现自定义 bean定义 逻辑

## Annotation-based Container Configuration

* 基于注解的注入 比XML 注入更早执行,所以xml的注入会覆盖注解的注入
* 基于注解的注入实际上是 一个个beanPostProcessor
* 隐式注册这些beanPostProcessor :  <context:annotation-config/>
  *  [`AutowiredAnnotationBeanPostProcessor`](https://docs.spring.io/spring-framework/docs/5.2.7.RELEASE/javadoc-api/org/springframework/beans/factory/annotation/AutowiredAnnotationBeanPostProcessor.html)
  * [`CommonAnnotationBeanPostProcessor`](https://docs.spring.io/spring-framework/docs/5.2.7.RELEASE/javadoc-api/org/springframework/context/annotation/CommonAnnotationBeanPostProcessor.html)
  * [`PersistenceAnnotationBeanPostProcessor`](https://docs.spring.io/spring-framework/docs/5.2.7.RELEASE/javadoc-api/org/springframework/orm/jpa/support/PersistenceAnnotationBeanPostProcessor.html) 
  * [`RequiredAnnotationBeanPostProcessor`](https://docs.spring.io/spring-framework/docs/5.2.7.RELEASE/javadoc-api/org/springframework/beans/factory/annotation/RequiredAnnotationBeanPostProcessor.html)

### 	@Required

标识该setter方法的注入必须, 已过期,推荐使用构造器注入

```
public class SimpleMovieLister {

    private MovieFinder movieFinder;

    @Required
    public void setMovieFinder(MovieFinder movieFinder) {
        this.movieFinder = movieFinder;
    }

    // ...
}
```

### @Autowired

构造方法上(当 只有一个构造方法时,不是很必要)

```java
public class MovieRecommender {

    private final CustomerPreferenceDao customerPreferenceDao;

    @Autowired
    public MovieRecommender(CustomerPreferenceDao customerPreferenceDao) {
        this.customerPreferenceDao = customerPreferenceDao;
    }
}
```

setter注入

```
public class SimpleMovieLister {

    private MovieFinder movieFinder;

    @Autowired
    public void setMovieFinder(MovieFinder movieFinder) {
        this.movieFinder = movieFinder;
    }

    // ...
}
```

字段注入

```java
 @Autowired
    private MovieCatalog movieCatalog;
```

可以注入某一类Bean

可以@Order或者order接口,实现注入的排序,否则顺序以注册顺序为准,@Order也会影响依赖注入顺序

```java
public class MovieRecommender {
    @Autowired
    private MovieCatalog[] movieCatalogs;
}
public class MovieRecommender {
    private Set<MovieCatalog> movieCatalogs;
    @Autowired
    public void setMovieCatalogs(Set<MovieCatalog> movieCatalogs) {
        this.movieCatalogs = movieCatalogs;
    }
}
```

Map注入

这回注入所有 beanname,和某一类型的bean

```java
public class MovieRecommender {
    private Map<String, MovieCatalog> movieCatalogs;
    @Autowired
    public void setMovieCatalogs(Map<String, MovieCatalog> movieCatalogs) {
        this.movieCatalogs = movieCatalogs;
    }
}
```

可以不启用

```
public class SimpleMovieLister {
    private MovieFinder movieFinder;
    @Autowired(required = false)
    public void setMovieFinder(MovieFinder movieFinder) {
        this.movieFinder = movieFinder;
    }
}
```

注入Optional类

```java
public class SimpleMovieLister {
    @Autowired
    public void setMovieFinder(Optional<MovieFinder> movieFinder) {
    }
}
```

可以使用@nullable

```java
    @Autowired
    public void setMovieFinder(@Nullable MovieFinder movieFinder) {
        ...
    }
```

可以注入spring相关的bean

```
BeanFactory, ApplicationContext, Environment, ResourceLoader, ApplicationEventPublisher, and MessageSource
ConfigurableApplicationContext or ResourcePatternResolver
public class MovieRecommender {

    @Autowired
    private ApplicationContext context;

    public MovieRecommender() {
    }
}
```

**注意事项**

* 一个bean可能有多个 构造器, 但只有一个 构造器能 @Autowired(required = true),其他的必须是false
* 通过 匹配容器中的bean 满足依赖关系最多的 构造器 将会被使用,如果都不满足 使用默认构造器
* '@Autowired`, `@Inject`, `@Value`, and `@Resource' 这些注入 不能用于BeanPostProcessor或者BeanFactoryPostProcessor

### @Primary

* 通过类型注册可能有多个类型的候选者,可以使用primary指定

```java
@Configuration
public class MovieConfiguration {

    @Bean
    @Primary
    public MovieCatalog firstMovieCatalog() { ... }

    @Bean
    public MovieCatalog secondMovieCatalog() { ... }

    // ...
}
```

xml配置

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:context="http://www.springframework.org/schema/context"
    xsi:schemaLocation="http://www.springframework.org/schema/beans
        https://www.springframework.org/schema/beans/spring-beans.xsd
        http://www.springframework.org/schema/context
        https://www.springframework.org/schema/context/spring-context.xsd">

    <context:annotation-config/>

    <bean class="example.SimpleMovieCatalog" primary="true">
        <!-- inject any dependencies required by this bean -->
    </bean>

    <bean class="example.SimpleMovieCatalog">
        <!-- inject any dependencies required by this bean -->
    </bean>

    <bean id="movieRecommender" class="example.MovieRecommender"/>

</beans>
```

### @Qualifiers

* 与id不同,类似于 手动给定bean分类, 类似于某种bean的过滤器

* 这在注入 集合类的时候 作用尤为明显 `Set<MovieCatalog>`
* 如果没有其他指示器(类似primary,qualifier) ,而且存在多个候选者,则spring会根据 注入字段名或参数名匹配bean
* 如果你想通过bean名匹配,最好使用@Resource, @Autowired的语义是:先找同类型的,然后寻找指定的Qualifer
* 可以通过@Resource引用集合
* qualifier可以自引用,但顺序是最后的

```java
public class MovieRecommender {

    @Autowired
    @Qualifier("main")
    private MovieCatalog movieCatalog;

    // ...
}


public class MovieRecommender {

    private MovieCatalog movieCatalog;

    private CustomerPreferenceDao customerPreferenceDao;

    @Autowired
    public void prepare(@Qualifier("main") MovieCatalog movieCatalog,
            CustomerPreferenceDao customerPreferenceDao) {
        this.movieCatalog = movieCatalog;
        this.customerPreferenceDao = customerPreferenceDao;
    }

    // ...
}
```

xml配置

```hxml
 <bean class="example.SimpleMovieCatalog">
        <qualifier value="main"/> 

        <!-- inject any dependencies required by this bean -->
    </bean>
```

**创建自己的qualifier注解**,给qualifier分类

```java
@Target({ElementType.FIELD, ElementType.PARAMETER})
@Retention(RetentionPolicy.RUNTIME)
@Qualifier
public @interface Genre {

    String value();
}
```

使用注解

```java
public class MovieRecommender {

    @Autowired
    @Genre("Action")
    private MovieCatalog actionCatalog;

    private MovieCatalog comedyCatalog;

    @Autowired
    public void setComedyCatalog(@Genre("Comedy") MovieCatalog comedyCatalog) {
        this.comedyCatalog = comedyCatalog;
    }

    // ...
}
```

可以使用短类名,或者全限定类名

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:context="http://www.springframework.org/schema/context"
    xsi:schemaLocation="http://www.springframework.org/schema/beans
        https://www.springframework.org/schema/beans/spring-beans.xsd
        http://www.springframework.org/schema/context
        https://www.springframework.org/schema/context/spring-context.xsd">

    <context:annotation-config/>

    <bean class="example.SimpleMovieCatalog">
        <qualifier type="Genre" value="Action"/>
        <!-- inject any dependencies required by this bean -->
    </bean>

    <bean class="example.SimpleMovieCatalog">
        <qualifier type="example.Genre" value="Comedy"/>
        <!-- inject any dependencies required by this bean -->
    </bean>

    <bean id="movieRecommender" class="example.MovieRecommender"/>

</beans>
```

给qualifier添加类别,属性

```java
@Target({ElementType.FIELD, ElementType.PARAMETER})
@Retention(RetentionPolicy.RUNTIME)
@Qualifier
public @interface MovieQualifier {

    String genre();

    Format format();
}
public enum Format {
    VHS, DVD, BLURAY
}
```

按照分类注入

```java
public class MovieRecommender {

    @Autowired
    @MovieQualifier(format=Format.VHS, genre="Action")
    private MovieCatalog actionVhsCatalog;

    @Autowired
    @MovieQualifier(format=Format.VHS, genre="Comedy")
    private MovieCatalog comedyVhsCatalog;

    @Autowired
    @MovieQualifier(format=Format.DVD, genre="Action")
    private MovieCatalog actionDvdCatalog;

    @Autowired
    @MovieQualifier(format=Format.BLURAY, genre="Comedy")
    private MovieCatalog comedyBluRayCatalog;

    // ...
}
```

同样可以使用meta标签 简写 qualifier标签,会自动查找该qualifier的值

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:context="http://www.springframework.org/schema/context"
    xsi:schemaLocation="http://www.springframework.org/schema/beans
        https://www.springframework.org/schema/beans/spring-beans.xsd
        http://www.springframework.org/schema/context
        https://www.springframework.org/schema/context/spring-context.xsd">

    <context:annotation-config/>

    <bean class="example.SimpleMovieCatalog">
        <qualifier type="MovieQualifier">
            <attribute key="format" value="VHS"/>
            <attribute key="genre" value="Action"/>
        </qualifier>
        <!-- inject any dependencies required by this bean -->
    </bean>

    <bean class="example.SimpleMovieCatalog">
        <qualifier type="MovieQualifier">
            <attribute key="format" value="VHS"/>
            <attribute key="genre" value="Comedy"/>
        </qualifier>
        <!-- inject any dependencies required by this bean -->
    </bean>

    <bean class="example.SimpleMovieCatalog">
        <meta key="format" value="DVD"/>
        <meta key="genre" value="Action"/>
        <!-- inject any dependencies required by this bean -->
    </bean>

    <bean class="example.SimpleMovieCatalog">
        <meta key="format" value="BLURAY"/>
        <meta key="genre" value="Comedy"/>
        <!-- inject any dependencies required by this bean -->
    </bean>

</beans>
```

### Using Generics

使用泛型自动注入

假设下面的类 实现了 某个泛型接口

```j
Store<String>` and `Store<Integer>
```



```java
@Configuration
public class MyConfiguration {

    @Bean
    public StringStore stringStore() {
        return new StringStore();
    }

    @Bean
    public IntegerStore integerStore() {
        return new IntegerStore();
    }
}


@Autowired
private Store<String> s1; // <String> qualifier, injects the stringStore bean

@Autowired
private Store<Integer> s2; // <Integer> qualifier, injects the integerStore bean
```

泛型同样支持 集合类

```java
@Autowired
private List<Store<Integer>> s;
```

### CustomAutowireConfigurer

*`CustomAutowireConfigurer` 是一个 BeanFactoryPostProcessor*  可以让你注册自己的 qualifier

xml配置

```xml
<bean id="customAutowireConfigurer"
        class="org.springframework.beans.factory.annotation.CustomAutowireConfigurer">
    <property name="customQualifierTypes">
        <set>
            <value>example.CustomQualifier</value>
        </set>
    </property>
</bean>
```

*AutowireCandidateResolver* (QualifierAnnotationAutowireCandidateResolver)选取候选者的方式

* autowire-candidate 每一个bean的 自动注入候选者
* 在`<beans>`中的 default-autowire-candidates
* @qualifier的限定类
* CustomAutowireConfigurer 中的候选类

### @Resource

* 如果没有指定名字 取 方法参数名或者 字段名
* 名称是由 ApplicationContext 提供查找(由CommonAnnotationBeanPostProcessor 注入)

### @Value

注入外部属性

```java
@Component
public class MovieRecommender {

    private final String catalog;

    public MovieRecommender(@Value("${catalog.name}") String catalog) {
        this.catalog = catalog;
    }
}

@Configuration
@PropertySource("classpath:application.properties")
public class AppConfig { }

catalog.name=MovieCatalog
```

如果想要严格控制 不存在的值可以如下申明

```java
@Configuration
public class AppConfig {

     @Bean
     public static PropertySourcesPlaceholderConfigurer propertyPlaceholderConfigurer() {
           return new PropertySourcesPlaceholderConfigurer();
     }
}

```

* 当配置这个JavaConfig Bean时,必须是static
* 可以设置前缀后缀 分隔符,setPlaceholderPrefix`, `setPlaceholderSuffix`, or `setValueSeparator
* *PropertySourcesPlaceholderConfigurer* springboot自动带一个,会从 application.properties` and `application.yml解析

* 值转换的过程 可以自定义ConversionService

  ```java
  @Configuration
  public class AppConfig {
  
      @Bean
      public ConversionService conversionService() {
          DefaultFormattingConversionService conversionService = new DefaultFormattingConversionService();
          conversionService.addConverter(new MyCustomConverter());
          return conversionService;
      }
  }
  ```

* 支持 EL表达式

  ```java
  @Component
  public class MovieRecommender {
  
      private final String catalog;
  
      public MovieRecommender(@Value("#{systemProperties['user.catalog'] + 'Catalog' }") String catalog) {
          this.catalog = catalog;
      }
  }
  
  @Component
  public class MovieRecommender {
  
      private final Map<String, Integer> countOfMoviesPerCatalog;
  
      public MovieRecommender(
              @Value("#{{'Thriller': 100, 'Comedy': 300}}") Map<String, Integer> countOfMoviesPerCatalog) {
          this.countOfMoviesPerCatalog = countOfMoviesPerCatalog;
      }
  }
  ```

### @PostConstruct @PreDestroy

* 这两个注解也是 *CommonAnnotationBeanPostProcessor* 实现的
*  `@Resource`, the `@PostConstruct` and `@PreDestroy`  这个在 java9被移包,在11被分离需要手动引入javax.annotation-api



## 类路径扫描,容器管理

### Component

模板注解,@Repository`, `@Service`, and `@Controller是 它的特例,目前没有什么区别,以后可能会增加区别

### 使用元注解或组合注解

元注解

```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Component 
public @interface Service {

    // ...
}

Component 导致 service 跟 component同样对待
```

组合注解

@RestController等价于 @Controller` and `@ResponseBody

元注解可以重新申明 属性自定义属性的值

```java
@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Scope(WebApplicationContext.SCOPE_SESSION)
public @interface SessionScope {

    /**
     * Alias for {@link Scope#proxyMode}.
     * <p>Defaults to {@link ScopedProxyMode#TARGET_CLASS}.
     */
    @AliasFor(annotation = Scope.class)
    ScopedProxyMode proxyMode() default ScopedProxyMode.TARGET_CLASS;

}

@Service
@SessionScope
public class SessionScopedService {
    // ...
}
```

### 自动注册bean定义

spring会自动注册带有 模板注解component的类,并生成相应的bean定义 在applicationContext中

```java
@Service
public class SimpleMovieLister {

    private MovieFinder movieFinder;

    public SimpleMovieLister(MovieFinder movieFinder) {
        this.movieFinder = movieFinder;
    }
}
```

为了自动注册上面的类型必须 要在 带有 @Configuration的 类中 申明 ,包扫描,包之间可以用 空格, 逗号,冒号

```java
@Configuration
@ComponentScan(basePackages = "org.example")
public class AppConfig  {
    // ...
}

//@ComponentScan("org.example")简写形式

//xml 形式
 <context:component-scan base-package="org.example"/>
 它会激活 <context:annotation-config>注解
```

使用Ant编译工程时, you do not activate the files-only switch of the JAR task

同时 AutowiredAnnotationBeanPostProcessor` and `CommonAnnotationBeanPostProcessor这两个beanPostProcessor也会被注册

```
You can disable the registration of AutowiredAnnotationBeanPostProcessor and CommonAnnotationBeanPostProcessor by including the annotation-config attribute with a value of false.
```

### 使用过滤器 自定义扫描

* '@Component`, `@Repository`, `@Service`, `@Controller`, `@Configuration'默认只扫描带有这些注解的类 

* 通过ComponentScan的 includeFilters ,excludeFilters 属性 设定不同类型的过滤器去 取 或者排除 相应的类

* 过滤器的类型

  | ilter Type           | Example Expression           | Description                                                  |
  | :------------------- | :--------------------------- | :----------------------------------------------------------- |
  | annotation (default) | `org.example.SomeAnnotation` | 指定  注释类上或者元类上的注解类                             |
  | assignable           | `org.example.SomeClass`      | A class (or interface) that the target components are assignable to (extend or implement). |
  | aspectj              | `org.example..*Service+`     | 使用 aspectj注入语法                                         |
  | regex                | `org\.example\.Default.*`    | 使用正则匹配类全限定名                                       |
  | custom               | `org.example.MyTypeFilter`   | A custom implementation of the `org.springframework.core.type.TypeFilter` interface. |

* 使用

  ```java
  @Configuration
  @ComponentScan(basePackages = "org.example",
          includeFilters = @Filter(type = FilterType.REGEX, pattern = ".*Stub.*Repository"),
          excludeFilters = @Filter(Repository.class))
  public class AppConfig {
      ...
  }
  ```

  ```xml
  <beans>
      <context:component-scan base-package="org.example">
          <context:include-filter type="regex"
                  expression=".*Stub.*Repository"/>
          <context:exclude-filter type="annotation"
                  expression="org.springframework.stereotype.Repository"/>
      </context:component-scan>
  </beans>
  
  ```

  

* 'useDefaultFilters=false use-default-filters="false"' 可以使得系统不会自动 *`@Component`, `@Repository`, `@Service`, `@Controller`, `@RestController`, or `@Configuration`.* 扫描这些注解

### 使用component定义元数据

* 可以使用component定义元数据
* 方法级别的bean定义,类似于 提供一个工厂方法

```java
@Component
public class FactoryMethodComponent {

    @Bean
    @Qualifier("public")
    public TestBean publicInstance() {
        return new TestBean("publicInstance");
    }

    public void doWork() {
        // Component method implementation omitted
    }
}
```

```java
@Component
public class FactoryMethodComponent {

    private static int i;

    @Bean
    @Qualifier("public")
    public TestBean publicInstance() {
        return new TestBean("publicInstance");
    }

    // use of a custom qualifier and autowiring of method parameters
    @Bean
    protected TestBean protectedInstance(
            @Qualifier("public") TestBean spouse,
            @Value("#{privateInstance.age}") String country) {
        TestBean tb = new TestBean("protectedInstance", 1);
        tb.setSpouse(spouse);
        tb.setCountry(country);
        return tb;
    }

    @Bean
    private TestBean privateInstance() {
        return new TestBean("privateInstance", i++);
    }

    @Bean
    @RequestScope
    public TestBean requestScopedInstance() {
        return new TestBean("requestScopedInstance", 3);
    }
}
```

```java
@Component
public class FactoryMethodComponent {

    @Bean @Scope("prototype")
    public TestBean prototypeInstance(InjectionPoint injectionPoint) {
        return new TestBean("prototypeInstance for " + injectionPoint.getMember());
    }
}
```

As of Spring Framework 4.3,可以使用 InjectionPoint(DependencyDescriptor更详细的子类) 可以访问到请求注入点,当然这适用于 原型作用域,

* 在普通 component的 @bean与 @configuration的@Bean不同
  * configuration 中 cglib会对其增强, 通过 代理@bean方法的调用来创建元数据引用
  * component 下的@bean是正常的java语义
  * 由于CGLIB需要继承 该类,所以@bean方法不能是final和 private,可以设置成 static,这样避免spring拦截

### componet自动命名

* 自动命名取 短类名
* 如果命名重复  [`BeanNameGenerator`](https://docs.spring.io/spring-framework/docs/5.2.7.RELEASE/javadoc-api/org/springframework/beans/factory/support/BeanNameGenerator.html) 可以注册这个类实现自定义自动命名它的子类为,FullyQualifiedAnnotationBeanNameGenerator

```java
@Configuration
@ComponentScan(basePackages = "org.example", nameGenerator = MyNameGenerator.class)
public class AppConfig {
    // ...
}
```

```xml
<beans>
    <context:component-scan base-package="org.example"
        name-generator="org.example.MyNameGenerator" />
</beans>
```

### component设置作用域

```java
@Scope("prototype")
@Repository
public class MovieFinderImpl implements MovieFinder {
    // ...
}
```

扫描特定作用域

实现 [`ScopeMetadataResolver`](https://docs.spring.io/spring-framework/docs/5.2.7.RELEASE/javadoc-api/org/springframework/context/annotation/ScopeMetadataResolver.html) 

```java
@Configuration
@ComponentScan(basePackages = "org.example", scopeResolver = MyScopeResolver.class)
public class AppConfig {
    // ...
}
```

扫描特定 作用域代理方式

```java
@Configuration
@ComponentScan(basePackages = "org.example", scopedProxy = ScopedProxyMode.INTERFACES)
public class AppConfig {
    // ...
}
```

### Qualifier标注Component

```java
@Component
@Qualifier("Action")
public class ActionMovieCatalog implements MovieCatalog {
    // ...
}
```



### 产生候选组件索引

```xml
<dependencies>
    <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-context-indexer</artifactId>
        <version>5.2.7.RELEASE</version>
        <optional>true</optional>
    </dependency>
</dependencies>
```

在编译期产生 候选者的索引,可以避免在类路径扫描,提升查找速度

```kotlin
dependencies {
    compileOnly "org.springframework:spring-context-indexer:5.2.7.RELEASE"
}
```

* 会产生  META-INF/spring.components 文件
* spring-context-indexer必须要注册到容器中来
* 如果类路径下 META-INF/spring.components 有这个文件,且有相关依赖,则该特性会被激活,spring.index.ignore可以关闭

## 使用JSR330标准注解

* 需要引入

  ```xml
  <dependency>
      <groupId>javax.inject</groupId>
      <artifactId>javax.inject</artifactId>
      <version>1</version>
  </dependency>
  ```

### 依赖注入:Inject,Named

```java
import javax.inject.Inject;

public class SimpleMovieLister {

    private MovieFinder movieFinder;

    @Inject
    public void setMovieFinder(MovieFinder movieFinder) {
        this.movieFinder = movieFinder;
    }

    public void listMovies() {
        this.movieFinder.findMovies(...);
        // ...
    }
}
```

### provider注入

可以注入 Provider包装的类, 提供懒加载,按需加载

```java
import javax.inject.Inject;
import javax.inject.Provider;

public class SimpleMovieLister {

    private Provider<MovieFinder> movieFinder;

    @Inject
    public void setMovieFinder(Provider<MovieFinder> movieFinder) {
        this.movieFinder = movieFinder;
    }

    public void listMovies() {
        this.movieFinder.get().findMovies(...);
        // ...
    }
}
```

### Optional注入

注入Optional 包装类,或者使用@nullable

```jav
public class SimpleMovieLister {

    @Inject
    public void setMovieFinder(Optional<MovieFinder> movieFinder) {
        // ...
    }
}
```

### named注入

名称注入named

```java
import javax.inject.Inject;
import javax.inject.Named;

public class SimpleMovieLister {

    private MovieFinder movieFinder;

    @Inject
    public void setMovieFinder(@Named("main") MovieFinder movieFinder) {
        this.movieFinder = movieFinder;
    }

    // ...
}
```

###  @named,@ManagedBean

与Component相同的 @named,@ManagedBean,二者不可组合

```java
import javax.inject.Inject;
import javax.inject.Named;

@Named("movieListener")  // @ManagedBean("movieListener") could be used as well
public class SimpleMovieLister {

    private MovieFinder movieFinder;

    @Inject
    public void setMovieFinder(MovieFinder movieFinder) {
        this.movieFinder = movieFinder;
    }

    // ...
}

import javax.inject.Inject;
import javax.inject.Named;

@Named
public class SimpleMovieLister {

    private MovieFinder movieFinder;

    @Inject
    public void setMovieFinder(MovieFinder movieFinder) {
        this.movieFinder = movieFinder;
    }

    // ...
}
```

### JSR330注解限制

| Spring              | javax.inject.*        | javax.inject restrictions / comments                         |
| :------------------ | :-------------------- | :----------------------------------------------------------- |
| @Autowired          | @Inject               | `@Inject` has no 'required' attribute. Can be used with Java 8’s `Optional` instead. |
| @Component          | @Named / @ManagedBean | JSR-330 does not provide a composable model, only a way to identify named components. |
| @Scope("singleton") | @Singleton            | The JSR-330 default scope is like Spring’s `prototype`. However, in order to keep it consistent with Spring’s general defaults, a JSR-330 bean declared in the Spring container is a `singleton` by default. In order to use a scope other than `singleton`, you should use Spring’s `@Scope` annotation. `javax.inject` also provides a [@Scope](https://download.oracle.com/javaee/6/api/javax/inject/Scope.html) annotation. Nevertheless, this one is only intended to be used for creating your own annotations. |
| @Qualifier          | @Qualifier / @Named   | `javax.inject.Qualifier` is just a meta-annotation for building custom qualifiers. Concrete `String` qualifiers (like Spring’s `@Qualifier` with a value) can be associated through `javax.inject.Named`. |
| @Value              | -                     | no equivalent                                                |
| @Required           | -                     | no equivalent                                                |
| @Lazy               | -                     | no equivalent                                                |
| ObjectFactory       | Provider              | `javax.inject.Provider` is a direct alternative to Spring’s `ObjectFactory`, only with a shorter `get()` method name. It can also be used in combination with Spring’s `@Autowired` or with non-annotated constructors and setter methods. |

## 基于Java的注解.

### 实例化 注解配置容器

`AnnotationConfigApplicationContext` 

* 这个通用的applicationContext可以接受@configuration的配置,也可以接受@Component的注解
* 带有@configuration注解的类被解析成 bean定义,@bean也会被解析成bean定义

实例化

```java
public static void main(String[] args) {
    ApplicationContext ctx = new AnnotationConfigApplicationContext(AppConfig.class);
    MyService myService = ctx.getBean(MyService.class);
    myService.doStuff();
}

public static void main(String[] args) {
    ApplicationContext ctx = new AnnotationConfigApplicationContext(MyServiceImpl.class, Dependency1.class, Dependency2.class);
    MyService myService = ctx.getBean(MyService.class);
    myService.doStuff();
}
```

编程方式实例化

```java
public static void main(String[] args) {
    AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext();
    ctx.register(AppConfig.class, OtherConfig.class);
    ctx.register(AdditionalConfig.class);
    ctx.refresh();
    MyService myService = ctx.getBean(MyService.class);
    myService.doStuff();
}
```

扫描

```java
public static void main(String[] args) {
    AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext();
    ctx.scan("com.acme");
    ctx.refresh();
    MyService myService = ctx.getBean(MyService.class);
}

@Configuration
@ComponentScan(basePackages = "com.acme") 
public class AppConfig  {
    ...
}
```

web应用 `AnnotationConfigWebApplicationContext`

* 'WebApplicationContext'的变体 `AnnotationConfigWebApplicationContext` 来配置spring的 ContextLoaderListener servlet
* Spring MVC `DispatcherServlet`

```xml
<web-app>
    <!-- Configure ContextLoaderListener to use AnnotationConfigWebApplicationContext
        instead of the default XmlWebApplicationContext -->
    <context-param>
        <param-name>contextClass</param-name>
        <param-value>
            org.springframework.web.context.support.AnnotationConfigWebApplicationContext
        </param-value>
    </context-param>

    <!-- Configuration locations must consist of one or more comma- or space-delimited
        fully-qualified @Configuration classes. Fully-qualified packages may also be
        specified for component-scanning -->
    <context-param>
        <param-name>contextConfigLocation</param-name>
        <param-value>com.acme.AppConfig</param-value>
    </context-param>

    <!-- Bootstrap the root application context as usual using ContextLoaderListener -->
    <listener>
        <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
    </listener>

    <!-- Declare a Spring MVC DispatcherServlet as usual -->
    <servlet>
        <servlet-name>dispatcher</servlet-name>
        <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
        <!-- Configure DispatcherServlet to use AnnotationConfigWebApplicationContext
            instead of the default XmlWebApplicationContext -->
        <init-param>
            <param-name>contextClass</param-name>
            <param-value>
                org.springframework.web.context.support.AnnotationConfigWebApplicationContext
            </param-value>
        </init-param>
        <!-- Again, config locations must consist of one or more comma- or space-delimited
            and fully-qualified @Configuration classes -->
        <init-param>
            <param-name>contextConfigLocation</param-name>
            <param-value>com.acme.web.MvcConfig</param-value>
        </init-param>
    </servlet>

    <!-- map all requests for /app/* to the dispatcher servlet -->
    <servlet-mapping>
        <servlet-name>dispatcher</servlet-name>
        <url-pattern>/app/*</url-pattern>
    </servlet-mapping>
</web-app>
```

### @Bean注解

#### 生命周期回调

* '@PostConstruct` and `@PreDestroy' 构造器调用完后,setter注入前,  销毁前
* 支持 spring常规的 回调'InitializingBean`, `DisposableBean`, or `Lifecycle' 
* `*Aware` interfaces 注入接口回调
* 支持 init-method` and `destroy-method 属性

* 关闭生命周期回调

  ```java
  @Bean(destroyMethod="")
  public DataSource dataSource() throws NamingException {
      return (DataSource) jndiTemplate.lookup("MyDS");
  }
  ```

#### 指定scope域

```java
// an HTTP Session-scoped bean exposed as a proxy
@Bean
@SessionScope
public UserPreferences userPreferences() {
    return new UserPreferences();
}

@Bean
public Service userService() {
    UserService service = new SimpleUserService();
    // a reference to the proxied userPreferences bean
    service.setUserPreferences(userPreferences());
    return service;
}
```

#### bean别名

```java
@Configuration
public class AppConfig {

    @Bean({"dataSource", "subsystemA-dataSource", "subsystemB-dataSource"})
    public DataSource dataSource() {
        // instantiate, configure and return DataSource bean...
    }
}
```

#### bean描述

```java
@Configuration
public class AppConfig {

    @Bean
    @Description("Provides a basic example of a bean")
    public Thing thing() {
        return new Thing();
    }
}
```

### @import

```java
@Configuration
public class ConfigA {

    @Bean
    public A a() {
        return new A();
    }
}

@Configuration
@Import(ConfigA.class)
public class ConfigB {

    @Bean
    public B b() {
        return new B();
    }
}


public static void main(String[] args) {
    ApplicationContext ctx = new AnnotationConfigApplicationContext(ConfigB.class);

    // now both beans A and B will be available...
    A a = ctx.getBean(A.class);
    B b = ctx.getBean(B.class);
}
```

这样只用引入 ConfigB就可以同时引入ConfigA

### 条件性的包含bean

[`@Conditional`](https://docs.spring.io/spring-framework/docs/5.2.7.RELEASE/javadoc-api/org/springframework/context/annotation/Conditional.html).

实现Conditional接口

@Profile的实现

```java
@Override
public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
    // Read the @Profile annotation attributes
    MultiValueMap<String, Object> attrs = metadata.getAllAnnotationAttributes(Profile.class.getName());
    if (attrs != null) {
        for (Object value : attrs.get("value")) {
            if (context.getEnvironment().acceptsProfiles(((String[]) value))) {
                return true;
            }
        }
        return false;
    }
    return true;
}
```

Condition接口

```java
boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata);
```

### Java配置与XML配置结合

* 以Java为中心的配置 AnnotationConfigApplicationContext and the  @ImportResource 导入xml

```javascript
@Configuration
@ImportResource("classpath:/com/acme/properties-config.xml")
public class AppConfig {

    @Value("${jdbc.url}")
    private String url;

    @Value("${jdbc.username}")
    private String username;

    @Value("${jdbc.password}")
    private String password;

    @Bean
    public DataSource dataSource() {
        return new DriverManagerDataSource(url, username, password);
    }
}
```

## 环境抽象

spring对环境的抽象建模 主要是 两块: properties 和 profile

* profile的含义是 条件选择

* properties (包括配置文件,系统属性,系统环境变量,JNDI,servletContext参数,等等)

### profile

```java
@Configuration
@Profile("development")
public class StandaloneDataConfig {

    @Bean
    public DataSource dataSource() {
        return new EmbeddedDatabaseBuilder()
            .setType(EmbeddedDatabaseType.HSQL)
            .addScript("classpath:com/bank/config/sql/schema.sql")
            .addScript("classpath:com/bank/config/sql/test-data.sql")
            .build();
    }
}
```

profile名称支持 如下语法

- `!`: A logical “not” of the profile
- `&`: A logical “and” of the profiles
- `|`: A logical “or” of the profiles

可以自定义注解

```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Profile("production")
public @interface Production {
}
```

@Profile({"p1", "!p2"}) {} 标识 或逻辑

基于xml的配置

```xml
<beans profile="development"
    xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:jdbc="http://www.springframework.org/schema/jdbc"
    xsi:schemaLocation="...">

    <jdbc:embedded-database id="dataSource">
        <jdbc:script location="classpath:com/bank/config/sql/schema.sql"/>
        <jdbc:script location="classpath:com/bank/config/sql/test-data.sql"/>
    </jdbc:embedded-database>
</beans>

<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:jdbc="http://www.springframework.org/schema/jdbc"
    xmlns:jee="http://www.springframework.org/schema/jee"
    xsi:schemaLocation="...">

    <!-- other bean definitions -->

    <beans profile="development">
        <jdbc:embedded-database id="dataSource">
            <jdbc:script location="classpath:com/bank/config/sql/schema.sql"/>
            <jdbc:script location="classpath:com/bank/config/sql/test-data.sql"/>
        </jdbc:embedded-database>
    </beans>

    <beans profile="production">
        <jee:jndi-lookup id="dataSource" jndi-name="java:comp/env/jdbc/datasource"/>
    </beans>
</beans>
```

### 激活profile

通过编程的方式 ,使用Environment接口 通过容器

```java
AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext();
ctx.getEnvironment().setActiveProfiles("development");
ctx.register(SomeConfig.class, StandaloneDataConfig.class, JndiDataConfig.class);
ctx.refresh();
```

使用注解 @ActiveProfiles

使用变量名spring.profiles.active

```
  -Dspring.profiles.active="profile1,profile2"
```

spring.profiles.default设置默认环境变量

```java
@Configuration
@Profile("default")
public class DefaultDataConfig {

    @Bean
    public DataSource dataSource() {
        return new EmbeddedDatabaseBuilder()
            .setType(EmbeddedDatabaseType.HSQL)
            .addScript("classpath:com/bank/config/sql/schema.sql")
            .build();
    }
}
```

### PropertiesSource

* propertiesSource是 spring对环境变量的抽象,基于键值对的抽象

* standardEnvironment 是包含两个 properties Source 

  * JVM system properties (`System.getProperties()`
  * system environment variables (`System.getenv()`).

* StandardServletEnvrionment 包含 servletconfig,servletContext参数,可选的JNDIPropertySource

* 环境变量的查找有层级优先级,以StandardServletEnvrionment 为例

  * ServletConfig parameters (if applicable — for example, in case of a `DispatcherServlet` context)
  * ServletContext parameters (web.xml context-param entries)
  * JNDI environment variables (`java:comp/env/` entries)
  * JVM system properties (`-D` command-line arguments)
  * JVM system environment (operating system environment variables)

* 以上查找机制是可配的,可自定义

  ```java
  ConfigurableApplicationContext ctx = new GenericApplicationContext();
  MutablePropertySources sources = ctx.getEnvironment().getPropertySources();
  sources.addFirst(new MyPropertySource());
  ```

  经过以上配置,可以注册自定义的MypropertySource 并且由先先级是最高

### @PropertySource

```java
@Configuration
@PropertySource("classpath:/com/myco/app.properties")
public class AppConfig {

    @Autowired
    Environment env;

    @Bean
    public TestBean testBean() {
        TestBean testBean = new TestBean();
        testBean.setName(env.getProperty("testbean.name"));
        return testBean;
    }
}
```

```java
@Configuration
@PropertySource("classpath:/com/${my.placeholder:default/path}/app.properties")
public class AppConfig {

    @Autowired
    Environment env;

    @Bean
    public TestBean testBean() {
        TestBean testBean = new TestBean();
        testBean.setName(env.getProperty("testbean.name"));
        return testBean;
    }
}
```

my.placeholder是其他已经定义过的 属性,default/path是找不到数据源使用默认的属性

### 占位符解析

环境变量贯穿整个容器,只要在定义之前这个变量已经被注册进去就可以

```xml
<beans>
    <import resource="com/bank/service/${customer}-config.xml"/>
</beans>
```

##  Registering a LoadTimeWeaver

当类被装载进虚拟机时,动态的转换类

```java
@Configuration
@EnableLoadTimeWeaving
public class AppConfig {
}
```

```xml
<beans>
    <context:load-time-weaver/>
</beans>
```



## ApplicationContext额外的功能

* *org.springframework.beans.factory* 提供基本的管理和维护bean的功能,org.springframework.context 添加了[`ApplicationContext`](https://docs.spring.io/spring-framework/docs/5.2.7.RELEASE/javadoc-api/org/springframework/context/ApplicationContext.html)接口不仅扩展了beanFactory接口,也提供了其他功能
* 大部分应用程序以声明的方式 使用ApplicationContext,例如依赖 ContextLoader的类 
* 额外的功能如下
  * *i18n* 风格的 消息访问,  MessageSource
  * 访问URL资源: ResourceLoader
  * 时间发布: ApplicationListener,ApplicationEventPublisher
  * 带有层次接口的多上下文,每个applicationContext只关注特定的层, 例如web层, 通过 HierarchicalBeanFactory接口

### 使用 `MessageSource` 国际化

```java
String getMessage(String code, @Nullable Object[] args, @Nullable String defaultMessage, Locale locale);
```

根据Loclae,  code对应的 带参数的message

1. applicationContext首先会查找当前容器中有没有 messageSource的bean名
2. 如果找到了则使用该 bean作为 消息源
3. 如果没找到 则往父 bean找,如果还是找不到则 在 messageSource代理类 *DelegatingMessageSource* 设置空的source

4. spring提供了 两个 messageSource
   1. *ResourceBundleMessageSource*
   2. *StaticMessageSource*
   3. 都继承于HierarchicalMessageSource(为了处理 嵌套的消息)
   
   4. 名称要覆盖messageSource
   5. basename,即资源包的名称,会默认去类路径下查找:  classpath:basename.properties, basename-en.properties,等不同地域的文件

```
    @Bean("messageSource")
    public  ResourceBundleMessageSource resourceBundleMessageSource(){
        ResourceBundleMessageSource resourceBundleMessageSource = new ResourceBundleMessageSource();
        resourceBundleMessageSource.setAlwaysUseMessageFormat(true);
        resourceBundleMessageSource.setBasenames("format","exceptions","windows","messages");
        return resourceBundleMessageSource;
    }
    
```

* 还有可重载的 *ResourceBundleMessageSource* : ReloadableResourceBundleMessageSource
  * 允许从spring 任意的 location 加载文件
  * 支持热加载



### 标准事件和自定义事件

#### spring提供的内置事件

| Event                        | Explanation                                                  |
| :--------------------------- | :----------------------------------------------------------- |
| `ContextRefreshedEvent`      | 1. 当`ApplicationContext`被初始化或者被刷新时(例如调用`refresh) 2. 在容器关闭前,context可以被刷新任意次 |
| `ContextStartedEvent`        | Published when the `ApplicationContext` is started by using the `start()` method on the `ConfigurableApplicationContext` interface |
| `ContextStoppedEvent`        | Published when the `ApplicationContext` is stopped by using the `stop()` method on the `ConfigurableApplicationContext` interface. Here, “stopped” means that all `Lifecycle` beans receive an explicit stop signal. A stopped context may be restarted through a `start()` call. |
| `ContextClosedEvent`         | Published when the `ApplicationContext` is being closed by using the `close()` method on the `ConfigurableApplicationContext` interface or via a JVM shutdown hook. Here, "closed" means that all singleton beans will be destroyed. Once the context is closed, it reaches its end of life and cannot be refreshed or restarted. |
| `RequestHandledEvent`        | A web-specific event telling all beans that an HTTP request has been serviced. This event is published after the request is complete. This event is only applicable to web applications that use Spring’s `DispatcherServlet`. |
| `ServletRequestHandledEvent` | A subclass of `RequestHandledEvent` that adds Servlet-specific context information |

#### 自定义事件

通过ApplicationEvent自定义事件发布,ApplicationListener 自定义事件接收, ApplicationEventPublisher在容器bean中发布事件

```java
//定义事件
public class BlackListEvent extends ApplicationEvent {

    private final String address;
    private final String content;

    public BlackListEvent(Object source, String address, String content) {
        super(source);
        this.address = address;
        this.content = content;
    }

    // accessor and other methods...
}
//定义 发布事件的服务
public class EmailService implements ApplicationEventPublisherAware {

    private List<String> blackList;
    private ApplicationEventPublisher publisher;

    public void setBlackList(List<String> blackList) {
        this.blackList = blackList;
    }

    public void setApplicationEventPublisher(ApplicationEventPublisher publisher) {
        this.publisher = publisher;
    }

    public void sendEmail(String address, String content) {
        if (blackList.contains(address)) {
            publisher.publishEvent(new BlackListEvent(this, address, content));
            return;
        }
        // send email...
    }
}
//定义监听该事件的类
public class BlackListNotifier implements ApplicationListener<BlackListEvent> {

    private String notificationAddress;

    public void setNotificationAddress(String notificationAddress) {
        this.notificationAddress = notificationAddress;
    }

    public void onApplicationEvent(BlackListEvent event) {
        // notify appropriate parties via notificationAddress...
    }
}
```

#### 注意事项

* 该事件发布是同步的,会等待所有监听者 处理完事件才会返回,比较利于事务

* 另外一个事件发布的策略:异步多播 [`ApplicationEventMulticaster`](https://docs.spring.io/spring-framework/docs/5.2.7.RELEASE/javadoc-api/org/springframework/context/event/ApplicationEventMulticaster.html) interface and [`SimpleApplicationEventMulticaster`](https://docs.spring.io/spring-framework/docs/5.2.7.RELEASE/javadoc-api/org/springframework/context/event/SimpleApplicationEventMulticaster.html)

#### 基于注解的事件监听

```java
public class BlackListNotifier {

    private String notificationAddress;

    public void setNotificationAddress(String notificationAddress) {
        this.notificationAddress = notificationAddress;
    }

    @EventListener
    public void processBlackListEvent(BlackListEvent event) {
        // notify appropriate parties via notificationAddress...
    }
}

//多事件 监听
@EventListener({ContextStartedEvent.class, ContextRefreshedEvent.class})
public void handleContextStart() {
    // ...
}

//运行时对事件 过滤
@EventListener(condition = "#blEvent.content == 'my-event'")
public void processBlackListEvent(BlackListEvent blEvent) {
    // notify appropriate parties via notificationAddress...
}

//处理完事件后 发布另一事件,可以通过集合发布多个事件
@EventListener
public ListUpdateEvent handleBlackListEvent(BlackListEvent event) {
    // notify appropriate parties via notificationAddress and
    // then publish a ListUpdateEvent...
}

//异步事件处理
@EventListener
@Async
public void processBlackListEvent(BlackListEvent event) {
    // BlackListEvent is processed in a separate thread
}
异步事件的限制
如果异步调用发生异常,不会传给调用者,详见AsyncUncaughtExceptionHandler 
无法通过返回值 发布事件,只能手动 注入ApplicationEventPublisher,发布
    
//可排序的监听器执行    
@EventListener
@Order(42)
public void processBlackListEvent(BlackListEvent event) {
    // notify appropriate parties via notificationAddress...
}    

//基于泛型的 监听器选择
@EventListener
public void onPersonCreated(EntityCreatedEvent<Person> event) {
    // ...
}
只会选择 Person类型的监听器
前提是 此类已将泛型具体化

//可以使用 ResolveableTypeProvider 来让spring自动识别解析类型
public class EntityCreatedEvent<T> extends ApplicationEvent implements ResolvableTypeProvider {

    public EntityCreatedEvent(T entity) {
        super(entity);
    }

    @Override
    public ResolvableType getResolvableType() {
        return ResolvableType.forClassWithGenerics(getClass(), ResolvableType.forInstance(getSource()));
    }
}
```

el表达式有专门语义环境的变量

| Name            | Location           | Description                                                  | Example                                                      |
| :-------------- | :----------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| Event           | root object        | The actual `ApplicationEvent`.                               | `#root.event` or `event`                                     |
| Arguments array | root object        | The arguments (as an object array) used to invoke the method. | `#root.args` or `args`; `args[0]` to access the first argument, etc. |
| *Argument name* | evaluation context | The name of any of the method arguments. If, for some reason, the names are not available (for example, because there is no debug information in the compiled byte code), individual arguments are also available using the `#a<#arg>` syntax where `<#arg>` stands for the argument index (starting from 0). | `#blEvent` or `#a0` (you can also use `#p0` or `#p<#arg>` parameter notation as an alias) |

### 资源访问

*  application context 是一个ResourceLoader,可以导入resource对象
* resource对象本质上是一个 更加版本丰富的 java.net.URL
* Resource对象可以以间接的方式,透明的从大多数路径 下获取资源
* 包括 类路径,文件系统路径,任何以URL标准形式的路径
* 如果一个资源 没有指定任何  资源前缀, 则默认是 application context type
* 提供给 ApplicationContext构造函数的 路径, 以string格式,根据实现的不同会当作不同路径的资源,例如(ClassPathXmlApplicationContext) 类路径下的
* 通过ResourceLoaderAware 注入 ResourceLoader,直接访问资源文件

### 访问ApplicationContext

* 通过申明式创建 ApplicationContext

  ```java
  <context-param>
      <param-name>contextConfigLocation</param-name>
      <param-value>/WEB-INF/daoContext.xml /WEB-INF/applicationContext.xml</param-value>
  </context-param>
  
  <listener>
      <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
  </listener>
  1. 监听器检测contextConfigLocation  参数下的文件
  2. 如果参数不存在,取默认/WEB-INF/applicationContext.xml
  3. 文件之间使用 : ; 空格等分隔
  4. 支持ANtPath风格
  ```

### springContextRAR部署

Deploying a Spring `ApplicationContext` as a Java EE RAR File

基于RAR的springContext部署

不需要web端

[`SpringContextResourceAdapter`](https://docs.spring.io/spring-framework/docs/5.2.7.RELEASE/javadoc-api/org/springframework/jca/context/SpringContextResourceAdapter.html)



## `BeanFactory`

*BeanFactory* 极其相关接口(BeanFactoryAware`, `InitializingBean`, `DisposableBean)  是集成第三方组件 的接入点,可以不需要注解或者反射,就能有效的使第三方组件与容器沟通

### `BeanFactory` or `ApplicationContext`

这两者的容器级别 与 对启动的影响

*ApplicationContext* 子类:GenericApplicationContext,AnnotationConfigApplicationContext 主要完成的工作

* 配置文件加载
* 类路径扫描
* 注册bean定义
*  (as of 5.0) registering functional bean definitions.



*ApplicationContext*  包含了所有 beanfactory的功能,除非想完全把控bean的处理过程

* *简单实现* DefaultListableBeanFactory 不会检测到  [`BeanPostProcessor`](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#beans-factory-extension-bpp) 



beanfactory与applicationContext对比

| Feature                                                 | `BeanFactory` | `ApplicationContext` |
| :------------------------------------------------------ | :------------ | :------------------- |
| Bean instantiation/wiring                               | Yes           | Yes                  |
| Integrated lifecycle management                         | No            | Yes                  |
| Automatic `BeanPostProcessor` registration              | No            | Yes                  |
| Automatic `BeanFactoryPostProcessor` registration       | No            | Yes                  |
| Convenient `MessageSource` access (for internalization) | No            | Yes                  |
| Built-in `ApplicationEvent` publication mechanism       | No            | Yes                  |

{% raw %}



# Resource

* 本章节介绍了spring如何处理资源,如何在spring中使用资源
* 对Java URL类的封装,提供了更强大的功能 ,推荐在自己代码中使用

## 内置的Resource实现

UrlResource

* UrlResource 包装了java.net.URL ,可以用来访问 Http,Ftp,file文件系统的访问
* 会解析 已知的关键字 classpath

ClassPathResource

* 加载类路径的资源文件
* 要么使用当前线程的类加载器或者给定类加载器,或者 指定类

FileSystemResource

ServletContextResource

* ServletContext*的实现,从web跟目录的相对路径加载文件



## ResourceLoader

* 用于加载resource的类

* 所有application context 实现了该接口,对于不同的容器返回不同类型的 Resource

  ```java
  Resource template = ctx.getResource("some/resource/path/myTemplate.txt");
  ```

  * *ClassPathXmlApplicationContext* 返回 ClassPathResource

  * *FileSystemXmlApplicationContext* 返回FileSystemResource

  * WebApplicationContext 返回ServletContextResource

  * 可以强制返回指定类型的资源

    ```java
    //返回类路径下的资源
    Resource template = ctx.getResource("classpath:some/resource/path/myTemplate.txt");
    //返回文件系统下的资源
    Resource template = ctx.getResource("file:///some/resource/path/myTemplate.txt");
    //返回http资源
    Resource template = ctx.getResource("https://myhost.com/resource/path/myTemplate.txt");
    ```

  显示指定前缀

| Prefix     | Example                          | Explanation                                                  |
| :--------- | :------------------------------- | :----------------------------------------------------------- |
| classpath: | `classpath:com/myapp/config.xml` | Loaded from the classpath.                                   |
| file:      | `file:///data/config.xml`        | Loaded as a `URL` from the filesystem. See also [`FileSystemResource` Caveats](https://docs.spring.io/spring/docs/5.2.7.RELEASE/spring-framework-reference/core.html#resources-filesystemresource-caveats). |
| http:      | `https://myserver/logo.png`      | Loaded as a `URL`.                                           |
| (none)     | `/data/config.xml`               | Depends on the underlying `ApplicationContext`.              |

## ResourceLoaderAware

* 申明注入resourceloader的接口
* 因为所有 ApplicationContext 都实现了resourceloader 所以可以使用他 来加载资源

## 依赖注入资源属性

* 使用PropertyEditor 注入自定义的属性文件,

  ```xml
  //注入的路径取决于 你的 applicationContext的类型,可以指定前缀来使用指定资源
  <bean id="myBean" class="...">
      <property name="template" value="some/resource/path/myTemplate.txt"/>
  </bean>
  
  ```

  



## 使用resource创建 applicationContext

```java
ApplicationContext ctx = new ClassPathXmlApplicationContext("conf/appContext.xml");

ApplicationContext ctx =
    new FileSystemXmlApplicationContext("conf/appContext.xml");
```

```java
com/
  foo/
    services.xml
    daos.xml
    MessengerService.class
    
使用上述路径的资源加载文件
ApplicationContext ctx = new ClassPathXmlApplicationContext(
    new String[] {"services.xml", "daos.xml"}, MessengerService.class);
```



资源加载中的通配符



## 文件系统资源使用 警告

为了向后兼容的原因 spring使用 FileSystemApplicationContext 时,会通通当成相对路径,

```java
这两个示例 时相等价的
ApplicationContext ctx =
    new FileSystemXmlApplicationContext("conf/context.xml");
    
ApplicationContext ctx =
    new FileSystemXmlApplicationContext("/conf/context.xml");
```



# 验证,数据绑定,类型转换

Validation, Data Binding, and Type Conversion



* *Validator* 与 *DataBinder* 组成 validation 包
* *BeanWrapper* 是spring非常基础的概念,在很多地方使用到
* *DataBinder* 和BeanWrapper 都使用 PropertyEditorSupport 的实现去 解析和格式化属性值



## spring validtor使用示例

```java
public class PersonValidator implements Validator {

    /**
     * This Validator validates only Person instances
     */
    public boolean supports(Class clazz) {
        return Person.class.equals(clazz);
    }

    public void validate(Object obj, Errors e) {
        ValidationUtils.rejectIfEmpty(e, "name", "name.empty");
        Person p = (Person) obj;
        if (p.getAge() < 0) {
            e.rejectValue("age", "negativevalue");
        } else if (p.getAge() > 110) {
            e.rejectValue("age", "too.darn.old");
        }
    }
}
```



## Resolving Codes to Error Messages

* 使用前面的验证器产生的错误消息,可以通过 *MessageCodesResolver* 解析code
* 例如默认的实现 DefaultMessageCodesResolver ,调用 rejectValue("age", "too.darn.old") 方法会在resource中注册 *too.darn.old*  *too.darn.old.age*,*too.darn.old.age.int*



## *Bean Manipulation and the `BeanWrapper`*

* BeanWrapper* 和它的实现 BeanWrapperImpl 可以批量设置或者读取 JavaBean属性
* 支持嵌套属性
* 能够添加 PropertyChangeListeners,VetoableChangeListeners
* 支持字段索引
* Bean Wrapper通常是 DataBinder和 BeanFactory 使用,不是给应用程序直接使用

### 访问语法

| Expression             | Explanation                                                  |
| :--------------------- | :----------------------------------------------------------- |
| `name`                 | javabean的属性名                                             |
| `account.name`         | 嵌套的属性名`getAccount().setName()` or `getAccount().getName()` methods. |
| `account[2]`           | 集合索引Indicates the *third* element of the indexed property `account`. Indexed properties can be of type `array`, `list`, or other naturally ordered collection. |
| `account[COMPANYNAME]` | map索引                                                      |



### 如何使用 beanWrapper

```java
public class Company {

    private String name;
    private Employee managingDirector;

    public String getName() {
        return this.name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Employee getManagingDirector() {
        return this.managingDirector;
    }

    public void setManagingDirector(Employee managingDirector) {
        this.managingDirector = managingDirector;
    }
}


public class Employee {

    private String name;

    private float salary;

    public String getName() {
        return this.name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public float getSalary() {
        return salary;
    }

    public void setSalary(float salary) {
        this.salary = salary;
    }
}
//使用
BeanWrapper company = new BeanWrapperImpl(new Company());
// setting the company name..
company.setPropertyValue("name", "Some Company Inc.");
// ... can also be done like this:
PropertyValue value = new PropertyValue("name", "Some Company Inc.");
company.setPropertyValue(value);

// ok, let's create the director and tie it to the company:
BeanWrapper jim = new BeanWrapperImpl(new Employee());
jim.setPropertyValue("name", "Jim Stravinsky");
company.setPropertyValue("managingDirector", jim.getWrappedInstance());

// retrieving the salary of the managingDirector through the company
Float salary = (Float) company.getPropertyValue("managingDirector.salary");
```



### *PropertyEditor* 内置的beanwrapper的实现

* spring使用 PropertyEditor 在 string名称的属性,与 对象 之间 转换
* springBean wrapper有两个使用场景
  * 通过使用 PropertyEditor 的实现 来实现 设置 bean,例如基于xml的依赖注入
  * spring MVC的Http参数解析中绑定,使用各种各样的 PropertyEditor
  * 您可以在CommandController的所有子类中手动绑定这些实现。
* spring 有很多 PropertyEditor 在 org.springframework.beans.propertyeditors包中,大部分由 BeanWrapperImpl 默认注册

| Class                     | Explanation                                                  |
| :------------------------ | :----------------------------------------------------------- |
| `ByteArrayPropertyEditor` | Editor for byte arrays. Converts strings to their corresponding byte representations. Registered by default by `BeanWrapperImpl`. |
| `ClassEditor`             | Parses Strings that represent classes to actual classes and vice-versa. When a class is not found, an `IllegalArgumentException` is thrown. By default, registered by `BeanWrapperImpl`. |
| `CustomBooleanEditor`     | Customizable property editor for `Boolean` properties. By default, registered by `BeanWrapperImpl` but can be overridden by registering a custom instance of it as a custom editor. |
| `CustomCollectionEditor`  | Property editor for collections, converting any source `Collection` to a given target `Collection` type. |
| `CustomDateEditor`        | Customizable property editor for `java.util.Date`, supporting a custom `DateFormat`. NOT registered by default. Must be user-registered with the appropriate format as needed. |
| `CustomNumberEditor`      | Customizable property editor for any `Number` subclass, such as `Integer`, `Long`, `Float`, or `Double`. By default, registered by `BeanWrapperImpl` but can be overridden by registering a custom instance of it as a custom editor. |
| `FileEditor`              | Resolves strings to `java.io.File` objects. By default, registered by `BeanWrapperImpl`. |
| `InputStreamEditor`       | One-way property editor that can take a string and produce (through an intermediate `ResourceEditor` and `Resource`) an `InputStream` so that `InputStream` properties may be directly set as strings. Note that the default usage does not close the `InputStream` for you. By default, registered by `BeanWrapperImpl`. |
| `LocaleEditor`            | Can resolve strings to `Locale` objects and vice-versa (the string format is `*[country]*[variant]`, same as the `toString()` method of `Locale`). By default, registered by `BeanWrapperImpl`. |
| `PatternEditor`           | Can resolve strings to `java.util.regex.Pattern` objects and vice-versa. |
| `PropertiesEditor`        | Can convert strings (formatted with the format defined in the javadoc of the `java.util.Properties` class) to `Properties` objects. By default, registered by `BeanWrapperImpl`. |
| `StringTrimmerEditor`     | Property editor that trims strings. Optionally allows transforming an empty string into a `null` value. NOT registered by default — must be user-registered. |
| `URLEditor`               | Can resolve a string representation of a URL to an actual `URL` object. By default, registered by `BeanWrapperImpl`. |



* 通过使用 java.beans.PropertyEditorManager 为  PropertyEditor 设置 搜索路径
* 搜索路径默认 包括 *sun.bean.editors* 
* JavaBean对象 自动发现 与他同名同包的 PropertyEditor 例如 *SomethingEditor* 与 *Something*

```java
public class SomethingBeanInfo extends SimpleBeanInfo {

    public PropertyDescriptor[] getPropertyDescriptors() {
        try {
            final PropertyEditor numberPE = new CustomNumberEditor(Integer.class, true);
            PropertyDescriptor ageDescriptor = new PropertyDescriptor("age", Something.class) {
                public PropertyEditor createPropertyEditor(Object bean) {
                    return numberPE;
                };
            };
            return new PropertyDescriptor[] { ageDescriptor };
        }
        catch (IntrospectionException ex) {
            throw new Error(ex.toString());
        }
    }
}
```



注册自定义的propertyEditor

* spring会自动注册 内置的propertyEditor

* javabean 框架会自动 发现 与javabean同名同包的 propertyEditor

* 也可以注册自己的propertyeditor

  * 手动注册: ConfigurableBeanFactory.registerCustomEditor.
  * 使用 post-processor , CustomEditorConfigurer 来注册
  * 使用 e bean factory post-processors  with `BeanFactory` 

  * 使用 PropertyEditorRegistrar

  ```java
  package com.foo.editors.spring;
  
  public final class CustomPropertyEditorRegistrar implements PropertyEditorRegistrar {
  
      public void registerCustomEditors(PropertyEditorRegistry registry) {
  
          // it is expected that new PropertyEditor instances are created
          registry.registerCustomEditor(ExoticType.class, new ExoticTypeEditor());
  
          // you could register as many custom property editors as are required here...
      }
  }
  
  ```

  ```xml
  <bean class="org.springframework.beans.factory.config.CustomEditorConfigurer">
      <property name="propertyEditorRegistrars">
          <list>
              <ref bean="customPropertyEditorRegistrar"/>
          </list>
      </property>
  </bean>
  
  <bean id="customPropertyEditorRegistrar"
      class="com.foo.editors.spring.CustomPropertyEditorRegistrar"/>
  ```

  ```
  继承SimpleFormController 初始化自定义的属性注册
  public final class RegisterUserController extends SimpleFormController {
  
      private final PropertyEditorRegistrar customPropertyEditorRegistrar;
  
      public RegisterUserController(PropertyEditorRegistrar propertyEditorRegistrar) {
          this.customPropertyEditorRegistrar = propertyEditorRegistrar;
      }
  
      protected void initBinder(HttpServletRequest request,
              ServletRequestDataBinder binder) throws Exception {
          this.customPropertyEditorRegistrar.registerCustomEditors(binder);
      }
  
      // other methods to do with registering a User
  }
  ```

  ## 类型转换

  ```java
  package org.springframework.core.convert.converter;
  
  public interface Converter<S, T> {
  
      T convert(S source);
  }
  ```

  ```java
  // 类型转换器 位于 core.convert.support
  package org.springframework.core.convert.support;
  
  final class StringToInteger implements Converter<String, Integer> {
  
      public Integer convert(String source) {
          return Integer.valueOf(source);
      }
  }
  ```

  ```java
  使用ConverterFactory
  package org.springframework.core.convert.converter;
  
  public interface ConverterFactory<S, R> {
  
      <T extends R> Converter<S, T> getConverter(Class<T> targetType);
  }
  
  ```

  ```java
  package org.springframework.core.convert.support;
  
  final class StringToEnumConverterFactory implements ConverterFactory<String, Enum> {
  
      public <T extends Enum> Converter<String, T> getConverter(Class<T> targetType) {
          return new StringToEnumConverter(targetType);
      }
  
      private final class StringToEnumConverter<T extends Enum> implements Converter<String, T> {
  
          private Class<T> enumType;
  
          public StringToEnumConverter(Class<T> enumType) {
              this.enumType = enumType;
          }
  
          public T convert(String source) {
              return (T) Enum.valueOf(this.enumType, source.trim());
          }
      }
  }
  ```



GenericConverter

```java
package org.springframework.core.convert.converter;

public interface GenericConverter {

    public Set<ConvertiblePair> getConvertibleTypes();

    Object convert(Object source, TypeDescriptor sourceType, TypeDescriptor targetType);
}
```

ConditionalGenericConverter

```java
public interface ConditionalConverter {

    boolean matches(TypeDescriptor sourceType, TypeDescriptor targetType);
}

public interface ConditionalGenericConverter extends GenericConverter, ConditionalConverter {
}
```

ConversionService

```java
package org.springframework.core.convert;

public interface ConversionService {

    boolean canConvert(Class<?> sourceType, Class<?> targetType);

    <T> T convert(Object source, Class<T> targetType);

    boolean canConvert(TypeDescriptor sourceType, TypeDescriptor targetType);

    Object convert(Object source, TypeDescriptor sourceType, TypeDescriptor targetType);

}
```

* 配置conversionService
* 编程方式使用 conversionService



Formatt

前面提到的 Type Convert类型转换 , spel,formatter,databinder就是基于 类型转换类 来工作的

* 使用
* 注解
* 注册



*Configuring a Global Date and Time Format*

spring全局默认使用 DateFormat.SHORT 

```java
org.springframework.format.datetime.standard.DateTimeFormatterRegistrar

org.springframework.format.datetime.DateFormatterRegistrar, or org.springframework.format.datetime.joda.JodaTimeFormatterRegistrar
```

注册全局日期格式

```java
@Configuration
public class AppConfig {

    @Bean
    public FormattingConversionService conversionService() {

        // Use the DefaultFormattingConversionService but do not register defaults
        DefaultFormattingConversionService conversionService = new DefaultFormattingConversionService(false);

        // Ensure @NumberFormat is still supported
        conversionService.addFormatterForFieldAnnotation(new NumberFormatAnnotationFormatterFactory());

        // Register JSR-310 date conversion with a specific global format
        DateTimeFormatterRegistrar registrar = new DateTimeFormatterRegistrar();
        registrar.setDateFormatter(DateTimeFormatter.ofPattern("yyyyMMdd"));
        registrar.registerFormatters(conversionService);

        // Register date conversion with a specific global format
        DateFormatterRegistrar registrar = new DateFormatterRegistrar();
        registrar.setFormatter(new DateFormatter("yyyyMMdd"));
        registrar.registerFormatters(conversionService);

        return conversionService;
    }
}
```

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="
        http://www.springframework.org/schema/beans
        https://www.springframework.org/schema/beans/spring-beans.xsd>

    <bean id="conversionService" class="org.springframework.format.support.FormattingConversionServiceFactoryBean">
        <property name="registerDefaultFormatters" value="false" />
        <property name="formatters">
            <set>
                <bean class="org.springframework.format.number.NumberFormatAnnotationFormatterFactory" />
            </set>
        </property>
        <property name="formatterRegistrars">
            <set>
                <bean class="org.springframework.format.datetime.joda.JodaTimeFormatterRegistrar">
                    <property name="dateFormatter">
                        <bean class="org.springframework.format.datetime.joda.DateTimeFormatterFactoryBean">
                            <property name="pattern" value="yyyyMMdd"/>
                        </bean>
                    </property>
                </bean>
            </set>
        </property>
    </bean>
</beans>
```

## Bean验证

```java
public class PersonForm {

    @NotNull
    @Size(max=64)
    private String name;

    @Min(0)
    private int age;
}
```

注入bean

```java
import javax.validation.Validator;

@Service
public class MyService {

    @Autowired
    private Validator validator;
}
```

自定义约束

每个自定义约束包含两个部分

* @Constraint 注解申明约束
* *javax.validation.ConstraintValidator*的实现

声明注解

```java
@Target({ElementType.METHOD, ElementType.FIELD})
@Retention(RetentionPolicy.RUNTIME)
@Constraint(validatedBy=MyConstraintValidator.class)
public @interface MyConstraint {
}
```

实现validator

```java
import javax.validation.ConstraintValidator;

public class MyConstraintValidator implements ConstraintValidator {

    @Autowired;
    private Foo aDependency;

    // ...
}
```

```java
import org.springframework.validation.beanvalidation.MethodValidationPostProcessor;

@Configuration

public class AppConfig {

    @Bean
    public MethodValidationPostProcessor validationPostProcessor() {
        return new MethodValidationPostProcessor;
    }
}
```

# SPEL表达式

Spring Expression Language (SpEL)

## SPEL支持的功能

- Literal expressions 字面量表达式:数学表达式
- Boolean and relational operators  布尔运算符,关系运算符
- Regular expressions : 正则表达式
- Class expressions : 类表达式
- Accessing properties, arrays, lists, and maps :访问数组,列表,map
- Method invocation:静态方法调用,对象方法调用
- Relational operators 关系运算符
- Assignment 赋值
- Calling constructors 调用构造器
- Bean references bean引用
- Array construction 数组构建
- Inline lists 内联lists
- Inline maps 内联 map
- Ternary operator  三元运算符
- Variables 变量
- User-defined functions 用户定义功能
- Collection projection 集合投影
- Collection selection 集合选择
- Templated expressions 模板表达式



## Evaluation

* spel的包 位于 *org.springframework.expression* ,它的子包 *spel.support*

* *ExpressionParser* 接口负责 解析 string类型的 表达式,可能会抛出两个异常,*ParseException* *EvaluationException*

* string类型的字面量用 单引号 包裹

* *parser.parseExpression` and `exp.getValue* 必须成对的调用

  ```java
  ExpressionParser parser = new SpelExpressionParser();
  Expression exp = parser.parseExpression("'Hello World'"); 
  String message = (String) exp.getValue();
  ```

### Example

#### 调用string.concat

```java
ExpressionParser parser = new SpelExpressionParser();
Expression exp = parser.parseExpression("'Hello World'.concat('!')"); 
String message = (String) exp.getValue();
```

#### 标准的点式调用

```java
ExpressionParser parser = new SpelExpressionParser();

// invokes 'getBytes().length'
Expression exp = parser.parseExpression("'Hello World'.bytes.length"); 
int length = (Integer) exp.getValue();
```

#### 按照Java语法

```java
ExpressionParser parser = new SpelExpressionParser();
Expression exp = parser.parseExpression("new String('hello world').toUpperCase()"); 
String message = exp.getValue(String.class);
```

从对象示例中取 变量的值

```java
// Create and set a calendar
GregorianCalendar c = new GregorianCalendar();
c.set(1856, 7, 9);

// The constructor arguments are name, birthday, and nationality.
Inventor tesla = new Inventor("Nikola Tesla", c.getTime(), "Serbian");

ExpressionParser parser = new SpelExpressionParser();

Expression exp = parser.parseExpression("name"); // Parse name as an expression
String name = (String) exp.getValue(tesla);
// name == "Nikola Tesla"

exp = parser.parseExpression("name == 'Nikola Tesla'");
boolean result = exp.getValue(tesla, Boolean.class);
// result == true
```


{% endraw %}