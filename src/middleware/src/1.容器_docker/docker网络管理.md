# 前言

Docker 存在 4种网络工作方式，和一些自定义网络模式

安装Docker时，它会自动创建三个网络，bridge（创建容器默认连接到此网络）、 none 、host

**host**

容器将不会虚拟出自己的网卡，配置自己的IP等，而是使用宿主机的IP和端口。

**Container**

创建的容器不会创建自己的网卡，配置自己的IP，而是和一个指定的容器共享IP、端口范围。

**None**

该模式关闭了容器的网络功能。

**Bridge**

此模式会为每一个容器分配、设置IP等，并将容器连接到一个docker0虚拟网桥，通过docker0网桥以及Iptables nat表配置与宿主机通信。



Docker内置这三个网络，运行容器时，你可以使用该--network标志来指定容器应连接到哪些网络。



**指定网络**

```sh
host模式：使用 --net=host 指定。

none模式：使用 --net=none 指定。

bridge模式：使用 --net=bridge 指定，默认设置。

container模式：使用 --net=container:NAME_or_ID 指定。
```







# 网络模式详解

## Host

**与宿主机在同一个网络中，但没有独立IP地址**

**命名空间隔离资源**

众所周知，Docker使用了Linux的Namespaces技术来进行资源隔离，

如PID Namespace隔离进程，Mount Namespace隔离文件系统，Network Namespace隔离网络等。

**network 隔离网络**

一个Network Namespace提供了一份独立的网络环境，包括网卡、路由、Iptable规则等都与其他的Network Namespace隔离。

一个Docker容器一般会分配一个独立的Network Namespace。

但如果启动容器的时候使用host模式，那么这个容器将不会获得一个独立的Network Namespace，而是和宿主机共用一个Network Namespace。容器将不会虚拟出自己的网卡，配置自己的IP等，而是使用宿主机的IP和端口。

**例如**

```sh
docker run --name nginx1 -p80:80 --net=host -d nginx
```

## Container

这个模式指定新创建的容器和已经存在的一个容器共享一个Network Namespace

而不是和宿主机共享。新创建的容器不会创建自己的网卡，配置自己的IP，而是和一个指定的容器共享IP、端口范围等

同样，两个容器除了网络方面，其他的如文件系统、进程列表等还是隔离的。两个容器的进程可以通过lo网卡设备通信。

## None

该模式关闭了容器的网络功能

## Bridge

容器使用独立network Namespace，并连接到docker0虚拟网卡

通过docker0网桥以及Iptables nat表配置与宿主机通信；bridge模式是Docker默认的网络设置，此模式会为每一个容器分配Network Namespace、设置IP等，并将一个主机上的Docker容器连接到一个虚拟网桥上。下面着重介绍一下此模式。

### Bridge模式的拓扑结构

**创建虚拟网桥**

当Docker server启动时，会在主机上创建一个名为docker0的虚拟网桥，此主机上启动的Docker容器会连接到这个虚拟网桥上

**分配容器IP**

虚拟网桥的工作方式和物理交换机类似，这样主机上的所有容器就通过交换机连在了一个二层网络中

接下来就要为容器分配IP了，Docker会从RFC1918所定义的私有IP网段中，选择一个和宿主机不同的IP地址和子网分配给docker0，连接到docker0的容器就从这个子网中选择一个未占用的IP使用，如一般Docker会使用172.17.0.0/16这个网段，并将172.17.0.1/16分配给docker0网桥

![](/images/docker-network-bridge-example1.jpg)







### 网络配置过程

*  在主机上创建一对虚拟网卡veth pair设备

  veth设备总是成对出现的，它们组成了一个数据的通道，数据从一个设备进入，就会从另一个设备出来。因此，veth设备常用来连接两个网络设备

* Docker将veth pair设备的一端放在新创建的容器中，并命名为eth0。另一端放在主机中，以veth65f9这样类似的名字命名，并将这个网络设备加入到docker0网桥中

* 从docker0子网中分配一个IP给容器使用，并设置docker0的IP地址为容器的默认网关。



### bridge模式下容器的通信

**容器间可以相互通信**

在bridge模式下，连在同一网桥上的容器可以相互通信

（若出于安全考虑，也可以禁止它们之间通信，方法是在DOCKER_OPTS变量中设置–icc=false，这样只有使用–link才能使两个容器通信）

