## 端口放开

- **TCP port 2377** for cluster management communications 用于集群管理通信
- **TCP** and **UDP port 7946** for communication among nodes  节点之间通信
- **UDP port 4789** for overlay network traffic  用于 overlay 网络 的流量通信
- 5000 端口 私有仓库访问端口

```
firewall-cmd --add-port=2377/tcp --permanent
firewall-cmd --add-port=7946/tcp --permanent
firewall-cmd --add-port=7946/udp --permanent
firewall-cmd --add-port=5000/tcp --permanent
firewall-cmd --add-port=443/tcp --permanent
firewall-cmd --reload
```



# 集群管理命令

## 初始化节点

```sh
# 初始化为 管理节点	
docker swarm init
```

## 以 工作节点加入到管理节点

```sh
 docker swarm join \
    --token SWMTKN-1-49nj1cmql0jkz5s954yi3oex3nedyz0fb0xx14ie39trti4wxv-8vxv8rssmk743ojnwacrr2e7c \
    192.168.99.100:2377
```

## 查看以工作节点加入到集群中的命令

```sh
docker swarm join-token worker
```

## 查看以管理节点加入到集群中的命令

```sh
docker swarm join-token manager
```





# 新建Registry 私有仓库

```sh
docker service create  --constraint node.role==manager    --name registry --publish published=5000,target=5000 registry:2
```

# 新建虚拟化服务

```sh
docker pull dockersamples/visualizer:latest
docker service create \
--name=viz \
--publish=8081:8080/tcp \
--constraint=node.role==manager \
--mount=type=bind,src=/var/run/docker.sock,dst=/var/run/docker.sock \
dockersamples/visualizer:latest
```

