# springboot配置步骤

1. 配置springboot依赖

```
<dependency>
  <groupId>io.dubbo.springboot</groupId>
  <artifactId>spring-boot-starter-dubbo</artifactId>
  <version>1.0.0</version>
</dependency> 
```

2. 配置zkclient客户端依赖

   ```
          <dependency>
               <groupId>com.github.sgroschupf</groupId>
               <artifactId>zkclient</artifactId>
               <version>0.1</version>
           </dependency>
   ```

3. 配置生产者

   1. 启用*@EnableDubboConfiguration*  自动注解 在 spring boot启动类

   2. ```
      @com.alibaba.dubbo.config.annotation.Service(interfaceClass = DemoService.class)
      注解在 接口的实现类中
      ```

   3. 配置生产者服务应用名

      *spring.application.name=dubbo-spring-boot-starter*

   4. 配置是否是 生产者服务方

      *spring.dubbo.server = true*

   5. 配置服务端口

      *spring.dubbo.protocol.port = 20880*

   6. 配置注册中心地址

      *spring.dubbo.registry.address=zookeeper://192.168.3.7:2181*

   7. 配置连接协议

      *dubbo.protocol.name=dubbo*

   8. 超时

      *dubbo.consumer.timeout=3000*

   9. 示例配置

      ```yaml
      spring:
       application:
        name: dubbo-spring-boot-starter
       dubbo:
        server: true
        protocl:
          port: 20880
        registry:
          address: zookeeper://192.168.3.7:2181
      ```

4. 配置消费者
   1. 定义与生产者 同包接口
   2. 启用*@EnableDubboConfiguration*  自动注解 在 spring boot启动类
   3. *@Reference* 引用启动类

# 动态配置中心

## 可以使用 zookeeper作为动态配置中心

1. XML配置

   *<dubbo:config-center address="zookeeper://127.0.0.1:2181"/>*

2. 配置文件配置

   *dubbo.config-center.address=zookeeper://127.0.0.1:2181*

3. API配置

   *ConfigCenterConfig configCenter = **new** ConfigCenterConfig(); configCenter.setAddress("zookeeper://127.0.0.1:2181");*

4. 配置中心更适合将一些公共配置如注册中心、元数据中心配置等抽取以便做集中管理。

## 配置优先级

*-Ddubbo.config-center.highest-priority=false*

外部配置 较 本地配置有 更高的优先级

## 作用域

外部化配置有全局和应用两个级别

全局配置是所有应用共享的，应用级配置是由每个应用自己维护且只对自身可见的

## 在Zookeeper上的存储

全局配置 */dubbo/config*

默认全局结点配置 */dubbo/config/dubbo/dubbo.properties* 

应用配置 */dubbo/config/dubbo/application/dubbo.properties* 

## 服务治理

configurators/tag-router/condition-router

不同的服务治理规则类型，node value存储具体规则内容



# Dubbo配置加载流程

主要讲在**应用启动阶段，Dubbo框架如何将所需要的配置采集起来**（包括应用配置、注册中心配置、服务配置等），以完成服务的暴露和引用流程。





