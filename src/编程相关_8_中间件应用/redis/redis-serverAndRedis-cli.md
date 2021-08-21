

# CLI工具的类型

| 可执行文件      | 作用                            |
| --------------- | ------------------------------- |
| redis-server    | Redis Srver相关                 |
| redis-cli       | Redis命令行工具                 |
| redis-benchmark | 基准测试工具                    |
| redis-check-aof | AOF持久化文件检测工具和修复工具 |
| redis-check-rdb | RDB持久化文件检测工具和修复工具 |
| redis-sentinel  | Redis哨兵系统                   |



# redis-cli

**命令方式**

```
# 直接得到命令的返回结果,显示在屏幕上。
redis-cli -h {host} -p {port} {command}
```

**交互式命令行方式**

```
redis-cli -h {host} -p {port} 
```

**命令**

| 选项                | 说明                                                       |      |
| ------------------- | ---------------------------------------------------------- | ---- |
| time                | 返回当前服务器时间                                         |      |
| eval                | 运行lua脚本                                                |      |
| evalsha             | 根据给定的 sha1 校验码，执行缓存在服务器中的脚本。         |      |
| script exists       | 查看指定的脚本是否已经被保存在缓存当中                     |      |
| script flush        | 从脚本缓存中移除所有脚本                                   |      |
| script kill         | 杀死当前正在运行的 Lua 脚本                                |      |
| script load         | 将脚本添加到脚本缓存中,并不立即执行这个脚本                |      |
| dbsize              | 返回当前数据库的 key 的数量                                |      |
| client list         | 返回所有连接到服务器的客户端信息和统计数据                 |      |
| select              | 切换到指定的库                                             |      |
| quit                | 关闭连接                                                   |      |
| auth                | 密码认证                                                   |      |
| echo                | 打印字符串                                                 |      |
| ping                | 查看服务是否运行,如果Redis存活会返回pong                   |      |
| client kill ip:port | 关闭地址为 `ip:port` 的客户端                              |      |
| save                | 将数据同步保存到磁盘                                       |      |
| bgsave              | 将数据异步保存到磁盘                                       |      |
| lastsave            | 返回上次成功将数据保存到磁盘的Unix时戳                     |      |
| shundown            | 异步保存数据到硬盘，并关闭服务器                           |      |
| info                | 提供服务器的信息和统计                                     |      |
| config resetstat    | 重置info命令中的某些统计数据                               |      |
| config get          | 获取配置文件信息,`CONFIG GET *`获取所有配置信息            |      |
| config set          | 动态地调整 Redis 服务器的配置而无须重启                    |      |
| config rewrite      | Redis 服务器时所指定的 `redis.conf` 文件进行改写           |      |
| monitor             | 实时监控收到的所有请求                                     |      |
| slaveof             | 将当前服务器转变为指定服务器的从属服务器(slave server)     |      |
| role                | 返回主从实例所属的角色                                     |      |
| BGREWRITEAOF        | 异步执行一个 AOF（AppendOnly File） 文件重写操作           |      |
| CLIENT GETNAME      | 获取连接的名称                                             |      |
| CLIENT SETNAME      | 设置当前连接的名称                                         |      |
| CLIENT PAUSE        | 阻塞客户端命令一段时间（以毫秒计）                         |      |
| CLUSTER SLOTS       | 获取集群节点的映射数组                                     |      |
| COMMAND             | 获取 Redis 命令详情数组                                    |      |
| COMMAND COUNT       | 获取 Redis 命令总数                                        |      |
| COMMAND GETKEYS     | 获取给定命令的所有键                                       |      |
| COMMAND INFO        | 获取指定 Redis 命令描述的数组                              |      |
| DEBUG OBJECT        | 获取 key 的调试信息                                        |      |
| DEBUG SEGFAULT      | 让 Redis 服务崩溃                                          |      |
| FLUSHALL            | 删除所有数据库的所有key                                    |      |
| FLUSHDB             | 删除当前数据库的所有key                                    |      |
| SLOWLOG             | 管理 redis 的慢日志                                        |      |
| SYNC                | 用于复制功能(replication)的内部命令                        |      |
| memory purge        | 重整内存碎片,主动释放已删除的内存,会阻塞主线程 4.0以上版本 |      |







# redis-sever

**语法格式：**redis-server [参数]

**常用参数：**

| 参数         | 说明                                                 |
| ------------ | ---------------------------------------------------- |
| --port       | 配置端口                                             |
| --slaveof    | 将当前服务器转变为指定服务器的从属服务器             |
| --loglevel   | 配置日志级别                                         |
| --sentinel   | 以哨兵模式运行                                       |
| --masterauth | 如果主库设置了主从密码, 从库需要用该参数指定主从密码 |
| -a           | 指定密码                                             |





