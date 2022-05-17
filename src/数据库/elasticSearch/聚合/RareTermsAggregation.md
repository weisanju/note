## Rare terms aggregation

基于多桶值源的聚合，可找到 “稀有” 术语-分布的长尾且不常见的术语。从概念上讲，这就像一个按 _count升序排序的术语聚合。如术语聚合文档中所述，实际上按计数升序排序术语agg具有无界错误。相反，您应该使用rare_terms聚合

A multi-bucket value source based aggregation which finds "rare" terms —词项处于分布的长尾且不频繁出现的

从概念上来讲，这就是一个 *terms* 聚合 ，通过 *_count*  升序排序

实际上，通过 *count* 升序对术语agg进行排序具有无界错误(*unbounded error*)。相反，您应该使用*rare_terms*聚合

## Syntax

```js
{
  "rare_terms": {
    "field": "the_field",
    "max_doc_count": 1
  }
}
```

## **Parameters**

| Parameter Name  | Description                                                  | Required | Default Value |
| --------------- | ------------------------------------------------------------ | -------- | ------------- |
| `field`         | 检索的字段                                                   | Required |               |
| `max_doc_count` | *term* 出现在的  最大文档个数                                | Optional | `1`           |
| `precision`     | The precision of the internal CuckooFilters. Smaller precision leads to better approximation, but higher memory usage. Cannot be smaller than `0.00001` | Optional | `0.01`        |
| `include`       | Terms that should be included in the aggregation             | Optional |               |
| `exclude`       | Terms that should be excluded from the aggregation           | Optional |               |
| `missing`       | The value that should be used if a document does not have the field being aggregated | Optional |               |

## Example

```console
GET /_search
{
  "aggs": {
    "genres": {
      "rare_terms": {
        "field": "genre"
      }
    }
  }
}
```

```console-result
{
  ...
  "aggregations": {
    "genres": {
      "buckets": [
        {
          "key": "swing",
          "doc_count": 1
        }
      ]
    }
  }
}
```

在此示例中，我们看到的唯一存储桶是 *swing* 存储桶，因为它是一个文档中出现的唯一术语。如果我们将max_doc_count增加到2，我们将看到更多的存储桶:

```console-result
{
  ...
  "aggregations": {
    "genres": {
      "buckets": [
        {
          "key": "swing",
          "doc_count": 1
        },
        {
          "key": "jazz",
          "doc_count": 2
        }
      ]
    }
  }
}
```





## Maximum document count

*max_doc_count*参数用于控制 *term* 可以具有的文档计数的上限



*rare_terms agg*没有像*terms agg*那样的大小限制。

这意味着将返回与*max_doc_count*标准匹配的术语

但是，这确实意味着如果选择不正确，可以返回大量结果。为了限制此设置的危险，maximum *max_doc_count* 最大为100



