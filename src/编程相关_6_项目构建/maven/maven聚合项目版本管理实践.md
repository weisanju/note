# Maven CI Friendly Versions

## 介绍

从Maven 3.5.0-beta-1版本开始，可以使用`${revision}`, `${sha1}` 和 `${changelist}`作为占位符来替换pom文件了。

## 单模块项目配置

```xml
<project>
  <modelVersion>4.0.0</modelVersion>
  <parent>
    <groupId>org.apache</groupId>
    <artifactId>apache</artifactId>
    <version>18</version>
  </parent>
  <groupId>org.apache.maven.ci</groupId>
  <artifactId>ci-parent</artifactId>
  <name>First CI Friendly</name>
  <version>${revision}</version>
  ...
</project>
```

**使用命令指定版本号**

```shell
mvn -Drevision=1.0.0-SNAPSHOT clean package
```

**使用properties**

```xml
<project>
  <modelVersion>4.0.0</modelVersion>
  <parent>
    <groupId>org.apache</groupId>
    <artifactId>apache</artifactId>
    <version>18</version>
  </parent>
  <groupId>org.apache.maven.ci</groupId>
  <artifactId>ci-parent</artifactId>
  <name>First CI Friendly</name>
  <version>${revision}</version>
  ...
  <properties>
    <revision>1.0.0-SNAPSHOT</revision>
  </properties>
</project>
```

**在配置文件中指定**

```
.mvn/maven.config
```

**可以组合properties**

```xml
<project>
  <modelVersion>4.0.0</modelVersion>
  <parent>
    <groupId>org.apache</groupId>
    <artifactId>apache</artifactId>
    <version>18</version>
  </parent>
  <groupId>org.apache.maven.ci</groupId>
  <artifactId>ci-parent</artifactId>
  <name>First CI Friendly</name>
  <version>${revision}${sha1}${changelist}</version>
  ...
  <properties>
    <revision>1.3.1</revision>
    <changelist>-SNAPSHOT</changelist>
    <sha1/>
  </properties>
</project>
```





## 多模块

**父项目配置**

```xml
<project>
  <modelVersion>4.0.0</modelVersion>
  <parent>
    <groupId>org.apache</groupId>
    <artifactId>apache</artifactId>
    <version>18</version>
  </parent>
  <groupId>org.apache.maven.ci</groupId>
  <artifactId>ci-parent</artifactId>
  <name>First CI Friendly</name>
  <version>${revision}</version>
  ...
  <properties>
    <revision>1.0.0-SNAPSHOT</revision>
  </properties>
  <modules>
    <module>child1</module>
    ..
  </modules>
</project>
```

**子项目配置**

```xml
<project>
  <modelVersion>4.0.0</modelVersion>
  <parent>
    <groupId>org.apache.maven.ci</groupId>
    <artifactId>ci-parent</artifactId>
    <version>${revision}</version>
  </parent>
  <groupId>org.apache.maven.ci</groupId>
  <artifactId>ci-child</artifactId>
   ...
</project>
```

**多项目中的依赖管理**

多模块工程结构下，会有很多模块依赖的情况，应该使用${project.version}来定义依赖（同父工程下的依赖）的版本或者使用 *dependencyManagement* 管理子模块依赖

**父项目**

```java
<project>
  <modelVersion>4.0.0</modelVersion>
  <parent>
    <groupId>org.apache</groupId>
    <artifactId>apache</artifactId>
    <version>18</version>
  </parent>
  <groupId>org.apache.maven.ci</groupId>
  <artifactId>ci-parent</artifactId>
  <name>First CI Friendly</name>
  <version>${revision}</version>
  ...
  <properties>
    <revision>1.0.0-SNAPSHOT</revision>
  </properties>
  <modules>
    <module>child1</module>
    ..
  </modules>
</project>
```

**子项目依赖另一个子项目**

