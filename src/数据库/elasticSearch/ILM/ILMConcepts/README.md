## ILM concepts

- [Index lifecycle](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-index-lifecycle.html)
- [Rollover](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/index-rollover.html)
- [Policy updates](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/update-lifecycle-policy.html)





## Index lifecycle

ILM defines five index lifecycle *phases*:

- **Hot**: 该索引正在积极更新和查询。.
- **Warm**: 索引不再更新，但仍在查询中.
- **Cold**: 该索引不再被更新，并且很少被查询。信息仍然需要搜索，但是如果这些查询速度较慢，也可以。.
- **Frozen**: 该索引不再被更新，并且很少被查询。信息仍然需要搜索，但是如果这些查询非常慢也可以.
- **Delete**: 该索引不再需要，可以安全地删除。

索引的生命周期策略指定了哪些阶段适用，在每个阶段中执行了哪些操作以及何时在阶段之间过渡。

您可以在创建索引时手动应用生命周期策略

对于时间序列索引，您需要将生命周期策略与用于在序列中创建新索引的索引模板相关联。

当索引滚动时，手动应用的策略不会自动应用于新索引

如果您使用Elasticsearch的安全功能，ILM仅具有在上次策略更新时分配给用户的 [roles](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/defining-roles.html) 。



### Phase transitions

ILM根据索引在整个生命周期中的年龄 移动索引，为了控制这些转换的时间，您可以为每个阶段设置一个最小年龄。

为了使索引移至下一阶段，当前阶段中的所有操作必须完成，并且索引必须早于下一阶段的最小年龄。



配置的最小年龄必须在后续阶段之间增加，例如，最小年龄为10天的 `warm` 阶段之后只能是最小年龄为未设置或> = 10天的 `cold` 阶段。

最小年龄默认为零，这会导致ILM在当前阶段的所有操作完成后立即将索引移至下一阶段

如果索引具有未分配的分片，并且群集运行状况( [cluster health status](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/cluster-health.html) i)为黄色，则该索引仍可以根据其索引生命周期管理策略过渡到下一阶段



但是，由于Elasticsearch只能在绿色集群上执行某些清理任务，因此可能会产生意外的副作用，为了避免增加磁盘使用率和可靠性问题，请及时解决任何群集运行状况问题。



### Phase execution



ILM控制执行一个阶段中的动作的顺序以及执行哪些步骤来执行每个动作的必要索引操作。

当索引进入阶段时，ILM会在索引元数据中缓存阶段定义，这样可以确保策略更新不会将索引置于永远无法退出阶段的状态。

如果可以安全地应用更改，则ILM会更新缓存的阶段定义。如果不能，则使用缓存的定义继续执行阶段。

ILM定期运行，检查索引是否符合策略标准，并执行所需的任何步骤。

为了避免竞争条件，ILM可能需要运行多次才能执行完成操作所需的所有步骤。

例如，如果ILM确定索引已满足*rollover*标准，则它开始执行完成展期操作所需的步骤。

如果它达到了  `前进到下一步不安全` 的地步，则执行停止

下次ILM运行时，ILM在停止执行的地方开始执行，这意味着，即使将indices.lifecycle.poll_interval设置为10分钟，并且索引满足翻转标准，也可能需要20分钟才能完成翻转。



### Phase actions

ILM在每个阶段都支持以下操作。ILM按列出的顺序执行操作。

- Hot
  - [Set Priority](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-set-priority.html)
  - [Unfollow](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-unfollow.html)
  - [Rollover](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-rollover.html)
  - [Read-Only](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-readonly.html)
  - [Shrink](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-shrink.html)
  - [Force Merge](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-forcemerge.html)
  - [Searchable Snapshot](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-searchable-snapshot.html)
- Warm
  - [Set Priority](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-set-priority.html)
  - [Unfollow](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-unfollow.html)
  - [Read-Only](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-readonly.html)
  - [Allocate](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-allocate.html)
  - [Migrate](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-migrate.html)
  - [Shrink](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-shrink.html)
  - [Force Merge](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-forcemerge.html)
- Cold
  - [Set Priority](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-set-priority.html)
  - [Unfollow](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-unfollow.html)
  - [Read-Only](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-readonly.html)
  - [Searchable Snapshot](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-searchable-snapshot.html)
  - [Allocate](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-allocate.html)
  - [Migrate](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-migrate.html)
  - [Freeze](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-freeze.html)
- Frozen
  - [Searchable Snapshot](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-searchable-snapshot.html)
- Delete
  - [Wait For Snapshot](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-wait-for-snapshot.html)
  - [Delete](https://www.elastic.co/guide/en/elasticsearch/reference/7.17/ilm-delete.html)