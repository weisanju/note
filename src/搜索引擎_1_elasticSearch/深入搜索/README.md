# 深入搜索

### **结构化查找**

term查询

### **全文搜索**

* 两个重要概念
* 两种类别的查询
* 通过 *bool* 组合查询

### **多字段搜索**

* 多字符串搜索
* 单字符串搜索
  * 字段中心式
    * 最佳字段
    * 多数字段
  * 词中心式
    * 跨字段

### **近视匹配**

`match` 查询可以告知我们索引中是否包含查询的词条，但却无法告知词语之间的关系。

#### 短语匹配

* 词项之间的距离 是紧挨着的
* *slop*参数可以指定 词项之间的距离
* 短语匹配 评分标准：越近越好
* 使用临近查询 一般用来提高相关度：因为有时候，用户只需要包含3个词项，但是匹配的词项越多越好。

#### 数组类型的短语匹配

```text
PUT /my_index/groups/1
{
    "names": [ "John Abraham", "Lincoln Smith"]
}
在分析 John Abraham 的时候， 产生了如下信息：

Position 1: john
Position 2: abraham
然后在分析 Lincoln Smith 的时候， 产生了：

Position 3: lincoln
Position 4: smith
```

#### 可以配置数组间的生长间距

```sense
PUT /my_index/_mapping/groups 
{
    "properties": {
        "names": {
            "type":                "string",
            "position_increment_gap": 100
        }
    }
}
```

#### 性能

[Lucene nightly benchmarks](http://people.apache.org/~mikemccand/lucenebench/) 表明一个简单的 `term` 查询比一个短语查询大约快 10 倍，比邻近查询(有 `slop` 的短语 查询)大约快 20 倍

那么我们应该如何限制短语查询和邻近近查询的性能消耗呢？

一种有用的方法是**减少需要通过短语查询检查的文档总数**。

结果集重新打分

```
GET /my_index/my_type/_search
{
    "query": {
        "match": {  
            "title": {
                "query":                "quick brown fox",
                "minimum_should_match": "30%"
            }
        }
    },
    "rescore": {
        "window_size": 50, 
        "query": {         
            "rescore_query": {
                "match_phrase": {
                    "title": {
                        "query": "quick brown fox",
                        "slop":  50
                    }
                }
            }
        }
    }
}
```

#### 相关词

*unigram*：唯一单元

*shingles*：多词单元

* *bigrams*：二词单元
* *trigrams*：三词单元

多字段采用不同的分析器

* 使用 普通分析器 增加 召回率：过滤文档
* 使用 *shingles*: 提高相关性：增加精确度

```
GET /my_index/my_type/_search
{
   "query": {
      "bool": {
         "must": {
            "match": {
               "title": "the hungry alligator ate sue"
            }
         },
         "should": {
            "match": {
               "title.shingles": "the hungry alligator ate sue"
            }
         }
      }
   }
}
```



**性能**

shingles 不仅比短语查询更灵活，而且性能也更好。 shingles 查询跟一个简单的 `match` 查询一样高效，而不用每次搜索花费短语查询的代价。只是在索引期间因为更多词项需要被索引会付出一些小的代价， 这也意味着有 shingles 的字段会占用更多的磁盘空间



### 部分匹配

#### 前缀查询

*依次匹配所有词项*

```
GET /my_index/address/_search
{
    "query": {
        "prefix": {
            "postcode": "W1"
        }
    }
}
```

#### 正则表达式匹配

```
GET /my_index/address/_search
{
    "query": {
        "wildcard": {
            "postcode": "W?F*HW" 
        }
    }
}
```

#### 模糊匹配的性能问题

`wildcard` 和 `regexp` 查询的工作方式与 `prefix` 查询完全一样，它们也需要扫描倒排索引中的词列表才能找到所有匹配的词，然后依次获取每个词相关的文档 ID ，与 `prefix` 查询的唯一不同是：它们能支持更为复杂的匹配模式。

这也意味着需要同样注意前缀查询存在性能问题，对有很多唯一词的字段执行这些查询可能会消耗非常多的资源，所以要避免使用左通配这样的模式匹配（如： `*foo` 或 `.*foo` 这样的正则式）。

#### 基于词项的模糊匹配

`prefix` 、 `wildcard` 和 `regexp` 查询是基于词操作的，如果用它们来查询 `analyzed` 字段，它们会检查字段里面的每个词，而不是将字段作为整体来处理。

#### 前缀查询匹配

```
{
    "match_phrase_prefix" : {
        "brand" : "johnnie walker bl"
    }
}
```

```
这种查询的行为与 match_phrase 查询一致，不同的是它将查询字符串的最后一个词作为前缀使用，换句话说，

"johnnie walker bl*"
```

**限制前缀匹配的数量**

```
{
    "match_phrase_prefix" : {
        "brand" : {
            "query":          "johnnie walker bl",
            "max_expansions": 50 //前缀匹配最多匹配50个
        }
    }
}
```

#### 索引时优化的前缀匹配

在搜索之前准备好供部分匹配的数据可以提高搜索的性能。

在索引时准备数据意味着要选择合适的分析链，这里部分匹配使用的工具是 *n-gram* 。可以将 *n-gram* 看成一个在词语上 *滑动窗口* ， *n* 代表这个 “窗口” 的长度。如果我们要 n-gram `quick` 这个词 —— 它的结果取决于 *n* 的选择长度：



- 长度 1（unigram）： [ `q`, `u`, `i`, `c`, `k` ]
- 长度 2（bigram）： [ `qu`, `ui`, `ic`, `ck` ]
- 长度 3（trigram）： [ `qui`, `uic`, `ick` ]
- 长度 4（four-gram）： [ `quic`, `uick` ]
- 长度 5（five-gram）： [ `quick` ]

我们会使用一种特殊的 n-gram 称为 *边界 n-grams* （edge n-grams）。所谓的边界 n-gram 是说它会固定词语开始的一边，以单词 `quick` 为例，它的边界 n-gram 的结果为：

- `q`
- `qu`
- `qui`
- `quic`
- `quick`

可能会注意到这与用户在搜索时输入 “quick” 的字母次序是一致的，换句话说，这种方式正好满足即时搜索（instant search）！

