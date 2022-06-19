## Data tiers



数据层是具有相同 data Role 的节点的集合，这些节点通常共享相同的硬件配置文件:

- [Content tier](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/data-tiers.html#content-tier) 节点  处理  诸如  产品目录之类的内容的索引和查询 负载
- [Hot tier](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/data-tiers.html#hot-tier) 节点 处理 时间序列数据 (如日志或指标) 的索引负载，并保存您最近的、最频繁访问的数据。
- [Warm tier](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/data-tiers.html#warm-tier) 节点保存访问频率较低且很少需要更新的时间序列数据。
- [Cold tier](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/data-tiers.html#cold-tier) 节点保存不经常访问且通常不更新的时间序列数据。
- [Frozen tier](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/data-tiers.html#frozen-tier) 节点保存很少访问且从未更新的时间序列数据，并保留在可搜索的快照中。



1. 当您将文档直接索引到特定索引时，它们会无限期地保留在content tier nodes
2. 当您将文档索引到数据流时，它们最初驻留在热层节点上
3. 可以配置  [index lifecycle management](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-lifecycle-management.html) (ILM)   策略 使其自动 转化时间序列 到   the hot, warm, and cold tiers 
4. [data role](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/modules-node.html#data-node)  配置在  `elasticsearch.yml`   。对于高性能节点配置  data_hot, data_content 角色

```
node.roles: ["data_hot", "data_content"]
```

### Content tier

1. 存储在  content tier 通常是  物品或文章的集合
2. 与时间序列数据不同，随着时间的推移，内容的值保持相对恒定，因此随着时间的推移，将其移动到具有不同性能特征的层是没有意义的
3.  content tier  nodes 通常针对查询性能进行了优化-它们优先考虑处理能力而不是IO吞吐量，因此它们可以处理复杂的搜索和聚合并快速返回结果
4. 虽然他们也负责索引，但 content data 的摄取速度通常不会像日志和指标等时间序列数据那样高
5. 从弹性的角度来看，此层中的索引应配置为使用一个或多个副本。
6. 内容层是必需的。系统索引和其他不属于数据流的索引会自动分配给内容层。



### Hot tier

1. hot层是时间序列数据的Elasticsearch入口点，
2. 保存您最近、最常搜索的时间序列数据。热层中的节点需要快速的读写，这需要更多的硬件资源和更快的存储 (ssd)。为了恢复弹性，应将hot层中的索引配置为使用一个或多个副本。
3. 热层是必需的。作为数据流一部分的新索引会自动分配给热层。



### Warm tier

1. 与热层中的最近索引的数据相比，时间序列数据被查询的频率较低，可以移动到温暖层。
2. 温暖层通常保存最近几周的数据。仍然允许更新，但可能很少。
3. 暖层中的节点通常不需要像热层中的节点那样快。
4. 为了恢复弹性，应将温暖层中的索引配置为使用一个或多个副本。



### Cold tier

1. 一旦数据不再被更新，它就可以从温暖层移动到寒冷层，在那里它停留，而不经常被查询
2. 冷层仍然 响应查询请求 ，但是冷层中的数据通常不会更新。
3. 当数据过渡到冷层时，它可以被压缩和缩小
4. 为了弹性， the cold tier can use [fully mounted indices](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/searchable-snapshots.html#fully-mounted) of [searchable snapshots](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/ilm-searchable-snapshot.html), 消除了对副本的需求。

### Frozen tier

1. 一旦数据不再被查询，或者很少被查询，它可能会从冷层移动到冻结层，在那里它会在其余生中停留。
2. The frozen tier uses [partially mounted indices](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/searchable-snapshots.html#partially-mounted) to store and load data from a snapshot repository. 
3. 这降低了本地存储和运营成本，同时仍然让您搜索冻结的数据. 
4. 由于Elasticsearch有时必须从快照存储库中获取冻结的数据，因此冻结层上的搜索通常比冷层上的要慢。
5. We recommend you use [dedicated nodes](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/modules-node.html#data-frozen-node) in the frozen tier.

### Data tier index allocation

> 数据分层时的索引分配



1. 创建索引时，默认情况下，Elasticsearch将  [`index.routing.allocation.include._tier_preference`](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/data-tier-shard-filtering.html#tier-preference-allocation-filter)  设置为data_content，以自动将索引分片分配给content tier.
2. 创建数据流时， [`index.routing.allocation.include._tier_preference`](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/data-tier-shard-filtering.html#tier-preference-allocation-filter)  设置为 `data_hot` 
3. 可以手动指定  [shard allocation filtering](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/shard-allocation-filtering.html) settings   以 覆盖默认
4. 您还可以显式设置*index.routing.allocation.include._Tier_preference*以选择退出默认的基于层的分配。如果将层首选项设置为null，则Elasticsearch会在分配过程中忽略数据层角色。



### Automatic data tier migration

> 自动data tier 迁移

1. ILM使用 “迁移” [migrate](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/ilm-migrate.html)  操作自动将托管索引转换到可用的数据层中。

2. 默认情况下，此操作会在每个阶段自动注入。
3. 您可以显式指定迁移操作以覆盖默认行为，或者使用分配操作 [allocate action](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/ilm-allocate.html)  手动指定分配规则。

