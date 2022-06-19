# Data management



您存储在Elasticsearch中的数据通常分为以下两类之一:

- Content: 您要搜索的项目集合，例如产品目录
- Time series data: 连续生成的带时间戳的数据流，例如日志条目



1. 内容可能会经常更新，但内容的值随着时间的推移保持相对恒定
2. 您希望能够快速检索项目，而不管它们的年龄有多大。
3. 时间序列数据会随着时间的推移而不断累积，因此您需要策略来平衡数据的价值与存储数据的成本
4. 随着年龄的增长，它往往变得不那么重要，并且访问频率较低，因此您可以将其移至价格更低，性能更低的硬件
5. 对于最旧的数据，重要的是您可以访问数据。如果查询需要更长的时间才能完成，这是可以接受的。

为了帮助您管理数据，Elasticsearch使您能够:

- Define [multiple tiers](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/data-tiers.html) of data nodes with different performance characteristics：定义具有不同性能特征的多层数据节点。
- 根据您的性能需求和保留策略，在数据层中自动转换索引，使用  [index lifecycle management](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-lifecycle-management.html) (ILM).
- 利用存储在远程存储库中的可搜索快照 [searchable snapshots](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/searchable-snapshots.html) ，为您的旧索引提供弹性，同时降低运营成本并保持搜索性能。
- 存储在性能较差的硬件上的数据执行异步搜索 [asynchronous searches](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/async-search-intro.html) 



 