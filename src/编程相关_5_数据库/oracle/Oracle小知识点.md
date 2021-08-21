# rownum

## rownum 的 产生过程

1. rownum初始化为0
2. 取一条记录 判断是否满足where条件
3. 如果满足条件 则rownum+1  
4. 如果不满足条件 则rownum不变
5. 继续第二步骤

## 关于where条件后 rownum >n 分析

由于rownum 初始为 0,所以 当n为正整数时, 该条件一开始就不成立,rownum 也永远不会增加

# `Oracle`排序函数

## `row_number`

`row_number() over(partition by column1 order by column2) as pxh`



# `Oracle`分组函数

## grouping sets:单排列

`select a,b,sum(c) from table_d group by grouping sets(a,b)`

等价于

`select a,'',sum(c) from table_d  group by a`

`union all`

`select  '',b,sum(c) from table_d group by b`

## rollup:降级排列

`select a,b,sum(c) from table_d group by rollup(a,b)`

等价于

`select a,b,sum(c) from table_d group by a,b`

`union all`

`select a,'',sum(c) from table_d group by a`

`union all`

`select '','',sum(c) from table_d group by ''`

## cube:全排列

`select a,b,sum(c) from table_d group by cube(a,b)`

等价于

`select a,b,sum(c) from table_d group by a,b`

`union all`

`select a,'',sum(c) from table_d group by a`

`union all`

`select '',b,sum(c) from table_d group by b`

`union all`

`select '','',sum(c) group by ''`

# Oracle行转列

## privot函数

```sql
select * from
(select yljzhdh,substr(jyrq,5,2) jyrq, jyje from lsb)
privot
(
sum(jyje) su,
avg(jyje) av,
for(jyrq) in (
'01' as "1月",
'02' as "2月",
'03' as "3月",
'04' as "4月"
)
)
```



# mergerinto

## 语法

`merger into table_name alias1`

`using (table |view | subquery) alias2`

`on (join condition)`

`when matched then update set`

`when not matched then insert`

## 注意事项

joinCondition必须一一对应,否则会报错

# Oracle空值处理函数

## `nvl(expr1,expr2)`

如果expr1为null,返回expr2,否则返回expr1,两者类型要一致

## `nvl2(expr1,expr2,expr3)` 

expr1不为null,返回expr2,否则返回expr3,类型以expr2为准

## `nullIf(expr1,expr2)`

expr1相等返回NULL,不相等返回expr1

## `coalsece(expr1,expr2,expr3)`

从左到右选择不为null的一项

## `case when else end`

......

## `decode(expr1,value,result,value,result)`

根据expr1的计算值, 选择相应的result

# Oracle创建序列

`create sequence seq_users seq_name`

`increment by 1`

`start with 1`

`minvalue 1`

`max value 9999`

`cycle / no cycle`

`cache 20`

`order / no order`



# Oracle权限管理

## 系统权限

| 权限名         | 权限操作                                                     |
| -------------- | ------------------------------------------------------------ |
| 索引权限       | create any index<br />alter any index<br />drop any index    |
|                |                                                              |
| 存储过程权限   | create procedure<br />create any procedure<br />alter any procedure<br />execute any procedure<br />drop any procedure |
| 角色权限       | create role<br />alter any role<br />drop any role<br />grant any role |
| 序列权限       | create sequence<br />create any sequence<br />alter any sequence<br />select any sequence<br />drop any sequence |
| 登录数据库权限 | create session                                               |
| 表空间权限     | create tablespace<br />alter tablespace<br />drop tablespace<br />mange tablepace<br />unlimited tablespace |
| 类型权限       | create type<br />create any type<br />alter any type<br />drop any type<br />execute any type<br />under any type |
| 视图权限       | create view<br />create any view<br />under any view<br />drop any view<br />flashback any table<br />merge any view |
| 表权限         | create table<br />create any table<br />alter any table<br />backup any table<br />delete any table<br />drop any table<br />delete any table<br />drop any table<br />insert any table<br />lock any table<br />select any table<br />flashback any table<br />update any table |
| 触发器         | create trigger<br />create any trigger<br />alter any trigger<br />drop any trigger<br />administer database trigger |
| 备份数据库     | exp_full_database<br />imp_full_database                     |

