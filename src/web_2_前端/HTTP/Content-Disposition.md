# *Content-Disposition*

## 含义

指示回复的内容 浏览器要以何种形式展示

* 是以**内联**的形式即网页或者页面的一部分）
* 还是以**附件**的形式下载并保存到本地

在 *multipart/form-data*类型的应答消息体中*Content-Disposition*消息头可以被用在multipart消息体的子部分中,用来给出其对应字段的相关信息

## 语法

作为消息主体中的消息头

```
Content-Disposition: inline
Content-Disposition: attachment
Content-Disposition: attachment; filename="filename.jpg"
```

作为multipart body中的消息头

```
Content-Disposition: form-data
Content-Disposition: form-data; name="fieldName"
Content-Disposition: form-data; name="fieldName"; filename="filename.jpg"
```

filename*

"filename" 和 "filename\*" 两个参数的唯一区别在于，"filename\*"采用了 [RFC 5987](https://tools.ietf.org/html/rfc5987) 中规定的编码方式。当"filename" 和 "filename*" 同时出现的时候，应该优先采用"filename*"，假如二者都支持的话。

## 示例

```html
200 OK
Content-Type: text/html; charset=utf-8
Content-Disposition: attachment; filename="cool.html"
Content-Length: 22

<HTML>Save me!</HTML>
```

```html
POST /test.html HTTP/1.1
Host: example.org
Content-Type: multipart/form-data;boundary="boundary"

--boundary
Content-Disposition: form-data; name="field1"

value1
--boundary
Content-Disposition: form-data; name="field2"; filename="example.txt"

value2
--boundary--
```

## 规范

| Specification                                   | Title                                                        |
| :---------------------------------------------- | :----------------------------------------------------------- |
| [RFC 7578](https://tools.ietf.org/html/rfc7578) | Returning Values from Forms: multipart/form-data             |
| [RFC 6266](https://tools.ietf.org/html/rfc6266) | Use of the Content-Disposition Header Field in the Hypertext Transfer Protocol (HTTP) |
| [RFC 2183](https://tools.ietf.org/html/rfc2183) | Communicating Presentation Information in Internet Messages: The Content-Disposition Header Field |