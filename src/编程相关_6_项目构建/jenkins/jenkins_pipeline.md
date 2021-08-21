# 概述

**该文档覆盖了 Jenkins Pipeline功能的所有推荐的方面**

包括

* 如何 在 UI界面、SCM、定义Pipeline 
* 创建并使用 Jenkinsfile
* git分支 与 pull request
* 在Pipeline中使用Docker
* Pipeline中的继承
* 使用不同的开发工具 加速 Pipeline的创建
* 使用流水线语法 - 此页面是所有声明式流水线语法的综合参考。



## 什么是JenkinsPipeline

持续交付 (CD) 管道是将软件从版本控制到用户和客户的过程的自动化

对您的软件（在源代码控制中提交）的每一次更改在发布之前都经历了一个复杂的过程。

此过程涉及以可靠且可重复的方式构建软件，以及通过多个测试和部署阶段推进构建的软件



Pipeline 提供了一组可扩展的工具，用于通过 Pipeline 域特定语言 (DSL) 语法将简单到复杂的交付管道“作为代码”建模。 

Jenkins 管道的定义被写入一个文本文件（称为 Jenkinsfile），该文件又可以提交到项目的源代码控制存储库。 

[2] 这是“Pipeline-as-code”的基础；

将 CD 管道视为要进行版本控制和审查的应用程序的一部分，就像任何其他代码一样。

创建 Jenkinsfile 并将其提交到源代码控制提供了许多直接的好处：

1. 自动为所有分支和拉取请求创建流水线构建过程。

2. 流水线上的代码审查/迭代（以及剩余的源代码）。

3. 管道的审计跟踪。

4. 管道的单一事实来源 [3]，可由项目的多个成员查看和编辑。

尽管在 Web UI 中或使用 Jenkinsfile 定义流水线的语法是相同的，但通常认为最佳实践是在 Jenkinsfile 中定义流水线并将其签入源代码控制。

## 声明式与脚本式 流水线语法

Jenkinsfile 可以使用两种类型的语法编写 - 声明式和脚本式。

声明式管道和脚本式管道的构造根本不同。

声明式流水线是 Jenkins 流水线的一个更新的特性，它：

提供比 Scripted Pipeline 语法更丰富的语法特性，旨在使编写和阅读 Pipeline 代码更容易。

然而，许多写入 Jenkinsfile 的单个语法组件（或“步骤”）对于声明式和脚本式流水线都是通用的。

## Why Pipeline?

从根本上说，Jenkins 是一个支持多种自动化模式的自动化引擎。

Pipeline 在 Jenkins 上添加了一组强大的自动化工具，支持从简单的持续集成到全面的 CD 管道的用例。

通过对一系列相关任务进行建模，用户可以利用 Pipeline 的许多特性：

