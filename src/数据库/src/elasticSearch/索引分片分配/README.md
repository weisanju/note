## Index Shard Allocation

该模块提供了  按索引设置来控制 分片分配给节点:

- [Shard allocation filtering](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/shard-allocation-filtering.html): 控制哪些分片分配给哪些节点。
- [Delayed allocation](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/delayed-allocation.html): 当节点离开之后。分片重分配工作会延迟
- [Total shards per node](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/allocation-total-shards.html): 每个节点来自相同索引的分片数量的硬限制。
- [Data tier allocation](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/data-tier-shard-filtering.html): Controls the allocation of indices to [data tiers](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/data-tiers.html).