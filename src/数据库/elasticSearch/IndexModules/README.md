# Index modules

索引模块是每个索引创建的模块，可控制与索引相关的所有方面。



## 先验知识

1. [closed index](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-open-close.html) 
2. 索引拆分 [split](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-split-index.html)  
3.  [routing field](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/mapping-routing-field.html)  
4.  [cached filters](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/query-filter-context.html)  
5.  [allocation filtering](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/shard-allocation-filtering.html) 
6.  [total shards per node](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/allocation-total-shards.html)

## Index Settings

索引级别的设置可能是:

- static

  它们只能在索引创建时或在 [closed index](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-open-close.html) 索引上设置。

- dynamic

  可以通过 [update-index-settings](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-update-settings.html) API 改变的设置

更改已关闭索引上的静态或动态索引设置可能会导致不正确的设置，而如果不删除和重新创建索引，则无法纠正这些设置。

### Static index settings

以下是索引通用的静态设置的列表:

- **`index.number_of_shards`**

  1. 索引应具有的主分片数量
  2. 默认1。只能在索引创建时设置。可以作用于closed index
  3. 每个索引的主分片数最多 `1024` 
  4. 此限制是一个安全限制，以防止意外创建索引，这些索引会由于资源分配而使群集不稳定。
  5. 这个限制可以通过  `export ES_JAVA_OPTS="-Des.index.max_number_of_shards=128"` 系统变量修改（每个节点都要修改）

  

