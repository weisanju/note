# 前言

Docker Swarm 是 Docker 的集群管理工具。

支持的工具包括但不限于以下各项：

- Dokku
- Docker Compose
- Docker Machine
- Jenkins



swarm 集群由管理节点（manager）和工作节点（work node）构成。

- **swarm mananger**：负责整个集群的管理工作包括集群配置、服务管理等所有跟集群有关的工作。
- **work node**：即图中的 available node，主要负责运行相应的服务来执行任务（task）。



Swarm 的配置和状态信息保存在一套位于所有管理节点上的分布式 etcd 数据库中。该数据库运行于内存中，并保持数据的最新状态。关于该数据库最棒的是，它几乎不需要任何配置，作为 Swarm 的一部分被安装，无须管理。





# 集群搭建

```sh
# 会将其切换到 Swarm 模式
docker swarm init

# docker swarm init会通知 Docker 来初始化一个新的 Swarm，并将自身设置为第一个管理节点。同时也会使该节点开启 Swarm 模式。
$ docker swarm init \
--advertise-addr 10.0.0.1:2377 \
--listen-addr 10.0.0.1:2377
```

**--advertise-addr** 指定其他节点用来连接到当前管理节点的 IP 和端口。这一属性是可选的，当节点上有多个 IP 时，可以用于指定使用哪个IP。此外，还可以用于指定一个节点上没有的 IP，比如一个负载均衡的 IP。

**--listen-addr** 指定用于承载 Swarm 流量的 IP 和端口。其设置通常与 --advertise-addr 相匹配，但是当节点上有多个 IP 的时候，可用于指定具体某个 IP。并且，如果 --advertise-addr 设置了一个远程 IP 地址（如负载均衡的IP地址），该属性也是需要设置的。建议执行命令时总是使用这两个属性来指定具体 IP 和端口。

Swarm 模式下的操作默认运行于 2337 端口。虽然它是可配置的，但 2377/tcp 是用于客户端与 Swarm 进行安全（HTTPS）通信的约定俗成的端口配置。

**添加工作结点**

```sh
docker swarm join --token SWMTKN-1-1h6l8jcv1k6dxwd79tfel7tj3c9q94y9eau875odx56ghje385-4hlef30rwhix3mfruw50h9mpm 192.168.1.73:2377
```

**添加管理结点**

```
docker swarm join-token manager
```

# Swarm 管理器高可用性（HA）

Swarm 的管理节点内置有对 HA 的支持。这意味着，即使一个或多个节点发生故障，剩余管理节点也会继续保证 Swarm 的运转。

从技术上来说，Swarm 实现了一种主从方式的多管理节点的 HA。这意味着，即使你可能有多个管理节点，也总是仅有一个节点处于活动状态。

通常处于活动状态的管理节点被称为“主节点”（leader），而主节点也是唯一一个会对 Swarm 发送控制命令的节点。也就是说，只有主节点才会变更配置，或发送任务到工作节点。如果一个备用（非活动）管理节点接收到了 Swarm 命令，则它会将其转发给主节点。

关于 HA，有以下两条最佳实践原则。

- 部署奇数个管理节点。
- 不要部署太多管理节点（建议 3 个或 5 个）。

# Docker Swarm服务的部署及相关操作

## 创建集群中的服务

使用 `docker service create` 命令创建一个新的服务。

```
docker service create --name web-fe \
-p 8080:8080 \
--replicas 5 \
nigelpoulton/pluralsight-docker-ci
```

* `docker service creale` 命令告知 Docker 正在声明一个新服务，

* 并传递 --name 参数将其命名为 web-fe。
* 将每个节点上的 8080 端口映射到服务副本内部的 8080 端口。
* 接下来，使用 --replicas 参数告知 Docker 应该总是有 5 个此服务的副本。
* 最后，告知 Docker 哪个镜像用于副本，重要的是，要了解所有的服务副本使用相同的镜像和配置。

敲击回车键之后，主管理节点会在 Swarm 中实例化 5 个副本，管理节点也会作为工作节点运行。相关各工作节点或管理节点会拉取镜像，然后启动一个运行在 8080 端口上的容器。

**轮询监控**

