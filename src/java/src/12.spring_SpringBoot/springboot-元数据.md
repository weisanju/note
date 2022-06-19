# 什么是元数据文件

* springboot jar包含 元数据文件,这些文件提供了所有支持的配置属性
* 这些文件是为了让开发者在编辑application.properties,application.yml文件时有代码完成功能
* 大部分的元数据文件是 在编译期通过 带有 ConfigurationProperties 自动生成的
* 所有元数据在 *META-INF/spring-configuration-metadata.json*文件中



# 元数据格式

* json格式, 元数据项在 group类,或者properties类,值提示在 hints类

* group是 前缀,properties是具体的项

* 不要求每个 properties有一个组

  ```xml
  {"groups": [
      {
          "name": "server",
          "type": "org.springframework.boot.autoconfigure.web.ServerProperties",
          "sourceType": "org.springframework.boot.autoconfigure.web.ServerProperties"
      },
      {
          "name": "spring.jpa.hibernate",
          "type": "org.springframework.boot.autoconfigure.orm.jpa.JpaProperties$Hibernate",
          "sourceType": "org.springframework.boot.autoconfigure.orm.jpa.JpaProperties",
          "sourceMethod": "getHibernate()"
      }
      ...
  ],"properties": [
      {
          "name": "server.port",
          "type": "java.lang.Integer",
          "sourceType": "org.springframework.boot.autoconfigure.web.ServerProperties"
      },
      {
          "name": "server.address",
          "type": "java.net.InetAddress",
          "sourceType": "org.springframework.boot.autoconfigure.web.ServerProperties"
      },
      {
            "name": "spring.jpa.hibernate.ddl-auto",
            "type": "java.lang.String",
            "description": "DDL mode. This is actually a shortcut for the \"hibernate.hbm2ddl.auto\" property.",
            "sourceType": "org.springframework.boot.autoconfigure.orm.jpa.JpaProperties$Hibernate"
      }
      ...
  ],"hints": [
      {
          "name": "spring.jpa.hibernate.ddl-auto",
          "values": [
              {
                  "value": "none",
                  "description": "Disable DDL handling."
              },
              {
                  "value": "validate",
                  "description": "Validate the schema, make no changes to the database."
              },
              {
                  "value": "update",
                  "description": "Update the schema if necessary."
              },
              {
                  "value": "create",
                  "description": "Create the schema and destroy previous data."
              },
              {
                  "value": "create-drop",
                  "description": "Create and then destroy the schema at the end of the session."
              }
          ]
      }
  ]}
  ```

## group属性

| Name           | Type   | Purpose                                                      |
| :------------- | :----- | :----------------------------------------------------------- |
| `name`         | String | 组名,必填                                                    |
| `type`         | String | 组的数据类型的类名 ,如果组是基于类上的`@ConfigurationProperties`注解,该属性的值就是类的全名,如果基于@Bean,就是返回值的类,否则忽略 |
| description    | String | 最后一行必须以点结尾                                         |
| `sourceType`   | String | 如果是 `@Bean`方法的上注解带`@ConfigurationProperties`,则为  `@Configuration`的全类名 |
| `sourceMethod` | String | `@ConfigurationProperties``@Bean` 上的方法注解,返回方法的签名,(带参数列表) |

## Property Attributes

| Name           | Type        | Purpose                                                      |
| :------------- | :---------- | :----------------------------------------------------------- |
| `name`         | String      | 属性名,小写, 英文句号 分割,必填                              |
| `type`         | String      | 属性的数据类型的全签名,(`java.lang.String`,`java.util.Map<java.lang.String,acme.MyEnum>`),基本类型使用对应的包装类型, |
| `description`  | String      | 以 . 结尾                                                    |
| `sourceType`   | String      | 带有`@ConfigurationProperties`的类的全类名                   |
| `defaultValue` | Object      | 默认值,可以为数组                                            |
| `deprecation`  | Deprecation | 是否过期                                                     |

## Hint Attributes

