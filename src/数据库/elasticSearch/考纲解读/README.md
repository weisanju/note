# 官方文档

>  https://www.elastic.co/cn/training/elastic-certified-engineer-exam

## **数据管理**

- 按照要求定义索引
  - 定义索引
- 使用数据可视化工具将文本文件上传到Elasticsearch

- 为满足一组给定要求的给定模式定义和使用索引模板
  - 索引模板定义
- 在 满足给定要求 的情况下 定义和使用的动态模板
  - 动态索引模板定义
- 为时间序列索引定义索引生命周期管理策略
  - ILM 索引生命周期管理
- 定义创建新数据流的索引模板
  - dataStream



## **Searching Data**

- 在索引的多个字段中使用 术语或者短语匹配
- 使用bool 联合过滤
- 异步查询
- 使用指标或桶查询
- 使用桶子查询
- 跨多个集群搜索

## **Developing Search Applications**

- 突出显示查询响应中的搜索词
- 按指定要求排序查询结果
- 实现分页
- 定义和使用 alias
- 定义和使用 搜索模板

## **Data Processing**

- 按照给定要求 定义 mapping
- 按照给定要求定义 自定义分析器
- 定义和使用具有不同数据类型和/或分析器的多字段
- 使用 reIndex 或者 updateByQuery 重新索引文档
- 按照给定要求 定义和使用 ingest pipeline  包括 使用 Painless 脚本修改文档
- 配置索引，使其正确维护对象的嵌套数组的关系

## **Cluster Management**

- 诊断分片问题并修复集群的运行状况
- 备份和还原集群和/或特定索引
- 将快照配置为可搜索的
- 为跨集群搜索配置集群
- 实现跨集群复制
- 使用Elasticsearch Security定义基于角色的访问控制



## 模块集合

部署、索引、检索、聚合、分析、文档、集群、安全



# 考试路线

1. 梳理考点
2. 学习并整理考点相关知识点
3.  学习并整理 知识星球 相关考点 解答
4.  刷题：主要是知识星球 真题演练
5. 总结经验：总结过来人经验
6. 报名事宜

# 考纲

## **Cluster Management**

### 普通方式与集群方式安装和基本配置

### 诊断分片问题并修复集群的运行状况

#### 文档

- https://www.elastic.co/guide/en/elasticsearch/reference/7.13/cluster-health.html
- https://www.elastic.co/guide/en/elasticsearch/reference/7.13/cluster-allocation-explain.html
- https://www.elastic.co/guide/en/elasticsearch/reference/7.13/cluster-reroute.html

#### 考点梳理

- cat api使用（很多，都要熟悉）
- 诊断集群健康状态，找到黄色或红色非健康能找到原因，并变成健康绿色状态
- 诊断集群分配未分配的原因，并恢复正常
- 集群分配迁移等重新路由实现

### 备份和还原集群和/或特定索引

#### 文档

https://www.elastic.co/guide/en/elasticsearch/reference/7.13/backup-cluster.html

https://www.elastic.co/guide/en/elasticsearch/reference/7.13/snapshot-restore.html

#### 考点梳理

- 快照备份集群并恢复
- 快照备份指定索引并恢复
- 一定要验证一下恢复是否正确，是否满足给定题目的条件

### 将快照配置为可搜索的

#### 文档

https://www.elastic.co/guide/en/elasticsearch/reference/7.13/searchable-snapshots.html

https://www.elastic.co/guide/en/elasticsearch/reference/7.13/ilm-searchable-snapshot.html

#### 考点梳理

- 配置可搜索快照
- 执行快照检索





### 为跨集群搜索配置集群

#### 文档

https://www.elastic.co/guide/en/elasticsearch/reference/7.13/modules-cross-cluster-search.html

#### 考点梳理

- 能实现跨集群检索配置
- 能实现跨集群检索
- 考试的时候，一定要验证返回结果是不同集群返回的才可以



### 实现跨集群复制

#### 文档

https://www.elastic.co/guide/en/elasticsearch/reference/current/xpack-ccr.html

https://www.elastic.co/guide/en/elasticsearch/reference/7.13/ccr-apis.html

#### 考点梳理

- 跨集群复制

### 使用Elasticsearch Security定义基于角色的访问控制

#### 文档

https://www.elastic.co/guide/en/elasticsearch/reference/7.13/security-api-put-role.html

https://www.elastic.co/guide/en/elasticsearch/reference/7.13/security-api-put-user.html

#### 考点梳理

- x-pack 一般会结合role 角色一起考
- 新建角色
- 新建用户&密码，修改密码
- 官方我咨询过：命令行或者kibana操作都可以，但要确保结果对。建议kibana，毕竟比较简洁。
- kibana权限设置，一定要加上能访问kibana，否则新建了用户会无法登录（可能会扣分）
- 举例：设置x-pack属性后（默认未开启），设置用户名、密码（可以kibana设置）、设置访问权限等。



# 知识星球脑图

## 集群

### 健康状态

* green
* red
* yellow
* 排查
  * health
  * explain

### 集群高可用

* 副本

* 备份

  * snapshot
  * Restore1

  

### 跨集群

* 检索
* 复制

### 冷热集群架构

* ILM 索引生命周期管理

### 数据导入

* ML

### 安全

* 三种类型

## 节点

### 角色

## 索引

### 增：put

### 删：delete

* 删除整个索引
* 删除索引数据：*delete_by_query*

### 改：reindex

* source
* dest

### 查

* search
* "...base_"

### template

* 动态模板
* 静态模板

### alias

* 给定索引指定别名
* 给定模板指定别名

### 组成

#### settings

* 动态参数
  * 副本数
  * *max_result_window*
* 静态参数
  * 主分片数

#### mappings

##### 类型的选型

* 数值类型
  * Integer
  * float
  * double
  * long
  * short
* 复杂数据
  * object
  * nested
    * 子文档更新不屏幕
  * join父子关联
    * 父子之间是放置 到 不同的 type 下面
    * 相同的type下面
    * 子文档频繁更新的场景
* 很多的其他类型
  * 日期类型
  * 地理位置类型
  * *runtime field* 运行的类型

##### 参数设置



## 分片

### 分片分配策略

* *shard allocation*  冷热架构
* *shard awareness allocation* 跨机房、跨机架的架构

## 文档Document

### 增

#### 单条数据新增

post

#### 批量数据增加

bulk



### 删

* Delete id 
* Delete_by_query

### 改

* 单 -- update
* 批量数据
  * update_by_query
    * Ingest processors 
    * script
      * painless 脚本
      * ctx
      * doc
      * _source

### 查

### redindex

## 检索

### 基础检索

### 检索的分类

#### 全文检索

* match
* Match_phrase
* query_string

#### 精确匹配

* terms
* term
* wlidcard
* exists
* range



### 组合检索

**bool**

* must
* must_not
* filter
* should *minnum_should_count*

### 自定义评分

* boost
* rescore
* script 方式

### 异步检索

## 聚合

### Metric 指标聚合 avg

### bucket 分桶聚合 terms

### pipeline 基于 聚合 结果 的聚合 （子聚合）





1. 集群手动分配分片：cluster/reoute
2. 集群分片分配解释：cluster/explain
3. 通过节点属性 个性化 集群分片分配 cluster/