- **Code**: 管道在代码中实现，通常会签入源代码管理，使团队能够编辑、审查和迭代他们的交付管道
- **Durable**: 管道可以在 Jenkins 控制器的计划内和计划外重启中存活下来。
- **Pausable**: 流水线可以选择停止并等待人工输入或批准，然后再继续流水线运行。 
- **Versatile**: 管道支持复杂的现实世界 CD 要求，包括分叉/加入、循环和并行执行工作的能力。
- **Extensible**: 管道插件支持对其 DSL [[1](https://www.jenkins.io/doc/book/pipeline/#_footnotedef_1)] 的自定义扩展以及与其他插件集成的多个选项。

基于 Jenkins 可扩展性的核心价值，Pipeline 也可以由使用 Pipeline Shared Libraries 的用户和插件开发人员扩展。 



下面的流程图是在 Jenkins Pipeline 中轻松建模的一个 CD 场景的示例：

# Pipeline concepts

## Pipeline

管道是用户定义的 CD 管道模型。

管道的代码定义了整个构建过程，通常包括

* 构建应用程序
* 测试应用程序
* 和交付应用程序的阶段。

## Node

节点是一台机器，它是 Jenkins 环境的一部分，能够执行流水线。

## Stage

阶段块定义了通过整个流水线（例如“构建”、“测试”和“部署”阶段）执行的概念上不同的任务子集，许多插件使用它来可视化或呈现 Jenkins 流水线状态/进度。 

## Step

一个任务。

从根本上说，步骤告诉 Jenkins 在特定时间点（或过程中的“步骤”）要做什么。

例如，要执行 shell 命令 make 使用 sh 步骤：sh 'make'。



# Pipeline syntax overview

以下流水线代码框架说明了声明式流水线语法和脚本式流水线语法之间的根本区别。

## Declarative Pipeline fundamentals

Jenkinsfile (Declarative Pipeline)

```groovy
pipeline {
    agent any 
    stages {
        stage('Build') { 
            steps {
                // 
            }
        }
        stage('Test') { 
            steps {
                // 
            }
        }
        stage('Deploy') { 
            steps {
                // 
            }
        }
    }
}
```

## Scripted Pipeline fundamentals

在脚本化流水线语法中，一个或多个节点块在整个流水线中完成核心工作。

尽管这不是 Scripted Pipeline 语法的强制性要求，但将 Pipeline 的工作限制在节点块内有两件事：

* 通过将项目添加到 Jenkins 队列来安排块中包含的步骤运行。一旦执行程序在节点上空闲，这些步骤就会运行。
* 创建一个工作区（特定于该特定管道的目录），可以在其中对从源代码管理检出的文件进行工作

> 根据您的 Jenkins 配置，某些工作区在一段时间不活动后可能不会自动清理。

Jenkinsfile (Scripted Pipeline)

```groovy
node {  
    stage('Build') { 
        // 
    }
    stage('Test') { 
        // 
    }
    stage('Deploy') { 
        // 
    }
}
```





## Pipeline example

Jenkinsfile (Declarative Pipeline)

```groovy
pipeline { 
    agent any 
    options {
        skipStagesAfterUnstable()
    }
    stages {
        stage('Build') { 
            steps { 
                sh 'make' 
            }
        }
        stage('Test'){
            steps {
                sh 'make check'
                junit 'reports/**/*.xml' 
            }
        }
        stage('Deploy') {
            steps {
                sh 'make publish'
            }
        }
    }
}
```

| [`pipeline`](https://www.jenkins.io/doc/book/pipeline/syntax#declarative-pipeline) is Declarative Pipeline-specific syntax that defines a "block" containing all content and instructions for executing the entire Pipeline. |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
|                                                              | [`agent`](https://www.jenkins.io/doc/book/pipeline/syntax#agent) is Declarative Pipeline-specific syntax that instructs Jenkins to allocate an executor (on a node) and workspace for the entire Pipeline. |
|                                                              | `stage` is a syntax block that describes a [stage of this Pipeline](https://www.jenkins.io/doc/book/pipeline/#stage). Read more about `stage` blocks in Declarative Pipeline syntax on the [Pipeline syntax](https://www.jenkins.io/doc/book/pipeline/syntax#stage) page. As mentioned [above](https://www.jenkins.io/doc/book/pipeline/#scripted-pipeline-fundamentals), `stage` blocks are optional in Scripted Pipeline syntax. |
|                                                              | [`steps`](https://www.jenkins.io/doc/book/pipeline/syntax#steps) is Declarative Pipeline-specific syntax that describes the steps to be run in this `stage`. |
|                                                              | `sh` is a Pipeline [step](https://www.jenkins.io/doc/book/pipeline/syntax#steps) (provided by the [Pipeline: Nodes and Processes plugin](https://plugins.jenkins.io/workflow-durable-task-step)) that executes the given shell command. |
|                                                              | `junit` is another Pipeline [step](https://www.jenkins.io/doc/book/pipeline/syntax#steps) (provided by the [JUnit plugin](https://plugins.jenkins.io/junit)) for aggregating test reports. |
|                                                              | `node` is Scripted Pipeline-specific syntax that instructs Jenkins to execute this Pipeline (and any stages contained within it), on any available agent/node. This is effectively equivalent to `agent` in Declarative Pipeline-specific syntax. |



