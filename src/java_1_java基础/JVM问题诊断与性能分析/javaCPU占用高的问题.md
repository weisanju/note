# 背景

线上系统，Java进程 如果出现 CPU 高负载运行，一直降不下来，导致 无法响应其他任何请求，可以按以下流程排查





# 定位办法

## 采用top命令定位进程

```
top
```

默认CPU使用率排序，找出Java进程



## 使用top -Hp命令定位线程

**打印线程**

```
 top -Hp <pid>
```

**查看该Java进程内所有线程的资源占用情况**



**转换成TID**

```
printf “%x\n” 命令（tid指线程的id号）将以上10进制的线程号转换为16进制：
```



## 采用jstack命令导出线程快照

```java
jstack -l 29706 > ./jstack_result.txt 
```



## 根据线程号定位具体代码

```shell
jstack  3054|grep -A10 bef
```



## 使用命令直接查找

```shell
ps -mp 3054 -o THREAD,tid,time | sort -rn
```





# 日志文件内容分析

## 线程状态

### Deadlock

> 死锁线程，一般指多个线程调用间，进入相互资源占用，导致一直等待无法释放的情况。

**案例**

```
Found one Java-level deadlock:
=============================
"Thread-0":
  waiting to lock monitor 0x00000000266c1cd8 (object 0x0000000715b5f120, a java.lang.Class),
  which is held by "main"
"main":
  waiting to lock monitor 0x00000000266c0838 (object 0x0000000715b63ca0, a java.lang.Object),
  which is held by "Thread-0"

```



### Runnable

执行中

### Waiting on condition

> WAITING，等待资源

```
"main" #1 prio=5 os_prio=0 tid=0x0000000002ba3800 nid=0x2ad0 waiting on condition [0x0000000002a9f000]
   java.lang.Thread.State: WAITING (parking)
        at sun.misc.Unsafe.park(Native Method)
        at java.util.concurrent.locks.LockSupport.park(LockSupport.java:304)
        at TestMain.park(TestMain.java:70)
        at TestMain.main(TestMain.java:6)
```

**睡眠**

```
"main" #1 prio=5 os_prio=0 tid=0x0000000002db3800 nid=0x5068 waiting on condition [0x0000000002c3f000]
   java.lang.Thread.State: TIMED_WAITING (sleeping)
        at java.lang.Thread.sleep(Native Method)
        at TestMain.sleep(TestMain.java:10)
        at TestMain.main(TestMain.java:6)

```



### Waiting on monitor entry

> BLOCKED，等待获取监视器

```
"Thread-0" #14 prio=5 os_prio=0 tid=0x0000000029d6f800 nid=0x43a0 waiting for monitor entry [0x000000002a53f000]
   java.lang.Thread.State: BLOCKED (on object monitor)
        at TestMain.lambda$deadLock$0(TestMain.java:16)
        - waiting to lock <0x0000000715b5f120> (a java.lang.Class for TestMain)
        - locked <0x0000000715b63ca0> (a java.lang.Object)
        at TestMain$$Lambda$1/1879492184.run(Unknown Source)
        at java.lang.Thread.run(Thread.java:748)

```





### Object.wait() 或 TIMED_WAITING

> WAITING，条件等待，也可以认为是一种等待资源

```java
"main" #1 prio=5 os_prio=0 tid=0x00000000028a3800 nid=0x40d0 in Object.wait() [0x000000000272f000]
   java.lang.Thread.State: WAITING (on object monitor)
        at java.lang.Object.wait(Native Method)
        - waiting on <0x0000000715ad86d0> (a java.lang.Class for TestMain)
        at java.lang.Object.wait(Object.java:502)
        at TestMain.waitOnConditional(TestMain.java:42)
        - locked <0x0000000715ad86d0> (a java.lang.Class for TestMain)
        at TestMain.main(TestMain.java:6)

```



### Suspended

已过时，推荐使用 *LockSupport.park*







## waitSet,与 entrySet的区别

![](/images/jstack_waitting_status.png)





# 通过 Jvisualvm可视化

## 配置JvisualVM

```
[root@localhost management]# pwd
/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.282.b08-1.el7_9.x86_64/jre/lib/management
[root@localhost management]# ls -l
total 28
-rw-r--r--. 1 root root  3998 Jan 22 10:41 jmxremote.access
-r--r--r--. 1 root root  2856 Jan 22 10:41 jmxremote.password.template
-rw-r--r--. 1 root root 14630 Jan 22 10:41 management.properties
-r--r--r--. 1 root root  3376 Jan 22 10:41 snmp.acl.template
```

**首先利用jmxremote.password.template 文件创建jmxremote.password文件，并且设置相应的读写权限。**

```
//默认两个角色 monitorRole 密码为 QED，controlRole的密码是 R&D

monitorRole  QED

controlRole   R&D
```

接着可以运行本地的  *jvisualvm.exe* 运行

