# 以IDEA为例

配置 `TLS` 实现安全的 Docker 远程连接。



# 非安全的连接方式

以CentOS为例

## 配置 socket-service

```
vim /etc/systemd/system/docker-tcp.socket

[Unit]
Description=Docker Socket for the API

[Socket]
# ListenStream=127.0.0.1:2375
ListenStream=2375
BindIPv6Only=both
Service=docker.service

[Install]
WantedBy=sockets.target
```

## 重新启动服务

```javascript
$ sudo systemctl daemon-reload
$ sudo systemctl enable docker-tcp.socket
$ sudo systemctl stop docker
$ sudo systemctl start docker-tcp.socket
$ sudo systemctl start docker

# 注意：这种方法必须先启动 docker-tcp.socket，再启动 Docker，一定要注意启动顺序！
```

## 客户端测试连接

```

docker -H 192.168.57.110:2375 info
```

## 配置环境变量简化连接

```
 export DOCKER_HOST="tcp://0.0.0.0:2375"
 docker info
```

# 配置TLS安全连接

[官方文档](https://docs.docker.com/engine/security/protect-access/)

[步骤](https://cloud.tencent.com/developer/article/1047265)



