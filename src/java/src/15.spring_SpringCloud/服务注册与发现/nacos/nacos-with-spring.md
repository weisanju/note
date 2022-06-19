# 与Spring集成

## 配置中心

### 依赖

```xml
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context</artifactId>
            <version>5.3.5</version>
        </dependency>
        <dependency>
            <groupId>com.alibaba.nacos</groupId>
            <artifactId>nacos-spring-context</artifactId>
            <version>1.0.0</version>
        </dependency>
```



### Spring配置

```java
package com.weisanju.nacos;

import com.alibaba.nacos.api.annotation.NacosProperties;
import com.alibaba.nacos.api.config.annotation.NacosValue;
import com.alibaba.nacos.spring.context.annotation.config.EnableNacosConfig;
import com.alibaba.nacos.spring.context.annotation.config.NacosPropertySource;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
@Configuration
@EnableNacosConfig(globalProperties = @NacosProperties(serverAddr = "127.0.0.1:8848"))
@NacosPropertySource(dataId = "test.yml", autoRefreshed = true)
@ComponentScan("com.weisanju.nacos")
public class NacosConfig {
}


@NacosValue(value = "${xjq}", autoRefreshed = true)
private String xjq;
```

### 使用

```java
package com.weisanju.nacos;

import com.alibaba.nacos.client.config.NacosConfigService;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

public class MainTest {
    public static void main(String[] args) throws InterruptedException {
        AnnotationConfigApplicationContext annotationConfigApplicationContext = new AnnotationConfigApplicationContext(NacosConfig.class);

        //NacosConfigService bean1 = annotationConfigApplicationContext.getBean(NacosConfigService.class);
        ComponentTest bean = annotationConfigApplicationContext.getBean(ComponentTest.class);

        while (true){
            System.out.println(bean.getXjq());
            Thread.sleep(1000);
        }
    }
}
```



## 启动服务发现

```
@EnableNacosDiscovery(globalProperties = @NacosProperties(serverAddr = "127.0.0.1:8848"))
//服务注入
@NacosInjected
NamingService namingService;
```



