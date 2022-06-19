# 简介

是一个高性能的key-value数据库

Redis支持数据的持久化

Redis不仅仅支持简单的key-value类型的数据，同时还提供list，set，zset，hash等数据结构的存储

Redis支持数据的备份，即master-slave模式的数据备份

# 安装

[下载地址](https://github.com/dmajkic/redis/downloads。)

运行命令:`redis-server.exe redis.conf`

`redis-cli.exe -h 127.0.0.1 -p 6379`

```shell
curl -O  http://download.redis.io/releases/redis-6.0.6.tar.gz
yum  -y  install  centos-release-scl
yum  -y  install  devtoolset-9-gcc  devtoolset-9-gcc-c++  devtoolset-9-binutils
scl enable devtoolset-9 bash
echo "source /opt/rh/devtoolset-9/enable" >>/etc/profile
tar -xf redis-6.0.6.tar.gz
cd redis-6.0.6
make

make  install  PREFIX=/usr/local/redis-6.0.6
cp redis.conf sentinel.conf   /usr/local/redis-6.0.6/
```



# 文件目录

| 文件名           | 说明                       |
| ---------------- | -------------------------- |
| redis-server     | 服务                       |
| redis-cli        | 命令行客户端               |
| redis-benchmark  | 性能测试工具               |
| redis-check-aof  | AOF 文件修复工具           |
| redis-check-dump | RDB 文件检查工具           |
| redis-sentinel   | Sentinel 服务器（v2.8 后） |

# 多数据库支持

Redis 默认支持 16 个数据库，不可以自定义数据库名，只能根据编号命名，默认从 0 开始。

不支持为每一个数据库设置密码，一个客户端要么能访问所有库，要么没权限访问。



# Redis 安全

* `CONFIG get requirepass`
* 设置密码:`CONFIG set requirepass "xjq"`
* 验证密码:`AUTH <password>`

# Redis 性能测试

`redis-benchmark [option] [option value]`

|      |           |                                            |           |
| ---- | --------- | ------------------------------------------ | --------- |
| 1    | **-h**    | 指定服务器主机名                           | 127.0.0.1 |
| 2    | **-p**    | 指定服务器端口                             | 6379      |
| 3    | **-s**    | 指定服务器 socket                          |           |
| 4    | **-c**    | 指定并发连接数                             | 50        |
| 5    | **-n**    | 指定请求数                                 | 10000     |
| 6    | **-d**    | 以字节的形式指定 SET/GET 值的数据大小      | 2         |
| 7    | **-k**    | 1=keep alive 0=reconnect                   | 1         |
| 8    | **-r**    | SET/GET/INCR 使用随机 key, SADD 使用随机值 |           |
| 9    | **-P**    | 通过管道传输 <numreq> 请求                 | 1         |
| 10   | **-q**    | 强制退出 redis。仅显示 query/sec 值        |           |
| 11   | **--csv** | 以 CSV 格式输出                            |           |
| 12   | **-l**    | 生成循环，永久执行测试                     |           |
| 13   | **-t**    | 仅运行以逗号分隔的测试命令列表。           |           |
| 14   | **-I**    | Idle 模式。仅打开 N 个 idle 连接并等待。   |           |

# 客户端命令

| S.N. | 命令               | 描述                                       |
| :--- | :----------------- | :----------------------------------------- |
| 1    | **CLIENT LIST**    | 返回连接到 redis 服务的客户端列表          |
| 2    | **CLIENT SETNAME** | 设置当前连接的名称                         |
| 3    | **CLIENT GETNAME** | 获取通过 CLIENT SETNAME 命令设置的服务名称 |
| 4    | **CLIENT PAUSE**   | 挂起客户端连接，指定挂起的时间以毫秒计     |
| 5    | **CLIENT KILL**    | 关闭客户端连接                             |



# 脚本

Lua 语言（Open Rest，Nginx 也可以使用 lua 语言，有机会学习了解一下）

**这里不详细记录 Lua 语法，不过有一点思考，既然 Nginx 也可以使用 Lua，那么可以就有一种场景，Nginx 通过 lua 访问 Redis 读取数据，并且用 lua 渲染模板，达到页面直出，这样应该效率很高。**

# 事务

*Redis 事务可以保证一个事务内的命令依次执行而不被其他命令插入。*

Redis 事务的异常处理，首先需要先明确什么原因导致执行出错。

1）语法错，一旦前面有错，后面不会执行；

2）运行错，一旦有错，后续的命令会继续执行；

*Redis 的事务没有关系数据库事务提供的回滚（rollback）[1]功能。为此开发者必须在事务执行出错后自己收拾剩下的摊子（将数据库复原回事务执行前的状态等）。*



# 持久化

- RDB （通过快照完成，当达到某种约定条件后自动生成一份备份并存储在硬盘上），**快照原理**
- AOF （存储非临时数据，每执行一条都会追加存储在硬盘，有一些性能影响）默认关闭，通过 `appendonly yes` 开启

允许同时开启 RDB 和 AOF 两种模式。

# 集群

Redis 支持集群，可以通过主从数据库来来规避单点数据库故障导致的问题。主数据库负责读写（读写分离也可以），当写操作导致数据变化时自动将数据同步给从库，从库只读，并只接受主库同步数据。

配置文件，通过 slaveof 主库地址 主库端口 来完成主从复制的配置。

> 通过复制可以实现读写分离，以提高服务器负载能力。

关键字记录，Redis 支持**哨兵**，一主多从，需要自动监控 Redis 运行情况，作用：1)监控主从数据库运行正常；2)主数据库故障自动将从数据库转换成主数据库；细节待补充一篇琢磨透彻一点的分析文。



