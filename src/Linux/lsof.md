# 基本使用

`lsof -i:port`



# 扩展接口

-a 列出打开文件存在的进程

-c<进程名> 列出指定进程所打开的文件

-g 列出GID号进程详情

-d<文件号> 列出占用该文件号的进程

+d<目录> 列出目录下被打开的文件

+D<目录> 递归列出目录下被打开的文件

-n<目录> 列出使用NFS的文件

-i<条件> 列出符合条件的进程。（4、6、协议、:端口、 @ip ）

-p<进程号> 列出指定进程号所打开的文件

-u 列出UID号进程详情

-h 显示帮助信息

-v 显示版本信息



# 表头含义

COMMAND：进程的名称 PID：进程标识符

USER：进程所有者

FD：文件描述符，应用程序通过文件描述符识别该文件。如cwd、txt等 TYPE：文件类型，如DIR、REG等

DEVICE：指定磁盘的名称

SIZE：文件的大小

NODE：索引节点（文件在磁盘上的标识）

NAME：打开文件的确切名称





# 示例

```
#查看所有进程的文件打开数
lsof |wc -l
#查看整个系统目前使用的文件句柄数
cat /proc/sys/fs/file-nr
#查看某个进程的的文件句柄数
lsof -p pid|wc -l
查看某个目录，文件被什么进程占用
lsof path(file)
```

```shell
#!/bin/sh 
set -x 
echo "">total_handler 
psid=`ps -ef|grep $1|head -1|awk '{print $2}'` 
count=0 
while [ $count -lt 3000 ] 
do 
 lsof -p $psid|wc -l >> total_handler 
 sleep 10 
 count=`expr $count + 1` 
done
```

