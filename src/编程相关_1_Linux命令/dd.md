# 简介

dd，是 device driver 的缩写，它可以称得上是“Linux 世界中的搬运工”，它用来读取设备、文件中的内容，并原封不动地复制到指定位置。





# 语法

**dd [OPERAND]...**

**dd OPTION**

copy一个文件 并 根据 操作 转换 与格式化



| OPERAND      | 说明                                     | 英文                                                         |
| ------------ | ---------------------------------------- | ------------------------------------------------------------ |
| bs=BYTES     | 一次读取和写入的 字节数                  | read and write up to BYTES bytes at a time                   |
| cbs=BYTES    | 一次转换的字节个数                       | convert BYTES bytes at a time                                |
| conv=CONVS   | 转换标志                                 | convert the file as per the comma separated symbol list      |
| count=N      | 仅复制 N 个输入块                        | copy only N input blocks                                     |
| ibs=BYTES    | 一次最多读取 BYTES 个字节（默认值：512） | read up to BYTES bytes at a time (default: 512)              |
| if=FILE      | 从文件读数据                             | read from FILE instead of stdin                              |
| iflag=FLAGS  | 输入标志，按照逗号分隔的符号列表读取     | read as per the comma separated symbol list                  |
| obs=BYTES    | 一次最多写入 BYTES 个字节（默认值：512） | write BYTES bytes at a time (default: 512)                   |
| of=FILE      | 写入文件                                 | write to FILE instead of stdout                              |
| oflag=FLAGS  | 输出标志                                 | write as per the comma separated symbol list                 |
| seek=N       | 在输出开始时跳过 N 个 obs 大小的块       | skip N obs-sized blocks at start of output                   |
| skip=N       | 在输入开始时跳过 N 个 ibs 大小的块       | skip N ibs-sized blocks at start of input                    |
| status=LEVEL | 要打印到 stderr 的信息级别；             | 'none' suppresses everything but error messages,<br /> 'noxfer' suppresses the final transfer statistics,<br />'progress' shows periodic transfer statistics |



 

# 单位

N 和 BYTES 后面可以跟以下乘法后缀：

```
c =1, w =2, b =512, kB =1000, K =1024, MB =1000*1000, M =1024*1024, xM =M
GB =1000*1000*1000, G =1024*1024*1024, and so on for T, P, E, Z, Y.
```

# 转换标志

```
ascii     from EBCDIC to ASCII
ebcdic    from ASCII to EBCDIC
ibm       from ASCII to alternate EBCDIC
block     pad newline-terminated records with spaces to cbs-size
unblock   replace trailing spaces in cbs-size records with newline
lcase     change upper case to lower case
ucase     change lower case to upper case
sparse    try to seek rather than write the output for NUL input blocks
swab      swap every pair of input bytes
sync      pad every input block with NULs to ibs-size; when used
            with block or unblock, pad with spaces rather than NULs

excl      fail if the output file already exists
nocreat   do not create the output file
notrunc   do not truncate the output file
noerror   continue after read errors
fdatasync  physically write output file data before finishing
fsync     likewise, but also write metadata


```

# 输入输出标志

```
append    append mode (makes sense only for output; conv=notrunc suggested)
direct    use direct I/O for data
directory  fail unless a directory
dsync     use synchronized I/O for data
sync      likewise, but also for metadata
fullblock  accumulate full blocks of input (iflag only)
nonblock  use non-blocking I/O
noatime   do not update access time
nocache   discard cached data
noctty    do not assign controlling terminal from file
nofollow  do not follow symlinks
count_bytes  treat 'count=N' as a byte count (iflag only)
skip_bytes  treat 'skip=N' as a byte count (iflag only)
seek_bytes  treat 'seek=N' as a byte count (oflag only)


```



# 发送 USR信号

Sending a USR1 signal to a running 'dd' process makes it

print I/O statistics to standard error and then resume copying.

```
$ dd if=/dev/zero of=/dev/null& pid=$!
$ kill -USR1 $pid; sleep 1; kill $pid
```

  

```
18335302+0 records in
18335302+0 records out
9387674624 bytes (9.4 GB) copied, 34.6279 seconds, 271 MB/s
```





# 使用案例

## 备份磁盘并恢复

```sh
# 这个命令将 sda 盘备份到指定文件 /root/sda.img 中去，其中用到了如下两个选项：
#if=文件名：指定输入文件名或者设备名，如果省略“if=文件名”，则表示从标准输入读取。
#of=文件名：指定输出文件名或者设备名，如果省略“of=文件名”，则表示写到标准输出。
dd if=/dev/sda of=/root/sda.img
```

通过上面的 dd 命令，我们得到了 sda.img 文件，它就是已经备份好了的磁盘映像文件，里面存储着 /dev/sda 整块硬盘的内容。

假如 /dev/sda 硬盘真的出现了故障，我们就可以将曾经备份的 sda.img 复制到另一台电脑上，并将其恢复到指定的 sdb 盘中去。

```sh
 dd if=/root/sda.img of=/dev/sdb
```



## 备份时进行压缩

```sh
dd if=/dev/sda | gzip > /root/sda.img.gz
dd if=/dev/sda | bzip2 > disk.img.bz2
bzip2 -dc /root/sda.img.gz | dd of=/dev/sdc
```

## 备份磁盘的 MBR

MBR，是 Master Boot Record，即硬盘的主引导记录，MBR 一旦损坏，分区表也就被破坏，数据大量丢失，系统就再也无法正常引导了

```sh
dd if=/dev/sda of=/root/sda_mbr.img count=1 bs=512
dd if=/root/sda_mbr.img of=/dev/sda
```



## 使用 /dev/zero 和 /dev/null 来测试磁盘

- /dev/null，也叫空设备，小名“无底洞”。任何写入它的数据都会被无情抛弃。
- /dev/zero，可以产生连续不断的 null 的流（二进制的零流），用于向设备或文件写入 null 数据，一般用它来对设备或文件进行初始化。
- /dev/urandom  它是“随机数设备”，它的本领就是可以生成理论意义上的随机数。

```
dd if=/dev/zero bs=1024 count=1000000 of=/root/1Gb.file
dd if=/root/1Gb.file bs=64k | dd of=/dev/null
time dd if=/dev/zero bs=1024 count=1000000 of=/root/1Gb.file
time dd if=/dev/zero bs=2048 count=500000 of=/root/1Gb.file
time dd if=/dev/zero bs=4096 count=250000 of=/root/1Gb.file


dd if=/dev/urandom of=/dev/sda
```

