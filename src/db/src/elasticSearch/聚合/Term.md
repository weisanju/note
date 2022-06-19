## 简介

从例子中可以了解到，Terms Aggregation 以指定域的唯一值（term）构建出多个桶，每个桶包含了 key 为域的值（term），doc_count 为含有这个值（term）的文档数。

```
GET /_search
{
    "aggs" : {
        "genres" : {
            "terms" : { "field" : "genre" }
        }
    }
}
```

```
{
    ...
    "aggregations" : {
        "genres" : {
            "doc_count_error_upper_bound": 0, 
            "sum_other_doc_count": 0, 
            "buckets" : [ 
                {
                    "key" : "electronic",
                    "doc_count" : 6
                },
                {
                    "key" : "rock",
                    "doc_count" : 3
                },
                {
                    "key" : "jazz",
                    "doc_count" : 2
                }
            ]
        }
    }
}
```

默认情况下，响应将返回 10 个桶。当然也可以指定 size 参数改变返回的桶的数量。





## Size 与精度

Terms Aggregation 返回结果中的 doc_count- 是近似的，并不是一个准确数。

根据 Elasticsearch 的机制，每个分片都会根据各自拥有的数据进行计算并且进行排序，最后协调节点对各个分片的计算结果进行整理并返回给客户端。

而就是因为这样，产生出精度的问题，也就是计算的结果有误差。



下面为官方给出的例子：有一组数据，数据中包含一个 product 字段，记录了产品的数量。请求获取产品数量 TOP 5 的数据。而数据存在有 3 个分片的索引中。

```
GET /_search
{
    "aggs" : {
        "products" : {
            "terms" : {
                "field" : "product",
                "size" : 5
            }
        }
    }
}
```

会有如下动作发生：

各个分片计算得出的结果。

|      | ShardA      | ShardB      | ShardC      |
| ---- | ----------- | ----------- | ----------- |
| 1    | productA 25 | productA 30 | productA 45 |
| 2    | productB 18 | productB 25 | productC 44 |
| 3    | productC 6  | productF 17 | productZ 36 |
| 4    | productD 3  | productZ 16 | productG 30 |
| 5    | productE 2  | productG 15 | productE 29 |
| 6    | productF 2  | productH 14 | productH 28 |
| 7    | productG 2  | productI 10 | productQ 2  |
| 8    | productH 2  | productQ 8  | productD 1  |
| 9    | productI 1  | productJ 6  |             |
| 10   | productJ 1  | productC 4  |             |

然后分片将会把 TOP 5 的数据返回给协调节点。

|      | ShardA      | ShardB      | ShardC      |
| ---- | ----------- | ----------- | ----------- |
| 1    | productA 25 | productA 30 | productA 45 |
| 2    | productB 18 | productB 25 | productC 44 |
| 3    | productC 6  | productF 17 | productZ 36 |
| 4    | productD 3  | productZ 16 | productG 30 |
| 5    | productE 2  | productG 15 | productE 29 |

最后，协调节点将会根据各个节点给出的数据进行整理得出最后 TOP 5 的数据并返回给客户端。

|      | 最终数据      |
| ---- | ------------- |
| 1    | productA(100) |
| 2    | productZ(52)  |
| 3    | productC(50)  |
| 4    | productG(45)  |
| 5    | productB(43)  |

可以看出，在第二步中，由于各个分片的数据有所不同，数据*ProductC*在分片 A 能排得上 TOP 5 的 term 在分片 B 却排不上，所以统计的 count个数可能不准确

但是，只要各个分片都返回足够多的数据给协调节点，客户端得到的结果将是精准的。

而开始提到的参数 size 就会控制分片返回给节点的数据量以及返回给客户端的数据量。可见，参数 size 越大，获取的结果的精度越高。



## Shard Size

