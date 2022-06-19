# Use volumes

> 使用卷

卷是持久化 Docker 容器生成和使用的数据的首选机制。
虽然绑定挂载依赖于主机的目录结构和操作系统，但卷完全由 Docker 管理。
与绑定安装相比，卷有几个优点：

- 卷比绑定安装更容易备份或迁移。
- 您可以使用 Docker CLI 命令或 Docker API 管理卷。 
- 卷适用于 Linux 和 Windows 容器。
- 可以更安全地在多个容器之间共享卷
- 卷驱动程序允许您将卷存储在远程主机或云提供商上，以加密卷的内容或添加其他功能。
-  新卷的内容可以由容器预先填充
- Volumes on Docker Desktop具有比来自 Mac 和 Windows 主机的绑定挂载更高的性能

此外，与在容器的可写层中持久化数据相比，卷通常是更好的选择，因为卷不会增加使用它的容器的大小，并且卷的内容存在于给定容器的生命周期之外。

![/images/types-of-mounts-volume.png](types-of-mounts-volume.png)

如果您的容器生成非持久状态数据，请考虑使用 [tmpfs](https://docs.docker.com/storage/tmpfs/) 挂载以避免将数据永久存储在任何地方，并通过避免写入容器的可写层来提高容器的性能。
Volumes use `rprivate` bind propagation, and bind propagation is not configurable for volumes.

# Choose the -v or --mount flag

一般来说，--mount 更明确和详细。
最大的区别是 -v 语法将所有选项组合在一个字段中，而 --mount 语法将它们分开。
这是每个标志的语法比较。
如果需要指定卷驱动程序选项，则必须使用 --mount。

* `v` or `--volume` ： 由三个字段组成，以冒号字符 (:) 分隔。字段必须按正确顺序排列，每个字段的含义并不是很明显。
    * 在命名卷的情况下，第一个字段是卷的名称，并且在给定的主机上是唯一的。对于匿名卷，第一个字段被省略。
    * 第二个字段是文件或目录在容器中挂载的路径
    * 第三个字段是可选的，是一个以逗号分隔的选项列表，例如 ro。
        
* `--mount`：

由多个键值对组成，以逗号分隔，每个键值对由一个 = 元组组成。 
--mount 语法比 -v 或 --volume 更冗长，但键的顺序并不重要，标志的值更容易理解。

* `type` 挂载的类型， 可以是 bind、volume 或 tmpfs。This topic discusses volumes, so the type is always `volume`.
* `source` 挂载源 ：对于命名卷，这是卷的名称，对于匿名卷，此字段被省略。
    可以指定为 source 或 src。
* `destination`  将文件或目录安装在容器中的路径作为其值。
    可以指定为  destination、dst 或 target。
* `readonly`   if present, causes the bind mount to be [mounted into the container as read-only](https://docs.docker.com/storage/volumes/#use-a-read-only-volume).
* `volume-opt`  可以多次指定，它采用由选项名称及其值组成的键值对。

**从外部 CSV 解析器转义值**

如果您的卷驱动程序接受逗号分隔列表作为选项，您必须从外部 CSV 解析器中转义该值。
要对 volume-opt 进行转义，请用双引号 (") 将其括起来，并用单引号 (') 将整个挂载参数括起来。



```
$ docker service create \
--mount 
'type=volume,src=<VOLUME-NAME>,dst=<CONTAINER-PATH>,volume-driver=local,volume-opt=type=nfs,volume-opt=device=<nfs-server>:<nfs-path>,"volume-opt=o=addr=<nfs-address>,vers=4,soft,timeo=180,bg,tcp,rw"'
    --name myservice \
    <IMAGE>
```

# Create and manage volumes

**Create a volume**:

```sh
# 自动创建卷 时会指定 默认挂载位置
# ${docker_root_dir}/volumes/my-vol/_data
$ docker volume create my-vol
```

**List volumes**:

```sh
docker volume ls
```

**Inspect a volume**:

```sh
$ docker volume inspect my-vol
[
    {
        "Driver": "local",
        "Labels": {},
        "Mountpoint": "/var/lib/docker/volumes/my-vol/_data",
        "Name": "my-vol",
        "Options": {},
        "Scope": "local"
    }
]
```

**Remove a volume**:

```sh
$ docker volume rm my-vol
```



# 启动一个带卷的容器

## 使用命令指定

如果您使用尚不存在的卷启动容器，Docker 会为您创建该卷。
以下示例将卷 myvol2 安装到容器中的 /app/ 中。

```sh
$ docker run -d \
  --name devtest \
  --mount source=myvol2,target=/app \
  nginx:latest
```

## docker-compose指定

**在 首次 调用过程中 会自动创建**

```
version: "3.9"
services:
  frontend:
    image: node:lts
    volumes:
      - myapp:/home/node/app
volumes:
  myapp:
```

## Start a service with volumes

```sh
$ docker service create -d \
  --replicas=4 \
  --name devtest-service \
  --mount source=myvol2,target=/app \
  nginx:latest
```

删除该服务不会删除该服务创建的任何卷。
卷删除是一个单独的步骤。

docker service create 命令不支持 -v 或 --volume 标志。
将卷挂载到服务的容器中时，您必须使用 --mount 标志。

## Use a read-only volume

多个容器可以挂载同一个卷，并且可以同时为其中一些容器以读写方式挂载，对其他容器以只读方式挂载

```
$ docker run -d \
  --name=nginxtest \
  --mount source=nginx-vol,destination=/usr/share/nginx/html,readonly \
  nginx:latest
```



# 在机器之间共享数据

构建容错应用程序时，您可能需要配置同一服务的多个副本才能访问相同的文件

在开发应用程序时，有多种方法可以实现这一点。
一种是向您的应用程序添加逻辑，以将文件存储在 Amazon S3 等云对象存储系统上。
另一种方法是使用支持将文件写入外部存储系统（如 NFS 或 Amazon S3）的驱动程序创建卷。

卷驱动程序允许您从应用程序逻辑中抽象出底层存储系统。
例如，如果您的服务使用带有 NFS 驱动程序的卷，您可以更新服务以使用不同的驱动程序，例如将数据存储在云中，而无需更改应用程序逻辑。

## Use a volume driver

当您使用 docker volume create 创建卷时，或者当您启动使用尚未创建的卷的容器时，您可以指定卷驱动程序。
以下示例首先在创建独立卷时使用 vieux/sshfs 卷驱动程序，然后在启动创建新卷的容器时使用。

### Initial set-up

在 Docker 主机上，安装 vieux/sshfs 插件：

```sh
$ docker plugin install --grant-all-permissions vieux/sshfs
```

## Create a volume using a volume driver

此示例指定了 SSH 密码，但如果两台主机配置了共享密钥，则可以省略密码。
每个卷驱动程序可能有零个或多个可配置选项，每个选项都使用 -o 标志指定。

```sh
docker volume create --driver vieux/sshfs \
  -o sshcmd=test@node2:/home/test \
  -o password=testpassword \
  sshvolume
```

## 创建 NFS 卷的服务

此示例说明如何在创建服务时创建 NFS 卷。
本示例使用 10.0.0.10 作为 NFS 服务器，使用 /var/docker-nfs 作为 NFS 服务器上的导出目录。
请注意，指定的卷驱动程序是本地的。

```sh
$ docker service create -d \
  --name nfs-service \
  --mount 'type=volume,source=nfsvolume,target=/app,volume-driver=local,volume-opt=type=nfs,volume-opt=device=:/var/docker-nfs,volume-opt=o=addr=10.0.0.10' \
  nginx:latest
```

```sh
docker service create -d \
    --name nfs-service \
    --mount 'type=volume,source=nfsvolume,target=/app,volume-driver=local,volume-opt=type=nfs,volume-opt=device=:/var/docker-nfs,"volume-opt=o=addr=10.0.0.10,rw,nfsvers=4,async"' \
    nginx:latest
```

## Create CIFS/Samba volumes

> 请注意，如果使用主机名而不是 IP，则需要 addr 选项，以便 docker 可以执行主机名查找。

```sh
docker volume create \
	--driver local \
	--opt type=cifs \
	--opt device=//uxxxxx.your-server.de/backup \
	--opt o=addr=uxxxxx.your-server.de,username=uxxxxxxx,password=*****,file_mode=0777,dir_mode=0777 \
	--name cif-volume
```

**手动创建**

```
docker volume create \
	--driver local \
	--opt type=cifs \
	--opt device=//192.168.1.166/gitrepo/docker_test \
	--opt o=username=networkshare,password=123456,file_mode=0777,dir_mode=0777 \
	--name cif-volume
```

**docker-compose语法**

```yaml
volumes:
  cif-volume:
    driver: local
    driver_opts:
      type: cifs
      device: //192.168.1.166/gitrepo/docker_test
      o:  username=networkshare,password=123456,file_mode=0777,dir_mode=0777
```

# 备份、恢复或迁移数据卷

## 备份容器

```sh
$ docker run -v /dbdata --name dbstore ubuntu /bin/bash

$ docker run --rm --volumes-from dbstore -v $(pwd):/backup ubuntu tar cvf /backup/backup.tar /dbdata
```

## 从备份恢复容器

```sh
$ docker run -v /dbdata --name dbstore2 ubuntu /bin/bash
$ docker run --rm --volumes-from dbstore2 -v $(pwd):/backup ubuntu bash -c "cd /dbdata && tar xvf /backup/backup.tar --strip 1"
```

# Remove volumes

删除容器后，Docker 数据卷仍然存在。
有两种类型的卷需要考虑：

命名卷具有来自容器外部的特定来源，例如 awesome:/bar。
匿名卷没有特定的来源，所以当容器被删除时，指示 Docker 引擎守护进程将它们删除。

## Remove anonymous volumes

要自动删除匿名卷，请使用 --rm 选项。
例如，此命令创建匿名 /foo 卷。
当容器被移除时，Docker 引擎会移除 /foo 卷而不是 awesome 卷。

```sh
$ docker run --rm -v /foo -v awesome:/bar busybox top
```

## Remove all volumes

To remove all unused volumes and free up space:

```sh
$ docker volume prune
```



