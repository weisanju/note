# 



# 优化策略

- 出现了 Using temporary；
- rows 过多，或者几乎是全表的记录数；
- filtered 太低
- key 是 (NULL)；
- possible_keys 出现过多（待选）索引。

# explain

## **ID**

> 表示表的读取顺序

- id 相同的情况下:执行顺序由上至下

- id 不同的情况下: id 越大,越先被执行( 例如最里面的越先被执行)

## select_type

> 数据是以何种方式读取的

**simple**

简单的子查询,不包含子查询或者*union*

**primary**

查询中若包含多层子查询,最外层的查询为*primary*查询

**subquery**

子查询

**derived**

- 子查询衍生的虚表的表名格式为 derived\${id},
- 其中 id 为 explan 表 的 id,表示由这个 id 代表的某步骤产生的临时表
- 用于 from 子句里有子查询的情况。MySQL 会递归执行这些子查询，把结果放在临时表里

**union**

- 若第二个 select 出现在 union 之后,则被标记为 union
- 若 union 包含在 from 子句的 子查询中,则外层的 select 将被标记为 drived

**union result**

- 从 union 表获取结果的 select

## table

> 表名

## type

> 实际索引使用方式,性能升序增加

**ALL**

> 全表扫描

**index** （带索引的全表扫描）

这种连接类型只是另外一种形式的全表扫描，只不过它的扫描顺序是按照索引的顺序。这种扫描根据索引然后回表取数据，和 all 相比，他们都是取得了全表的数据，而且 index 要先读索引而且要回表随机取数据，**因此 index 不可能会比 all 快**（取同一个表数据），但为什么官方的手册将它的效率说的比 all 好，唯一可能的场景在于，按照索引扫描全表的数据是有序的。这样一来使用该索引排序 时 比 ALL 要好

```m
mysql> explain select * from employee order by `no` ;
+----+-------------+----------+------+---------------+------+---------+------+------+----------------+
| id | select_type | table    | type | possible_keys | key  | key_len | ref  | rows | Extra          |
+----+-------------+----------+------+---------------+------+---------+------+------+----------------+
|  1 | SIMPLE      | employee | ALL  | NULL          | NULL | NULL    | NULL |    5 | Using filesort |
+----+-------------+----------+------+---------------+------+---------+------+------+----------------+
mysql> explain select * from employee order by rec_id ;
+----+-------------+----------+-------+---------------+---------+---------+------+------+-------+
| id | select_type | table    | type  | possible_keys | key     | key_len | ref  | rows | Extra |
+----+-------------+----------+-------+---------------+---------+---------+------+------+-------+
|  1 | SIMPLE      | employee | index | NULL          | PRIMARY | 4       | NULL |    5 | NULL  |
+----+-------------+----------+-------+---------------+---------+---------+------+------+-------+
```

**range**

- 只检索给定范围的行,使用一个索引来选择行,索引开始于某一点,结束于某一点
- 例如 between in,> < , in, or 也是索引扫描

**ref**

- 非唯一性索引扫描,返回匹配某个单独值得所有行

**eq_ref**

- 唯一性索引扫描,对于每个索引键 只有一条记录与之匹配,常见于主键或唯一扫描

**const**

- 只有一条记录匹配,通常常见于 主键和唯一性索引

**system**

表只有一行记录,等于系统表,是 const 类型的特例

```
system>const>eq_ref>ref>fulltext>ref_or_null>index_merge>unique_subquery>index_subquery>rang>index>all
保证达到 range级别 即可,最好能达到ref
```

## possibleKeys

> 可能用到的索引,因为一张表索引只能使用一个

## keys

> 实际使用到的索引

- 覆盖索引

  查询的字段 刚好建立了索引,且查询顺序一致

## key_len

- 表示索引中使用的字节数,可通过该计算查询中使用的索引的长度,长度越短越好
- 根据表定义计算得出得 索引字节数

## ref

> 显示该次查询 使用的索引的值 是 引用得哪个地方的, 一般有两种引用

