# Maven Assembly Plugin

## 简介

[首页](http://maven.apache.org/plugins/maven-assembly-plugin/index.html)

Maven的Assembly Plugin使开发人员能够将项目输出组合到一个可分发的存档中，该存档还包含依赖项，模块，站点文档和其他文件。

可以 使用 预制的 装配描述符 轻松构建 装配体

这些描述符处理许多常见的操作，这些描述符处理许多常见的操作，例如将项目的工件和生成的文档打包到[单个zip归档文件，另外，您的项目可以提供自己的descriptor，并可以对依赖项，模块，文件集和单个文件打包在程序集中。

目前，它可以创建以下格式的发行版：-

- zip
- tar
- tar.gz (or tgz)
- tar.bz2 (or tbz2)
- tar.snappy
- tar.xz (or txz)
- jar
- dir
- war
- 以及已为ArchiveManager配置的任何其他格式

如果您的项目想要将工件打包在uber-jar中，则Assembly插件仅提供基本支持。
要进行更多控制，请使用[Maven Shade插件]（http://maven.apache.org/plugins/maven-shade-plugin/）。



要在Maven中使用Assembly Plugin，您只需要：

- 选择或编写要使用的 组装描述符文件
- 在项目的`pom.xml`中配置程序集插件，然后-
- `mvn assembly:single`

要编写自己的自定义程序集，您需要参考[程序集描述符格式](http://maven.apache.org/plugins/maven-assembly-plugin/assembly.html)参考。

## What is an Assembly?

An "assembly"  是一组文件，目录和依赖项，它们被组合成存档格式并进行分发，例如，假设一个Maven项目定义了一个同时包含控制台应用程序和Swing应用程序的JAR工件。这样的项目可以定义两个“程序集”，将应用程序与一组不同的支持脚本和依赖项集捆绑在一起。
一个程序集将是控制台应用程序的程序集，另一个程序集将是一个捆绑有稍微不同的依赖项的Swing应用程序。

The Assembly Plugin 插件提供了一种描述符格式,该格式允许您定义项目中文件和目录的任意程序集。

如果您的Maven项目包含目录“ src / main / bin”，则可以指示程序集插件将该目录的内容复制到程序集的“ bin”目录中，并更改目录中文件的权限。 
“ bin”目录进入UNIX模式755。用于配置此行为的参数通过[程序集描述符](http://maven.apache.org/plugins/maven-assembly-plugin/assembly)提供给程序集插件

## Goals

The main goal in the assembly plugin is the [single](http://maven.apache.org/plugins/maven-assembly-plugin/single-mojo.html) goal. It is used to create all assemblies.

For more information about the goals that are available in the Assembly Plugin, see [the plugin documentation page](http://maven.apache.org/plugins/maven-assembly-plugin/plugin-info.html).

assembly plugin 主要目标是[single](http://maven.apache.org/plugins/maven-assembly-plugin/single-mojo.html)目标。它用于创建所有程序集。
有关Assembly Plugin中可用目标的更多信息，请参见[插件文档页面](http://maven.apache.org/plugins/maven-assembly-plugin/plugin-info.html)。

## Usage

### Configuration

使用预定义的装配体描述符之一，请配置要与`<descriptorRefs>`
如果要使用自定义程序集描述符，则可以使用  `<descriptors> <descriptor>` 参数来配置描述符的路径。

请注意，对程序集插件的一次调用 实际上可以从多个描述符生成程序集，从而使您能够最大程度地自定义项目所生成的二进制文件套件。
创建程序集后，使用 project 名称 + assemblyId 名称

**jar example**

```xml
<project>
  [...]
  <build>
    [...]
    <plugins>
      <plugin>
        <!-- NOTE: We don't need a groupId specification because the group is
             org.apache.maven.plugins ...which is assumed by default.
         -->
        <artifactId>maven-assembly-plugin</artifactId>
        <version>3.3.0</version>
        <configuration>
          <descriptorRefs>
            <descriptorRef>jar-with-dependencies</descriptorRef>
          </descriptorRefs>
        </configuration>
        [...]
</project>
```

Assembly Plugin允许您一次指定多个描述符引用，以在一次调用中生成多种类型的程序集。
另外，我们在src / assembly目录中创建了一个名为src.xml的自定义程序集描述符

**使用文件**

```xml
<project>
  [...]
  <build>
    [...]
    <plugins>
      <plugin>
        <artifactId>maven-assembly-plugin</artifactId>
        <version>3.3.0</version>
        <configuration>
          <descriptors>
            <descriptor>src/assembly/src.xml</descriptor>
          </descriptors>
        </configuration>
        [...]
</project>
```

**绑定到 构建阶段**

```xml
<project>
  [...]
  <build>
    [...]
    <plugins>
      <plugin>
        <artifactId>maven-assembly-plugin</artifactId>
        <version>3.3.0</version>
        <configuration>
          <descriptorRefs>
            <descriptorRef>jar-with-dependencies</descriptorRef>
          </descriptorRefs>
        </configuration>
        <executions>
          <execution>
            <id>make-assembly</id> <!-- this is used for inheritance merges -->
            <phase>package</phase> <!-- bind to the packaging phase -->
            <goals>
              <goal>single</goal>
            </goals>
          </execution>
        </executions>
      </plugin>
      [...]
</project>
```

### Advanced Configuration

> 仅jar和war汇编格式支持配置 archiver 元素。

**创建可执行jar**

Assembly插件支持配置 *archiver* 元素，该元素由maven-archiver处理

```xml
<project>
  [...]
  <build>
    [...]
    <plugins>
      <plugin>
        <artifactId>maven-assembly-plugin</artifactId>
        <version>3.3.0</version>
        <configuration>
          [...]
          <archive>
            <manifest>
              <mainClass>org.sample.App</mainClass>
            </manifest>
          </archive>
        </configuration>
        [...]
      </plugin>
      [...]
</project>
```

### 示例配置

```xml
            <plugin>
                //配置插件 artifactId
                <artifactId>maven-assembly-plugin</artifactId>
                //配置执行阶段
                <executions>
                    <execution>
                        <id>makeAssembly</id>
                        <phase>package</phase>
                        <goals>
                            <goal>assembly</goal>
                        </goals>
                    </execution>
                </executions>
                //插件配置
                <configuration>
                    <archive>
                        <manifest>
                            <mainClass>MainDemo01</mainClass>
                        </manifest>
                    </archive>
                    //配置描述符文件
                    <descriptors>
                        <descriptor>src/main/resources/assembly.xml</descriptor>
                        <descriptor>src/main/resources/assembly-copy.xml</descriptor>
                    </descriptors>
                    //配置变量替换源文件
                    <filters>
                        <filter>src/main/resources/subsititute.properties</filter>
                    </filters>
                </configuration>
            </plugin>
```

*archive* 标签 详见 



## asembly xml文件

```xml
<assembly xmlns="http://maven.apache.org/ASSEMBLY/2.1.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/ASSEMBLY/2.1.0 http://maven.apache.org/xsd/assembly-2.1.0.xsd">
  <id/>
  <formats/>
  <includeBaseDirectory/>
  <baseDirectory/>
  <includeSiteDirectory/>
  <containerDescriptorHandlers>
    <containerDescriptorHandler>
      <handlerName/>
      <configuration/>
    </containerDescriptorHandler>
  </containerDescriptorHandlers>
  <moduleSets>
    <moduleSet>
      <useAllReactorProjects/>
      <includeSubModules/>
      <includes/>
      <excludes/>
      <sources>
        <useDefaultExcludes/>
        <outputDirectory/>
        <includes/>
        <excludes/>
        <fileMode/>
        <directoryMode/>
        <fileSets>
          <fileSet>
            <useDefaultExcludes/>
            <outputDirectory/>
            <includes/>
            <excludes/>
            <fileMode/>
            <directoryMode/>
            <directory/>
            <lineEnding/>
            <filtered/>
            <nonFilteredFileExtensions/>
          </fileSet>
        </fileSets>
        <includeModuleDirectory/>
        <excludeSubModuleDirectories/>
        <outputDirectoryMapping/>
      </sources>
      <binaries>
        <outputDirectory/>
        <includes/>
        <excludes/>
        <fileMode/>
        <directoryMode/>
        <attachmentClassifier/>
        <includeDependencies/>
        <dependencySets>
          <dependencySet>
            <outputDirectory/>
            <includes/>
            <excludes/>
            <fileMode/>
            <directoryMode/>
            <useStrictFiltering/>
            <outputFileNameMapping/>
            <unpack/>
            <unpackOptions>
              <includes/>
              <excludes/>
              <filtered/>
              <nonFilteredFileExtensions/>
              <lineEnding/>
              <useDefaultExcludes/>
              <encoding/>
            </unpackOptions>
            <scope/>
            <useProjectArtifact/>
            <useProjectAttachments/>
            <useTransitiveDependencies/>
            <useTransitiveFiltering/>
          </dependencySet>
        </dependencySets>
        <unpack/>
        <unpackOptions>
          <includes/>
          <excludes/>
          <filtered/>
          <nonFilteredFileExtensions/>
          <lineEnding/>
          <useDefaultExcludes/>
          <encoding/>
        </unpackOptions>
        <outputFileNameMapping/>
      </binaries>
    </moduleSet>
  </moduleSets>
  <fileSets>
    <fileSet>
      <useDefaultExcludes/>
      <outputDirectory/>
      <includes/>
      <excludes/>
      <fileMode/>
      <directoryMode/>
      <directory/>
      <lineEnding/>
      <filtered/>
      <nonFilteredFileExtensions/>
    </fileSet>
  </fileSets>
  <files>
    <file>
      <source/>
      <sources/>
      <outputDirectory/>
      <destName/>
      <fileMode/>
      <lineEnding/>
      <filtered/>
    </file>
  </files>
  <dependencySets>
    <dependencySet>
      <outputDirectory/>
      <includes/>
      <excludes/>
      <fileMode/>
      <directoryMode/>
      <useStrictFiltering/>
      <outputFileNameMapping/>
      <unpack/>
      <unpackOptions>
        <includes/>
        <excludes/>
        <filtered/>
        <nonFilteredFileExtensions/>
        <lineEnding/>
        <useDefaultExcludes/>
        <encoding/>
      </unpackOptions>
      <scope/>
      <useProjectArtifact/>
      <useProjectAttachments/>
      <useTransitiveDependencies/>
      <useTransitiveFiltering/>
    </dependencySet>
  </dependencySets>
  <repositories>
    <repository>
      <outputDirectory/>
      <includes/>
      <excludes/>
      <fileMode/>
      <directoryMode/>
      <includeMetadata/>
      <groupVersionAlignments>
        <groupVersionAlignment>
          <id/>
          <version/>
          <excludes/>
        </groupVersionAlignment>
      </groupVersionAlignments>
      <scope/>
    </repository>
  </repositories>
  <componentDescriptors/>
</assembly>
```

### 通用元素

| Element                                                   | Type                                    | Description                                                  |
| :-------------------------------------------------------- | :-------------------------------------- | :----------------------------------------------------------- |
| `id`                                                      | `String`                                | 用作 产生工件时的文件名后缀                                  |
| `formats/format*`                                         | `List<String>`                          | 支持输出的格式                                               |
| `includeBaseDirectory`                                    | `boolean`                               | 是否包含基目录，true的话默认为  构件的名称作为目录 即 ${project.build.finalName} |
| `baseDirectory`                                           | `String`                                | true的话默认为${project.build.finalName}                     |
| `includeSiteDirectory`                                    | `boolean`                               | 是否构建 siteDirectory <br/>**Default**: `false`             |
| `containerDescriptorHandlers/containerDescriptorHandler*` | `List<ContainerDescriptorHandlerConfig> | 一组组件，可从常规存档流中过滤掉各种容器描述符，以便可以对其进行汇总然后添加。 |
| `moduleSets/moduleSet*`                                   | `List<ModuleSet>`                       | 模块                                                         |
| `fileSets/fileSet*`                                       | `List<FileSet>`                         | 文件集                                                       |
| `files/file*`                                             | `List<FileItem>`                        | 包含文件                                                     |
| `dependencySets/dependencySet*`                           | `List<DependencySet>`                   | 依赖                                                         |
| `repositories/repository*`                                | `List<Repository>`                      | 装载依赖使用的仓库                                           |
| `componentDescriptors/componentDescriptor*`               | `List<String>`                          | descriptor xml 可以基于 descriptorRef,也可以指定相对路径     |

[详见](http://maven.apache.org/plugins/maven-assembly-plugin/assembly.html)

## Example

### 添加编译后的输出文件

```xml
    <fileSets>
        <fileSet>
            <directory>src/main/resources</directory>
            <includes>
                <include>**/*.properties</include>
            </includes>
            <outputDirectory>/</outputDirectory>
            <filtered>true</filtered>
        </fileSet>
        <fileSet>
            <directory>${project.build.outputDirectory}</directory>
            <includes>
                <include>**/*.class</include>
            </includes>
            <outputDirectory>/</outputDirectory>
        </fileSet>
    </fileSets>
```



### 添加资源文件

```xml
<assembly>

    <id>file-copy</id><!-- 配置文件的标识，同时生成的jar包名字会把这个文本包含进去 -->
    <formats>
        <format>jar</format><!-- 打包类型，此处为jar -->
    </formats>


    <includeBaseDirectory>false</includeBaseDirectory>
    <files>
        <file>
            <source>src/main/resources/README.txt</source>
            <outputDirectory></outputDirectory>
            <filtered>true</filtered>
        </file>
    </files>
</assembly>
```

### 添加项目依赖

```xml
<assembly>

    <id>with-dependence-unpack</id><!-- 配置文件的标识，同时生成的jar包名字会把这个文本包含进去 -->
    <formats>
        <format>jar</format><!-- 打包类型，此处为jar -->
    </formats>
    <includeBaseDirectory>false</includeBaseDirectory>
    <dependencySets>
        <dependencySet>
            <unpack>false</unpack><!-- 是否解压 -->
            <scope>runtime</scope>
            <outputDirectory>lib/</outputDirectory>
        </dependencySet>
    </dependencySets>
    <fileSets>
        <fileSet>
            <directory>${project.build.outputDirectory}</directory>
            <outputDirectory>/</outputDirectory>
        </fileSet>
    </fileSets>
</assembly>
```

### 添加ManiFest

```xml
<plugin>
    <groupId>org.apache.maven.plugins</groupId>
    <artifactId>maven-assembly-plugin</artifactId>
    <version>3.3.0</version>
    <executions>
        <execution>
            <id>makeAssembly</id>
            <phase>package</phase>
            <goals>
                <goal>single</goal>
            </goals>
        </execution>
    </executions>
    <configuration>
        <archive>
            <manifest>
                <addClasspath>true</addClasspath>
                <classpathPrefix>lib</classpathPrefix>
                <mainClass>MainDemo01</mainClass>
                <addDefaultImplementationEntries>true</addDefaultImplementationEntries>
            </manifest>
        </archive>
        <descriptors>
            <descriptor>src/main/resources/assembly-copy.xml</descriptor>
        </descriptors>
        <filters>
            <filter>src/main/resources/subsititute.properties</filter>
        </filters>
    </configuration>
</plugin>
```

