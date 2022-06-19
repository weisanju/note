配置环境变量

在mysql安装路径下 建 mysql.ini,与 data目录

```dart
[mysql]

# 设置mysql客户端默认字符集
default-character-set=utf8 

[mysqld]

#设置3306端口
port = 3306 

# 设置mysql的安装目录
basedir=F:\mysql\mysql-5.7.24-winx64\mysql-5.7.24-winx64

# 设置mysql数据库的数据的存放目录
datadir=F:\mysql\mysql-5.7.24-winx64\mysql-5.7.24-winx64\data

# 允许最大连接数
max_connections=200

# 服务端使用的字符集默认为8比特编码的latin1字符集
character-set-server=utf8

# 创建新表时将使用的默认存储引擎
default-storage-engine=INNODB
```



初始化mysql服务

```
mysqld --initialize-insecure --user=mysql
mysqld install
net start mysql
```

登录mysql修改密码

```
mysql -u root -p
mysqladmin -u root password
```

