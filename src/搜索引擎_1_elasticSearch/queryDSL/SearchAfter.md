# Search After

结果的分页可以通过使用 from 和 size 来完成，但是当达到深度分页时成本变得禁止。 *index.max_result_window* 默认为 10,000 是一种保护，

搜索请求占用堆内存和时间与 from + size 成比例。 

建议使用 Scroll api 进行高效的深层滚动，但滚动上下文是昂贵的，不建议将其用于实时用户请求。 

search_after 参数通过提供活动光标来规避此问题。 



这个想法是使用前一页的结果来帮助检索下一页。

假设检索第一页的查询如下所示：

```json
GET twitter/tweet/_search
{
    "size": 10,
    "query": {
        "match" : {
            "title" : "elasticsearch"
        }
    },
    "sort": [
        {"date": "asc"},
        {"_uid": "desc"}
    ]
}
```

**注意**

每个文档具有一个唯一值的字段应用作排序规范的仲裁。 

否则，具有相同排序值的文档的排序顺序将是未定义的。 建议的方法是使用字段 _uid，它确保每个文档包含一个唯一值。

上述请求的结果包括每个文档的排序值数组。 这些排序值可以与 search_after 参数结合使用，以便在结果列表中的任何文档之后“返回”结果。

 例如，我们可以使用最后一个文档的排序值，并将其传递给search_after 以检索下一页结果：

```
GET twitter/tweet/_search
{
    "size": 10,
    "query": {
        "match" : {
            "title" : "elasticsearch"
        }
    },
    "search_after": [1463538857, "tweet#654323"],
    "sort": [
        {"date": "asc"},
        {"_uid": "desc"}
    ]
}
```

**当使用 search_after 时，参数 from 必须设置为 0（或 -1 ）。**

search_after 不是一种自由地跳到随机页面的解决方案，而是一种并行地滚动许多查询的解决方案。 它非常类似于滚动 API，

但不同的是，search_after 参数是无状态的，它总是解决对搜索器的最新版本。 因此，排序顺序可能会在步行期间更改，具体取决于您的索引的更新和删除。





