# MongoDB 连接

**格式**

```js
//username:password@hostname/dbname
./mongo mongodb://admin:123456@localhost/test
```

## 连接实例

**连接本地数据库服务器，端口是默认的。**

```
mongodb://localhost
```

**使用用户名fred，密码foobar登录localhost的admin数据库。**

```
mongodb://fred:foobar@localhost
```

**使用用户名fred，密码foobar登录localhost的baz数据库。**

```
mongodb://fred:foobar@localhost/baz
```

**连接 replica pair, 服务器1为example1.com服务器2为example2。**

```
mongodb://example1.com:27017,example2.com:27017
```

**连接 replica set 三台服务器 (端口 27017, 27018, 和27019):**

```
mongodb://localhost,localhost:27018,localhost:27019
```

**连接 replica set 三台服务器, 写入操作应用在主服务器 并且分布查询到从服务器。**

```
mongodb://host1,host2,host3/?slaveOk=true
```

**直接连接第一个服务器，无论是replica set一部分或者主服务器或者从服务器。**

```
mongodb://host1,host2,host3/?connect=direct;slaveOk=true
```

当你的连接服务器有优先级，还需要列出所有服务器，你可以使用上述连接方式。

**安全模式连接到localhost:**

```
mongodb://localhost/?safe=true
```

**以安全模式连接到replica set，并且等待至少两个复制服务器成功写入，超时时间设置为2秒。**

```
mongodb://host1,host2,host3/?safe=true;w=2;wtimeoutMS=2000
```





## 标准格式

`mongodb://[username:password@]host1[:port1][,host2[:port2],...[,hostN[:portN]]][/[database][?options]]`

## 选项

| 选项                | 描述                                                         |
| ------------------- | ------------------------------------------------------------ |
| replicaSet=name     | 验证replica set的名称。 Impliesconnect=replicaSet.           |
| slaveOk=true\|false | true:在connect=direct模式下，驱动会连接第一台机器，即使这台服务器不是主。在connect=replicaSet模式下，驱动会发送所有的写请求到主并且把读取操作分布在其他从服务器。false: 在 connect=direct模式下，驱动会自动找寻主服务器. 在connect=replicaSet 模式下，驱动仅仅连接主服务器，并且所有的读写命令都连接到主服务器。 |
| safe=true\|false    | true: 在执行更新操作之后，驱动都会发送getLastError命令来确保更新成功。(还要参考 wtimeoutMS).false: 在每次更新之后，驱动不会发送getLastError来确保更新成功。 |
| w=n                 | 驱动添加 { w : n } 到getLastError命令. 应用于safe=true。     |
| wtimeoutMS=ms       | 驱动添加 { wtimeout : ms } 到 getlasterror 命令. 应用于 safe=true. |
| fsync=true\|false   | true: 驱动添加 { fsync : true } 到 getlasterror 命令.应用于 safe=true.false: 驱动不会添加到getLastError命令中。 |
| journal=true\|false | 如果设置为 true, 同步到 journal (在提交到数据库前写入到实体中). 应用于 safe=true |
| connectTimeoutMS=ms | 可以打开连接的时间。                                         |
| socketTimeoutMS=ms  | 发送和接受sockets的时间。                                    |

# MongoDB数据库创建与删除



## 创建数据库

MongoDB 创建数据库的语法格式如下：

```
use DATABASE_NAME
```

如果数据库不存在，则创建数据库，否则切换到指定数据库。



### 实例

以下实例我们创建了数据库 php:

```
> use php
switched to db php
> db
php
>
```

如果你想查看所有数据库，可以使用 **show dbs** 命令：

```
> show dbs
local  0.078GB
test   0.078GB
>
```

可以看到，我们刚创建的数据库 php 并不在数据库的列表中， 要显示它，我们需要向 php 数据库插入一些数据。

```
> db.php.insert({"name":"php中文网"})
WriteResult({ "nInserted" : 1 })
> show dbs
local   0.078GB
php  0.078GB
test    0.078GB
>
```

## 删除数据库

### 语法

MongoDB 删除数据库的语法格式如下：

```
db.dropDatabase()
```

删除当前数据库，默认为 test，你可以使用 db 命令查看当前数据库名。





# 文档管理

本章节中我们将向大家介绍如何将数据插入到MongoDB的集合中。

文档的数据结构和JSON基本一样。

所有存储在集合中的数据都是BSON格式。

BSON是一种类json的一种二进制形式的存储格式,简称Binary JSON。



