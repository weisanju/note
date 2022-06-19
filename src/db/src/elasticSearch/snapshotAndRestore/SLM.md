## SLM: Manage the snapshot lifecycle

1. 您可以设置快照生命周期策略来自动控制快照的定时、频率和保留。快照策略可以应用于多个数据流和索引。

2. The snapshot lifecycle management (SLM) [CRUD APIs](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/snapshot-lifecycle-management-api.html) 为Kibana管理的一部分提供快照策略功能的构建块。
3. [Snapshot and Restore](https://www.elastic.co/guide/en/kibana/7.13/snapshot-repositories.html) 可以轻松设置策略，注册快照存储库，查看和管理快照以及还原数据流或索引。

4. 您可以停止并重新启动SLM，以在执行升级或其他维护时暂时暂停自动备份。


## 教程

1. 本教程演示如何使用SLM策略自动备份Elasticsearch数据流和索引

2. 该策略对集群中的所有数据流和索引进行快照，并将它们存储在本地存储库中。它还定义了保留策略，并在不再需要快照时自动删除快照。

要使用SLM管理快照，您可以

1. [Register a repository](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/getting-started-snapshot-lifecycle-management.html#slm-gs-register-repository).
2. [Create an SLM policy](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/getting-started-snapshot-lifecycle-management.html#slm-gs-create-policy).

要测试策略，您可以手动触发它以获取初始快照。

### Register a repository

1. 要使用SLM，您必须配置快照存储库。

2. 存储库可以是本地 (共享文件系统) 或远程 (云存储)。远程存储库可以驻留在S3、HDFS、Azure、谷歌云存储或存储库插件  [repository plugin](https://www.elastic.co/guide/en/elasticsearch/plugins/7.13/repository.html)  支持的任何其他平台上。
3. 远程存储库一般用于生产部署。

```console
PUT /_snapshot/my_repository
{
  "type": "fs",
  "settings": {
    "location": "my_backup_location"
  }
}
```

### Set up a snapshot policy

1. 一旦有了存储库，就可以定义SLM策略以自动拍摄快照
2. 该策略定义了何时拍摄快照，应包括哪些数据流或索引  以及如何命名快照。
3. 策略还可以指定保留策略 [retention policy](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/slm-retention.html) ，并在不再需要快照时自动删除快照。
4. 不要害怕配置频繁快照的策略。快照是增量的，可以有效地利用存储。

**注意**

您可以通过Kibana Management或使用创建或更新策略API定义和管理策略。

#### **Example**

例如，您可以定义一个夜间快照策略，以每天在UTC上午1:30备份所有数据流和索引。

```console
PUT /_slm/policy/nightly-snapshots
{
  "schedule": "0 30 1 * * ?",  // 1. Cron syntax
  "name": "<nightly-snap-{now/d}>", //2. 快照命名
  "repository": "my_repository",  //存储库
  "config": { 
    "indices": ["*"]  //包含的索引
  },
  "retention": {  //快照维持天数
    "expire_after": "30d",  //保持快照30天，
    "min_count": 5, //无论年龄大小，至少保留5张且不超过50张快照
    "max_count": 50  
  }
}
```

1.  [Cron syntax](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/trigger-schedule.html#schedule-cron)
2.  [date math](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/date-math-index-names.html) 



#### **其他配置**

您可以指定其他快照配置选项来自定义快照的拍摄方式

例如，如果缺少指定的数据流或索引之一，则可以将策略配置为使快照失败，详见： [snapshot requests](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/snapshots-take-snapshot.html)





### Test the snapshot policy

1. SLM拍摄的快照与其他快照一样

2. 您可以在Kibana Management中查看有关快照的信息，也可以使用快照api获取信息。 [snapshot APIs](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/snapshots-monitor-snapshot-restore.html)
3. 此外，SLM会跟踪策略的成功和失败，因此您可以深入了解策略的工作方式
4. 如果策略至少执行了一次，则 [get policy](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/slm-api-get-policy.html)  将返回其他元数据，这些元数据将显示快照是否成功。

#### 手动执行策略

1. 可以手动立即执行快照策略
2. 这对于在进行配置更改、升级或测试新策略之前拍摄快照非常有用
3. 手动执行策略不会影响其配置的计划。

```
POST /_slm/policy/nightly-snapshots/_execute
```

强制运行nightly-snapshots策略后，您可以检索策略以获取成功或失败信息。

```console
GET /_slm/policy/nightly-snapshots?human
```

1. 仅返回最近的成功和失败，但所有策略执行都记录在 `.slm-history*` indices. 
2. 响应还显示策略何时计划下一步执行。
3. 响应显示策略是否成功启动快照，但是，这并不能保证快照成功完成，例如，如果在复制文件时丢失了与远程存储库的连接，则启动的快照可能会失败。

```console-result
{
  "nightly-snapshots" : {
    "version": 1,
    "modified_date": "2019-04-23T01:30:00.000Z",
    "modified_date_millis": 1556048137314,
    "policy" : {
      "schedule": "0 30 1 * * ?",
      "name": "<nightly-snap-{now/d}>",
      "repository": "my_repository",
      "config": {
        "indices": ["*"],
      },
      "retention": {
        "expire_after": "30d",
        "min_count": 5,
        "max_count": 50
      }
    },
    "last_success": { //关于策略最后一次成功创建快照的信息                           
      "snapshot_name": "nightly-snap-2019.04.24-tmtnyjtrsxkhbrrdcgg18a", //成功启动的快照名称
      "time_string": "2019-04-24T16:43:49.316Z",
      "time": 1556124229316
    } ,
    "last_failure": { //有关策略上次启动快照失败的信息
      "snapshot_name": "nightly-snap-2019.04.02-lohisb5ith2n8hxacaq3mw",
      "time_string": "2019-04-02T01:30:00.000Z",
      "time": 1556042030000,
      "details": "{\"type\":\"index_not_found_exception\",\"reason\":\"no such index [important]\",\"resource.type\":\"index_or_alias\",\"resource.id\":\"important\",\"index_uuid\":\"_na_\",\"index\":\"important\",\"stack_trace\":\"[important] IndexNotFoundException[no such index [important]]\\n\\tat org.elasticsearch.cluster.metadata.IndexNameExpressionResolver$WildcardExpressionResolver.indexNotFoundException(IndexNameExpressionResolver.java:762)\\n\\tat org.elasticsearch.cluster.metadata.IndexNameExpressionResolver$WildcardExpressionResolver.innerResolve(IndexNameExpressionResolver.java:714)\\n\\tat org.elasticsearch.cluster.metadata.IndexNameExpressionResolver$WildcardExpressionResolver.resolve(IndexNameExpressionResolver.java:670)\\n\\tat org.elasticsearch.cluster.metadata.IndexNameExpressionResolver.concreteIndices(IndexNameExpressionResolver.java:163)\\n\\tat org.elasticsearch.cluster.metadata.IndexNameExpressionResolver.concreteIndexNames(IndexNameExpressionResolver.java:142)\\n\\tat org.elasticsearch.cluster.metadata.IndexNameExpressionResolver.concreteIndexNames(IndexNameExpressionResolver.java:102)\\n\\tat org.elasticsearch.snapshots.SnapshotsService$1.execute(SnapshotsService.java:280)\\n\\tat org.elasticsearch.cluster.ClusterStateUpdateTask.execute(ClusterStateUpdateTask.java:47)\\n\\tat org.elasticsearch.cluster.service.MasterService.executeTasks(MasterService.java:687)\\n\\tat org.elasticsearch.cluster.service.MasterService.calculateTaskOutputs(MasterService.java:310)\\n\\tat org.elasticsearch.cluster.service.MasterService.runTasks(MasterService.java:210)\\n\\tat org.elasticsearch.cluster.service.MasterService$Batcher.run(MasterService.java:142)\\n\\tat org.elasticsearch.cluster.service.TaskBatcher.runIfNotProcessed(TaskBatcher.java:150)\\n\\tat org.elasticsearch.cluster.service.TaskBatcher$BatchedTask.run(TaskBatcher.java:188)\\n\\tat org.elasticsearch.common.util.concurrent.ThreadContext$ContextPreservingRunnable.run(ThreadContext.java:688)\\n\\tat org.elasticsearch.common.util.concurrent.PrioritizedEsThreadPoolExecutor$TieBreakingPrioritizedRunnable.runAndClean(PrioritizedEsThreadPoolExecutor.java:252)\\n\\tat org.elasticsearch.common.util.concurrent.PrioritizedEsThreadPoolExecutor$TieBreakingPrioritizedRunnable.run(PrioritizedEsThreadPoolExecutor.java:215)\\n\\tat java.base/java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1128)\\n\\tat java.base/java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:628)\\n\\tat java.base/java.lang.Thread.run(Thread.java:834)\\n\"}"
    } ,
    "next_execution": "2019-04-24T01:30:00.000Z",                        
    "next_execution_millis": 1556048160000 //下次策略执行时
  }
}
```





## Security and SLM

启用Elasticsearch安全功能时，以下群集权限控制对SLM操作的访问:

- **`manage_slm`**

  允许用户执行所有SLM操作，包括创建和更新策略以及启动和停止SLM。

- **`read_slm`**

  允许用户执行所有只读SLM操作，例如获取策略和检查SLM状态。

- **`cluster:admin/snapshot/\*`**

​		允许用户获取和删除任何索引的快照，无论他们是否有权访问该索引。

1. 您可以通过Kibana Management创建和管理角色来分配这些权限。

2. 要授予创建和管理SLM策略和快照所需的权限，您可以使用manage_slm和 `cluster:admin/snapshot/*`   集群权限和对SLM历史索引的完全访问权限。



例如，以下请求创建了slm-admin角色:

```console
POST /_security/role/slm-admin
{
  "cluster": ["manage_slm", "cluster:admin/snapshot/*"],
  "indices": [
    {
      "names": [".slm-history-*"],
      "privileges": ["all"]
    }
  ]
}
```

1. 要授予对SLM策略和快照历史记录的只读访问权限，可以设置具有*read_slm* 集群权限的角色，并读取对快照生命周期管理历史记录索引的访问权限。
2. 例如，以下请求创建了一个  `slm-read-only` 

```console
POST /_security/role/slm-read-only
{
  "cluster": ["read_slm"],
  "indices": [
    {
      "names": [".slm-history-*"],
      "privileges": ["read"]
    }
  ]
}
```



## Snapshot retention

1. 您可以在SLM策略中包含保留策略，以自动删除旧快照
2. Retention 作为集群级任务运行，并且不与特定策略的计划相关联
3.  retention criteria 作为 Retention task 的一部分进行评估，而不是在策略执行时
4. 为了使保留任务自动删除快照，您需要在SLM策略中包含一个 [`retention`](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/slm-api-put-policy.html#slm-api-put-retention)  Object



1. 要控制保留任务何时运行 配置  [`slm.retention_schedule`](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/slm-settings.html#slm-retention-schedule) 集群配置
2. 可以定义周期性或者 绝对时间  [cron schedule](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/trigger-schedule.html#schedule-cron). 
3. The [`slm.retention_duration`](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/slm-settings.html#slm-retention-duration) 设置 限制SLM删除旧快照应该花费多长时间。
4. 使用 [update settings](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/cluster-update-settings.html)  动态变更 schedule and duration  设置
5. 可以使用  [execute retention ](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/slm-api-execute-retention.html)API 手动执行

The retention task 仅考虑通过 SLM策略 拍摄的快照，无论是根据策略计划还是通过 [execute lifecycle](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/slm-api-execute-lifecycle.html)  

手动快照将被忽略，并且不会计入 retention limits。



要检索有关快照保留任务历史记录的信息， [get stats](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/slm-api-get-stats.html) API:

```console
GET /_slm/stats
```

**response**

```json
{
  "retention_runs": 13, //运行的次数
  "retention_failed": 0, //失败的次数
  "retention_timed_out": 0,  //超时的次数：retention次数达到slm.retention_duration时间限制，必须在删除所有符合条件的快照之前停止
  "retention_deletion_time": "1.4s",  //定期删除快照总花费时间
  "retention_deletion_time_millis": 1404, 
  "policy_stats": [ //被 daily-snapshots 策略 拍摄的快照信息
    {
      "policy": "daily-snapshots",
      "snapshots_taken": 1,
      "snapshots_failed": 1,
      "snapshots_deleted": 0, 
      "snapshot_deletion_failures": 0 
    }
  ],
  "total_snapshots_taken": 1,
  "total_snapshots_failed": 1,
  "total_snapshots_deleted": 0, 
  "total_snapshot_deletion_failures": 0 
}
```

