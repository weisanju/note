### 重启

```
multipass exec primary sudo systemctl restart elasticsearch

multipass exec node-2 sudo systemctl restart elasticsearch
 
multipass exec node-3 sudo systemctl restart elasticsearch
```



### 配置AKSK

```
export accessKey=xxxx
export secretkey=xxxxxxxxxxx
export nodeName=node-3
echo $accessKey | multipass exec $nodeName  sudo /usr/share/elasticsearch/bin/elasticsearch-keystore \
add -- -f  s3.client.default.access_key 

echo $secretkey | multipass exec $nodeName  sudo /usr/share/elasticsearch/bin/elasticsearch-keystore \
add   -- -f s3.client.default.secret_key 
```



### 配置Client

```
PUT _snapshot/mys3repository
{
  "type": "s3",
  "settings": {
    "bucket": "bucket-name",
    "base_path": "xiaojiaquan", 
    "endpoint":"xxx-beijing.aliyuncs.com",
    "protocol":"http",
    "compress": false,
    "disable_chunked_encoding":true
  }
}
```


### 拍摄快照
```
PUT /_snapshot/mys3repository/snapshot_3
{
  "indices": "person",
  "ignore_unavailable": true,
  "metadata": {
    "taken_by": "weisanju",
    "taken_because": "firstS3Backup"
  }
}
```

### 恢复快照

```

POST /_snapshot/mys3repository/snapshot_2/_restore
{
  "include_global_state": false,
  "ignore_unavailable": true,
  "indices": "person",
  "rename_pattern": "(person)",
  "rename_replacement": "$1_copy2_restore"
}
```

