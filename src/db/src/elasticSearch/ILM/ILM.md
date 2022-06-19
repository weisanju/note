### 前序

综合上述拆解分析可知：

1. 有了：冷热集群架构，集群的不同节点有了明确的角色之分，冷热数据得以物理隔离，SSD 固态盘使用效率会更高。

2. 有了：rollover 滚动索引，索引可以基于文档个数、时间、占用磁盘容量滚动升级，实现了索引的动态变化。
3. 有了：Shrink 压缩索引、Frozen 冷冻索引，索引可以物理层面压缩、冷冻，分别释放了磁盘空间和内存空间，提高了集群的可用性。
4. 除此之外，还有：Force merge 段合并、Delete 索引数据删除等操作，索引的“生、老、病、死”的全生命周期的

如上指令单个操作，非常麻烦和繁琐，有没有更为快捷的方法呢？

第一：命令行可以 DSL 大综合实现。

第二：可以借助 Kibana 图形化界面实现。

### Index lifecycle

ILM defines five index lifecycle *phases*:

#### **Hot**

频繁更新与查询

#### **Warm**

索引不再更新，但仍在查询中。

#### **Cold**

不在被更新。查询得不是很频繁

#### **Frozen**

该索引不再被更新，并且很少被查询。信息仍然需要搜索，但是如果这些查询非常慢，也可以。

#### **Delete**

该索引不再需要，可以安全地删除。



索引的生命周期策略指定了哪些阶段适用哪些操作，在每个阶段中执行了哪些操作以及何时在阶段之间过渡。



您可以在创建索引时手动应用生命周期策略。

对于时间序列索引，您需要将生命周期策略与用于在序列中创建新索引的索引模板相关联。

当索引滚动时，手动应用的策略不会自动应用于新索引。

### Phase transitions

ILM根据索引的年龄在整个生命周期中移动索引。

1. 为了控制这些转换的时间，您可以为每个阶段设置一个最小年龄。

2. 为了使索引移至下一阶段，当前阶段中的所有操作必须完成，并且索引必须早于下一阶段的最小年龄。配置的最小年龄必须在后续阶段之间增加，
3. 例如，最小年龄为10天的 “warm” 阶段之后，cold阶段只能 大于 10天
4. 最小年龄默认是 0：这就导致 ILM 完成阶段中的action 之后 就立马 转移到下一阶段
5. 如果索引 有未分配的 分片，集群状态 为 黄色，索引仍然能改 转移 到下一个阶段 
6. 但是，由于Elasticsearch只能在绿色集群上执行某些清理任务，因此可能会产生意外的副作用。
7. 为了避免增加磁盘使用率和可靠性问题，请及时解决任何群集运行状况问题。

### Phase execution

ILM控制执行一个阶段中的动作的顺序以及执行哪些步骤来执行每个动作的必要索引操作。



当索引进入某一阶段时，ILM会在索引元数据中缓存阶段定义。

这样可以确保策略更新不会将索引置于永远无法退出阶段的状态

如果可以安全地应用更改，则ILM会更新缓存的阶段定义。如果不能，则使用缓存的定义继续执行阶段。



1. ILM定期运行，检查索引是否符合策略标准，并执行所需的任何步骤。

2. 为了避免竞争条件，ILM可能需要运行多次才能执行完成操作所需的所有步骤。
3. 例如，如果ILM确定索引已满足展期标准，则它开始执行完成展期操作所需的步骤。
4. 如果它达到了前进到下一步不安全的地步，则执行停止。
5. 下次ILM运行时，ILM会从停止的地方开始执行。这意味着，即使将indices.lifecycle.poll_interval设置为10分钟，并且索引满足翻转标准，也可能需要20分钟才能完成翻转。





## 各生命周期 Actions 设定

### Hot 阶段

-  	基于：max_age=3天、最大文档数为5、最大size为：50gb rollover 滚动索引。 	
-  	设置优先级为：100（值越大，优先级越高）。 	

### Warm 阶段 	 	 	 	

-  	实现段合并，max_num_segments 设置为1. 	
-  	副本设置为 0。 	
-  	数据迁移到：warm 节点。 	
-  	优先级设置为：50。 	

### Cold 阶段

-  	冷冻索引 	
-  	数据迁移到冷节点 	

### Delete 阶段 	

-  	删除索引 	

关于触发滚动的条件： 	 	

-  	Hot 阶段的触发条件：手动创建第一个满足模板要求的索引。 	
-  	其余阶段触发条件：min_age，索引自创建后的时间。 	

时间类似：业务里面的 热节点保留 3 天，温节点保留 7 天，冷节点保留 30 天的概念。





### 实战

#### 节点配置

- 节点 node-022：主节点+数据节点+热节点（Hot）。 	
- 节点 node-023：主节点+数据节点+温节点（Warm）。 	
- 节点 node-024：主节点+数据节点+冷节点（Cold）。 



**节点属性配置**

```
- node.attr.box_type: hot
- node.attr.box_type: warm
- node.attr.box_type: cold
```



#### 集群刷新频率

```
PUT _cluster/settings
{
  "persistent": {
    "indices.lifecycle.poll_interval": "1s"
  }
}
```

#### 新建ILM Policy

```

# step2:测试需要，值调的很小
PUT _ilm/policy/my_custom_policy_filter
{
  "policy": {
    "phases": {
      "hot": {
        "actions": {
          "rollover": {
            "max_age": "3d",
            "max_docs": 5,
            "max_size": "50gb"
          },
          "set_priority": {
            "priority": 100
          }
        }
      },
      "warm": {
        "min_age": "15s",
        "actions": {
          "forcemerge": {
            "max_num_segments": 1
          },
          "allocate": {
            "require": {
              "box_type": "warm"
            },
            "number_of_replicas": 0
          },
          "set_priority": {
            "priority": 50
          }
        }
      },
      "cold": {
        "min_age": "30s",
        "actions": {
          "allocate": {
            "require": {
              "box_type": "cold"
            }
          },
          "freeze": {}
        }
      },
      "delete": {
        "min_age": "45s",
        "actions": {
          "delete": {}
        }
      }
    }
  }
}
```



#### 索引模板

```

# step3:创建模板，关联配置的ilm_policy
PUT _index_template/timeseries_template
{
  "index_patterns": ["timeseries-*"],                 
  "template": {
    "settings": {
      "number_of_shards": 1,
      "number_of_replicas": 0,
      "index.lifecycle.name": "my_custom_policy_filter",      
      "index.lifecycle.rollover_alias": "timeseries",
      "index.routing.allocation.require.box_type": "hot"
    }
  }
}


```

**新建初始索引**

```

# step4:创建起始索引（便于滚动）
PUT timeseries-000001
{
  "aliases": {
    "timeseries": {
      "is_write_index": true
    }
  }
}
```

#### 插入数据

```

# step5：插入数据
PUT timeseries/_bulk
{"index":{"_id":1}}
{"title":"testing 01"}
{"index":{"_id":2}}
{"title":"testing 02"}
{"index":{"_id":3}}
{"title":"testing 03"}
{"index":{"_id":4}}
{"title":"testing 04"}

# step6：临界值（会滚动）
PUT timeseries/_bulk
{"index":{"_id":5}}
{"title":"testing 05"}

# 下一个索引数据写入
PUT timeseries/_bulk
{"index":{"_id":6}}
{"title":"testing 06"}
```
