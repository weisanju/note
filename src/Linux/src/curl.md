# 概述

[curl](http://curl.haxx.se/)是一种命令行工具，作用是发出网络请求，然后得到和提取数据，显示在"标准输出"（stdout）上面。

# **查看网页源码**

直接在curl命令后加上网址，就可以看到网页源码。我们以网址www.sina.com为例

```
curl www.sina.com
```

**如果要把这个网页保存下来，可以使用`-o`参数**

```
curl -o [文件名] www.sina.com
```



# **响应重定向**

有的网址是自动跳转的。使用`-L`参数，curl就会跳转到新的网址。

```
curl -L www.sina.com
```

# **显示头信息**

`-i`参数可以显示http response的头信息，连同网页代码一起。

```
curl -i www.sina.com
```

`-I`参数则是只显示http response的头信息。

# **显示通信过程**

`-v`参数可以显示一次http通信的整个过程，包括端口连接和http request头信息。

更详细的

```
 curl --trace output.txt www.sina.com
 curl --trace-ascii output.txt www.sina.com
```

# **发送表单信息**

**GET方法**

```
curl example.com/form.cgi?data=xxx
```

**POST方法**

必须把数据和网址分开，curl就要用到--data参数。

```sh
curl -X POST --data "data=xxx" example.com/form.cgi
curl -X POST--data-urlencode "date=April 1" example.com/form.cgi

```



# POST 请求的数据体

```
curl -d 'login=emma' -d 'password=123' -X POST  https://google.com/login
```

`-d`参数可以读取本地文本文件的数据，向服务器发送。

```
curl -d '@data.txt' https://google.com/login
```

`--data-urlencode`参数等同于`-d`，发送 POST 请求的数据体，区别在于会自动将发送的数据进行 URL 编码。

```
$ curl --data-urlencode 'comment=hello world' https://google.com/login
```



# **HTTP动词**

curl默认的HTTP动词是GET，使用`-X`参数可以支持其他动词。

```
curl -X POST www.example.com
curl -X DELETE www.example.com
```



# 文件上传

```html
　　<form method="POST" enctype='multipart/form-data' action="upload.cgi">
　　　　<input type=file name=upload>
　　　　<input type=submit name=press value="OK">
　　</form>
```

```
curl --form upload=@localfilename --form press=OK [URL]
```





# **Referer字段**

> -e

提供一个referer字段，表示你是从哪里跳转过来的。

```sh
curl --referer http://www.example.com
```

# **User Agent字段**

或者 *-A*

这个字段是用来表示客户端的设备信息。服务器有时会根据这个字段，针对不同设备，返回不同格式的网页，比如手机版和桌面版。

```
$ curl --user-agent "[User Agent]" [URL]
```



# **cookie**

使用`--cookie`参数，可以让curl发送cookie。

```
$ curl --cookie "name=xxx" www.example.com
```

至于具体的cookie的值，可以从http response头信息的`Set-Cookie`字段中得到。

`-c cookie-file`可以保存服务器返回的cookie到文件，`-b cookie-file`可以使用这个文件作为cookie信息，进行后续的请求。

```sh
$ curl -c cookies http://example.com
$ curl -b cookies http://example.com
```

# **增加头信息**

> -H

```
curl --header "Content-Type:application/json"
```

# **HTTP认证**

```
curl --user name:password example.com
```



# 上传二进制

上面命令会给 HTTP 请求加上标头`Content-Type: multipart/form-data`，然后将文件`photo.png`作为`file`字段上传。

```
 curl -F 'file=@photo.png' https://google.com/profile
```

`-F`参数可以指定 MIME 类型。

```
$ curl -F 'file=@photo.png;type=image/png' https://google.com/profile
```

**指定文件名**

```
$ curl -F 'file=@photo.png;filename=me.png' https://google.com/profile
```



# 构造URL查询字符串

```
$ curl -G -d 'q=kitties' -d 'count=20' https://google.com/search
```

上面命令会发出一个 GET 请求，实际请求的 URL 为`https://google.com/search?q=kitties&count=20`。如果省略`-G`，会发出一个 POST 请求。

如果数据需要 URL 编码，可以结合`--data--urlencode`参数。

```sh
$ curl -G --data-urlencode 'comment=hello world' https://www.example.com
```



# 跳过 SSL 检测

-k

```sh
$ curl -k https://www.example.com
```

# 限制 HTTP 请求和回应的带宽

**模拟慢网速的环境**

```sh
$ curl --limit-rate 200k https://google.com
```

# 将服务器的回应保存成文件

并将 URL 的最后部分当作文件名。

```sh
curl -o example.html https://www.example.com
```

# 设置服务器认证的用户名和密码

```sh
$ curl -u 'bob:12345' https://google.com/login
```

# 指定 HTTP 请求的代理。

> -x

```sh
$ curl -x socks5://james:cats@myproxy.com:8080 https://www.example.com
```