所有的服务都会被 Swarm 持续监控，Swarm 会在后台进行轮训检查（Reconciliation Loop），来持续比较服务的实际状态和期望状态是否一致。如果一致，则无须任何额外操作；如果不一致，Swarm 会使其一致。换句话说，Swarm 会一直确保实际状态能够满足期望状态的要求。

假如运行有 web-fe 副本的某个工作节点宕机了，则 web-fe 的实际状态从 5 个副本降为 4 个，从而不能满足期望状态的要求。Docker 变回启动一个新的 web-fe 副本来使实际状态与期望状态保持一致。这一特性功能强大，使得服务在面对节点宕机等问题时具有自愈能力。

## 服务命令

**查看服务**

```sh
# 只能在管理结点运行
docker service ls
```

**查看实际进程**

```sh
# 只能在管理结点运行
docker service ps service_name
```

**查看详细命令**

```sh
docker service inspect
```

## 副本服务 vs 全局服务

服务的默认复制模式（Replication Mode）是副本模式（replicated）。

这种模式会部署期望数量的服务副本，并尽可能均匀地将各个副本分布在整个集群中。

另一种模式是全局模式（global），在这种模式下，每个节点上仅运行一个副本。可以通过给 `docker service create` 命令传递 --mode global 参数来部署一个全局服务。

## 服务的扩缩容

```sh
docker service scale web-fe=10
```

## 删除服务

```sh
docker service rm
```

## 滚动升级

```sh
# 同时升级 两台
--update-parallelism 2 \
# 20s 延迟
--update-delay 20s 
```

```sh
docker service update
```



# Docker Swarm服务日志及相关配置

**日志驱动**

[Docker](http://c.biancheng.net/docker/) Swarm 服务的日志可以通过执行 `docker service logs` 命令来查看，然而并非所有的日志驱动（Logging Driver）都支持该命令。

**默认日志驱动**

Docker 节点默认的配置是，服务使用 json-file 日志驱动，其他的驱动还有 journald（仅用于运行有 systemd 的 Linux 主机）、syslog、splunk 和 gelf。

json-file 和 journald 是较容易配置的，二者都可用于 `docker service logs` 命令。

```sh
docker service logs <service-name>
```


如下是在 daemon.json 配置文件中定义使用 syslog 作为日志驱动的示例。

```
{
  "log-driver": "syslog"
}
```

通过在执行 `docker service create` 命令时传入 --logdriver 和 --log-opts 参数可以强制某服务使用一个不同的日志驱动，这会覆盖 daemon.json 中的配置。



服务日志能够正常工作的前提是，容器内的应用程序运行于 PID 为 1 的进程，并且将日志发送给 STDOUT，错误信息发送给 STDERR。日志驱动会将这些日志转发到其配置指定的位置。



# SWARM常用命令

| 命令                        | 说明                                                         |
| --------------------------- | ------------------------------------------------------------ |
| docker swarm init           | 用于创建一个新的 Swarm。执行该命令的节点会成为第一个管理节点，并且会切换到 Swarm 模式。 |
| docker swarm join-token     | 用于查询加入管理节点和工作节点到现有 Swarm 时所使用的命令和 Token。 要获取新增管理节点的命令，请执行 docker swarm join-token manager 命令； 要获取新增工作节点的命令，请执行 docker swarm join-token worker 命令。 |
| docker node ls              | 用于列出 Swarm 中的所有节点及相关信息，包括哪些是管理节点、哪个是主管理节点。 |
| docker service create       | 用于创建一个新服务。                                         |
| docker service ls           | 用于列出 Swarm 中运行的服务，以及诸如服务状态、服务副本等基本信息。 |
| docker service ps <service> | 该命令会给出更多关于某个服务副本的信息                       |
| docker service inspect      | 用于获取关于服务的详尽信息。附加 --pretty 参数可限制仅显示重要信息。 |
| docker service scale        | 用于对服务副本个数进行增减。                                 |
| docker service update       | 用于对运行中的服务的属性进行变更。                           |
| docker service logs         | 用于查看服务的日志。                                         |
| docker service rm           | 用于从 Swarm 中删除某服务。该命令会在不做确认的情况下删除服务的所有副本，所以使用时应保持警惕。 |

