# springboot devtools是做什么的?

* 使Spring Boot应用支持热部署，提高开发者的开发效率，无需手动重启Spring Boot应用
* 深层原理是使用了两个ClassLoader，一个Classloader加载那些不会改变的类（第三方Jar包），另一个ClassLoader加载会更改的类,称为restart ClassLoader
* 但如果你期望第三方jar文件的变动也会触发devtools进行自动重启，则可以创建`META-INF/spring-devtools.properties`,在文件中通过`restart.include.xxx`指定需要自动重启的jar，通过`restart.exclude.xxx`指定不需要自动重启的jar，属性值采用正则表达式匹配
* 有代码更改的时候，原来的restart ClassLoader 被丢弃，重新创建一个restart ClassLoader，由于需要加载的类相比较少，所以实现了较快的重启时间。
* 默认情况下，/META-INF/maven, /META-INF/resources, /resources, /static, /public, 和 /templates下面的资源的变化不会触发自动重启
* 如果想监听非Classpath路径下的文件的变更进行自动重启，则可以通过`spring.devtools.restart.additional-paths`属性进行指定。
* 如果你想禁用devtools的自动重启功能，则可以指定spring.devtools.restart.enabled=false，也可以在Spring Boot启动的main程序中，在调用SpringApplication.run(..)之前指定系统属性spring.devtools.restart.enabled为false
* spring.devtools.restart.trigger-file,指定触发自动重启的文件可以实现一次重启
  * spring.devtools.restart.trigger-file=application.properties
* 上面的针对devtools的配置都是基于单个项目的配置，如果希望上述配置能够针对所有的项目都生效，则可以在HOME目录下新建一个`.spring-boot-devtools.properties`文件（注意文件名是以点开头的），在其中定义那些配置属性。

# maven配置

```
<dependencies>
	<dependency>
		<groupId>org.springframework.boot</groupId>
		<artifactId>spring-boot-devtools</artifactId>
		<optional>true</optional>
	</dependency>
	<dependency>
		<groupId>org.springframework.boot</groupId>
		<artifactId>spring-boot-starter-test</artifactId>
		<scope>test</scope>
	</dependency>
</dependencies>
<build>
	<plugins>
		<plugin>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-maven-plugin</artifactId>
			<configuration>
				<fork>true</fork>
			</configuration>
		</plugin>
	</plugins>
</build>
```

# devtools的配置

```
#热部署生效
spring.devtools.restart.enabled: true
#设置重启的目录
#spring.devtools.restart.additional-paths: src/main/java
#classpath目录下的WEB-INF文件夹内容修改不重启
spring.devtools.restart.exclude: WEB-INF/**
```

# IDEA配置

* 当我们修改了Java类后，IDEA默认是不自动编译的
* 而spring-boot-devtools又是监测classpath下的文件发生变化才会重启应用
* 所以需要设置IDEA的自动编译：
  * File-Settings-Compiler-Build Project automatically
  * ctrl + shift + alt + /,选择Registry,勾上 Compiler autoMake allow when app running



# 热部署的文件类型

* devtools可以实现页面热部署
  * 这个可以直接在application.properties文件中配置spring.thymeleaf.cache=false来实现
* 实现类文件热部署（类文件修改后不会立即生效）
* 实现对属性文件的热部署。



# 配置项

前缀spring.devtools.restart

| 属性名                         | 默认值 | 说明                                         |
| ------------------------------ | ------ | -------------------------------------------- |
| log-condition-evaluation-delta | false  | 是否报告                                     |
| exclude                        |        | exclude=static/**,public/**,指定要排除的文件 |
| additional-exclude             |        | 额外排除的文件                               |
| enabled                        | false  | 是否启用                                     |
| additional-paths               |        | 类路径之外的路径                             |
| trigger-file                   |        | 使用触发文件                                 |

[官网说明](https://docs.spring.io/spring-boot/docs/2.0.3.RELEASE/reference/html/using-boot-devtools.html)

