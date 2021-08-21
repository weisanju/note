# HTML5表单

## 新增元素与属性

#### 新增表单相关属性

| 属性名       | 作用的元素            | 取值           | 作用                               |
| ------------ | --------------------- | -------------- | ---------------------------------- |
| form         | 一般作用于input元素上 | 取form表单ID   | 可以把form表单的元素不写在form内部 |
| formaction   | input type=submit     | URL            | 使得提交到不同URL                  |
| formmethod   | input type=submit     | get,post       | 以不同方法提交                     |
| placeholder  | input type=text,area  | string         | 提示文字                           |
| autofocus    | 文本框,选择框,按钮    | boolean        | 画面打开时自动获得焦点             |
| list         | input type=text       | datalist标签ID | 可供选择条目                       |
| autocomplete | input type=text       | on off         | 自动填充                           |
|              |                       |                |                                    |

#### 新增input元素类型

| 新增input元素类型           | 说明                      |
| --------------------------- | ------------------------- |
| email                       | 邮箱地址输入              |
| url                         | url输入                   |
| number                      | 数值类型输入              |
| range                       | 范围类型                  |
| 日期选择类型:date           | 选择年月日                |
| 日期选择类型:month          | 选择月年                  |
| 日期选择类型:week           | 选择周,年                 |
| 日期选择类型:time           | 选取时间,小时分钟         |
| 日期选择类型:datetime       | 选取日月年,时间(UTC时间)  |
| 日期选择类型:datetime-local | 选取日月年,时间(本地时间) |
| color                       | 颜色选择                  |
| search                      | 搜索框                    |

#### output元素

1. \<output onforminput="value=range1.value"></output>

## 表单验证

#### 自动验证属性

| 属性名   | 作用              | 举例                                      |
| -------- | ----------------- | ----------------------------------------- |
| required | 必输项            | <input type=text required>                |
| pattern  | 满足正则          | <input type=text pattern="[0-9][A-Z]{3}"> |
| min,max  | 限制最小值,最大值 | <input type=number min=1 max=100>         |
| step     | 步进,值位5的倍数  | <input type=text step=5 >                 |

#### 显示验证,取消验证,自定义错误

1. 手动验证方法:checkValidity()
2. 取消验证:<input type=submit formnovalidate>
3. 自定义错误:setCustomValidity

## 增强页面元素

| 元素名            | 作用                                                         | 例子                                                         |
| ----------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| figure,figcaption | figuration:独立的内容,不是很相关,可以是图片;figcaption:从属于figure,为它的标题 | none                                                         |
| details,summary   | 详细展开                                                     | <details><summary>这是总结</summary><p>HelloWorld</p></details> |
| progress          | 进度条                                                       | none                                                         |
| meter             | 范围内的数量值,value,min,max,low,high,optimum                | none                                                         |
| 改良的ol          | 增加start,reversed属性                                       | none                                                         |
| 改良的dl          | 配合dt,dd进行术语定义,名词解释                               | none                                                         |
| cite              | 用于书名,电影名等                                            | none                                                         |
| small             | 小字印刷体                                                   | none                                                         |



## 文件API

1. fileList与file对象

```html
<html>
    <head>
    </head>
    <script>
        function showfilename(){
            var file;
            for(var i=0;i<document.getElementById('file').files.length;i++){
                file= document.getElementById('file').files[i];
                alert(file.name);
            }
        }
    </script>
    <body>
        <input type="file" multiple id=file size=80>
        <input type="button" onclick="showfilename()" value="文件上传">
    </body>
</html>

```

2. blob对象的属性与方法
   1. size
   2. type:以 image/jgp 样式
3. accept

```html
        <input type="file" multiple id=file size=80 accept="image/*">
        <input type="button" onclick="showfilename()" value="文件上传" >
```

4. filereader对象

| 方法名             | 参数描述           |
| ------------------ | ------------------ |
| readAsBinaryString | file,读二进制      |
| readAsText         | file,encoding:文本 |
| readAsDataURL      | file:读作DataURL   |
| abort              | none,中断读取操作  |

| 事件        | 描述                    |
| ----------- | ----------------------- |
| obabort     | 中断                    |
| onerror     | 出错                    |
| onloadstart | 开始读取                |
| onprogress  | 正在读取                |
| onload      | 读取成功完成            |
| onloadend   | 读取完成,无论成功或失败 |

基本使用

