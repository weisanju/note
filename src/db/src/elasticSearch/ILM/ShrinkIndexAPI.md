## Shrink index API

缩减现有索引的主分片 到新的索引

```console
POST /my-index-000001/_shrink/shrunk-my-index-000001
```





### Request

```
POST /<index>/_shrink/<target-index>
PUT /<index>/_shrink/<target-index>
```



### Prerequisites

- If the Elasticsearch security features are enabled, you must have the `manage` [index privilege](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/security-privileges.html#privileges-list-indices) for the index.
- Before you can shrink an index:
  - The index must be read-only
  - A copy of every shard in the index must reside on the same node.
  - The index must have a `green` [health status](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/cluster-health.html).

简单起见 推荐移除 索引的副本分片，之后在 重新添加副本

1. You can use the following [update index settings API](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-update-settings.html) request to remove an index’s replica shards
2. relocates the index’s remaining shards to the same node, and make the index read-only.

```console
PUT /my_source_index/_settings
{
  "settings": {
    "index.number_of_replicas": 0,//1          
    "index.routing.allocation.require._name": "shrink_node_name", //2
    "index.blocks.write": true //3
  }
}
```



1. Removes replica shards for the index.
2. Relocates the index’s shards to the shrink_node_name node. See [Index-level shard allocation filtering](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/shard-allocation-filtering.html).
3. Prevents write operations to this index. Metadata changes, such as deleting the index, are still allowed.

It can take a while to relocate the source index. Progress can be tracked with the [`_cat recovery` API](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/cat-recovery.html), or the [`cluster health` API](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/cluster-health.html) can be used to wait until all shards have relocated with the `wait_for_no_relocating_shards` parameter.





### Description

1. shrink API 允许你 缩减一个现有 的索引 到一个新索引。该索引只有很少的主分片
2. 目标索引中请求的主分片数量必须是源索引中分片数量的一个因子。
3. 例如 8个主分片 只能缩减为 4、2、1，15个主分片 只能缩减为 5、3、1
4. 如果是一个质数：则只能 缩减为 1个主分片
5. 在收缩之前，索引中每个分片的 (主或副本) 副本必须存在于同一节点上。
6. 当前 data stream  的 写索引 不能缩减，必须先要 rollover 才能缩减之前的索引

#### How shrinking works

1. 创建一个新的目标索引，其定义与源索引相同，但主分片数量较少。
2. 从源索引到目标索引的硬链接段。(如果文件系统不支持硬链接，那么所有段都将复制到新索引中，这是一个更耗时的过程。如果使用多个数据路径，如果不同数据路径上的分片不在同一磁盘上，则需要段文件的完整副本，因为硬链接无法跨磁盘工作)
3. Recovers the target index as though it were a closed index which had just been re-opened.

#### Shrink an index

To shrink `my_source_index` into a new index called `my_target_index`, issue the following request:

```console
POST /my_source_index/_shrink/my_target_index
{
  "settings": {
    "index.routing.allocation.require._name": null, //1
    "index.blocks.write": null //2
  }
}

```

1. Clear the allocation requirement copied from the source index.
2. Clear the index write block copied from the source index.



The above request returns immediately once the target index has been added to the cluster state — it doesn’t wait for the shrink operation to start.

Indices can only be shrunk if they satisfy the following requirements:

- The target index must not exist.
- The source index must have more primary shards than the target index.
- The number of primary shards in the target index must be a factor of the number of primary shards in the source index. The source index must have more primary shards than the target index.
- The index must not contain more than `2,147,483,519` documents in total across all shards that will be shrunk into a single shard on the target index as this is the maximum number of docs that can fit into a single shard.
- The node handling the shrink process must have sufficient free disk space to accommodate a second copy of the existing index.

The `_shrink` API is similar to the [`create index` API](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-create-index.html) and accepts `settings` and `aliases` parameters for the target index:

```console
POST /my_source_index/_shrink/my_target_index
{
  "settings": {
    "index.number_of_replicas": 1,
    "index.number_of_shards": 1, 
    "index.codec": "best_compression" 
  },
  "aliases": {
    "my_search_indices": {}
  }
}
```

Best compression will only take affect when new writes are made to the index, such as when [force-merging](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-forcemerge.html) the shard to a single segment.

Mappings may not be specified in the `_shrink` request.



#### Monitor the shrink process

**监控**

The shrink process can be monitored with the [`_cat recovery` API](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/cat-recovery.html), or the [`cluster health` API](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/cluster-health.html) can be used to wait until all primary shards have been allocated by setting the `wait_for_status` parameter to `yellow`.

**shrinkAPI返回**

1. _shrinkAPI 一旦被添加到集群状态上，会马上返回
2. 此时分片还未分配到任务节点上 所有分片 处于 未分配状态
3. 处于某种原因，不能将 索引分配到 shrink node 。主节点会保持 未分配状态 直到 有节点可用
4. 一旦主分片 被分配 了，会成为 initializing 状态。shrink 进程就会开始。
5. shrink 进程完成后。分片就会 active 
6. 在那时，Elasticsearch将尝试分配任何副本，并可能决定将主分片重新定位到另一个节点。

#### Wait for active shards

Because the shrink operation creates a new index to shrink the shards to, the [wait for active shards](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-create-index.html#create-index-wait-for-active-shards) setting on index creation applies to the shrink index action as well.



### Path parameters

- **`<index>`**

  (Required, string) Name of the source index to shrink.

- **`<target-index>`**

  (Required, string) Name of the target index to create.Index names must meet the following criteria:Lowercase onlyCannot include `\`, `/`, `*`, `?`, `"`, `<`, `>`, `|`, ` ` (space character), `,`, `#`Indices prior to 7.0 could contain a colon (`:`), but that’s been deprecated and won’t be supported in 7.0+Cannot start with `-`, `_`, `+`Cannot be `.` or `..`Cannot be longer than 255 bytes (note it is bytes, so multi-byte characters will count towards the 255 limit faster)Names starting with `.` are deprecated, except for [hidden indices](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-modules.html#index-hidden) and internal indices managed by plugins



### Query parameters

**`wait_for_active_shards`**

(Optional, string) The number of shard copies that must be active before proceeding with the operation. Set to `all` or any positive integer up to the total number of shards in the index (`number_of_replicas+1`). Default: 1, the primary shard.

See [Active shards](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/docs-index_.html#index-wait-for-active-shards).

**`master_timeout`**

(Optional, [time units](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#time-units)) Period to wait for a connection to the master node. If no response is received before the timeout expires, the request fails and returns an error. Defaults to `30s`.

**`timeout`**

(Optional, [time units](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#time-units)) Period to wait for a response. If no response is received before the timeout expires, the request fails and returns an error. Defaults to `30s`.

### Request body

**`aliases`**

索引对象

**`settings`**

(Optional, [index setting object](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-modules.html#index-modules-settings)) Configuration options for the target index. See [Index Settings](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-modules.html#index-modules-settings).

**max_primary_shard_size**

(Optional, byte units) The max primary shard size for the target index. 

1. 用于查找目标索引的最佳分片数。
2. 设置此参数时，每个分片在目标索引中的存储不会大于该参数
3. 目标索引的分片数仍将是源索引的分片计数的一个因素，但是，如果参数小于源索引中的单个分片大小，则目标索引的分片计数将等于源索引的分片计数。
4. 例如该 参数设置为 50GB。如果源索引有60个主分片，总共100G， 那么会收缩为2个主分片。每个50G，如果源索引 有60个主分片。总共100G。将会收缩到20个主分片。如果是60，4000G。则保持 原样不变 
5. 这个参数跟  number_of_shards settings 冲突。两者只能设置其一





