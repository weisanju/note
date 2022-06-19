## Introduction to Archetypes

### What is Archetype?

简而言之，原型是一个Maven项目模板工具包

原型被定义为*原始模式或模型，从中可以制造所有其他相同类型的东西*。

这个名字很合适，因为我们试图提供一种机制，这种机制提供一种生成Maven项目的一致方法。

原型将帮助作者为用户创建 Maven 项目模板，并为用户提供生成这些项目模板的参数化版本的方法。

使用原型提供了一种很好的方法，可以以与项目或组织采用的最佳实践一致的方式快速支持开发人员。



在 Maven 项目中，我们使用*archetypes* ，通过提供一个演示 Maven 许多功能的示例项目，同时向新用户介绍 Maven 采用的最佳实践，尝试让用户尽快启动和运行。

在几秒钟内，新用户可以有一个正在工作的Maven项目，用作调查Maven中更多功能的跳板。



 We have also tried to make the Archetype mechanism additive, and by that we mean allowing portions of a project to be captured in an archetype so that pieces or aspects of a project can be added to existing projects.



一个很好的例子是 Maven site archetype。

您已使用快速入门原型生成工作项目，然后可以使用该现有项目中的站点原型快速为该项目创建站点

你可以用原型做任何类似的事情。

您可能希望在组织内标准化 J2EE 开发，因此您可能希望为 EJB、WAR 或 Web 服务提供原型

.在组织的存储库中创建并部署这些原型后，组织内的所有开发人员都可以使用它们。

### Using an Archetype

To create a new project based on an Archetype, you need to call `mvn archetype:generate` goal, like the following:

```xml
mvn archetype:generate
```