## 权限赋予与撤销

### 权限赋予

```sql
grant select,delete,insert,update on username.table_name to username
grant execute on procedure_name to username
```

### 限制修改的列

```sql
grant update(column1,column2) on table_name to users
```

### 收回权限

```sql
revoke insert on table_name from username
```

## 权限查询

### 查询用户拥有的系统权限

```sql
select grantee,privilege from dba_sys_privs where grantee = 'scott'
```

### 查询用户拥有的对象权限

```
select grantee,table_name,privilege from dba_tab_privs where grantee='SCOTT'

```

### 查询用户所拥有的角色

```
select grantee , granted_role from dba_role_privs where grantee = 'SCOTT'
```

# Oracle优化器

## 优化器类型

### RBO

(rule based optimizer)

基于规则的优化

### CBO

(costed based optimizer) 

基于成本的优化

通过目标sql语句所设计的表索引,列等的统计信息 选择一条执行成本最小的执行计划

## cardinaltity 

结果集的势

指结果集所包含的列数

## selectivty

where条件筛选出来的记录数占总记录数的比率,越小越好

## 可传递性

### 简单谓语传递

`t1.c1 = t2.c2 and t1.c1 = 10`  添加 `t2.c2  = 10`

### 连接谓语传递

`t1.c1 = t2.c1   and  t2.c1 = t3.c1`  添加 `t3.c1 = t1.c1`

### 外连接谓语传递

`t1.c1 = t2.c1(+) and t1.c1=10`  添加 `t2.c1(+) = 10`

## 优化器模式

### RULE

基于规则的优化RBO

### CHOOSE

如果目标含有统计信息则 使用 CBO 

### FIRST_ROW

快速的返回记录,侧重响应时间

### ALL_ROWS

侧重最佳吞吐量,默认模式

## 数据访问方式

### 全表扫描

从表所占用的第一个extent 第一个块开始,一直到该表的最高水位线

### ROWID扫描

直接通过数据所在的rowID 定位到物理存储空间

### 索引扫描

#### 索引唯一性扫描

index unique scan : 适用于等值查询的目标

#### 索引范围扫描

index range scan

​	当扫描的对象是唯一性索引时,此时where条件为范围条件

​    当扫描的对象是非唯一性索引,where条件不做限制

#### 索引全扫描

index full scan

​	扫描索引的所有叶子块的所有行

#### 索引快速扫描

index fast full scan

#### 索引跳跃扫描

index skip scan

## 表连接的方式

### 排序合并

`sort meger into` 

1. 以目标sql中的谓语条件去访问表t1,对返回结果按连接列进行排序,得到结果集 s1
2. 以目标sql中的谓语条件访问 t2,对返回结果按连接列进行排序,得到结果集 s2
3. 对结果s1,s2 进行合并

### 嵌套循环

`nested loops join`

1. 根据规则决定 t1,t2 谁是驱动表,谁是被动表,驱动表做外层循环,被驱动表做内层
2. 假设t1 是 驱动表,t2是被驱动表
3. 用目标sql的谓语条件 访问表 t1 得到结果集 s1
4. 循环取出驱动表中的每条记录 与被驱动表关联

### 哈希连接

`hash join`

1. 依据 `hash_area_size`, `db_block_size`,`_hash_multiblock_io_count`决定`hash partition`数量
2. 使用谓语条件 访问t1,t2 得到结果集 s1,s2 ,假设s1数量少与s2
3. 取s1为驱动结果集, 遍历s1的每条记录, 取两个hash函数 哈希运算计算出来的结果放在不同的partition  的不同的bucket 
4. 同时构造s1,s2 的哈希table

# Oracle to_char

## 用于时间转换的格式字符

## 用于数值格式化

9 带有指定位数的值

0 前导0的值

. 小数点

, 分组(千) 分隔符

# Oracle表空间管理

## 简介

