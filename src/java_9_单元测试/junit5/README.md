## Overview

本文档的目标是为编写测试的程序员，扩展作者和引擎作者以及构建工具和IDE供应商提供全面的参考文档。



## What is JUnit 5?

与以前的Junit版本不同，JUnit 5由来自三个不同子项目的几个不同模块组成。

**JUnit 5 = JUnit Platform + JUnit Jupiter + JUnit Vintage**



 **JUnit Platform** 

* The **JUnit Platform** serves as a foundation for [launching testing frameworks](https://junit.org/junit5/docs/current/user-guide/#launcher-api) on the JVM.
*  It also defines the `TestEngine` API for developing a testing framework that runs on the platform. 
* Furthermore, the platform provides a [Console Launcher](https://junit.org/junit5/docs/current/user-guide/#running-tests-console-launcher) to launch the platform from the command line and the [JUnit Platform Suite Engine](https://junit.org/junit5/docs/current/user-guide/#junit-platform-suite-engine) for running a custom test suite using one or more test engines on the platform. 
* First-class support for the JUnit Platform also exists in popular IDEs (see [IntelliJ IDEA](https://junit.org/junit5/docs/current/user-guide/#running-tests-ide-intellij-idea), [Eclipse](https://junit.org/junit5/docs/current/user-guide/#running-tests-ide-eclipse), [NetBeans](https://junit.org/junit5/docs/current/user-guide/#running-tests-ide-netbeans), and [Visual Studio Code](https://junit.org/junit5/docs/current/user-guide/#running-tests-ide-vscode)) and build tools (see [Gradle](https://junit.org/junit5/docs/current/user-guide/#running-tests-build-gradle), [Maven](https://junit.org/junit5/docs/current/user-guide/#running-tests-build-maven), and [Ant](https://junit.org/junit5/docs/current/user-guide/#running-tests-build-ant)).

**JUnit Jupiter**

JUnit Jupiter is the combination of the new [programming model](https://junit.org/junit5/docs/current/user-guide/#writing-tests) and [extension model](https://junit.org/junit5/docs/current/user-guide/#extensions) for writing tests and extensions in JUnit 5. The Jupiter sub-project provides a `TestEngine` for running Jupiter based tests on the platform.

**JUnit Vintage**

**JUnit Vintage** provides a `TestEngine` for running JUnit 3 and JUnit 4 based tests on the platform. It requires JUnit 4.12 or later to be present on the class path or module path.

### Supported Java Versions

JUnit 5 requires Java 8 (or higher) at runtime. However, you can still test code that has been compiled with previous versions of the JDK.

### Getting Help

Ask JUnit 5 related questions on [Stack Overflow](https://stackoverflow.com/questions/tagged/junit5) or chat with the community on [Gitter](https://gitter.im/junit-team/junit5).

### Getting Started

#### Downloading JUnit Artifacts

To find out what artifacts are available for download and inclusion in your project, refer to [Dependency Metadata](https://junit.org/junit5/docs/current/user-guide/#dependency-metadata). To set up dependency management for your build, refer to [Build Support](https://junit.org/junit5/docs/current/user-guide/#running-tests-build) and the [Example Projects](https://junit.org/junit5/docs/current/user-guide/#overview-getting-started-example-projects).

#### JUnit 5 Features

To find out what features are available in JUnit 5 and how to use them, read the corresponding sections of this User Guide, organized by topic.

要了解JUnit 5中可用的功能以及如何使用它们，请阅读本用户指南的相应部分 (按主题组织)。

- [Writing Tests in JUnit Jupiter](https://junit.org/junit5/docs/current/user-guide/#writing-tests)
- [Migrating from JUnit 4 to JUnit Jupiter](https://junit.org/junit5/docs/current/user-guide/#migrating-from-junit4)
- [Running Tests](https://junit.org/junit5/docs/current/user-guide/#running-tests)
- [Extension Model for JUnit Jupiter](https://junit.org/junit5/docs/current/user-guide/#extensions)
- Advanced Topics
  - [JUnit Platform Launcher API](https://junit.org/junit5/docs/current/user-guide/#launcher-api)
  - [JUnit Platform Test Kit](https://junit.org/junit5/docs/current/user-guide/#testkit)

### Example Projects

To see complete, working examples of projects that you can copy and experiment with, the [`junit5-samples`](https://github.com/junit-team/junit5-samples) repository is a good place to start. The `junit5-samples` repository hosts a collection of sample projects based on JUnit Jupiter, JUnit Vintage, and other testing frameworks. You’ll find appropriate build scripts (e.g., `build.gradle`, `pom.xml`, etc.) in the example projects. The links below highlight some of the combinations you can choose from.

- For Gradle and Java, check out the `junit5-jupiter-starter-gradle` project.
- For Gradle and Kotlin, check out the `junit5-jupiter-starter-gradle-kotlin` project.
- For Gradle and Groovy, check out the `junit5-jupiter-starter-gradle-groovy` project.
- For Maven, check out the `junit5-jupiter-starter-maven` project.
- For Ant, check out the `junit5-jupiter-starter-ant` project.





## 关于JUnit5

JUnit是常用的java单元测试框架，5是当前最新版本，其整体架构如下(图片来自网络)：

![img](../../images/junit_platform.png)

- 从上图可见，整个Junit5可以划分成三层：顶层框架(Framework)、中间的引擎（Engine），底层的平台（Platform）；
- 官方定义Junit5由三部分组成：Platform、Jupiter、Vintage，功能如下；
- **Platform**：位于架构的最底层，是JVM上执行单元测试的基础平台，还对接了各种IDE（例如IDEA、eclipse），并且还与引擎层对接，定义了引擎层对接的API；
- **Jupiter**：位于引擎层，支持5版本的编程模型、扩展模型；
- **Vintage**：位于引擎层，用于执行低版本的测试用例；
- 可见整个Junit Platform是开放的，通过引擎API各种测试框架都可以接入；





