# **慢查询**

慢查询，到底多慢才叫慢？有没有统一的标准？其实呀，这并没有统一的标准，每个公司，甚至同一公司不同场景(数据库)都会有不同标准



像OLTP(联机事务处理), OLAP (联机分析处理) 这两者对慢查询的标准就不一样

OLAP 实时性则没那么高，对慢查询容错性也会更高些

而OLTP(联机事务处理)属于事务处理型，实时性要求高，响应时间快，对慢查询几乎零容忍



一个成熟系统中，有监控系统实时监控线上查询运行状态，提前将慢查询筛选出来，也是避免生产事故，降低风险的有效措施



有些数据库中也内置慢查询监控，如：MySQL慢查询日志就是其一。



# **开启慢查询日志**

在 MySQL中，提供了慢查询查询日志，基于性能方面的考虑，该配置默认为OFF(关闭) 状态。那么如何开启慢日志查询呢？其步骤如下：

```sql
show variables like "slow_query_log";
set global slow_query_log = "ON";
show variables like "slow_query_log_file";
# 其中: path 表示路径， filename 表示文件名，如果不指定，其默认filename 为hostname。
set global slow_query_log_file = ${path}/${filename}.log;
```

慢查询 查询时间，当SQL执行时间超过该值时，则会记录在slow_query_log_file 文件中，其默认为 10 ，最小值为 0，(单位：秒)。

```sql
 show variables like "long_query_time";
 set global long_query_time = 5;
```

当设置值小于0时，默认为 0。

 通过上述设置后，退出当前会话或者开启一个新的会话，执行如下命令：

```sql
select sleep(11);
```



```log
# Time: 200310 13:30:57
# User@Host: root[root] @ localhost []  Id: 21528
# Query_time: 6.000164  Lock_time: 0.000000 Rows_sent: 1  Rows_examined: 0
SET timestamp=1583818257;
select sleep(6);
```



# **慢查询日志文件**

1. 慢查询日志以#作为起始符。
2. User@Host：表示用户 和 慢查询查询的ip地址。
3. 如上所述，表示 root用户 localhost地址。
4. Query_time: 表示SQL查询持续时间， 单位 (秒)。
5. Lock_time: 表示获取锁的时间， 单位(秒)。
6. Rows_sent: 表示发送给客户端的行数。
7. Rows_examined: 表示：服务器层检查的行数。
8. set timestamp ：表示 慢SQL 记录时的时间戳。
9. 其中 select sleep(6) 则表示慢SQL语句。

### **注意事项**

1. 在 MySQL 中，慢查询日志中默认不记录管理语句，如：

**alter table, analyze table，check table等。**

```sql
set global log_slow_admin_statements = "ON";
```

2. 在 MySQL 中，还可以设置将**未走索引的SQL语句记录**在慢日志查询文件中(默认为关闭状态)。通过下述属性即可进行设置：

```sql
set global log_queries_not_using_indexes = "ON";
```

3. 日志输出格式有支持：FILE(默认)，TABLE 两种，可进行组合使用。如下所示:
   set global log_output = "FILE,TABLE";

这样设置会同时在 FILE, mysql库中的slow_log表中同时写入。

```text
 select * from slow_log;
```





# 特别注意

1. **设置该属性后，只要SQL未走索引，即使查询时间小于long_query_time值，也会记录在慢SQL日志文件中。**
2. **该设置会导致慢日志快速增长，开启前建议检查慢查询日志文件所在磁盘空间是否充足。**
3. **在生产环境中，不建议开启该参数。**





# 删除慢查询日志

```shell
mysqladmin -uroot -p flush-logs
```

