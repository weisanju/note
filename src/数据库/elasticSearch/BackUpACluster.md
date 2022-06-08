## Back up a cluster

1. 备份群集的唯一可靠且受支持的方法是拍摄快照。您不能通过复制Elasticsearch集群节点的数据目录来备份它。没有支持的方法可以从文件系统级备份中还原任何数据。

2. 如果您尝试从这样的备份中恢复群集，它可能会因损坏或丢失文件或其他数据不一致的报告而失败，或者它似乎已经成功地无声地丢失了一些数据。



To have a complete backup for your cluster:

1. [Back up the data](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/backup-cluster-data.html)
2. [Back up the cluster configuration](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/backup-cluster-configuration.html)
3. [Back up the security configuration](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/security-backup.html)



To restore your cluster from a backup:

1. [Restore the data](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/restore-cluster-data.html)
2. [Restore the security configuration](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/restore-security-configuration.html)



### Back up a cluster’s data

### Back up the cluster configuration

### Back up the security configuration

### Restore the security configuration

### Restore the data