Please refer to [Archetype Plugin page](https://maven.apache.org/archetype/maven-archetype-plugin/usage.html).

### Provided Archetypes

Maven provides several Archetype artifacts:

| Archetype ArtifactIds       | Description                                                  |
| :-------------------------- | :----------------------------------------------------------- |
| maven-archetype-archetype   | An archetype to generate a sample archetype project.         |
| maven-archetype-j2ee-simple | An archetype to generate a simplifed sample J2EE application. |
| maven-archetype-mojo        | An archetype to generate a sample a sample Maven plugin.     |
| maven-archetype-plugin      | An archetype to generate a sample Maven plugin.              |
| maven-archetype-plugin-site | An archetype to generate a sample Maven plugin site.         |
| maven-archetype-portlet     | An archetype to generate a sample JSR-268 Portlet.           |
| maven-archetype-quickstart  | An archetype to generate a sample Maven project.             |
| maven-archetype-simple      | An archetype to generate a simple Maven project.             |
| maven-archetype-site        | An archetype to generate a sample Maven site which demonstrates some of the supported document types like APT, XDoc, and FML and demonstrates how to i18n your site. |
| maven-archetype-site-simple | An archetype to generate a sample Maven site.                |
| maven-archetype-webapp      | An archetype to generate a sample Maven Webapp project.      |

For more information on these archetypes, please refer to the [Maven Archetype Bundles page](https://maven.apache.org/archetypes/index.html).

### What makes up an Archetype?

Archetypes are packaged up in a JAR and they consist of the archetype metadata which describes the contents of archetype, and a set of [Velocity](http://velocity.apache.org/) templates which make up the prototype project. If you would like to know how to make your own archetypes, please refer to our [Guide to creating archetypes](https://maven.apache.org/guides/mini/guide-creating-archetypes.html).

原型被打包在JAR中，

* 它们由描述原型内容的原型元数据和构成原型项目的一组[Velocity](http://velocity.apache.org/)模板组成。

如果您想知道如何制作自己的原型，请参阅我们的 [Guide to creating archetypes](https://maven.apache.org/guides/mini/guide-creating-archetypes.html).





## Guide to Creating Archetypes

创建原型是一个非常直接的过程。原型是一个非常简单的工件，其中包含要创建的项目原型。原型由以下部分组成：

- an [archetype descriptor](https://maven.apache.org/archetype/archetype-models/archetype-descriptor/archetype-descriptor.html) (`archetype-metadata.xml` in directory: `src/main/resources/META-INF/maven/`). 它列出了将包含在原型中的所有文件，并对它们进行分类，以便原型生成机制可以正确处理它们。
- the prototype files that are copied by the archetype plugin (directory: `src/main/resources/archetype-resources/`)
- the prototype pom (`pom.xml` in: `src/main/resources/archetype-resources`)
- a pom for the archetype (`pom.xml` in the archetype's root directory).

To create an archetype follow these steps:

### Create a new project and pom.xml for the archetype artifact

```xml
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
 
  <groupId>my.groupId</groupId>
  <artifactId>my-archetype-id</artifactId>
  <version>1.0-SNAPSHOT</version>
  <packaging>maven-archetype</packaging>
 
  <build>
    <extensions>
      <extension>
        <groupId>org.apache.maven.archetype</groupId>
        <artifactId>archetype-packaging</artifactId>
        <version>3.1.1</version>
      </extension>
    </extensions>
  </build>
</project>
```

All you need to specify is a `groupId`, `artifactId` and `version`.

 These three parameters will be needed later for invoking the archetype via `archetype:generate` from the commandline.

### Create the archetype descriptor

The [archetype descriptor](https://maven.apache.org/archetype/archetype-models/archetype-descriptor/archetype-descriptor.html) is a file called `archetype-metadata.xml` which must be located in the `src/main/resources/META-INF/maven/` directory. An example of an archetype descriptor can be found in the quickstart archetype:

```xml
<archetype-descriptor
        xmlns="http://maven.apache.org/plugins/maven-archetype-plugin/archetype-descriptor/1.1.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
        xsi:schemaLocation="http://maven.apache.org/plugins/maven-archetype-plugin/archetype-descriptor/1.1.0 https://maven.apache.org/xsd/archetype-descriptor-1.1.0.xsd"
        name="quickstart">
    <fileSets>
        <fileSet filtered="true" packaged="true">
            <directory>src/main/java</directory>
        </fileSet>
        <fileSet>
            <directory>src/test/java</directory>
        </fileSet>
    </fileSets>
</archetype-descriptor>
```

1. The attribute `name` tag should be the same as the `artifactId` in the archetype `pom.xml`.

2. The boolean attribute `partial` show if this archetype is representing a full Maven project or only parts.

The `requiredProperties`, `fileSets` and `modules` tags represent the differents parts of the project:

- `<requiredProperties>` : List of required properties to generate a project from this archetype
- `<fileSets>` : File sets definition
- `<modules>` : Modules definition

At this point one can only specify individual files to be created but not empty directories.

Thus the quickstart archetype shown above defines the following directory structure:

```
archetype
|-- pom.xml
`-- src
    `-- main
        `-- resources
            |-- META-INF
            |   `-- maven
            |       `--archetype-metadata.xml
            `-- archetype-resources
                |-- pom.xml
                `-- src
                    |-- main
                    |   `-- java
                    |       `-- App.java
                    `-- test
                        `-- java
                            `-- AppTest.java
```

### Create the prototype files and the prototype pom.xml

The next component of the archetype to be created is the prototype `pom.xml`. Any `pom.xml` will do, just don't forget to the set `artifactId` and `groupId` as variables ( `${artifactId}` / `${groupId}` ). 

Both variables will be initialized from the commandline when calling `archetype:generate`.

```xml
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
 
    <groupId>${groupId}</groupId>
    <artifactId>${artifactId}</artifactId>
    <version>${version}</version>
    <packaging>jar</packaging>
 
    <name>${artifactId}</name>
    <url>http://www.myorganization.org</url>
 
    <dependencies>
        <dependency>
                <groupId>junit</groupId>
                <artifactId>junit</artifactId>
                 <version>4.12</version>
                <scope>test</scope>
        </dependency>
    </dependencies>
</project>
```

###  Install the archetype and run the archetype plugin

```
mvn install
```



现在，您已经创建了一个原型，可以使用以下命令在本地系统上尝试它。

在此命令中，您需要指定有关要使用的原型的完整信息（其 groupId、其 artifactId、其version）以及有关要创建的新项目的信息（artifactId 和 groupId）。不要忘记包含原型的版本（if you don't include the version，you archetype creation may fail with a message that version:RELEASE was not found）

```
mvn archetype:generate                                  \
  -DarchetypeGroupId=<archetype-groupId>                \
  -DarchetypeArtifactId=<archetype-artifactId>          \
  -DarchetypeVersion=<archetype-version>                \
  -DgroupId=<my.groupid>                                \
  -DartifactId=<my-artifactId>
```



### Alternative way to start creating your Archetype

```shell
mvn archetype:generate
  -DgroupId=[your project's group id]
  -DartifactId=[your project's artifact id]
  -DarchetypeGroupId=org.apache.maven.archetypes
  -DarchetypeArtifactId=maven-archetype-archetype
```

```shell
mvn archetype:generate  -DgroupId="com.alibaba.cola.demo.service" -DartifactId=demo-service -Dversion="1.0.0-SNAPSHOT" -Dpackage"=com.alibaba.demo" -DarchetypeArtifactId="cola-framework-archetype-servic"e -DarchetypeGroupId="com.alibaba.cola" -DarchetypeVersion="4.0.1"
```

