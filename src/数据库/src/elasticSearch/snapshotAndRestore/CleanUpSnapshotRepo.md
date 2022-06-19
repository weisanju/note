## Clean up snapshot repository API

触发对快照存储库内容的审查，并删除现有快照未引用的所有陈旧数据。

```
POST /_snapshot/my_repository/_cleanup
```



### Request

```
POST /_snapshot/<repository>/_cleanup
```





### Description

随着时间的推移，快照存储库可能会累积不再被现有快照引用的陈旧数据。

尽管此未引用的数据不会对快照存储库的性能或安全性产生负面影响，但它可能导致比必要时更多的存储使用。

您可以使用 “清理快照存储库” API来检测和删除此未引用的数据。

小提示

1. 当从存储库中删除快照时，此API执行的大多数清理操作都是自动执行的。
2. 如果您定期删除快照，调用此API可能只会稍微减少您的存储空间，或者根本不会减少。



### Path parameters

**`<repository>`**

(Required, string) Name of the snapshot repository to review and clean up.





### Query parameters

- **`master_timeout`**

  (Optional, [time units](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#time-units)) Period to wait for a connection to the master node. If no response is received before the timeout expires, the request fails and returns an error. Defaults to `30s`.

- **`timeout`**

  (Optional, [time units](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#time-units)) Period to wait for a response. If no response is received before the timeout expires, the request fails and returns an error. Defaults to `30s`.

### Response body

#### **results**

##### **deleted_bytes**

(整数) 通过清理操作释放的字节数。

**deleted_blobs**

(整数) 在清理操作期间从快照存储库中删除的二进制大对象 (blob) 的数量。任何非零值都意味着发现了未引用的blobs并随后进行了清理。