- const 引用的常量
- test.t1.id 引用 某个库的某个表的某个字段 作为索引值的来源

## rows

- 找到记录大致要读取的行数

## extra

> 表示不适合在其他列中显示,但十分重要的额外信息

- _using filesort_ 没有用到索引的排序,因为联合索引 字段顺序问题

  ```
  example
  index(col1,col2,col3)
  col1='ac' order by col2,col3
  这种情况是会用到索引
  如何建索引 , 就 如何按照索引走
  ```

- _using temporary_:使用了临时表,保存中间结果,常见于 order by,group by

  - 对于联合索引 请按照建立的顺序使用

- _using index_

  - select 操作中使用了覆盖索引,效率不错
  - 如果同时出现了*usingwhere* 表明索引被用来 执行索引键值的查找
  - 如果没有出现 _using where_ 表明索引用来读取数据而非执行查找

- _using where_

  - 使用*where*过滤

- _using join buffer_

  - 使用了连接缓存

- _impossiable where_

  - 不可能的 where 过滤条件

- _select table optimized away_

  - 在没有 groupby 子句的情况下,基于索引优化 MIN/MAX 操作
  - 或 对于 MyISAM 存储引擎 优化 count(\*) 操作,不必等到执行阶段计算

- _distinct_

# Using temporary/Using filesort

## **Using temporary**

