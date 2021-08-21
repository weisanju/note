# 设置webhook

以 gitee为例

1. git生成token,[生成地址](https://gitee.com/profile/personal_access_tokens)
2. Jenkins安装 git插件 重启
3. 复制其 webhookURL到gitee项目的 [webhook](https://gitee.com/weisanju/note/hooks)

4. 在Jenkins中生成webhook密码
5. 测试推送是否成功

# 为Jenkins启用反向代理

1. 修改Jenkins上下文路径

   在文件 /etc/default/jenkins 

   ```
   JENKINS_ARGS="--webroot=/var/cache/$NAME/war --httpPort=$HTTP_PORT --prefix=/jenkins"
   ```

   

2. Manage Jenkins / Configure System，将Jenkins URL后添加`/jenkins`

3. nginx配置

   ```nginx
   	server{
   		listen 192.168.3.15:80;
   		server_name  "";
   		#root /home/pi/gitbook/_book;
   		location /jenkins/ {
   			 proxy_pass http://localhost:8080;
   		}
   	}
   ```

   




