### Get snapshot API

```
GET /_snapshot/my_repository/my_snapshot
```



### Request

```
GET /_snapshot/<repository>/<snapshot>
```





### Description

使用get snapshot API返回有关一个或多个快照的信息，包括: •

- Start and end time values
- Version of Elasticsearch that created the snapshot
- List of included indices
- Current state of the snapshot
- List of failures that occurred during the snapshot



### Path parameters

**`<repository>`**

(Required, string) Snapshot repository name used to limit the request



**`<snapshot>`**

(Required, string) Comma-separated list of snapshot names to retrieve. Also accepts wildcards (`*`).

* To get information about all snapshots in a registered repository, use a wildcard (`*`) or `_all`.
* To get information about any snapshots that are currently running, use `_current`.



Using _all in a request fails if any snapshots are unavailable. Set ignore_unavailable to true to return only available snapshots.





### Query parameters

#### **master_timeout**

1. [time units](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#time-units)
2. 等待主节点连接的超时时长
3. 如果指定超时时间没有返回。则失败。
4. 默认30是

#### **ignore_unavailable**

1. 部分快照不可用。任然可以返回
2. 默认FALSE

#### **verbose**

1. 返回额外信息：拍摄快照时的集群版本
2. 快照开始结束时间
3. 快照的分片数量
4. 默认TRUE。如果是FALSE 则忽略额外信息

#### **index_details**

1. 返回索引的详细信息
2. 包括索引中的分片数，索引的总大小 (以字节为单位) 以及索引中每个分片的最大段数。默认为false，意味着此信息被省略。



### Response body

#### **snapshot**

(string) Name of the snapshot.

#### **`uuid`**

(string) Universally unique identifier (UUID) of the snapshot.

#### **version_id**

ES 版本ID



#### VERSION

ES版本



#### **indices**

快照索引



#### **index_details**

快照索引明细

1. shard_count:索引分片数量
2. **size**:分片总大小。当human 参数设置时才有
3. **size_in_bytes**:分片总大小。当human 参数设置时才有
4. **max_segments_per_shard**:
   1. 当前索引 快照的 最大 段数





### **data_streams**

包含的dataStream



#### **include_global_state**

是否包含全局状态

**feature_states**

指定的全局状态



#### **start_time**

#### **start_time_in_millis**

#### **end_time**

#### **end_time_in_millis**

#### **duration_in_millis**

#### **failures**

#### **shards**

分片信息

##### `**total**

快照`中最多 分片数

**`successful`**

(integer) Number of shards that were successfully included in the snapshot.

**`failed`**

(integer) Number of shards that failed to be included in the snapshot. 



### **state**

#### **IN_PROGRESS**

运行中



#### **SUCCESS**

运行成功

#### **FAILED**

失败



### **`PARTIAL`**

部分失败



### Examples

```console
GET /_snapshot/my_repository/snapshot_2
```

```console-result
{
  "snapshots": [
    {
      "snapshot": "snapshot_2",
      "uuid": "vdRctLCxSketdKb54xw67g",
      "version_id": <version_id>,
      "version": <version>,
      "indices": [],
      "data_streams": [],
      "feature_states": [],
      "include_global_state": true,
      "state": "SUCCESS",
      "start_time": "2020-07-06T21:55:18.129Z",
      "start_time_in_millis": 1593093628850,
      "end_time": "2020-07-06T21:55:18.876Z",
      "end_time_in_millis": 1593094752018,
      "duration_in_millis": 0,
      "failures": [],
      "shards": {
        "total": 0,
        "failed": 0,
        "successful": 0
      }
    }
  ]
}
```