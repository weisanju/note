# linux cgroups 简介

cgroups(Control Groups) 是 linux 内核提供的一种机制

**这种机制可以根据需求把一系列系统任务及其子任务整合(或分隔)到按资源划分等级的不同组内，从而为系统资源管理提供一个统一的框架**

简单说，cgroups 可以限制、记录任务组所使用的物理资源。

本质上来说，cgroups 是内核附加在程序上的一系列钩子(hook)

通过程序运行时对资源的调度触发相应的钩子以达到资源追踪和限制的目的。





# 为什么要了解 cgroups

在以容器技术为代表的虚拟化技术大行其道的时代了解 cgroups 技术是非常必要的！

**比如我们可以很方便的限制某个容器可以使用的 CPU、内存等资源**，这究竟是如何实现的呢？

通过了解 cgroups 技术，我们可以窥探到 linux 系统中整个资源限制系统的脉络。从而帮助我们更好的理解和使用 linux 系统。







# cgroups 的四大功能

实现 cgroups 的主要目的是为不同用户层面的资源管理提供一个统一化的接口。从单个任务的资源控制到操作系统层面的虚拟化，cgroups 提供了四大功能：

* 资源限制

cgroups 可以对任务是要的资源总额进行限制。比如设定任务运行时使用的内存上限，一旦超出就发 OOM。

* 进程调度优先级

通过分配的 CPU 时间片数量和磁盘 IO 带宽，实际上就等同于控制了任务运行的优先级。

* 资源统计

cgoups 可以统计系统的资源使用量，比如 CPU 使用时长、内存用量等。这个功能非常适合当前云端产品按使用量计费的方式。

* 任务控制

cgroups 可以对任务执行挂起、恢复等操作。









# 相关概念

## **Task(任务)** 

在 linux 系统中，内核本身的调度和管理并不对进程和线程进行区分，只是根据 clone 时传入的参数的不同来从概念上区分进程和线程。这里使用 task 来表示系统的一个进程或线程。

## **Cgroup(控制组)**

> 在这个控制组里 定义了多种 资源控制

cgroups 中的资源控制以 cgroup 为单位实现。Cgroup 表示按某种资源控制标准划分而成的任务组，包含一个或多个子系统。一个任务可以加入某个 cgroup，也可以从某个 cgroup 迁移到另一个 cgroup。

## **Subsystem(子系统)**

cgroups 中的子系统就是一个**资源调度控制器**(又叫 controllers)。

比如 CPU 子系统可以控制 CPU 的时间分配，内存子系统可以限制内存的使用量。

**cat /proc/cgroups**

| subsys_name | description                                                  | hierarchy | num_cgroups | enabled |
| ----------- | ------------------------------------------------------------ | --------- | ----------- | ------- |
| cpuset      | 给 cgroup 中的任务分配独立的 CPU(多处理器系统) 和内存节点    | 1         | 1           | 1       |
| cpu         | 限制 CPU 时间片的分配，与 cpuacct 挂载在同一目录             | 2         | 1           | 1       |
| cpuacct     | 生成 cgroup 中的任务占用 CPU 资源的报告，与 cpu 挂载在同一目录 | 3         | 1           | 1       |
| blkio       | 对块设备的 IO 进行限制                                       | 4         | 1           | 1       |
| memory      | 对 cgroup 中的任务的可用内存进行限制，并自动生成资源占用报告 | 5         | 1           | 1       |
| devices     | 允许或禁止 cgroup 中的任务访问设备                           | 6         | 1           | 1       |
| freezer     | 暂停/恢复 cgroup 中的任务                                    | 7         | 1           | 1       |
| net_cls     | 使用等级识别符（classid）标记网络数据包，这让 Linux 流量控制器（tc 指令）可以识别来自特定 cgroup 任务的数据包，并进行网络限制。 | 8         | 1           | 1       |
| perf_event  | 允许使用 perf 工具来监控 cgroup。                            | 9         | 1           | 1       |
| net_prio    | 允许基于 cgroup 设置网络流量(netowork traffic)的优先级。     | 10        | 1           | 1       |
| hugetlb     | 限制使用的内存页数量。                                       | 11        | 1           | 1       |
| pids        | 限制任务的数量。                                             | 12        | 1           | 1       |
| rdma        |                                                              | 13        | 1           | 1       |

层级有一系列 cgroup 以一个树状结构排列而成，

每个层级通过绑定对应的子系统进行资源控制。

层级中的 cgroup 节点可以包含零个或多个子节点，子节点继承父节点挂载的子系统。

