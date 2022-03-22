# 概述

本Maven教程的目的是使您了解Maven的工作方式。因此，本教程重点介绍Maven的核心概念。

实际上，Maven开发人员声称Maven不仅仅是一个构建工具。参见 [Philosophy of Maven](http://maven.apache.org/background/philosophy-of-maven.html)

**Maven Version**

该Maven教程的第一个版本基于Maven 3.6.3。

**Maven Website**

[[http://maven.apache.org](http://maven.apache.org/)]



## What is a Build Tool?

A build tool is a tool that automates everything related to building the software project. 

构建工具是一种工具，它可以自动完成与构建软件项目相关的所有操作，构建软件工程一半需要以下几个步骤

- 产生源代码
- 产生文档
- 编译源代码
- 打包编译后的源代码
- 部署源代码

任何给定的软件项目可能具有比完成最终软件所需的更多活动。通常可以将此类活动插入构建工具中，因此也可以将这些活动自动化。

## 安装Maven

1. Set the `JAVA_HOME` environment variable to point to a valid Java SDK (e.g. Java 8).
2. Set the `M2_HOME` environment variable to point to the directory you unzipped Maven to.
3. Set the `M2` environment variable to point to `M2_HOME/bin` (`%M2_HOME%\bin` on Windows, `$M2_HOME/bin` on unix).
4. Add `M2` to the `PATH` environment variable (`%M2%` on Windows, `$M2` on unix).
5. 验证 *mvn* 命令





