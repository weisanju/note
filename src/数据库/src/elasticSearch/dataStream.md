### 疑问

-  	没有 data stream 的时候，如何管理时序型数据？ 	
-  	什么是 data stream？ 	
-  	data stream 的特点有哪些？ 	
-  	为什么要有 data stream？ 	
-  	data stream 能做什么？ 	
-  	data stream 应用场景？ 	
-  	data stream 和 索引 index 的关系？ 	
-  	data stream 和 索引生命周期管理 ILM 的关系？ 	
-  	data stream 实操有哪些注意事项？

### 没有 data stream 的时候，如何管理时序型数据？

#### 基于 rollover 滚动索引机制管理时序数据

```
PUT mylogs-2021.07.24-1
{
  "aliases": {
    "mylogs_write": {}
  }
}

GET mylogs-2021.07.24-1

PUT mylogs_write/_doc/1
{
  "message": "a dummy log"
}

POST mylogs_write/_bulk
{"index":{"_id":4}}
{"title":"test 04"}
{"index":{"_id":2}}
{"title":"test 02"}
{"index":{"_id":3}}
{"title":"test 03"}


POST  mylogs_write/_rollover
{
  "conditions": {
    "max_docs":   3
  }
}


再次导入批量数据

POST mylogs_write/_doc/14
{"title":"test 14"}

POST mylogs_write/_bulk
{"index":{"_id":5}}
{"title":"test 05"}
{"index":{"_id":6}}
{"title":"test 06"}
{"index":{"_id":7}}
{"title":"test 07"}
{"index":{"_id":8}}
{"title":"test 08"}
{"index":{"_id":9}}
{"title":"test 09"}
{"index":{"_id":10}}
{"title":"test 10"}
```

早期生产环境使用 rollover，有个比较**麻烦**的地方就在于——需要自己结合滚动的三条件，在给定的时间点（比如凌晨0:00）定时脚本执行一下 rollover，滚动才能生效。



看似脚本处理很简单，实际会有这样那样的问题，用过你就知道有多苦。

- rollover 优点：实现了最原始的索引滚动。 	
-  rollover 缺点：需要手动或者脚本定时 rollover 非常麻烦。 	



###  ILM 索引生命周期管理时序数据

ILM 是模板、别名、生命周期 policy 的综合体。

 	 	 	

-  	ILM 优点：一次配置，索引生命周期全自动化。 	
-  	ILM 适用场景：更适合和冷热集群架构结合的业务场景。 	
-  	ILM 缺点：ILM是普适的概念，强调大而全，不是专门针对时序数据特点的方案，且需要为 ilm 配置 index.lifecycle.rollover_alias 设置（对时序数据场景，这非常麻烦）。 	



官方强调：别名在 Elasticsearch 中的实现方式存在一些不足（官方没有细说哪些不足。我实战环境发现：一个别名对应多个索引，一个索引对应多个别名，索引滚动关联别名也可能滚动，开发者可能很容易出错和混淆），使用起来很混乱。





相比于别名具有广泛的用途，而数据流将是针对时序数据的解决方案。



### 什么是 data stream？

存储时序数据的多个索引的抽象集合，简称为：数据流（data stream）

数据流可以跨多个后备索引存储仅追加（append-only，下文有详细解释）的时间序列数据，同时对外提供一个同一访问入口。

所以，它是索引、模板、rollover、ilm 基于时序性数据的综合产物。



### data stream 的特点有哪些？

#### 关联后备支撑索引（backing indices）

#### @timestamp 字段不可缺

- 每个写入到 dataSteam 的文档必须包含 @timestamp 字段。 	
- @timestamp 字段必须是：date 类型（若不指定，默认：date 类型）或者 date_nanos 类型。 	

#### data stream 后备索引规范

`.ds-<data-stream>-<yyyy.MM.dd>-<generation>`

举例索引真实名称：data-stream-2021.07.25-000001。

-  	.ds：前缀开头不可少。 	
-  	data-stream： 自定义的数据流的名称。 	
-  	yyyy.MM.dd：日期格式 	
-  	generation：rollover 累积值：—— 默认从：000001 开始。 	



#### Append-only 仅追加

仅追加：指只支持 **op_type=create** 的索引请求，我理解的是仅支持向后追加（区别于对历史数据的删除、更新操作）。



数据流只支持：update_by_query 和 delete_by_query 实现批量操作，单条文档的更新和删除操作只能通过指定后备索引的方式实现。



对于频繁更新或者删除文档的业务场景，用 data stream 不合适，而相反的，使用：模板+别名+ILM更为合适。



### 为什么要有 data stream？

原有实现由于别名的缺陷实现不了时序数据的管理或实现起来会繁琐、麻烦，data stream 是更为纯粹的存储仅追加时序数据的方式。



### data stream 能做什么？

