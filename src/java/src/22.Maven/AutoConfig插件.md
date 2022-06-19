## 需求分析

在一个应用中，我们总是会遇到一些参数，例如：

- 数据库服务器IP地址、端口、用户名；
- 用来保存上传资料的目录。
- 一些参数，诸如是否打开cache、加密所用的密钥名称等等。

这些参数有一个共性，那就是：*它们和应用的逻辑无关，只和当前环境、当前系统用户相关*。以下场景很常见：

- 在开发、测试、发布阶段，使用不同的数据库服务器；
- 在开发阶段，使用Windows的A开发者将用户上传的文件存放在`d:\my_upload`目录中，而使用Linux的B开发者将同样的文件存放在`/home/myname/my_upload`目录中。
- 在开发阶段设置`cache=off`，在生产环境中设置`cache=on`。

很明显，*这些参数不适合被“硬编码”在配置文件或代码中*。因为每一个从源码库中取得它们的人，都有可能需要修改它们，使之与自己的环境相匹配。

## 解决方案

### 运行时替换的placeholders

很多框架支持在运行时刻替换配置文件中的placeholder占位符。例如， Webx/Spring就有这个功能。

```xml
<services:property-placeholder />

<services:webx-configuration>
    <services:productionMode>${productionMode:true}</services:productionMode>
</services:webx-configuration>
```

在上面这个例子中，你可以在启动应用时，加上JVM参数：“`-DproductionMode=false|true`”来告诉系统用哪一种模式来工作。如果不指定，则取默认值“`true`”。

运行时替换placeholder是一种非常实用的技术，它有如下优缺点：

| 优点                                                         | 缺点                                                         |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| 配置文件是静态的、不变的。即使采用不同的参数值，你也不需要更改配置文件本身。你可以随时改变参数的值，只需要启动时指定不同的JVM参数、或指定不同的properties文件即可。这种配置对于应用程序各组件是透明的 —— 应用程序不需要做特别的编程，即可使用placeholders。 | 并非所有框架都支持这种技术。<br />支持该技术的框架各有不同的用法。例如：Spring和Log4j都支持placeholder替换，然则它们的做法是完全不同的。Spring通过`PropertyPlaceholderConfigurer`类来配置，而Log4j则需要在`DomConfigurator`中把参数传进去。 |

### 中心配置服务器（Config Server）

这也是一种运行时技术。它可以在运行时刻，将应用所需的参数推送到应用中。

**中心配置服务器的优缺点**

| 优点                                                         | 缺点                                                         |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| 它可以集中管理所有应用的配置，避免可能的错误；它可以在运行时改变参数的值，并推送到所有应用中。参数的更改可立即生效。 | 需要一套独立的服务器系统。性能、可用性（availability）都是必须考虑的问题。对应用不是透明的，有一定的侵入性。应用程序必须主动来配合该技术。因此，该技术不可能适用于所有情况，特别对于第三方提供的代码，很难使用该技术。为了连接到中心配置服务器，你仍然需要配置适当的IP、端口等参数。你需要用其它技术来处理这些参数（例如placeholders）。 |

## Maven Filtering机制

Maven提供了一种过滤机制，可以在资源文件被复制到目标目录的同时，替换其中的placeholders。

```
web-project
 │  pom.xml
 │
 └─src
     └─main
         ├─java
         ├─resources
         └─webapp
             └─WEB-INF
                     web.xml
```

在`pom.xml`中这样写：

```xml
<build>
    <filters>
        <filter>${user.home}/antx.properties</filter> 
    </filters>
    <resources>
        <resource>
            <directory>src/main/resources</directory> 
            <includes>
                <include>**.xml</include>
            </includes>
            <filtering>true</filtering>
        </resource>
        <resource>
            <directory>src/main/resources</directory>
            <excludes>
                <exclude>**.xml</exclude>
            </excludes>
        </resource>
    </resources>
    <plugins>
        <plugin>
            <artifactId>maven-war-plugin</artifactId>
            <configuration>
                <webResources>
                    <resource>
                        <directory>src/main/webapp</directory> 
                        <includes>
                            <include>WEB-INF/**.xml</include>
                        </includes>
                        <filtering>true</filtering>
                    </resource>
                    <resource>
                        <directory>src/main/webapp</directory>
                        <excludes>
                            <include>WEB-INF/**.xml</include>
                        </excludes>
                    </resource>
                </webResources>
            </configuration>
        </plugin>
    </plugins>
</build>
```

这段pom定义告诉maven：

1. 用指定的properties文件（`${user.home}/antx.properties`）中的值，替换文件中的placeholders
2. 过滤`src/main/resources/`目录中的所有xml文件，替换其中的placeholders。
3. 过滤`src/webapp/WEB-INF/`目录中的所有xml文件，替换其中的placeholders。

如果上述xml文件中，包含“`${xxx.yyy.zzz}`”这样的placeholders，将被替换成properties文件中的相应值。

