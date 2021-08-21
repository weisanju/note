# Maven 依赖管理

## 可传递性依赖发现

| 功能     | 功能描述                                                     |
| :------- | :----------------------------------------------------------- |
| 依赖调节 | 决定当多个手动创建的版本同时出现时，哪个依赖版本将会被使用。 如果两个依赖版本在依赖树里的深度是一样的时候，第一个被声明的依赖将会被使用。 |
| 依赖管理 | 直接的指定手动创建的某个版本被使用。例如当一个工程 C 在自己的依赖管理模块包含工程 B，即 B 依赖于 A， 那么 A 即可指定在 B 被引用时所使用的版本。 |
| 依赖范围 | 包含在构建过程每个阶段的依赖。scope                          |
| 依赖排除 | 任何可传递的依赖都可以通过 "exclusion" 元素被排除在外。举例说明，A 依赖 B， B 依赖 C，因此 A 可以标记 C 为 "被排除的"。 |
| 依赖可选 | 任何可传递的依赖可以被标记为可选的，通过使用 "optional" 元素。例如：A 依赖 B， B 依赖 C。因此，B 可以标记 C 为可选的， 这样 A 就可以不再使用 C。 |

## 依赖范围

传递依赖发现可以通过使用如下的依赖范围来得到限制：

| 范围     | 标识       | 描述                                                         |
| :------- | ---------- | :----------------------------------------------------------- |
| 编译阶段 | *compiler* | 该范围表明相关依赖是只在项目的类路径下有效。默认取值。       |
| 供应阶段 | *provided* | 该范围表明相关依赖是由运行时的 JDK 或者 网络服务器提供的。(运行时由其他提供，例如从网络加载或者被别人依赖时由别人提供<br />在编译测试阶段由自己提供) |
| 运行阶段 | *runtime*  | 该范围表明相关依赖在编译阶段不是必须的，但是在执行阶段是必须的。 |
| 测试阶段 | *test*     | 该范围表明相关依赖只在测试编译阶段和执行阶段。               |
| 系统阶段 | *system*   | 该范围表明你需要提供一个系统路径的jar包                      |
| 导入阶段 | *import*   | 该范围只在依赖是一个 pom 里定义的依赖时使用。同时，当前项目的POM 文件的 部分定义的依赖关系可以取代某特定的 POM。 |

## 依赖范围对于 classpath的影响

| 依赖范围（scope） | 对于编译classpath有效 | 对于测试classpath有效 | 对于运行时classpath有效 | 例子                            |
| ----------------- | --------------------- | --------------------- | ----------------------- | ------------------------------- |
| compile           | Y                     | Y                     | Y                       | spring-core                     |
| test              | -                     | Y                     | -                       | JUnit                           |
| provided          | Y                     | Y                     | -                       | servlet-api                     |
| runtime           | -                     | Y                     | Y                       | JDBC驱动实现                    |
| system            | Y                     | Y                     | -                       | 本地的，Maven仓库之外的类库文件 |

## 依赖传递

| 第一依赖范围\第二依赖范围 | compile | test | provided | runtime |
| ------------------------- | ------- | ---- | -------- | ------- |
| compile                   | compile | N    |          |         |
| test                      |         |      |          |         |
| provider                  |         |      |          |         |
| runtime                   |         |      |          |         |





## 使用父依赖

App-UI-WAR依赖App-core-lib,App-data-lib

**app-ui-war的pom文件**

```
<project xmlns="http://maven.apache.org/POM/4.0.0"
   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
   xsi:schemaLocation="http://maven.apache.org/POM/4.0.0
   http://maven.apache.org/xsd/maven-4.0.0.xsd">
      <modelVersion>4.0.0</modelVersion>
      <groupId>com.companyname.groupname</groupId>
      <artifactId>App-UI-WAR</artifactId>
      <version>1.0</version>
      <packaging>war</packaging>
      <dependencies>
         <dependency>
            <groupId>com.companyname.groupname</groupId>
            <artifactId>App-Core-lib</artifactId>
            <version>1.0</version>
         </dependency>
      </dependencies>  
      <dependencies>
         <dependency>
            <groupId>com.companyname.groupname</groupId>
            <artifactId>App-Data-lib</artifactId>
            <version>1.0</version>
         </dependency>
      </dependencies>  
</project>
```

**App-Core-lib 的 pom.xml** 

```
<project xmlns="http://maven.apache.org/POM/4.0.0"
   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
   xsi:schemaLocation="http://maven.apache.org/POM/4.0.0
   http://maven.apache.org/xsd/maven-4.0.0.xsd">
      <parent>
         <artifactId>Root</artifactId>
         <groupId>com.companyname.groupname</groupId>
         <version>1.0</version>
      </parent>
      <modelVersion>4.0.0</modelVersion>
      <groupId>com.companyname.groupname</groupId>
      <artifactId>App-Core-lib</artifactId>
      <version>1.0</version> 
      <packaging>jar</packaging>
</project>
```

**App-Data-lib 的 pom.xml** 

```xml
<project xmlns="http://maven.apache.org/POM/4.0.0"
   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
   xsi:schemaLocation="http://maven.apache.org/POM/4.0.0
   http://maven.apache.org/xsd/maven-4.0.0.xsd">
      <parent>
         <artifactId>Root</artifactId>
         <groupId>com.companyname.groupname</groupId>
         <version>1.0</version>
      </parent>
      <modelVersion>4.0.0</modelVersion>
      <groupId>com.companyname.groupname</groupId>
      <artifactId>App-Data-lib</artifactId>
      <version>1.0</version>   
      <packaging>jar</packaging>
</project>
```

**Root 的 pom.xml 文件代码**

```
<project xmlns="http://maven.apache.org/POM/4.0.0"
   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
   xsi:schemaLocation="http://maven.apache.org/POM/4.0.0
   http://maven.apache.org/xsd/maven-4.0.0.xsd">
      <modelVersion>4.0.0</modelVersion>
      <groupId>com.companyname.groupname</groupId>
      <artifactId>Root</artifactId>
      <version>1.0</version>
      <packaging>pom</packaging>
      <dependencies>
         <dependency>
            <groupId>com.companyname.groupname1</groupId>
            <artifactId>Lib1</artifactId>
            <version>1.0</version>
         </dependency>
      </dependencies>  
      <dependencies>
         <dependency>
            <groupId>com.companyname.groupname2</groupId>
            <artifactId>Lib2</artifactId>
            <version>2.1</version>
         </dependency>
      </dependencies>  
      <dependencies>
         <dependency>
            <groupId>com.companyname.groupname3</groupId>
            <artifactId>Lib3</artifactId>
            <version>1.1</version>
         </dependency>
      </dependencies>  
</project>
```