1. Oracle的逻辑结构包括表空间 段 区 块
2. 表空间是Oracle的逻辑组成部分, 一个表空间可以拥有多个数据文件
3. 在逻辑上,表空间由 段segment 组成
4. 段 segment 是由区构成
5. 块是由 Oracle数据块构成

## 建立表空间

1. 拥有 `create tablespace` 权限

2. 语法

   `create tablesapce tablespacename datafile'filePath' size 20M uniform size 128K`

## 修改表空间

`alter tablesapce tablename offline|online|read only| read write`

## 查询表空间所有信息

`select * from all_tables where tablesapce_name = ''`

## 删除表空间

`drop tablespace 'tablespacename' including contents and datafiles`

## 扩展表空间

### 增加数据文件

`alter tablespace tablesapcename add datafile 'filepath' size 20M`

## 增加数据文件大小

`alter tablespace tablespacename  'filepath' resize 20M`

## 设置文件自动增长

`alter tablesapce tablespacename 'filepath' autoextend on next 10m maxsize 500M`



# Oracle分区管理

## 概述

Oracle分区是一种处理超大型表,索引等的技术

## 优缺点

### 优点

增强可用性

可维护性

均衡IO

改善查询性能

### 缺点

已经存在的表无法直接转换分区表

## 分区方法

范围分区

Hash分区

列表分区

范围-散列分区

范围-列表分区

# Oracle优化

## 索引优化

1. Oracle不会索引空值 所以判断为空不为空 无法使用索引

   可以使用 < > 替代,或者空值用特定值替代

2. 不等于 <> 不会应用索引 可以改为 > < 

3. `>= 3 与 >2` 的比较

   1. `>`2  先找出 2的索引 然后作比较
   2. `>=3`  直接找出3 然后作比较

4. like操作符

   aa like '%AAA%' 不会应用索引

   可以改为 aa like 'BAAA%' or aa like 'CAAA%' 

5. in not in 会自动转换成外连接 

   尽量使用外连接

6. union ,union all , union 会排序去重

7. sql 语句书写, 是否大小写, 是否写用户名前缀,等等差异 会导致 识别为不同sql

8. where 条件执行顺序 是从 右到 左 , 尽量 过滤大头的 在 右边

9. 表连接时, 如果有 统计信息分析, 则会自动 先小表, 后大表

   `generate statistics on <table>`

10. 采用函数处理的字段无法使用索引

    1. substr(bh,1,4) = '5400' 改成 bh like '5400'
    2. trunc(rq) = trunc(sysdate)  rq > = trunc(sysdate) and rq < trunc(sysdate+1) 

11. 进行了显示或者 隐式运算的字段不能 索引

    1. df + 20 >50  不能用索引
    2. rq + 5  = sysdate 不能使用索引
    3. bh = 132456789 ; 不能使用索引,  bh = '132456789'

12. 条件包含了多个本表的字段运算时不能索引

    a > b 无法优化

    a||b = '123456' 优化 成 a='123' and b = '456'

    



# Oracle日期函数

## 函数

to_number(char)

to_date(char,pattern)

to_char(date,pattern)

date'char'

trunct(date,pattern)

extract(day|month|year| from date)返回数字格式

date + int 给日期加天数

last_day(date) 最后一天

round(date,'pattern') 四舍五入

months_betwteen(date,date)  两个日期之间的月份

next_day(sysdate,'星期三') 下个星期三的日期

## pattern

y 年的最后一位

yy 最后两位

yyy 后三位

yyyy 四位



mm 两位月份

mon 英文简写形式 11月或者nov

month 全称

dd 当月的第几天

ddd 当年的第几天

dy 当周第几天 简写星期5,fri

day 全称 星期五



hh 小时12进制

hh24 24小时制

mi 分钟

ss 秒

q 季度

ｗw 当年第几周

w 当月第几周

## 日期运算

sysdate - interval'7' year|month|minute|second 减去七年,月,分钟,秒

# Oracle数据库导入导出

## sqlldr加载平面文件

## 数据泵:二进制数据导入导出

## 外部表:以上两种方案的简化工具



