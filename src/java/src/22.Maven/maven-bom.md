# 什么是BOM

**BOM**（Bill of Materials）是由Maven提供的功能,它通过定义一**整套相互兼容**的jar包版本集合，

使用时只需要依赖该BOM文件，即可放心的使用需要的依赖jar包，且**无需再指定版本号**。





BOM的**维护方负责版本升级**，并保证BOM中定义的jar包版本之间的兼容性。



# BOM作用

使用BOM除了可以方便使用者在声明依赖的客户端时**不需要指定版本号**外

最主要的原因是可以**解决依赖冲突**，如考虑以下的依赖场景：



```
项目A依赖项目B 2.1和  项目C 1.2版本：

项目B 2.1依赖项目 D 1.1版本；

项目C 1.2依赖项目 D 1.3版本；
```

在该例中，项目A对于项目D的依赖就会出现冲突，按照maven dependency mediation的规则，最后生效的可能是:项目A中会依赖到项目D1.1版本（就近原则，取决于路径和依赖的先后,和Maven版本有关系）。



在这种情况下，由于项目C依赖1.3版本的项目D，但是在运行时生效的确是1.1版本，



所以在运行时很容易产生问题，如 **NoSuchMethodError**, **ClassNotFoundException**等，



有些jar包冲突定位还是比较难的，这种方式可以节省很多定位此类问题的时间。



**Spring、SpringBoot、SpringCloud**自身都采用了此机制来解决第三方包的冲突，





# 常见官方提供的BOM：

## **RESTEasy Maven BOM dependency**

```xml
<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>org.jboss.resteasy</groupId>
            <artifactId>resteasy-bom</artifactId>
            <version>3.0.6.Final</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement>
```

## **JBOSS Maven BOM dependency**

```xml
<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>org.jboss.bom</groupId>
            <artifactId>jboss-javaee-6.0-with-tools</artifactId>
            <version>${some.version}</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement> 
```

## **Spring Maven BOM dependency**

```xml
<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-framework-bom</artifactId>
            <version>4.0.1.RELEASE</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement>
```

## **Jersey Maven BOM dependency**

```xml
<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>org.glassfish.jersey</groupId>
            <artifactId>jersey-bom</artifactId>
            <version>${jersey.version}</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement>
```

## **SpringCloud SpringBoot Maven BOM dependency**

```xml
<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-dependencies</artifactId>
            <version>2.4.4</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-dependencies</artifactId>
            <version>2020.0.2</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement>
```



# BOM是如何解决依赖冲突的

## 定义BOM

BOM本质上是一个普通的POM文件，区别是对于**使用方**而言，生效的只有 <dependencyManagement>这一个部分。

只需要在`<dependencyManagement>`定义对外发布的客户端版本即可，

比如需要在项目中**统一**所有SpringBoot和SpringCloud的**版本**



第一步需要在POM文件中增加两个的官方BOM，

以目前最新稳定的SpringBoot版本为例，使用官方推荐的版本组合比较稳定，一般不会有什么大的问题

```xml
<groupId>com.niu.not</groupId>
<artifactId>niu-dependency</artifactId>
<version>1.1.1</version>
<modelVersion>4.0.0</modelVersion>
<packaging>pom</packaging>
<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-dependencies</artifactId>
            <version>2.4.6</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-dependencies</artifactId>
            <version>2020.0.3</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement>
<dependencies>
    <dependency>
        <groupId>com.google.code.gson</groupId>
        <artifactId>gson</artifactId>
        <version>2.8.6</version>
    </dependency>
</dependencies>
```



**下面的Gson是除了SpringBoot和SpingCloud外需要统一版本的jar**

## 其他工程使用方法

在项目主pom.xml文件中`<dependencyManagement></dependencyManagement>`节点下加入BOM的GAV信息如下：

```xml
<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>com.niu.not</groupId>
            <artifactId>niu-dependency</artifactId>
            <version>1.1.1</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement>
```

在需要使用相关JAR包的pom.xml文件中<dependencies></dependencies>节点下引入如下：

```xml
<dependencies>
    <!--此时用到Spring和Gson都不需要加版本号,会自动引用BOM中提供的版本-->
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-openfeign</artifactId>
    </dependency>
    <dependency>
        <groupId>com.google.code.gson</groupId>
        <artifactId>gson</artifactId>
    </dependency>
</dependencies>
```

**这种设置后，如果项目要求升级Spring版本，只需要在提供方升级验证兼容性，然后修改BOM依赖即可**

如果需要使用不同于当前bom中所维护的jar包版本，则加上<version>覆盖即可，如：



```xml
<dependencies>
    <!--此时用到Spring和Gson都不需要加版本号,会自动引用BOM中提供的版本-->
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-openfeign</artifactId>
    </dependency>
    <dependency>
        <groupId>com.google.code.gson</groupId>
        <artifactId>gson</artifactId>
        <!--会覆盖掉BOM中声明的版本2.8.6，使用自定义版本2.8.2-->
        <version>2.8.2</version>
    </dependency>
</dependencies>
```