**限制容器间的通信**

Docker可以开启容器间通信（意味着默认配置--icc=true），也就是说，宿主机上的所有容器可以不受任何限制地相互通信，这可能导致拒绝服务攻击。



Docker可以通过--ip_forward和--iptables两个选项控制容器间、容器和外部世界的通信。

```
-A POSTROUTING -s 172.17.0.0/16 ! -o docker0 -j MASQUERADE
```



```
这条规则会将源地址为172.17.0.0/16的包（也就是从Docker容器产生的包），并且不是从docker0网卡发出的，进行源地址转换，转换成主机网卡的地址。

这么说可能不太好理解，举一个例子说明一下。假设主机有一块网卡为eth0，IP地址为10.10.101.105/24，网关为10.10.101.254。从主机上一个IP为172.17.0.1/16的容器中ping百度（180.76.3.151）。IP包首先从容器发往自己的默认网关docker0，包到达docker0后，也就到达了主机上。然后会查询主机的路由表，发现包应该从主机的eth0发往主机的网关10.10.105.254/24。接着包会转发给eth0，并从eth0发出去（主机的ip_forward转发应该已经打开）。这时候，上面的Iptable规则就会起作用，对包做SNAT转换，将源地址换为eth0的地址。这样，在外界看来，这个包就是从10.10.101.105上发出来的，Docker容器对外是不可见的。
```



### 外面的机器是如何访问Docker容器的服务呢？

```sh
 docker run --name=nginx_bridge --net=bridge -p 80:80 -d nginx
```

```sh
iptables -L
-A DOCKER ! -i docker0 -p tcp -m tcp --dport 80 -j DNAT --to-destination 172.17.0.2:80
```



```
此条规则就是对主机eth0收到的目的端口为80的tcp流量进行DNAT转换，将流量发往172.17.0.2:80，也就是我们上面创建的Docker容器。所以，外界只需访问10.10.101.105:80就可以访问到容器中的服务。

除此之外，我们还可以自定义Docker使用的IP地址、DNS等信息，甚至使用自己定义的网桥，但是其工作方式还是一样的。
```



## 自定义网络

建议使用自定义的网桥来控制哪些容器可以相互通信，还可以自动DNS解析容器名称到IP地址。Docker提供了创建这些网络的默认网络驱动程序，你可以创建一个新的Bridge网络，Overlay或Macvlan网络。你还可以创建一个网络插件或远程网络进行完整的自定义和控制。



你可以根据需要创建任意数量的网络，并且可以在任何给定时间将容器连接到这些网络中的零个或多个网络。此外，您可以连接并断开网络中的运行容器，而无需重新启动容器。当容器连接到多个网络时，其外部连接通过第一个非内部网络以词法顺序提供。



### 自定义桥接网络

```sh
docker network create --driver bridge new_bridge

创建网络后，可以看到新增加了一个网桥（172.18.0.1）。

72: br-2edfc1326986: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN group default 
    link/ether 02:42:07:cc:f8:33 brd ff:ff:ff:ff:ff:ff
    inet 172.18.0.1/16 scope global br-2edfc1326986
       valid_lft forever preferred_lft forever
```

### Macvlan

**简介**

Macvlan是一个新的尝试，是真正的网络虚拟化技术的转折点。Linux实现非常轻量级，因为与传统的Linux Bridge隔离相比，它们只是简单地与一个Linux以太网接口或子接口相关联，以实现网络之间的分离和与物理网络的连接。

Macvlan提供了许多独特的功能，并有充足的空间进一步创新与各种模式。这些方法的两个高级优点是绕过Linux网桥的正面性能以及移动部件少的简单性。删除传统上驻留在Docker主机NIC和容器接口之间的网桥留下了一个非常简单的设置，包括容器接口，直接连接到Docker主机接口。由于在这些情况下没有端口映射，因此可以轻松访问外部服务。

**Macvlan Bridge模式示例用法**

略

[详见](https://www.cnblogs.com/zuxing/articles/8780661.html)





# docker网络管理命令

```sh
#创建自定义网络
docker network create
# 上线一个网络
docker network connect
# 查看网络
docker network ls
# 删除网络
docker network rm
# 下线网络
docker network disconnect
# 查看网络明细
docker network inspect
```



