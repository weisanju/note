### Create snapshot API

```
PUT /_snapshot/my_repository/my_snapshot

```



### Request

```
PUT /_snapshot/<repository>/<snapshot>
POST /_snapshot/<repository>/<snapshot>
```







### Description

您可以使用create snapshot API创建快照，该快照是从正在运行的Elasticsearch集群中获取的备份。



默认情况下，快照包括集群中的所有数据流和开放索引，以及集群状态。您可以通过在快照请求的request body 中 指定要备份的数据流和索引列表来更改此行为。

注意必须先注册 快照仓库

快照是增量的

快照过程以非阻塞方式执行，因此所有索引和搜索操作都可以在 正在快照的数据流或索引同时运行。

快照表示创建快照的时刻的时间点视图。快照过程开始后，没有添加到数据流或索引的记录将出现在快照中。



1. 对于尚未启动且当前未重新定位的主分片，快照过程将立即启动。

2. 如果分片正在启动或重新定位，Elasticsearch会在拍摄快照之前等待这些进程完成。
3. 重要：
   1. 拍摄快照期间。分片移动到其他节点
   2. 分配重路由、重定位 会被快照 干扰知道快照过程结束
   3. 除了copy数据之外。还copy集群元配置、包括索引模板、组件模板、集群持久化配置 、
   4. 瞬时配置 和 注册的 快照仓库 配置 不会快照

### Query parameters

**`master_timeout`**

(Optional, [time units](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#time-units)) Period to wait for a connection to the master node. If no response is received before the timeout expires, the request fails and returns an error. Defaults to `30s`.

**`wait_for_completion`**

(Optional, Boolean) If `true`, the request returns a response when the snapshot is complete. If `false`, the request returns a response when the snapshot initializes. Defaults to `false`.





### Request body

#### **ignore_unavailable**

1. 是否忽略 不可用索引
2. 如果 数据流或者 索引 被关闭了。或者丢失了。则该快照过程失败
3. 默认 FALSE，不失败



### **indices**

1. 逗号分割的 索引列表
2. 详见：[Multi-index syntax](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/multi-index.html)
3. 快照默认包含集群中的所有数据流和索引。如果提供此参数，则快照仅包含指定的数据流和群集。

### **include_global_state**

1. 包含当前集群的全局状态
2. 默认TRUE
3. 全局状态包括
   1. Persistent cluster settings
   2. Index templates、Legacy index templates
   3. Ingest pipelines
   4. ILM lifecycle policies
   5. Data stored in system indices, such as Watches and task records (configurable via feature_states)

### **feature_states**

1. 可选的 数组string
2. 快照一些列的特点。
3. 可以使用 [get features API](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/get-features-api.html) 得到 这些描述
4. 每个特征状态包括一个或多个系统索引
   1. 每个特征状态包括一个或多个系统索引，
   2. 其中包含该特征的功能所需的数据。
   3. 提供空数组将在快照中不包含任何特征状态，
   4. 而不管include_global_state的值如何。
   5. 默认的。所有可用的 特征 状态 将会被 快照
   6. 如果 include_global_state 设置为TRUE。则会包含所有的。设置为FALSE 则 不包含任何

### **metadata**

1. 附加自定义元数据。例如 谁建立的快照、为什么建立快照 。必须小于 1024个字节

### **partial**

1. 如果设置成FALSE 。一旦 当中某些索引 存在 主分片不可用的情况 则 快照失败
2. 设置为TRUE表示 可以 接受部分 快照



### example

```console
PUT /_snapshot/my_repository/snapshot_2?wait_for_completion=true
{
  "indices": "index_1,index_2",
  "ignore_unavailable": true,
  "include_global_state": false,
  "metadata": {
    "taken_by": "user123",
    "taken_because": "backup before upgrading"
  }
}
```



```
{
  "snapshot": {
    "snapshot": "snapshot_2",
    "uuid": "vdRctLCxSketdKb54xw67g",
    "version_id": <version_id>,
    "version": <version>,
    "indices": [],
    "data_streams": [],
    "feature_states": [],
    "include_global_state": false,
    "metadata": {
      "taken_by": "user123",
      "taken_because": "backup before upgrading"
    },
    "state": "SUCCESS",
    "start_time": "2020-06-25T14:00:28.850Z",
    "start_time_in_millis": 1593093628850,
    "end_time": "2020-06-25T14:00:28.850Z",
    "end_time_in_millis": 1593094752018,
    "duration_in_millis": 0,
    "failures": [],
    "shards": {
      "total": 0,
      "failed": 0,
      "successful": 0
    }
  }
}
```

