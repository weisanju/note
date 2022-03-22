# MySQL 5.7 mysqlpump 备份工具说明

mysqlpump和mysqldump一样，属于逻辑备份，备份以SQL形式的文本保存。逻辑备份相对物理备份的好处是不关心undo log的大小，直接备份数据即可。它最主要的特点是：

- **并行备份**数据库和数据库中的对象的，加快备份过程。
- **更好的控制数据库和数据库对象**（表，存储过程，用户帐户）的备份。
- 备份用户账号作为帐户管理语句（CREATE USER，GRANT），而不是直接插入到MySQL的系统数据库。
- **备份出来直接生成压缩后的备份文件。**
- 备份进度指示（估计值）。
- 重新加载（还原）备份文件，先建表后插入数据最后建立索引，减少了索引维护开销，加快了还原速度。
- 备份可以排除或则指定数据库。

# 选项说明

**参数：**绝大部分参数和mysqldump一致，顺便复习一下。对于mysqlpump参数会标记出来。



## mysqldump与mysqlpump共同拥有

[--add-drop-database](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_add-drop-database)：在建立库之前先执行删库操作。

```
DROP DATABASE IF EXISTS `...`;
```



[--add-drop-table](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_add-drop-table)：在建表之前先执行删表操作。

```
DROP TABLE IF EXISTS `...`.`...`;
```



[--add-locks](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_add-locks)：备份表时，使用LOCK TABLES和UNLOCK TABLES。**注意：**这个参数不支持并行备份，需要关闭并行备份功能：[--default-parallelism](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_default-parallelism)=0 

```
LOCK TABLES `...`.`...` WRITE;
...
UNLOCK TABLES;
```



[--all-databases](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_all-databases)：备份所有库，-A。



[--bind-address](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_bind-address)：指定通过哪个网络接口来连接Mysql服务器（一台服务器可能有多个IP），防止同一个网卡出去影响业务。



[--complete-insert](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_complete-insert)：dump出包含所有列的完整insert语句。



[--compress](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_compress)： 压缩客户端和服务器传输的所有的数据，-C。



[--databases](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_databases)：手动指定要备份的库，支持多个数据库，用空格分隔，-B。

[--default-character-set](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_default-character-set)：指定备份的字符集。



[--events](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_events)：备份数据库的事件，默认开启，关闭使用--skip-events参数。

[--insert-ignore](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_insert-ignore)：备份用insert ignore语句代替insert语句。

[--log-error-file](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_log-error-file)：备份出现的warnings和erros信息输出到一个指定的文件。

[--max-allowed-packet](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_max-allowed-packet)：备份时用于client/server直接通信的最大buffer包的大小。

[--net-buffer-length](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_net-buffer-length)：备份时用于client/server通信的初始buffer大小，当创建多行插入语句的时候，mysqlpump 创建行到N个字节长。



[--no-create-db](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_no-create-db)：备份不写CREATE DATABASE语句。要是备份多个库，需要使用参数-B，而使用-B的时候会出现create database语句，该参数可以屏蔽create database 语句。

[--no-create-info](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_no-create-info)：备份不写建表语句，即不备份表结构，只备份数据，-t。

[--hex-blob](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_hex-blob)： 备份binary字段的时候使用十六进制计数法，受影响的字段类型有BINARY、VARBINARY、BLOB、BIT。

[--host](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_host) ：备份指定的数据库地址，-h。

[--password](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_password)：备份需要的密码。

[--port](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_port) ：备份数据库的端口。

[--protocol={TCP|SOCKET|PIPE|MEMORY}](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_protocol)：指定连接服务器的协议。

[--replace](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_replace)：备份出来replace into语句。

[--routines](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_routines)：备份出来包含存储过程和函数，默认开启，需要对 `mysql.proc表有查看权限。生成的文件中会包含CREATE PROCEDURE 和 CREATE FUNCTION语句以用于恢复，关闭则需要用--skip-routines参数。`

[--triggers](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_triggers)：备份出来包含触发器，默认开启，使用`--skip-triggers来关闭。`

[--set-charset](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_set-charset)：备份文件里写SET NAMES default_character_set 到输出，此参默认开启。 -- skip-set-charset禁用此参数，不会在备份文件里面写出set names...

[--single-transaction](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_single-transaction)：该参数在事务隔离级别设置成Repeatable Read，并在dump之前发送start transaction 语句给服务端。这在使用innodb时很有用，因为在发出start transaction时，保证了在不阻塞任何应用下的一致性状态。对myisam和memory等非事务表，还是会改变状态的，当使用此参的时候要确保没有其他连接在使用ALTER TABLE、CREATE TABLE、DROP TABLE、RENAME TABLE、TRUNCATE TABLE等语句，否则会出现不正确的内容或则失败。--add-locks和此参互斥，在mysql5.7.11之前，--default-parallelism大于1的时候和此参也互斥，必须使用--default-parallelism=0。5.7.11之后解决了--single-transaction和--default-parallelism的互斥问题。