```html
<html>
    <head>
    </head>
    <script>
      if(typeof FileReader == 'undefined'){
          result.innerHTML ="<p>浏览器不支持fileReader</p>"
      }
      function readAsDataURL(){
          var file = document.getElementById('file').files[0];

          if(!/image\/\w+/.test(file.type)){
              alert('图像文件格式不对');
              return false;
          }
          var reader = new FileReader();
          reader.readAsDataURL(file);
          reader.onload= function(e){
            var result = document.getElementById('result');
            result.innerHTML='<img src="'+this.result+'"/>'
          }
      }
      function readAsBinaryString(){
        var file = document.getElementById('file').files[0];
        var reader = new FileReader();
        reader.readAsBinaryString(file);
        reader.onload = function(f) {
            var result = document.getElementById('result');
            result.innerHTML=this.result
        }
      }
      function readAsText(){
        var file = document.getElementById('file').files[0];
        var reader = new FileReader();
        reader.readAsText(file);
        reader.onload=function (f) {
            var result = document.getElementById('result');
            result.innerHTML=this.result
        }
      }
    </script>
    <body>
        <p>
            <label>请选择一个文件</label>
            <input type="file"  id="file">
            <input type="button" value="读取图像" onclick="readAsDataURL()">
            <input type="button" value="读取二进制数据" onclick="readAsBinaryString()">
            <input type="button" value="读取文本" onclick="readAsText()">
        </p>
        
        <output id=result >

        </output>
    </body>
</html>
```

## 拖放API

### 实现拖放的步骤

1. 设置draggable 属性(img a标签默认可拖放)
2. 编写拖放事件代码

| 事件      | 产生事件的元素           | 描述                                 |
| --------- | ------------------------ | ------------------------------------ |
| dragstart | 被拖放的元素             | 开始拖放操作                         |
| drag      | 被拖放的元素             | 拖放过程中                           |
| dragenter | 拖放过程中鼠标经过的元素 | 被拖放的元素开始进入本元素的范围内   |
| drgaover  | 拖放过程中鼠标经过的元素 | 被拖放的元素开始在本元素的范围内移动 |
| dragleave | 拖放过程中鼠标经过的元素 | 被拖放的元素开始离开本元素的范围     |
| drop      | 拖放的目标元素           | 有其他元素被拖放到了本元素中         |
| dragend   | 拖放的对象元素           | 拖放操作结束                         |

```html
<html>
<head>
</head>
<script>
    function init(){
        var source = document.getElementById('dragme');
        var dest = document.getElementById('text');
        source.addEventListener("dragstart",function (ev) {
            var dt = ev.dataTransfer;
            dt.effectAllowed = 'move';
            dt.setData("text/plain",'你好');

        },false);

        dest.addEventListener('dragend',function (ev) {
            ev.preventDefault();
        },false);

        dest.addEventListener('drop',function (ev) {
            var dt = ev.dataTransfer;
            var text = dt.getData("text/plain");
            dest.innerHTML='<img src=222.jpg>'
            ev.preventDefault();
            ev.stopPropagation();
        },false);
    }
   document.ondragover = function(e){e.preventDefault();}
   document.ondrop = function(e){e.preventDefault();}
</script>

<body onload="init()">
    <div id="dragme" draggable="true" style="width:200px;border :1px solid gray;">
        请拖放
    </div>
    <div id="text" style="width: 200px; height: 200px; border: 1px solid gray;"></div>
</body>
</html>
```

### DataTransfer对象的属性与方法

| 属性/方法                                  | 描述                                             |
| ------------------------------------------ | ------------------------------------------------ |
| dropeffect属性                             | 拖放操作的视觉效果,指定的值在effectAllowed范围内 |
| effectAllowed属性                          | 指定元素被拖放时所允许的效果                     |
| types属性                                  | 存入数据的种类,伪数组                            |
| clearData(DOMString format)                | 清楚DataTransfer对象存放的数据,省略参数清除全部  |
| setData(DOMString format,DOMSttring data)  | 向DataTransfer对象中存数据                       |
| getData(DOMString firmat)                  | 从DT取数据                                       |
| setDragImage(Element image,long x,long  y) | 用img元素来设置拖放图标                          |

### 自定义拖放ICON

```
<html>
<head>
</head>
<script>
    var dragIcon = document.createElement('img');
    dragIcon.src = '222.jpg';

    function init(){
        var source = document.getElementById('dragme');
        var dest = document.getElementById('text');

        source.addEventListener("dragstart",function (ev) {
            var dt = ev.dataTransfer;
            dt.effectAllowed = 'all';
            dt.dropEffect = 'copyMove';
            dt.setDragImage(dragIcon,120,150);
            dt.setData("text/plain",'你好');

        },false);

        dest.addEventListener('dragend',function (ev) {
            ev.preventDefault();
        },false);

        dest.addEventListener('drop',function (ev) {
            var dt = ev.dataTransfer;
            var text = dt.getData("text/plain");
            dest.innerHTML='<img src=222.jpg>'
            ev.preventDefault();
            ev.stopPropagation();
        },false);
    }
   document.ondragover = function(e){e.preventDefault();}
   document.ondrop = function(e){e.preventDefault();}
</script>

<body onload="init()">
    <div id="dragme" draggable="true" style="width:200px;border :1px solid gray;">
        请拖放
    </div>
    <div id="text" style="width: 200px; height: 200px; border: 1px solid gray;"></div>
</body>
</html>
```

