# 介绍

top 命令可以动态地持续监听进程地运行状态，与此同时，该命令还提供了一个交互界面，用户可以根据需要，人性化地定制自己的输出，进而更清楚地了进程的运行状态。



# 格式

**基本格式**

```
top [选项]
```

**选项**

- -d 秒数：指定 top 命令每隔几秒更新。默认是 3 秒；
- -b：使用批处理模式输出。一般和"-n"选项合用，用于把 top 命令重定向到文件中；
- -n 次数：指定 top 命令执行的次数。
- -p 进程PID：仅查看指定 ID 的进程；
- -q 没有任何延迟的刷新
- -s：使 top 命令在安全模式中运行，避免在交互模式中出现错误；
- -u 用户名：只监听某个用户的进程；
- -i 不显示任何闲置进程或者僵尸进程
- -c 显示整个命令行

**交互式命令选项**

常用交互操作

- 基础操作

    - 1：显示CPU详细信息，每核显示一行
    - d / s ：修改刷新频率，单位为秒
    - h：可显示帮助界面
    - n：指定进程列表显示行数，默认为满屏行数
    - q：退出top

- 面板隐藏显示

    - l：隐藏/显示第1行负载信息；
    - t：隐藏/显示第2~3行CPU信息；
    - m：隐藏/显示第4~5行内存信息；

- 进程列表排序

    - M：根据驻留内存大小进行排序；
    - P：根据CPU使用百分比大小进行排序；
    - T：根据时间/累计时间进行排序；

    

#  top 命令的执行结果

```
top - 12:26:46 up 1 day, 13:32, 2 users, load average: 0.00, 0.00, 0.00
Tasks: 95 total, 1 running, 94 sleeping, 0 stopped, 0 zombie
Cpu(s): 0.1%us, 0.1%sy, 0.0%ni, 99.7%id, 0.1%wa, 0.0%hi, 0.1%si, 0.0%st
Mem: 625344k total, 571504k used, 53840k free, 65800k buffers
Swap: 524280k total, 0k used, 524280k free, 409280k cached
PID   USER PR NI VIRT  RES  SHR S %CPU %MEM   TIME+ COMMAND
19002 root 20  0 2656 1068  856 R  0.3  0.2 0:01.87 top
1     root 20  0 2872 1416 1200 S  0.0  0.2 0:02.55 init
2     root 20  0    0    0    0 S  0.0  0.0 0:00.03 kthreadd
3     root RT  0    0    0    0 S  0.0  0.0 0:00.00 migration/0
4     root 20  0    0    0    0 S  0.0  0.0 0:00.15 ksoftirqd/0
5     root RT  0    0    0    0 S  0.0  0.0 0:00.00 migration/0
6     root RT  0    0    0    0 S  0.0  0.0 0:10.01 watchdog/0
7     root 20  0    0    0    0 S  0.0  0.0 0:05.01 events/0
8     root 20  0    0    0    0 S  0.0  0.0 0:00.00 cgroup
9     root 20  0    0    0    0 S  0.0  0.0 0:00.00 khelper
10    root 20  0    0    0    0 S  0.0  0.0 0:00.00 netns
11    root 20  0    0    0    0 S  0.0  0.0 0:00.00 async/mgr
12    root 20  0    0    0    0 S  0.0  0.0 0:00.00 pm
13    root 20  0    0    0    0 S  0.0  0.0 0:01.70 sync_supers
14    root 20  0    0    0    0 S  0.0  0.0 0:00.63 bdi-default
15    root 20  0    0    0    0 S  0.0  0.0 0:00.00 kintegrityd/0
16    root 20  0    0    0    0 S  0.0  0.0 0:02.52 kblockd/0
17    root 20  0    0    0    0 S  0.0  0.0 0:00.00 kacpid
18    root 20  0    0    0    0 S  0.0  0.0 0:00.00 kacpi_notify
```

1. 第一部分是前五行，显示的是整个系统的资源使用状况，我们就是通过这些输出来判断服务器的资源使用状态的；

**第一行**

