# 资源

[docker镜像](https://hub.docker.com/r/oracleinanutshell/oracle-xe-11g)

[客户端下载](https://www.oracle.com/database/technologies/instant-client/downloads.html)



# 安装Oracle服务器

```sh
docker pull oracleinanutshell/oracle-xe-11g
docker run -d -p 49161:1521 -e ORACLE_ALLOW_REMOTE=true oracleinanutshell/oracle-xe-11g

```

**默认配置**

```
hostname: localhost
port: 49161
sid: xe
username: system
password: oracle
```



# 客户端

* 新建 *ORACLE_HOME* 
* 新建 *TNS_ADMIN*   *NETWORK/ADMIN* 环境变量

