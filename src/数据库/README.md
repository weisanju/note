1. 分片手动分配 /cluster/reoute
2. 通过 节点 自定义属性 指定分片分配
3. Index recovery prioritization：节点恢复优先级。





主要学习了节点的分配

1. 包括 _custer/reoute API手动分配分片，_
2. cluster/allocation/explain 分片分配 或 未分配的详细解释，
3. 通过自定义节点属性 node.attributes.size=big 自定义索引的分片分配，
4. 通过 index.routing.allocation.total_shards_per_node 和 cluster.routing.allocation.total_shards_per_node  分别 指定索引和集群 层面在每个节点上分配的最大的分片数。
5. 通过  index.priority 指定索引在 集群恢复时 的 分片分配的优先级，
6. 以及当节点从集群离开时的 分片的延迟 重分配