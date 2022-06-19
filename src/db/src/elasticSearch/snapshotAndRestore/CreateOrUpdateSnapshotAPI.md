## Create or update snapshot repository API

Registers or updates a [snapshot repository](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/snapshots-register-repository.html).

```console
PUT /_snapshot/my_repository
{
  "type": "fs",
  "settings": {
    "location": "my_backup_location"
  }
}
```



### Request

```
PUT /_snapshot/<repository>
POST /_snapshot/<repository>
```

### Description

必须先注册 repository 才能执行 快照跟恢复操作 [snapshot and restore](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/snapshot-restore.html) 

快照格式每个大版本都会变，详见 See [Version compatibility](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/snapshot-restore.html#snapshot-restore-version-compatibility).



### Path parameters

- **`<repository>`**

  (Required, string) Name of the snapshot repository to register or update.

### Query parameters

注意：可以使用 query parameter 或 request body parameter 指定此API的多个选项。如果两个参数都指定，则仅使用查询参数。

- **`master_timeout`**

  (Optional, [time units](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#time-units)) Specifies the period of time to wait for a connection to the master node. If no response is received before the timeout expires, the request fails and returns an error. Defaults to `30s`.

- **`timeout`**

  (Optional, [time units](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#time-units)) Specifies the period of time to wait for a response. If no response is received before the timeout expires, the request fails and returns an error. Defaults to `30s`.

- **`verify`**

  (Optional, Boolean) If `true`, the request verifies the repository is functional on all master and data nodes in the cluster. If `false`, this verification is skipped. Defaults to `true`.You can manually perform this verification using the [verify snapshot repository API](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/verify-snapshot-repo-api.html).

### Request body

#### **type**

快照仓库类型

- [repository-s3](https://www.elastic.co/guide/en/elasticsearch/plugins/7.13/repository-s3.html) for S3 repository support
- [repository-hdfs](https://www.elastic.co/guide/en/elasticsearch/plugins/7.13/repository-hdfs.html) for HDFS repository support in Hadoop environments
- [repository-azure](https://www.elastic.co/guide/en/elasticsearch/plugins/7.13/repository-azure.html) for Azure storage repositories
- [repository-gcs](https://www.elastic.co/guide/en/elasticsearch/plugins/7.13/repository-gcs.html) for Google Cloud Storage repositories

#### settings

仓库容器配置

##### **chunk_size**

1. 文件块大小：字节数
2. 如果快照比这个数值大。则快照会被拆分成几个小文件
3. 默认为空，没有限制

##### **compress**

1. 如果为TRUE 则 metadata files 例如 索引 mappings settings 会被压缩存储。索引数据不会压缩。
2. 默认TRUE

##### **max_number_of_snapshots**

1. 仓库最大 快照数
2. 默认500
3. 不建议增大这个值。因为 过大的快照仓库 会 影响 主节点的性能。带来稳定性问题。
4. 相反，删除旧的快照或者使用 多仓库

##### **max_restore_bytes_per_sec**

1. 最大的每秒 恢复速度。字节值
2. 默认无限制。
3. restore 也会受到 [recovery settings](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/put-snapshot-repo-api.html#:~:text=also%20throttled%20through-,recovery%20settings,-.)影响

##### **max_snapshot_bytes_per_sec**

1. 最大每秒 快照取样时间。单位字节
2. 默认40M/s

##### **readonly**

1. 为TRUE 则 仓库是只读的
2. 只能读快照 不能写快照
3. 默认为 FALSE
4. 如果多集群中注册了同一个快照存储，则只有一个集群具有对该存储库的写访问 权限，让多个集群同时写入存储库有损坏存储库内容的风险。
5. 只有具有写访问权限的集群才能在存储库中创建快照。连接到存储库的所有其他群集都应将readonly参数设置为true。这意味着这些群集可以从存储库中检索或还原快照，但不能在其中创建快照。

其他可接受的设置属性取决于使用type参数设置的存储库类型。

#### FS repo settings

##### **location**

1. 必选。本地文件系统的位置
2. 必须注册在 主节点或者数据节点的 path.repo 设置的路径

#### source repo settings

##### **delegate_type**

1. 代码存储库类型
2. source repositories 可以使用 代理存储库的 配置
3. 详见：[source ONLY repository](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/snapshots-register-repository.html#snapshots-source-only-repository)

#### URL repo settings

1. 基于URL的 共享文件系统 repository 根路径
2. 支持以下格式
   1. file
   2. ftp
   3. http
   4. https
   5. jar
3. 使用文件协议的url必须指向群集中所有主节点和数据节点都可以访问的共享文件系统的位置。此位置必须在path.repo设置中注册。
4. 必须通过*repositoriesurl.allowed_urls*设置明确允许使用http、https或ftp协议的url。此设置支持在URL中代替主机、路径、查询或片段的通配符。



#### **verify**

1. 检验储存库是否在所有 主节点或者数据节点 起作用

