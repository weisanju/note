## Frozen Indices

经常搜索的索引保存在内存中，因为需要时间来重建它们并帮助进行有效的搜索。

另一方面，可能有一些我们很少访问的索引。这些索引不需要占用内存，可以在需要时重新构建。这种索引被称为冻结索引。



Elasticsearch在每次搜索该分片时都会构建冻结索引的每个分片的瞬态数据结构，并在搜索完成后立即丢弃这些数据结构。

由于Elasticsearch不会在内存中维护这些瞬态数据结构，因此冻结的索引比正常索引消耗的堆要少得多。

This allows for a much higher disk-to-heap ratio than would otherwise be possible.



### Example for Freezing and Unfreezing

```
POST /index_name/_freeze
POST /index_name/_unfreeze
```

冻结索引的搜索预计会缓慢执行。

冻结索引不适合高搜索负载。

冻结索引的搜索可能需要几秒钟或几分钟才能完成，即使在索引未冻结的情况下以毫秒完成相同的搜索。

冷冻后，不能写入



### Searching a Frozen Index

每个节点并发加载的冻结索引的数量受search_throttled threadpool中的线程数量的限制，默认情况下为1。要包含冻结索引，必须使用查询参数 − ignore_throttled = false执行搜索请求。

```
GET /index_name/_search?q=user:tpoint&ignore_throttled=false
```



### Monitoring Frozen Indices

```
GET /_cat/indices/index_name?v&h=i,sth
```