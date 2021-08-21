# 简介

当人们提到 Linux 时，他们通常指的是 Linux 发行版。

严格来说，Linux 是一个内核，是操作系统的核心组件，简单地说，它就像是软件应用程序和硬件之间的桥梁。 



**Linux 发行版是由 Linux 内核、GNU 工具和库以及软件集合组成的操作系统。**



通常，Linux 发行版包括

1. 桌面环境
2. 包管理系统和
3. 一组预装的应用程序。



一些最流行的 Linux 发行版是 Debian、Red Hat、Ubuntu、Arch Linux、Fedora、CentOS、Kali Linux、OpenSUSE、Linux Mint 等。

当您第一次登录 Linux 系统时，在做任何工作之前，最好先检查一下机器上运行的是什么版本的 Linux。

例如，确定 Linux 发行版可以帮助您确定应该使用哪个包管理器来安装新包。



本文展示了如何使用命令行检查系统上安装的 Linux 发行版和版本。







# `lsb_release` 

lsb_release 实用程序显示有关 Linux 发行版的 LSB（Linux 标准库）信息。此命令应该适用于安装了 lsb-release 软件包的所有 Linux 发行版：

```sh
lsb_release -a
```

```output
No LSB modules are available.
Distributor ID:	Debian
Description:	Debian GNU/Linux 9.5 (stretch)
Release:	9.5
Codename:	stretch
```



# `/etc/os-release`

 /etc/os-release 文件包含操作系统标识数据，包括有关分发的信息。

这个文件是 systemd 包的一部分，应该存在于所有运行 systemd 的系统上。

要查看 os-release 文件的内容，请使用 cat 或 less ：

```sh
echo "$(cat /etc/os-release|grep -w ID= | awk -F'=' '{print $2}'|sed 's/"//g')"
```







# `/etc/issue`

文件包含在登录提示之前打印的系统标识文本。

通常，此文件包含有关 Linux 版本的信息：



## `hostnamectl` command

hostnamectl 实用程序是 systemd 的一部分，用于查询和更改系统主机名。

此命令还显示 Linux 发行版和内核版本。



# `/etc/*release`

如果上述命令都不适合您，那么很可能您正在运行一个非常陈旧且过时的 Linux 发行版。

在这种情况下，您可以使用以下命令之一，它应该打印发行版或版本文件的内容



