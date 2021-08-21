# 语法定义

## 整体语法

```shell
Method Request-URI HTTP-Version
Header-field: Header-value

Request-Body
```

## 注释

```
// or #
// A basic request
GET http://example.com/a/
```

## 短格式

```
// A basic request
http://example.com/a/
```



* ctrl + space 可以有提示

* `### 标识 请求结束`

  ```
  // A basic request
  http://example.com/a/
  
  ###
  
  // A second request using the GET method
  http://example.com:8080/api/html/get?id=123&value=content
  
  ```

## 请求分行

```
// Using line breaks with indent
GET http://example.com:8080
    /api
    /html
    /get
    ?id=123
    &value=content
```



## webservice认证

```shell
// Basic authentication
GET http://example.com
Authorization: Basic username password

###

// Digest authentication
GET http://example.com
Authorization: Digest username password
```

## 请求体

与请求URL,请求头 隔一行

```shell
// The request body is provided in place
POST http://example.com:8080/api/html/post HTTP/1.1
Content-Type: application/json Cookie: key=first-value

{ "key" : "value", "list": [1, 2, 3] }
```

如果指定了Content-Type,会自动进行语言帮助

## 从文件中加载请求体

```shell
// The request body is read from a file
POST http://example.com:8080/api/html/post
Content-Type: application/json

< ./input.json
```

## 使用 *multipart/form-data*

```shell
POST http://example.com/api/upload HTTP/1.1
Content-Type: multipart/form-data; boundary=boundary

--boundary
Content-Disposition: form-data; name="first"; filename="input.txt"

// The 'input.txt' file will be uploaded
< ./input.txt

--boundary
Content-Disposition: form-data; name="second"; filename="input-second.txt"

// A temporary 'input-second.txt' file with the 'Text' content will be created and uploaded
Text
--boundary
Content-Disposition: form-data; name="third";

// The 'input.txt' file contents will be sent as plain text.
< ./input.txt --boundary--
```

## 是否跟随重定向

```
// @no-redirect
example.com/status/301
```

## 是否记录请求记录

```
// @no-log
GET example.com/api
```

### Enable or disable saving received cookies to the cookies jar

```
// @no-cookie-jar
GET example.com/api
```

# 使用变量

请求的主机,端口,路径,查询参数,请求头或者请求体,或者外部文件 都可以使用变量

## 变量使用

```
双花括号
{{variable}}
```

## 变量来源

*  [environment variables](https://www.jetbrains.com/help/idea/exploring-http-syntax.html#environment-variables)

* [dynamic variables](https://www.jetbrains.com/help/idea/exploring-http-syntax.html#dynamic-variables)动态变量

  ```
  $uuid, $timestamp, and $randomInt
  ```

* 编程方式定义: [response handler scripts](https://www.jetbrains.com/help/idea/http-client-in-product-code-editor.html#using-response-handler-scripts) 

  ```
  client.global.set
  ```



## 环境变量

点击新增环境变量, 包括 公共变量,私有变量

```json
{
    "development": {
        "host": "localhost",
        "id-value": 12345,
        "username": "",
        "password": "",
        "my-var": "my-dev-value"
    },

    "production": {
        "host": "example.com",
        "id-value": 6789,
        "username": "",
        "password": "",
        "my-var": "my-prod-value"
    }
}
```