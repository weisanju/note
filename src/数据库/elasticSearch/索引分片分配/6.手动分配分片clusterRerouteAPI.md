## Cluster reroute API

更改集群中分片的分配。



### Request

```
POST /_cluster/reroute
```



### Description

重新路由命令允许手动更改群集中单个分片的分配。

1. 例如，可以将分片从一个节点显式地移动到另一个节点

2. 可以取消分配，
3. 并且可以将未分配的分片显式地分配给特定节点。



重要的是，在处理任何重新路由命令之后，Elasticsearch将正常执行重新平衡 (尊重诸如*cluster.routing.rebalance.enable*等设置的值)，以保持平衡状态。

例如，如果请求的分配包括将分片从node1移动到node2，则这可能导致分片从node2移回node1以使事情变得均匀。



The cluster can be set to disable allocations using the `cluster.routing.allocation.enable` setting. 

If allocations are disabled then the only allocations that will be performed are explicit ones given using the `reroute` command, and consequent allocations due to rebalancing.



**dry_run**

It is possible to run `reroute` commands in "dry run" mode by using the `?dry_run` URI query parameter, or by passing `"dry_run": true` in the request body. This will calculate the result of applying the commands to the current cluster state, and return the resulting cluster state after the commands (and re-balancing) has been applied, but will not actually perform the requested changes.

**explain**

If the `?explain` URI query parameter is included then a detailed explanation of why the commands could or could not be executed is included in the response.

集群会 尝试分配分片 ，并且重试 index.allocation.max_retries 

This scenario can be caused by structural problems such as having an analyzer which refers to a stopwords file which doesn’t exist on all nodes.

一旦问题得到纠正，可以通过 reoute?retry_failed URI查询参数，进行一轮分片分配尝试



### Query parameters

**`dry_run`**

(Optional, Boolean) If `true`, then the request simulates the operation only and returns the resulting state.

**`explain`**

(Optional, Boolean) If `true`, then the response contains an explanation of why the commands can or cannot be executed.

**`metric`**

(Optional, string) Limits the information returned to the specified metrics. Defaults to all but metadata The following options are available:

```
**`_all`**

Shows all metrics.

**`blocks`**

Shows the `blocks` part of the response.

**`master_node`**

Shows the elected `master_node` part of the response.

**`metadata`**

Shows the `metadata` part of the response. If you supply a comma separated list of indices, the returned output will only contain metadata for these indices.

**`nodes`**

Shows the `nodes` part of the response.

**`routing_table`**

Shows the `routing_table` part of the response.

**`version`**

Shows the cluster state version.
```

**`retry_failed`**

(Optional, Boolean) If `true`, then retries allocation of shards that are blocked due to too many subsequent allocation failures.

**`master_timeout`**

(Optional, [time units](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#time-units)) Period to wait for a connection to the master node. If no response is received before the timeout expires, the request fails and returns an error. Defaults to `30s`.

**`timeout`**

(Optional, [time units](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#time-units)) Period to wait for a response. If no response is received before the timeout expires, the request fails and returns an error. Defaults to `30s`.



### Request body

**`commands`**

(Required, array of objects) Defines the commands to perform. Supported commands are:

**Properties of commands**

**`move`**

Move a started shard from one node to another node. Accepts `index` and `shard` for index name and shard number, `from_node` for the node to move the shard from, and `to_node` for the node to move the shard to.

**`cancel`**

Cancel allocation of a shard (or recovery). Accepts `index` and `shard` for index name and shard number, and `node` for the node to cancel the shard allocation on. This can be used to force resynchronization of existing replicas from the primary shard by cancelling them and allowing them to be reinitialized through the standard recovery process. By default only replica shard allocations can be cancelled. If it is necessary to cancel the allocation of a primary shard then the `allow_primary` flag must also be included in the request.

**`allocate_replica`**

Allocate an unassigned replica shard to a node. Accepts `index` and `shard` for index name and shard number, and `node` to allocate the shard to. Takes [allocation deciders](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/modules-cluster.html) into account.

Two more commands are available that allow the allocation of a primary shard to a node. These commands should however be used with extreme care, as primary shard allocation is usually fully automatically handled by Elasticsearch. Reasons why a primary shard cannot be automatically allocated include the following:

- A new index was created but there is no node which satisfies the allocation deciders.
- An up-to-date shard copy of the data cannot be found on the current data nodes in the cluster. To prevent data loss, the system does not automatically promote a stale shard copy to primary.



The following two commands are dangerous and may result in data loss. They are meant to be used in cases where the original data can not be recovered and the cluster administrator accepts the loss. If you have suffered a temporary issue that can be fixed, please see the `retry_failed` flag described above. To emphasise: if these commands are performed and then a node joins the cluster that holds a copy of the affected shard then the copy on the newly-joined node will be deleted or overwritten.



**allocate_stale_primary**
Allocate a primary shard to a node that holds a stale copy. Accepts the index and shard for index name and shard number, and node to allocate the shard to. Using this command may lead to data loss for the provided shard id. If a node which has the good copy of the data rejoins the cluster later on, that data will be deleted or overwritten with the data of the stale copy that was forcefully allocated with this command. To ensure that these implications are well-understood, this command requires the flag accept_data_loss to be explicitly set to true.

**allocate_empty_primary**
Allocate an empty primary shard to a node. Accepts the index and shard for index name and shard number, and node to allocate the shard to. Using this command leads to a complete loss of all data that was indexed into this shard, if it was previously started. If a node which has a copy of the data rejoins the cluster later on, that data will be deleted. To ensure that these implications are well-understood, this command requires the flag accept_data_loss to be explicitly set to true.

```console
POST /_cluster/reroute
{
  "commands": [
    {
      "move": {
        "index": "test", "shard": 0,
        "from_node": "node1", "to_node": "node2"
      }
    },
    {
      "allocate_replica": {
        "index": "test", "shard": 1,
        "node": "node3"
      }
    }
  ]
}
```