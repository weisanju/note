# 依赖配置

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-autoconfigure</artifactId>
    <version>2.2.1.RELEASE</version>
</dependency>
<dependency>
    <groupId>org.projectlombok</groupId>
    <artifactId>lombok</artifactId>
    <version>1.18.16</version>
</dependency>
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-configuration-processor</artifactId>
    <version>2.4.0</version>
</dependency>
```

# 定义Service业务类

```java
package com.weisanju;

public class MsgService {
    String url;
    String accessKeySecret;
    String accessKeyId;
    public MsgService(String url,String accessKeyId, String accessKeySecret) {
        this.url = url;
        this.accessKeySecret = accessKeySecret;
        this.accessKeyId = accessKeyId;
    }
    public void sendMsgService(String msg){
        HttpClientUtils.sendMsg(url,accessKeyId,accessKeySecret,msg);
    }

    public MsgService() {
    }
}
```



# 定义配置类

```java
package com.weisanju;

import lombok.Data;
import org.springframework.boot.context.properties.ConfigurationProperties;

@ConfigurationProperties(prefix = "msg")
@Data
public class MsgProperties {
    private String url;
    private String accessKeyId;
    private String accessKeySecret;

    public String getUrl() {
        return url;
    }

    public void setUrl(String url) {
        this.url = url;
    }

    public String getAccessKeyId() {
        return accessKeyId;
    }

    public void setAccessKeyId(String accessKeyId) {
        this.accessKeyId = accessKeyId;
    }

    public String getAccessKeySecret() {
        return accessKeySecret;
    }

    public void setAccessKeySecret(String accessKeySecret) {
        this.accessKeySecret = accessKeySecret;
    }
}
```



# 定义自动配置类

```java
package com.weisanju;

import com.weisanju.MsgService;
import org.springframework.boot.autoconfigure.condition.ConditionalOnClass;
import org.springframework.boot.autoconfigure.condition.ConditionalOnMissingBean;
import org.springframework.boot.autoconfigure.condition.ConditionalOnProperty;
import org.springframework.boot.context.properties.EnableConfigurationProperties;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import javax.annotation.Resource;

@Configuration
@ConditionalOnClass(MsgService.class)
@EnableConfigurationProperties(MsgProperties.class)
public class MsgAutoConfiguration {
    //注入属性配置类
    @Resource
    private MsgProperties msgProperties;

    @Bean
    @ConditionalOnMissingBean(MsgService.class)
    @ConditionalOnProperty(prefix = "msg",value = "enabled",havingValue = "true")
    public MsgService msgService() {
        return new MsgService(msgProperties.getUrl(),msgProperties.getAccessKeyId() ,msgProperties.getAccessKeySecret());
    }
}
```

# 定义 类SPI查找

*src/main/resources/META-INF/spring.factories*

路径中 定义 自动配置类 *org.springframework.boot.autoconfigure.EnableAutoConfiguration=com.weisanju.MsgAutoConfiguration*

