# 概述

[playBooks 练习](https://github.com/ansible/ansible-examples) 



# PlayBook语言示例

* playbook 由一个或多个 ‘plays’ 组成.它的内容是一个以 ‘plays’ 为元素的列表.
* play是由一系列 中的 tasks组成
* 一个任务是对一个 absinel模块的调用

```yaml
---
- hosts: webservers
  vars:
    http_port: 80
    max_clients: 200
  remote_user: root
  tasks:
  - name: ensure apache is at the latest version
    yum: pkg=httpd state=latest
  - name: write the apache config file
    template: src=/srv/httpd.j2 dest=/etc/httpd.conf
    notify:
    - restart apache
  - name: ensure apache is running
    service: name=httpd state=started
  handlers:
    - name: restart apache
      service: name=httpd state=restarted
```

# playbook基础

## 主机与用户

**要执行的主机，与用户**

```yaml
---
- hosts: webservers
  remote_user: root
```

**再者,在每一个 task 中,可以定义自己的远程用户:**

```yaml
---
- hosts: webservers
  remote_user: root
  tasks:
    - name: test connection
      ping:
      remote_user: yourname
```

**也支持从 sudo 执行命令:**

```sh
---
- hosts: webservers
  remote_user: yourname
  sudo: yes
```

同样的,你可以仅在一个 task 中,使用 sudo 执行命令,而不是在整个 play 中使用 sudo:

```sh
---
- hosts: webservers
  remote_user: yourname
  tasks:
    - service: name=nginx state=started
      sudo: yes
```

你也可以登陆后,sudo 到不同的用户身份,而不是使用 root:

```sh
---
- hosts: webservers
  remote_user: yourname
  sudo: yes
  sudo_user: postgres
```

> 如果你需要在使用 sudo 时指定密码,可在运行 ansible-playbook 命令时加上选项 `--ask-sudo-pass` (-K). 如果使用 sudo 时,playbook 疑似被挂起,可能是在 sudo prompt 处被卡住,这时可执行 Control-C 杀死卡住的任务,再重新运行一次.



> 当使用 sudo_user 切换到 非root 用户时,模块的参数会暂时写入 /tmp 目录下的一个随机临时文件. 当命令执行结束后,临时文件立即删除.这种情况发生在普通用户的切换时,比如从 ‘bob’ 切换到 ‘timmy’, 切换到 root 账户时,不会发生,如从 ‘bob’ 切换到 ‘root’,直接以普通用户或root身份登录也不会发生. 如果你不希望这些数据在短暂的时间内可以被读取（不可写）,请避免在 sudo_user 中传递未加密的密码. 其他情况下,’/tmp’ 目录不被使用,这种情况不会发生.Ansible 也有意识的在日志中不记录密码参数.





## Tasks 列表

每一个 play 包含了一个 task 列表（任务列表）.一个 task 在其所对应的所有主机上（通过 host pattern 匹配的所有主机）执行完毕之后,下一个 task 才会执行

有一点需要明白的是（很重要）,在一个 play 之中,所有 hosts 会获取相同的任务指令

每个 task 的目标在于执行一个 moudle, 通常是带有特定的参数来执行.在参数中可以使用变量（variables）.









