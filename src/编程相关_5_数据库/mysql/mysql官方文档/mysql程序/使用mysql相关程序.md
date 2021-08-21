## 选项参数与非选项参数

**选项参数**以 `- 或者 --`   开始的命令选项

**非选项参数** 提供额外信息给命令行程序

例如 mysql命令 的第一个非选项 参数 为数据库名

使用最多的选项 **连接参数**

`--host -h`

`--user -u`

`--password -p`

`--port -P`

`--socket -S` 指定 UnixSocketFile 或者windows上的具名管道



## 指定选项参数的 几种方式

### 命令名称后面 跟随

*  **后面的覆盖前面的**

```
mysql --column-names --skip-column-names
```

* `--` 为选项全拼 `-` 为选项缩写

* 缩写没有 等于号,选项名 与选项值 直接可以有 空格

  ```
  -h localhost or --host=localhost
  ```

* 大小写敏感

* `- _` 一致

  ``` 
  --skip-grant-tables and --skip_grant_tables是一样的
  ```

* 数值类型可以 有 `K M G`

```
mysqladmin --count=1K --sleep=10 ping
ping服务器 1024次 每次等10s
```

* 带有空格的 选项值 带引号

```
shell> mysql -u root -p -e "SELECT VERSION();SELECT NOW()"
Enter password: ******
+------------+
| VERSION()  |
+------------+
| 5.7.29     |
+------------+
+---------------------+
| NOW()               |
+---------------------+
| 2019-09-03 10:36:28 |
+---------------------+
shell>
```



### 选项文件

忽略空行

头尾空格自动去除

非空行有以下几种形式

* 注释 #*`comment`*`, `;*`comment`*

  Comment lines start with `#` or `;`. A `#` comment can start in the middle of a line as well.