# Java使用redis

## 单链接

```java
    Jedis jedis = new Jedis("localhost", 6379, 100000);
    jedis.auth("xjq");

    int i = 0;
    try {
        long start = System.currentTimeMillis();// 开始毫秒数
        while (true) {
            long end = System.currentTimeMillis();
            if (end - start >= 1000) {// 当大于等于1000毫秒（相当于1秒）时，结束操作
                break;
            }
            i++;
            jedis.set("test" + i, i + "");
        }
    } finally {// 关闭连接
        jedis.close();
    }
    // 打印1秒内对Redis的操作次数
    System.out.println("redis每秒操作：" + i + "次");
}
```

## 连接池

```java
        JedisPoolConfig poolConfig = new JedisPoolConfig();
// 最大空闲数
        poolConfig.setMaxIdle(50);
// 最大连接数
        poolConfig.setMaxTotal(100);
// 最大等待毫秒数
        poolConfig.setMaxWaitMillis(20000);
// 使用配置创建连接池
        JedisPool pool = new JedisPool(poolConfig, "localhost");
// 从连接池中获取单个连接
        Jedis jedis = pool.getResource();
// 如果需要密码
        jedis.auth("xjq");

        System.out.println(jedis.get("xjq"));
```

## 在 Spring 中使用 Redis

```java
    @Bean
    public RedisTemplate redisTemplate(){
        RedisStandaloneConfiguration configuration = new RedisStandaloneConfiguration();
        configuration.setHostName("127.0.0.1");
        configuration.setPort(6379);
        configuration.setPassword("xjq");
        JedisConnectionFactory jedisConnectionFactory = new JedisConnectionFactory(configuration);
        RedisTemplate<Object, Object> redisTemplate = new RedisTemplate<>();
        redisTemplate.setConnectionFactory(jedisConnectionFactory);
        redisTemplate.setKeySerializer(new StringRedisSerializer());
        redisTemplate.setValueSerializer(new StringRedisSerializer() );
        return redisTemplate;
    }
```

```java
private static void spring_test() {
    AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext(MyConfig.class);
    applicationContext.register(MyConfig.class);

    RedisTemplate bean = applicationContext.getBean(RedisTemplate.class);
    bean.opsForValue().set("xjq","sxxx");
}
```

## springBooter配置

```java
@Bean
public RedisTemplate redisTemplate(RedisConnectionFactory redisConnectionFactory){
    RedisTemplate<String, String> redisTemplate = new RedisTemplate<>();
    redisTemplate.setKeySerializer(new StringRedisSerializer());
    redisTemplate.setConnectionFactory(redisConnectionFactory);
    redisTemplate.setValueSerializer(new StringRedisSerializer());
    return redisTemplate;
}
```

```
spring:
  redis:
    password: xjq
    host: localhost
    port: 6379


  main:
    web-application-type: none
```





[redis学习](https://github.com/yuzh233/redis-learning)

