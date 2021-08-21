# 简介

Spring cloud有两种服务调用方式，一种是ribbon+restTemplate，另一种是feign，

ribbon是一个负载均衡客户端，可以很好的控制htt和tcp的一些行为

ribbon 已经默认实现了这些配置bean：

- IClientConfig ribbonClientConfig: DefaultClientConfigImpl
- IRule ribbonRule: ZoneAvoidanceRule
- IPing ribbonPing: NoOpPing
- ServerList ribbonServerList: ConfigurationBasedServerList
- ServerListFilter ribbonServerListFilter: ZonePreferenceServerListFilter
- ILoadBalancer ribbonLoadBalancer: ZoneAwareLoadBalancer





# 使用方式

## 依赖申明

```xml
       <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-starter-openfeign</artifactId>
        </dependency>
```

**定义实现接口**

```java
package com.weisanju.nacConsumer;

import org.springframework.cloud.openfeign.FeignClient;
import org.springframework.stereotype.Service;
import org.springframework.web.bind.annotation.GetMapping;

@Service
@FeignClient("service-provider")
public interface RemoteService {
    @GetMapping("/time")
    String timeService();
}
```

**启用扫描代理**

```java
@EnableFeignClients(basePackages = "com.weisanju.nacConsumer")
```

**使用**

```java
    public static class TestController {

        private final RestTemplate restTemplate;
        @Autowired
        private RemoteService remoteService;

        @Autowired
        public TestController(RestTemplate restTemplate) {
            this.restTemplate = restTemplate;
        }

        @RequestMapping(value = "/echo/{str}", method = RequestMethod.GET)
        public String echo(@PathVariable String str) {
            return restTemplate.getForObject("http://service-provider/echo/" + str, String.class);
        }
        @RequestMapping(value = "/myTime", method = RequestMethod.GET)
        public String echo() {
            return remoteService.timeService();
        }
    }
```

