## 全文索引

The full text queries enable you to search [analyzed text fields](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/analysis.html) such as the body of an email

索引的分析器与 查询的分析器必须相同



The queries in this group are:

- **[`intervals` query](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/query-dsl-intervals-query.html)**

  A full text query that allows fine-grained control of the ordering and proximity of matching terms.

- **[`match` query](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/query-dsl-match-query.html)**

  用于执行全文查询的标准查询，包括模糊匹配和短语或邻近查询。

- **[`match_bool_prefix` query](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/query-dsl-match-bool-prefix-query.html)**

  Creates a `bool` query that matches each term as a `term` query, except for the last term, which is matched as a `prefix` query

  创建一个bool查询，该查询将每个term match为术语查询，但最后一个术语除外，该术语被匹配为前缀查询

- **[`match_phrase` query](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/query-dsl-match-query-phrase.html)**

  Like the `match` query but used for matching exact phrases or word proximity matches.

- **[`match_phrase_prefix` query](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/query-dsl-match-query-phrase-prefix.html)**

  Like the `match_phrase` query, but does a wildcard search on the final word.

- **[`multi_match` query](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/query-dsl-multi-match-query.html)**

  The multi-field version of the `match` query.

- **[`combined_fields` query](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/query-dsl-combined-fields-query.html)**

  Matches over multiple fields as if they had been indexed into one combined field.

- **[`query_string` query](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/query-dsl-query-string-query.html)**

  Supports the compact Lucene [query string syntax](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/query-dsl-query-string-query.html#query-string-syntax), allowing you to specify AND|OR|NOT conditions and multi-field search within a single query string. For expert users only.

- **[`simple_query_string` query](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/query-dsl-simple-query-string-query.html)**

  A simpler, more robust version of the `query_string` syntax suitable for exposing directly to users.





