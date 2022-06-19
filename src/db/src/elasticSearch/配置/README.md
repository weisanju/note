## Configuring Elasticsearch

1. ElasticSearch开箱即用 配置很少
2. 大部分集群配置可以通过  [Cluster update settings](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/cluster-update-settings.html) API
3. 特定于节点 的静态配置才需要 使用到次配置文件
   1. cluster.name
   2. network.host

## Config files location

### 配置文件

- `elasticsearch.yml` for configuring Elasticsearch
- `jvm.options` for configuring Elasticsearch JVM settings
- `log4j2.properties` for configuring Elasticsearch logging

### 安装位置取决于安装方式

1. 手动安装 取决于 $ES_HOME/config ES_PATH_CONF 可以修改

2. RPM、Debian 包安装：/etc/elasticsearch 路径下

   1. /etc/default/elasticsearch (for the Debian package)
   2. /etc/sysconfig/elasticsearch

   修改上述文件中的 ES_PATH_CONF=/etc/elasticsearch 可以修改默认配置文件路径





## 配置文件格式

[YAML](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/settings.html#:~:text=configuration%20format%20is-,YAML,-.%20Here%20is%20an)

```yaml
path:
    data: /var/lib/elasticsearch
    logs: /var/log/elasticsearch
```

```yaml
discovery.seed_hosts:
   - 192.168.1.10:9300
   - 192.168.1.11
   - seeds.mydomain.com
```



## 环境变量替换

```yaml
node.name:    ${HOSTNAME}
network.host: ${ES_NETWORK_HOST}
```

多个值可以使用 逗号分割

```yaml
export HOSTNAME=“host1,host2"
```





## Cluster and node setting types

群集和节点 配置 可以根据它们的配置方式进行分类:

### **Dynamic**

* 可以使用  the [cluster update settings API](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/cluster-update-settings.html).  运行时配置
* 也可以在启动前配置 

使用群集更新设置API进行的更新可以是持久性的，适用于群集重新启动，

也可以是瞬态的，在群集重新启动后重置。

您还可以通过使用API为瞬态或持久设置分配空值来重置它们。



如果使用多种方法配置相同的设置，Elasticsearch将按以下优先顺序应用设置:

1. Transient setting
2. Persistent setting
3. `elasticsearch.yml` setting
4. Default setting value

例如，您可以应用瞬态设置来覆盖持久设置或elasticsearch.yml设置。

但是，对elasticsearch.yml设置的更改不会覆盖已定义的瞬态或持久设置。



**最佳实践**

1. 最好使用群集更新设置API设置动态的群集范围设置，

2. 并仅将elasticsearch.yml用于本地配置。
3. 使用群集更新设置API可确保所有节点上的设置相同。
4. 如果您不小心在不同节点上的elasticsearch.yml中配置了不同的设置，可能会很难注意到差异。



### **Static**

Static settings can only be configured on an unstarted or shut down node using `elasticsearch.yml`.

* 只能未启动时配置、无法运行时配置
* 每个节点都要配置





