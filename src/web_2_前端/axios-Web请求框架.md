{% raw %}

# 功能特点

- 浏览器发送 XMLHttpRequest
- nodejs 中发送 Http 请求
- 支持 PromiseAPI
- 拦截请求和响应
- 转换请求和响应数据

# 支持的请求方式

- _axios(config)_
- _axios.request(config)_
- _axios.get(url[,config])_
- _axios.delete(url[,config])_
- _axios.head(url[,config])_
- _axios.post(url[,data[,config]])_
- _axios.put(url[,data[,config]])_
- _axios.patch(url[,data[,config]])_

# 示例

## 基本使用

```js
import Axios from "axios";
Axios({
  url: "http://httpbin.org//",
  params: {
    type: "pop",
    page: 1,
  },
})
  .then((e) => {
    console.log(e);
  })
  .catch((e) => {
    console.log(e);
  });
```

## 多请求发送并合并

```js
Axios.all([
  Axios({
    url: "http://httpbin.org/post",
    method: "post",
  }),
  Axios({
    url: "http://httpbin.org/",
  }),
]).then(([r1, r2]) => {
  console.log(r1);
  console.log(r2);
});
```

# 全局配置

```
Axios.defaults.baseURL = 'http://httpbin.org'
Axiox.defaults.timeout = 5000
Axios.defaults.post['Content-Type'] = 'application/x-www-frorm-urlencoded'
```

## 常见配置项

| 配置项                                                        | 说明               |
| ------------------------------------------------------------- | ------------------ |
| url                                                           |                    |
| method                                                        | 请求方法,get,post  |
| baseURL                                                       | 基本路径           |
| transformRequest:[function(data){}]                           | 请求前的数据处理   |
| transformResponse:[function(data){}]                          | 请求后的数据处理   |
| headers:{'x-Requested-With':'XMLHttpRequest' }                | 自定义请求头       |
| params:{}                                                     | 查询对象           |
| paramsSerializer:function(params){}                           | 查询对象序列化哈数 |
| requestBody: data:{key:'aa'}                                  | 请求体             |
| timeout:1000                                                  | 超时设置毫秒       |
| withCredentials:false                                         | 跨域是否携带 token |
| adapter:funtion(resolve,reject,config)                        | 自定义请求处理     |
| auth:{ uname:'',pwd:''}                                       | 简单身份认证       |
| responseType:'json' json,blob,document,arraybuffer,textstream | 响应数据格式       |

{% endraw %}
