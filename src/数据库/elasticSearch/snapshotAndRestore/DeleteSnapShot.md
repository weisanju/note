## Delete snapshot API

```
DELETE /_snapshot/my_repository/my_snapshot

```



### Request

```
DELETE /_snapshot/<repository>/<snapshot>
```



### Description

1. 使用删除快照API删除快照，快照是从运行中的Elasticsearch集群获取的备份。
2. 从存储库中删除快照时，Elasticsearch会删除与该快照关联且未被任何其他快照使用的所有文件。与至少一个其他现有快照共享的所有文件都保持不变。
3. 如果在创建快照时尝试删除快照，快照过程将中止，并且所有关联的快照都将被删除。
4. 要在单个请求中删除多个快照，请使用逗号分隔快照名称或使用通配符 (*)。提示
5. 使用删除快照API取消错误启动的长时间运行的快照操作。



### Path parameters

- **`<repository>`**

  (Required, string) Name of the repository to delete a snapshot from.

- **`<snapshot>`**

  (Required, string) Comma-separated list of snapshot names to delete. Also accepts wildcards (`*`).









