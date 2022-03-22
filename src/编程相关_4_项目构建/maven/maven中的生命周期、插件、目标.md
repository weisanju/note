# 生命周期（lifecycle）

Maven的生命周期就是对所有的 **构建过程** 进行抽象和统一。包含了项目的清理、初始化、编译、测试、打包、集成测试、验证、部署和站点生成等几乎所有的构建步骤。

Maven 内置的三套生命周期：

* clean 清理项目
* default 构建、发布项目
* site 生成项目站点





# 阶段（phase）

* 阶段是生命周期的组成部分。

* 特点：各个阶段之间 在生命周期内具有顺序性
* 执行生命周期的某个阶段会将该阶段之前的所有 其他阶段执行

**clean周期共有3个阶段**

1. pre-clean
2. clean
3. post-clean







# 插件:目标（plugin:goal）

* **插件与 目标 构成 阶段**

* **插件与目标可以单独执行**
* 插件可以 绑定到 某个阶段上





## **clean阶段**

| 顺序 | 阶段       | 插件:目标   |
| ---- | ---------- | ----------- |
| 1    | pre-clean  |             |
| 2    | clean      | clean:clean |
| 3    | post-clean |             |

## **default阶段**

| 顺序 | 阶段                    | 插件:目标               |
| ---- | ----------------------- | ----------------------- |
| 1    | validate（校验）        |                         |
| 2    | initialize（初始化）    |                         |
| 3    | generate-sources        |                         |
| 4    | process-sources         |                         |
| 5    | generate-resources      |                         |
| 6    | process-resources       | resources:resources     |
| 7    | compile                 | compiler:compile        |
| 8    | process-classes         |                         |
| 9    | generate-test-sources   |                         |
| 10   | process-test-sources    |                         |
| 11   | generate-test-resources |                         |
| 12   | process-test-resources  | resources:testResources |
| 13   | test-compile            | compiler:testCompile    |
| 14   | process-test-classes    |                         |
| 15   | test surefire:test      |                         |
| 16   | prepare-package         |                         |
| 17   | package                 |                         |
| 18   | pre-integration-test    |                         |
| 19   | integration-test        |                         |
| 20   | post-integration-test   |                         |
| 21   | verify                  |                         |
| 22   | install                 | install:install         |
| 23   | deploy deploy:deploy    | deploy:deploy           |

生命周期site(4个阶段)

| 顺序 | 阶段        | 插件:目标   |
| ---- | ----------- | ----------- |
| 1    | pre-site    |             |
| 2    | site        | site:site   |
| 3    | post-site   |             |
| 4    | site-deploy | site:deploy |





# 示例

* 该插件有两个目标，一个是 flattern,一个是 clean
* 第一个目标 绑定到  *process-resources* 阶段上
* 第二个目标绑定到 *clean* 阶段上

```xml
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

