[官方资料](https://plugins.jetbrains.com/docs/intellij/welcome.html)

# IntelliJ Platform

## What is the IntelliJ Platform?

> 什么是 *IntelliJ Platform*

* IntelliJ平台本身不是产品，而是提供了用于构建IDE的平台，IntelliJ平台提供了这些 IDE需要提供丰富的语言工具支持 所需的所有基础结构。

* 它是一个组件驱动的，基于跨平台JVM的应用程序主机，带有一个高级用户界面工具包，用于创建工具窗口，树视图和列表（支持快速搜索）以及弹出菜单和对话框。

* IntelliJ平台具有一个全文本编辑器，该编辑器具有语法突出显示，代码折叠，代码完成和其他富文本编辑功能的抽象实现。
    还包括一个图像编辑器。

* 此外，它包括用于构建标准IDE功能的开放API，例如项目模型和构建系统。
    它还通过与语言无关的高级断点支持，调用堆栈，监视窗口和表达式解析，为丰富的调试体验提供了基础结构。但是IntelliJ平台的真正力量来自程序结构接口（PSI）。
* 它是一组功能，用于解析文件，构建代码的丰富语法和语义模型以及从该数据构建索引。 
    PSI提供了许多功能，从快速导航到文件，类型和符号，到代码完成窗口的内容以及查找用法，代码检查和代码重写，以实现快速修复或重构以及许多其他功能。
* IntelliJ平台包括用于多种语言的解析器和PSI模型，其可扩展性意味着可以添加对其他语言的支持。



## Plugins﻿

IntelliJ平台完全支持插件，并且JetBrains托管JetBrains插件存储库，可用于分发支持一种或多种产品的插件。
也可以托管您的存储库并单独分发插件。

插件可以通过多种方式扩展平台，从添加简单的菜单项到添加对完整语言，构建系统和调试器的支持。 
IntelliJ平台中的许多现有功能都是作为插件编写的，可以根据最终产品的需求包括或不包括。
有关更多详细信息，请参见《[快速入门指南](https://plugins.jetbrains.com/docs/intellij/basics.html)》。

IntelliJ平台是一个JVM应用程序，主要用Java和Kotlin编写。
您应该对这些语言，使用它们编写的大型库，它们的相关工具以及大型开源项目有丰富的经验，可以为基于IntelliJ平台的产品编写插件。
目前，不可能以非JVM语言扩展IntelliJ平台。

## Open Source﻿

The IntelliJ Platform is Open Source, under the [Apache License](https://upsource.jetbrains.com/idea-ce/file/idea-ce-ba0c8fc9ab9bf23a71a6a963cd84fc89b09b9fc8/LICENSE.txt), and [hosted on GitHub](https://github.com/JetBrains/intellij-community)

尽管本指南将IntelliJ平台称为一个单独的实体，但没有“ IntelliJ平台” GitHub存储库。
相反，该平台被视为与IntelliJ IDEA社区版几乎完全重叠，后者是IntelliJ IDEA Ultimate的免费和开源版本

[JetBrains/intellij-community](https://github.com/JetBrains/intellij-community)

IntelliJ平台的版本由IntelliJ IDEA社区版的相应发行版定义。
例如，要针对IntelliJ IDEA（2019.1.1）生成插件，内部版本＃191.6707.61意味着指定相同的内部版本号标签以从intellij-community存储库中获取正确的Intellij Platform文件。
有关与版本编号相对应的内部版本号的更多信息，请参见[内部版本号范围](https://plugins.jetbrains.com/docs/intellij/build-number-ranges.html)页面。

通常，基于IntelliJ平台的IDE将把intellij-community存储库作为Git子模块，并提供配置以描述intellij-community中的哪些插件，以及将构成产品的自定义插件。
这就是IDEA Ultimate团队的工作方式，他们为自定义插件和IntelliJ Platform本身贡献代码。



## 基于IntelliJ平台的IDE

IntelliJ平台是许多JetBrains IDE的基础。 
IntelliJ IDEA Ultimate是IntelliJ IDEA社区版的超集，但包括封闭源代码插件（[请参阅此功能比较](https://www.jetbrains.com/idea/features/editions_comparison_matrix.html)）。
同样，其他产品，例如WebStorm和DataGrip，都基于IntelliJ IDEA社区版，但是包含一组不同的插件，但不包括其他默认插件。
这使插件可以针对多个产品，因为每个产品都将包含基本功能以及IntelliJ IDEA Community Edition存储库中的一系列插件。

以下IDE基于IntelliJ平台：

- JetBrains IDEs
    - [AppCode](https://www.jetbrains.com/objc/)
    - [CLion](https://www.jetbrains.com/clion/)
    - [DataGrip](https://www.jetbrains.com/datagrip/)
    - [GoLand](https://www.jetbrains.com/go/)
    - [IntelliJ IDEA](https://www.jetbrains.com/idea/)
    - [MPS](https://www.jetbrains.com/mps/)
    - [PhpStorm](https://www.jetbrains.com/phpstorm/)
    - [PyCharm](https://www.jetbrains.com/pycharm/)
    - [Rider](https://plugins.jetbrains.com/docs/intellij/intellij-platform.html#rider)
    - [RubyMine](https://www.jetbrains.com/ruby/)
    - [WebStorm](https://www.jetbrains.com/webstorm/)
- [Android Studio](https://developer.android.com/studio/index.html) IDE from Google.
- [Comma](https://commaide.com/) IDE for Raku (formerly known as Perl 6)
- [CUBA Studio](https://www.cuba-platform.com/)



## 参与IntellJ平台贡献



