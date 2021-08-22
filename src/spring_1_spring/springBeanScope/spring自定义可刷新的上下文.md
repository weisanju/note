# 步骤

## 自定义Scope注解

```java
@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Scope(AutoRefreshScope.REFRESH_SCOPE) //指定scopename
@Documented
public @interface RefreshScope {
    // 设置proxyMode的值为ScopedProxyMode.TARGET_CLASS
    // 目的是使用cglib生成一个代理对象，通过这个代理对象来访问目标bean对象
    ScopedProxyMode proxyMode() default ScopedProxyMode.TARGET_CLASS;
}
```

## 自定义SpringScope处理类

```java
public class AutoRefreshScope implements Scope {
    /**
     * 单例模式，声明一个实例
     */
    private static final AutoRefreshScope instance = new AutoRefreshScope();
    /**
     * 来个map用来缓存bean
     */
    private Map<String, Object> beanMap = new ConcurrentHashMap<>();

    private AutoRefreshScope() {
    }

    /**
     * 获取实例
     *
     * @return
     */
    public static AutoRefreshScope getInstance() {
        return instance;
    }

    /**
     * 清理指定名称的key
     */
    public static void clean(String name) {
        instance.beanMap.remove(name);
    }

    /**
     * 定义作用域名称为：refresh
     */
    public static final String REFRESH_SCOPE = "refresh";

    @Override
    public Object get(String name, ObjectFactory<?> objectFactory) {
        Object bean = beanMap.get(name);
        if (bean == null) {
            bean = objectFactory.getObject();
            beanMap.put(name, bean);
        }
        return bean;
    }

    
    @Override
    public Object remove(String s) {
        return null;
    }

    @Override
    public void registerDestructionCallback(String s, Runnable runnable) {

    }

    @Override
    public Object resolveContextualObject(String s) {
        return null;
    }

    @Override
    public String getConversationId() {
        return null;
    }
}
```

## 注册Scope

```java
@Configuration
public class CommonConfig {
    @Autowired
    ConfigurableBeanFactory factory;
    @Autowired
    ConfigurableEnvironment environment;
    @Autowired
    PropertiesDataFactory propertiesDataFactory;

    @PostConstruct
    public void init(){
        // 将自定义作用域注册到spring容器中
        factory.registerScope(AutoRefreshScope.REFRESH_SCOPE, AutoRefreshScope.getInstance());

        // 更新配置信息
        propertiesDataFactory.UpdateUserConfig(environment);
    }
}
```

## 配置更新类

```java
@Component
public class PropertiesDataFactory {
    /**
     * 获取用户配置信息
     * 为了演示，就模拟一下，真实项目里可能是从数据库，或者其它方式取得配置信息
     *
     * @return map
     */
    public static Map<String, Object> getUserConfig() {
        Map<String, Object> map = new HashMap<>(16);

        // 注意：这里map的key必须要和@Value("${key}")中的key相同
        map.put("user.name", UUID.randomUUID().toString().replaceAll("-", ""));
        map.put("user.sex", "18");
        map.put("user.phone", "180-0000-0000");
        map.put("user.address", "海底大世界");

        return map;
    }

    /**
     * 更新配置信息
     */
    public void UpdateUserConfig(ConfigurableEnvironment environment) {
        // 模拟从其它地方获取配置信息，可能是数据库，也可能是其它渠道
        Map<String, Object> userConfig = PropertiesDataFactory.getUserConfig();
        // 创建一个MapPropertySource,将配置信息放到其中，
        // MapPropertySource可以理解为Map<String, PropertySource>
        MapPropertySource propertySource = new MapPropertySource("user", userConfig);
        // 将propertySource放到MutablePropertySources里，后面Environment解析时会使用到
        // addFirst是为了放到首位，解析速度更快
        environment.getPropertySources().addFirst(propertySource);
    }
}
```

## 在配置类上使用

```java
@Data
@Component
@RefreshScope
public class UserConfig {
    @Value("${user.name}")
    private String name;

    @Value("${user.sex}")
    private String sex;

    @Value("${user.phone}")
    private String phone;

    @Value("${user.address}")
    private String address;

    @Override
    public String toString() {
        return "UserConfig{" +
                "name='" + name + '\'' +
                ", sex='" + sex + '\'' +
                ", phone='" + phone + '\'' +
                ", address='" + address + '\'' +
                '}';
    }

    public UserConfig() {
        System.out.println(1);
    }
}
```

## 更新配置类

```java
/**
 * 更新配置信息
 */
public void UpdateUserConfig(ConfigurableEnvironment environment) {
    // 模拟从其它地方获取配置信息，可能是数据库，也可能是其它渠道
    Map<String, Object> userConfig = PropertiesDataFactory.getUserConfig();
    // 创建一个MapPropertySource,将配置信息放到其中，
    // MapPropertySource可以理解为Map<String, PropertySource>
    MapPropertySource propertySource = new MapPropertySource("user", userConfig);
    // 将propertySource放到MutablePropertySources里，后面Environment解析时会使用到
    // addFirst是为了放到首位，解析速度更快
    environment.getPropertySources().addFirst(propertySource);
}
```

