# 关于版本控制

版本控制是一种记录一个或若干文件内容变化,以便将来查阅特定版本修订情况的系统



## 版本控制系统的变迁

###  本地版本控制

* Revision Control System ([RCS](https://www.gnu.org/software/rcs/))  是一种最流行的本地版本控制系统
* 工作原理是在硬盘上保存补丁集（补丁是指文件修订前后的变化）,通过应用所有的补丁，可以重新计算出各个版本的文件内容。



### 集中化的版本控制系统

​	但是如何让在不同系统上的开发者协同工作？集中化的版本控制系统（Centralized Version Control Systems，简称 CVCS）应运而生,诸如 CVS、Subversion 以及 Perforce 等,都有一个单一的集中管理的服务器，保存所有文件的修订版本,而协同工作的人们都通过客户端连到这台服务器，取出最新的文件或者提交更新

​	如果中心数据库所在的磁盘发生损坏,项目的整个变更历史将会丢失



### 分布式版本控制系统

​	分布式版本控制系统（Distributed Version Control System，简称 DVCS）很好的解决了上面的问题,像 Git、Mercurial、Bazaar 以及 Darcs 等

​	客户端并不只提取最新版本的文件快照， 而是把代码仓库完整地镜像下来，包括完整的历史记录

​	任何一处协同工作用的服务器发生故障，事后都可以用任何一个镜像出来的本地仓库恢复



## 什么是git

### Git 和其它版本控制系统的差别

Git 和其它版本控制系统（包括 Subversion 和近似工具）的主要差别在于 Git 对待数据的方法

* 其他系统对待数据的方式

  一组基本文件和每个文件随时间逐步累积的差异,通常给称为 (**基于差异（delta-based）**)

* 而Git 更像是把数据看作是对小型文件系统的一系列快照
  * 每当你提交更新或保存项目状态时，它基本上就会对当时的全部文件创建一个快照并保存这个快照的索引
  * 为了效率,如果文件没有修改，Git 不再重新存储该文件，而是只保留一个链接指向之前存储的文件
  * Git 对待数据更像是一个 **快照流**。
  *  Git 更像是一个小型的文件系统

### GIT保证完整性

* Git 中所有的数据在存储前都计算校验和，然后以校验和来引用
* Git 用以计算校验和的机制叫做 SHA-1 散列
*  这是一个由 40 个十六进制字符(0~F)组成的字符串
* Git 数据库中保存的信息都是以文件内容的哈希值来索引



### Git 一般只添加数据

* 你执行的 Git 操作，几乎只往 Git 数据库中 **添加** 数据, 你很难让 Git 执行任何不可逆操作



### 三种状态

**已提交（committed）**

**已修改（modified）** 

**已暂存（staged）**



## GIT的配置

### 配置文件

*  `git config` 命令来帮助设置 `控制 Git 外观和行为的` 配置变量
* 这些变量可能会存储在三个不同的位置
  * /etc/gitconfig :所有用户的通用配置, `git config` 时带上 `--system` 选项时会读写该文件的配置变量
  * *~/.gitconfig` 或 `~/.config/git/config* 当前用户的变量,--global选项为读写此文件,为当前用户的所有仓库的通用配置
  * *.git/config* 针对该仓库 --local选项读取该文件,默认情况下使用它(当然，你需要进入某个 Git 仓库中才能让该选项生效)

每一个级别会覆盖上一级别的配置

### 设置用户信息

安装完 Git 之后，要做的第一件事就是设置你的用户名和邮件地址

```shell
$ git config --global user.name "John Doe"
$ git config --global user.email johndoe@example.com
```

### 文本编辑器

```shell
git config --global core.editor emacs

$ git config --global core.editor "'C:/Program Files/Notepad++/notepad++.exe' -multiInst -notabbar -nosession -noPlugin"
```



### 命令其他使用

你可能会看到重复的变量名，因为 Git 会从不同的文件中读取同一个配置,

```shell
//检查某一项配置
git config <key>
git config list
```



### 获取帮助

```shell
//全面手册
git help config

//快速参考
git add -h
```

