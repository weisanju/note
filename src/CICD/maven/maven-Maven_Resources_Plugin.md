# 简介

* Maven Resources Plugin是帮助处理项目资源文件 copy到 输出目录的插件

* 有两种不同的资源: JavaCode相关联的资源,TestCode相关联的资源 
* 从 2.3开始,使用 the [Maven Filtering](http://maven.apache.org/shared/maven-filtering/) shared component for filtering resources.

# 总览

有三种处理资源的方式

* [resources:resources](http://maven.apache.org/plugins/maven-resources-plugin/resources-mojo.html)
  *  JavaMainCode资源 copy到主输出路径
  * 会自动执行,因为绑定到了默认 process-resources (资源处理)生命周期阶段
  * 通常使用 project.build.resources 指定资源
  * project.build.outputDirectory 作为输出路径
* [resources:testResources](http://maven.apache.org/plugins/maven-resources-plugin/testResources-mojo.html) 
  * 拷贝test资源 到test输出路径
  * 默认会加载因为 绑定到了 process-test-resources 生命周期处理 阶段
  * 使用 project.build.testResources  指定资源
  * project.build.testOutputDirectory 指定输出目录
* [resources:copy-resources](http://maven.apache.org/plugins/maven-resources-plugin/copy-resources-mojo.html) 
  *  copy任意目标 到任意目录
  * 需要指定资源 和目标目录

# 使用实例

## Specifying a character encoding scheme

统一指定为读写资源文件 指定字符编码格式

```xml
<properties>
   <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
   ...
 </properties>
```

特定任务指定

```xml
<project>
  ...
  <build>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-resources-plugin</artifactId>
        <version>3.1.0</version>
        <configuration>
          ...
          <encoding>UTF-8</encoding>
          ...
        </configuration>
      </plugin>
    </plugins>
    ...
  </build>
  ...
</project>
```

## Specifying resource directories

指定资源路径

```xml
<project>
 ...
 <build>
   ...
   <resources>
     <resource>
       <directory>[your folder here]</directory>
     </resource>
   </resources>
   ...
 </build>
 ...
</project>


---

   <resources>
     <resource>
       <directory>src/my-resources</directory>
     </resource>
   </resources>


 ...指定目录
   <resources>
     <resource>
       <directory>resource1</directory>
     </resource>
     <resource>
       <directory>resource2</directory>
     </resource>
     <resource>
       <directory>resource3</directory>
     </resource>
   </resources>
   ...
```



## Filtering

* 针对maven有的变量 对 资源文件的占位符替换

* 或者使用环境变量

```
Hello ${name}
mvn resources:resources
mvn resources:resources -Dname="world"
```

* 指定 可替换的资源文件

  ```xml
  <project>
    ...
    <name>My Resources Plugin Practice Project</name>
    ...
    <build>
      ...
      <filters>
        <filter>[a filter property]</filter>
      </filters>
      ...
    </build>
    ...
  </project>
  
     ...
      <filters>
        <filter>my-filter-values.properties</filter>
      </filters>
      ...
  
   <build>
      ...
      <resources>
        <resource>
          <directory>src/main/resources-filtered</directory>
          <filtering>true</filtering>
        </resource>
        ...
      </resources>
      ...
    </build>
  ```

配置解析转义符

```xml
 <configuration>
          ...
          <escapeString>\</escapeString>
          ...
        </configuration>
```

## 包含和排除

包含

```xml
<build>
    ...
    <resources>
      <resource>
        <directory>[your directory]</directory>
        <includes>
          <include>[resource file #1]</include>
          <include>[resource file #2]</include>
          <include>[resource file #3]</include>
          ...
          <include>[resource file #n]</include>
        </includes>
      </resource>
      ...
    </resources>
    ...
  </build>
```

排除

```xml
<build>
    ...
    <resources>
      <resource>
        <directory>[your directory]</directory>
        <excludes>
          <exclude>[non-resource file #1]</exclude>
          <exclude>[non-resource file #2]</exclude>
          <exclude>[non-resource file #3]</exclude>
          ...
          <exclude>[non-resource file #n]</exclude>
        </excludes>
      </resource>
      ...
    </resources>
    ...
  </build>
```

排除该目录下的所有文件

```xml
<project>
  ...
  <name>My Resources Plugin Practice Project</name>
  ...
  <build>
    ...
    <resources>
      <resource>
        <directory>src/my-resources</directory>
        <excludes>
          <exclude>**/*.bmp</exclude>
          <exclude>**/*.jpg</exclude>
          <exclude>**/*.jpeg</exclude>
          <exclude>**/*.gif</exclude>
        </excludes>
      </resource>
      ...
    </resources>
    ...
  </build>
  ...
</project>
```



## Copy Resources

```xml
<project>
  ...
  <build>
    <plugins>
      <plugin>
        <artifactId>maven-resources-plugin</artifactId>
        <version>3.1.0</version>
        <executions>
          <execution>
            <id>copy-resources</id>
            <!-- here the phase you need -->
            <phase>validate</phase>
            <goals>
              <goal>copy-resources</goal>
            </goals>
            <configuration>
              <outputDirectory>${basedir}/target/extra-resources</outputDirectory>
              <resources>          
                <resource>
                  <directory>src/non-packaged-resources</directory>
                  <filtering>true</filtering>
                </resource>
              </resources>              
            </configuration>            
          </execution>
        </executions>
      </plugin>
    </plugins>
    ...
  </build>
  ...
</project>
```

## Binary filtering

```xml
<project>
  ...
  <build>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-resources-plugin</artifactId>
        <version>3.1.0</version>
        <configuration>
          ...
          <nonFilteredFileExtensions>
            <nonFilteredFileExtension>pdf</nonFilteredFileExtension>
            <nonFilteredFileExtension>swf</nonFilteredFileExtension>
          </nonFilteredFileExtensions>
          ...
        </configuration>
      </plugin>
    </plugins>
    ...
  </build>
  ...
</project>
```

## 自定义解析过滤器

```xml
实现该类
public class ItFilter
    implements MavenResourcesFiltering
```

