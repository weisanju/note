# Query DSL



Elasticsearch提供了一个基于JSON的完整查询DSL (域特定语言) 来定义查询。将查询DSL视为查询的AST (抽象语法树)，由两种类型的子句组成:



### **Leaf query clauses**



叶查询子句在特定字段中查找特定值，例如 *match*、*term* 、*range*。这些查询可以自己使用。



### **Compound query clauses**

复合查询子句包装其他叶子或复合查询，用于以逻辑方式组合多个查询 例如：*bool*、*dis_max*

或者 修改默认行为  [`constant_score`](https://www.elastic.co/guide/en/elasticsearch/reference/7.10/query-dsl-constant-score-query.html) 

Query clauses behave differently depending on whether they are used in [query context or filter context](https://www.elastic.co/guide/en/elasticsearch/reference/7.10/query-filter-context.html).

查询子句的行为 具体取决于它们是在查询上下文中还是在过滤器上下文中使用（ [query context or filter context](https://www.elastic.co/guide/en/elasticsearch/reference/7.10/query-filter-context.html).）



### **Allow expensive queries**

某些类型的查询通常会由于其实现方式而执行慢，这会影响群集的稳定性。这些查询可以分类如下:

* 需要进行线性扫描以识别匹配的查询: 
  * [`script queries`](https://www.elastic.co/guide/en/elasticsearch/reference/7.10/query-dsl-script-query.html)

* 具有较高前期成本的查询:
  * [`fuzzy queries`](https://www.elastic.co/guide/en/elasticsearch/reference/7.10/query-dsl-fuzzy-query.html) (except on [`wildcard`](https://www.elastic.co/guide/en/elasticsearch/reference/7.10/keyword.html#wildcard-field-type) fields)
  * [`regexp queries`](https://www.elastic.co/guide/en/elasticsearch/reference/7.10/query-dsl-regexp-query.html) (except on [`wildcard`](https://www.elastic.co/guide/en/elasticsearch/reference/7.10/keyword.html#wildcard-field-type) fields)
  * [`prefix queries`](https://www.elastic.co/guide/en/elasticsearch/reference/7.10/query-dsl-prefix-query.html) (except on [`wildcard`](https://www.elastic.co/guide/en/elasticsearch/reference/7.10/keyword.html#wildcard-field-type) fields or those without [`index_prefixes`](https://www.elastic.co/guide/en/elasticsearch/reference/7.10/index-prefixes.html))
  * [`wildcard queries`](https://www.elastic.co/guide/en/elasticsearch/reference/7.10/query-dsl-wildcard-query.html) (except on [`wildcard`](https://www.elastic.co/guide/en/elasticsearch/reference/7.10/keyword.html#wildcard-field-type) fields)
  * [`range queries>> on < and [`keyword`](https://www.elastic.co/guide/en/elasticsearch/reference/7.10/keyword.html) fields
* [`Joining queries`](https://www.elastic.co/guide/en/elasticsearch/reference/7.10/joining-queries.html)
* Queries on [deprecated geo shapes](https://www.elastic.co/guide/en/elasticsearch/reference/7.10/geo-shape.html#prefix-trees)
* 每个文档成本可能很高的查询:
  * [`script score queries`](https://www.elastic.co/guide/en/elasticsearch/reference/7.10/query-dsl-script-score-query.html)
  * [`percolate queries`](https://www.elastic.co/guide/en/elasticsearch/reference/7.10/query-dsl-percolate-query.html)

可以通过将*search.allow_expensive_queries*设置的值设置为false (默认为true) 来防止此类查询的执行。