和运行时替换placeholders方案相比，Maven Filtering是一个build时进行的过程。它的优缺点是：

**Maven Filtering机制的优缺点**

| 优点                                                         | 缺点                                                         |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| Maven filtering机制和应用所采用的技术、框架完全无关，对应用完全透明，通用性好。 | Maven filtering机制在build时刻永久性改变被过滤的配置文件的内容，build结束以后无法更改。这将导致一个问题：如果要改变配置文件的参数，必须获取源码并重新build。缺少验证机制。当某个placeholder拼写错误；当properties中的值写错；当某配置文件中新增了一个placeholder，而你的properties文件中没有对应的值时，maven不会提醒你。而这些错误往往被拖延到应用程序运行时才会被报告出来。 |

## AutoConfig机制

AutoConfig是一种类似于Maven Filtering的*build时刻的工具*。

这意味着该机制与应用所采用的技术、框架完全无关，对应用完全透明，具有良好的通用性。

同时，AutoConfig与运行时的配置技术并不冲突。

它可以和运行时替换的placeholders以及中心配置服务器完美并存，互为补充。



AutoConfig书写placeholder的方法和Maven Filtering机制完全相同。

换言之，Maven Filtering的配置文件模板（前例中的`/WEB-INF/**.xml`）可以不加修改地用在AutoConfig中。

然而，autoconfig成功克服了Maven Filtering的主要问题。

**Maven Filtering和AutoConfig的比较**

| 问题                              | Maven Filtering                                    | AutoConfig                                                   |
| :-------------------------------- | :------------------------------------------------- | :----------------------------------------------------------- |
| 如何修改配置文件的参数？          | Maven Filtering必须获得源码并重新build；           | 而AutoConfig不需要提取源码，也不需要重新build，即可改变*目标文件*中所有配置文件中placeholders的值。 |
| 如何确保placeholder替换的正确性？ | Maven Filtering不能验证placeholder值的缺失和错误； | 但AutoConfig可以对placeholder及其值进行检查。                |

## AutoConfig的设计

为了把事情说清楚，我们必须要定义两种角色：*开发者（Developer）*和*部署者（Deployer）*。

### **角色和职责**

| 角色名称 | 职责                                                         |
| :------- | :----------------------------------------------------------- |
| 开发者   | 定义应用所需要的properties，及其限定条件；提供包含placeholders的配置文件模板。 |
| 部署者   | 根据所定义的properties，提供符合限定条件的属性值。调用AutoConfig来生成目标配置文件。 |

例如，一个宠物店（petstore）的WEB应用中需要指定一个用来上传文件的目录。于是，

### **Petstore应用中的角色和职责**

| 开发者                                                       | 部署者                                                       |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| 开发者定义了一个property：`petstore.upload_dir`，限定条件为：“合法的文件系统的目录名”。 | 部署者取得petstore的二进制发布包，通过AutoConfig了解到，应用需要一个名为`petstore.upload_dir`目录名。部署者便指定一个目录给petstore，该目录名的具体值可能因不同的系统而异。AutoConfig会检验该值是否符合限定条件（是否为合法目录名），如果检验通过，就生成配置文件，并将其中的`${petstore.upload_dir}`替换成该目录名。 |

需要注意的是，一个“物理人”所对应的“角色”不是一成不变的。例如：某“开发者”需要试运行应用，此时，他就变成“部署者”。



### 分享二进制目标文件

假设现在有两个team要互相合作，team A的开发者创建了project A，而team B的开发者创建了project B。假定project B依赖于project A。如果我们利用maven这样的build工具，那么最显而易见的合作方案是这样的：

- Team A发布一个project A的版本到maven repository中。
- Team B从maven repository中取得project A的二进制目标文件。

这种方案有很多好处，

- 每个team都可以独立控制自己发布版本的节奏；
- Team之间的关系较松散，唯一的关系纽带就是maven repository。
- Team之间不需要共享源码。

然而，假如project A中有一些配置文件中的placeholders需要被替换，如果使用Maven Filtering机制，就会出现问题。因为Maven Filtering只能在project A被build时替换其中的placeholders，一旦project A被发布到repository中，team B的人将无法修改任何project A中的配置参数。除非team B的人取得project A的源码，并重新build。这将带来很大的负担。



然而，假如project A中有一些配置文件中的placeholders需要被替换，如果使用Maven Filtering机制，就会出现问题。因为Maven Filtering只能在project A被build时替换其中的placeholders，一旦project A被发布到repository中，team B的人将无法修改任何project A中的配置参数。除非team B的人取得project A的源码，并重新build。这将带来很大的负担。



AutoConfig解决了这个问题。因为当team B的人从maven repository中取得project A的二进制包时，仍然有机会修改其配置文件里的placeholders。Team B的人甚至不需要了解project A里配置文件的任何细节，AutoConfig会自动发现所有的properties定义，并提示编辑。



