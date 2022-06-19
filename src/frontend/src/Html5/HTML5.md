## HTML5的组成

```txt
1.文档声明
    <!DOCTYPE html>
    
2.html头,页面属性<head></head>
    1.标题
        <title></title>
    2.字符编码
        <meta charset="UTF-8">
    3.页面语言
        1.如果整个页面只有一种语言,则使用
        <html lang="en">
        2.如果出现多个语言 则用 <div>包裹

    4.添加样式表
        <link href="style.css" rel="stylesheet" >
        CSS语言是网页中唯一可用的样式语言

    5.添加JavaScript <script src="a.js"></script>
        1.不用写 language=javascript
        2.必须加 </script>

3.html体 <body></body>

4.<html></html> 用html标签包裹上述
```
## H5语法与风格

### 语法

1. 标签名不区分大小写
2. 允许省略关闭空元素 \<br \\> \<br\\>  \<br>
3. 属性值中不包含受限的值可以不使用 "", \<img src=a.jpg>
4. 可以省略属性名 \<input type="checkbox" checked>

### H5风格

1. 包含html,head,body,标签,能将页面内容与页面属性分离
2. 标签全部小写
3. 位属性值加引号

## HTML5的改变

### 新增的元素

| 类别                       | 元素                                                         |
| -------------------------- | ------------------------------------------------------------ |
| 用于构建页面的结构语义元素 | article,aside,figcaption,figure,footer,header,hgroup,nav,section,details,summary |
| 表示文本的语义元素         | mark,time,wbr                                                |
| 表单交互                   | input,datalist,keygen,meter,progress,command,menu,output     |
| 音视频及插件               | audio,video,source,embed                                     |
| canvas(画布)               | canvas                                                       |
| 非英语支持                 | bdo,rp,rt,ruby                                               |

### 删除的元素

1. H5不欢迎 表现型元素的思想,所谓表现型即 仅为网页添加样式的元素(例如:big,center,font,tt,strike)
2. 同样表现型属性 也不推荐使用
3. 不在使用frame框架