```java
<project>
  <modelVersion>4.0.0</modelVersion>
  <parent>
    <groupId>org.apache.maven.ci</groupId>
    <artifactId>ci-parent</artifactId>
    <version>${revision}</version>
  </parent>
  <groupId>org.apache.maven.ci</groupId>
  <artifactId>ci-child</artifactId>
   ...
  <dependencies>
		<dependency>
      <groupId>org.apache.maven.ci</groupId>
      <artifactId>child2</artifactId>
      <version>${project.version}</version>
    </dependency>
  </dependencies>
</project>
```



**部署到本地仓库、远程仓库**



```xml
<project>
  <modelVersion>4.0.0</modelVersion>
  <parent>
    <groupId>org.apache</groupId>
    <artifactId>apache</artifactId>
    <version>18</version>
  </parent>
  <groupId>org.apache.maven.ci</groupId>
  <artifactId>ci-parent</artifactId>
  <name>First CI Friendly</name>
  <version>${revision}</version>
  ...
  <properties>
    <revision>1.0.0-SNAPSHOT</revision>
  </properties>
 
 <build>
  <plugins>
    <plugin>
      <groupId>org.codehaus.mojo</groupId>
      <artifactId>flatten-maven-plugin</artifactId>
      <version>1.2.5</version>
      <configuration>
        <updatePomFile>true</updatePomFile>
        <flattenMode>resolveCiFriendliesOnly</flattenMode>
      </configuration>
      <executions>
        <execution>
          <id>flatten</id>
          <phase>process-resources</phase>
          <goals>
            <goal>flatten</goal>
          </goals>
        </execution>
        <execution>
          <id>flatten.clean</id>
          <phase>clean</phase>
          <goals>
            <goal>clean</goal>
          </goals>
        </execution>
      </executions>
    </plugin>
  </plugins>
  </build>
  <modules>
    <module>child1</module>
    ..
  </modules>
</project>
```





# deploy 发布到私服

## 配置发布仓库

```xml
    <!-- 部署module的jar到私有仓库 -->
    <distributionManagement>
        <repository>
            <id>public</id>
            <name>releases</name>
            <url>http://ip:port/repository/maven-releases/</url>
        </repository>
        <snapshotRepository>
            <id>snapshot</id>
            <name>snapshot</name>
            <url>http://ip:port/repository/maven-snapshots/</url>
        </snapshotRepository>
    </distributionManagement>
```

## **配置仓库认证信息**

**注意：**次id必须与上述配置的仓库id保持 一致

```xml
<server>
    <id>public</id>
    <username>111</username>
    <password>111</password>
</server>

<server>
    <id>snapshot</id>
    <username>111</username>
    <password>111</password>
</server>
```



## **统一版本号**

* 使用 *revision* 变量统一 各个子模块的版本

* 使用 dependence Manager 在父工程统一管理依赖

**定义版本号**

```xml
<properties>
        <revision>1.0.1-SNAPSHOT</revision>
<properties>
```

**定义父POM的版本号**

```xml
<groupId>com.aiseeding.ase</groupId>
<artifactId>ase-parent</artifactId>
<packaging>pom</packaging>
<version>${revision}</version>
```

**定义子POM的版本**

```xml
 <dependencyManagement>
        <dependencies>
            <dependency>
                <groupId>com.aiseeding.ase</groupId>
                <artifactId>ase-workflow</artifactId>
                <version>${revision}</version>
            </dependency>

            <dependency>
                <groupId>com.aiseeding.ase</groupId>
                <artifactId>ase-message</artifactId>
                <version>${revision}</version>
            </dependency>

            <dependency>
                <groupId>com.aiseeding.ase</groupId>
                <artifactId>ase-user-docking</artifactId>
                <version>${revision}</version>
            </dependency>
        </dependencies>
    </dependencyManagement>
```



## 发布到本地仓库与私服

配置*flatMap*更新打包方式

