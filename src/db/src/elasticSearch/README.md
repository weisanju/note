## 索引

key value key是某种属性，value是文档列表

**正排索引**

根据文档相关的属性形成的key。例如 文档ID、文档创建时间等等

**倒排索引**

key为 文档内容通过分词形成的单词

**倒排列表**

包含出现过某个单词的所有文档列表、出现的位置信息、以及出现的词频



## 相关度

衡量某个文档与查询的匹配程度

**TF-IDF**

词频：某个单词在某篇文章的出现次数，词频与相关度正相关

逆文档频率：某个单词在整个 文档库出现的次数，逆文档频率与相关度逆相关

**BM-25**

基于



## 搜索引擎的三个核心指标

### 更全

### 更准

### 更块



### 搜索引擎的三个核心问题

**用户的真正需求是什么**

**哪些信息是和用户的需求真正相关的**

**哪些信息是用户可以信赖的**





### 评价搜索引擎的指标

|        | 在搜索结果中 | 不在搜索结果中 |
| ------ | ------------ | -------------- |
| 相关   | N            | K              |
| 不相关 | M            | L              |

精准率 = N/(N+M)

召回率=N/(N+K)



针对搜索引擎的业务场景：精准率更为重要