-  	data stream 支持直接的写入、查询请求。 	
-  	data stream 会自动将客户端请求路由至关联索引，以用来存储流式数据。 	
-  	可以使用索引生命周期管理 ILM 自动管理这些关联索引。 	

### data stream 的适用场景

日志（logs）、事件（events）、指标（metrics）和其他持续生成的数据。



### data stream 和 模板的关系？

相同的索引模板可以用来支撑多个 data streams。可以类比为：1：N 关系。





### data stream  和 ilm 的关系？

ILM 在 data stream 中起到索引生命周期管理的作用。



data stream 操作时序数据优势体现在：不再需要为 ilm 配置 index.lifecycle.rollover_alias。



### data stream 实操指南

#### 创建索引生命周期 policy。

```

PUT _ilm/policy/my-lifecycle-policy
{
  "policy": {
    "phases": {
      "hot": {
        "actions": {
          "rollover": {
            "max_primary_shard_size": "50gb",
            "max_docs": 10
          }
        }
      },
      "warm": {
        "min_age": "1m",
        "actions": {
          "shrink": {
            "number_of_shards": 1
          },
          "forcemerge": {
            "max_num_segments": 1
          },
          "allocate": {
            "include": {
              "nodeType":"warm"
            }
          }
        }
      },
      "cold": {
        "min_age": "2m",
        "actions": {
          "allocate": {
            "include": {
              "nodeType":"cold"
            }
          }
        }
      },
      "delete": {
        "min_age": "4m",
        "actions": {
          "delete": {}
        }
      }
    }
  }
}

```

#### 创建模板

```
PUT _component_template/my-mappings
{
  "template": {
    "mappings": {
      "properties": {
        "@timestamp": {
          "type": "date",
          "format": "date_optional_time||epoch_millis"
        },
        "message": {
          "type": "wildcard"
        }
      }
    }
  }
}

# Creates a component template for index settings
PUT _component_template/my-settings
{
  "template": {
    "settings": {
      "index.lifecycle.name": "my-lifecycle-policy"
    }
  }
}


PUT _index_template/my-index-template
{
  "index_patterns": ["my-data-stream*"],
  "data_stream": { },
  "composed_of": [ "my-mappings", "my-settings" ],
  "priority": 500
}
```

### 创建 data stream

- 方式一：直接创建数据流 my-data-stream。 	

`PUT _data_stream/my-data-stream` 

- 方式二：直接批量或者逐个导入数据（会间接生成 data stream 的创建）。 	

```
PUT my-data-stream/_bulk
{ "create":{ } }
{ "@timestamp": "2099-05-06T16:21:15.000Z", "message": "192.0.2.42 - - [06/May/2099:16:21:15 +0000] \"GET /images/bg.jpg HTTP/1.0\" 200 24736" }
{ "create":{ } }
{ "@timestamp": "2099-05-06T16:25:42.000Z", "message": "192.0.2.255 - - [06/May/2099:16:25:42 +0000] \"GET /favicon.ico HTTP/1.0\" 200 3638" }

POST my-data-stream/_doc
{
  "@timestamp": "2099-05-06T16:21:15.000Z",
  "message": "192.0.2.42 - - [06/May/2099:16:21:15 +0000] \"GET /images/bg.jpg HTTP/1.0\" 200 24736"
}
```

- 第一：批量 bulk 操作，必须使用：create 指令，而非 index（使用 index 不会报错， 会把流当做索引处理了）。 	
- 第二：文档必须包含：@timestamp  时间戳字段。 	

如果不包含 @timestamp 会报错如下：

 "reason" : "data stream timestamp field [@timestamp] is missing" 



### data stream 删

```
DELETE _data_stream/my-data-stream
```

### 单条删除文档

```
DELETE data-stream-2021.07.25-000001/_doc/1 
```

### 批量删除文档

```
POST /my-data-stream/_delete_by_query
{
  "query": {
    "match": {
      "user.id": "vlb44hny"
    }
  }
}
```

### data stream 改

```
# 插入一条数据
POST my-data-stream/_bulk
{"create":{"_id":1}}
{"@timestamp":"2099-05-06T16:21:15.000Z","message":"192.0.2.42 - - [06/May/2099:16:21:15 +0000] \"GET /images/bg.jpg HTTP/1.0\" 200 24736"}

# 获取数据流关联索引
GET /_data_stream/my-data-stream

# 执行更新
PUT .ds-my-data-stream-2021.07.25-000001/_doc/1?if_seq_no=1&if_primary_term=1
{
  "@timestamp": "2099-03-08T11:06:07.000Z",
  "user": {
    "id": "8a4f500d"
  },
  "message": "Login successful"
}

# 查看验证是否已经更新（已经验证，可以更新）
GET .ds-my-data-stream-2021.07.25-000001/_doc/1
```

### 批量更新