### 部署二进制目标文件

部署应用的人（即部署者、deployer）也从中受益。因为deployer不再需要亲手去build源代码，而是从maven repository中取得二进制目标文件即可。

从这个意义上讲，AutoConfig不应当被看成是一个build时的简单配置工具

，而是一个“*软件安装工具*”。如同我们安装一个Windows软件 —— 我们当然不需要从源码开始build它们，而是执行其安装程序，设定一些参数诸如安装目录、文档目录、可选项等。安装程序就会自动把软件设置好，确保软件可正确运行于当前的Windows环境中。





### AutoConfig特性列表

| 名称                           | 描述                                                         |
| :----------------------------- | :----------------------------------------------------------- |
| 两种用法                       | 既可独立使用（支持Windows和Unix-like平台）。也可以作为maven插件来使用。 |
| 对目标文件而不是源文件进行配置 | 可对同一个目标文件反复配置。配置时不依赖于项目源文件。支持嵌套包文件，例如：ear包含war，war又包含jar。高性能，特别对于嵌套的包文件。 |
| 验证和编辑properties           | 自动发现保存于war包、jar包、ear包中的properties定义。验证properties的正确性。交互式编辑properties。当配置文件中出现未定义的placeholders时，提示报错。 |

## AutoConfig的使用 —— 开发者指南

### 建立AutoConfig目录结构

和Maven Filtering不同的是，AutoConfig是针对目标文件的配置工具

因此AutoConfig关心的目录结构是*目标文件的目录结构*

不同的build工具，创建同一目标目录结构所需要的源文件的目录结构会各有不同。

本文仅以maven标准目录结构为例，来说明源文件的目录结构编排。

#### WAR包的目录结构

这里所说的war包，可以是一个以zip方式打包的文件，也可以是一个展开的目录。下面以maven标准目录为例，说明项目源文件和目标文件的目录结构的对比：



```
war-project（源目录结构）               -> war-project.war（目标目录结构）
 │  pom.xml
 │
 └─src
     └─main
         ├─java
         ├─resources                    -> /WEB-INF/classes
         │      file1.xml                      file1.xml
         │      file2.xml                      file2.xml
         │
         └─webapp                       -> /
             ├─META-INF                 -> /META-INF
             │  └─autoconf              -> /META-INF/autoconf 
             │        auto-config.xml          auto-config.xml 
             │
             └─WEB-INF                  -> /WEB-INF
                   web.xml                     web.xml
                   file3.xml                   file3.xml
```

1. */META-INF/autoconf*目录用来存放AutoConfig的描述文件，以及可选的模板文件。
2. `auto-config.xml`是用来指导AutoConfig行为的关键描述文件。

创建war包的AutoConfig机制，关键在于创建war目标文件中的`/META-INF/autoconf/auto-config.xml`描述文件。该描述文件对应的maven项目源文件为：`/src/main/webapp/META-INF/autoconf/auto-config.xml`。

####  JAR包的目录结构

这里所说的jar包，可以是一个以zip方式打包的文件，也可以是一个展开的目录。下面以maven标准目录为例，说明项目源文件和目标文件的目录结构的对比：

**JAR包的源文件和目标文件目录结构**

```
jar-project（源目录结构）               -> jar-project.jar（目标目录结构）
 │  pom.xml
 │
 └─src
     └─main
         ├─java
         └─resources                    -> /
             │  file1.xml                      file1.xml
             │  file2.xml                      file2.xml
             │
             └─META-INF                 -> /META-INF
                 └─autoconf             -> /META-INF/autoconf 
                       auto-config.xml         auto-config.xml 
```

1. /META-INF/autoconf目录用来存放AutoConfig的描述文件，以及可选的模板文件。
2. 创建jar包的AutoConfig机制，关键在于创建jar目标文件中的`/META-INF/autoconf/auto-config.xml`描述文件。该描述文件对应的maven项目源文件为：`/src/main/resources/META-INF/autoconf/auto-config.xml`。

#### 普通目录

AutoConfig也支持对普通文件目录进行配置。

```
directory
 │  file1.xml
 │  file2.xml
 │
 └─conf 
       auto-config.xml 
```

1. 默认情况下，AutoConfig在/conf目录中寻找AutoConfig的描述文件，以及可选的模板文件。



### 建立auto-config.xml描述文件

AutoConfig系统的核心就是`auto-config.xml`描述文件。该描述文件中包含两部分内容：

1. 定义properties：properties的名称、描述、默认值、约束条件等信息；
2. 指定包含placeholders的模板文件。

下面是`auto-config.xml`文件的样子：（以petstore应用为例）

