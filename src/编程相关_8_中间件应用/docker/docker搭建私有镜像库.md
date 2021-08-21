# 前言

主要介绍registry、harbor两种私有仓库搭建。



# registry 的搭建

Docker 官方提供了一个搭建私有仓库的镜像 **registry** ，只需把镜像下载下来，运行容器并暴露5000端口，就可以使用了。



```
docker pull registry:2
docker run -d -v /opt/registry:/var/lib/registry -p 5000:5000 --name myregistry registry:2

```

Registry服务默认会将上传的镜像保存在容器的/var/lib/registry，我们将主机的/opt/registry目录挂载到该目录，即可实现将镜像保存到主机的/opt/registry目录了。



浏览器访问http://127.0.0.1:5000/v2，出现下面情况说明registry运行正常。





现在通过push镜像到registry来验证一下。

**要通过docker tag将该镜像标志为要推送到私有仓库：**

```sh
使用docker tag将session-web:latest这个镜像标记为 127.0.0.1:5000/session-web:latest
#格式为
docker tag IMAGE[:TAG][REGISTRY_HOST[:REGISTRY_PORT]/]REPOSITORY[:TAG]
docker tag session-web:latest 127.0.0.1:5000/session-web:latest
#使用docker push上传标记的镜像
docker push 127.0.0.1:5000/session-web:latest
```

**下载私有仓库的镜像，使用如下命令：**

```sh
docker pull localhost:5000/镜像名:版本号
docker pull localhost:5000/nginx:latest
```



# harbor 的搭建

docker 官方提供的私有仓库 registry，用起来虽然简单 ，但在管理的功能上存在不足。 Harbor是一个用于存储和分发Docker镜像的企业级Registry服务器，harbor使用的是官方的docker registry(v2命名是distribution)服务去完成。harbor在docker distribution的基础上增加了一些安全、访问控制、管理的功能以满足企业对于镜像仓库的需求。

## 搭建

### 下载

[github](https://github.com/goharbor/harbor/releases)

### 配置

```
tar -xvf harbor-offline-installer-{version}.tgz
```

**修改 harbor.yml **

```
#hostname 改为 0.0.0.0
hostname = 
https 配置注释掉
```

