## Restore snapshot API

```console
POST /_snapshot/my_repository/my_snapshot/_restore
```



### Request

```
POST /_snapshot/<repository>/<snapshot>/_restore
```





### Description

1. 使用restore snapshot API恢复集群的快照，包括快照中的所有数据流和索引。
2. 如果您不想恢复整个快照，可以选择要恢复的特定数据流或索引。



**恢复的条件**

1. 您可以在 

   1. 包含所选主节点
   2. 并 具有足够容量以容纳要还原的快照的数据节点的 

    群集上运行还原操作。

2. 仅当现有索引已关闭并且与快照中的索引具有相同数量的分片时，才能恢复它们。

3. 如果它们已关闭：还原操作会自动打开已恢复的索引

4. 如果它们在群集中不存在：则会创建新的索引 



**数据流的恢复**

1. 如果恢复了数据流，则其后备索引也将恢复。

2. 或者，您可以在不恢复整个数据流的情况下恢复单个备份索引。
3. 如果您恢复单个备份索引，它们不会自动添加到任何现有数据流中。
4. 例如，如果只有。ds-logs-2099.03.08-00003备份索引从快照中恢复，则不会自动将其添加到现有日志数据流中。



重要点

1.  `index_settings` and `ignore_index_settings` 这两个参数只影响 数据流的后备索引
2. 新的后备索引使用  [index template](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/set-up-a-data-stream.html#create-index-template). 指定的设置
3. 如果在 restore过程中改变了 索引设置 推荐 最好对  index template 也做同样的配置，这确保 新的后备索引 保持同样的 设置

​	



### Path parameters

- **`<repository>`**

  (Required, string) Name of the repository to restore a snapshot from.

- **`<snapshot>`**

  (Required, string) Name of the snapshot to restore.

### Query parameters

- **`master_timeout`**

  (Optional, [time units](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#time-units)) Period to wait for a connection to the master node. If no response is received before the timeout expires, the request fails and returns an error. Defaults to `30s`.

- **`wait_for_completion`**

  (Optional, Boolean) If `true`, the request returns a response when the restore operation completes. The operation is complete when it finishes all attempts to [recover primary shards](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/snapshots-monitor-snapshot-restore.html#_monitoring_restore_operations) for restored indices. This applies even if one or more of the recovery attempts fail.If `false`, the request returns a response when the restore operation initializes. Defaults to `false`.





### Request body

#### **ignore_unavailable**

1. 设置为FALSE。则如果索引或者数据流缺失或者关闭的情况下该请求会报错

#### **ignore_unavailable**

1. (可选，字符串) 以逗号分隔的索引设置列表，不应从快照中还原。

#### **ignore_index_settings**

1. 不应该还原的配置
2. 逗号分割的key

#### **`include_aliases`**

1. 是否恢复 索引别名
2. 默认TRUE

#### **include_global_state**

1. 是否还原 全局配置。默认FALSE不还原
2. 如果为TRUE 则以下几个状态会被还原
   1. Persistent cluster settings
   2. Index templates
   3. Legacy index templates
   4. Ingest pipelines
   5. ILM lifecycle policies
   6. For snapshots taken after 7.12.0, data stored in system indices, such as Watches and task records, replacing any existing configuration (configurable via `feature_states`)

**还原细节**

如果include_global_state为true，则还原操作将群集中的Legacy index templates与快照中包含的模板合并，并替换名称与快照中的模板匹配的任何现有模板。它完全删除了集群中存在的所有持久性设置，非传统索引模板，摄取管道和ILM生命周期策略，并将其替换为快照中的相应项目。



#### **`feature_states`**

1. 可选的。逗号分隔的字符串
2. 指定 还原的 状态
3. 每一个  feature state  包含 系统索引状态
4. 通过  [Get Snapshot API](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/get-snapshot-api.html#get-snapshot-api-feature-states).  可以获取 feature states
5.  feature states 会直接覆盖
6. 空数组 则导致 不会恢复  feature states, 从而就忽略 `include_global_state`
7. 默认下，当include_global_state 设置成 TRUE表示 恢复所有的。设置为FALSE 表示都不恢复

#### **index_settings**

1. 逗号分割的 设置列表
2. 可以用来 新增或者修改 所有索引的配置
3. 对于 索引级别的配置 详见 [index modules](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-modules.html).

#### **indices**

1. 逗号分割的字符串。指定恢复的索引
2. [Multi-index syntax](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/multi-index.html) is supported.
3. 默认情况下，还原操作包括快照中的所有数据流和索引。如果提供此参数，则还原操作仅包括指定的数据流和索引。



#### **partial**

1. 设置为FALSE，如果快照中包含的一个或多个索引中 如果没有做到 所有主分片都是可用的，则整个还原操作将失败。默认为false。
2. 设置为TRUE：允许部分索引 的不可用分片出现
3. 只有成功包含在快照中的分片才会被还原。所有丢失的分片将被重新创建为空的。



#### **rename_pattern**

1. 定义一个 索引或数据流 重命名的模式

2. 如果匹配这个模式 则 会被重命名

3. 可以使用正则替换。支持引用原始文本。根据  [`appendReplacement`](https://docs.oracle.com/javase/8/docs/api/java/util/regex/Matcher.html#appendReplacement-java.lang.StringBuffer-java.lang.String-) 逻辑

4. 如果两个不同索引被 重命名成一个名字 则 请求失败

5. 数据流的后备索引同样会被重命名 

   ```
   例如 logs -> restored-logs
   .ds-logs-2099.03.09-000005 
   is renamed to 
   .ds-restored-logs-2099.03.09-000005
   ```

6. 注意：要确保 数据流的 索引模板 能匹配新的 索引名字。否则就不能 rollover

#### **rename_replacement**

1. 定义替换后的索引名



### Examples

```console
POST /_snapshot/my_repository/snapshot_2/_restore?wait_for_completion=true
{
  "indices": "index_1,index_2",
  "ignore_unavailable": true,
  "include_global_state": false,
  "rename_pattern": "index_(.+)",
  "rename_replacement": "restored_index_$1",
  "include_aliases": false
}
```