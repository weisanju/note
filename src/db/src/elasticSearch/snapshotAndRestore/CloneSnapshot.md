## Clone snapshot API

Clones part or all of a snapshot into a new snapshot.

```console
PUT /_snapshot/my_repository/source_snapshot/_clone/target_snapshot
{
  "indices": "index_a,index_b"
}
```



### Request

```
PUT /_snapshot/<repository>/<source_snapshot>/_clone/<target_snapshot>
```





### Description

clone snapshot API允许在同一存储库中创建全部或部分现有快照的副本。



### Path parameters

- **`<repository>`**

  (Required, string) Name of the snapshot repository that both source and target snapshot belong to.

### Query parameters



- **`master_timeout`**

  (Optional, [time units](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#time-units)) Specifies the period of time to wait for a connection to the master node. If no response is received before the timeout expires, the request fails and returns an error. Defaults to `30s`.

- **`timeout`**

  (Optional, [time units](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#time-units)) Specifies the period of time to wait for a response. If no response is received before the timeout expires, the request fails and returns an error. Defaults to `30s`.

- **`indices`**

  (Required, string) A comma-separated list of indices to include in the snapshot. [Multi-index syntax](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/multi-index.html) is supported.

