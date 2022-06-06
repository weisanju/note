## Snapshot and restore

1. 快照是从正在运行的Elasticsearch集群获取的备份。

2. 您可以拍摄整个集群的快照，包括其所有数据流和索引。

3. 您也可以仅对集群中的特定数据流或索引进行快照。



### 注册快照仓库

1. You must [register a snapshot repository](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/snapshots-register-repository.html) before you can [create snapshots](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/snapshots-take-snapshot.html).

2. 快照可以存储在本地或远程存储库中
3. Remote repositories can reside on Amazon S3, HDFS, Microsoft Azure, Google Cloud Storage, and other platforms supported by a [repository plugin](https://www.elastic.co/guide/en/elasticsearch/plugins/7.13/repository.html)

**Elasticsearch会增量获取快照:** 

1. 可以安全地以最小的开销非常频繁地拍摄快照
2. 这种增量仅适用于单个存储库。因为存储库之间没有共享数据
3. 快照在逻辑上也彼此独立，即使在单个存储库中也是如此: 删除快照不会影响任何其他快照的完整性。
4. 但是，您可以选择仅从快照中恢复群集状态或特定数据流或索引。
5. You can use [snapshot lifecycle management](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/getting-started-snapshot-lifecycle-management.html) to automatically take and manage snapshots.



备份群集的唯一可靠且受支持的方法是拍摄快照。您不能通过复制Elasticsearch集群节点的数据目录来备份它。没有支持的方法可以从文件系统级备份中还原任何数据。如果您尝试从这样的备份中恢复群集，它可能会因损坏或丢失文件或其他数据不一致的报告而失败，或者它似乎已经成功地无声地丢失了一些数据。



群集节点的数据目录的副本不能用作备份，因为它不是它们在单个时间点的内容的一致表示。您不能通过在复制时关闭节点来解决这个问题，也不能通过获取原子文件系统级快照来解决这个问题，因为Elasticsearch具有跨越整个集群的一致性要求。集群备份必须使用内置的快照功能。



### Version compatibility

版本兼容性是指基础Lucene索引兼容性。在版本之间迁移时，请遵循[升级文档。](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/snapshot-restore.html#:~:text=Upgrade%20documentation)

快照包含包含索引或数据流的备份索引的磁盘上数据结构的副本。这意味着快照只能恢复到可以读取索引的Elasticsearch版本。



快照包含包含索引或数据流的后备索引的磁盘上数据结构的副本。这意味着快照只能恢复到可以读取索引的Elasticsearch版本。



下表显示了版本之间的快照兼容性。第一列表示可以从中恢复快照的基本版本。



| **Cluster version**  |                                                              |                                                              |                                                              |                                                              |
| -------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| **Snapshot version** | 2.x                                                          | 5.x                                                          | 6.x                                                          | 7.x                                                          |
| **1.x** →            | ![Yes](https://doc-icons.s3.us-east-2.amazonaws.com/icon-yes.png) | ![No](https://doc-icons.s3.us-east-2.amazonaws.com/icon-no.png) | ![No](https://doc-icons.s3.us-east-2.amazonaws.com/icon-no.png) | ![No](https://doc-icons.s3.us-east-2.amazonaws.com/icon-no.png) |
| **2.x** →            | ![Yes](https://doc-icons.s3.us-east-2.amazonaws.com/icon-yes.png) | ![Yes](https://doc-icons.s3.us-east-2.amazonaws.com/icon-yes.png) | ![No](https://doc-icons.s3.us-east-2.amazonaws.com/icon-no.png) | ![No](https://doc-icons.s3.us-east-2.amazonaws.com/icon-no.png) |
| **5.x** →            | ![No](https://doc-icons.s3.us-east-2.amazonaws.com/icon-no.png) | ![Yes](https://doc-icons.s3.us-east-2.amazonaws.com/icon-yes.png) | ![Yes](https://doc-icons.s3.us-east-2.amazonaws.com/icon-yes.png) | ![No](https://doc-icons.s3.us-east-2.amazonaws.com/icon-no.png) |
| **6.x** →            | ![No](https://doc-icons.s3.us-east-2.amazonaws.com/icon-no.png) | ![No](https://doc-icons.s3.us-east-2.amazonaws.com/icon-no.png) | ![Yes](https://doc-icons.s3.us-east-2.amazonaws.com/icon-yes.png) | ![Yes](https://doc-icons.s3.us-east-2.amazonaws.com/icon-yes.png) |
| **7.x** →            | ![No](https://doc-icons.s3.us-east-2.amazonaws.com/icon-no.png) | ![No](https://doc-icons.s3.us-east-2.amazonaws.com/icon-no.png) | ![No](https://doc-icons.s3.us-east-2.amazonaws.com/icon-no.png) | ![Yes](https://doc-icons.s3.us-east-2.amazonaws.com/icon-yes.png) |

总结：

快照只能向前兼容一个majar版本，没法向后兼容



以下条件适用于跨版本恢复快照和索引: •

- **Snapshots**: 您不能将Elasticsearch版本的快照恢复到运行早期Elasticsearch版本的集群中。例如，您无法将7.6.0中拍摄的快照还原到运行7.5.0的群集。
- **Indices**: 您不能将索引还原到运行Elasticsearch版本的集群中，该版本比用于快照索引的Elasticsearch版本更新了多个 marjar verson。例如，无法将索引从5.0中获取的快照还原到运行7.0的群集。

需要注意的是，Elasticsearch 2.0拍摄的快照可以在运行Elasticsearch 5.0的集群中恢复。



每个快照都可以包含在各种版本的Elasticsearch中创建的索引。这包括为数据流创建的支持索引。还原快照时，必须可以将所有这些索引还原到目标群集中。如果快照中的任何索引都是在不兼容的版本中创建的，则将无法还原快照。



**注意**

1. 在升级前备份数据时，记住 如果升级后的版本不兼容，则不能还原快照
2. 如果实在需要 还原到 不兼容的版本，可以先还原到 最近兼容版本，使用 [reindex-from-remote](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/docs-reindex.html#reindex-from-remote) 去重建数据流或者索引到 当前的版本
3. 获取数据 然后 重新索引 会比 快照恢复 更加慢，建议操作之前 先计算 消耗时长





