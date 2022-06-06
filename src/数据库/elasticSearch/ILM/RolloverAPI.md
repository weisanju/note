## Rollover API

Creates a new index for a [data stream](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/data-streams.html) or [index alias](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-add-alias.html).

### Request

```
POST /<rollover-target>/_rollover/
POST /<rollover-target>/_rollover/<target-index>
```





### Description

推荐使用 ILM’s [`rollover`](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/ilm-rollover.html) action to automate rollovers. See [Index lifecycle](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/ilm-index-lifecycle.html).

rollover API为数据流或索引别名创建新索引。API的行为取决于翻转目标。



**Roll over a data stream**

If you roll over a data stream, the API creates a new write index for the stream. The stream’s previous write index becomes a regular backing index. A rollover also increments the data stream’s generation. See [Rollover](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/data-streams.html#data-streams-rollover).



**Roll over an index alias with a write index**

Prior to Elasticsearch 7.9, you would typically use an [index alias](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-aliases.html) with a write index to manage time series data. Data streams replace this functionality, require less maintenance, and automatically integrate with [data tiers](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/data-tiers.html).

See [Convert an index alias to a data stream](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/set-up-a-data-stream.html#convert-index-alias-to-data-stream).



1. 如果索引别名指向多个索引。则 其中一个必须是  [write index](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-aliases.html#write-index).
2.  rollover API  为 索引别名  创建新索引 。并设置  `is_write_index` = `true`
3. 同样给之前的 索引 设置  `is_write_index`  = false



**Roll over an index alias with one index**

如果滚动仅指向一个索引的索引别名，则API会为该别名创建一个新索引，并从该别名中删除原始索引。

#### Increment index names for an alias

1. 滚动索引时可以执行索引名
2. 如果 没有指定索引名，且当前索引以  ` -number` 结尾 例如  `my-index-000001` or `my-index-3`, 则number递增
3. 这个 数字 是填充0的六位数

**Use date math with index alias rollovers**

1. 可以在 time series data  中 是会用   [date math](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/date-math-index-names.html) 
2.  `<my-index-{now/d}-000001>`  see [Roll over an index alias with a write index](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-rollover-index.html#roll-over-index-alias-with-write-index).



#### Wait for active shards

A rollover creates a new index and is subject to the [`wait_for_active_shards`](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-create-index.html#create-index-wait-for-active-shards) setting.



### Path parameters

- **`<rollover-target>`**

  (Required, string) Name of the data stream or index alias to roll over.

- **`<target-index>`**

  (Optional, string) Name of the index to create. Supports [date math](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/date-math-index-names.html).

  1.  Data streams 不支持
  2. 如果名称没有 以 -number 结尾 则 必须要指定该值
  3. 只能小写，不允许  `\`, `/`, `*`, `?` `"`, `<`, `>`, `|`,  (space character), `,`, `#`:
  4. 不能超过255个字节
  5. starting with `.` are deprecated, except for [hidden indices](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-modules.html#index-hidden) and internal indices managed by plugins

### Query parameters

**`dry_run`**

(Optional, Boolean) If `true`, checks whether the current index matches one or more specified `conditions` but does not perform a rollover. Defaults to `false`.

**`wait_for_active_shards`**

(Optional, string) The number of shard copies that must be active before proceeding with the operation. Set to `all` or any positive integer up to the total number of shards in the index (`number_of_replicas+1`). Default: 1, the primary shard.

See [Active shards](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/docs-index_.html#index-wait-for-active-shards).

**`master_timeout`**

(Optional, [time units](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#time-units)) Period to wait for a connection to the master node. If no response is received before the timeout expires, the request fails and returns an error. Defaults to `30s`.

**`timeout`**

(Optional, [time units](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#time-units)) Period to wait for a response. If no response is received before the timeout expires, the request fails and returns an error. Defaults to `30s`.



### Request body

#### **`aliases`**

别名对象

#### Properties of aliases objects

**`filter`**

(Optional, [Query DSL object](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/query-dsl.html)) Query used to limit documents the alias can access.

**`index_routing`**

(Optional, string) Value used to route indexing operations to a specific shard. If specified, this overwrites the `routing` value for indexing operations.

**`is_hidden`**

(Optional, Boolean) If `true`, the alias is [hidden](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/multi-index.html#hidden). Defaults to `false`. All indices for the alias must have the same `is_hidden` value.

**`is_write_index`**

(Optional, Boolean) If `true`, the index is the [write index](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-aliases.html#write-index) for the alias. Defaults to `false`.

**`routing`**

(Optional, string) Value used to route indexing and search operations to a specific shard.

**`search_routing`**

(Optional, string) Value used to route search operations to a specific shard. If specified, this overwrites the `routing` value for search operations.

#### **`conditions`**

1. 只当满足 至少一个或者多个条件才 rollover
2. 如果未指定此参数，则Elasticsearch会无条件执行翻转。

3. 要翻转索引，在请求的时刻当前索引必须满足 条件
4. es不会 监控 索引状态
5. To automate rollover, use ILM’s [`rollover`](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/ilm-rollover.html) instead.

#### Properties of conditions

**`max_age`**

(Optional, [time units](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#time-units)) 

1. 至 索引从创建时间以来经历的最大时间
2. even if the index origination date is configured to a custom date, such as when using the [index.lifecycle.parse_origination_date](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/ilm-settings.html#index-lifecycle-parse-origination-date) or [index.lifecycle.origination_date](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/ilm-settings.html#index-lifecycle-origination-date) settings.

**`max_docs`**

(Optional, integer) Triggers rollover after the specified maximum number of documents is reached. Documents added since the last refresh are not included in the document count. The document count does **not** include documents in replica shards.

**`max_size`**

(Optional, [byte units](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#byte-units)) Triggers rollover when the index reaches a certain size. This is the total size of all primary shards in the index. Replicas are not counted toward the maximum index size.

To see the current index size, use the [_cat indices](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/cat-indices.html) API. The `pri.store.size` value shows the combined size of all primary shards.

**`max_primary_shard_size`**

(Optional, [byte units](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#byte-units)) Triggers rollover when the largest primary shard in the index reaches a certain size. This is the maximum size of the primary shards in the index. As with `max_size`, replicas are ignored.

To see the current shard size, use the [_cat shards](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/cat-shards.html) API. The `store` value shows the size each shard, and `prirep` indicates whether a shard is a primary (`p`) or a replica (`r`).



#### **`mappings`**

(Optional, [mapping object](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/mapping.html)) Mapping for fields in the index. If specified, this mapping can include:

- Field names
- [Field data types](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/mapping-types.html)
- [Mapping parameters](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/mapping-params.html)

See [Mapping](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/mapping.html).

Data streams do not support this parameter.

#### **`settings`**

(Optional, [index setting object](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-modules.html#index-modules-settings)) Configuration options for the index. See [Index Settings](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-modules.html#index-modules-settings).

Data streams do not support this parameter.



### Response body

**`acknowledged`**

(Boolean) If `true`, the request received a response from the master node within the `timeout` period.

**`shards_acknowledged`**

(Boolean) If `true`, the request received a response from [active shards](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/docs-index_.html#index-wait-for-active-shards) within the `master_timeout` period.

**`old_index`**

(string) Previous index for the data stream or index alias. For data streams and index aliases with a write index, this is the previous write index.

**`new_index`**

(string) Index created by the rollover. For data streams and index aliases with a write index, this is the current write index.

**`rolled_over`**

(Boolean) If `true`, the data stream or index alias rolled over.

**`dry_run`**

(Boolean) If `true`, Elasticsearch did not perform the rollover.

**`condition`**

(object) Result of each condition specified in the request’s `conditions`. If no conditions were specified, this is an empty object.

(Boolean) The key is each condition. The value is its result. If `true`, the index met the condition at rollover.



### Examples

#### Roll over a data stream

```console
POST my-data-stream/_rollover


POST my-data-stream/_rollover
{
  "conditions": {
    "max_age": "7d",
    "max_docs": 1000,
    "max_primary_shard_size": "50gb"
  }
}

```



#### **按日志rollover 并设置写索引**

```console
PUT %3Cmy-index-%7Bnow%2Fd%7D-000001%3E
{
  "aliases": {
    "my-alias": {
      "is_write_index": true
    }
  }
}
```



如果别名的索引名称使用date math，并且您定期滚动索引，则可以使用日期数学来缩小搜索范围。例如，以下搜索目标是最近三天创建的索引。

```console
# GET /<my-index-{now/d}-*>,<my-index-{now/d-1d}-*>,<my-index-{now/d-2d}-*>/_search
GET /%3Cmy-index-%7Bnow%2Fd%7D-*%3E%2C%3Cmy-index-%7Bnow%2Fd-1d%7D-*%3E%2C%3Cmy-index-%7Bnow%2Fd-2d%7D-*%3E/_search
```

#### Roll over an index alias with one index

```console
# PUT <my-index-{now/d}-000001>
PUT %3Cmy-index-%7Bnow%2Fd%7D-000001%3E
{
  "aliases": {
    "my-write-alias": { }
  }
}
```

#### Specify settings during a rollover

```console
POST my-alias/_rollover
{
  "settings": {
    "index.number_of_shards": 2
  }
}
```