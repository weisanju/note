# 描述

* 是一个开源的分布式的,为分布式应用提供协调服务的Apache项目
* 基于观察者模式设计的分布式服务管理框架
* 储存和管理大家关心的数据

* 一个领导者,多个跟随者组成的集群

# 特性

* 集群中只要有半数以上节点存活,zookeeper 集群就能正常服务
* 全局数据一致,每个节点 数据相同
* 原子更新
* 实时性

# 数据模型结构

* 与文件系统类似,整体上可以看作是一颗树
* 每个节点称作 znode,每个znode能存储1MB的数据
* 每个znode可以通过路径唯一标识

# 应用场景

* 统一命名服务
* 统一配置管理
* 统一集群管理
* 服务节点动态上下线
* 负载均衡

# 基本操作

*./zkServer.sh start|status|stop*

*./zkCli.sh*

# 配置解读

| 配置名       | 解释                            |
| ------------ | ------------------------------- |
| *tickTime*   | 心跳帧发送的时长                |
| *initLimit*  | 启动时,最大延迟的 心跳帧的 个数 |
| *syncLimit*  | 启动后,最大延迟的心跳帧的个数   |
| *dataDir*    | 数据保存目录                    |
| *clientPort* | 监听客户端连接的  端口          |

# 内部原理

选举机制

* 半数机制

  集群中半数以上机器存活,集群可用,所以zookeeper适合安装奇数台服务器

* 选举机制
  * zookeeper的master 和 slave 是通过选举产生的
  * 根据启动顺序 投最大的id的 服务,达到半数以上则选举成功\

节点类型

* 持久 

  客户端和服务器断开的连接后,节点不删除

  分两类持久化节点

  * 持久化目录节点
  * 持久化顺序编号目录节点
    * 按顺序编号
    * 可以用于所有事件进行全局排序

* 短暂 

  客户端和服务器断开的连接后,节点自己删除

# Zookeeper集群搭建

1. 同步文件

2. 在*zkData* 创建 *myid*文件

3. 增加 服务器配置

   ```
   server.2=hadoop1:2888:3888
   server.3=hadoop2:2888:3888
   server.4=hadoop3:2888:3888
   2888:leader与follower 的交换信息的端口
   3888:选举端口
   ```

4. ```
   systemctl stop firewalld && ./zookeeper/bin/zkServer.sh start && ./zookeeper/bin/zkServer.sh status
   ```

   

# 客户端命令行操作

| 操作                          | 解析                     |
| ----------------------------- | ------------------------ |
| *ziCli.sh*                    | 启动客户端               |
| *help*                        | 帮助                     |
| *ls /*                        | 查看当前节点             |
| *create /path  data_string*   | 创建普通节点             |
| *get /path*                   | 获取节点的值             |
| *create -e /path data_string* | 短暂结点                 |
| *create -s /path data_string* | 序号节点                 |
| *set /path data_value*        | 修改节点的值             |
| *get /path watch*             | 监听节点的值变化         |
| *ls /path watch*              | 监听目录节点             |
| *delete /path*                | 删除空节点或者非目录节点 |
| *rmr*                         | 递归删除节点             |
| *stat*                        | 查看节点状态             |

# *stat*结构体解析

| 名称             | 解析                                           |
| ---------------- | ---------------------------------------------- |
| *czxid*          | 事务id,每次修改zookeeper都会收到*zxid*的时间戳 |
| *ctime*          | znode被创建的毫秒数                            |
| *mzxid*          | 最后更新的事务*zxid*                           |
| *mtime*          | 最后被修改的毫秒数                             |
| *pzxid*          | 最后更新的子节点的*zxid*                       |
| *cversion*       | 子节点变化号,*znode*子节点修改次数             |
| *dataversion*    | 数据变化号                                     |
| *aclVersion*     | 访问控制列表变化号                             |
| *ephemeralOwner* | 临时节点的拥有者的 sessionID,不是临时节点为0   |
| *dataLength*     | 数据长度                                       |
| *numChildren*    | 子节点数量                                     |



# C/S跨进程间监听器原理

1. 客户端在main线程中创建两个线程,一个*connect*负责 与*zookeeper*服务器网络通信,另一个*listener*负责监听
2. 通过*connect*将被监听的对象 发送到 服务器
3. 服务器检测到 监听事件发生,将消息发送给 *listener*线程
4. *listener*线程调用 *process*方法

# 写数据流程

* 如果*server1*不是*leader* ,将请求转发给*leader*
* *leader*会将写的数据广播给*follower*
* *follower*数据写成功了之后会通知*leader*
* 当大多数写成功了,*leader*即可认为写成功,然后通知客户端

# ACL访问权限与节点模式



# 服务器动态上下线功能

* 往服务器注册监听,临时节点
* 客户端监听*getChildren*,一旦有服务器下线,就会自动重新注册