[--skip-definer](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_skip-definer)：忽略那些创建视图和存储过程用到的 DEFINER 和 SQL SECURITY 语句，恢复的时候，会使用默认值，否则会在还原的时候看到没有DEFINER定义时的账号而报错。

[--socket](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_socket)：对于连接到localhost，Unix使用套接字文件，在Windows上是命名管道的名称使用，-S。



[--ssl](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_ssl)：--ssl参数将要被去除，用[--ssl-mode](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_ssl)取代。关于ssl相关的备份，请看[官方文档](https://dev.mysql.com/doc/refman/5.7/en/secure-connection-options.html#option_general_ssl-mode)。



[--tz-utc](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_tz-utc)：备份时会在备份文件的最前几行添加SET TIME_ZONE='+00:00'。**注意：**如果还原的服务器不在同一个时区并且还原表中的列有timestamp字段，会导致还原出来的结果不一致。默认开启该参数，用 `--skip-tz-utc来关闭参数。`

[--user](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_user)：备份时候的用户名，-u。

[--users](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_users)：备份数据库用户，备份的形式是CREATE USER...，GRANT...，只备份数据库账号可以通过如下命令：

```
mysqlpump --exclude-databases=% --users    #过滤掉所有数据库
```

[--watch-progress](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_watch-progress)：定期显示进度的完成，包括总数表、行和其他对象。该参数默认开启，用`--skip-watch-progress来关闭。`

## mysqlpump新增功能

[--add-drop-user](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_add-drop-user)：在CREATE USER语句之前增加DROP USER，**注意：**这个参数需要和[--users](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_users)一起使用，否者不生效。

```
DROP USER 'backup'@'192.168.123.%';
```

[--compress-output](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_compress-output)：默认不压缩输出，目前可以使用的压缩算法有LZ4和ZLIB。

```
shell> mysqlpump --compress-output=LZ4 > dump.lz4
shell> lz4_decompress dump.lz4 dump.txt

shell> mysqlpump --compress-output=ZLIB > dump.zlib
shell> zlib_decompress dump.zlib dump.txt
```

[--skip-dump-rows](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_skip-dump-rows)：

只备份表结构，不备份数据，-d。**注意：**mysqldump支持--no-data，mysqlpump不支持--no-data

[--default-parallelism](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_default-parallelism)：指定并行线程数，默认是2，如果设置成0，表示不使用并行备份。**注意：**每个线程的备份步骤是：先create table但不建立二级索引（主键会在create table时候建立），再写入数据，最后建立二级索引。

[--defer-table-indexes](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_defer-table-indexes)：

延迟创建索引，直到所有数据都加载完之后，再创建索引，默认开启。若关闭则会和mysqldump一样：先创建一个表和所有索引，再导入数据，因为在加载还原数据的时候要维护二级索引的开销，导致效率比较低。关闭使用参数：[--skip--defer-table-indexes](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_defer-table-indexes)。

[--exclude-databases](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_exclude-databases)：备份排除该参数指定的数据库，多个用逗号分隔。类似的还有[--exclude-events](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_exclude-events)、[--exclude-routines](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_exclude-routines)、[--exclude-tables](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_exclude-tables)、[--exclude-triggers](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_exclude-triggers)、[--exclude-users](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_exclude-users)。

```
mysqlpump --exclude-databases=mysql,sys    #备份过滤mysql和sys数据库

mysqlpump --exclude-tables=rr,tt   #备份过滤所有数据库中rr、tt表

mysqlpump -B test --exclude-tables=tmp_ifulltext,tt #备份过滤test库中的rr、tt表
```

要是只备份数据库的账号，需要添加参数[--users](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_users)，并且需要过滤掉所有的数据库，如：

```
mysqlpump --users --exclude-databases=sys,mysql,db1,db2 --exclude-users=dba,backup  #备份除dba和backup的所有账号。
```

[--include-databases](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_include-databases)：指定备份数据库，多个用逗号分隔，类似的还有[--include-events](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_include-events)、[--include-routines](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_include-routines)、[--include-tables](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_include-tables)、[--include-triggers](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_include-triggers)、[--include-users](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html#option_mysqlpump_include-users)，大致方法使用同

`--parallel-schemas=[N:]db_list：`

指定并行备份的库，多个库用逗号分隔，如果指定了N，将使用N个线程的地队列，如果N不指定，将由 --default-parallelism才确认N的值，可以设置多个`--parallel-schemas。`

```
mysqlpump --parallel-schemas=4:vs,aa --parallel-schemas=3:pt   #4个线程备份vs和aa，3个线程备份pt。通过show processlist 可以看到有7个线程。

mysqlpump --parallel-schemas=vs,abc --parallel-schemas=pt  #默认2个线程，即2个线程备份vs和abc，2个线程备份pt

####当然要是硬盘IO不允许的话，可以少开几个线程和数据库进行并行备份
```



[官方文档](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html)

[参考文档](https://www.cnblogs.com/zhoujinyi/p/5684903.html)

