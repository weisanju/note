## Kibana Query Language



Kibana查询语言 (KQL) 是一种简单的语法，用于使用自由文本搜索或基于字段的搜索来过滤Elasticsearch数据。KQL仅用于过滤数据，没有对数据进行排序或聚合的作用。



KQL能够在您键入时提示字段名称、值和运算符。

提示的性能由Kibana[设置控制](https://www.elastic.co/guide/en/kibana/7.16/settings.html)。



KQL具有与Lucene查询语法不同的功能集。KQL能够查询嵌套字段和[脚本字段](https://www.elastic.co/guide/en/kibana/7.16/managing-index-patterns.html#scripted-fields)。

KQL不支持正则表达式或用模糊项搜索。要使用旧版Lucene语法，请单击搜索字段旁边的KQL，然后关闭KQL。



## Terms query

1. 术语查询使用精确的搜索词。空格分隔每个搜索词

2. 并且只需要一个词就可以匹配文档。
3. 使用引号表示短语匹配。

要使用精确的搜索词进行查询，请输入字段名称，后跟冒号，然后输入以空格分隔的值:

```yaml
http.response.status_code:400 401 404
```



对于文本字段，无论顺序如何，这都将匹配任何值:

```yaml
http.response.body.content.text:quick brown fox
```

要查询确切的短语，请在值周围使用引号:

```yaml
http.response.body.content.text:"quick brown fox"
```



字段名称不是KQL所必需的。如果未提供字段名称，则术语将与索引设置中的默认字段匹配。要跨字段搜索:

```yaml
"quick brown fox"
```

## Boolean queries

KQL支持or，and，not。

默认情况下，and 具有比 or 更高的优先级。

要覆盖默认优先级，请在括号中对运算符进行分组。这些运算符可以是大写或小写的。

```yaml
response:200 or extension:php
```

```yaml
response:(200 or 404)
```

```yaml
response:200 and (extension:php or extension:css)
```

```yaml
response:200 and extension:php or extension:css
```

```yaml
not response:200
```

```yaml
response:200 and not (extension:php or extension:css)
```

```yaml
tags:(success and info and security)
```

## Range queries

KQL supports `>`, `>=`, `<`, and `<=` on numeric and date types.

```yaml
account_number >= 100 and items_sold <= 200
```

## Date range queries

```yaml
@timestamp < "2021-01-02T21:55:59"
```

```yaml
@timestamp < "2021-01"
```

```yaml
@timestamp < "2021"
```

## Exist queries

```yaml
response:*
```

## Wildcard queries

通配符查询可用于按术语前缀搜索或搜索多个字段。

 The default settings of Kibana **prevent leading wildcards** for performance reasons, but this can be allowed with an [advanced setting](https://www.elastic.co/guide/en/kibana/7.16/advanced-options.html#query-allowleadingwildcards).



To match documents where `machine.os` starts with `win`, such as "windows 7" and "windows 10":

```yaml
machine.os:win*
```

```yaml
machine.os*:windows 10
```

当您具有字段的文本和关键字版本时，此语法非常方便。该查询检查术语为windows 10的machine.os和machine.os.关键字。



## Nested field queries

查询嵌套字段的主要考虑因素是如何将嵌套查询的部分与单个嵌套文档进行匹配。您可以:

* 仅将查询的部分与单个嵌套文档匹配。这是大多数用户在嵌套字段上查询时想要的。
* 将查询的部分与不同的嵌套文档进行匹配。这就是常规对象字段的工作方式。此查询通常不如匹配单个文档有用。

在下面的文档中，items是一个嵌套字段。嵌套字段中的每个文档都包含名称，股票和类别。

```json
{
  "grocery_name": "Elastic Eats",
  "items": [
    {
      "name": "banana",
      "stock": "12",
      "category": "fruit"
    },
    {
      "name": "peach",
      "stock": "10",
      "category": "fruit"
    },
    {
      "name": "carrot",
      "stock": "9",
      "category": "vegetable"
    },
    {
      "name": "broccoli",
      "stock": "5",
      "category": "vegetable"
    }
  ]
}
```

### Match a single document

To match stores that have more than 10 bananas in stock:

```yaml
items:{ name:banana and stock > 10 }
```

items是嵌套路径。花括号 (嵌套组) 内的所有内容都必须与单个嵌套文档匹配。

以下查询不返回任何匹配项，因为没有单个嵌套文档具有库存为9的香蕉。

```yaml
items:{ name:banana and stock:9 }
```

### Match different documents

以下子查询位于单独的嵌套组中，可以匹配不同的嵌套文档:

```yaml
items:{ name:banana } and items:{ stock:9 }
```

名称: banana匹配数组中的第一个文档，stock:9匹配数组中的第三个文档。

### Match single and different documents

```yaml
items:{ name:banana and stock > 10 } and items:{ category:vegetable }
```

The first nested group (`name:banana and stock > 10`) must match a single document, but the `category:vegetables` subquery can match a different nested document because it is in a separate group.



### Nested fields inside other nested fields

KQL支持其他嵌套字段内部的嵌套字段-您必须指定完整路径。在本文档中，level1和level2是嵌套字段:

```json
{
  "level1": [
    {
      "level2": [
        {
          "prop1": "foo",
          "prop2": "bar"
        },
        {
          "prop1": "baz",
          "prop2": "qux"
        }
      ]
    }
  ]
}
```

```yaml
level1.level2:{ prop1:foo and prop2:bar }
```