# 前言

**inventor文件**

Ansible 可同时操作属于一个组的多台主机,组和主机之间的关系通过 inventory 文件配置. 默认的文件路径为 `/etc/ansible/hosts`

**组名分组**

方括号[]中是组名,用于对系统进行分类,便于对不同系统进行个别的管理.

**一个系统可从属不同的组**

一个系统可以属于不同的组,比如一台服务器可以同时属于 webserver组 和 dbserver组

```ini
mail.example.com

[webservers]
foo.example.com
bar.example.com

[dbservers]
one.example.com
two.example.com
three.example.com
```

**ssh连接收管主机**

如果有主机的SSH端口不是标准的22端口,可在主机名之后加上端口号,用冒号分隔

```
badwolf.example.com:5309
```





# 构建清单

* 清单文件是 受管主机的IP或主机名 列表

* 组名 用 [groupname]标识，组之间用 组名分隔

* 可以使用YAML格式

* 组

    * 有两个默认组，all, ungruoped
    * all组包含每一个主机
    * ungrouped 包含没有组的主机
    * 每一个组至少有两个组 all，ungrouped
    * 每一个主机 可以放在多个组

* 主机名 符号

    * 数值区间：`www[01:50].example.com`
    * 字母区间：`db-p[a:f].example.com`

* 添加变量

    * INI：`host1 http_port=80 maxRequestsPerChild=808`

    * YAML：

        ```yaml
        atlanta:
          host1:
            http_port: 80
            maxRequestsPerChild: 808
        
        ```

* 添加组变量，`:vars`

    ```
    [atlanta]
    host1
    host2
    
    [atlanta:vars]
    ntp_server=ntp.atlanta.example.com
    proxy=proxy.atlanta.example.com
    ```

    ```
    atlanta:
      hosts:
        host1:
        host2:
      vars:
        ntp_server: ntp.atlanta.example.com
        proxy: proxy.atlanta.example.com
    ```

    

* 使用children: 给组分组

    ```yaml
    all:
      children:
        usa:
          children:
            southeast:
              children:
                atlanta:
                  hosts:
                    host1:
                    host2:
                raleigh:
                  hosts:
                    host2:
                    host3:
              vars:
                some_server: foo.southeast.example.com
                halon_system_timeout: 30
                self_destruct_countdown: 60
                escape_pods: 2
            northeast:
            northwest:
            southwest:
    ```

* 子组的变量 会覆盖父组的变量

* 主机变量与 组变量 可以定义在如下路径

    ```
    /etc/ansible/group_vars/raleigh # can optionally end in '.yml', '.yaml', or '.json'
    /etc/ansible/group_vars/webservers
    /etc/ansible/host_vars/foosball
    ```

    