- **`index.number_of_routing_shards`**

  1. 用于拆分 [split](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-split-index.html)  索引的路由分片数

  2. 五分片的索引，设置  number_of_routing_shards 为 30 （5x2x3）

  3. 换句话说，

     ```
     `5` → `10` → `30`
     
     `5` → `15` → `30` 
     
     `5` → `30`
     ```

  4. 此设置的默认值取决于索引中主分片的数量。
  5. 默认值旨在允许您按2的因子拆分，最多1024个分片
  6. 在Elasticsearch 7.0.0及更高版本中，此设置会影响文档在各个分片之间的分布方式
  7. 使用自定义路由重新索引较旧的索引时，您必须显式设置*index.number_of_routing_shards*以保持相同的文档分布 See the [related breaking change](https://www.elastic.co/guide/en/elasticsearch/reference/7.0/breaking-changes-7.0.html#_document_distribution_changes).

- **`index.shard.check_on_startup`**

  Whether or not shards should be checked for corruption before opening.

  1.  打开分片前 是否检查 分片是否损坏。如果一损坏则不会打开
  2. 默认，false。不检查
  3. checksum：检查物理 校验和
  4. true： 既检查物理也检查逻辑损坏。这个很耗CPU跟内存、仅限专家。在大型索引上检查分片可能需要大量时间。

- **`index.codec`**

  1. 默认值使用LZ4压缩来压缩存储的数据，
  2. 但这可以设置为*best_compression*，它使用 [DEFLATE](https://en.wikipedia.org/wiki/DEFLATE) 来获得更高的压缩比，但以较慢的存储字段性能为代价。
  3. 如果要更新compression type，则在合并段后将应用新的压缩类型
  4. Segment merging can be forced using [force merge](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-forcemerge.html).

  

- **`index.routing_partition_size`**

  The number of shards a custom [routing](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/mapping-routing-field.html) value can go to. 

  1. 自定义路由值。可以 [routing](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/mapping-routing-field.html)  的分片数。
  2. 默认为1，只能在索引创建时设置。
  3. 必须小于  `index.number_of_shards` 。除非  `index.number_of_shards`设置为1 
  4. 详见： [Routing to an index partition](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/mapping-routing-field.html#routing-index-partition) 

- **`index.soft_deletes.enabled`**
  1. [7.6.0] Deprecated in 7.6.0. 
  2. 不建议使用禁用软删除的创建索引，并且将在以后的Elasticsearch版本中删除。
  3. 指示是否在索引上启用了软删除
  4. 软删除只能在索引创建时配置，并且只能在Elasticsearch 6.5.0上或之后创建的索引上配置。默认为true。



- **`index.soft_deletes.retention_lease.period`**
  1. 保留分片历史记录的最长时间
  2. 碎片历史记录保留租约确保在合并Lucene索引期间保留软删除。
  3. 如果软删除在可以复制到flollower 之前被合并了
  4. 以下过程将由于 leader 的历史记录不完整而失败. 
  5. Defaults to `12h`
- **`index.load_fixed_bitset_filters_eagerly`**
  1. load_fixed_bitset_filters_eagerly
  2. 是否对 嵌套查询 预加载  [cached filters](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/query-filter-context.html)  
  3. 默认FALSE
- **`index.hidden`**
  1. 指示默认情况下是否应该隐藏索引
  2. 使用通配符表达式时，默认情况下不返回隐藏索引。
  3. 通过使用*expand_wildcards*参数来控制此行为。
  4. 默认FALSE

### Dynamic index settings

以下是与任何特定索引模块不关联的所有动态索引设置



- **`index.number_of_replicas`**
  1. 每个主分片的副本数。默认为1。
- **`index.auto_expand_replicas`**
  1. 根据集群中的数据节点数自动扩展副本数
  2. 设置为限定下限和上限的破折号 (例如. `0-5`) or use `all` for the upper bound (e.g. `0-all`). 
  3. 默认FALSE禁用。
  4. 分片自动扩展数量 只 会考虑  [allocation filtering](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/shard-allocation-filtering.html)  会忽略  [total shards per node](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/allocation-total-shards.html), 如果适用的规则阻止分配所有副本，则这可能导致群集运行状况变为黄色。
  5. If the upper bound is `all` then [shard allocation awareness](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/modules-cluster.html#shard-allocation-awareness) and [`cluster.routing.allocation.same_shard.host`](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/modules-cluster.html#cluster-routing-allocation-same-shard-host) are ignored for this index.
- **`index.search.idle.after`**
  1. 在被认为搜索空闲之前，分片无法接收搜索或获取请求多长时间。(默认为30秒)



- **`index.refresh_interval`**
  1. 执行刷新操作的频率，这使索引的最新更改对搜索可见
  2. 默认1s。-1 则是禁止刷新
  3. 如果未明确设置此设置，则至少在*index.search.idle*秒前。没有看到搜索流量的分片，在收到搜索请求之前，它们将不会收到后台刷新。
  4. 命中空闲且刷新被阻塞的分片的搜索请求。将会在下次后台刷新（1s）
  5. 此行为旨在在不执行搜索的默认情况下自动优化批量索引。
  6. 为了选择退出此行为，应将此值显示的设置为 1



- **`index.max_result_window`**

  

  1. from+size 搜索的最大大小
  2. 默认1w
  3. 搜索请求 消耗大量堆、跟时间
  4. 详见： [Scroll](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/paginate-search-results.html#scroll-search-results) or [Search After](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/paginate-search-results.html#search-after) 

- **`index.max_inner_result_window`**

  1. The maximum value of `from + size` for inner hits definition and top hits aggregations to this index. 
  2. 分片内部返回的最大值
  3. 默认100。

  

- **`index.max_rescore_window`**

  1. The maximum value of `window_size` for `rescore` requests in searches of this index.
  2. . Defaults to `index.max_result_window` which defaults to `10000`. 
  3. Search requests take heap memory and time proportional to `max(window_size, from + size)` and this limits that memory.

- **`index.max_docvalue_fields_search`**

  1. The maximum number of `docvalue_fields` that are allowed in a query. 
  2. Defaults to `100`. Doc-value fields are costly since they might incur a per-field per-document seek.

- **`index.max_script_fields`**

  The maximum number of `script_fields` that are allowed in a query. Defaults to `32`.

- **`index.max_ngram_diff`**

  

  对于NGramTokenizer和NGramTokenFilter，min_gram和max_gram之间的最大允许差。默认为1。

- **`index.max_shingle_diff`**

  The maximum allowed difference between max_shingle_size and min_shingle_size for the [`shingle` token filter](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/analysis-shingle-tokenfilter.html). Defaults to `3`.

- **`index.max_refresh_listeners`**

  Maximum number of refresh listeners available on each shard of the index. These listeners are used to implement [`refresh=wait_for`](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/docs-refresh.html).

- **`index.analyze.max_token_count`**

  The maximum number of tokens that can be produced using _analyze API. Defaults to `10000`.

- **`index.highlight.max_analyzed_offset`**

  The maximum number of characters that will be analyzed for a highlight request. This setting is only applicable when highlighting is requested on a text that was indexed without offsets or term vectors. Defaults to `1000000`.



- **`index.max_terms_count`**

  The maximum number of terms that can be used in Terms Query. Defaults to `65536`.



- **`index.max_regex_length`**

  The maximum length of regex that can be used in Regexp Query. Defaults to `1000`.

- **`index.query.default_field`**

  (string or array of strings) Wildcard (`*`) patterns matching one or more fields. The following query types search these matching fields by default:[More like this](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/query-dsl-mlt-query.html)[Multi-match](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/query-dsl-multi-match-query.html)[Query string](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/query-dsl-query-string-query.html)[Simple query string](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/query-dsl-simple-query-string-query.html)Defaults to `*`, which matches all fields eligible for [term-level queries](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/term-level-queries.html), excluding metadata fields.

- **`index.routing.allocation.enable`**

  1. 控制此索引的分片分配。
  2. It can be set to:`all` (default) - 允许所有分片的分片分配。
  3. `primaries` -仅允许主分片分配.
  4. `new_primaries` - 只允许新创建的主分片分配.
  5. `none` - 不允许分片分配。

- **`index.routing.rebalance.enable`**

  1. 启用此索引的分片再平衡
  2.  It can be set to:`all` (default) - Allows shard rebalancing for all shards.
  3. `primaries` - Allows shard rebalancing only for primary shards.
  4. `replicas` - Allows shard rebalancing only for replica shards.
  5. `none` - No shard rebalancing is allowed.

- **`index.gc_deletes`**

  The length of time that a [deleted document’s version number](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/docs-delete.html#delete-versioning) remains available for [further versioned operations](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/docs-index_.html#index-versioning). Defaults to `60s`.

- **`index.default_pipeline`**
  1. The default [ingest node](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/ingest.html) pipeline for this index. 
  2. Index requests will fail if the default pipeline is set and the pipeline does not exist. 
  3. The default may be overridden using the `pipeline` parameter. 
  4. The special pipeline name `_none` indicates no ingest pipeline should be run.

- **`index.final_pipeline`**
  1. The final [ingest node](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/ingest.html) pipeline for this index. 
  2. Indexing requests will fail if the final pipeline is set and the pipeline does not exist. 
  3. The final pipeline always runs after the request pipeline (if specified) and the default pipeline (if it exists). 
  4. The special pipeline name `_none` indicates no ingest pipeline will run.
  5. You can’t use a final pipelines to change the `_index` field. If the pipeline attempts to change the `_index` field, the indexing request will fail.

### Settings in other index modules

Other index settings are available in index modules:

- **[Analysis](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/analysis.html)**

  Settings to define analyzers, tokenizers, token filters and character filters.

- **[Index shard allocation](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-modules-allocation.html)**

  Control over where, when, and how shards are allocated to nodes.

- **[Mapping](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-modules-mapper.html)**

  Enable or disable dynamic mapping for an index.

- **[Merging](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-modules-merge.html)**

  Control over how shards are merged by the background merge process.

- **[Similarities](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-modules-similarity.html)**

  Configure custom similarity settings to customize how search results are scored.

- **[Slowlog](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-modules-slowlog.html)**

  Control over how slow queries and fetch requests are logged.

- **[Store](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-modules-store.html)**

  Configure the type of filesystem used to access shard data.

- **[Translog](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-modules-translog.html)**

  Control over the transaction log and background flush operations.

- **[History retention](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-modules-history-retention.html)**

  Control over the retention of a history of operations in the index.

- **[Indexing pressure](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-modules-indexing-pressure.html)**

  Configure indexing back pressure limits.

### X-Pack index settings

- **[Index lifecycle management](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/ilm-settings.html)**

  Specify the lifecycle policy and rollover alias for an index.