上面提到，size 的大小会影响到聚合结果的精准度，size 值越大，精度越高。为了更高得精度，请求的时候将 size 值设置得偏大，这时会有一个问题，就是客户端将会得到大量的响应数据，而且这些响应数据对于客户端来说大部分都是没用的，而大量的响应数据还会耗费网络资源。





这时，就要使用到另一个参数 shard_size 。shard_size **只会控制分片返回给协调节点的数据量**，而**最后协调节点整理并返回的数据量由 size 控制**，这样既能提升精度，也避免了上述由于要提升精度而导致协调节点返回大量响应数据给客户端的问题。



上面的内容由提到，size 会控制分片返回给协调节点的数据量，这段描述即正确也不正确。默认情况下，shard_size 的大小为 (size * 1.5 + 10) ，确实由 size 值控制，但是如果在请求时显式提供 shard_size 参数，自然 size 与分片返回给节点的数据量无关。





## 没显示的文档数

在响应结果中，有一个 *sum_other_doc_count* 值。假如 size 设定为 5，那么响应中只有 doc_count 前 5 的桶的数据，**而 sum_other_doc_count 表示的就是没有返回的其他桶的文档数的总和。**



## 文档数计算错误上限

Terms Aggreagation 的响应结果中，有一个 *doc_count_error_upper_bound* 值。这个值表示的是在聚合中，没有在最终结果（响应给客户端的结果）中的 term 最大可能有 *doc_count_error_upper_bound* 个文档含有。这是 ES 预估可能出现的最坏的结果。



doc_count_error_upper_bound 是这样计算出来的：假如请求像上面 Product 的例子一样，

size = 5，协调节点会将各个分片的排第五的 term 的文档数相加起来（根据上面的例子就是 2+15+29），得出的结果便是 doc_count_error_upper_bound 。



根据 doc_count_error_upper_bound 的计算是基于这样的猜想（继续以上面的 Product 为例子），

可能存在 Product Z 1，它在分片 A 中，包含它的文档数是 2，在分片 B 中它的文档数是 15，在分片 C 中它的文档数是 29，然后在各个分片的排名均是第六位，这样在协调节点将获取不到有关 Product Z1 的数据，便会将这个 Product Z1 排除在外，然而实际上这个 Product Z1 是足以排进前 5 的。



当然上述提到的情况并不没有这么容易发生，但是 doc_count_error_upper_bound 越大，错误发生的可能性也越大（这个大是指与响应的结果作比较）。这时候可以适当增大 size 的值，让更多的数据参与到协调节点的整理过程中。



## 每个桶的错误上限

如上面提到的文档数计算错误上限类似，不过这个是精确到每个桶的。
这个默认是关闭的，要开启就需要传递 **show_term_doc_count_error** 参数。

```
GET /_search
{
    "aggs" : {
        "products" : {
            "terms" : {
                "field" : "product",
                "size" : 5,
                "show_term_doc_count_error": true
            }
        }
    }
}
```

```


{
    ...
    "aggregations" : {
        "products" : {
            "doc_count_error_upper_bound" : 46,
            "sum_other_doc_count" : 79,
            "buckets" : [
                {
                    "key" : "Product A",
                    "doc_count" : 100,
                    "doc_count_error_upper_bound" : 0
                },
                {
                    "key" : "Product Z",
                    "doc_count" : 52,
                    "doc_count_error_upper_bound" : 2
                }
                ...
            ]
        }
    }
}


```

每个桶的错误上限是这样计算的：响应结果中的 term 在没有返回相关数据的分片的最后一名的文档数之和。以上述例子中的 Product Z 为例，分片 B，分片 C 响应给协调节点的数据均包含了 Product Z 相关的数据，但是分片 A 却没有，那么就有可能是 Product Z 在分片 A 中排不到前 5，那么 Product Z 在分片 A 中最大的可能值就是与 Product E 一样，也就是 2。

当然像 Product A 一样各个分片都有返回相关的数据的话，这个错误上限就是 0。





```
1 ...... 10000
```

