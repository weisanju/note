### 启动命令

```
sudo docker run -v /home/lighthouse/wallabag/data:/var/www/wallabag/data  -v /home/lighthouse/wallabag/images:/var/www/wallabag/web/assets/images -p 80:80  -e "SYMFONY__ENV__DOMAIN_NAME=http://IP" wallabag/wallabag
```

