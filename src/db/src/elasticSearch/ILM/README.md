# ILM: Manage the index lifecycle



您可以配置索引生命周期管理 (ILM) 策略，根据您的性能、弹性和保留要求自动管理索引。例如，您可以使用ILM来:

- 当索引达到一定大小或文档数量时，启动新索引
- Create a new index each day, week, or month and archive previous ones
- 每天、每周或每月创建一个新索引，并存档以前的索引
- 删除陈旧索引以强制执行数据保留标准



您可以通过*Kibana Management*或*ILM api*创建和管理索引生命周期策略。为Beats或Logstash Elasticsearch输出插件启用索引生命周期管理时，会自动配置默认策略。

To automatically back up your indices and manage snapshots, use [snapshot lifecycle policies](https://www.elastic.co/guide/en/elasticsearch/reference/7.10/getting-started-snapshot-lifecycle-management.html).

## ILM overview

您可以创建和应用索引生命周期管理 (ILM) 策略，以根据您的性能、弹性和保留要求自动管理索引。
索引生命周期策略可以触发以下操作:

- **Rollover**: 当当前索引达到一定大小，文档数量或年龄时，创建一个新的写入索引。.
- **Shrink**: 减少索引中的主分片数量。
- **Force merge**: 触发 [force merge](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/indices-forcemerge.html) 减少分片中段的数量.
- **Freeze**: [Freezes](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/freeze-index-api.html) an index and makes it read-only.
- **Delete**: Permanently remove an index, including all of its data and metadata.



ILM使用 hot-warm-cold架构管理索引，这在处理日志和指标等时间序列数据时很常见。

您可以指定:

* 要滚动到新索引的最大分片大小，文档数量或年龄。
* 索引不再更新的点，并且可以减少主分片的数量。
* 何时强制合并以永久删除标记为删除的文档。
* 可以将索引移动到性能较低的硬件的点。
* 可用性不是那么关键，并且可以减少副本数量的点。
* 当可以安全地删除索引时。

例如，如果要将atm机组中的指标数据索引到Elasticsearch中，则可能会定义一个策略，该策略显示:

1. 当索引的主分片的总大小达到50gb时，滚动到一个新的索引。
2. 将旧索引移至暖阶段，将其标记为只读，然后将其缩小为单个分片。
3. 7天后，将索引移至冷阶段，然后将其移至较便宜的硬件。
4. 一旦达到所需的30天保留期，请删除索引。

要使用ILM，集群中的所有节点都必须运行相同的版本。尽管可能可以在混合版本的集群中创建和应用策略，但不能保证它们能够按预期工作。

尝试使用包含群集中所有节点都不支持的操作的策略将导致错误。