# 客户端选项`redis-cli`

|                     |                                                              |      |
| ------------------- | ------------------------------------------------------------ | ---- |
| 选项                | 说明                                                         | 案例 |
| -h                  | 指定Redis server地址                                         |      |
| -p                  | 指定Redis server端口号                                       |      |
| -s                  | 指定服务器套接字(覆盖主机名和端口)。                         |      |
| -a                  | 指定密码                                                     |      |
| -u                  | url格式的地址                                                |      |
| -r                  | 将命令重复执行N次                                            |      |
| -i                  | 每隔N秒执行一次命令，必须与-r一起使用。                      |      |
| -n                  | 选择库号                                                     |      |
| -x                  | 代表从标准输入读取数据作为该命令的最后一个参数。             |      |
| -d                  | 原始格式中的多块分隔符(默认值:\n)。                          |      |
| -c                  | 连接cluster集群结点时用的，此选项可防止moved和ask异常。      |      |
| --csv               | 将数据导出为CSV格式的文件                                    |      |
| --scan              | 获取服务器所有的键                                           |      |
| --pattern           | 指定scan获取的key的pattern,正则表达式用于scan命令后过滤.     |      |
| --slave             | 当前客户端模拟成当前redis节点的从节点，可用来获取指定redis节点的更新操作 |      |
| --rdb               | 导出rdb文件，保存导到指定的位置                              |      |
| --pipe              | 将命令封装成redis通信协议定义的数据格式，批量发送给redis执行。 |      |
| --pipe-timeout      | 设置管道超时时间                                             |      |
| --bigkeys           | 统计bigkey的分布，使用scan命令对redis的键进行采样，从中找到内存占用比较大的键 |      |
| --hotkeys           | 找出server中热点key                                          |      |
| --stat              | 实时获取redis的统计信息。istat和info相比可以看到一些增加的数据,如:每秒请求数 |      |
| --raw               | 显示格式化的效果                                             |      |
| --no-raw            | 要求返回原始格式                                             |      |
| --eval              | 用于执行lua脚本                                              |      |
| --latency           | 持续采样服务器延迟                                           |      |
| --latency-history   | 持续采样服务器延迟并每隔(15秒)输出一个记录; 可以使用-i 更改间隔时间 |      |
| --latency-dist      | 使用彩色终端显示一系列延时特征                               |      |
| --intrinsic-latency | 固有延迟,由于操作系统或虚拟机/容器带来的延迟,需要在redis-server的本器上进行测量. |      |
| --ldb               | 与--eval一起使用可以启用Redis Lua调试器                      |      |
| --ldb-sync-mode     | 比如--ldb，但是使用了同步Lua调试器, 此模式将阻塞服务器并更改脚本 |      |
| --lru-test          |                                                              |      |

# `redis-cli stat`

| 选项        | 说明                                                         | 案例 |
| ----------- | ------------------------------------------------------------ | ---- |
| keys        | server中key的数量                                            |      |
| mem         | 键值对的总内存量                                             |      |
| clients     | 当前连接的总clients数量                                      |      |
| blocked     | 正在等待执行阻塞命令（BLPOP、BRPOP、BRPOPLPUSH 等等）的客户端数量 |      |
| requests    | 服务器请求总次数 (+1) 截止上次请求增加次数                   |      |
| connections | 服务器连接次数                                               |      |



# 性能测试工具`redis-benchmark`

`redis-benchmark`命令不属于`redis-cli`而是在Redis的其他工具,默认在Redis目录下

| 选项  | 说明                                       | 案例 |
| ----- | ------------------------------------------ | ---- |
| -h    | 指定服务器主机名                           |      |
| -p    | 指定服务器端口                             |      |
| -s    | 指定服务器 socket                          |      |
| -c    | 指定并发连接数                             |      |
| -n    | 指定请求数                                 |      |
| -d    | 以字节的形式指定 SET/GET 值的数据大小      |      |
| -k    | 1=keep alive 0=reconnect                   |      |
| -r    | SET/GET/INCR 使用随机 key, SADD 使用随机值 |      |
| -P    | 通过管道传输 <numreq> 请求                 |      |
| -q    | 强制退出 redis。仅显示 query/sec 值        |      |
| --csv | 以 CSV 格式输出                            |      |
| -l    | 生成循环，永久执行测试                     |      |
| -t    | 仅运行以逗号分隔的测试命令列表。           |      |
| -I    | Idle 模式。仅打开 N 个 idle 连接并等待。   |      |