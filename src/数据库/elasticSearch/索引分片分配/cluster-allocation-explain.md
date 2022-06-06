## Cluster allocation explain API

提供分片当前分配的解释。



```console
GET _cluster/allocation/explain
{
  "index": "my-index-000001",
  "shard": 0,
  "primary": false,
  "current_node": "my-node"
}
```



### Description

the cluster allocation explain API  的目的是为集群中的分片分配提供解释。

1. 对于未分配的分片，explain API提供了为什么未分配分片的解释。

2. 对于分配的分片，explain API提供了一个解释，说明为什么分片保留在其当前节点上并且没有移动到或重新平衡到另一个节点。

3. 当您尝试诊断分片未分配的原因或为什么分片继续保留在其当前节点上时，此API可能非常有用。



### Query parameters

- **`include_disk_info`**

  (Optional, Boolean) If `true`, returns information about disk usage and shard sizes. Defaults to `false`.

- **`include_yes_decisions`**

  (Optional, Boolean) If `true`, returns *YES* decisions in explanation. Defaults to `false`.

### Request body

- **`current_node`**

  (Optional, string) Specifies the node ID or the name of the node to only explain a shard that is currently located on the specified node. :  申明  位于指定节点的 分片

- **`index`**

  (Optional, string) Specifies the name of the index that you would like an explanation for. 申明  位于指定索引的 分片

- **`primary`**

  (Optional, Boolean) If `true`, returns explanation for the primary shard for the given shard ID.

  ​	对于给定分片ID 返回 该分片ID的 主分片

- **`shard`**

  (Optional, integer) Specifies the ID of the shard that you would like an explanation for.

​			指定分片ID







## 实例

**查看索引信息**

```
GET _cat/indices/person2?v
```

```
[
    {
        "health": "green",
        "status": "open",
        "index": "person2",
        "uuid": "_M8Ya5LrSYKKe8Uqh4Tgvw",
        "pri": "2",
        "rep": "1",
        "docs.count": "2",
        "docs.deleted": "0",
        "store.size": "15.6kb",
        "pri.store.size": "7.8kb"
    }
]
```

**查看索引分片信息**

```
GET _cat/shards/person2?v
```

```json
[
    {
        "index": "person2",
        "shard": "1",
        "prirep": "r",
        "state": "STARTED",
        "docs": "1",
        "store": "3.9kb",
        "ip": "192.168.64.6",
        "node": "node-1"
    },
    {
        "index": "person2",
        "shard": "1",
        "prirep": "p",
        "state": "STARTED",
        "docs": "1",
        "store": "3.9kb",
        "ip": "192.168.64.12",
        "node": "node-2"
    },
    {
        "index": "person2",
        "shard": "0",
        "prirep": "p",
        "state": "STARTED",
        "docs": "1",
        "store": "3.9kb",
        "ip": "192.168.64.6",
        "node": "node-1"
    },
    {
        "index": "person2",
        "shard": "0",
        "prirep": "r",
        "state": "STARTED",
        "docs": "1",
        "store": "3.9kb",
        "ip": "192.168.64.12",
        "node": "node-2"
    }
]
```

**查看某一分片的分配详细信息**

```
GET _cluster/allocation/explain
{
  "index": "my-index-000001",
  "shard": 0,
  "primary": false,
  "current_node": "my-node"
}
```

```console-result
{
  "index" : "my-index-000001",
  "shard" : 0,
  "primary" : true,
  "current_state" : "unassigned",//分片分配状态    
  "unassigned_info" : { //分片未分配的 原始原因
    "reason" : "INDEX_CREATED",                   
    "at" : "2017-01-04T18:08:16.600Z",
    "last_allocation_status" : "no"
  },
  "can_allocate" : "no", //是否可分配       
  "allocate_explanation" : "cannot allocate because allocation is not permitted to any of the nodes", //分配解释
  "node_allocation_decisions" : [ //决定分片分配到节点的解释
    {
      "node_id" : "8qt2rY-pT6KNZB3-hGfLnw",
      "node_name" : "node-0",
      "transport_address" : "127.0.0.1:9401",
      "node_attributes" : {},
      "node_decision" : "no",                     
      "weight_ranking" : 1,
      "deciders" : [
        {
          "decider" : "filter",    //导致分片没有分配到节点的 员原因               
          "decision" : "NO",
          "explanation" : "node does not match index setting [index.routing.allocation.include] filters [_name:\"nonexistent_node\"]"  
        }
      ]
    }
  ]
}
```



