# Description

cobbler 都是一个Linux安装的服务 能够让你快速 搭建网络安装环境。把很多与Linux相关的任务粘合在一起，就不必再部署应用时 不必再各种命令行跳来跳去，Cobbler可以帮助你配置管理DNS，DHCP,包更新，电源管理，配置管理安排等等

# quickstart

* installation

```
setselinux 1
yum install cobbler
```

* 修改 yaml 格式的 设置/etc/cobbler/settings

  ```yaml
  default_password_crypted  //设置新的root密码：openssl passwd -1
  
  server: 127.0.0.1 // HTTP TFTP服务器
  
  next_server:127.0.0.1 // network bootfile 下载的 TFTP服务器IP
  
  manage_dhcp: 0 //是否启动DHCP服务，默认关闭，开启后会根据/etc/cobbler/dhcp.template 生成dhcpd.conf 配置文件
  
  vi /etc/cobbler/dhcp.template
  subnet 192.168.1.0 netmask 255.255.255.0 {
       option routers             192.168.1.1;
       option domain-name-servers 192.168.1.210,192.168.1.211;
       option subnet-mask         255.255.255.0;
       filename                   "/pxelinux.0";
       default-lease-time         2.8.0;
       max-lease-time             43200;
       next-server                $next_server;
  }
  不要修改next-server                $next_server;
  ```

* 文件和目录注释
  * /var/www/cobbler/ks_mirror 是镜像与发行版本