| 内 容                         | 说 明                                                        |
| ----------------------------- | ------------------------------------------------------------ |
| 12:26:46                      | 系统当前时间                                                 |
| up 1 day, 13:32               | 系统的运行时间.本机己经运行 1 天 13 小时 32 分钟             |
| 2 users                       | 当前登录了两个用户                                           |
| load average: 0.00,0.00，0.00 | 系统在之前 1 分钟、5 分钟、15 分钟的平均负载。如果 CPU 是单核的，则这个数值超过 1 就是高负载：如果 CPU 是四核的，则这个数值超过 4 就是高负载 （这个平均负载完全是依据个人经验来进行判断的，一般认为不应该超过服务器 CPU 的核数） |

**第二行**

> 系统进程统计信息

| 内 容           | 说 明                                          |
| --------------- | ---------------------------------------------- |
| Tasks: 95 total | 系统中的进程总数                               |
| 1 running       | 正在运行的进程数                               |
| 94 sleeping     | 睡眠的进程数                                   |
| 0 stopped       | 正在停止的进程数                               |
| 0 zombie        | 僵尸进程数。如果不是 0，则需要手工检查僵尸进程 |

**第三行**

> CPU 信息

| 内 容           | 说 明                                                        |
| --------------- | ------------------------------------------------------------ |
| Cpu(s): 0.1 %us | 用户模式占用的 CPU 百分比                                    |
| 0.1%sy          | 系统模式占用的 CPU 百分比                                    |
| 0.0%ni          | 改变过优先级的用户进程占用的 CPU 百分比                      |
| 99.7%id         | 空闲 CPU 占用的 CPU 百分比                                   |
| 0.1%wa          | 等待输入/输出的进程占用的 CPU 百分比                         |
| 0.0%hi          | 硬中断请求服务占用的 CPU 百分比                              |
| 0.1%si          | 软中断请求服务占用的 CPU 百分比                              |
| 0.0%st          | st（steal time）意为虚拟时间百分比，当有虚拟机时，虚拟机被hypervisor偷去的CPU时间（如果当前处于一个hypervisor下的vm，实际上hypervisor也是要消耗一部分CPU处理时间的） |

第四行

> 物理内存信息

| 内 容              | 说 明                                                        |
| ------------------ | ------------------------------------------------------------ |
| Mem: 625344k total | 物理内存的总量，单位为KB                                     |
| 571504k used       | 己经使用的物理内存数量                                       |
| 53840k free        | 空闲的物理内存数量。我们使用的是虚拟机，共分配了 628MB内存，所以只有53MB的空闲内存 |
| 65800k buffers     | 用作内核缓冲的内存量                                         |

第五行

> 交换分区（swap）信息

| 内 容               | 说 明                        |
| ------------------- | ---------------------------- |
| Swap: 524280k total | 交换分区（虚拟内存）的总大小 |
| O k used            | 已经使用的交换分区的大小     |
| 524280 k free       | 空闲交换分区的大小           |
| 409280k cached      | 作为缓存的交换分区的大小     |



如果 1 分钟、5 分钟、15 分钟的平均负载高于 1，则证明系统压力较大。如果 CPU 的使用率过高或空闲率过低，则证明系统压力较大。如果物理内存的空闲内存过小，则也证明系统压力较大。



第二部分从第六行开始，显示的是系统中**进程的信息**；

**显示字段管理面板**

在top命令中按f按可以查看显示的列信息，按对应字母来开启/关闭列，大写字母表示开启，小写字母表示关闭。带*号的是默认列。