* [*`group`*]

  组号,或程序的 名称, 后面跟着的是 应用与该程序的 选项

  * the `[mysqld]` and `[mysql]` groups apply to the [**mysqld**](https://dev.mysql.com/doc/refman/5.7/en/mysqld.html) server and the [**mysql**](https://dev.mysql.com/doc/refman/5.7/en/mysql.html) client program, respectively.

  * The `[client]` option group is read by all client programs provided in MySQL distributions (but *not* by [**mysqld**](https://dev.mysql.com/doc/refman/5.7/en/mysqld.html)). 

    To understand how third-party client programs that use the C API can use option files, see the C API documentation at [mysql_options()](https://dev.mysql.com/doc/c-api/5.7/en/mysql-options.html).

  * `[mysqldump]` enables [**mysqldump**](https://dev.mysql.com/doc/refman/5.7/en/mysqldump.html)-specific options to override `[client]` options. 可以覆盖前面的 client指定的同名选项

  * 指定版本

    ```
    [mysqld-5.7]
    sql_mode=TRADITIONAL
    ```

  * 包含其他 选项文件

    ```
    !include /home/mydir/myopt.cnf
    
    查找目录
    !includedir /home/mydir, 查找任何以 .cnf .ini
    Any files to be found and included using the !includedir directive on Unix operating systems must have file names ending in .cnf. On Windows, this directive checks for files with the .ini or .cnf extension.
    ```

* `opt_name`*=*`value`

  选项名, 相比命令行 选项 去掉 短横杠

* 转义处理

  ```
  \b, \t, \n, \r, \\ \s
  backspace, tab, newline, carriage return, backslash, and space characters.
  windows路径目录特殊处理
  
  
  basedir="C:\Program Files\MySQL\MySQL Server 5.7"
  basedir="C:\\Program Files\\MySQL\\MySQL Server 5.7"
  basedir="C:/Program Files/MySQL/MySQL Server 5.7"
  basedir=C:\\Program\sFiles\\MySQL\\MySQL\sServer\s5.7
  ```

* 影响 选项文件处理 的命令行参数

  * `--print-defaults`

    打印所有从 文件中读取的选项,密码会被掩盖

  * `--no-defaults`

    不要从文件中读取选项

    有一个例外是 客户端程序 从  .mylogin.cnf  文件中读取选项  不受影响

    `.mylogin.cnf` is created by the [**mysql_config_editor**](https://dev.mysql.com/doc/refman/5.7/en/mysql-config-editor.html) 

  * `--login-path=name`

    从 loginFile中读取 选项 , 其中的选项 涉及 登录信息,  mysql_config_editor 工具来编辑此文件

    ```
    mysql --login-path=mypath
    默认读取 [client] [mysql] 组
    还有  [mypath] 组
    ```

    指定备用登录文件组名

    ```
    MYSQL_TEST_LOGIN_FILE 环境变量
    ```

  * `--defaults-group-suffix=str`

    ```
    指定其他组名前缀读取
    [client] and [mysql]
    --defaults-group-suffix=_other
    mysql also reads the [client_other] and [mysql_other] groups.
    ```

  * `--defaults-file=file_name`

    给定 指定的 选项文件读取

    同样不会影响  `.mylogin.cnf`

  * `--defaults-extra-file=file_name`

    全局选项文件 读取后 , 用户选项文件读取前,  .mylogin.cnf 读取前

* 程序选项 修饰符

  在查询时 不输出字段名

  ```
  --disable-column-names 
  --skip-column-names 
  --column-names=0
  ```

  ```
  --column-names
  --enable-column-names
  --column-names=1
  ```

  使用 `--loose` 前缀 时 如果不存在该选项 会警告 而不报错退出

  ```
  shell> mysql --loose-no-such-option
  mysql: WARNING: unknown option '--loose-no-such-option'
  ```

   `--maximum` 设置 session级别 变量 的最大值

  ```
   --maximum-max_heap_table_size=32M  最大表堆的大小
  ```

* 在执行时可以使用表达式. 在启动时使用标识符 

  ```
   mysql --max_allowed_packet=16M
   SET GLOBAL max_allowed_packet=16*1024*1024;
  ```

**建立连接的命令行选项**

|                                                              |                                                              |            |
| :----------------------------------------------------------- | :----------------------------------------------------------- | :--------- |
| Option Name                                                  | Description                                                  | Deprecated |
| [--default-auth](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_default-auth) | 客户端认证所使用的插件,See [Section 6.2.13, “Pluggable Authentication”](https://dev.mysql.com/doc/refman/5.7/en/pluggable-authentication.html). |            |
| [--host](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_host) | 主机名或者IPV4 地址                                          |            |
| [--password](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_password) | 密码,使用命令行输入密码不安全, See [Section 6.1.2.1, “End-User Guidelines for Password Security”](https://dev.mysql.com/doc/refman/5.7/en/password-security-user.html). |            |
| [--pipe](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_pipe) | Connect to server using named pipe (Windows only)<br />This option applies only if the server was started with the [`named_pipe`](https://dev.mysql.com/doc/refman/5.7/en/server-system-variables.html#sysvar_named_pipe) system variable enabled to support named-pipe connections. In addition, the user making the connection must be a member of the Windows group specified by the [`named_pipe_full_access_group`](https://dev.mysql.com/doc/refman/5.7/en/server-system-variables.html#sysvar_named_pipe_full_access_group) system variable. |            |
| [--plugin-dir](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_plugin-dir) | 寻找插件的目录,See [Section 6.2.13, “Pluggable Authentication”](https://dev.mysql.com/doc/refman/5.7/en/pluggable-authentication.html). |            |
| [--port](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_port) | TCP/IP port number for connection                            |            |
| [--protocol](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_protocol) | `TCP|SOCKET|PIPE|MEMORY`                                     |            |
| [--secure-auth](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_secure-auth) | Do not send passwords to server in old (pre-4.1) format<br />[Section 6.4.1.3, “Migrating Away from Pre-4.1 Password Hashing and the mysql_old_password Plugin”](https://dev.mysql.com/doc/refman/5.7/en/account-upgrades.html). | Yes        |
| [--shared-memory-base-name](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_shared-memory-base-name) | On Windows, the shared-memory name to use for connections made using shared memory to a local server. The default value is `MYSQL`. The shared-memory name is case-sensitive.<br />This option applies only if the server was started with the [`shared_memory`](https://dev.mysql.com/doc/refman/5.7/en/server-system-variables.html#sysvar_shared_memory) system variable enabled to support shared-memory connections. |            |
| [--socket](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_socket) | Unix socket file 用来做本地连接,默认名是 /tmp/mysql.sock<br />在Windows是命名管道,默认名是MySQL<br />On Windows, this option applies only if the server was started with the named_pipe system variable enabled to support named-pipe connections. In addition, the user making the connection must be a member of the Windows group specified by the named_pipe_full_access_group system variable. |            |
| [--user](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_user) | MySQL user name to use when connecting to server             |            |

**协议介绍**

| [`--protocol`](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_protocol) Value | Transport Protocol Used                    | Applicable Platforms       |
| :----------------------------------------------------------- | :----------------------------------------- | :------------------------- |
| `TCP`                                                        | TCP/IP transport to local or remote server | All                        |
| `SOCKET`                                                     | Unix socket-file transport to local server | Unix and Unix-like systems |
| `PIPE`                                                       | Named-pipe transport to local server       | Windows                    |
| `MEMORY`                                                     | Shared-memory transport to local server    | Windows                    |



**加密连接选项**

| Option Name                                                  | Description                                                  | Introduced |
| :----------------------------------------------------------- | :----------------------------------------------------------- | :--------- |
| [--get-server-public-key](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_get-server-public-key) | Request RSA public key from server                           | 5.7.23     |
| [--server-public-key-path](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_server-public-key-path) | Path name to file containing RSA public key                  |            |
| [--skip-ssl](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_ssl) | Disable connection encryption                                |            |
| [--ssl](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_ssl) | Enable connection encryption                                 |            |
| [--ssl-ca](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_ssl-ca) | File that contains list of trusted SSL Certificate Authorities |            |
| [--ssl-capath](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_ssl-capath) | Directory that contains trusted SSL Certificate Authority certificate files |            |
| [--ssl-cert](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_ssl-cert) | File that contains X.509 certificate                         |            |
| [--ssl-cipher](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_ssl-cipher) | Permissible ciphers for connection encryption                |            |
| [--ssl-crl](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_ssl-crl) | File that contains certificate revocation lists              |            |
| [--ssl-crlpath](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_ssl-crlpath) | Directory that contains certificate revocation-list files    |            |
| [--ssl-key](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_ssl-key) | File that contains X.509 key                                 |            |
| [--ssl-mode](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_ssl-mode) | Desired security state of connection to server               | 5.7.11     |
| [--ssl-verify-server-cert](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_ssl-verify-server-cert) | Verify host name against server certificate Common Name identity |            |
| [--tls-version](https://dev.mysql.com/doc/refman/5.7/en/connection-options.html#option_general_tls-version) | Permissible TLS protocols for encrypted connections          | 5.7.10     |

**--ssl-mode=mode**

* disable

  没有加密的连接

  ```
   --ssl=0 option or its synonyms (--skip-ssl, --disable-ssl).
  ```

* PREFERRED

  尝试建立加密连接,如果不能建立 则建立非加密连接

* REQUIRED

  需要建立加密连接,否则无法建立连接

* VERIFY_CA

### 读环境变量