```
POST /my-data-stream/_update_by_query
{
  "query": {
    "match": {
      "user.id": "l7gk7f82"
    }
  },
  "script": {
    "source": "ctx._source.user.id = params.new_id",
    "params": {
      "new_id": "XgdX0NoX"
    }
  }
}
```

### data stream 查

`GET _data_stream/my-data-stream` 

```
{
  "data_streams" : [
    {
      "name" : "my-data-stream",
      "timestamp_field" : {
        "name" : "@timestamp"
      },
      "indices" : [
        {
          "index_name" : ".ds-my-data-stream-2021.07.25-000001",
          "index_uuid" : "Akg3-bWgStiKG_39Tk5PRw"
        }
      ],
      "generation" : 1,
      "status" : "GREEN",
      "template" : "my-index-template",
      "ilm_policy" : "my-lifecycle-policy",
      "hidden" : false
    }
  ]
}
```

### reindex 操作

```
POST /_reindex
{
  "source": {
    "index": "archive"
  },
  "dest": {
    "index": "my-data-stream",
    "op_type": "create"
  }
}
```

### 滚动操作

```
POST my-data-stream/_rollover 
```

### 查看 data stream 基础信息

```
GET /_data_stream/my-data-stream 
```





## Data streams

1. 数据流使您可以跨多个索引存储仅追加的时间序列数据，同时为您提供用于请求的单个命名资源。数据流非常适合日志、事件、度量和其他连续生成的数据。

2. 您可以直接向数据流提交索引和搜索请求

3. 流自动将请求路由到存储流数据的支持索引。

4. 您可以使用索引生命周期管理 (ILM) 来自动化这些支持索引的管理。

5. 例如，您可以使用ILM自动将较旧的备份索引移动到较便宜的硬件并删除不需要的索引。随着数据的增长，ILM可以帮助您降低成本和开销。



### Backing indices

1. 数据流由一个或多个[隐藏](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/data-streams.html#backing-indices:~:text=one%20or%20more-,hidden,-%2C%20auto%2Dgenerated%20backing)的自动生成的后备索引组成。
2. 数据流需要匹配一个[索引模板](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-templates.html)。模板包含用于配置 后备索引的mappings and settings 。
3. 索引到数据流的每个文档都必须包含一个 @ timestamp字段，映射为日期或date_nanos字段类型。如果索引模板没有为 @ timestamp字段指定映射，Elasticsearch将 @ timestamp映射为具有默认选项的日期字段。
4. 相同的索引模板可以用于多个数据流。不能删除数据流使用的索引模板。



### Read requests

当您向数据流提交读取请求时，该流将请求路由到其所有的后备索引。

### Write index

最近创建的备份索引是数据流的写入索引。流仅将新文档添加到此索引。

即使直接向索引发送请求，也无法将新文档添加到其他支持索引。



您也不能对可能阻碍索引的写索引执行操作，例如:·

- [Clone](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-clone-index.html)
- [Delete](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-delete-index.html)
- [Freeze](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/freeze-index-api.html)
- [Shrink](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-shrink-index.html)
- [Split](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-split-index.html)



### Rollover

1. A [rollover](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-rollover-index.html) creates a new backing index that becomes the stream’s new write index.

2. We recommend using [ILM](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-lifecycle-management.html) to automatically roll over data streams when the write index reaches a specified age or size. 

If needed, you can also [manually roll over](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/use-a-data-stream.html#manually-roll-over-a-data-stream) a data stream.

### Generation

每个数据流跟踪其生成: 一个六位数、零填充的整数，作为流的滚动的累积计数，从000001开始。

创建支持索引时，索引将使用以下约定命名:

```
.ds-<data-stream>-<yyyy.MM.dd>-<generation>
```

<yyyy.MM.dd> 是支持索引的创建日期。具有较高的代的支持索引包含较新的数据。例如， `web-server-logs` 数据流具有生成34。2099年3月7日创建的流的最新后备索引被命名。`ds-web-server-logs-2099.03.07-000034。`

某些操作 (例如收缩或还原) 可以更改支持索引的名称。这些名称更改不会从其数据流中删除后备索引。

### Append-only

Data streams are designed for use cases where existing data is rarely, if ever, updated. You cannot send update or deletion requests for existing documents directly to a data stream. Instead, use the [update by query](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/use-a-data-stream.html#update-docs-in-a-data-stream-by-query) and [delete by query](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/use-a-data-stream.html#delete-docs-in-a-data-stream-by-query) APIs.

If needed, you can [update or delete documents](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/use-a-data-stream.html#update-delete-docs-in-a-backing-index) by submitting requests directly to the document’s backing index.

**注意**

If you frequently update or delete existing documents, use an [index alias](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/indices-add-alias.html) and [index template](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-templates.html) instead of a data stream. You can still use [ILM](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/index-lifecycle-management.html) to manage indices for the alias.