> 表示由于排序没有走索引、使用`union`、子查询连接查询、使用某些视图等原因（详见[internal-temporary-tables](https://dev.mysql.com/doc/refman/5.6/en/internal-temporary-tables.html)），因此创建了一个内部临时表。
>
> 注意这里的临时表**可能是内存上的临时表**，也有可能是**硬盘上的临时表**

**内存临时表 or 硬盘临时表**

查看 sql 执行时使用的是内存临时表还是硬盘临时表，需要使用如下命令：

```
mysql> show global status like '%tmp%';
+-------------------------+-------+
| Variable_name           | Value |
+-------------------------+-------+
| Created_tmp_disk_tables | 0     |
| Created_tmp_files       | 5     |
| Created_tmp_tables      | 11    |
+-------------------------+-------+
3 rows in set
```

[Created_tmp_tables](https://dev.mysql.com/doc/refman/5.6/en/server-status-variables.html#statvar_Created_tmp_tables) 表示 mysql 创建的内部临时表的总数（包括内存临时表和硬盘临时表）；

[Created_tmp_disk_tables](https://dev.mysql.com/doc/refman/5.6/en/server-status-variables.html#statvar_Created_tmp_disk_tables) 表示 mysql 创建的硬盘临时表的总数。

**与临时表有关的参数**

当 mysql 需要创建临时表时，选择内存临时表还是硬盘临时表取决于参数[tmp_table_size](https://dev.mysql.com/doc/refman/5.6/en/server-system-variables.html#sysvar_tmp_table_size)和[max_heap_table_size](https://dev.mysql.com/doc/refman/5.6/en/server-system-variables.html#sysvar_max_heap_table_size)，

当`临时表的容量 > Min(tmp_table_size ,max_heap_table_size )` , mysql 就会使用硬盘临时表存放数据。

用户可以在 mysql 的配置文件里修改该两个参数的值，两者的默认值均为 16M。

```
tmp_table_size = 16M
max_heap_table_size = 16M
12
```

查看`tmp_table_size`和`max_heap_table_size`值：

```sql
mysql> show global variables like 'max_heap_table_size' or 'tmp_table_size';
```

## Using filesort

> `Using filesort`仅仅表示没有使用索引的排序,`filesort`与文件无关。消除`Using filesort`的方法就是让查询 sql 的排序走索引

**简介**

`filesort`使用的算法是`QuickSort`，即对需要排序的记录生成元数据进行**分块排序**，然后再使用 mergesort 方法**合并块**。其中`filesort`可以使用的内存空间大小为参数[sort_buffer_size](https://dev.mysql.com/doc/refman/5.5/en/server-system-variables.html#sysvar_sort_buffer_size)的值，默认为 2M。当排序记录太多`sort_buffer_size`不够用时，mysql 会**使用临时文件来存放各个分块**，然后各个分块排序后再多次合并分块最终全局完成排序。

```
mysql> show global variables like 'sort_buffer_size';
+------------------+--------+
| Variable_name    | Value  |
+------------------+--------+
| sort_buffer_size | 262144 |
+------------------+--------+
1 row in set
```

[Sort_merge_passes](https://dev.mysql.com/doc/refman/5.6/en/server-status-variables.html#statvar_Sort_merge_passes)表示`filesort`执行过的文件分块合并次数的总和，如果该值比较大，建议增大`sort_buffer_size`的值。

**使用算法**

`filesort`使用的排序方法有两种：

**rowid 回表排序**

第一种方法是对需要排序的记录生成`<sort_key,rowid>`的元数据进行排序，该元数据仅包含排序字段和 rowid。排序完成后只有按字段排序的 rowid，因此还需要通过 rowid 进行回表操作获取所需要的列的值，可能会导致大量的随机 IO 读消耗；

**带元数据排序**

第二种方法是是对需要排序的记录生成`<sort_key,additional_fields>`的元数据，该元数据包含排序字段和需要返回的所有列。排序完后不需要回表，但是元数据要比第一种方法长得多，需要更多的空间用于排序。

参数[max_length_for_sort_data](https://dev.mysql.com/doc/refman/5.6/en/server-system-variables.html#sysvar_max_length_for_sort_data)字段用于控制`filesort`使用的排序方法，当所有需要排序记录的字段数量总和小于`max_length_for_sort_data`时使用第二种算法，否则会用第一种算法。该值的默认值为 1024

# 小表驱动大表

## 重要知识点

- **EXPLAIN 结果中，第一行出现的表就是驱动表**
- **对驱动表可以直接排序**，**对非驱动表（的字段排序）需要对循环查询的合并结果（临时表）进行排序\*\***（Important!）\*\*
- **永远用小结果集驱动大结果集(mysql 中)**

## Nested Loop Join

以驱动表的结果集作为循环的基础数据，然后将结果集中的数据作为过滤条件一条条地到下一个表中查询数据，最后合并结果；此时还有第三个表，则将前两个表的 Join 结果集作为循环基础数据，再一次通过循环查询条件到第三个表中查询数据，如此反复。

## 驱动表的定义

进行多表连接查询时， **[驱动表]** 的定义为：
1）指定了联接条件时，**满足查询条件的记录行数少**的表为[驱动表]；

2）未指定联接条件时，**行数少**的表为[驱动表]

3.  left join 一定程度上会 将 左表设置为 [驱动表] (除非 右表的查询足够小)

# KeyLen 的计算

key_len 表示索引使用的字节数，根据这个值可以判断索引的使用情况,**特别是在组合索引的时候**,判断该索引有多少部分被使用到
在计算 key_len 时，下面是一些需要考虑的点:

- **索引字段的附加信息:**可以分为变长和定长数据类型讨论
  - 当索引字段为定长数据类型时,如 char，int，datetime,需要有是否为空的标记,这个标记占用 1 个字节(对于 not null 的字段来说,则不需要这 1 字节);
  - 对于变长数据类型,比如 varchar,除了是否为空的标记外,还需要有长度信息,需要占用两个字节。
- 对于,char、varchar、blob、text 等字符集来说，key len 的长度还和字符集有关
  - latin1 一个字符占用 1 个字节
  - gbk 一个字符占用 2 个字节
  - utf8 一个字符占用 3 个字节。

综上，下面来看一些例子:

| 列类型                         | KEY_LEN           | 备注                                                               |
| ------------------------------ | ----------------- | ------------------------------------------------------------------ |
| id int                         | key_len = 4+1     | int 为 4bytes,允许为 NULL,加 1byte                                 |
| id bigint not null             | key_len=8         | bigint 为 8bytes                                                   |
| user char(30) utf8             | key_len=30\*3+1   | utf8 每个字符为 3bytes,允许为 NULL,加 1byte                        |
| user varchar(30) not null utf8 | key_len=30\*3+2   | utf8 每个字符为 3bytes,变长数据类型,加 2bytes                      |
| user varchar(30) utf8          | key_len=30\*3+2+1 | utf8 每个字符为 3bytes,允许为 NULL,加 1byte,变长数据类型,加 2bytes |
| detail text(10) utf8           | key_len=30\*3+2+1 | TEXT 截取部分,被视为动态列类型。                                   |

key_len 只指示了**where 中用于条件过滤时被选中的索引列**，是不包含 order by/group by 这一部分被选中的索引列的
例如,有个联合索引 idx(c1,c2,c3),3 列均是 int not null,那么下面的 SQL 执行计划中

```sql
//key_len的值是8而不是12:
select ... from tb where c1=? and c2=? order by c1;
```

# indexMerge

> **对多个索引分别进行条件扫描，然后将它们各自的结果进行合并(intersect/union)** 同一个表的多个索引的范围扫描可以对结果进行合并

## **示例**

```sql
SELECT * FROM tbl_name WHERE key1 = 10 OR key2 = 20;
SELECT * FROM tbl_name WHERE (key1 = 10 OR key2 = 20) AND non_key=30;
SELECT * FROM t1, t2 WHERE (t1.key1 IN (1,2) OR t1.key2 LIKE 'value%') AND t2.key1=t1.some_col;
SELECT * FROM t1, t2 WHERE t1.key1=1 AND (t2.key1=t1.some_col OR t2.key2=t1.some_col2);
```

```
1,SIMPLE,a,,index_merge,"index_shift_results_worker_id,index_shift_results_org_code","index_shift_results_worker_id,index_shift_results_org_code","51,81",,61,100,"Using union(index_shift_results_worker_id,index_shift_results_org_code); Using where
```



## Using intersect(index_1,index_2...)

```
//取交集
SELECT * FROM tbl_name WHERE key1 = 10 and key2 = 20;
```



## **Using union(index_1,index_2)**

```
//取并集
SELECT * FROM tbl_name WHERE key1 = 10 or key2 = 20;
```

以及它们的组合(先内部 intersect 然后在外面 union)。

## Using sort_union(index_1,index_2)

```sql
select id,worker_id,org_code from shift_results a where worker_id between '10197' and '102000' or org_code between  '100101100' and '100201100'
```

两个结果集进行并集 运算 需要排序去重





# ref_or_null

https://dev.mysql.com/doc/refman/8.0/en/is-null-optimization.html

```sql
select id,worker_id,org_code from shift_results a where
worker_id = '10197' or worker_id is null
```

# mysql 子查询



## 子查询定义

**SUBQUERY**

子查询中的第一个 SELECT

**DEPENDENT SUBQUERY**

子查询中的第一个 SELECT，**取决于外面的查询** 。

```
换句话说，就是 子查询对 g2 的查询方式依赖于外层 g1 的查询。


第一步，MySQL 根据 select gid,count(id) from shop_goods where status=0 group by gid; 得到一个大结果集 t1，其数据量就是上图中的 rows=850672 了。

第二步，上面的大结果集 t1 中的每一条记录，都将与子查询 SQL 组成新的查询语句：select gid from shop_goods where sid in (15...blabla..29) and gid=%t1.gid%。等于说，子查询要执行85万次……即使这两步查询都用到了索引，但不慢才怪。

如此一来，子查询的执行效率居然受制于外层查询的记录数，那还不如拆成两个独立查询顺序执行呢。
```

**优化策略 **

子查询转临时表 做 join**关联**





# where条件分析

所有SQL的where条件，均可归纳为3大类

*Index Key (First Key & Last Key)*

*Index Filter*

*Table Filter*

对 where 中过滤条件的处理 ,根据索引使用情况分成了三种：index key, index filter, table filter

**index key**

用于确定SQL查询在索引中的连续范围(起始范围+结束范围)的查询条件，被称之为Index Key。由于一个范围，至少包含一个起始与一个终止，因此Index Key也被拆分为Index First Key和Index Last Key，分别用于定位索引查找的起始，以及索引查询的终止条件。也就是说根据索引来确定扫描的范围。

**index filter**

在使用 index key 确定了起始范围和介绍范围之后，在此范围之内，还有一些记录不符合where 条件，如果这些条件可以使用索引进行过滤，那么就是 index filter。也就是说用索引来进行where条件过滤。

**table filter**

where 中的条件不能使用索引进行处理的，只能访问table，进行条件过滤了。

### 什么是ICP？

即所索引条件下推（index condition pushdown）

它能减少在使用 二级索引 过滤where条件时的回表次数 和 减少MySQL server层和引擎层的交互次数。在索引组织表中，使用二级索引进行回表的代价相比堆表中是要高一些的。

也就是说各种各样的 where 条件，在进行处理时，分成了上面三种情况，一种条件会使用索引确定扫描的范围；一种条件可以在索引中进行过滤；一种必须回表进行过滤；

在 MySQL5.6 之前，并不区分Index Filter与Table Filter，统统将Index First Key与Index Last Key范围内的索引记录，回表读取完整记录，然后返回给MySQL Server层进行过滤。

而在MySQL 5.6之后，Index Filter与Table Filter分离，Index Filter下降到InnoDB的索引层面进行过滤，减少了回表与返回MySQL Server层的记录交互开销，提高了SQL的执行效率。

所以所谓的 ICP 技术，其实就是 index filter 技术而已。只不过因为MySQL的架构原因，分成了server层和引擎层，才有所谓的“下推”的说法。所以ICP其实就是实现了index filter技术，将原来的在server层进行的table filter中可以进行index filter的部分，在引擎层面使用index filter进行处理，不再需要回表进行table filter。



using index

using index;using where

using where

using index condition



## 示例

```sql
// 索引`INDEX(zipcode, lastname, firstname)`

SELECT * FROM people
WHERE zipcode='95054'
AND lastname LIKE '%etrunia%'
AND address LIKE '%Main Street%';
```

MySQL可以使用索引去定位那些`zipcode='95054'`的信息，但是第二个条件`lastname LIKE '%etrunia%'`却没法用于减少必须要扫描的表行，所以如果没有ICP优化，在执行此查询时必须读取所有`zipcode='95054'`的表行数据。

如果启用了ICP优化，因为MySQL使用了索引`INDEX(zipcode, lastname, firstname)`，并且WHERE语句中的第二部分`lastname LIKE '%etrunia%'`仅仅使用了索引中的列`lastname`，所以在读取完整表行数据前可以基于此过滤那些索引中`lastname`不符合条件的索引数据，这样就能避免对那些满足`zipcode='95054'`但是不满足条件`lastname LIKE '%etrunia%'`表行数据的访问。

ICP可以通过系统变量`optimizer_switch`中的`index_condition_pushdown`进行启用和关闭：

```sql
SET optimizer_switch = 'index_condition_pushdown=off';
SET optimizer_switch = 'index_condition_pushdown=on';
```











# 物化表

> MySQL 引入了`Materialization`（物化）这一关键特性用于子查询（比如在 IN/NOT IN 子查询以及 FROM 子查询）优化。

**具体方式是**

- 在 SQL 执行过程中，第一次需要子查询结果时执行子查询并将子查询的结果保存为临时表 ，后续对子查询结果集的访问将直接通过临时表获得。

- 与此同时，优化器还具有延迟物化子查询的能力，先通过其它条件判断子查询是否真的需要执行。
- 物化子查询优化 SQL 执行的关键点在于对子查询只需要执行一次。 与之相对的执行方式是对外表的每一行都对子查询进行调用，其执行计划中的查询类型为“DEPENDENT SUBQUERY”。





# 查询优化开关

```
show variables  like 'optimizer_switch'
```



