## Cluster health API

返回集群健康信息







## Request

```
GET /_cluster/health/<target>
```



集群运行状况API返回集群运行状况的简单状态。您也可以使用API仅获取指定数据流和索引的健康状态。

对于数据流，API检索流的支持索引的健康状态。



集群健康状态为: 绿色、黄色或红色。

1. 在分片级别上，

* 红色表示集群中未分配特定shard，
* 黄色表示已分配主shard但副本未分配，
* 绿色表示已分配所有shard。

2. 索引级别状态由最差分片状态控制。

3. 集群状态由最差索引状态控制。



API的主要好处之一是可以等到集群达到一定的高水标健康水平。

例如，以下内容将等待50秒，以使群集达到黄色级别 (如果在50秒过去之前达到绿色或黄色状态，它将在该点上返回):



```console
GET /_cluster/health?wait_for_status=yellow&timeout=50s
```



### Path parameters

#### **`<target>`**

* 可选的逗号分割的字符串
* 可以表示 索引、别名 dataStreams
* 可以支持 *

### Query parameters

#### **`level`**

* 集群的级别： 
  * cluster 默认
  * indices
  * shards



#### **local**

* 只从管理节点拿状态
* 默认false
* false表示从 主节点拿状态



#### **master_timeout**

* 可选的 [时间单位](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/cluster-health.html#:~:text=(Optional%2C-,time%20units,-)%20Period%20to%20wait%20for%20a%20connection)
* 连接主节点的超时时间
* 默认30s

#### **timeout**

* 可选的 [时间单位](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/cluster-health.html#:~:text=(Optional%2C-,time%20units,-)%20Period%20to%20wait%20for%20a%20connection)
* 等待响应的 时间
* 默认30s



#### **`wait_for_active_shards`**

* 等待多少个活跃分片 
* all表示 等全部活跃分片
* 默认0 表示 不等待



#### **`wait_for_events`**

(Optional, string) Can be one of `immediate`, `urgent`, `high`, `normal`, `low`, `languid`. Wait until all currently queued events with the given priority are processed.



#### **wait_for_no_initializing_shards**

(可选，布尔值) 一个布尔值，该值控制是否等待 (直到提供超时) 以使群集没有分片初始化。默认为false，这意味着它不会等待初始化分片。

#### **`wait_for_no_relocating_shards`**

(可选，布尔值) 一个布尔值，该值控制是否等待 (直到提供超时) 以使群集没有分片重定位。默认为false，这意味着它不会等待重新定位分片。



#### **`wait_for_nodes`**

(可选，字符串) 请求等待，直到指定数量的N个节点可用。它也接受> = N，<= N，> N和 <N。或者，可以使用ge(N)，le(N)，gt(N) 和lt(N) 表示法。



#### **`wait_for_status`**

(可选，字符串) 绿色，黄色或红色之一。将等待 (直到提供的超时)，直到群集的状态更改为提供的状态或更好的状态，即绿色> 黄色> 红色。默认情况下，不会等待任何状态。





### Response body

### **`cluster_name`**

(string) The name of the cluster.



#### **`status`**

(string) Health status of the cluster, based on the state of its primary and replica shards. Statuses are:

- **`green`**

  All shards are assigned.

- **`yellow`**

  All primary shards are assigned, but one or more replica shards are unassigned. If a node in the cluster fails, some data could be unavailable until that node is repaired.

- **`red`**

  One or more primary shards are unassigned, so some data is unavailable. This can occur briefly during cluster startup as primary shards are assigned.



**`timed_out`**

(Boolean) If `false` the response returned within the period of time that is specified by the `timeout` parameter (`30s` by default).



#### **`number_of_nodes`**

(integer) The number of nodes within the cluster.





#### **`number_of_data_nodes`**

(integer) The number of nodes that are dedicated data nodes.





#### **active_primary_shards**

(integer) The number of active primary shards.



#### **`active_shards`**

(integer) The total number of active primary and replica shards.



#### **relocating_shards**

(integer) The number of shards that are under relocation.



#### **initializing_shards**

(integer) The number of shards that are under initialization.



#### **`unassigned_shards`**

(integer) The number of shards that are not allocated.



#### **`delayed_unassigned_shards`**

(integer) The number of shards whose allocation has been delayed by the timeout settings.

#### **number_of_pending_tasks**

(integer) The number of cluster-level changes that have not yet been executed.



#### **number_of_in_flight_fetch**

(integer) The number of unfinished fetches.



#### **task_max_waiting_in_queue_millis**

(整数) 任务最长等待时间

#### **active_shards_percent_as_number**

(float) The ratio of active shards in the cluster expressed as a percentage.



### example

```
{
    "cluster_name": "my-application",
    "status": "green",
    "timed_out": false,
    "number_of_nodes": 1,
    "number_of_data_nodes": 1,
    "active_primary_shards": 3,
    "active_shards": 3,
    "relocating_shards": 0,
    "initializing_shards": 0,
    "unassigned_shards": 0,
    "delayed_unassigned_shards": 0,
    "number_of_pending_tasks": 0,
    "number_of_in_flight_fetch": 0,
    "task_max_waiting_in_queue_millis": 0,
    "active_shards_percent_as_number": 100
}
```





