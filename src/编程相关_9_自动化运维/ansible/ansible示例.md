

# 公钥互信机制

```sh
ssh-key-gen -t rsa
ssh-copy-key 192.168.3.101  192.168.3.102 192.168.3.103
```





# ping所有主机

```
# ping所有主机
ansible all -m ping
# 带用户名
ansible all -m ping -u bruce  
# 以sudo运行
ansible all -m ping -u bruce --sudo
# 以 sudo用户执行
ansible all -m ping -u bruce --sudo --sudo-user batman
```



# 往所有机器上写东西

```shell
ansible myserver  -m shell -a "echo helloWorld>~/a.txt"
```



# 包管理

```shell
# 安装 epel数据源
ansible myserver -m yum -a "name=https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm state=present"

# 安装nginx
ansible myserver -m yum -a "name=nginx.x86_64 state=present"
```