一个操作系统中可以有多个层级

# cgroups 的文件系统接口

cgroups 以文件的方式提供应用接口，我们可以通过 mount 命令来查看 cgroups 默认的挂载点：

`cat /sys/fs/cgroup`

```
tmpfs on /sys/fs/cgroup type tmpfs (rw,nosuid,nodev,noexec,relatime,mode=755)
cgroup2 on /sys/fs/cgroup/unified type cgroup2 (rw,nosuid,nodev,noexec,relatime,nsdelegate)
cgroup on /sys/fs/cgroup/cpuset type cgroup (rw,nosuid,nodev,noexec,relatime,cpuset)
cgroup on /sys/fs/cgroup/cpu type cgroup (rw,nosuid,nodev,noexec,relatime,cpu)
cgroup on /sys/fs/cgroup/cpuacct type cgroup (rw,nosuid,nodev,noexec,relatime,cpuacct)
cgroup on /sys/fs/cgroup/blkio type cgroup (rw,nosuid,nodev,noexec,relatime,blkio)
cgroup on /sys/fs/cgroup/memory type cgroup (rw,nosuid,nodev,noexec,relatime,memory)
cgroup on /sys/fs/cgroup/devices type cgroup (rw,nosuid,nodev,noexec,relatime,devices)
cgroup on /sys/fs/cgroup/freezer type cgroup (rw,nosuid,nodev,noexec,relatime,freezer)
cgroup on /sys/fs/cgroup/net_cls type cgroup (rw,nosuid,nodev,noexec,relatime,net_cls)
cgroup on /sys/fs/cgroup/perf_event type cgroup (rw,nosuid,nodev,noexec,relatime,perf_event)
cgroup on /sys/fs/cgroup/net_prio type cgroup (rw,nosuid,nodev,noexec,relatime,net_prio)
cgroup on /sys/fs/cgroup/hugetlb type cgroup (rw,nosuid,nodev,noexec,relatime,hugetlb)
cgroup on /sys/fs/cgroup/pids type cgroup (rw,nosuid,nodev,noexec,relatime,pids)
cgroup on /sys/fs/cgroup/rdma type cgroup (rw,nosuid,nodev,noexec,relatime,rdma)
```

**第一行说明**

* **/sys/fs/cgroup** 目录下的文件 都是存在于内存中的临时文件。

* 在使用 systemd 系统的操作系统中，/sys/fs/cgroup 目录都是由 systemd 在系统启动的过程中挂载的，并且挂载为只读的类型



**以 memory为例**

```sh
ls  /sys/fs/cgroup/memory
```

```
cgroup.clone_children       memory.kmem.max_usage_in_bytes      memory.memsw.failcnt             memory.stat
cgroup.event_control        memory.kmem.tcp.failcnt             memory.memsw.limit_in_bytes      memory.swappiness
cgroup.procs                memory.kmem.tcp.limit_in_bytes      memory.memsw.max_usage_in_bytes  memory.usage_in_bytes
cgroup.sane_behavior        memory.kmem.tcp.max_usage_in_bytes  memory.memsw.usage_in_bytes      memory.use_hierarchy
memory.failcnt              memory.kmem.tcp.usage_in_bytes      memory.move_charge_at_immigrate  notify_on_release
memory.force_empty          memory.kmem.usage_in_bytes          memory.oom_control               release_agent
memory.kmem.failcnt         memory.limit_in_bytes               memory.pressure_level            tasks
memory.kmem.limit_in_bytes  memory.max_usage_in_bytes           memory.soft_limit_in_bytes
```

这些文件就是 cgroups 的 memory 子系统中的根级设置。

比如 memory.limit_in_bytes 中的数字用来限制进程的最大可用内存，

memory.swappiness 中保存着使用 swap 的权重等等。

# 查看进程所属的 cgroups

```sh
可以通过 /proc/[pid]/cgroup 来查看指定进程属于哪些 cgroup：
```

**以docker为例**

```sh
cat /proc/$(cat /var/run/docker.pid)/cgroup
```

```
13:rdma:/
12:pids:/
11:hugetlb:/
10:net_prio:/
9:perf_event:/
8:net_cls:/
7:freezer:/
6:devices:/
5:memory:/
4:blkio:/
3:cpuacct:/
2:cpu:/
1:cpuset:/
0::/
```

每一行包含用冒号隔开的**三列**，他们的含义分别是：

- cgroup 树的 ID， 和 /proc/cgroups 文件中的 ID 一一对应。
- 和 cgroup 树绑定的所有 subsystem，多个 subsystem 之间用逗号隔开
- 进程在 cgroup 树中的路径，即进程所属的 cgroup，这个路径是相对于挂载点的相对路径