| Name        | Type            | Purpose                                                      |
| :---------- | :-------------- | :----------------------------------------------------------- |
| `name`      | String          | 点号分隔的属性名,如果属性为 map则会提示map的keys,或者值提示values |
| `values`    | ValueHint[]     | 数组值                                                       |
| `providers` | ValueProvider[] | 定义 providers的 名字和参数                                  |

## 值提示的values

| Name          | Type   | Purpose  |
| :------------ | :----- | :------- |
| `value`       | Object | 值       |
| `description` | String | 以点结尾 |

## providers

| Name         | Type        | Purpose                                                 |
| :----------- | :---------- | :------------------------------------------------------ |
| `name`       | String      | 提供该值的提供者,用于指示属性的值由项目中的哪些属性提供 |
| `parameters` | JSON object | 额外的参数                                              |



## 属性过期指定

```java
@ConfigurationProperties("app.acme")
public class AcmeProperties {

    private String name;

    public String getName() { ... }

    public void setName(String name) { ... }

    @DeprecatedConfigurationProperty(replacement = "app.acme.name")
    @Deprecated
    public String getTarget() {
        return getName();
    }

    @Deprecated
    public void setTarget(String target) {
        setName(target);
    }
}
```

# 手动值提示

## 示例

```java
@ConfigurationProperties("sample")
public class SampleProperties {

    private Map<String,Integer> contexts;
    // getters and setters
}
```

```json
{"hints": [
    {
        "name": "sample.contexts.keys",
        "values": [
            {
                "value": "sample1"
            },
            {
                "value": "sample2"
            }
        ]
    }
]}
```



## Value Providers

valueProviders是一个强大的方式给 属性附加额外语义

