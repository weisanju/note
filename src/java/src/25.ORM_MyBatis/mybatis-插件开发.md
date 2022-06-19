# mybatis插件介绍

## 什么是插件

与其称为Mybatis插件，不如叫Mybatis拦截器，更加符合其功能定位，实际上它就是一个拦截器，应用代理模式，在方法级别上进行拦截。

## 支持拦截的方法

- 执行器Executor（update、query、commit、rollback等方法）；
- 参数处理器ParameterHandler（getParameterObject、setParameters方法）；
- 结果集处理器ResultSetHandler（handleResultSets、handleOutputParameters等方法）；
- SQL语法构建器StatementHandler（prepare、parameterize、batch、update、query等方法）；

## 拦截阶段

# mybatis插件场景

> Mybatis插件可以在DAO层进行拦截，如打印执行的SQL语句日志，做一些权限控制，分页等功能

## 分页功能

​	mybatis的分页默认是基于内存分页的（查出所有，再截取），数据量大的情况下效率较低，不过使用mybatis插件可以改变该行为，只需要拦截StatementHandler类的prepare方法，改变要执行的SQL语句为分页语句即可；

## 公共字段统一赋值

​	一般业务系统都会有创建者，创建时间，修改者，修改时间四个字段，对于这四个字段的赋值，实际上可以在DAO层统一拦截处理，可以用mybatis插件拦截Executor类的update方法，对相关参数进行统一赋值即可；

## 性能监控

​	对于SQL语句执行的性能监控，可以通过拦截Executor类的update, query等方法，用日志记录每个方法执行的时间；



# mybatis SQL抽象

## MappedStatement

```csharp
<select id="selectAuthorLinkedHashMap" resultType="java.util.LinkedHashMap">
        select id, username from author where id = #{value}
</select>
```

一个sql标签 对应 一个  MappedStatement对象

## SqlSource

负责根据用户传递的parameterObject,动态地生成SQL语句,将信息封装到BoundSql对象中，并返回

## BoundSql

表示动态生成的SQL语句以及相应的参数信息