## 插入文档

MongoDB 使用 insert() 或 save() 方法向集合中插入文档，语法如下：

```
db.COLLECTION_NAME.insert(document)
```

### 实例

以下文档可以存储在 MongoDB 的 php 数据库 的 col集合中：

```
>db.col.insert({title: 'MongoDB 教程', 
    description: 'MongoDB 是一个 Nosql 数据库',
    by: 'php中文网',
    url: 'http://www.php.cn',
    tags: ['mongodb', 'database', 'NoSQL'],
    likes: 100
})
```

## 更新文档

MongoDB 使用 **update()** 和 **save()** 方法来更新集合中的文档。接下来让我们详细来看下两个函数的应用及其区别。

### update() 方法

update() 方法用于更新已存在的文档。语法格式如下：

```
db.collection.update(
   <query>,
   <update>,
   {
     upsert: <boolean>,
     multi: <boolean>,
     writeConcern: <document>
   }
)
```



**参数说明：**

- **query** : update的查询条件，类似sql update查询内where后面的。
- **update** : update的对象和一些更新的操作符（如$,$inc...）等，也可以理解为sql update查询内set后面的
- **upsert** : 可选，这个参数的意思是，如果不存在update的记录，是否插入objNew,true为插入，默认是false，不插入。
- **multi** : 可选，mongodb 默认是false,只更新找到的第一条记录，如果这个参数为true,就把按条件查出来多条记录全部更新。
- **writeConcern** :可选，抛出异常的级别。



**实例**

```js
db.col.update(
	//query
	{'title':'MongoDB 教程'},
	//updates
	{$set:{'title':'MongoDB'}}
	//options
	{multi:true}
)
```

### save() 方法

save() 方法通过传入的文档来替换已有文档。语法格式如下：

```js
db.collection.save(
   <document>,
   {
     writeConcern: <document>
   }
)
```

**参数说明：**

- **document** : 文档数据。
- **writeConcern** :可选，抛出异常的级别。



以下实例中我们替换了 _id  为 56064f89ade2f21f36b03136 的文档数据：

```js
>db.col.save({
	"_id" : ObjectId("56064f89ade2f21f36b03136"),
    "title" : "MongoDB",
    "description" : "MongoDB 是一个 Nosql 数据库",
    "by" : "php",
    "url" : "http://www.php.cn",
    "tags" : [
            "mongodb",
            "NoSQL"
    ],
    "likes" : 110
})
```



### 更多实例

```
只更新第一条记录：
db.col.update( { "count" : { $gt : 1 } } , { $set : { "test2" : "OK"} } );
全部更新：

db.col.update( { "count" : { $gt : 3 } } , { $set : { "test2" : "OK"} },false,true );
只添加第一条：

db.col.update( { "count" : { $gt : 4 } } , { $set : { "test5" : "OK"} },true,false );
全部添加加进去:

db.col.update( { "count" : { $gt : 5 } } , { $set : { "test5" : "OK"} },true,true );
全部更新：

db.col.update( { "count" : { $gt : 15 } } , { $inc : { "count" : 1} },false,true );
只更新第一条记录：

db.col.update( { "count" : { $gt : 10 } } , { $inc : { "count" : 1} },false,false );
```

## 删除文档

remove() 方法的基本语法格式如下所示：

```js
db.collection.remove(
   <query>,
   <justOne>
)
```

如果你的 MongoDB 是 2.6 版本以后的，语法格式如下：

```js
db.collection.remove(
   <query>,
   {
     justOne: <boolean>,
     writeConcern: <document>
   }
)
```

**参数说明：**

- **query** :（可选）删除的文档的条件。
- **justOne** : （可选）如果设为 true 或 1，则只删除一个文档。
- **writeConcern** :（可选）抛出异常的级别。





## 查询文档

### MongoDB 与 RDBMS Where 语句比较

如果你熟悉常规的 SQL 数据，通过下表可以更好的理解 MongoDB 的条件语句查询：

