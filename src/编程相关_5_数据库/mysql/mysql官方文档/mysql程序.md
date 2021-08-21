# mysql server 启停脚本

## [**mysqld**](https://dev.mysql.com/doc/refman/5.7/en/mysqld.html)

服务程序,主要进程

## [**mysqld_safe**](https://dev.mysql.com/doc/refman/5.7/en/mysqld-safe.html)

启动脚本

 [**mysqld_safe**](https://dev.mysql.com/doc/refman/5.7/en/mysqld-safe.html) attempts to start [**mysqld**](https://dev.mysql.com/doc/refman/5.7/en/mysqld.html)

See [Section 4.3.2, “**mysqld_safe** — MySQL Server Startup Script”](https://dev.mysql.com/doc/refman/5.7/en/mysqld-safe.html).



## [**mysql.server**](https://dev.mysql.com/doc/refman/5.7/en/mysql-server.html)

启动脚本

使用在 system-V 风格的  通过目录脚本划分 系统特定服务等级

它调用了 [**mysqld_safe**](https://dev.mysql.com/doc/refman/5.7/en/mysqld-safe.html) to start the MySQL server. 

See [Section 4.3.3, “**mysql.server** — MySQL Server Startup Script”](https://dev.mysql.com/doc/refman/5.7/en/mysql-server.html).



## [**mysqld_multi**](https://dev.mysql.com/doc/refman/5.7/en/mysqld-multi.html)

多服务启停

See [Section 4.3.4, “**mysqld_multi** — Manage Multiple MySQL Servers”](https://dev.mysql.com/doc/refman/5.7/en/mysqld-multi.html).



# 安装升级脚本

## [**comp_err**](https://dev.mysql.com/doc/refman/5.7/en/comp-err.html)

在编译构建过程中, 从错误文件中 解析出错误消息

See [Section 4.4.1, “**comp_err** — Compile MySQL Error Message File”](https://dev.mysql.com/doc/refman/5.7/en/comp-err.html).



## [**mysql_install_db**](https://dev.mysql.com/doc/refman/5.7/en/mysql-install-db.html)

初始化 mysql data目录

创建mysql数据库

初始化默认表权限

初始化innodb 表空间

 when first installing MySQL on a system. See [Section 4.4.2, “**mysql_install_db** — Initialize MySQL Data Directory”](https://dev.mysql.com/doc/refman/5.7/en/mysql-install-db.html), and [Section 2.10, “Postinstallation Setup and Testing”](https://dev.mysql.com/doc/refman/5.7/en/postinstallation.html).



## [**mysql_plugin**](https://dev.mysql.com/doc/refman/5.7/en/mysql-plugin.html)

This program configures MySQL server plugins. See [Section 4.4.3, “**mysql_plugin** — Configure MySQL Server Plugins”](https://dev.mysql.com/doc/refman/5.7/en/mysql-plugin.html).



## [**mysql_secure_installation**](https://dev.mysql.com/doc/refman/5.7/en/mysql-secure-installation.html)

This program enables you to improve the security of your MySQL installation. See [Section 4.4.4, “**mysql_secure_installation** — Improve MySQL Installation Security”](https://dev.mysql.com/doc/refman/5.7/en/mysql-secure-installation.html).



## [**mysql_ssl_rsa_setup**](https://dev.mysql.com/doc/refman/5.7/en/mysql-ssl-rsa-setup.html)

This program creates the SSL certificate and key files and RSA key-pair files required to support secure connections, if those files are missing. Files created by [**mysql_ssl_rsa_setup**](https://dev.mysql.com/doc/refman/5.7/en/mysql-ssl-rsa-setup.html) can be used for secure connections using SSL or RSA. See [Section 4.4.5, “**mysql_ssl_rsa_setup** — Create SSL/RSA Files”](https://dev.mysql.com/doc/refman/5.7/en/mysql-ssl-rsa-setup.html).

## [**mysql_tzinfo_to_sql**](https://dev.mysql.com/doc/refman/5.7/en/mysql-tzinfo-to-sql.html)

This program loads the time zone tables in the `mysql` database using the contents of the host system zoneinfo database (the set of files describing time zones). See [Section 4.4.6, “**mysql_tzinfo_to_sql** — Load the Time Zone Tables”](https://dev.mysql.com/doc/refman/5.7/en/mysql-tzinfo-to-sql.html)



## [**mysql_upgrade**](https://dev.mysql.com/doc/refman/5.7/en/mysql-upgrade.html)

This program is used after a MySQL upgrade operation. It updates the grant tables with any changes that have been made in newer versions of MySQL, and checks tables for incompatibilities and repairs them if necessary. See [Section 4.4.7, “**mysql_upgrade** — Check and Upgrade MySQL Tables”](https://dev.mysql.com/doc/refman/5.7/en/mysql-upgrade.html).



# mysql客户端程序

## [**mysql**](https://dev.mysql.com/doc/refman/5.7/en/mysql.html)

mysql命令行工具,从文件中执行批量sql

See [Section 4.5.1, “**mysql** — The MySQL Command-Line Client”](https://dev.mysql.com/doc/refman/5.7/en/mysql.html).



## [**mysqladmin**](https://dev.mysql.com/doc/refman/5.7/en/mysqladmin.html)

执行管理员操作

数据库创建

重新加载授权表

重开日志文件

查询版本,进程,状态信息,

See [Section 4.5.2, “**mysqladmin** — A MySQL Server Administration Program”](https://dev.mysql.com/doc/refman/5.7/en/mysqladmin.html).



## [**mysqlcheck**](https://dev.mysql.com/doc/refman/5.7/en/mysqlcheck.html)

检查维护分析 优化表,

See [Section 4.5.3, “**mysqlcheck** — A Table Maintenance Program”](https://dev.mysql.com/doc/refman/5.7/en/mysqlcheck.html).

## [**mysqldump**](https://dev.mysql.com/doc/refman/5.7/en/mysqldump.html)

导出数据库为文件

See [Section 4.5.4, “**mysqldump** — A Database Backup Program”](https://dev.mysql.com/doc/refman/5.7/en/mysqldump.html).

## [**mysqlimport**](https://dev.mysql.com/doc/refman/5.7/en/mysqlimport.html)

导入程序

See [Section 4.5.5, “**mysqlimport** — A Data Import Program”](https://dev.mysql.com/doc/refman/5.7/en/mysqlimport.html).

## [**mysqlpump**](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html)

A client that dumps a MySQL database into a file as SQL. See [Section 4.5.6, “**mysqlpump** — A Database Backup Program”](https://dev.mysql.com/doc/refman/5.7/en/mysqlpump.html).



## **mysqlsh**

MySQL Shell is an advanced client and code editor for MySQL Server. See [MySQL Shell 8.0 (part of MySQL 8.0)](https://dev.mysql.com/doc/mysql-shell/8.0/en/). In addition to the provided SQL functionality, similar to [**mysql**](https://dev.mysql.com/doc/refman/5.7/en/mysql.html),



see [Chapter 19, *Using MySQL as a Document Store*](https://dev.mysql.com/doc/refman/5.7/en/document-store.html).

see [Chapter 20, *InnoDB Cluster*](https://dev.mysql.com/doc/refman/5.7/en/mysql-innodb-cluster-userguide.html).



## [**mysqlshow**](https://dev.mysql.com/doc/refman/5.7/en/mysqlshow.html)

显示数据库信息 

数据库

表

列

索引

See [Section 4.5.7, “**mysqlshow** — Display Database, Table, and Column Information”](https://dev.mysql.com/doc/refman/5.7/en/mysqlshow.html).



## [**mysqlslap**](https://dev.mysql.com/doc/refman/5.7/en/mysqlslap.html)

负载模拟客户端

See [Section 4.5.8, “**mysqlslap** — A Load Emulation Client”](https://dev.mysql.com/doc/refman/5.7/en/mysqlslap.html).



# 管理工具

## [**innochecksum**](https://dev.mysql.com/doc/refman/5.7/en/innochecksum.html)

离线Innodb 文件校验和工具

See [Section 4.6.1, “**innochecksum** — Offline InnoDB File Checksum Utility”](https://dev.mysql.com/doc/refman/5.7/en/innochecksum.html).

## [**myisam_ftdump**](https://dev.mysql.com/doc/refman/5.7/en/myisam-ftdump.html)

查看 myisam 全文索引 信息的工具

## [**myisamchk**](https://dev.mysql.com/doc/refman/5.7/en/myisamchk.html)

描述,检查,优化 修复MyISAM 表

 See [Section 4.6.3, “**myisamchk** — MyISAM Table-Maintenance Utility”](https://dev.mysql.com/doc/refman/5.7/en/myisamchk.html).



## [**myisamlog**](https://dev.mysql.com/doc/refman/5.7/en/myisamlog.html)

处理myisam 日志文件内容

See [Section 4.6.4, “**myisamlog** — Display MyISAM Log File Contents”](https://dev.mysql.com/doc/refman/5.7/en/myisamlog.html).

## [**myisampack**](https://dev.mysql.com/doc/refman/5.7/en/myisampack.html)

A utility that compresses `MyISAM` tables to produce smaller read-only tables. See [Section 4.6.5, “**myisampack** — Generate Compressed, Read-Only MyISAM Tables”](https://dev.mysql.com/doc/refman/5.7/en/myisampack.html).

## [**mysql_config_editor**](https://dev.mysql.com/doc/refman/5.7/en/mysql-config-editor.html)

A utility that enables you to store authentication credentials in a secure, encrypted login path 

file named `.mylogin.cnf`. See [Section 4.6.6, “**mysql_config_editor** — MySQL Configuration Utility”](https://dev.mysql.com/doc/refman/5.7/en/mysql-config-editor.html).

## [**mysqlbinlog**](https://dev.mysql.com/doc/refman/5.7/en/mysqlbinlog.html)

A utility for reading statements from a binary log. The log of executed statements contained in the binary log files can be used to help recover from a crash. 

See [Section 4.6.7, “**mysqlbinlog** — Utility for Processing Binary Log Files”](https://dev.mysql.com/doc/refman/5.7/en/mysqlbinlog.html).

## [**mysqldumpslow**](https://dev.mysql.com/doc/refman/5.7/en/mysqldumpslow.html)

慢sql日志

See [Section 4.6.8, “**mysqldumpslow** — Summarize Slow Query Log Files”](https://dev.mysql.com/doc/refman/5.7/en/mysqldumpslow.html).





mysql客户端 服务器通信时 使用如下环境变量



| Environment Variable | Meaning                                                      |
| :------------------- | :----------------------------------------------------------- |
| `MYSQL_UNIX_PORT`    | The default Unix socket file; used for connections to `localhost` |
| `MYSQL_TCP_PORT`     | The default port number; used for TCP/IP connections         |
| `MYSQL_PWD`          | The default password                                         |
| `MYSQL_DEBUG`        | Debug trace options when debugging                           |
| `TMPDIR`             | The directory where temporary tables and files are created   |

For a full list of environment variables used by MySQL programs, see [Section 4.9, “Environment Variables”](https://dev.mysql.com/doc/refman/5.7/en/environment-variables.html).

Use of `MYSQL_PWD` is insecure. See [Section 6.1.2.1, “End-User Guidelines for Password Security”](https://dev.mysql.com/doc/refman/5.7/en/password-security-user.html).



