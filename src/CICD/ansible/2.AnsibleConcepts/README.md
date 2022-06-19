# [Ansible concepts](https://docs.ansible.com/ansible/latest/user_guide/basic_concepts.html#ansible-concepts)

这些概念对于Ansible的所有用途都是通用的。您需要了解它们才能将Ansible用于任何类型的自动化。本基本介绍提供了您需要遵循用户指南其余部分的背景。

- [Control node](https://docs.ansible.com/ansible/latest/user_guide/basic_concepts.html#control-node)
- [Managed nodes](https://docs.ansible.com/ansible/latest/user_guide/basic_concepts.html#managed-nodes)
- [Inventory](https://docs.ansible.com/ansible/latest/user_guide/basic_concepts.html#inventory)
- [Collections](https://docs.ansible.com/ansible/latest/user_guide/basic_concepts.html#collections)
- [Modules](https://docs.ansible.com/ansible/latest/user_guide/basic_concepts.html#modules)
- [Tasks](https://docs.ansible.com/ansible/latest/user_guide/basic_concepts.html#tasks)
- [Playbooks](https://docs.ansible.com/ansible/latest/user_guide/basic_concepts.html#playbooks)

## [Control node](https://docs.ansible.com/ansible/latest/user_guide/basic_concepts.html#id1)

1. 任何安装了Ansible的机器
2. 您可以通过从任何控制节点调用Ansible或ansible-playbook命令来运行ansible命令和playbook。
3. 您可以使用任何具有Python安装的计算机作为控制节点-笔记本电脑，共享台式机和服务器都可以运行Ansible。
4. 但是，您不能将Windows机器用作控制节点。
5. 您可以有多个控制节点。



## [Managed nodes](https://docs.ansible.com/ansible/latest/user_guide/basic_concepts.html#id2)

您使用Ansible管理的网络设备 (和/或服务器)。托管节点有时也称为 “主机”。Ansible未安装在托管节点上。

## [Inventory](https://docs.ansible.com/ansible/latest/user_guide/basic_concepts.html#id3)



1. 托管节点列表。inventory file 有时也称为 “hostfile”
2. 您的清单可以为每个托管节点指定像ip地址这样的信息
3. 清单还可以组织托管节点，创建和嵌套组，以便于扩展
4. 详见： [the Working with Inventory](https://docs.ansible.com/ansible/latest/user_guide/intro_inventory.html#intro-inventory) 



## [Collections](https://docs.ansible.com/ansible/latest/user_guide/basic_concepts.html#id4)

1. Collections 是Ansible内容的发布格式，可以包括剧本、角色、模块和插件。

2.  You can install and use collections through [Ansible Galaxy](https://galaxy.ansible.com/). 
3. To learn more about collections, see [Using collections](https://docs.ansible.com/ansible/latest/user_guide/collections_using.html#collections).

## [Modules](https://docs.ansible.com/ansible/latest/user_guide/basic_concepts.html#id5)

1. Ansible执行的代码单位
2. 每个模块都有特定的用途，从在特定类型的数据库上管理用户到在特定类型的网络设备上管理VLAN接口
3. 您可以使用任务调用单个模块，或者在剧本中调用几个不同的模块
4. 从Ansible 2.10开始，模块在集合中分组
5. 关于Ansible包括多少个collections，详见 [Collection Index](https://docs.ansible.com/ansible/latest/collections/index.html#list-of-collections).

## [Tasks](https://docs.ansible.com/ansible/latest/user_guide/basic_concepts.html#id6)

Ansible中的行动单位。您可以使用临时命令执行一次单个任务。

## [Playbooks](https://docs.ansible.com/ansible/latest/user_guide/basic_concepts.html#id7)

1. 已保存的任务顺序列表，以便您可以按该顺序重复运行这些任务

2. 剧本可以包括变量和任务。
3. 剧本用YAML编写，易于阅读，写作，分享和理解
4. 详见： [Intro to playbooks](https://docs.ansible.com/ansible/latest/user_guide/playbooks_intro.html#about-playbooks).



