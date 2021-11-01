# 什么是 Elasticsearch

Elasticsearch 基于分布式的搜索和分析引擎，是 Elastic Stack 的 核心。Logstash 和 Beats 用于收集、聚合，将数据存储在 Elasticsearch种，Kibana  可以互动的探索、可视化共享数据，并且管理和监控 堆栈

Elasticsearch 提供所有数据类型的 近实时搜索跟分析，无论你的数据是不是结构化，或者是地理位置数据，

Elasticsearch 都能有效的以最快的搜索的方式存储、索引

还可以 对数据进行聚合分析，随着数据量增长，天然分布式的Elasticsearch  也能无缝随之增长

使用场景

- app中的搜索引擎
- 日志存储分析
- 使用机器学习自动实时模拟数据的行为
- 使用 Elasticsearch 作为存储引擎 实现工作流程自动化
- 使用 Elasticsearch  作为 地理信息系统（GIS）管理、集成、分析空间信息
- 使用 Elasticsearch  作为生物信息学研究工具存储和处理遗传数据



## Data in: documents and indices

> 数据与存储

Elasticsearch 是一个分布式的文档存储 库，不是将数据存储为列

Elasticsearch 使用 JSON 存储复杂的 数据结构，当有多个节点时，文档会跨节点

当文档存储时，它会被索引、并在1s内可搜索

Elasticsearch 使用 倒排索引支持快速的 全文搜索

倒排索引列出所有文档的每个 原子的 单词，并标识出 它出现在所有文档得所有的位置

一个 **index** 可以认为是一种优化过的 **document** 的集合，每个 **document** 又都是 **filed** 的集合，**field** 本身又是 key-value键值对

默认情况下，Elasticsearch 会索引 所有 字段、，每个索引字段都有 专用的、优化的数据结构，例如基于 文本的字段 使用 倒排索引存储，基于 数值或 地理信息的 存储在 BKD树，使用每个字段数据结构来组装和返回搜索结果的能力是Elasticsearch 如此之快的原因。 

Elasticsearch  同样有 *schema-less* 的 能力：即不用 事先预定义 字段，当启用动态映射时，Elasticsearch 会自动添加到索引中

当然也可以 通过 定义规则，去完全掌控 动态映射 

自定义自己的映射可以使你

- 区分全文索引字符串 和 精确值字符串
- 执行特定语言的文本分析
- 优化 字段 的部分匹配
- 自定义日期格式
- 强制使用 `geo_point` and `geo_shape` 等这类不能自动检测的数据类型

通常也可以 对同一个字段进行不同的索引

例如：你想索引一个字符串字段 ，作为全文索引以供查询  以及 作为 关键字字段 以供排序跟 聚合，或者 您可能会选择使用多个语言分析仪来处理包含用户输入的字符串字段的内容。

在索引过程中应用于全文字段的分析链也在搜索时使用

.当您查询全文字段时，查询文本在索引中查找术语之前会进行相同的分析。

## Information out: search and analyze

> 产出

Elasticsearch 真正强大的 地方是 能够轻松访问 Apache Lucene 搜索引擎库上构建的完整搜索功能套件

Elasticsearch提供简单的、一致的REST API，用来管理集群、索引、查询数据，在测试时，可以直接在 Kibana 的 e Developer Console 上提交请求，对于应用程序，可以使用  [Elasticsearch client](https://www.elastic.co/guide/en/elasticsearch/client/index.html)  ： Java, JavaScript, Go, .NET, PHP, Perl, Python or Ruby

### Searching your data

可以使用 json风格的 查询语法 ([Query DSL](https://www.elastic.co/guide/en/elasticsearch/reference/current/query-dsl.html)). 也可以构建   [SQL-style queries](https://www.elastic.co/guide/en/elasticsearch/reference/current/sql-overview.html)  的查询语句，还可以兼容：JDBC and ODBC



### Analyzing your data

Elasticsearch 聚合使您能够构建复杂的数据摘要，并深入了解关键指标、模式和趋势



### 机器学习

想要自动分析您的时间系列数据？，

您可以使用 [machine learning](https://www.elastic.co/guide/en/machine-learning/7.15/ml-overview.html)  功能，，在数据中创建正确的正常行为基线，并识别异常模式。

通过机器学习，您可以检测到：

* Anomalies related to temporal deviations in values, counts, or frequencies

- Statistical rarity
- Unusual behaviors for a member of a population

最好的部分呢？您可以不必指定算法、模型或其他与数据科学相关的配置。





## Scalability and resilience: clusters, nodes, and shards

> 可扩展和弹性：集群、节点和碎片

Elasticsearch天生 分布式、对客户端无侵入

如何实现分布式：索引（index）是 对 一个或 多个 物理碎片（shard） 的逻辑分组，每个碎片都是一个 self-contained 的索引，将一个索引的多个文档 分布式在不同碎片上，将这些碎片 分布在不同节点上，Elasticsearch 可以允许冗余，这既可防止硬件故障，又可在组集添加节点时增加查询能力，集群增长或收缩 Elasticsearch 自动迁移 碎片 以重新平衡集群

两种类型的 碎片：primary、replicas（主碎片或 副本碎片）,每个文档属于主碎片，副本碎片是 主碎片的 copy

主碎片的 数量在 索引创建时 已经确定，副本碎片可以在运行时 动态调整，不会影响其他操作

### 碎片大小确定

索引的 碎片数量 和 主碎片数量的配置 有很多的 性能考量和权衡， 碎片越多，维持这些索引的间接消费就越多。碎片大小越大，当Elasticsearch 需要重新平衡集群时，移动碎片所需的时间越长。

Querying lots of small shards makes the processing per shard faster, but more queries means more overhead, so querying a smaller number of larger shards might be faster. 

查询大量小碎片使每个碎片的处理速度更快,但更多的查询意味着更多的开销，因此查询少量较大的碎片可能更快。

总之。。。这取决于。

作为起点：

- 旨在将平均碎片大小保持在几 GB 和几十 GB 之间。对于具有基于时间的数据的用例，常见于 20GB 到 40GB 范围内的碎片。
- 避免出现碎片问题。节点可容纳的碎片数量与可用堆空间成正比。一般来说，每 GB 堆空间的碎片数量应小于 20。

确定使用案例最佳配置的最佳方式是通过: [testing with your own data and queries](https://www.elastic.co/elasticon/conf/2016/sf/quantitative-cluster-sizing).

### 灾难备份

集群中的节点之间需要良好的连接，需要 节点需要放置到同一个数据中心或者 附近，另一方面 也需要避免单点故障。一个地区发生重大中断，另一个位置的服务器需要能够接管。解决方案是：Cross-cluster replication (CCR).

CCR 提供 自动 从 主集群中 同步索引到 副集群中 作为热备。如果主集群失败，副集群可以接管。还可以使用 CCR 创建次要集群，以在地理上更接近用户时提供服务读取请求。

跨集群复制是 active-passive ，主集群上得索引 是 leader 索引，处理所有写请求，副集群是 只读的 follower



### Care and feeding

作为企业级系统，需要 安全、管理、监控 Elasticsearch 集群

使用 [Kibana](https://www.elastic.co/guide/en/kibana/7.15/introduction.html)  作为 集群管理得 控制中心，例如 [data rollups](https://www.elastic.co/guide/en/elasticsearch/reference/current/xpack-rollup.html)   [index lifecycle management](https://www.elastic.co/guide/en/elasticsearch/reference/current/index-lifecycle-management.html) 

