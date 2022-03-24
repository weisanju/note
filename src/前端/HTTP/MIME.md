# MIME概述

>  *Multipurpose Internet Mail Extensions or MIME type*

* 多用途网络邮件扩展

* *IANA*(*InternetAssigned NumbersAuthority*) 组织 负责 所有官方的MIME类型制定

* [所有MIME列表](https://www.iana.org/assignments/media-types/media-types.xhtml)

* 浏览器 根据 MIME类型 处理url, 而不是文件的扩展名



# MIME结构

*type/subtype;parameter=value*

* *type*表示 一般类别

* *subtype*表示 具体类别

* 参数 常见的有

  *charset=UTF-8* 如果不指定这个,则默认采用  *[ASCII](https://developer.mozilla.org/en-US/docs/Glossary/ASCII) (`US-ASCII`)*编码

* 所有参数 大小写敏感

# Types

* 单部件类别(*discrete*)

  表示一个文件,或者一个媒体

* 多部件类别(*multipart*)

  表示一个文档 有多个部件组成,每个部件有自己的MIME类别,例如邮件中就包含文本,HTML,附件等多种类别

## discrete

*application*

通用的二进制, 一般有  `application/octet-stream`. `application/pdf`, `application/pkcs8`, and `application/zip`.

*audio*

音频数据, `audio/mpeg,audio/vorbis`

*example*

保留, 用作占位符,待确定 例如 *audio/example* *example/xxxx*

*font*

字体,*font/woff, font/ttf,font/otf*

*image*

图像,image/jpeg , image/png , image/svg+xml

*model*

3d模型数据,  *model/3mf` and `model/vml*

*text*

文本, `text/plain`, `text/csv`, and `text/html`.

*video*

视频 *video/mp4*



**注意** 对于文本文档,不清楚类型 要使用 *text/plain* 如果是二进制文件不清楚.要使用*application/octet-stream*

## Multipart Types

> 在邮件这种组合文档中常用,有一个例外,  `multipart/form-data , multipart/byteranges` ,使用 206发送文档的一部分, `Partial Content`

`message`  [List at IANA](https://www.iana.org/assignments/media-types/media-types.xhtml#message)

* 可以表示 转发另一邮件的 内容 `message/rfc822`
* 或者分离大消息文本为几块 `message/partial` ,接收者会自动组装

`multipart`[List at IANA](https://www.iana.org/assignments/media-types/media-types.xhtml#multipart)

数据由多个不通的 mimeTpyes的部分组成

Examples include `multipart/form-data` (for data produced using the [`FormData`](https://developer.mozilla.org/en-US/docs/Web/API/FormData) API) and `multipart/byteranges` (defined in [RFC 7233: 5.4.1](https://tools.ietf.org/html/rfc7233) and used with [HTTP](https://developer.mozilla.org/en-US/docs/Glossary/HTTP)'s [`206`](https://developer.mozilla.org/en-US/docs/Web/HTTP/Status/206) "Partial Content" response returned when the fetched data is only part of the content, such as is delivered using the [`Range`](https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Range) header).



# Web开发中重要的类型

*application/octet-stream*

未知的二进制格式, 浏览器不会执行它, 只会提示下载框

*text/plain*

文本类型

*text/css , text/html, text/javascript*

*application/xml or application/xhtml+xml*

*multipart/form-data*

```
Content-Type: multipart/form-data; boundary=aBoundaryString
(other headers associated with the multipart document as a whole)

--aBoundaryString
Content-Disposition: form-data; name="myFile"; filename="img.jpg"
Content-Type: image/jpeg

(data)
--aBoundaryString
Content-Disposition: form-data; name="myField"

(data)
--aBoundaryString
(more subparts)
--aBoundaryString--
```



# MIME sniffing

**mime 嗅探**

In the absence of a MIME type, or in certain cases where browsers believe they are incorrect, browsers may perform *MIME sniffing* — guessing the correct MIME type by looking at the bytes of the resource.

Each browser performs MIME sniffing differently and under different circumstances. (For example, Safari will look at the file extension in the URL if the sent MIME type is unsuitable.) There are security concerns as some MIME types represent executable content. Servers can prevent MIME sniffing by sending the [`X-Content-Type-Options`](https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/X-Content-Type-Options) header.



[参考](https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types)

