既然 cgroups 是以这些文件作为 API 的，那么我就可以通过创建或者是修改这些文件的内容来应用 cgroups。具体该怎么做呢？比如我们怎么才能限制某个进程可以使用的资源呢？

# cgroups 工具

```sh
sudo apt install cgroup-tools
```

# demo：限制进程可用的 CPU

```sh
$ cd  /sys/fs/cgroup/cpu
$ sudo mkdir nick_cpu
```

此操作会自动 在 `/sys/fs/cgroup/cpu/nick_cpu/` 新建以下文件

```
cgroup.clone_children  cpu.cfs_period_us  cpu.rt_period_us   cpu.shares  notify_on_release
cgroup.procs           cpu.cfs_quota_us   cpu.rt_runtime_us  cpu.stat    tasks
```

**限制只能使用 1/10的CPU**

```
$ sudo su
$ echo 100000 > nick_cpu/cpu.cfs_period_us
$ echo 10000 > nick_cpu/cpu.cfs_quota_us
```

**CPU密集程序**

```sh
void main()
{
    unsigned int i, end;

    end = 1024 * 1024 * 1024;
    for(i = 0; i < end; )
    {
        i ++;
    }
}
```

**编译**

```
$ gcc cputime.c -o cputime
$ sudo su
$ time ./cputime
$ time cgexec -g cpu:nick_cpu ./cputime
```

time 命令可以为我们报告程序执行消耗的时间，

其中的 real 就是我们真实感受到的时间。

使用 cgexec 能够把我们添加的 cgroup 配置 nick_cpu 应用到运行 cputime 程序的进程上。 

```sh
$ time ./cputime
real    0m0.479s
user    0m0.478s
sys     0m0.000s
$time cgexec -g cpu:nick_cpu ./cputime

real    0m4.788s
user    0m0.483s
sys     0m0.000s
```

**上图显示：实际执行要 0.4s，资源限制之后 要4.7s**

当修改成

```sh
#周期100ms,占用额度100ms
$ echo 100000 > nick_cpu/cpu.cfs_period_us
$ echo 100000 > nick_cpu/cpu.cfs_quota_us
```

```
$time cgexec -g cpu:nick_cpu ./cputime

real    0m0.472s
user    0m0.470s
sys     0m0.000s
```

# demo：限制进程可用的内存

```

cd /sys/fs/cgroup/memory 
sudo mkdir nick_memory
#下面的设置把进程的可用内存限制在最大 300M，并且不使用 swap：
# 物理内存 + SWAP <= 300 MB；1024*1024*300 = 314572800s
echo 314572800 > nick_memory/memory.limit_in_bytes
echo 0 > nick_memory/memory.swappiness

```

然后创建一个不断分配内存的程序，它分五次分配内存，每次申请 100M：

```c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define CHUNK_SIZE 1024 * 1024 * 100

void main()
{
    char *p;
    int i;

    for(i = 0; i < 5; i ++)
    {
        p = malloc(sizeof(char) * CHUNK_SIZE);
        if(p == NULL)
        {
            printf("fail to malloc!");
            return ;
        }
        // memset() 函数用来将指定内存的前 n 个字节设置为特定的值
        memset(p, 0, CHUNK_SIZE);
        printf("malloc memory %d MB\n", (i + 1) * 100);
    }
}
```



```
gcc nick_memory.c  -o nick_mem
```

```
$./nick_mem
malloc memory 100 MB
malloc memory 200 MB
malloc memory 300 MB
malloc memory 400 MB
malloc memory 500 MB
```

```
 cgexec -g memory:nick_memory ./nick_mem
 malloc memory 100 MB
malloc memory 200 MB
Killed
```

由于内存不足且禁止使用 swap，所以被限制资源的进程在申请内存时被强制杀死了。



下面再使用 stress 程序测试一个类似的场景(通过 stress 程序申请 500M 的内存)：

```
sudo cgexec -g memory:nick_memory stress --vm 1 --vm-bytes 500000000 --vm-keep --verbose
```

stress 程序能够提供比较详细的信息，进程被杀掉的方式是收到了 SIGKILL(signal 9) 信号。

实际应用中往往要同时限制多种的资源，比如既限制 CPU 资源又限制内存资源。使用 cgexec 实现这样的用例其实很简单，直接指定多个 -g 选项就可以了：

```
$ cgexec -g cpu:nick_cpu -g memory:nick_memory ./cpumem
```

