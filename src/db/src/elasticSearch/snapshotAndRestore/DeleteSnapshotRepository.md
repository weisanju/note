## Delete snapshot repository API

Unregisters one or more [snapshot repositories](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/snapshots-register-repository.html).

当取消注册存储库时，Elasticsearch仅删除对存储库存储快照的位置的引用。快照本身保持不变。

```
DELETE /_snapshot/my_repository
```



### Request

```
DELETE /_snapshot/<repository>
```



### Path parameters

(Required, string) Name of the snapshot repository to unregister. Wildcard (*) patterns are supported.



### Query parameters

- **`master_timeout`**

  (Optional, [time units](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#time-units)) Specifies the period of time to wait for a connection to the master node. If no response is received before the timeout expires, the request fails and returns an error. Defaults to `30s`.

- **`timeout`**

  (Optional, [time units](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#time-units)) Specifies the period of time to wait for a response. If no response is received before the timeout expires, the request fails and returns an error. Defaults to `30s`.