```xml
<?xml version="1.0" encoding="UTF-8"?>
<config>
    <group>

        <property name="petstore.work"
                    description="应用程序的工作目录" /> 

        <property name="petstore.loggingRoot" 
                    defaultValue="${petstore.work}/logs"
                    description="日志文件目录" /> 

        <property name="petstore.upload"
                    defaultValue="${petstore.work}/upload"
                    description="上传文件的目录" /> 

        <property name="petstore.loggingLevel"
                    defaultValue="warn"
                    description="日志文件级别"> 

            <validator name="choice"
                         choice="trace, debug, info, warn, error" /> 
        </property>

    </group>
    <script>
        <generate template="WEB-INF/web.xml" /> 
        <generate template="WEB-INF/common/resources.xml" />
    </script>
</config>
```

#### 定义properties

```xml
<property
    name="..."
    [defaultValue="..."]
    [description="..."]
    [required="true|false"]
>
    <validator name="..." />
    <validator name="..." />
    ...
</property>
```

**定义property时可用的参数**

| 参数名                 | 说明                                                         |
| :--------------------- | :----------------------------------------------------------- |
| `name`                 | Property名称。                                               |
| `defaultValue`（可选） | 默认值。默认值中可包含对其它property的引用，如`${petstore.work}/logs`。 |
| `description`（可选）  | 对字段的描述，这个描述会显示给deployer，这对他理解该property非常重要。 |
| `required`（可选）     | 是否“必填”，默认为`true`。如果deployer未提供必填项的值，就会报错。 |

**定义property的验证规则**

**可用的property验证规则**

| 验证规则                                                     | 说明                                                         |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| `<validator name="boolean" />`                               | Property值必须为`true`或`false`。                            |
| `<validator name="choice"           choice="trace, debug, info, warn, error" />` | Property值必须为choice所定义的值之一。                       |
| `<validator** name="email" />`                               | Property值必须为合法的email格式。                            |
| `<validator name="fileExist"           [file="WEB-INF/web.xml"] />` | Property值必须为某个存在的文件或目录。如果指定了file，那就意味着property值所指的目录下，必须存在file所指的文件或子目录。 |
| `<validator name="hostExist" />`                             | Property值必须为合法的IP地址，或者可以解析得到的域名。       |
| `<validator name="keyword" />`                               | Property值必须为字母、数字、下划线的组合。                   |
| `<validator name="number" />`                                | Property值必须为数字的组合。                                 |
| `<validator name="regexp"           regexp="..."           [mode="exact|prefix|contain"] />` | Property值必须符合regexp所指的正则表达式。其中，mode为匹配的方法：完全匹配exact前缀匹配prefix包含contain如未指定mode，默认mode为contain。 |
| `<validator name="url"           [checkHostExist="false"]           [protocols="http, https"]           [endsWithSlash="true"] />` | Property值必须是合法URL。假如指定了`checkHostExist=true`，那么还会检查域名或IP的正确性；假如指定了protocols，那么URL的协议必须为其中之一；假如指定了`endsWithSlash=true`，那么URL必须以/结尾。 |

#### 生成配置文件的指令

描述文件中，每个`<generate>`标签指定了一个包含*placeholders*的配置文件模板，具体格式为：

```
<generate
    template="..."
    [destfile="..."]
    [charset="..."]
    [outputCharset="..."]
>
```

**生成配置文件的指令参数**

| 参数名                  | 说明                                                         |
| :---------------------- | :----------------------------------------------------------- |
| `template`              | 需要配置的模板名。模板名为相对路径，相对于当前jar/war/ear包的根目录。 |
| `destfile`（可选）      | 目标文件。如不指定，表示目标文件和模板文件相同。             |
| `charset`（可选）       | 模板的字符集编码。XML文件不需要指定`charset`，因为AutoConfig可以自动取得XML文件的字符集编码；对其它文件必须指定charset。 |
| `outputCharset`（可选） | 目标文件的输出字符集编码。如不指定，表示和模板charset相同。  |



### 建立模板文件

#### 模板文件的位置

定义完`auto-config.xml`描述文件以后，就可以创建模板了。模板放在哪里呢？举例说明。

假设在一个典型的WEB应用中，你的`auto-config.xml`中包含指定了如下模板：

```xml
<?xml version="1.0" encoding="UTF-8"?>
<config>
    <group>
        ...
    </group>
    <script>
        <generate template="WEB-INF/classes/file1.xml" />
        <generate template="WEB-INF/classes/file2.xml" />
        <generate template="WEB-INF/file3.xml" />
    </script>
</config>
```

那么，你可以把`file1.xml`、`file2.xml`、`file3.xml`放在下面的位置：

