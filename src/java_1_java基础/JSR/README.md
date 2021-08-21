# java体系

Java分为三个体系，分别为 

* *Java SE*（J2SE，Java to Platform Standard Edition，标准版），
* *JavaEE*（J2EE，Java to Platform, Enterprise Edition，企业版），
* Java ME（J2ME，Java to Platform Micro Edition，微型版）。



# 概述

[JCP官网](https://www.jcp.org/en/home/index)

* 任何一门语言的问世和流传，以及取得这样大的影响力都离不开厂商、组织、开发者与用户们的共同参与，而Sun公司为了发展和更新这门语言组成了一个**开放性国际组织JCP**（Java Community Process），
* 任何想要提议加入Java功能或特性都必须以**JSR正式文件**（Java Specification Request）（Java 规范提案），进行提交然后经过JCP执行委员会投票，通过即成为最终标准文件
* 然后必须根据这个JSR做出免费且开发原始码的**参考实现RI**（Reference Implementation），并提供**技术兼容性测试工具包TCK**（Technology Compatibility Kit），厂商可以根据JSR实现产品。

* JCP维护的规范包括J2ME、J2SE、J2EE，XML，OSS，JAIN等。组织成员可以提交JSR（Java Specification Requests），通过特定程序以后主要包括（Java技术规范、参考实现（RI）、技术兼容包（TCK）)，进入到下一版本的规范里面。
* 所有声称符合J2EE规范的J2EE类产品（应用服务器、应用软件、开发工具等），必须通过该组织提供的TCK兼容性测试（需要购买测试包），通过该测试后，需要缴纳J2EE商标使用费。两项完成，即是通过J2EE认证（Authorized Java Licensees of J2EE）。

现在Java无疑已经成为了业界共同制定的一个标准，每一个标准也代表着业界面临的一些问题，而**一个JSR规范标准可以有多种技术解决方案**。

* 下面列出了基于java三个平台的一系列标准JSR（标准）链接：
    * Java EE (54 JSRs) （https://www.jcp.org/en/jsr/platform?listBy=3&listByType=platform）
    * Java SE (57 JSRs) （https://www.jcp.org/en/jsr/platform?listBy=2&listByType=platform）
    * Java ME (85 JSRs) （https://www.jcp.org/en/jsr/platform?listBy=1&listByType=platform）
      



# 以JTA为样例

现在我们进入到一个（Java EE (54 JSRs)）标准的JSR下载页面（JSR-000907 JTA）的API标准：



其中有下载链接有两个文档

* 从字面意思明白一个是 **这些规则及标准的评估**，
* 一个是这些**规则的实施**，打开实施规则的文档可以看出
    * 这些标准提供了一些接口文档和协议。
    * 里面提供了一些面对开发人员的接口以及面对具体实现接口的各个软件公司，也就是这些接口的具体的实现包，由各个软件厂商实现。











# JSR规范整理

| JSR编号 | 模块与功能                               |
| ------- | ---------------------------------------- |
| JSR 310 | Java Date与Time API (时间与日期API)      |
| JSR 315 | Java Servlet 3.0（servlet规范）          |
| JSR 303 | Bean Validation1.0（bean检验）           |
| JSR 380 | Bean Validation 2.0                      |
| JSR 317 | Java Persistence 2.0（持久化）           |
| JSR 338 | Java Persistence 2.2                     |
| JSR 907 | Java Transaction API (JTA)（事务管理器） |
| JSR 250 | Common Annotations for the Java Platform |
| JSR 107 | JCache API（缓存）                       |

# Java EE 8技术对应的JSR标准

Java EE 8 基于Java EE 7. 下面是Java EE 8在java EE7之上的更新或者新增JSR:

- JSR 366 – Java EE 8 Platform
- JSR 365 – Contexts and Dependency Injection (CDI) 2.0
- JSR 367 – The Java API for JSON Binding (JSON-B) 1.0
- JSR 369 – Java Servlet 4.0
- JSR 370 – Java API for RESTful Web Services (JAX-RS) 2.1
- JSR 372 – JavaServer Faces (JSF) 2.3
- JSR 374 – Java API for JSON Processing (JSON-P)1.1
- JSR 375 – Java EE Security API 1.0
- JSR 380 – Bean Validation 2.0
- JSR 250 – Common Annotations 1.3
- JSR 338 – Java Persistence 2.2
- JSR 356 – Java API for WebSocket 1.1
- JSR 919 – JavaMail 1.6

[参考链接](https://www.oracle.com/java/technologies/java-ee-glance.html)

