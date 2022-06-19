# 全局修改地方

```
protected-mode no
appendonly yes
```



# 变量定义

## redis通用变量

```
port 6001
pidfile "/var/run/redis_6001.pid"
bind 172.16.48.129 127.0.0.1
```

## redis进程

```
requirepass "123456"
masterauth "123456"
```

## redis从进程

```
#新增
requirepass "123456"
masterauth "123456"
slaveof 192.168.1.88 6001
```

## 哨兵进程

```
sentinel monitor mymaster 192.168.1.32 6001 2
sentinel down-after-milliseconds mymaster 5000
sentinel failover-timeout mymaster 15000
sentinel parallel-syncs mymaster 2
sentinel auth-pass mymaster 123456
```



# IP配置

```
#通用变量
REDIS_IMAGE_NAME=myredis-server
SENTINEl_IMAGE_NAME=myredis-sentinel

# 创建 一主两从，三哨兵

MASTER1=172.18.1.2
SLAVE1=172.18.1.3
SLAVE2=172.18.1.4
SENTINEL1=172.18.1.5
SENTINEL2=172.18.1.6
SENTINEL3=172.18.1.7



# 函数定义  --------------------------------------------------------------------------------------
# 创建容器
create_container(){
 ip=$1;shift;
 image_name=$1;shift;
 docker run -d --name ${image_name}_${ip} --ip $ip --network=mynet   $image_name
}

function delete_container(){
        ip=$1;shift;
        image_name=$1;shift;
        docker stop ${image_name}_${ip}
        docker rm ${image_name}_${ip}
}

function restartContainer(){
        ip=$1;shift;
        image_name=$1;shift;
        docker restart ${image_name}_${ip}
}


# 函数定义结束  --------------------------------------------------------------------------------------
```