| 操作       | 格式                     | 范例                                        | RDBMS中的类似语句        |
| ---------- | ------------------------ | ------------------------------------------- | ------------------------ |
| 等于       | `{<key>:<value>`}        | `db.col.find({"by":"php中文网"}).pretty()`  | `where by = 'php中文网'` |
| 小于       | `{<key>:{$lt:<value>}}`  | `db.col.find({"likes":{$lt:50}}).pretty()`  | `where likes < 50`       |
| 小于或等于 | `{<key>:{$lte:<value>}}` | `db.col.find({"likes":{$lte:50}}).pretty()` | `where likes <= 50`      |
| 大于       | `{<key>:{$gt:<value>}}`  | `db.col.find({"likes":{$gt:50}}).pretty()`  | `where likes > 50`       |
| 大于或等于 | `{<key>:{$gte:<value>}}` | `db.col.find({"likes":{$gte:50}}).pretty()` | `where likes >= 50`      |
| 不等于     | `{<key>:{$ne:<value>}}`  | `db.col.find({"likes":{$ne:50}}).pretty()`  | `where likes != 50`      |



### find

**findOne用于查一个**

MongoDB 查询数据的语法格式如下：

```
>db.COLLECTION_NAME.find({query})
```

**find() 方法以非结构化的方式来显示所有文档。**

如果你需要以易读的方式来读取数据，可以使用 pretty() 方法，语法格式如下：

```
>db.col.find().pretty()
```

### MongoDB AND 条件

```
>db.col.find({key1:value1, key2:value2}).pretty()
```

### MongoDB OR 条件

```js
>db.col.find(
   {
      $or: [
	     {key1: value1}, {key2:value2}
      ]
   }
).pretty()

```

### AND 和 OR 联合使用

**'where likes>50 AND (by = 'php中文网' OR title = 'MongoDB 教程')'**

```
db.col.find({"likes": {$gt:50}, $or: [{"by": "php中文网"},{"title": "MongoDB 教程"}]}).pretty()
```



查看已插入文档：

```js
> db.col.find()
{ "_id" : ObjectId("56064886ade2f21f36b03134"), "title" : "MongoDB 教程", "description" : "MongoDB 是一个 Nosql 数据库", "by" : "php中文网", "url" : "http://www.php.cn", "tags" : [ "mongodb", "database", "NoSQL" ], "likes" : 100 }
>
```

**定义变量**

我们也可以将数据定义为一个变量，如下所示：

```js
document=({title: 'MongoDB 教程', 
    description: 'MongoDB 是一个 Nosql 数据库',
    by: 'php中文网',
    url: 'http://www.php.cn',
    tags: ['mongodb', 'database', 'NoSQL'],
    likes: 100
});

db.col.insert(document)
```

插入文档你也可以使用 db.col.save(document) 命令。如果不指定 _id 字段 save() 方法类似于 insert() 方法。如果指定 _id 字段，则会更新该 _id 的数据。



# 其他关键字

## $type 操作符

MongoDB 中可以使用的类型如下表所示：

| **类型**                | **数字** | **备注**         |
| ----------------------- | -------- | ---------------- |
| Double                  | 1        |                  |
| String                  | 2        |                  |
| Object                  | 3        |                  |
| Array                   | 4        |                  |
| Binary data             | 5        |                  |
| Undefined               | 6        | 已废弃。         |
| Object id               | 7        |                  |
| Boolean                 | 8        |                  |
| Date                    | 9        |                  |
| Null                    | 10       |                  |
| Regular Expression      | 11       |                  |
| JavaScript              | 13       |                  |
| Symbol                  | 14       |                  |
| JavaScript (with scope) | 15       |                  |
| 32-bit integer          | 16       |                  |
| Timestamp               | 17       |                  |
| 64-bit integer          | 18       |                  |
| Min key                 | 255      | Query with `-1`. |
| Max key                 | 127      |                  |



$type操作符是基于BSON类型来检索集合中匹配的数据类型，并返回结果。

如果想获取 "col" 集合中 title 为 String 的数据，你可以使用以下命令：

```
db.col.find({"title" : {$type : 2}})
```



## Limit()

如果你需要在MongoDB中读取指定数量的数据记录，可以使用MongoDB的Limit方法，limit()方法接受一个数字参数，该参数指定从MongoDB中读取的记录条数。

```
>db.COLLECTION_NAME.find().limit(NUMBER)
```



## Skip() 

我们除了可以使用limit()方法来读取指定数量的数据外，还可以使用skip()方法来跳过指定数量的数据，skip方法同样接受一个数字参数作为跳过的记录条数。

```
>db.COLLECTION_NAME.find().limit(NUMBER).skip(NUMBER)
```

## sort()

sort()方法可以通过参数指定排序的字段，并使用 1 和 -1 来指定排序的方式，其中 1 为升序排列，而-1是用于降序排列。

```
db.COLLECTION_NAME.find().sort({KEY:1})
```