```java
            <plugin>
                <!-- https://mvnrepository.com/artifact/org.codehaus.mojo/flatten-maven-plugin -->
                <groupId>org.codehaus.mojo</groupId>
                <artifactId>flatten-maven-plugin</artifactId>
                <version>1.2.5</version>

                <configuration>
                    <!--是否更新pom文件，此处还有更高级的用法-->
                    <updatePomFile>true</updatePomFile>
                    <flattenMode>resolveCiFriendliesOnly</flattenMode>
                </configuration>
                <executions>
                    <execution>
                        <id>flatten</id>
                        <phase>process-resources</phase>
                        <goals>
                            <goal>flatten</goal>
                        </goals>
                    </execution>
                    <execution>
                        <id>flatten.clean</id>
                        <phase>clean</phase>
                        <goals>
                            <goal>clean</goal>
                        </goals>
                    </execution>
                </executions>
            </plugin>
```

**跳过不需要发布的包**

```java
    <properties>
        <maven.deploy.skip>true</maven.deploy.skip>
    </properties>

```

**指定发布的包**

*deploy* 右键 -> *modify run configuration* -> 修改命令

```
deploy -pl ase-message -am -f pom.xml
```





# *SNAPSHOT* 与 RELEASE

* 在Nexus仓库中，一个仓库一般分为public(Release)仓和SNAPSHOT仓，前者存放正式版本，后者存放快照版本。

* 如果在项目配置文件中（无论是build.gradle还是pom.xml）指定的版本号带有’-SNAPSHOT’后缀，比如版本号为’Junit-4.10-SNAPSHOT’，那么打出的包就是一个快照版本。

* 快照版本和正式版本的主要区别在于，**本地获取这些依赖的机制有所不同**。

    **example**

```
假设你依赖一个库的正式版本，构建的时候构建工具会先在本次仓库中查找是否已经有了这个依赖库，如果没有的话才会去远程仓库中去拉取。

所以假设你发布了Junit-4.10.jar到了远程仓库，有一个项目依赖了这个库，它第一次构建的时候会把该库从远程仓库中下载到本地仓库缓存，以后再次构建都不会去访问远程仓库了。

所以如果你修改了代码，向远程仓库中发布了新的软件包，但仍然叫Junit-4.10.jar，那么依赖这个库的项目就无法得到最新更新。

你只有在重新发布的时候升级版本，比如叫做Junit-4.11.jar，然后通知依赖该库的项目组也修改依赖版本为Junit-4.11,这样才能使用到你最新添加的功能。
```

* 弊处

这种方式在团队内部开发的时候会变的特别蛋痛。假设有两个小组负责维护两个组件，example-service和example-ui,其中example-ui项目依赖于example-service。而这两个项目每天都会构建多次，如果每次构建你都要升级example-service的版本，那么你会疯掉。这个时候SNAPSHOT版本就派上用场了。每天日常构建时你可以构建example-service的快照版本，比如example-service-1.0-SNAPSHOT.jar，而example-ui依赖该快照版本。每次example-ui构建时，会优先去远程仓库中查看是否有最新的example-service-1.0-SNAPSHOT.jar，如果有则下载下来使用。即使本地仓库中已经有了example-service-1.0-SNAPSHOT.jar，它也会尝试去远程仓库中查看同名的jar是否是最新的。有的人可能会问，这样不就不能充分利用本地仓库的缓存机制了吗？别着急，Maven比我们想象中的要聪明。在配置Maven的Repository的时候中有个配置项，可以配置对于SNAPSHOT版本**向远程仓库中查找的频率**。频率共有四种，分别是**always、daily、interval、never**。当本地仓库中存在需要的依赖项目时，always是每次都去远程仓库查看是否有更新，daily是只在第一次的时候查看是否有更新，当天的其它时候则不会查看；interval允许设置一个分钟为单位的间隔时间，在这个间隔时间内只会去远程仓库中查找一次，never是不会去远程仓库中查找（这种就和正式版本的行为一样了）。

```xml
<repository>
    <id>myRepository</id>
    <url>...</url>
    <snapshots>
        <enabled>true</enabled>
        <updatePolicy>XXX</updatePolicy>
    </snapshots>
</repository>
```





# 参考链接

[maven-ci-friendly](https://maven.apache.org/maven-ci-friendly.html)

https://www.mojohaus.org/flatten-maven-plugin/