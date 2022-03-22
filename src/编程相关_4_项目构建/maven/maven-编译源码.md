# Apache Maven Source Plugin

source plugin 创建当前项目源文件的 jar 存档



# Goals Overview

The Source Plugin has five goals:

**source:aggregate**: 在聚合项目的所有模块 中 聚合所有源代码
**source:jar**: 用于将项目的主要来源捆绑到一个jar中
**source:test-jar** :将项目的测试源捆绑到 jar 存档中
**source:jar-no-fork** ：类似于 jar 但不分叉构建生命周期.
**source:test-jar-no-fork** ：类似于 test-jar，但不分叉构建生命周期。.



# 模板配置

```xml
<project>
  ...
  <build>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-source-plugin</artifactId>
        <version>3.2.0</version>
        <configuration>
          <outputDirectory>/absolute/path/to/the/output/directory</outputDirectory>
          <finalName>filename-of-generated-jar-file</finalName>
          <attach>false</attach>
        </configuration>
      </plugin>
    </plugins>
  </build>
  ...
</project>
```



```xml
<plugin>
    <groupId>org.apache.maven.plugins</groupId>
    <artifactId>maven-source-plugin</artifactId>
    <version>3.2.0</version>
    <executions>
        <execution>
            <id>attach-sources</id>
            <phase>package</phase>
            <goals>
                <goal>jar-no-fork</goal>
            </goals>
        </execution>
    </executions>
</plugin>
```





生成的 jar 文件，如果是主源，会以 finalName 的值加上“-sources”来命名。否则，如果它是测试源，它将是 finalName 加上“-test-sources”。它将在指定的 outputDirectory 中生成。 attach 参数指定 java 源是否将附加到项目的工件列表中。