```
war-project（源目录结构）               -> war-project.war（目标目录结构）
 │  pom.xml
 │
 └─src
     └─main
         ├─java
         ├─resources                    -> /WEB-INF/classes
         │     file1.xml                       file1.xml - 建议放在这里
         │     file2.xml                       file2.xml - 建议放在这里
         │
         └─webapp
             ├─META-INF
             │  └─autoconf
             │      │  auto-config.xml
             │      │
             │      └─WEB-INF           -> /WEB-INF
             │          │ file3.xml            file3.xml - 也可以放在这里
             │          │
             │          └─classes       -> /WEB-INF/classes
             │                file1.xml        file1.xml - 也可以放在这里
             │                file2.xml        file2.xml - 也可以放在这里
             │
             └─WEB-INF                  -> /WEB-INF
                   file3.xml                   file3.xml - 建议放在这里
```

AutoConfig的寻找模板的逻辑是：

- 如果在`auto-config.xml`所在的目录下发现模板文件，就使用它；
- 否则在包的根目录中查找模板文件；如果两处均未找到，则报错。

#### 模板的写法

书写模板是很简单的事，你只要：

- 把需要配置的点替换成placeholder：“`${property.name}`”。当然，你得确保property.name被定义在`auto-config.xml`中。
- 假如模板中包含*不希望被替换的*运行时的placeholder“*`$`*`{...}`”，需要更改成“*`${D}`*`{...}`” 。



```xml
...
<context-param>
    <param-name>loggingRoot</param-name>
    <param-value>${petstore.loggingRoot}</param-value>
</context-param>
<context-param>
    <param-name>loggingLevel</param-name>
    <param-value>${petstore.loggingLevel}</param-value>
</context-param>
...
${D}{runtime.placeholder}
```

此外，AutoConfig模板其实是由Velocity模板引擎来渲染的。因此，所有的placeholder必须能够通过velocity的语法。

**使用不符合velocity语法的placeholders**

例如，下面的placeholder被velocity看作非法：

```
${my.property.2}
```

解决的办法是，改写成如下样式：

```
${my_property_2}
```

## AutoConfig的使用 —— 部署者指南

部署者有两种方法可以使用AutoConfig：

- 在命令行上直接运行。
- 在maven中调用AutoConfig plugin。

### 在命令行中使用AutoConfig

#### 取得可执行文件

AutoConfig提供了Windows以及Unix-like（Linux、Mac OS等）等平台上均可使用的native可执行程序。可执行程序文件被发布在Maven repository中。

如果你已经配置好了maven，那么可以让maven来帮你下载目标文件。

**例 13.12. 让maven帮忙下载AutoConfig可执行文件**

请创建一个临时文件：`pom.xml`。

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
    <parent>
        <groupId>com.alibaba.citrus.tool</groupId>
        <artifactId>antx-parent</artifactId>
        <version>1.2</version> 
    </parent>
    <modelVersion>4.0.0</modelVersion>
    <artifactId>temp</artifactId>
</project>
```

文件中的parent pom的版本号（1.2）决定了你要取得的AutoConfig的版本号。

然后在命令行上执行如下命令：

```
mvn dependency:copy
```

这样就取得了两个文件：

- `autoconfig-1.2.tgz`
- `autoexpand-1.2.tgz` - AutoExpand是另一个小工具。它是用来展开war、jar、ear包的。关于AutoExpand的详情，请见[第 14 章 *AutoExpand工具使用指南*](https://jjm2473.github.io/citrus-doc/autoexpand.html)。

你也可以直接去maven repository中手工下载以上两个包：

- http://repo1.maven.org/maven2/com/alibaba/citrus/tool/antx-autoconfig/1.2/antx-autoconfig-1.2.tgz
- http://repo1.maven.org/maven2/com/alibaba/citrus/tool/antx-autoexpand/1.2/antx-autoexpand-1.2.tgz

取得压缩包以后，可以用下面的命令来展开并安装工具。

**展开并安装工具**

| Unix-like系统                                                | Windows系统                                                  |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| `tar zxvf autoconfig-1.2.tgz tar zxvf autoexpand-1.2.tgz cp autoconfig /usr/local/bin cp autoexpand /usr/local/bin` | `tar zxvf autoconfig-1.2.tgz tar zxvf autoexpand-1.2.tgz copy autoconfig.exe c:\windows\system32 copy autoexpand.exe c:\windows\system32` |

#### 执行AutoConfig命令

取得可执行文件以后，就可以试用一下：在命令行上输入**autoconfig**。不带参数的**autoconfig**命令会显示出如下帮助信息。

**例 13.13. AutoConfig的帮助信息**

```
$ autoconfig
Detected system charset encoding: UTF-8
If your can't read the following text, specify correct one like this: 
  autoconfig -c mycharset

使用方法：autoconfig [可选参数] [目录名|包文件名]
                