| Name                    | Description                                                  |
| :---------------------- | :----------------------------------------------------------- |
| `any`                   | 可以添加任何属性,                                            |
| `class-reference`       | 自动完成 引用可用的类,可以被 变量 限制                       |
| `handle-as`             | Handles the property as if it were defined by the type defined by the mandatory `target` parameter. |
| `logger-name`           | Auto-completes valid logger names and [logger groups](https://docs.spring.io/spring-boot/docs/2.3.1.RELEASE/reference/html/spring-boot-features.html#boot-features-custom-log-groups). Typically, package and class names available in the current project can be auto-completed as well as defined groups. |
| `spring-bean-reference` | Auto-completes the available bean names in the current project. Usually constrained by a base class that is specified by the `target` parameter. |
| `spring-profile-name`   | Auto-completes the available Spring profile names in the project. |

### any

如果您具有值列表并且任何其他值仍应视为有效，

```json
{"hints": [
    {
        "name": "system.state",
        "values": [
            {
                "value": "on"
            },
            {
                "value": "off"
            }
        ],
        "providers": [
            {
                "name": "any"
            }
        ]
    }
]}
```

### Class Reference

| Parameter  | Type               | Default value | Description                                 |
| :--------- | :----------------- | :------------ | :------------------------------------------ |
| `target`   | `String` (`Class`) | *none*        | 指定类名,则只会提示项目里存在的类或者其子类 |
| `concrete` | `boolean`          | true          | 是否需要精确到 全类名                       |

```json
{"hints": [
    {
        "name": "server.servlet.jsp.class-name",
        "providers": [
            {
                "name": "class-reference",
                "parameters": {
                    "target": "javax.servlet.http.HttpServlet"
                }
            }
        ]
    }
]}
```

### Handle As

| Parameter    | Type               | Default value | Description      |
| :----------- | :----------------- | :------------ | :--------------- |
| **`target`** | `String` (`Class`) | *none*        | 依赖其他高级属性 |

- Any `java.lang.Enum`: Lists the possible values for the property. (We recommend defining the property with the `Enum` type, as no further hint should be required for the IDE to auto-complete the values)
- `java.nio.charset.Charset`: Supports auto-completion of charset/encoding values (such as `UTF-8`)
- `java.util.Locale`: auto-completion of locales (such as `en_US`)
- `org.springframework.util.MimeType`: Supports auto-completion of content type values (such as `text/plain`)
- `org.springframework.core.io.Resource`: Supports auto-completion of Spring’s Resource abstraction to refer to a file on the filesystem or on the classpath (such as `classpath:/sample.properties`)

```json
{"hints": [
    {
        "name": "spring.liquibase.change-log",
        "providers": [
            {
                "name": "handle-as",
                "parameters": {
                    "target": "org.springframework.core.io.Resource"
                }
            }
        ]
    }
]}
```

### Logger Name

| Parameter | Type      | Default value | Description                                        |
| :-------- | :-------- | :------------ | :------------------------------------------------- |
| `group`   | `boolean` | `true`        | Specify whether known groups should be considered. |

内置的logger日志级别定义

```
{"hints": [
    {
        "name": "logging.level.keys",
        "values": [
            {
                "value": "root",
                "description": "Root logger used to assign the default logging level."
            },
            {
                "value": "sql",
                "description": "SQL logging group including Hibernate SQL logger."
            },
            {
                "value": "web",
                "description": "Web logging group including codecs."
            }
        ],
        "providers": [
            {
                "name": "logger-name"
            }
        ]
    },
    {
        "name": "logging.level.values",
        "values": [
            {
                "value": "trace"
            },
            {
                "value": "debug"
            },
            {
                "value": "info"
            },
            {
                "value": "warn"
            },
            {
                "value": "error"
            },
            {
                "value": "fatal"
            },
            {
                "value": "off"
            }

        ],
        "providers": [
            {
                "name": "any"
            }
        ]
    }
]}
```

### Spring Bean Reference

| Parameter | Type               | Default value | Description                    |
| :-------- | :----------------- | :------------ | :----------------------------- |
| `target`  | `String` (`Class`) | *none*        | 配置成springBean的类的全限定名 |

```json
{"hints": [
    {
        "name": "spring.jmx.server",
        "providers": [
            {
                "name": "spring-bean-reference",
                "parameters": {
                    "target": "javax.management.MBeanServer"
                }
            }
        ]
    }
]}
```

### Spring Profile Name

与*spring.profiles.active* 相对应

```
{"hints": [
    {
        "name": "spring.profiles.active",
        "providers": [
            {
                "name": "spring-profile-name"
            }
        ]
    }
]}
```



# 产生自己的元数据

1. META-INF/additional-spring-configuration-metadata.json文件位置

2. 通过 `spring-boot-configuration-processor` jar的 @*ConfigurationProperties*配置产生自己的元数据

   ```xml
   <dependency>
       <groupId>org.springframework.boot</groupId>
       <artifactId>spring-boot-configuration-processor</artifactId>
       <optional>true</optional>
   </dependency>
   ```

   

3. If you are using an `additional-spring-configuration-metadata.json` file, the `compileJava` task should be configured to depend on the `processResources` task, as shown in the following example:

   ```java
   compileJava.inputs.files(processResources)
   ```

   

3. 处理器会处理 类和方法上的 @ConfigurationProperties注解

```xml
@ConfigurationProperties(prefix = "acme.messaging")
public class MessagingProperties {

    private List<String> addresses = new ArrayList<>(Arrays.asList("a", "b"));

    private ContainerType containerType = ContainerType.SIMPLE;

    // ... getter and setters

    public enum ContainerType {

        SIMPLE,
        DIRECT

    }

}
{"properties": [
    {
        "name": "acme.messaging.addresses",
        "defaultValue": ["a", "b"]
    },
    {
        "name": "acme.messaging.container-type",
        "defaultValue": "simple"
    }
]}
```

## 嵌套属性

```
@ConfigurationProperties(prefix="server")
public class ServerProperties {

    private String name;

    private Host host;

    // ... getter and setters

    public static class Host {

        private String ip;

        private int port;

        // ... getter and setters

    }

}
对应内部类
可以对该字段使用 @NestedConfigurationProperty 来使用外部类
```





[官网地址](https://docs.spring.io/spring-boot/docs/2.3.1.RELEASE/reference/html/appendix-configuration-metadata.html)



