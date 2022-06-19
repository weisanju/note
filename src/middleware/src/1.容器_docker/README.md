{% raw %}
# 简介

[Docker](http://c.biancheng.net/docker/) 是一种运行于 Linux 和 Windows 上的软件，用于创建、管理和编排容器。

Docker 是在 GitHub 上开发的 Moby 开源项目的一部分。

“Docker”一词来自英国口语，意为码头工人（Dock Worker），即从船上装卸货物的人。

## Docker 运行时与编排引擎

多数技术人员在谈到 Docker 时，主要是指 **Docker 引擎**。

Docker 引擎是用于运行和编排容器的基础设施工具。有 VMware 管理经验的读者可以将其类比为 ESXi。

ESXi 是运行虚拟机的核心管理程序，而 Docker 引擎是运行容器的核心容器运行时。

其他 Docker 公司或第三方的产品都是围绕 Docker 引擎进行开发和集成的。

如下图所示，Docker 引擎位于中心，其他产品基于 Docker 引擎的核心功能进行集成。

Docker 引擎可以从 Docker 网站下载，也可以基于 GitHub 上的源码进行构建。无论是开源版本还是商业版本，都有 Linux 和 Windows 版本。

Docker 引擎主要有两个版本：企业版（EE）和社区版（CE）。

每个季度，企业版和社区版都会发布一个稳定版本。社区版本会提供 4 个月的支持，而企业版本会提供 12 个月的支持。

社区版还会通过 Edge 方式发布月度版。

从 2017 年第一季度开始，Docker 版本号遵循 YY.MM-xx 格式，类似于 Ubuntu 等项目。例如，2018 年 6 月第一次发布的社区版本为 18.06.0-ce。

## Docker开源项目（Moby）

Docker”一词也会用于指代开源 Docker 项目。其中包含一系列可以从 Docker 官网下载和安装的工具，比如 Docker 服务端和 Docker 客户端。

不过，该项目在 2017 年于 Austin 举办的 DockerCon 上正式命名为 Moby 项目。

由于这次改名，GitHub 上的 docker/docker 库也被转移到了 moby/moby，并且拥有了项目自己的 Logo，如下图所示。

Moby 项目的目标是基于开源的方式，发展成为 Docker 上游，并将 Docker 拆分为更多的模块化组件。

Moby 项目托管于 GitHub 的 Moby 代码库，包括子项目和工具列表。核心的 Docker 引擎项目位于 GitHub 的 moby/moby，但是引擎中的代码正持续被拆分和模块化。

作为一个开源项目，其源码是公开可得的，在遵循 Apache 协议 2.0 的情况下，任何人都可以自由地下载、贡献、调整和使用。

如果查看项目的提交历史，可以发现其中包含来自如下公司的基础技术：红帽、微软、IBM、思科，以及 HPE。此外，还可以看到一些并非来自大公司的贡献者。

多数项目及其工具都是基于 Golang 编写的，这是谷歌推出的一种新的系统级编程语言，又叫 Go 语言。使用 Go 语言的读者，将更容易为该项目贡献代码。

Mody/Docker 作为开源项目的好处在于其所有的设计和开发都是开放的，并摒弃了私有代码闭源开发模式下的陈旧方法。

因此发布过程也是公开进行的，不会再出现某个秘密的版本提前几个月就宣布要召开发布会和庆功会的荒唐情况。

Moby/Docker 不是这样运作的，项目中多数内容都是开放并欢迎任何人查看和作出贡献的。

Moby 项目以及更广泛的 Docker 运动一时间掀起了一波热潮。GitHub 上已经有数以千计的提交请求（pull request），以及数以万计的基于容器化技术的项目了，更不用说 Docker Hub 上数十亿的镜像下载。

Moby 项目已经给软件产业带来了翻天覆地的变化。

这并非妄想，Docker 已经得到了广泛的应用！



## 容器生态

Docker 公司的一个核心哲学通常被称为“含电池，但可拆卸”（Batteries included but removable）。

意思是许多 Docker 内置的组件都可以替换为第三方的组件，网络技术栈就是一个很好的例子。

Docker 核心产品内置有网络解决方案。但是网络技术栈是可插拔的，这意味着 Docker 内置的网络方案可以被替换为第三方的方案。许多人都会这样使用。

早期的时候，经常出现第三方插件比 Docker 提供的内置组件更好的情况。然而这会对 Docker 公司的商业模式造成冲击。毕竟，Docker 公司需要依靠盈利来维持基业长青。

因此，“内置的电池”变得越来越好用了。这也导致了生态内部的紧张关系和竞争的加剧。

简单来说，Docker 内置的“电池”仍然是可插拔的，然而越来越不需要将它们移除了。

尽管如此，容器生态在一种良性的合作与竞争的平衡中还是得以繁荣发展。

在谈及容器生态时，人们经常使用到诸如“co-opetition”（意即合作与竞争，英文中 co-operation 与 competition 合并的词）与“frenemy”（英文中朋友 friend 与敌人 enemy 合并的词）这样的字眼。这是一个好现象！因为良性的竞争是创新之母。



## 开放容器计划

如果不谈及开放容器计划（The Open Container Initiative, OCI）的话，对 Docker 和容器生态的探讨总是不完整的。





# Docker安装

```shell
$ curl https://get.docker.com/ | sh
$ systemctl start docker
$ useradd docker -g docker
$ docker --version
$ docker system info
$ systemctl enable docker
$ systemctl is-enabled docker
$ systemctl is-active docker
```



# 镜像

## 介绍

首先需要先从镜像仓库服务中拉取镜像。常见的镜像仓库服务是 Docker Hub，但是也存在其他镜像仓库服务。

拉取操作会将镜像下载到本地 Docker 主机，可以使用该镜像启动一个或者多个容器。

镜像由多个层组成，每层叠加之后，从外部看来就如一个独立的对象。**镜像内部是一个精简的操作系统（OS），同时还包含应用运行所必须的文件和依赖包。**



通常使用docker container run和docker service create命令从某个镜像启动一个或多个容器。

一旦容器从镜像启动后，二者之间就变成了互相依赖的关系，并且在镜像上启动的容器全部停止之前，镜像是无法被删除的。尝试删除镜像而不停止或销毁使用它的容器，会导致出错。

## 镜像通常比较小

容器目的就是运行应用或者服务，这意味着容器的镜像中必须包含应用/服务运行所必需的操作系统和应用文件。

但是，容器又追求快速和小巧，这意味着构建镜像的时候通常需要裁剪掉不必要的部分，保持较小的体积。

例如，Docker 镜像通常不会包含 6 个不同的 Shell 让读者选择——通常 Docker 镜像中只有一个精简的Shell，甚至没有 Shell。

镜像中还不包含内核——容器都是共享所在 Docker 主机的内核。所以有时会说容器仅包含必要的操作系统（通常只有操作系统文件和文件系统对象）。

Docker 官方镜像 Alpine Linux 大约只有 4MB，可以说是 Docker 镜像小巧这一特点的比较典型的例子。

## 拉取镜像

**拉取**

```shell
docker image pull
docker pull ubuntu
```

默认情况下，镜像会从 Docker Hub 的仓库中拉取。

```shell
docker image pull alpine:latest 
```

命令会从 Docker Hub 的 alpine 仓库中拉取标签为 latest 的镜像。

**位置**

Linux Docker 主机本地镜像仓库通常位于` /var/lib/docker/<storage-driver>，`Windows Docker 主机则是 C:\ProgramData\docker\windowsfilter。

**查看镜像**

可以使用以下命令检查 Docker 主机的本地仓库中是否包含镜像。

```
docker image ls
```



## 镜像仓库服务

### 镜像仓库服务

Docker 镜像存储在镜像仓库服务（Image Registry）当中。

Docker 客户端的镜像仓库服务是可配置的，默认使用 Docker Hub。

镜像仓库服务包含多个镜像仓库（Image Repository）。同样，一个镜像仓库中可以包含多个镜像。

![](/images/docker-image-reposerver.gif)

#### 官方和非官方镜像仓库

Docker Hub 也分为官方仓库（Official Repository）和非官方仓库（Unofficial Repository）。

官方仓库中的镜像是由 Docker 公司审查的。这意味着其中的镜像会及时更新，由高质量的代码构成，这些代码是安全的，有完善的文档和最佳实践。

非官方仓库更像江湖侠客，其中的镜像不一定具备官方仓库的优点，但这并不意味着所有非官方仓库都是不好的！非官方仓库中也有一些很优秀的镜像。

## 镜像命名和标签

只需要给出镜像的名字和标签，就能在官方仓库中定位一个镜像（采用“:”分隔）。从官方仓库拉取镜像时，docker image pull 命令的格式如下。

```sh
docker image pull <repository>:<tag>
```

```sh
$ docker image pull alpine:latest
$ docker image pull ubuntu:latest
```

这两条命令从 alpine 和 ubuntu 仓库拉取了标有“latest”标签的镜像。

如果没有在仓库名称后指定具体的镜像标签，则 Docker 会假设用户希望拉取标签为 latest 的镜像。



标签为 latest 的镜像没有什么特殊魔力！标有 latest 标签的镜像不保证这是仓库中最新的镜像！例如，Alpine 仓库中最新的镜像通常标签是 *edge*。通常来讲，使用 *latest* 标签时需要谨慎！

从非官方仓库拉取镜像也是类似的，读者只需要在仓库名称面前加上 Docker Hub 的用户名或者组织名称。

```sh
$ docker image pull nigelpoulton/tu-demo:v2
```

## 为镜像打多个标签

一个镜像可以根据用户需要设置多个标签。这是因为标签是存放在镜像元数据中的任意数字或字符串。

在 `docker image pull` 命令中指定 -a 参数来拉取仓库中的全部镜像。接下来可以通过运行 `docker image ls` 查看已经拉取的镜像。

## 镜像查询过滤

那些没有标签的镜像被称为悬虚镜像，在列表中展示为`<none>:<none>`

```shell
$ docker image ls --filter dangling=true
REPOSITORY TAG IMAGE ID CREATED SIZE
<none> <none> 4fd34165afe0 7 days ago 14.5MB
```

通常出现这种情况，**是因为构建了一个新镜像，然后为该镜像打了一个已经存在的标签**。

当此情况出现，Docker 会构建新的镜像，然后发现已经有镜像包含相同的标签，接着 Docker 会移除旧镜像上面的标签，将该标签标在新的镜像之上。

Docker 目前支持如下的过滤器。

- dangling：可以指定 true 或者 false，仅返回悬虚镜像（true），或者非悬虚镜像（false）。
- before：需要镜像名称或者 ID 作为参数，返回在之前被创建的全部镜像。
- since：与 before 类似，不过返回的是指定镜像之后创建的全部镜像。
- label：根据标注（label）的名称或者值，对镜像进行过滤。docker image ls命令输出中不显示标注内容。

其他的过滤方式可以使用 reference。

```sh
$ docker image ls --filter=reference="*:latest"
REPOSITORY TAG IMAGE ID CREATED SIZE
alpine latest 3fd9065eaf02 8 days ago 4.15MB
test latest 8426e7efb777 3 days ago 122MB
```

可以使用 --format 参数来通过 Go 模板对输出内容进行格式化。

```sh
$ docker image ls --format "{{.Size}}"
99.3MB
111MB
82.6MB
88.8MB
4.15MB
108MB
```

使用下面命令返回全部镜像，但是只显示仓库、标签和大小信息。

```shell
$ docker image ls --format "{{.Repository}}: {{.Tag}}: {{.Size}}"
dodge: challenger: 99.3MB
ubuntu: latest: 111MB
python: 3.4-alpine: 82.6MB
python: 3.5-alpine: 88.8MB
alpine: latest: 4.15MB
nginx: latest: 108MB
```

## 通过 CLI 方式搜索 Docker Hub

`docker search` 命令允许通过 CLI 的方式搜索 Docker Hub。可以通过“NAME”字段的内容进行匹配，并且基于返回内容中任意列的值进行过滤。

简单模式下，该命令会搜索所有“NAME”字段中包含特定字符串的仓库。例如，下面的命令会查找所有“NAME”包含“nigelpoulton”的仓库。

```shell
$ docker search nigelpoulton
```

需要注意，上面返回的镜像中既有官方的也有非官方的。读者可以使用 --filter "is-official=true"，使命令返回内容只显示官方镜像。

```sh
$ docker search alpine --filter "is-official=true"
NAME DESCRIPTION STARS OFFICIAL AUTOMATED
alpine A minimal Docker.. 2988 [OK]
```

重复前面的操作，但这次只显示自动创建的仓库。

```sh
$ docker search alpine --filter "is-automated=true"
NAME DESCRIPTION OFFICIAL AUTOMATED
anapsix/alpine-java Oracle Java 8 (and 7).. [OK]
frolvlad/alpine-glibc Alpine Docker image.. [OK]
kiasaki/alpine-postgres PostgreSQL docker.. [OK]
zzrot/alpine-caddy Caddy Server Docker.. [OK]
<Snip>
```

关于 `docker search` 需要注意的最后一点是，默认情况下，Docker 只返回 25 行结果。但是，可以通过指定 --limit 参数来增加返回内容行数，最多为 100 行。



## 镜像和分层

Docker 镜像由一些松耦合的只读镜像层组成。如下图所示。

![](/images/docker-image-layer.gif)

Docker 负责堆叠这些镜像层，并且将它们表示为单个统一的对象。

查看镜像分层的方式可以通过 docker image inspect 命令。下面同样以 ubuntu:latest 镜像为例。

```shell
$ docker image inspect ubuntu:latest
[
{
"Id": "sha256:bd3d4369ae.......fa2645f5699037d7d8c6b415a10",
"RepoTags": [
"ubuntu:latest"

<Snip>

"RootFS": {
  "Type": "layers",
  "Layers": [
   "sha256:c8a75145fc...894129005e461a43875a094b93412",
   "sha256:c6f2b330b6...7214ed6aac305dd03f70b95cdc610",
   "sha256:055757a193...3a9565d78962c7f368d5ac5984998",
   "sha256:4837348061...12695f548406ea77feb5074e195e3",
   "sha256:0cad5e07ba...4bae4cfc66b376265e16c32a0aae9"
  ]
  }
}
]
```

缩减之后的输出也显示该镜像包含 5 个镜像层。只不过这次的输出内容中使用了镜像的 SHA256 散列值来标识镜像层。不过，两中命令都显示了镜像包含 5 个镜像层。

`docker history` 命令显示了镜像的构建历史记录，但其并不是严格意义上的镜像分层。例如，有些 Dockerfile 中的指令并不会创建新的镜像层。比如 ENV、EXPOSE、CMD 以及 ENTRY- POINT。不过，这些命令会在镜像中添加元数据。

所有的 Docker 镜像都起始于一个基础镜像层，当进行修改或增加新的内容时，就会在当前镜像层之上，创建新的镜像层。

举一个简单的例子，假如基于 Ubuntu Linux 16.04 创建一个新的镜像，这就是新镜像的第一层；如果在该镜像中添加 [Python](http://c.biancheng.net/python/) 包，就会在基础镜像层之上创建第二个镜像层；如果继续添加一个安全补丁，就会创建第三个镜像层。



![](/images/docker-image-layer-three-layer.gif)

在外部看来整个镜像只有 6 个文件，这是因为最上层中的文件 7 是文件 5 的一个更新版本。

这种情况下，上层镜像层中的文件覆盖了底层镜像层中的文件。这样就使得文件的更新版本作为一个新镜像层添加到镜像当中。

Docker 通过存储引擎（新版本采用快照机制）的方式来实现镜像层堆栈，并保证多镜像层对外展示为统一的文件系统。

## 共享镜像层

多个镜像之间可以并且确实会共享镜像层。这样可以有效节省空间并提升性能。

```shell
$ docker image pull -a nigelpoulton/tu-demo

latest: Pulling from nigelpoulton/tu-demo
237d5fcd25cf: Pull complete
a3ed95caeb02: Pull complete
<Snip>
Digest: sha256:42e34e546cee61adb100...a0c5b53f324a9e1c1aae451e9

v1: Pulling from nigelpoulton/tu-demo
237d5fcd25cf: Already exists
a3ed95caeb02: Already exists
<Snip>
Digest: sha256:9ccc0c67e5c5eaae4beb...24c1d5c80f2c9623cbcc9b59a

v2: Pulling from nigelpoulton/tu-demo
237d5fcd25cf: Already exists
a3ed95caeb02: Already exists
<Snip>
eab5aaac65de: Pull complete
Digest: sha256:d3c0d8c9d5719d31b79c...fef58a7e038cf0ef2ba5eb74c

Status: Downloaded newer image for nigelpoulton/tu-demo

$ docker image ls
REPOSITORY TAG IMAGE ID CREATED SIZE
nigelpoulton/tu-demo v2 6ac...ead 4 months ago 211.6 MB
nigelpoulton/tu-demo latest 9b9...e29 4 months ago 211.6 MB
nigelpoulton/tu-demo v1 9b9...e29 4 months ago 211.6 MB
```

注意那些以 Already exists 结尾的行。

由这几行可见，Docker 很聪明，可以识别出要拉取的镜像中，哪几层已经在本地存在。

在本例中，Docker 首先尝试拉取标签为 latest 的镜像。然后，当拉取标签为 v1 和 v2 的镜像时，Docker 注意到组成这两个镜像的镜像层，有一部分已经存在了。出现这种情况的原因是前面 3 个镜像相似度很高，所以共享了很多镜像层。

如前所述，Docker 在 Linux 上支持很多存储引擎（Snapshotter）。每个存储引擎都有自己的镜像分层、镜像层共享以及写时复制（CoW）技术的具体实现。

但是，其最终效果和用户体验是完全一致的。尽管 Windows 只支持一种存储引擎，还是可以提供与 Linux 相同的功能体验。

## 根据摘要拉取镜像

咱们前面介绍了通过标签来拉取镜像，这也是常见的方式。但问题是，标签是可变的！这意味着可能偶尔出现给镜像打错标签的情况，有时甚至会给新镜像打一个已经存在的标签。这些都可能导致问题！

假设镜像 golftrack:1.5 存在一个已知的 Bug。因此可以拉取该镜像后修复它，并使用相同的标签将更新的镜像重新推送回仓库。

一起来思考下刚才发生了什么。镜像 golftrack:1.5 存在 Bug，这个镜像已经应用于生产环境。如果创建一个新版本的镜像，并修复了这个 Bug。

那么问题来了，构建新镜像并将其推送回仓库时使用了与问题镜像相同的标签！原镜像被覆盖，但在生产环境中遗留了大量运行中的容器，没有什么好办法区分正在使用的镜像版本是修复前还是修复后的，因为两个镜像的标签是相同的！

Docker 1.10 中引入了新的内容寻址存储模型。作为模型的一部分，每一个镜像现在都有一个基于其内容的密码散列值。

为了讨论方便，用摘要代指这个散列值。因为摘要是镜像内容的一个散列值，所以镜像内容的变更一定会导致散列值的改变。这意味着摘要是不可变的。这种方式可以解决前面讨论的问题。

每次拉取镜像，摘要都会作为 `docker image pull` 命令返回代码的一部分。只需要在 `docker image ls` 命令之后添加 --digests 参数即可在本地查看镜像摘要。

```sh
$ docker image pull alpine
Using default tag: latest
latest: Pulling from library/alpine
e110a4a17941: Pull complete
Digest: sha256:3dcdb92d7432d56604d...6d99b889d0626de158f73a
Status: Downloaded newer image for alpine:latest

$ docker image ls --digests alpine
REPOSITORY TAG DIGEST IMAGE ID CREATED SIZE
alpine latest sha256:3dcd...f73a 4e38e38c8ce0 10 weeks ago 4.8 MB
```



## 镜像散列值（摘要）

从 Docker 1.10 版本开始，镜像就是一系列松耦合的独立层的集合。

镜像本身就是一个配置对象，其中包含了镜像层的列表以及一些元数据信息。

镜像层才是实际数据存储的地方（比如文件等，镜像层之间是完全独立的，并没有从属于某个镜像集合的概念）。

镜像的唯一标识是一个加密 ID，即配置对象本身的散列值。每个镜像层也由一个加密 ID 区分，其值为镜像层本身内容的散列值。

这意味着修改镜像的内容或其中任意的镜像层，都会导致加密散列值的变化。所以，镜像和其镜像层都是不可变的，任何改动都能很轻松地被辨别。

这就是所谓的内容散列（Content Hash）。

到目前为止，事情都很简单。但是接下来的内容就有点儿复杂了。

在推送和拉取镜像的时候，都会对镜像层进行压缩来节省网络带宽以及仓库二进制存储空间。

但是压缩会改变镜像内容，这意味着镜像的内容散列值在推送或者拉取操作之后，会与镜像内容不相符！这显然是个问题。

例如，在推送镜像层到 Docker Hub 的时候，Docker Hub 会尝试确认接收到的镜像没有在传输过程中被篡改。

为了完成校验，Docker Hub 会根据镜像层重新计算散列值，并与原散列值进行比较。

因为镜像在传输过程中被压缩（发生了改变），所以散列值的校验也会失败。

为避免该问题，每个镜像层同时会包含一个分发散列值（Distribution Hash）。这是一个压缩版镜像的散列值，当从镜像仓库服务拉取或者推送镜像的时候，其中就包含了分发散列值，该散列值会用于校验拉取的镜像是否被篡改过。

这个内容寻址存储模型极大地提升了镜像的安全性，因为在拉取和推送操作后提供了一种方式来确保镜像和镜像层数据是一致的。

该模型也解决了随机生成镜像和镜像层 ID 这种方式可能导致的 ID 冲突问题。



## 多层架构的镜像

Docker 最值得称赞的一点就是使用方便。例如，运行一个应用就像拉取镜像并运行容器这么简单。无须担心安装、依赖或者配置的问题。开箱即用。

但是，随着 Docker 的发展，事情开始变得复杂——尤其是在添加了新平台和架构之后，例如 Windows、ARM 以及 s390x。

这是会突然发现，在拉取镜像并运行之前，需要考虑镜像是否与当前运行环境的架构匹配，这破坏了 Docker 的流畅体验。

多架构镜像（Multi-architecture Image）的出现解决了这个问题！

Docker（镜像和镜像仓库服务）规范目前支持多架构镜像。这意味着某个镜像仓库标签（repository:tag）下的镜像可以同时支持 64 位 Linux、PowerPC Linux、64 位 Windows 和 ARM 等多种架构。

简单地说，就是一个镜像标签之下可以支持多个平台和架构。下面通过实操演示该特性。

为了实现这个特性，镜像仓库服务 API 支持两种重要的结构：Manifest 列表（新）和 Manifest。

Manifest 列表是指某个镜像标签支持的架构列表。其支持的每种架构，都有自己的 Mainfest 定义，其中列举了该镜像的构成。

下图使用 Golang 官方镜像作为示例。图左侧是 Manifest 列表，其中包含了该镜像支持的每种架构。

Manifest 列表的每一项都有一个箭头，指向具体的 Manifest，其中包含了镜像配置和镜像层数据。



![](/images/docker-image-golang-multiarchitecture.gif)

在具体操作之前，先来了解一下原理。

假设要在 Raspberry Pi（基于 ARM 架构的 Linux）上运行 Docker。

在拉取镜像的时候，Docker 客户端会调用 Docker Hub 镜像仓库服务相应的 API 完成拉取。

如果该镜像有 Mainfest 列表，并且存在 Linux on ARM 这一项，则 Docker Client 就会找到 ARM 架构对应的 Mainfest 并解析出组成该镜像的镜像层加密 ID。

然后从 Docker Hub 二进制存储中拉取每个镜像层。

下面的示例就展示了多架构镜像是如何在拉取官方 Golang 镜像（支持多架构）时工作的，并且通过一个简单的命令展示了 Go 的版本和所在主机的 CPU 架构。

需要注意的是，两个例子都使用相同的命令 docker container run。不需要告知 Docker 具体的镜像版本是 64 位 Linux 还是 64 位 Windows。



示例中只运行了普通的命令，选择当前平台和架构所需的正确镜像版本是有由 Docker 完成的。



## 删除镜像

当读者不再需要某个镜像的时候，可以通过 `docker image rm` 命令从 Docker 主机删除该镜像。其中，rm 是 remove 的缩写。

```
docker image rm 02674b9cb179
```

## 镜像常用命令总结

```
docker image pull
docker image pull alpine:latest
docker image ls
docker image inspect
docker image rm
```

# 容器

## 简介

```sh
docker container run <image> <app>

# 会启动某个 Ubuntu Linux 容器，并运行 Bash Shell 作为其应用。
docker container run -it ubuntu /bin/bash
# 启动 PowerShell 并运行一个应用，则可以使用命令
docker container run -it microsoft- /powershell:nanoserver pwsh.exe
# 运行ubuntu 休眠10s
docker container run  ubuntu sleep 10
# 停止容器
docker container stop
# 开启容器
docker container start
```

-it 参数可以将当前终端连接到容器的 Shell 终端之上。

容器随着其中运行应用的退出而终止。其中 Linux 容器会在 Bash Shell 退出后终止，而 Windows 容器会在 PowerShell 进程终止后退出。

一个简单的验证方法就是启动新的容器，并运行 sleep 命令休眠 10s。容器会启动，然后运行休眠命令，在 10s 后退出。

## 容器和虚拟机

容器和虚拟机都依赖于宿主机才能运行。宿主机可以是笔记本，是数据中心的物理服务器，也可以是公有云的某个实例。

在下面的示例中，假设宿主机是一台需要运行 4 个业务应用的物理服务器。

在虚拟机模型中，首先要开启物理机并启动 Hypervisor 引导程序。一旦 Hypervisor 启动，就会占有机器上的全部物理资源，如 CPU、RAM、存储和 NIC。

Hypervisor 接下来就会将这些物理资源划分为虚拟资源，并且看起来与真实物理资源完全一致。

然后 Hypervisor 会将这些资源打包进一个叫作虚拟机（VM）的软件结构当中。这样用户就可以使用这些虚拟机，并在其中安装操作系统和应用。

前面提到需要在物理机上运行 4 个应用，所以在 Hypervisor 之上需要创建 4 个虚拟机并安装 4 个操作系统，然后安装 4 个应用。当操作完成后，结构如下图所示。

![](/images/vm_structure.gif)





而容器模型则略有不同。

服务器启动之后，所选择的操作系统会启动。在 Docker 世界中可以选择 Linux，或者内核支持内核中的容器原语的新版本 Windows。

与虚拟机模型相同，OS 也占用了全部硬件资源。在 OS 层之上，需要安装容器引擎（如 Docker）。

容器引擎可以获取系统资源，比如进程树、文件系统以及网络栈，接着将资源分割为安全的互相隔离的资源结构，称之为容器。

每个容器看起来就像一个真实的操作系统，在其内部可以运行应用。按照前面的假设，需要在物理机上运行 4 个应用。

![](/images/docker-structure.gif)

从更高层面上来讲，Hypervisor 是硬件虚拟化（Hardware Virtualization）——Hypervisor 将硬件物理资源划分为虚拟资源。

容器是操作系统虚拟化（OS Virtualization）——容器将系统资源划分为虚拟资源。

## 虚拟机的额外开销

基于前文所述内容，接下来会着重探讨 Hypervisor 模型的一个主要问题。

首先我们的目标是在一台物理机上运行 4 个业务相关应用。每种模型示例中都安装了一个操作系统或者 Hypervisor（一种针对虚拟机高度优化后的操作系统）。

虚拟机模型将底层硬件资源划分到虚拟机当中。每个虚拟机都是包含了虚拟 CPU、虚拟 RAM、虚拟磁盘等资源的一种软件结构。

因此，每个虚拟机都需要有自己的操作系统来声明、初始化并管理这些虚拟资源。

但是，操作系统本身是有其额外开销的。例如，每个操作系统都消耗一点 CPU、一点 RAM、一点存储空间等。

每个操作系统都需要独立的许可证，并且都需要打补丁升级，每个操作系统也都面临被攻击的风险。

通常将这种现象称作 OS Tax 或者 VM Tax，每个操作系统都占用一定的资源。

容器模型具有在宿主机操作系统中运行的单个内核。在一台主机上运行数十个甚至数百个容器都是可能的——容器共享一个操作系统/内核。

这意味着只有一个操作系统消耗 CPU、RAM 和存储资源，只有一个操作系统需要授权，只有一个操作系统需要升级和打补丁。同时，只有一个操作系统面临被攻击的风险。简言之，就是只有一份 OS 损耗。

在上述单台机器上只需要运行 4 个业务应用的场景中，也许问题尚不明显。但当需要运行成百上千应用的时候，就会引起质的变化。

另一个值得考虑的事情是启动时间。因为容器并不是完整的操作系统，所以其启动要远比虚拟机快。

切记，在容器内部并不需要内核，也就没有定位、解压以及初始化的过程——更不用提在内核启动过程中对硬件的遍历和初始化了。

这些在容器启动的过程中统统都不需要！唯一需要的是位于下层操作系统的共享内核是启动了的！最终结果就是，容器可以在 1s 内启动。唯一对容器启动时间有影响的就是容器内应用启动所花费的时间。

这就是容器模型要比虚拟机模型简洁并且高效的原因了。使用容器可以在更少的资源上运行更多的应用，启动更快，并且支付更少的授权和管理费用，同时面对未知攻击的风险也更小。

## 检查 Docker daemon

```sh
$ docker version
Client:
Version: API 17.05.0-ce
version: Go 1.29
version: Git go1.7.5
commit: 89658be
Built: Thu May 4 22:10:54 2017
OS/Arch: linux/amd64

Server:
Version: 17.05.0-ce
API version: 1.29 (minimum version 1.12)
Go version: go1.7.5
Git commit: 89658be
Built: Thu May 4 22:10:54 2017
OS/Arch: linux/amd64
Experimental: false
```



当命令输出中包含 Client 和 Server 的内容时，可以继续下面的操作。如果在 Server 部分中包含了错误码，这表示 Docker daemon 很可能没有运行，或者当前用户没有权限访问。

如果在 Linux 中遇到无权限访问的问题，需要确认当前用户是否属于本地 Docker UNIX 组。如果不是，可以通过`usermod -aG docker <user>`来添加，然后退出并重新登录 Shell，改动即可生效。

如果当前用户已经属于本地 docker 用户组，那么问题可能是 Docker daemon 没有运行导致。

```sh
//使用 Systemd 在 Linux 系统中执行该命令
$ service docker status
docker start/running, process 29393

//使用Systemd在Linux系统中执行该命令
$ systemctl is-active docker
active

//在Windows Server 2016的PowerShell窗口中运行该命令
> Get-Service docker

Status Name DisplayName
------ ---- -----------
Running Docker docker
```

## 启动一个简单容器

```sh
#Windows 示例。

$ docker container run -it microsoft/powershell:nanoserver pwsh.exe

#命令的基础格式为：
# 示例中使用 docker container run 来启动容器，这也是启动新容器的标准命令。

$ docker container run <options> <im- age>:<tag> <app>
```

## 容器生命周期

```sh
# 新建容器 名称为 precy
$ docker container run --name percy -it ubuntu:latest /bin/bash
#暂停容器
$ docker container stop <container-id or container-name>
# 写输入到容器
$ root@9cb2d2fd1d65:/tmp# echo "DevOps FTW" > newfile
# 退出容器
ctrl +QP
# 暂停容器
$ docker container stop percy
# 查看运行中的容器
$ docker container ls
# 所有容器
$ docker container ls -a
# 启动容器
$ docker container start percy
# 连接容器
$ docker container exec -it percy bash
$ docker container stop percy
# 删除容器
$ docker container rm percy

```

## 优雅地停止容器

```
# 向容器内的 PID 1 进程发送了 SIGTERM 这样的信号。
$ docker container stop
```

就像前文提到的一样，会为进程预留一个清理并优雅停止的机会。如果 10s 内进程没有终止，那么就会收到 SIGKILL 信号。这是致命一击。但是，进程起码有 10s 的时间来“解决”自己。

`docker container rm <container> -f` 命令不会先友好地发送 SIGTERM，这条命令会直接发出 SIGKILL。就像刚刚所打的比方一样，该命令悄悄接近并对容器发起致命一击。



## 利用重启策略进行容器的自我修复

通常建议在运行容器时配置好重启策略。这是容器的一种自我修复能力，可以在指定事件或者错误后重启来完成自我修复。

重启策略应用于每个容器，可以作为参数被强制传入 `docker-container run` 命令中，或者在 Compose 文件中声明（在使用 Docker Compose 以及 Docker Stacks 的情况下）。

容器支持的重启策略包括 

* always
* unless-stopped 
* on-failed

always 策略是一种简单的方式。除非容器被明确停止，比如通过 `docker container stop` 命令，否则该策略会一直尝试重启处于停止状态的容器。

## 容器常用命令

```sh
# 启动新容器的命令。该命令的最简形式接收镜像和命令作为参数。镜像用于创建容器，而命令则是希望容器运行的应用。
# Ctrl-PQ 会断开 Shell 和容器终端之间的链接，并在退出后保持容器在后台处于运行（UP）状态。
$ docker container run
# 此命令会停止运行中的容器，并将状态置为 Exited(0)。
$ docker container ls
# 用于在运行状态的容器中，启动一个新进程。该命令在将 Docker 主机 Shell 连接到一个运行中容器终端时非常有用。
$ docker container exec
#此命令会停止运行中的容器，并将状态置为 Exited(0)。
#该命令通过发送 SIGTERM 信号给容器内 PID 为 1 的进程达到目的。
#如果进程没有在 10s 之内得到清理并停止运行，那么会接着发送 SIGKILL 信号来强制停止该容器。
$ docker container stop
# 重启处于停止（Exited）状态的容器。可以在 docker container start 命令中指定容器的名称或者 ID。
$ docker container start
# 删除停止运行的容器
$ docker container rm
# 该命令接收容器名称和容器 ID 作为主要参数。
$ docker container inspect
```



# 应用容器化

## 概述

*Docker* 的核心思想就是如何将应用整合到容器中，并且能在容器中实际运行。

将应用整合到容器中并且运行起来的这个过程，称为“容器化”（Containerizing），有时也叫作“Docker化”（Dockerizing）。

容器是为应用而生的，具体来说，容器能够简化应用的构建、部署和运行过程。

完整的应用容器化过程主要分为以下几个步骤。

- 编写应用代码。
- 创建一个 Dockerfile，其中包括当前应用的描述、依赖以及该如何运行这个应用。
- 对该 Dockerfile 执行 `docker image build` 命令。
- 等待 Docker 将应用程序构建到 Docker 镜像中。

一旦应用容器化完成（即应用被打包为一个 Docker 镜像），就能以镜像的形式交付并以容器的方式运行了。

## 单体应用容器化

应用容器化的过程大致分为如下几个步骤：

- 获取应用代码。
- 分析 Dockerfile。
- 构建应用镜像。
- 运行该应用。
- 测试应用。
- 容器应用化细节。
- 生产环境中的多阶段构建。
- 最佳实践。

```
FROM alpine # 以 alpine 镜像作为当前镜像基础
LABEL maintainer="nigelpoulton@hotmail.com" #指定维护者（maintainer）为“nigelpoultion@hotmail.com”
RUN apk add --update nodejs nodejs-npm # 安装 Node.js 和 NPM
COPY . /src #将应用的代码复制到镜像当中
WORKDIR /src #设置新的工作目录
RUN npm install #安装依赖包
EXPOSE 8080 #记录应用的网络端口，
ENTRYPOINT ["node", "./app.js"] #最后将 app.js 设置为默认运行的应用。
```

Dockerfile 主要包括两个用途：

- 对当前应用的描述。
- 指导 Docker 完成应用的容器化（创建一个包含当前应用的镜像）。

不要因 Dockerfile 就是一个描述文件而对其有所轻视！Dockerfile 能实现开发和部署两个过程的无缝切换。

同时 Dockerfile 还能帮助新手快速熟悉这个项目。Dockerfile 对当前的应用及其依赖有一个清晰准确的描述，并且非常容易阅读和理解。

**每个 Dockerfile 文件第一行都是 FROM 指令。**

FROM 指令指定的镜像，会作为当前镜像的一个基础镜像层，当前应用的剩余内容会作为新增镜像层添加到基础镜像层之上。

**Example解读**

本例中的应用基于 Linux 操作系统，所以在 FROM 指令当中所引用的也是一个 Linux 基础镜像；如果要容器化的应用是一个基于 Windows 操作系统的应用，就需要指定一个像 microsoft/aspnetcore-build 这样的 Windows 基础镜像了。

截至目前，基础镜像的结构如下图所示。

![](/images/docker-layer-basic-1.gif)



接下来，Dockerfile 中通过标签（LABLE）方式指定了当前镜像的维护者为“nigelpoulton@hotmail. com”。

每个标签其实是一个键值对（Key-Value），在一个镜像当中可以通过增加标签的方式来为镜像添加自定义元数据。

`RUN apk add --update nodejs nodejs-npm` 指令使用 alpine 的 apk 包管理器将 nodejs 和 nodejs-npm 安装到当前镜像之中。

RUN 指令会在 FROM 指定的 alpine 基础镜像之上，新建一个镜像层来存储这些安装内容。当前镜像的结构如下图所示。

COPY. / src 指令将应用相关文件从构建上下文复制到了当前镜像中，并且新建一个镜像层来存储。COPY 执行结束之后，当前镜像共包含 3 层，如下图所示。

![](/images/docker-layer-2.gif)



下一步，Dockerfile 通过 WORKDIR 指令，为 Dockerfile 中尚未执行的指令设置工作目录。

该目录与镜像相关，并且会作为元数据记录到镜像配置中，但不会创建新的镜像层。

然后，`RUN npm install` 指令会根据 package.json 中的配置信息，使用 npm 来安装当前应用的相关依赖包。



目前镜像一共包含 4 层，如下图所示。

![](/images/docker-layer-3.gif)



因为当前应用需要通过 TCP 端口 8080 对外提供一个 Web 服务，所以在 Dockerfile 中通过 EXPOSE 8080 指令来完成相应端口的设置。

这个配置信息会作为镜像的元数据被保存下来，并不会产生新的镜像层。

最终，通过 ENTRYPOINT 指令来指定当前镜像的入口程序。

ENTRYPOINT 指定的配置信息也是通过镜像元数据的形式保存下来，而不是新增镜像层。

**启动容器**

```sh
#-d 参数的作用是让应用程序以守护线程的方式在后台运行。
# -p 80:8080 参数的作用是将主机的80端口与容器内的8080端口进行映射。
$ docker container run -d --name c1 -p 80:8080  web:latest 
```

`docker image history`

每行内容都对应了 Dockerfile 中的一条指令（顺序是自下而上）。CREATE BY 这一列中还展示了当前行具体对应 Dockerfile 中的哪条指令。

其次，从这个输出内容中，可以观察到只有 4 条指令会新建镜像层（就是那些 SIZE 列对应的数值不为零的指令），分别对应 Dockerfile 中的 FROM、RUN 以及 COPY 指令。

虽然其他指令看上去跟这些新建镜像层的指令并无区别，但实际上它们只在镜像中新增了元数据信息。这些指令之所以看起来没有区别，是因为 Docker 对之前构建镜像层方式的兼容。

使用 FROM 指令引用官方基础镜像是一个很好的习惯，这是因为官方的镜像通常会遵循一些最佳实践，并且能帮助使用者规避一些已知的问题。

通过 `docker image build` 命令具体的输出内容，可以了解镜像构建的过程。

在下面的片段中，可以看到基本的构建过程是，运行临时容器 -> 在该容器中运行 Dockerfile 中的指令 -> 将指令运行结果保存为一个新的镜像层 -> 删除临时容器。

```
Step 3/8 : RUN apk add --update nodejs nodejs-npm
---> Running in e690ddca785f << Run inside of temp container
fetch http://dl-cdn...APKINDEX.tar.gz
fetch http://dl-cdn...APKINDEX.tar.gz
(1/10) Installing ca-certificates (20171114-r0)
<Snip>
OK: 61 MiB in 21 packages
---> c1d31d36b81f << Create new layer
Removing intermediate container << Remove temp container
Step 4/8 : COPY . /src
```

## 生产环境中的多阶段构建

### 过大体积的问题

对于 Docker 镜像来说，过大的体积并不好！

越大则越慢，这就意味着更难使用，而且可能更加脆弱，更容易遭受攻击。

鉴于此，Docker 镜像应该尽量小。对于生产环境镜像来说，目标是将其缩小到仅包含运行应用所必需的内容即可。问题在于，生成较小的镜像并非易事。

不同的 Dockerfile 写法就会对镜像的大小产生显著影响。

**使用 && 连接多个命令**

常见的例子是，每一个 RUN 指令会新增一个镜像层。因此，通过使用 && 连接多个命令以及使用反斜杠（\）换行的方法，将多个命令包含在一个 RUN 指令中，通常来说是一种值得提倡的方式。

**构建工具残留**

另一个问题是开发者通常不会在构建完成后进行清理。当使用 RUN 执行一个命令时，可能会拉取一些构建工具，这些工具会留在镜像中移交至生产环境。

**采用构建者模式**

有多种方式来改善这一问题——比如常见的是采用建造者模式（Builder Pattern）。但无论采用哪种方式，通常都需要额外的培训，并且会增加构建的复杂度。

建造者模式需要至少两个 Dockerfile，一个用于开发环境，一个用于生产环境。

首先需要编写 Dockerfile.dev，它基于一个大型基础镜像（Base Image），拉取所需的构建工具，并构建应用。

接下来，需要基于 Dockerfile.dev 构建一个镜像，并用这个镜像创建一个容器。

这时再编写 Dockerfile.prod，它基于一个较小的基础镜像开始构建，并从刚才创建的容器中将应用程序相关的部分复制过来。

整个过程需要编写额外的脚本才能串联起来。

这种方式是可行的，但是比较复杂。

多阶段构建（Multi-Stage Build）是一种更好的方式！

多阶段构建能够在不增加复杂性的情况下优化构建过程。

### **下面介绍一下多阶段构建方式**

多阶段构建方式使用一个 Dockerfile，其中包含多个 FROM 指令。每一个 FROM 指令都是一个新的构建阶段（Build Stage），并且可以方便地复制之前阶段的构件。

```
FROM node:latest AS storefront
WORKDIR /usr/src/atsea/app/react-app
COPY react-app .
RUN npm install
RUN npm run build

FROM maven:latest AS appserver
WORKDIR /usr/src/atsea
COPY pom.xml .
RUN mvn -B -f pom.xml -s /usr/share/maven/ref/settings-docker.xml dependency
\:resolve
COPY . .
RUN mvn -B -s /usr/share/maven/ref/settings-docker.xml package -DskipTests

FROM java:8-jdk-alpine AS production
RUN adduser -Dh /home/gordon gordon
WORKDIR /static
COPY --from=storefront /usr/src/atsea/app/react-app/build/ .
WORKDIR /app
COPY --from=appserver /usr/src/atsea/target/AtSea-0.0.1-SNAPSHOT.jar .
ENTRYPOINT ["java", "-jar", "/app/AtSea-0.0.1-SNAPSHOT.jar"]
CMD ["--spring.profiles.active=postgres"]
```

首先注意到，Dockerfile 中有 3 个 FROM 指令。每一个 FROM 指令构成一个单独的构建阶段。

各个阶段在内部从 0 开始编号。不过，示例中针对每个阶段都定义了便于理解的名字。

- 阶段 0 叫作 storefront。
- 阶段 1 叫作 appserver。
- 阶段 2 叫作 production。

**storefront** 

storefront 阶段拉取了大小超过 600MB 的 node:latest 镜像，然后设置了工作目录，复制一些应用代码进去，然后使用 2 个 RUN 指令来执行 npm 操作。

这会生成 3 个镜像层并显著增加镜像大小。指令执行结束后会得到一个比原镜像大得多的镜像，其中包含许多构建工具和少量应用程序代码。

**appserver** 

appserver 阶段拉取了大小超过 700MB 的 maven:latest 镜像。然后通过 2 个 COPY 指令和 2 个 RUN 指令生成了 4 个镜像层。

这个阶段同样会构建出一个非常大的包含许多构建工具和非常少量应用程序代码的镜像。

production 阶段拉取 java:8-jdk-alpine 镜像，这个镜像大约 150MB，明显小于前两个构建阶段用到的 node 和 maven 镜像。

这个阶段会创建一个用户，设置工作目录，从 storefront 阶段生成的镜像中复制一些应用代码过来。

之后，设置一个不同的工作目录，然后从 appserver 阶段生成的镜像中复制应用相关的代码。最后，production 设置当前应用程序为容器启动时的主程序。

重点在于 COPY --from 指令，它从之前的阶段构建的镜像中仅复制生产环境相关的应用代码，而不会复制生产环境不需要的构件。

还有一点也很重要，多阶段构建这种方式仅用到了一个 Dockerfile，并且 `docker image build` 命令不需要增加额外参数。

```sh
 docker image build -t multi:stage .
# 示例中 multi:stage 标签是自行定义的，可以根据自己的需要和规范来指定标签名称。不过并不要求一定必须为多阶段构建指定标签。
```

可见它明显比之前阶段拉取和生成的镜像要小。这是因为该镜像是基于相对精简的 java:8-jdk-alpine 镜像构建的，并且仅添加了用于生产环境的应用程序文件。

最终，无须额外的脚本，仅对一个单独的 Dockerfile 执行 `docker image build` 命令，就创建了一个精简的生产环境镜像。

多阶段构建是随 Docker 17.05 版本新增的一个特性，用于构建精简的生产环境镜像。

## 最佳实践

### 利用构建缓存

Docker 的构建过程利用了缓存机制。观察缓存效果的一个方法，就是在一个干净的 Docker 主机上构建一个新的镜像，然后再重复同样的构建。

第一次构建会拉取基础镜像，并构建镜像层，构建过程需要花费一定时间；第二次构建几乎能够立即完成。

这就是因为第一次构建的内容（如镜像层）能够被缓存下来，并被后续的构建过程复用。

`docker image build` 命令会从顶层开始解析 Dockerfile 中的指令并逐行执行。而对每一条指令，Docker 都会检查缓存中是否已经有与该指令对应的镜像层。

如果有，即为缓存命中（Cache Hit），并且会使用这个镜像层；如果没有，则是缓存未命中（Cache Miss），Docker 会基于该指令构建新的镜像层。

缓存命中能够显著加快构建过程。

```
FROM alpine
RUN apk add --update nodejs nodejs-npm
COPY . /src
WORKDIR /src
RUN npm install
EXPOSE 8080
ENTRYPOINT ["node", "./app.js"]
```

**缓存策略**

如果主机中已经存在这个镜像，那么构建时会直接跳到下一条指令；如果镜像不存在，则会从 Docker Hub（docker.io）拉取。

下一条指令（RUN apk...）对镜像执行一条命令。

此时，Docker 会检查构建缓存中是否存在基于同一基础镜像，并且执行了相同指令的镜像层。

在此例中，Docker 会检查缓存中是否存在一个基于 alpine:latest 镜像且执行了 `RUN apk add --update nodejs nodejs-npm` 指令构建得到的镜像层。

如果找到该镜像层，Docker 会跳过这条指令，并链接到这个已经存在的镜像层，然后继续构建；如果无法找到符合要求的镜像层，则设置缓存无效并构建该镜像层。

**一旦没命中缓存后续再不缓存**

此处“设置缓存无效”作用于本次构建的后续部分。也就是说 Dockerfile 中接下来的指令将全部执行而不会再尝试查找构建缓存。

假设 Docker 已经在缓存中找到了该指令对应的镜像层（缓存命中），并且假设这个镜像层的 ID 是 AAA。

下一条指令会复制一些代码到镜像中（COPY . /src）。因为上一条指令命中了缓存，Docker 会继续查找是否有一个缓存的镜像层也是基于 AAA 层并执行了 COPY . /src 命令。

如果有，Docker 会链接到这个缓存的镜像层并继续执行后续指令；如果没有，则构建镜像层，并对后续的构建操作设置缓存无效。

假设 Docker 已经有一个对应该指令的缓存镜像层（缓存命中），并且假设这个镜像层的 ID 是 BBB。

那么 Docker 将继续执行 Dockerfile 中剩余的指令。

理解以下几点很重要。

首先，**一旦有指令在缓存中未命中（没有该指令对应的镜像层），则后续的整个构建过程将不再使用缓存**。

在编写 Dockerfile 时须特别注意这一点，**尽量将易于发生变化的指令置于 Dockerfile 文件的后方执行。**

这意味着缓存未命中的情况将直到构建的后期才会出现，从而构建过程能够尽量从缓存中获益。

通过对 `docker image build` 命令加入 --nocache=true 参数可以强制忽略对缓存的使用。

还有一点也很重要，那就是 COPY 和 ADD 指令会检查复制到镜像中的内容自上一次构建之后是否发生了变化。

例如，有可能 Dockerfile 中的 COPY . /src 指令没有发生变化，但是被复制的目录中的内容已经发生变化了。

为了应对这一问题，Docker 会计算每一个被复制文件的 Checksum 值，并与缓存镜像层中同一文件的 checksum 进行对比。如果不匹配，那么就认为缓存无效并构建新的镜像层。

### 合并镜像

合并镜像并非一个最佳实践，因为这种方式利弊参半。

总体来说，Docker 会遵循正常的方式构建镜像，但之后会增加一个额外的步骤，将所有的内容合并到一个镜像层中。

当镜像中层数太多时，合并是一个不错的优化方式。例如，当创建一个新的基础镜像，以便基于它来构建其他镜像的时候，这个基础镜像就最好被合并为一层。

缺点是，合并的镜像将无法共享镜像层。这会导致存储空间的低效利用，而且 push 和 pull 操作的镜像体积更大。

执行 `docker image build`命令时，可以通过增加 --squash 参数来创建一个合并的镜像。

![](/images/docker-image-merge.gif)



两个镜像的内容是完全一样的，区别在于是否进行了合并。在使用 `docker image push` 命令发送镜像到 Docker Hub 时，合并的镜像需要发送全部字节，而不合并的镜像只需要发送不同的镜像层即可。

### 使用 no-install-recommends

在构建 Linux 镜像时，若使用的是 APT 包管理器，则应该在执行 apt-get install 命令时增加 no-install-recommends 参数。

这能够确保 APT 仅安装核心依赖（Depends 中定义）包，而不是推荐和建议的包。这样能够显著减少不必要包的下载数量。

### 不要安装 MSI 包（Windows）

在构建 Windows 镜像时，尽量避免使用 MSI 包管理器。因其对空间的利用率不高，会大幅增加镜像的体积。

### Dockerfile简介

Dockerfile 由一行行命令语句组成，并支持以 # 开头的注释行。例如：

```dockerfile
# Test web-app to use with Pluralsight courses and Docker Deep Dive book
# Linux x64
FROM alpine

LABEL maintainer="nigelpoulton@hotmail.com"

# Install Node and NPM
RUN apk add --update nodejs nodejs-npm

# Copy app to /src
COPY . /src

WORKDIR /src

# Install dependencies
RUN  npm install

EXPOSE 8080

ENTRYPOINT ["node", "./app.js"]
```

使用 -t 参数为镜像打标签，使用 -f 参数指定 Dockerfile 的路径和名称，使用 -f 参数可以指定位于任意路径下的任意名称的 Dockerfile。

构建上下文是指应用文件存放的位置，可能是本地 Docker 主机上的一个目录或一个远程的 Git 库。



Dockerfile 中的 FROM 指令用于指定要构建的镜像的基础镜像。它通常是 Dockerfile 中的第一条指令。

Dockerfile 中的 RUN 指令用于在镜像中执行命令，这会创建新的镜像层。每个 RUN 指令创建一个新的镜像层。

Dockerfile 中的 COPY 指令用于将文件作为一个新的层添加到镜像中。通常使用 COPY 指令将应用代码赋值到镜像中。

Dockerfile 中的 EXPOSE 指令用于记录应用所使用的网络端口。

Dockerfile 中的 ENTRYPOINT 指令用于指定镜像以容器方式启动后默认运行的程序。

其他的 Dockerfile 指令还有 LABEL、ENV、ONBUILD、HEALTHCHECK、CMD 等。

{% endraw %}