可选参数：
 -c,--charset                输入/输出编码字符集
 -d,--include-descriptors
                             包含哪些配置描述文件，例如：conf/auto-config.xml，可使用*、**、?通配符，如有多项，用逗号分隔
 -D,--exclude-descriptors    排除哪些配置描述文件，可使用*、**、?通配符，如有多项，用逗号分隔
 -g,--gui                    图形用户界面（交互模式）
 -h,--help                   显示帮助信息
 -i,--interactive            交互模式：auto|on|off，默认为auto，无参数表示on
 -I,--non-interactive        非交互模式，相当于--interactive=off
 -n,--shared-props-name      共享的属性文件的名称
 -o,--output                 输出文件名或目录名
 -P,--exclude-packages       排除哪些打包文件，可使用*、**、?通配符，如有多项，用逗号分隔
 -p,--include-packages
                             包含哪些打包文件，例如：target/*.war，可使用*、**、?通配符，如有多项，用逗号分隔
 -s,--shared-props           共享的属性文件URL列表，以逗号分隔
 -T,--type                   文件类型，例如：war, jar, ear等
 -t,--text                   文本用户界面（交互模式）
 -u,--userprop               用户属性文件
 -v,--verbose                显示更多信息

总耗费时间：546毫秒
```

**最简单的AutoConfig命令**

```
autoconfig petstore.war
```

无论`petstore.war`是一个zip包还是目录，AutoConfig都会正确地生成其中的配置文件。

### 在maven中使用AutoConfig

AutoConfig也可以通过maven plugin来执行。

这种方式使用方式，方便了开发者试运行并测试应用程序。开发者可以在build项目的同时，把AutoConfig也配置好。然而对于非开发的应用测试人员、发布应用的系统管理员来说，最好的方法是使用独立可执行的AutoConfig来配置应用的二进制目标文件。

为了使用maven插件，你需要修改项目的`pom.xml`来设定它。请注意，一般来说，不要在parent `pom.xml`中设定AutoConfig，因为这个设置会作用在每个子项目上，导致不必要的AutoConfig执行。只在生成最终目标文件的子项目`pom.xml`中设定AutoConfig就可以了。例如，对于一个web项目，你可以在生成war包的子项目上设置AutoConfig plugin。

**在pom.xml中设定AutoConfig plugin**

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
    ...
    <properties>
        ...
        <!-- 定义autoconfig的版本，建议将此行写在parent pom.xml中。 -->
        <autoconfig-plugin-version>1.2</autoconfig-plugin-version>
    </properties>
    ...
    <build>
        <plugins>
            <plugin>
                <groupId>com.alibaba.citrus.tool</groupId>
                <artifactId>autoconfig-maven-plugin</artifactId>
                <version>${autoconfig-plugin-version}</version>
                <configuration>
                    <!-- 要进行AutoConfig的目标文件，默认为${project.artifact.file}。 
                    <dest>${project.artifact.file}</dest>
                    -->
                    <!-- 配置后，是否展开目标文件，默认为false，不展开。 
                    <exploding>true</exploding>
                    -->
                    <!-- 展开到指定目录，默认为${project.build.directory}/${project.build.finalName}。 
                    <explodedDirectory>
                        ${project.build.directory}/${project.build.finalName}
                    </explodedDirectory>
                    -->
                </configuration>
                <executions>
                    <execution>
                        <phase>package</phase>
                        <goals>
                            <goal>autoconfig</goal>
                        </goals>
                    </execution>
                </executions>
            </plugin>
        </plugins>
    </build>
</project>
```

这样，每次执行`mvn package`或者`mvn install`时，都会激活AutoConfig，对package目标文件进行配置。

想要避免AutoConfig，只需要一个额外的命令行参数：

**避免执行AutoConfig**

```
mvn install –Dautoconfig.skip
```

### 运行并观察AutoConfig的结果

第一次执行AutoConfig，无论通过何种方式（独立命令行或maven插件），

AutoConfig都会提示你修改user properties文件，以提供所需要的properties值。

AutoConfig提供了一套基于文本的交互式界面来编辑这些properties。

**交互式编辑properties**

```
╭───────────────────────┈┈┈┈
│
│ 您的配置文件需要被更新：
│
│ file:/.../antx.properties
│
│ 这个文件包括了您个人的特殊设置，
│ 包括服务器端口、您的邮件地址等内容。
│
└───────┈┈┈┈┈┈┈┈┈┈┈

 如果不更新此文件，可能会导致配置文件的内容不完整。
 您需要现在更新此文件吗? [Yes][No] y
```

当你通过交互式界面填写了所有properties的值，并通过了AutoConfig的验证以后，AutoConfig就开始生成配置文件：

```
即将保存到文件"file:/.../antx.properties"中, 确定? [Yes][No] y

╭───────────────────────┈┈┈┈
│ 保存文件 file:/.../antx.properties...
│┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
│petstore.loggingLevel  = warn
│petstore.loggingRoot   = ${petstore.work}/logs
│petstore.upload        = ${petstore.work}/upload
│petstore.work          = /tmp
└───────┈┈┈┈┈┈┈┈┈┈┈
 已保存至文件: file:/.../antx.properties
Loading file:/.../antx.properties
<jar:file:/.../Work/my/apps/petstore-webx3/target/petstore.war!/>
    Generating WEB-INF/web.xml [UTF-8] => WEB-INF/web.xml [UTF-8]

<jar:file:/.../Work/my/apps/petstore-webx3/target/petstore.war!/>
    Generating WEB-INF/common/resources.xml [UTF-8] => WEB-INF/common/resources.xml [UTF-8]

<jar:file:/.../Work/my/apps/petstore-webx3/target/petstore.war!/>
    Generating log file: META-INF/autoconf/auto-config.xml.log

Expanding: /.../Work/my/apps/petstore-webx3/target/petstore.war
       To: /.../Work/my/apps/petstore-webx3/target/petstore
done.
```

假如发现模板中某个placeholder，并未在`auto-config.xml`中定义，就会出现以下错误：

```
ERROR - Undefined placeholders found in template:
- Template:   META-INF/autoconf/WEB-INF/web.xml
- Descriptor: META-INF/autoconf/auto-config.xml
- Base URL:   file:/.../Work/my/apps/petstore-webx3/target/petstore/
---------------------------------------------------------------
-> petstore.loggingRoot
---------------------------------------------------------------
```

出现错误以后，Maven会报错，并停止build过程。假如你不希望maven停止，可以用下面的命令来执行maven：

**避免maven因为placeholder未定义而停止**

```
mvn ... –Dautoconfig.strict=false
```

AutoConfig会生成一个日志文件，就在`auto-config.xml`所在的目录下，名字为：`auto-config.xml.log`。

**AutoConfig所生成的日志文件**

```
Last Configured at: Fri Jun 18 13:54:22 CST 2010

Base URL: file:/.../Work/my/apps/petstore-webx3/target/petstore/
Descriptor: META-INF/autoconf/auto-config.xml

Generating META-INF/autoconf/WEB-INF/web.xml [UTF-8] => WEB-INF/web.xml [UTF-8]
Generating META-INF/autoconf/WEB-INF/common/resources.xml [UTF-8] => WEB-INF/common/resources.xml [UTF-8]
```

最后，让我们查看一下AutoConfig所生成的文件，其中所有的placeholders应当被替换成你所提供的值了。

**AutoConfig生成的结果**

```
...
<context-param>
    <param-name>loggingRoot</param-name>
    <param-value>/tmp/logs</param-value>
</context-param>
<context-param>
    <param-name>loggingLevel</param-name>
    <param-value>warn</param-value>
</context-param>
...
${runtime.placeholder}
```

### 共享properties文件

当需要配置的内容越来越多时，即使使用AutoConfig这样的机制，也会变得不胜其烦。

假如你的项目包含了好几个模块，而你只负责其中的一个模块。

一般来说，你对其它模块的配置是什么并不清楚，事实上你也懒得去关心。

但是你为了运行这个项目，你不得不去配置这些模块。

假如模块A就是一个你不想关心的模块，但为了运行它，你需要告诉模块A一些参数：数据库连接的参数、域名、端口、文件目录、搜索引擎……可你并不清楚这些参数应该取什么值。



好在AutoConfig提供了一个共享properties文件的方法。

**共享的properties文件**

你可以创建一系列文件：`module-a-db.properites`，`module-a-searchengine.properties`等等。每个文件中都包含了某个运行环境中的关于module A模块的配置参数。

现在，你可以不关心module A了！你只要使用下面的命令：

`-s`参数代表“共享的properties文件”。

```
autoconfig -s module-a-db.properties,module-a-searchengine.properties ……
```

同时，你的`antx.properties`也被简化了，因为这里只会保存你定义的配置项，而不会包含共享的配置项。

### 共享整个目录

假如共享的文件很多的话，AutoConfig还有一个贴心的功能，你可以把这些文件按目录来组织：

```
shared-properties/
 ├─test/                                 // 测试环境的共享配置
 │    module-a-db.properties
 │    module-a-searchengine.properties
 │    module-b.properties
 └─prod/                                 // 生产环境的共享配置
       module-a-db.properties
       module-a-searchengine.properties
       module-b.properties
```

然后，你可以直接在AutoConfig中引用目录：

```
autoconfig -s shared-propertes/test/ ……
```

AutoConfig就会为你装载这个目录下的所有共享配置文件。（注意，*目录必须以斜杠“/”结尾*）

#### 将共享目录放在http、https或ssh服务器上

AutoConfig还支持从http、https或ssh服务器上取得共享配置文件，只需要将前面例子中的文件名改成http或ssh的URI就可以了：

```
autoconfig -s http://share.alibaba.com/shared-propertes/test/ ……
autoconfig -s http://myname@share.alibaba.com/shared-propertes/test/ ……
autoconfig -s https://share.alibaba.com/shared-propertes/test/ ……
autoconfig -s https://myname@share.alibaba.com/shared-propertes/test/ ……
autoconfig -s ssh://myname@share.alibaba.com/shared-propertes/test/ ……
```

由于Subversion、Git服务器是支持HTTP/HTTPS协议的，因此将properties文件存放在Subversion或Git服务器上，也是一个极好的办法。由于采用了Subversion或Git，properties文件的版本管理问题也被一举解决了。

需要注意的是，访问http和ssh有可能需要验证用户和密码。当需要验证时，AutoConfig会提示你输入用户名和密码。输入以后，密码将被保存在*`$HOME/passwd.autoconfig`*文件中，以后就不需要重复提问了。

#### 在多种配置项中切换

当你使用前文所述的`autoconfig –s`命令来生成`antx.properties`文件时，你会发现`antx.properties`中增加了几行特别的内容：

**包含共享文件、目录信息的`antx.properties`文件**

```
antx.properties.default  = http://share.alibaba.com/shared-propertes/test/
```

如果你在`-s`参数中指定了多项共享properties文件或目录，那么`antx.properties`中将会这样：

```
antx.properties.default.1  = http://share.alibaba.com/shared-propertes/test/
antx.properties.default.2  = file:/shared-properties/test/my-1.properites
antx.properties.default.3  = file:/shared-properties/test/my-2.properites
```

事实上，AutoConfig还支持多组共享配置，请试用下面的命令：

 **使用多组共享配置**

```
autoconfig -s http://share.alibaba.com/shared-propertes/test/ -n test ……
```

为当前共享配置定义一个名字，以后可以用这个名字来简化命令。

antx.properties就会是这个样子：

```
antx.properties = test
antx.properties.test = http://share.alibaba.com/shared-propertes/test/
```

```
autoconfig -s http://share.alibaba.com/shared-propertes/prod/ -n prod ……
```

antx.properties就会变成这个样子：

```
antx.properties = prod
antx.properties.test = http://share.alibaba.com/shared-propertes/test/
antx.properties.prod = http://share.alibaba.com/shared-propertes/prod/
```

以后再执行，就不需要再指定`-s`参数了，只需用`-n`参数选择一组共享properties文件即可。例如：

```
autoconfig -n prod ……                      // 使用prod生产环境的参数
autoconfig -n test ……                      // 使用test测试环境的参数
autoconfig  ……                             // 不指定，则使用最近一次所选择的共享文件
```

### AutoConfig常用命令

下面罗列了AutoConfig的常用的命令及参数：

#### 指定交互式界面的charset

一般不需要特别指定charset，除非AutoConfig自动识别系统编码出错，导致显示乱码。

```
运行AutoConfig独立可执行程序	
autoconfig ... -c GBK
运行AutoConfig maven插件	
mvn ... -Dautoconfig.charset=GBK


```

#### 指定交互模式

默认情况下，交互模式为自动（auto）。仅当user properties中的值不满足auto-config.xml中的定义时，才会交互式地引导用户提供properties值。

但你可以强制打开交互模式：

```
运行AutoConfig独立可执行程序	
autoconfig ... –i
autoconfig ... –i on
```

```
运行AutoConfig maven插件	
mvn ... -Dautoconfig.interactive
mvn ...  -Dautoconfig.interactive=true
```

或强制关闭交互模式：

```
运行AutoConfig独立可执行程序	
autoconfig ... –I
autoconfig ... –i off
运行AutoConfig maven插件	
mvn ...  -Dautoconfig.interactive=false
```

#### 指定user properties

默认情况下，AutoConfig会按下列顺序查找user properties：

1. `当前目录/antx.properties`
2. `当前用户HOME目录/antx.properties`

但你可以指定一个自己的properties文件，用下面的命令：

```
运行AutoConfig独立可执行程序	
autoconfig ... –u my.props
运行AutoConfig maven插件	
mvn ... -Dautoconfig.userProperties=my.props
```



#### 指定输出文件

默认情况下，AutoConfig所生成的配置文件以及日志信息会直接输出到当前包文件或目录中。例如以下命令会改变`petstore.war`的内容：

```
autoconfig petstore.war
```

但你可以指定另一个输出文件或目录，这样，原来的文件或目录就不会被修改：

```
运行AutoConfig独立可执行程序	
autoconfig petstore.war –o petstore-configured.war
运行AutoConfig maven插件	不适用
```

#### 避免执行AutoConfig

将AutoConfig和maven package phase绑定以后，每次build都会激活AutoConfig。假如你想跳过这一步，只需要下面的命令：

```
mvn ... -Dautoconfig.skip
```

#### 避免中断maven build

```
mvn ... -Dautoconfig.strict=false
```

AutoConfig是一个简单而有用的小工具，弥补了Maven Filtering及类似机制的不足。但它还有不少改进的余地。

- 界面不够直观。如果能够通过GUI或WEB界面来配置，就更好了。
- Properties validator目前不易扩展。
- 缺少集成环境的支持。





[COPY LINK](https://jjm2473.github.io/citrus-doc/autoconfig.html)