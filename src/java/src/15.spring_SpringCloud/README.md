# 微服务的优点

## 概述

“微服务”一词来源于 Martin Fowler 的《Microservices》一文。微服务是一种架构风格，即将单体应用划分为小型的服务单元，微服务之间使用 HTTP 的 API 进行资源访问与操作。

在笔者看来，微服务架构的演变更像是一个公司的发展过程，从最开始的小公司，到后来的大集团。大集团可拆分出多个子公司，每个子公司的都有自己独立的业务、员工，各自发展，互不影响，合起来则是威力无穷。

臃肿的系统、重复的代码、超长的启动时间带给开发人员的只有无限的埋怨，丝毫没有那种很舒服的、很流畅的写代码的感觉。他们把大部分时间都花在解决问题和项目启动上面了。



## 微服务架构的优势

### 服务的独立部署

每个服务都是一个独立的项目，可以独立部署，不依赖于其他服务，耦合性低。

### 服务的快速启动

拆分之后服务启动的速度必然要比拆分之前快很多，因为依赖的库少了，代码量也少了。

### 更加适合敏捷开发

敏捷开发以用户的需求进化为核心，采用迭代、循序渐进的方法进行。服务拆分可以快速发布新版本，修改哪个服务只需要发布对应的服务即可，不用整体重新发布。

### 职责专一

由专门的团队负责专门的服务,业务发展迅速时，研发人员也会越来越多，每个团队可以负责对应的业务线，服务的拆分有利于团队之间的分工。

### 服务可以动态按需扩容

当某个服务的访问量较大时，我们只需要将这个服务扩容即可。

### 代码的复用

每个服务都提供 REST API，所有的基础服务都必须抽出来，很多的底层实现都可以以接口方式提供。

## 微服务架构的劣势

微服务其实是一把双刃剑，既然有利必然也会有弊。下面我们来谈谈微服务有哪些弊端，以及能采取什么办法避免。

### 分布式部署，调用的复杂性高

单体应用的时候，所有模块之前的调用都是在本地进行的，在微服务中，每个模块都是独立部署的，通过 HTTP 来进行通信，这当中会产生很多问题，比如网络问题、容错问题、调用关系等。

### 独立的数据库，分布式事务的挑战

每个微服务都有自己的数据库，这就是所谓的去中心化的数据管理。这种模式的优点在于不同的服务，可以选择适合自身业务的数据，比如订单服务可以用 MySQL、评论服务可以用 Mongodb、商品搜索服务可以用 Elasticsearch。

缺点就是事务的问题了，目前最理想的解决方案就是柔性事务中的最终一致性，后面的章节会给大家做具体介绍。

### 测试的难度提升

服务和服务之间通过接口来交互，当接口有改变的时候，对所有的调用方都是有影响的，这时自动化测试就显得非常重要了，如果要靠人工一个个接口去测试，那工作量就太大了。这里要强调一点，就是 API 文档的管理尤为重要。

### 运维难度的提升

我们可能只需要关注一个 Tomcat 的集群、一个 MySQL 的集群就可以了，但这在微服务架构下是行不通的。当业务增加时，服务也将越来越多，服务的部署、监控将变得非常复杂，这个时候对于运维的要求就高了。





# SpringCloud是什么

Spring Cloud是一系列框架的有序集合。它利用 Spring Boot 的开发便利性，巧妙地简化了分布式系统基础设施的开发，如服务注册、服务发现、配置中心、消息总线、负载均衡、断路器、数据监控等，这些都可以用 Spring Boot 的开发风格做到一键启动和部署。

通俗地讲，Spring Cloud 就是用于构建微服务开发和治理的框架集合（并不是具体的一个框架），主要贡献来自 Netflix OSS。



Spring Cloud 模块的相关介绍如下：

- Eureka：服务注册中心，用于服务管理。
- Ribbon：基于客户端的负载均衡组件。
- Hystrix：容错框架，能够防止服务的雪崩效应。
- Feign：Web 服务客户端，能够简化 HTTP 接口的调用。
- Zuul：API 网关，提供路由转发、请求过滤等功能。
- Config：分布式配置管理。
- Sleuth：服务跟踪。
- Stream：构建消息驱动的微服务应用程序的框架。
- Bus：消息代理的集群消息总线。





# SpringCloud版本介绍

## **与springBoot版本兼容**

| Release Train                                                | Boot Version                     |
| :----------------------------------------------------------- | :------------------------------- |
| [2020.0.x](https://github.com/spring-cloud/spring-cloud-release/wiki/Spring-Cloud-2020.0-Release-Notes) aka Ilford | 2.4.x                            |
| [Hoxton](https://github.com/spring-cloud/spring-cloud-release/wiki/Spring-Cloud-Hoxton-Release-Notes) | 2.2.x, 2.3.x (Starting with SR5) |
| [Greenwich](https://github.com/spring-projects/spring-cloud/wiki/Spring-Cloud-Greenwich-Release-Notes) | 2.1.x                            |
| [Finchley](https://github.com/spring-projects/spring-cloud/wiki/Spring-Cloud-Finchley-Release-Notes) | 2.0.x                            |
| [Edgware](https://github.com/spring-projects/spring-cloud/wiki/Spring-Cloud-Edgware-Release-Notes) | 1.5.x                            |
| [Dalston](https://github.com/spring-projects/spring-cloud/wiki/Spring-Cloud-Dalston-Release-Notes) | 1.5.x                            |