| 英文名  | 解释                                                         |
| ------- | ------------------------------------------------------------ |
| PID     | Process Id                                                   |
| USER    | Effective User Name                                          |
| PR      | Priority（进程优先级，不可更改）                             |
| NI      | Nice Value（进程优先级，可更改，负值表示高优先级，正值表示低优先级） |
| VIRT    | Virtual Image (KiB) 。进程使用的虚拟内存总量，单位kb。VIRT=SWAP+RES |
| RES     | Resident Size (KiB)，进程占用的物理内存，单位kb。RES=CODE+DATA |
| SHR     | Shared Memory (KiB)，共享内存大小                            |
| S       | Process Status，进程状态。D=不可中断的睡眠状态,R=运行,S=睡眠,T=跟踪/停止,Z=僵尸进程 |
| %CPU    | 上次更新到现在的CPU时间占用百分比                            |
| %MEM    | 进程使用的物理内存百分比                                     |
| TIME+   | (CPU Time, hundredths) 进程使用的CPU时间总计，单位1/100秒    |
| COMMAND | (Command name/line) 命令名/命令行                            |
| PPID    | (Parent Process Pid)                                         |
| UID     | Effective User Id                                            |
| RUID    | Real User Id                                                 |
| RUSER   | Real User Name                                               |
| SUID    | Saved User Id                                                |
| SUSER   | Saved User Name                                              |
| GID     | Group Id                                                     |
| GROUP   | Group Name                                                   |
| PGRP    | Process Group Id                                             |
| TTY     | Controlling Tty，启动进程的终端名。不是从终端启动的进程则显示为 ? |
| TPGID   | Tty Process Grp Id                                           |
| SID     | Session Id                                                   |
| nTH     | Number of Threads                                            |
| P       | (Last used cpu (SMP)) 最后使用的CPU，仅在多CPU环境下有意义   |
| TIME    | 进程使用的CPU时间总计，单位秒                                |
| SWAP    | (Swapped size (kb)) 进程使用的虚拟内存中                     |
| CODE    | (Code size (kb)) 可执行代码占用的物理内存大小，单位kb        |
| DATA    | 可执行代码以外的部分(数据段+栈)占用的物理内存大小，单位kb    |
| WCHAN   | (Sleeping in Function) 若该进程在睡眠，则显示睡眠中的系统函数名 |





# 具体交互性命令

```
 Z,B       Global: 'Z' change color mappings; 'B' disable/enable bold
            Z：修改颜色配置；B：关闭/开启粗体
  l,t,m     Toggle Summaries: 'l' load avg; 't' task/cpu stats; 'm' mem info
            l：隐藏/显示第1行负载信息；t：隐藏/显示第2~3行CPU信息；m：隐藏/显示第4~5行内存信息；
  1,I       Toggle SMP view: '1' single/separate states; 'I' Irix/Solaris mode
            1：单行/多行显示CPU信息；I：Irix/Solaris模式切换
  f,o     . Fields/Columns: 'f' add or remove; 'o' change display order
            f：列显示控制；o：列排序控制，按字母进行调整
  F or O  . Select sort field  选择排序列
  <,>     . Move sort field: '<' next col left; '>' next col right 上下移动内容
  R,H     . Toggle: 'R' normal/reverse sort; 'H' show threads
            R：内容排序；H：显示线程
  c,i,S   . Toggle: 'c' cmd name/line; 'i' idle tasks; 'S' cumulative time
            c：COMMAND列命令名称与完整命令行路径切换；i：忽略闲置和僵死进程开关；S：累计模式切换
  x,y     . Toggle highlights: 'x' sort field; 'y' running tasks
            x：列排序；y：运行任务
  z,b     . Toggle: 'z' color/mono; 'b' bold/reverse (only if 'x' or 'y')
            z：颜色模式；b：粗体开关 仅适用于x，y模式中
  u       . Show specific user only 按用户进行过滤，当输入错误可按Ctrl + Backspace进行删除
  n or #  . Set maximum tasks displayed 设置进程最大显示条数

  k,r       Manipulate tasks: 'k' kill; 'r' renice
            k：终止一个进程；r：重新设置一个进程的优先级别
  d or s    Set update interval  改变两次刷新之间的延迟时间（单位为s），如果有小数，就换算成ms。输入0值则系统将不断刷新，默认值是5s；
  W         Write configuration file 将当前设置写入~/.toprc文件中
  q         Quit       退出
          ( commands shown with '.' require a visible task display window )
            注意：带.的命令需要一个可见的任务显示窗口
```



# 批处理 获取进程信息

```
 top -b -n 10 > /root/top.log
```

**会产生 10次 top调用并输出**





