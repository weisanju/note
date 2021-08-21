# 连接到 已启动的容器中

## 使用docker attach进入Docker容器

```sh
# 创建 守护进程
sudo docker run -itd ubuntu:14.04 /bin/bash  
# 连接该容器 的输入输出 到 宿主机的标准输入输出
sudo docker attach 44fc0f0582d9  
```

## 使用SSH进入Docker容器

[不推荐使用](https://www.oschina.net/translate/why-you-dont-need-to-run-sshd-in-docker?cmp)



## 使用nsenter进入Docker容器

什么是 [nsenter](https://github.com/jpetazzo/nsenter)

　　nsenter可以访问另一个进程的名称空间。所以为了连接到某个容器我们还需要获取该容器的第一个进程的PID。可以使用docker inspect命令来拿到该PID。

docker inspect命令使用如下：

```sh
sudo docker inspect 44fc0f0582d9  
sudo docker inspect -f {{.State.Pid}} 44fc0f0582d9  
sudo nsenter --target 3326 --mount --uts --ipc --net --pid 
```





## **使用docker exec进入Docker容器**

**交互式模式终端**

```sh
# i:交互式连接，t：分配一个伪终端
docker exec -i -t [容器名] /bin/bash
```

**运行容器内的脚本**

```sh
docker exec -it [容器名] /bin/sh /root/runoob.sh
```

**利用容器ID**

```sh
docker exec -it [容器ID] /bin/bash
```



[参考链接](https://www.cnblogs.com/xhyan/p/6593075.html)



