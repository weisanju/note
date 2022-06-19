# 步骤

## 生成项目

建立父POM.不存放代码,只存放子模块

1. 官网生成,

   https://start.spring.io/

2. 在 idea可以选择POM方式打包



## 修改pom文件

1.  源代码目录删除

2. ```
   <packaging>pom</packaging>
   ```

3. 注释掉springboot的打包方式(只有springboot 主项目才需要springboot打包成可执行文件,依赖库不需要)

4. 添加子模块 

   ```xml
   <modules>
           <module>unifiedoutput</module>
           <module>common</module>
   </modules>
   ```

5. 子模块的POM文件

   1. 添加父模块依赖

      ```xml
      <parent>
          <groupId>com.weisanju</groupId>
          <artifactId>springboot-study</artifactId>
          <version>0.0.1-SNAPSHOT</version>
      <!--        <relativePath>../pom.xml</relativePath>-->
      </parent>
      ```

   2. 添加其他子模块依赖

      ```xml
              <dependency>
                  <groupId>com.weisanju</groupId>
                  <artifactId>common</artifactId>
                  <version>0.0.1-SNAPSHOT</version>
              </dependency>
      ```

      

   

## 完整的配置文件

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.3.1.RELEASE</version>
        <relativePath/> <!-- lookup parent from repository -->
    </parent>
    <groupId>com.weisanju</groupId>
    <artifactId>springboot-study</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <name>springboot-study</name>
    <description>Demo project for Spring Boot</description>
    <packaging>pom</packaging>
    <modules>
        <module>unifiedoutput</module>
        <module>common</module>
    </modules>
    <properties>
        <java.version>1.8</java.version>
    </properties>

    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>

        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-devtools</artifactId>
            <scope>runtime</scope>
            <optional>true</optional>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-configuration-processor</artifactId>
            <optional>true</optional>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
            <exclusions>
                <exclusion>
                    <groupId>org.junit.vintage</groupId>
                    <artifactId>junit-vintage-engine</artifactId>
                </exclusion>
            </exclusions>
        </dependency>
    </dependencies>
<!--
    <build>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
            </plugin>
        </plugins>
    </build>-->

</project>

```

4. 父项目的公共依赖,请确保加上*optional*,避免依赖重复
