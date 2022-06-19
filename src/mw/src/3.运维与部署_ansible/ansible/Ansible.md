# 关于Ansible

Ansible是一个IT自动化工具，能够配置系统，部署软件，编码高级的IT任务例如 持续部署或者 0宕机 滚动更新



# 安装Ansible

## 安装Ansible自身

`sudo yum install ansible`

## 安装shell命令行自动补全

`yum install epel-release`

`yum install python-argcomplete`

## 自动补全设置

`activate-global-python-argcomplete`



# 概念

## 控制节点

能够运行Ansible命令 和playbooks 通过

`/usr/bin/ansible or/usr/bin/ansible-playbook`

## 受管节点

Ansible管理的网络设备

`/etc/ansible/hosts` 中记录着 受管结点的主机名



## Inventory

一系列的受管节点，清单文件也叫做 hostfile,可以为每个受管节点指定IP，也可以用来创建或者嵌套组，方便扩容，

## Modules

ansible功能单位，每一个module都有专门的 功能，从管理特定数据库的用户到 管理特定类型的网络设备VLAN接口，可以执行一个模块的一个task，也可以执行多个模块的多个功能，也就是剧本

## tasks

Ansible的 执行动作单位

## playbooks

* 有序的任务列表
* 以YAML方式写的

# 动态清单

description

* 如果你的配置根据需求 时常变动，你可能需要从多个源头 载入hosts，例如云服务提供商，LDAP，Cobber,或者其他企业的CMDB
* Ansible提供两种方式，连接外部存储
  * inventory plugins：推荐使用plugins
  * inventory scripts
* 红帽的 RedHatAnsibleTower 提供GUI界面编辑与同步，并提供web and Rest服务

example with Cobber

* Ansible能与cobber无缝集成。cobber主要用于OS安装，DHCP,DNS 管理，从当轻量级的CMDB



# 模式：定位主机和组

* `ansible <pattern> -m <module_name> -a "<module options>""`

* pattern 是 playbook的 hosts 选项

* pattern模式

  | Description            | Pattern(s)                   | Targets                                             |
  | ---------------------- | ---------------------------- | --------------------------------------------------- |
  | All hosts              | all (or *)                   |                                                     |
  | One host               | host1                        |                                                     |
  | Multiple hosts         | host1:host2 (or host1,host2) |                                                     |
  | One group              | webservers                   |                                                     |
  | Multiple groups        | webservers:dbservers         | all hosts in webservers plus all hosts in dbservers |
  | Excluding groups       | webservers:!atlanta          | all hosts in webservers except those in atlanta     |
  | Intersection of groups | webservers:&staging          | any hosts in webservers that are also in staging    |

* pattern高级用法

  * 使用变量 ， ansible-playbook， -e 传递的ansible-playbook

  * 使用组定位，

    ```
    webservers[0]       # == cobweb
    webservers[-1]      # == weber
    webservers[0:2]     # == webservers[0],webservers[1]
                        # == cobweb,webbing
    webservers[1:]      # == webbing,weber
    webservers[:3]      # == cobweb,webbing,weber
    ```

  * 使用正则表达式 ，以 ~ 开头的

  * 在命令行选项指定 --limit 

    * ```
      ansible-playbook site.yml --limit datacenter2 //指定主机
      ```

    * ```
      ansible-playbook site.yml --limit @retry_hosts.txt //指定从文件读主机
      ```



