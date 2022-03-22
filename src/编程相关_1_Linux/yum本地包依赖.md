# 按需构建

## 1.把需要的rpm包下载到本地

```sh
yum install --downloadonly --downloaddir=/yum/ mysql-community-server
```

## 2.生成yum仓库数据文件信息(repodate信息)

```sh
yum install createrepo -y 	# 安装createrepo
createrepo /yum/			# 生成repomd.xml文件
```

*# 使用完成后会在/yum/目录里面生成repodate，这个文件里面存放的就是仓库的各项信息* 

## 3.配置本地仓库

```sh
echo '[local]
name=local repository
baseurl=file:///yum
enabled=1
gpgcheck=0'>
/etc/yum.repos.d/local.repo
```

## 4.检查本地仓库信息

```sh
yum repoinfo local		
# 这里的local可以是Repo-id，也可以是Repo-name
# Repo-id ：配置文件[]里面的内容
# Repo-name ：配置文件name的字段
# 输出正常表示可以进行正常使用
```



## 5.通过nginx配置URL访问



# 全库同步

```sh
reposync -r "$repoid" -p /yum	# $repoid就是镜像的id，同步并更新
createrepo --update /yum		# 更新repodate信息
yum clean all && yum repolist	# 清除缓存
```





# 其他

```sh
yum-config-manager enable/disable
```

