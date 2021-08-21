# NGINX启停

TERM,INT : quickShutdown

QUIT: graceful shutdown

HUP: 平滑的重载配置文件

USR1:重读日志文件,日志分割时有用

USR2:平滑升级

WINCH:优雅的关闭旧的进程

# Nginx目录描述

| 路径                      | 含义             |
| ------------------------- | ---------------- |
| /etc/nginx/nginx.conf     | 主配置文件       |
| /run/nginx.pid            | master进程的pid  |
| /var/log/nginx/access.log | 访问日志         |
| /var/log/nginx/error.log  | 错误日志         |
| /usr/share/nginx/html     | 默认静态文件位置 |
| /usr/share/nginx/modules  | 模块存放位置     |
| /etc/nginx/fastcgi.conf   | 可引用变量的集合 |
| /etc/nginx/mime.types     | 支持的文件类型   |
|                           |                  |



# Nginx配置块

* nginx是基于配置的静态服务器
* 包含以下几个配置

## 全局区

worker_processes 1; //工作进程的个数

## Event区

配置nginx连接特性

work_connections 1024 一个子进程最大允许建立的连接数

## server区

* 访问主机名:监听端口:访问地址/
* 访问地址 对应 静态文件的跟位置
* 首页



每个指令都有相应的上下文

# Location匹配

###  访问 192.168.0.1/ 的过程

0. 在匹配过程中 默认以 URL的最后一个名字为 文件名查找

1. 精准匹配 ,"/" ,发生URL重写
2. 重写后的 URL 192.168.0.1/index.html,然后去该精准匹配的块寻找该文件
3. 如果该文件没有找到,则继续寻找下一个匹配的 location,看是否存在 index.html
4. 匹配 location /aaa ,时不会截取匹配块

### 匹配类型

| 语法              | 匹配     | 匹配特点                                                     |
| ----------------- | -------- | ------------------------------------------------------------ |
| location = url{}  | 精准匹配 | 必须完全匹配                                                 |
| location url{...} | 一般匹配 | 按前缀匹配,记住前缀匹配最长的,等待正则匹配结束               |
| location ~{...}   | 正则匹配 | 正则匹配按顺序匹配,一旦匹配命中就返回结果,如果正则不命中,则返回前普通匹配结果 |
|                   |          |                                                              |

# Rewrite语法

* URL重写
* 可以放在location块
* 也可以放在server块

## 条件判断

### 示例

```nginx
//针对某个ip禁止访问
if ($remote_addr = 192.168.1.100) { return 403}

//重写URL
if ($http_user_agent ~ MSIE){
    rewrite ^.*$ ie.html
}
1.这样重写会导致请求一致被重写
2.因此需要break,这个location不参与之后的匹配
if ($http_user_agent ~ MSIE){
    rewrite ^.*$ ie.html
    break;
}

```

### 语法

=  :if ( $variablename = 1111)

~ :正则匹配 , 区分大小写,

~*:不区分大小写的正则

-f,-d,-e 类似于Linux的 文件判断

重写类似于内部转发,302重定向



Set是设置变量使用

```
if ($http_user_agent ~* msie) {
	set $isie 1;
}
if($fastcgi_script_name = ie.html){
	set $isie 0;
}
if($isie 1){
	rewrite ^.*$ ie.html
}

```



Http压缩

二进制文件压缩比很小,不建议对二进制启用

只针对文本压缩

gzip on|off

gzip_buffers 32 4K ,在内存缓冲多少块,每块多大

gzip_comp_level [1-9]推荐6

gzip_disable   #(正则匹配) UA 

gzip_min_length 200 #压缩的最小长度

gzip_http_version 1.0|1.1 

gzip_proxied 

gzip_types text/plain,application/xml

gzip_vray on|off



expire缓存

设置nginx中 过期时间

expires 20h;

原理是 当服务器请求时,