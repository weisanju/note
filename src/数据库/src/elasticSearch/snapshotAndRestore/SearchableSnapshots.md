## Searchable snapshots

1. 可搜索快照使您可以使用快照以非常经济高效的方式搜索不经常访问的数据和只读数据。
2. 冷数据层和冻结数据层使用可搜索的快照来降低存储和运营成本。
3. 可搜索的快照 消除了对副本分片的需求，从而可能将搜索数据所需的本地存储减半。可搜索的快照依赖于您已经用于备份的相同快照机制，并且对快照存储库存储成本的影响最小。



### Using searchable snapshots

1. 搜索可搜索的快照索引与搜索任何其他索引相同。
2. 默认情况下，可搜索的快照索引没有副本
3. 底层快照提供了弹性，并且预计查询量足够低，以至于单个分片副本就足够了。
4. 但是，如果您需要支持更高的查询量，则可以通过调整*index.number_of_replicas*索引设置来添加副本。
5. 如果某个节点发生故障，并且需要在其他地方恢复可搜索快照的分片，则在Elasticsearch将分片分配的其他节点时，集群运行状况不为绿色，会有一个短暂的时间窗口。击中这些分片的搜索可能会失败或返回部分结果，直到将分片重新分配给健康节点为止。
6. 您通常通过ILM管理可搜索的快照。可搜索快照操作在到达冷或冻结阶段时会自动将常规索引转换为可搜索快照索引。
7. 您还可以通过使用 [mount snapshot](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/searchable-snapshots-api-mount-snapshot.html) API. 手动挂载索引来使现有快照中的索引可搜索。
8. 要从包含多个索引的快照中挂载索引，我们建议创建快照的克隆 [clone](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/clone-snapshot-api.html) ，该克隆仅包含要搜索的索引，并挂载该克隆的快照
9. 如果快照有任何已挂在的索引，则不应删除该快照，因此，创建克隆使您能够独立于任何可搜索的快照来管理备份快照的生命周期。如果您使用ILM来管理您的可搜索快照，那么它将在根据需要克隆快照后自动查看。
10. 您可以使用与常规索引相同的机制来控制可搜索快照索引的分片的分配。
11. 例如可以使用  [Index-level shard allocation filtering](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/shard-allocation-filtering.html)  设置 将可搜索的快照分片数限制为节点数的子集。
12. 可搜索快照索引的恢复速度受到  repository setting： max_restore_bytes_per_sec 和节点设置 indices.recovery.max_bytes_per_sec的限制，就像正常的还原操作一样
13. 默认情况下，*max_restore_bytes_per_sec*是无限的，但*indices.recovery.max_bytes_per_sec*的默认值取决于节点的配置。See [Recovery settings](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/recovery.html#recovery-settings).
14. 我们建议您在  获取快照之前，将索引强制合并，使得每个分片 单个片段，该快照将作为可搜索的快照索引挂载
15. 从快照存储库中的每次读取都会花费时间并花费金钱，并且段越少，恢复快照或响应搜索所需的读取就越少。
16. 可搜索的快照是管理大量历史数据档案的理想选择。历史信息的搜索频率通常低于最近的数据，因此可能不需要副本来获得其性能优势。
17. 对于更复杂或耗时的搜索，您可以将异步搜索 [Async search](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/async-search.html)  与可搜索快照一起使用。

将以下任何存储库类型与可搜索的快照一起使用:

- [AWS S3](https://www.elastic.co/guide/en/elasticsearch/plugins/7.13/repository-s3.html)
- [Google Cloud Storage](https://www.elastic.co/guide/en/elasticsearch/plugins/7.13/repository-gcs.html)
- [Azure Blob Storage](https://www.elastic.co/guide/en/elasticsearch/plugins/7.13/repository-azure.html)
- [Hadoop Distributed File Store (HDFS)](https://www.elastic.co/guide/en/elasticsearch/plugins/7.13/repository-hdfs.html)
- [Shared filesystems](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/snapshots-register-repository.html#snapshots-filesystem-repository) such as NFS
- [Read-only HTTP and HTTPS repositories](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/snapshots-register-repository.html#snapshots-read-only-repository)

您还可以使用这些存储库类型的替代实现，例如 [Minio](https://www.elastic.co/guide/en/elasticsearch/plugins/7.13/repository-s3-client.html#repository-s3-compatible-services)，只要它们完全兼容即可。使用存储库分析API分析您的存储库是否适合与可搜索快照一起使用。



### How searchable snapshots work

> 可搜索快照如何工作

When an index is mounted from a snapshot, Elasticsearch allocates its shards to data nodes within the cluster.

1. 从快照挂载索引时，Elasticsearch将其分片分配给集群内的 data nodes
2. 然后，数据节点自动将相关的分片数据从存储库中检索到本地存储,基于 [mount options](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/searchable-snapshots.html#searchable-snapshot-mount-storage-options) .如果可能，搜索使用来自本地存储的数据.  如果数据在本地不可用，Elasticsearch将从快照存储库中下载所需的数据。
3. 如果持有其中一个分片的节点发生故障，Elasticsearch会自动将受影响的分片分配到另一个节点上，并且该节点会从存储库中恢复相关的分片数据。
4. 不需要副本，也不需要复杂的监视或编排来恢复丢失的分片
5. 尽管默认情况下可搜索的快照索引没有副本，您可以通过调整index.number_of_replicas将副本添加到这些索引。
6. 通过从快照存储库中复制数据来恢复可搜索快照分片的副本。就像可搜索快照 分片的主分片一样。
7. 相反，常规索引的副本 通过从主分片 复制数据来恢复。



#### Mount options



要搜索快照，您必须首先将其作为索引在本地挂载。通常ILM会自动执行此操作，但是您也可以自己调用 [mount snapshot](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/searchable-snapshots-api-mount-snapshot.html) API 

从快照挂载索引有两个选项，每个选项都具有不同的性能特征和本地存储足迹:

- **Fully mounted index**
  1. 将快照索引的分片的完整副本加载到群集内的节点本地存储中，ILM在 热和冷阶段 使用此选项
  2. 完全挂载索引的搜索性能通常与常规索引相当，因为访问快照存储库的需求最小。
  3. 在恢复过程中，搜索性能可能比常规索引慢，因为搜索可能需要一些尚未检索到本地副本中的数据，如果发生这种情况，Elasticsearch将急切地检索与正在进行的恢复  并行完成搜索所需的数据。

- **Partially mounted index**

  

  1. 使用仅包含最近搜索的快照索引数据部分的本地缓存。
  2. 此缓存具有固定大小，并且在冻结层中的节点之间共享。ILM在冻结阶段使用此选项，
  3. 如果搜索需要不在缓存中的数据，则Elasticsearch会从快照存储库中获取丢失的数据，需要这些提取的搜索速度较慢，但是提取的数据存储在缓存中，以便将来可以更快地提供类似的搜索。
  4. Elasticsearch将从缓存中逐出不经常使用的数据以释放空间
  5. 虽然比完全挂载索引或常规索引慢，部分挂载的索引仍然会快速返回搜索结果，即使对于大型数据集，因为存储库中数据的布局针对搜索进行了大量优化
  6. 在返回结果之前，许多搜索将只需要检索总分片数据的一小部分。
  7. 要部分挂载索引，必须有一个或多个具有共享缓存的节点。
  8. 默认情况下，专用冻结数据层节点（具有data_frozen角色且没有其他数据角色的节点）具有 共享缓存，可以使用 总磁盘空间的90%，以及 总磁盘空间减100GB的净空
  9. 强烈建议在生产中使用专用的冷冻层
  10. 如果没有专用的冻结层，则必须配置*xpack.searchable.snapshot.shared_cache.size*设置为一个或多个节点上的缓存预留空间。部分挂载的索引仅分配给具有共享缓存的节点。

- **`xpack.searchable.snapshot.shared_cache.size`**

  ([Static](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/settings.html#static-cluster-setting)) Disk space reserved for the shared cache of partially mounted indices. Accepts a percentage of total disk space or an absolute [byte value](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#byte-units). Defaults to `90%` of total disk space for dedicated frozen data tier nodes. Otherwise defaults to `0b`.

- **`xpack.searchable.snapshot.shared_cache.size.max_headroom`**

  ([Static](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/settings.html#static-cluster-setting), [byte value](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#byte-units)) For dedicated frozen tier nodes, the max headroom to maintain. If `xpack.searchable.snapshot.shared_cache.size` is not explicitly set, this setting defaults to `100GB`. Otherwise it defaults to `-1` (not set). You can only configure this setting if `xpack.searchable.snapshot.shared_cache.size` is set as a percentage.

为了说明这些设置如何协同工作，让我们看两个示例，当在专用冻结节点上使用设置的默认值时:

- A 4000 GB disk will result in a shared cache sized at 3900 GB. 90% of 4000 GB is 3600 GB, leaving 400 GB headroom. The default `max_headroom` of 100 GB takes effect, and the result is therefore 3900 GB.
- A 400 GB disk will result in a shared cache sized at 360 GB.

You can configure the settings in `elasticsearch.yml`:

```yaml
xpack.searchable.snapshot.shared_cache.size: 4TB
```

1. 目前，您可以在任何节点上配置 xpack.searchable.snapshot.shared_cache.size。
2. 如果在 没有  [`data_frozen`](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/modules-node.html#data-frozen-node)  角色 的节点上设置 该 设置 它将被视为设置为0b。
3. 此外，具有共享缓存的节点只能具有单个数据路径 [data path](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/important-settings.html#path-settings).

### Back up and restore searchable snapshots

1. 您可以使用常规快照 [regular snapshots](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/snapshot-lifecycle-management.html)  来备份包含可搜索快照索引的集群。
2. 恢复包含可搜索快照索引的快照时，这些索引将再次恢复为可搜索快照索引。
3. 在还原包含可搜索快照索引的快照之前，必须先注册  [register the repository](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/snapshots-register-repository.html)  包含原始索引快照的存储库。
4. 恢复后，可搜索的快照索引将从其原始存储库中挂载原始索引快照。
5. 如果需要，您可以将单独的存储库用于常规快照和可搜索快照。
6. 可搜索快照索引的快照仅包含少量元数据，这些元数据标识其原始索引快照，它不包含来自原始索引的任何数据
7. 备份的恢复将  无法恢复任何   原始索引快照不可用的  可搜索快照索引



### Reliability of searchable snapshots

> 可搜索快照的可靠性

1. 可搜索快照索引中数据的唯一副本是存储在存储库中的基础快照。

2. 如果存储库失败或损坏快照的内容，则数据将丢失
3. 尽管Elasticsearch可能已将数据复制到本地存储中，但这些副本可能不完整，并且在存储库失败后无法用于恢复任何数据。
4. 您必须确保您的存储库是可靠的，并在存储库中的数据处于静止状态时防止数据损坏。
5. 所有主要的公共云提供商 提供的blob存储通常提供非常好的保护，防止数据丢失或损坏。如果您管理自己的存储库存储，则应对其可靠性负责。