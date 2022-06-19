# [Installation](http://ansible.com.cn/docs/intro_installation.html#id9)



### [从Github获取Ansible](http://ansible.com.cn/docs/intro_installation.html#id10)

如果你有一个github账户,可以跟进Ansible在Github的项目: [Github project](https://github.com/ansible/ansible) 我们在这里保持对bugs和feature ideas的跟踪.





### [需要安装些什么](http://ansible.com.cn/docs/intro_installation.html#id11)

1. Ansible默认通过 SSH 协议管理机器.

2. 安装Ansible之后,不需要启动或运行一个后台进程,或是添加一个数据库.只要在一台电脑(可以是一台笔记本)上安装好,就可以通过这台电脑管理一组远程的机器.在远程被管理的机器上,不需要安装运行任何软件,
3. 因此升级Ansible版本不会有太多问题.

### [选择哪一个版本?](http://ansible.com.cn/docs/intro_installation.html#id12)

1. 因为Ansible可以很简单的从源码运行,且不必在远程被管理机器上安装任何软件,很多Ansible用户会跟进使用开发版本.

2. Ansible一般每两个月出一个发行版本.小bugs一般在下一个发行版本中修复,并在稳定分支中做backports.

3. 大bugs会在必要时出一个维护版本,不过这不是很频繁.
4. 若你希望使用Ansible的最新版本,并且你使用的操作系统是 Red Hat Enterprise Linux (TM), CentOS, Fedora, Debian, Ubuntu,我们建议使用系统的软件包管理器.
5. 另有一种选择是通过”pip”工具安装,”pip”是一个安装和管理Python包的工具.
6. 若你希望跟进开发版本,想使用和测试最新的功能特性,我们会分享如何从源码运行Ansible的方法.从源码运行程序不需要进行软件安装.



### [对管理主机的要求](http://ansible.com.cn/docs/intro_installation.html#id13)

1. 目前,只要机器上安装了 Python 2.6 或 Python 2.7 (windows系统不可以做控制主机),都可以运行Ansible.
2. 主机的系统可以是 Red Hat, Debian, CentOS, OS X, BSD的各种版本,等等.
3. 自2.0版本开始,ansible使用了更多句柄来管理它的子进程,对于OS X系统,你需要增加ulimit值才能使用15个以上子进程,方法 sudo launchctl limit maxfiles 1024 2048,否则你可能会看见”Too many open file”的错误提示.





### [对托管节点的要求](http://ansible.com.cn/docs/intro_installation.html#id14)

1. 通常我们使用 ssh 与托管节点通信，默认使用 sftp.如果 sftp 不可用，可在 ansible.cfg 配置文件中配置成 scp 的方式
2.  在托管节点上也需要安装 Python 2.4 或以上的版本.如果版本低于 Python 2.5 ,还需要额外安装一个模块:`python-simplejson`
3. 没安装python-simplejson,也可以使用Ansible的”raw”模块和script模块,因此从技术上讲,你可以通过Ansible的”raw”模块安装python-simplejson,之后就可以使用Ansible的所有功能了.
4. 如果托管节点上开启了SElinux,你需要安装libselinux-python,这样才可使用Ansible中与copy/file/template相关的函数.你可以通过Ansible的yum模块在需要的托管节点上安装libselinux-python.
5. Python 3 与 Python 2 是稍有不同的语言,大多数Python程序还不能在 Python 3 中正确运行.一些Linux发行版(Gentoo, Arch)没有默认安装 Python 2.X 解释器.在这些系统上,你需要安装一个 Python 2.X 解释器,并在 inventory (详见 [*Inventory文件*](http://ansible.com.cn/docs/intro_inventory.html)) 中设置 ‘ansible_python_interpreter’ 变量指向你的 2.X Python.你可以使用 ‘raw’ 模块在托管节点上远程安装Python 2.X.
6. 例如：ansible myhost --sudo -m raw -a "yum install -y python2 python-simplejson" 这条命令可以通过远程方式在托管节点上安装 Python 2.X 和 simplejson 模块.
7. Red Hat Enterprise Linux, CentOS, Fedora, and Ubuntu 等发行版都默认安装了 2.X 的解释器,包括几乎所有的Unix系统也是如此.

### [安装管理主机](http://ansible.com.cn/docs/intro_installation.html#id15)

1. 从项目的checkout中可以很容易运行Ansible,Ansible的运行不要求root权限,也不依赖于其他软件,不要求运行后台进程,也不需要设置数据库.

2. 因此我们社区的许多用户一直使用Ansible的开发版本,这样可以利用最新的功能特性,也方便对项目做贡献.因为不需要安装任何东西,跟进Ansible的开发版相对于其他开源项目要容易很多.



#### 从源码安装的步骤

```
$ git clone git://github.com/ansible/ansible.git --recursive
$ cd ./ansible
```

#### 使用 Bash:

```
$ source ./hacking/env-setup
```

#### 使用 Fish:

```
$ . ./hacking/env-setup.fish
```

If you want to suppress spurious warnings/errors, use:

```
$ source ./hacking/env-setup -q
```

如果没有安装pip, 请先安装对应于你的Python版本的pip:

```
$ sudo easy_install pip
```

以下的Python模块也需要安装

```
$ sudo pip install paramiko PyYAML Jinja2 httplib2 six
```

注意,当更新ansible版本时,不只要更新git的源码树,也要更新git中指向Ansible自身模块的 “submodules” (不是同一种模块)

```
$ git pull --rebase
$ git submodule update --init --recursive
```



一旦运行env-setup脚本,就意味着Ansible从源码中运行起来了.默认的inventory文件是 /etc/ansible/hosts.inventory文件也可以另行指定 (详见 [*Inventory文件*](http://ansible.com.cn/docs/intro_inventory.html)) :



```
$ echo "127.0.0.1" > ~/ansible_hosts
$ export ANSIBLE_HOSTS=~/ansible_hosts
```



你可以在手册的后续章节阅读更多关于 inventory 文件的使用,现在让我们测试一条ping命令:

```
$ ansible all -m ping --ask-pass
```

```
你也可以使用命令 “sudo make install”
```