#### unassigned primary shard 

先前已经分配的分片，但是现在未分配的主分片的解释。

```js
{
  "index" : "my-index-000001",
  "shard" : 0,
  "primary" : true,
  "current_state" : "unassigned",
  "unassigned_info" : {
    "reason" : "NODE_LEFT",
    "at" : "2017-01-04T18:03:28.464Z",
    "details" : "node_left[OIWe8UhhThCK0V5XfmdrmQ]",
    "last_allocation_status" : "no_valid_shard_copy"
  },
  "can_allocate" : "no_valid_shard_copy",
  "allocate_explanation" : "cannot allocate because a previous copy of the primary shard existed but can no longer be found on the nodes in the cluster"
}
```

#### Unassigned replica **shard**

由于延迟分配导致的副本分片未分配 https://www.elastic.co/guide/en/elasticsearch/reference/7.13/delayed-allocation.html

```js
{
  "index" : "my-index-000001",
  "shard" : 0,
  "primary" : false,
    //未分配
  "current_state" : "unassigned",
  "unassigned_info" : {
    "reason" : "NODE_LEFT",
    "at" : "2017-01-04T18:53:59.498Z",
    "details" : "node_left[G92ZwuuaRY-9n8_tc-IzEg]",
    "last_allocation_status" : "no_attempt"
  },
    //延迟分配
  "can_allocate" : "allocation_delayed",
    //等待过期节点加入集群
  "allocate_explanation" : "cannot allocate because the cluster is still waiting 59.8s for the departed node holding a replica to rejoin, despite being allowed to allocate the shard to at least one other node",
    //默认1minutes
  "configured_delay" : "1m",                      
  "configured_delay_in_millis" : 60000,
  "remaining_delay" : "59.8s",                    
  "remaining_delay_in_millis" : 59824,
  "node_allocation_decisions" : [
    {
      "node_id" : "pmnHu_ooQWCPEFobZGbpWw",
      "node_name" : "node_t2",
      "transport_address" : "127.0.0.1:9402",
      "node_decision" : "yes"
    },
    {
      "node_id" : "3sULLVJrRneSg0EfBB-2Ew",
      "node_name" : "node_t0",
      "transport_address" : "127.0.0.1:9400",
      "node_decision" : "no",
      "store" : {                                 
        "matching_size" : "4.2kb",
        "matching_size_in_bytes" : 4325
      },
      "deciders" : [
        {
          "decider" : "same_shard",
          "decision" : "NO",
          "explanation" : "a copy of this shard is already allocated to this node [[my-index-000001][0], node[3sULLVJrRneSg0EfBB-2Ew], [P], s[STARTED], a[id=eV9P8BN1QPqRc3B4PLx6cg]]"
        }
      ]
    }
  ]
}
```

#### Assigned shard

以下响应展示了：这个分片不能在待在 当前所处于的节点了，必须重新分片

```js
{
  "index" : "my-index-000001",
  "shard" : 0,
  "primary" : true,
  "current_state" : "started",
  "current_node" : {
    "id" : "8lWJeJ7tSoui0bxrwuNhTA",
    "name" : "node_t1",
    "transport_address" : "127.0.0.1:9401"
  },
    //是否能待在当前节点
  "can_remain_on_current_node" : "no", 
    //作出决定的决定器
  "can_remain_decisions" : [                      
    {
      "decider" : "filter",
      "decision" : "NO",
      "explanation" : "node does not match index setting [index.routing.allocation.include] filters [_name:\"nonexistent_node\"]"
    }
  ],
    //是否能移动到其他节点
  "can_move_to_other_node" : "no",                
    // 不能移动的解释
  "move_explanation" : "cannot move shard to another node, even though it is not allowed to remain on its current node",
  "node_allocation_decisions" : [
    {
      "node_id" : "_P8olZS8Twax9u6ioN-GGA",
      "node_name" : "node_t0",
      "transport_address" : "127.0.0.1:9400",
      "node_decision" : "no",
      "weight_ranking" : 1,
      "deciders" : [
        {
          "decider" : "filter",
          "decision" : "NO",
          "explanation" : "node does not match index setting [index.routing.allocation.include] filters [_name:\"nonexistent_node\"]"
        }
      ]
    }
  ]
}
```