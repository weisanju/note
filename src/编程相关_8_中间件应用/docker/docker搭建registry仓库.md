# 总览

> registry 是一个无状态、高度可扩展的服务器端应用程序，用于存储和分发 Docker 镜像







# 基本命令

**建立镜像**

```
docker run -d -p 5000:5000 --name registry registry:2
```

**推送与拉取镜像**

```sh
docker pull ubuntu
docker image tag ubuntu localhost:5000/myfirstimage
docker push localhost:5000/myfirstimage
docker pull localhost:5000/myfirstimage
```

**删除镜像**

```sh
docker container stop registry && docker container rm -v registry
```





# 镜像命名

典型 docker 命令中使用的 镜像名称反映了它们的来源：

- `docker pull ubuntu` instructs docker to pull an image named `ubuntu` from the official Docker Hub. This is simply a shortcut for the longer `docker pull docker.io/library/ubuntu` command
- `docker pull myregistrydomain:port/foo/bar` instructs docker to contact the registry located at `myregistrydomain:port` to find the image `foo/bar`





# 搭建外部访问的 registry

## 获取自签名证书

**修改  /etc/pki/tls/openssl.cnf**

```
sed  '/^\[ v3_ca \]/  a subjectAltName = IP:192.168.1.73'   /etc/pki/tls/openssl.cnf
```

**生成证书**

```sh
openssl req \
  -newkey rsa:4096 -nodes -sha256 -keyout certs/domain.key \
  -x509 -days 365 -out certs/domain.crt
  
  #-addext "subjectAltName = IP:192.168.1.73" \

```

**加入到docker**

```
docker secret rm domain.crt
docker secret rm domain.key
docker secret create domain.crt certs/domain.crt
docker secret create domain.key certs/domain.key
```

**copy证书到 所有docker**

```
cp certs/domain.crt  /etc/docker/certs.d/192.168.1.73:5000/ca.crt

```

**创建镜像**

```
docker service create \
  --name registry \
  --secret domain.crt \
  --secret domain.key \
  --constraint 'node.role==manager' \
  --mount type=bind,source=/data/registry,destination=/var/lib/registry \
  -e REGISTRY_HTTP_TLS_CERTIFICATE=/run/secrets/domain.crt \
  -e REGISTRY_HTTP_TLS_KEY=/run/secrets/domain.key \
  --publish published=5000,target=5000 \
  --replicas 1 \
  registry:2
```



