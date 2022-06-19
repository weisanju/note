# `CSS3`

## `CSS3的模块化`

### `详细模块表格`

| `模块名称`                          | `功能描述`                                      |
| ----------------------------------- | ----------------------------------------------- |
| `basic box model`                   | `盒相关样式`                                    |
| `line`                              | `直线相关样式`                                  |
| `lists`                             | `列表相关样式`                                  |
| `hyperlinkPresentation`             | `超链接,锚的显示方式,视觉效果`                  |
| `presentationLevel`                 | `元素不同的样式级别`                            |
| `speech`                            | `语言相关样式,音量,音速,间歇时间`               |
| `background and border`             | `背景和边框`                                    |
| `text`                              | `文字`                                          |
| `color`                             | `颜色`                                          |
| `font`                              | `字体`                                          |
| `pagedMedia`                        | `页眉页脚页数`                                  |
| `cascadingand inheritance`          | `属性赋值`                                      |
| `value units`                       | `值与单位`                                      |
| `imageValue`                        | `imgae元素赋值方式`                             |
| `2DTransforms`                      | `2维空间变形`                                   |
| `3DTransforms`                      | `3维空间变形`                                   |
| `transitions`                       | `平滑过渡视觉效果`                              |
| `animations`                        | `动画`                                          |
| `cssOM view`                        | `查看管理页面或页面视觉效果,处理元素的位置信息` |
| `syntax`                            | `CSS样式表的基本结构,样式表语法细节`            |
| `generated and replaced content`    | `怎么插入内容`                                  |
| `marquee`                           | `怎样显示溢出部分`                              |
| `ruby`                              | `定义ruby元素(用于显示拼音文字)`                |
| `writing modes`                     | `文本数据的布局方式`                            |
| `basic user interface`              | `定义在屏幕 纸张上进行输出时页面的渲染方式`     |
| `namespaces`                        | `使用命名空间的语法`                            |
| `media queries`                     | `根据媒体类型来 实现不同样式`                   |
| `'reader' media type`               | `屏幕阅读器之类的阅读程序 时的样式`             |
| `multi-column layout`               | `多栏布局方式`                                  |
| `template layout`                   | `特殊布局方式`                                  |
| `flexiblebox layout`                | `自适应浏览器窗口的流动布局`                    |
| `grid position`                     | `网格布局`                                      |
| `generated content for paged media` | `印刷时使用的布局方式`                          |

## `选择器`

### `属性选择器`

1. `[attr=val]{...} :普通属性选择器`
2. `[attr*=val]:包含:属性名包含val值的属性`
3. `[attr^=val]:开头:以val开头的属性名`
4. `[attr$=val]:结尾:以val结尾的属性名`

### `结构型伪类选择器`

1. `a标签上的伪类选择器`
   1. `a:link{...}`
   2. `a:visited`
   3. `a:hover`
   4. `a:active`

2. `伪元素选择器:针对CSS中已经定义好的元素选择器`

   1. `语法: 选择器 类名:伪元素{属性:值}`

   2. `伪元素`

      1. `first-line :选定的某个元素的 内容的第一行`
      2. `first-letter:选定的某个元素的 内容的第一个字`
      3. `before:选定的某个元素的 内容的前面插入一些内容,配合content使用`
      4. `after:选定的某个元素的 内容的后面插入一些内容,配合content使用`

   3. `content插入的元素`

      1. `插入文本 content = ""`

      2. `插入图像 content:url(imgurl)`

      3. `插入项目编号`

         1. `content:counter(计数器名[,编号器样式list-style-type])`

         2. `list-style-type`

            1. `upper-alpha`
            2. `upper-roman`

         3. `example`

            ```html
            <!DOCTYPE html>
            <html>
            	<head>
            		<meta charset="UTF-8">
            		<title></title>
            		<style type="text/css">
            			h1:before{
            				content: '第'counter(mycounter,upper-roman)'章节';
            				color: blue;
            				font-size: 42px;
            				
            			}
            			h1{
            				counter-increment: mycounter;
            			}
            		</style>
            	</head>
            	<body>
            		<h1>标题</h1>
            		<p>文字</p>
            		<h1>标题</h1>
            		<p>文字</p>
            		<h1>标题</h1>
            		<p>文字</p>
            	</body>
            </html>
            ```

         4. `编号嵌套`

            ```html
            <!DOCTYPE html>
            <html>
            	<head>
            		<meta charset="UTF-8">
            		<title></title>
            		<style type="text/css">
            			h1:before{
            				content: counter(counter1) '. ';
            			}
            			h1{
            				counter-increment: counter1;
            			}
            			h2:before{
            				content: counter(counter1)'-'counter(counter2) '. ';
            			}
            			h2{
            				counter-increment: counter2;
            				margin-left: 40px;
            			}
            		</style>
            	</head>
            	<body>
            		<h1>大标题</h1>
            		<h2>小标题</h2>
            		<h2>小标题</h2>
            		<h2>小标题</h2>
            		<h1>大标题</h1>
            		<h2>小标题</h2>
            		<h2>小标题</h2>
            		<h2>小标题</h2>
            		<h1>大标题</h1>
            		<h2>小标题</h2>
            		<h2>小标题</h2>
            		<h2>小标题</h2>
            	</body>
            </html>
            
            ```

         5. `插入编号`

            ```html
            <!DOCTYPE html>
            <html>
            	<head>
            		<meta charset="UTF-8">
            		<title></title>
            		<style type="text/css">
            			/*选择元素 插入quotes*/
            			h1:before{
            				content: open-quote;
            			}
            			h1:after{
            				content: close-quote;
            			}
            			/*定义quotes*/
            			h1{
            				quotes: "<<" ">>";
            			}
            		</style>
            	</head>
            	<body>
            		<h1>标题</h1>
            	</body>
            </html>
            
            ```

            

3. `结构型伪类选择器`

1. `root根选择器:设定 html整个样式`

   `:root{background-color:yellow}`

2. `not选择器:排除某个元素`

   `body *:not(h1){ background-color:yellow}`

3. `empty:单内容为空白时,指定的元素`

   `:empty{background-color:yellow}`

4. `target:对于具有超链接的a标签,只有在跳转之后,才会呈现样式`

   ```html
   <!DOCTYPE html>
   <html>
   	<head>
   		<meta charset="UTF-8">
   		<title></title>
   		<style type="text/css">
   			:target {
   				background-color: yellow;
   			}
   		</style>
   	</head>
   	<body>
   		<a href="#text1">示例1</a>
   		<a href="#text2">示例2</a>
   
   		<div id="text1">
   			<h2>示例文本</h2>
   			<p>此处省去</p>
   		</div>
   		<div id="text2">
   			<h2>示例文本</h2>
   			<p>此处省去</p>
   		</div>
   	</body>
   </html>
   ```

   

5. `子元素选择器`
   1. `first-child`
   2. `last-child`
   3. `nth-child(n), n=odd,基数,even偶数`
   4. `nth-last-child(n)`

6. `类型选择器:在nth-child选择时,不同元素类型也会被计数`

   1. `nth-of-type(n), nth-last-of-type(n)`

   2. `在计算子元素类型的时候就 只计算同类元素`

   3. `循环使用样式`

      `li:nth-child(4n+1) 4表示样式的种类,1表示循环的位置`

7. `only-child,only-of-type`

   1. `只针对只有一个子元素生效`

### `UI元素状态伪类选择器`

1. `E:hover`
2. `E:avtive`
3. `E:focus`
4. `E:enabled`
5. `E:disabled`
6. `E:read-only`
7. `E:read-write`
8. `E:checked`
9. `E:default`
10. `E:indeterminate`
11. `E::selection`

### `通用兄弟元素选择器`

1. `语法`

   ```css
       span~p{
           background: green;
       }
   ```

2. `解析`

   `寻找span标签后,所有的的兄弟标签 ,且该标签为 p标签`

## `文字与字体相关样式`

### `文字添加阴影`

1. `text-shadow: xlength ylength zlength color[,xlength ylength zlength color]`
2. `解释: 阴影离开横向方向,纵向方向,模糊半径,阴影颜色`
3. `可以指定多组`

### `文本自动换行`

1. `word-break:wordbreak-style;`
   1. `normal:使用浏览器默认规则`
   2. `keep-all:半角空格或连字符处换行`
   3. `break-all:单词内换行`

### `长单词与URL地址自动换行`

1. `word-wrap:break-word`

### `使用服务端字体`

1. `使用服务端字体的语法`

   ```css
   //声明使用web字体
   @font-face{
       font-family:webFont;
   	src:url('font/Fontin_Sans_R_45b.otf') format("opentype")
       font -weight:normal
   }
   //使用web字体
   h1{
       font-family:webFont
   }
   ```

2. `使用本地字体`

   ```css
   @font-face{
       font-famioly:Arial;
       src:local('Arial')
   }
   ```

3. `@font-face可以指定的属性`

   | `属性值`       | `说明`             | `取值`                                                       |
   | -------------- | ------------------ | ------------------------------------------------------------ |
   | `font-family`  | `设置字体系列名称` |                                                              |
   | `font-style`   | `字体样式`         | `normal:不使用斜体<br />italic:斜体<br />oblique:倾斜体<br />inherit:从父元素继承` |
   | `font-variant` | `设置字体大小`     | `normal:浏览器默认值<br />small-caps:小型大小字母<br />inherit:父元素继承` |
   | `font-weight`  | `设置字体的粗细`   | `normal:浏览器默认值<br />bold:使用粗体字符<br />bolder:更粗<br />lighter:更细字符<br />100-900:从细到粗,必须为100的整数倍` |
   | `font-stretch` | `字体是否变形伸缩` | `normal<br />wider<br />narrower<br />ultra-condensed<br />extra-condensed<br />condensed<br />semi-condensed<br />semi-expanded<br />extra-expanded<br />ultra-expanded:最宽的` |
   | `font-size`    | `字体大小`         |                                                              |
   | `src`          | `字体文件路径`     |                                                              |

   

### `修改字体种类而保持字体大小尺寸不变`

1. `改变字体的种类很可能会因为文字大小的变化而导致原来的页面布局产生混乱`

2. `font-size-adjust:0.49(aspect值)`

   `font-size-adjust 的大小 = x-length / 字体像素大小`

3. `c = (a/b) * s , a是 新字体的 aspect值, b是旧字体的aspect得值,s是指定的尺寸`

## `盒相关样式`

### `盒的类型`

1. `盒的基本类型`

   1. `inline: a,span,每行可容纳多个标签`

   2. `block`

   3. `inline-block`

      | `类型`         | `说明`                                                       | `典型标签`     |
      | -------------- | ------------------------------------------------------------ | -------------- |
      | `inline`       | `1.inline元素的宽度始终等于其内容的宽度<br />指定宽度的样式对inline元素无效<br />2. 可以允许多个inline元素` | `a,span,input` |
      | `block`        | `1. block元素的宽度 充满整行<br />2.高度 = getOrdefault(指定宽度,内容宽度)` | `div,p`        |
      | `inline-block` | `1.该元素的宽度 =  getOrdefault(指定宽度,内容宽度)`          |                |

2. `inline-table类型 :针对表格使用的inline`

3. `list-item:列表显示`

   ```css
   		div{
   			display: list-item;
   			list-style-type: circle;
   			margin-left: 30px;
   		}
   ```

4. `表格相关类型`

   | `元素`     | `所属类型`           | `说明`         |
   | ---------- | -------------------- | -------------- |
   | `table`    | `inline-table`       | `整个表格`     |
   | `tr`       | `table-row`          | `表格中的一行` |
   | `td`       | `table-cell`         | `单元格`       |
   | `th`       | `table-header`       | `列标题`       |
   | `tbody`    | `table-row-group`    | `表格所有行`   |
   | `thead`    | `table-header-group` | `表格表头部分` |
   | `tfoot`    | `table-footer-group` | `脚注部分`     |
   | `col`      | `table-column`       | `一列`         |
   | `colgroup` | `table-column-group` | `所有列`       |
   | `caption`  | `table-caption`      | `表格标题`     |

5. `none类型 , 隐藏元素`

### `盒中容纳不下的内容显示`

1. `指定 overflow属性指定盒容纳不下的内容的处理方式\`
2. `overflow,overflow-x,overflow-y,text-overflow`
3. `取值范围`
   1. `scroll 滚动条`
   2. `visible:超出容纳范围文字原样显示`
   3. `hidden:隐藏`

### `盒阴影`

1. `box-shadow:lengthx legnthy lengthz color`

   `阴影离开文字很想,纵向,阴影模糊半径,颜色`

2. `将参数设定为0:将绘制不向外模糊的阴影`

3. `对盒内子元素使用阴影`

4. `对第一个文字或者第一行使用阴影 div:first-letter`

5. `对表格单元格使用阴影`

### `指定 针对元素宽度与高度的 计算方法`

1. `使用box-sizing属性可选值`
   1. `content-box:用width,height指定的宽度与高度 不包括 内部留白(padding),边框的厚度`
   2. `border-box:用width,height指定的宽度与高度包括 内部留白(padding),边框的厚度` 

## `与背景边框相关样式`

### `与背景相关的 新增属性`

1. `backgroup-clip:背景默认显示的范围包括边框与内部留白`

   1. `border: 包括边框区域(如果边框为虚线的话)`

   2. `padding:不包括边框区域`

   3. `example`

      ```html
      <!DOCTYPE html>
      <html>
      	<head>
      		<meta charset="UTF-8">
      		<title></title>
      		<style type="text/css">
      			div{
      				background-color: red;
      				border: dashed 15px ;
      				/*padding: 30px;
      				color: white;
      				font-size: 30px;
      				font-weight: bold;*/
      			}
      			div#a1{
      				background-clip: border-box;
      			}
      			div#a2{
      				background-clip: padding-box;
      			}
      		</style>
      	</head>
      	<body>
      		<div id="a1">示例文字1</div>
      		<div id="a2">示例文字2</div>
      	</body>
      </html>
      
      ```

      

2. `background-origin:图像的绘制起点`

   1. `默认是从内部留白区域的左上角开始`

   2. `显示级别: 背景颜色 <  背景图片 < 边框,内容`

   3. `可选值为`

      1. `border:边框左上角开始,(边框会把背景覆盖)`
      2. `padding:内部留白左上角`
      3. `content:内容左上角`

   4. `example`

      ```html
      <!DOCTYPE html>
      <html>
      	<head>
      		<meta charset="UTF-8">
      		<title></title>
      		<style type="text/css">
      			div{
      				background-color: black;
      				background-image: url(img/下载.jpg);
      				background-repeat: no-repeat;
      				border: dashed 15px green;
      				padding: 30px;
      				color: white;
      				font-size:  2em;
      				font-weight: bold;
      			}
      			
      			div#div1{
      				background-origin: border-box;
      			}
      			div#div2{
      				background-origin: content-box;
      			}
      			div#div3{
      				background-origin: padding-box;
      			}
      		</style>
      	</head>
      	<body>
      		<div id="div1">示例文字1</div>
      		<div id="div2">示例文字2</div>
      		<div id="div3">示例文字3</div>
      	</body>
      </html>
      
      ```

3. `background-size:指定背景图像的大小`

   1. `background-size width height  or background-size auto(auto维持 纵横比)`

   2. `example`

      ```html
      <!DOCTYPE html>
      <html>
      	<head>
      		<meta charset="UTF-8">
      		<title></title>
      		<style type="text/css">
      			div{
      				background-color: black;
      				background-image: url(img/下载.jpg);
      				padding: 30px;
      				color:white;
      				font-size :2em;
      				font-weight: bold;
      				/*background-size: 40px 20px;*/
      				background-size: auto;
      			}
      		</style>
      	</head>
      	<body>
      			<div>示例文字</div>
      	</body>
      </html>
      
      ```

4. `background-break:指定内联元素背景图像进行平铺时的循环方式`

   1. `bounding-box:在整个内联元素中平铺`
   2. `each-box:在每一行中平铺`
   3. `continuous:下一行紧接着上一行图像继续平铺`

### `在一个元素中显示多个背景图像`

1. `通过 background-image:url(1),url(2)`
2. `通过 background-repeat: repeat-y,repeat-x;指定每个图片的平铺方式`
3. `最先指定的图片优先级最高`
4. `允许多重指定配合使用的属性有`
   1. `background-image`
   2. `background-repeat`
   3. `background-position`
   4. `background-clip`
   5. `background-origin`
   6. `background-size`

### `圆角边框的绘制`

1. `使用 border-radius:x y`
2. `指定两个半径有两种处理方式`
   1. `Firefox:第一个半径为边框左上角,右下角 的圆半径`
   2. `chrome:第一个半径为椭圆的 水平半径, 第二个为 垂直半径`
3. `可以 矩形四个角半径各不相同的边框`
   1. `border-top-left-radius`

### `使用图像边框`

1. `border-image url(path) a b c d`
   1. `ABCD四个参数表示 当浏览器 自动把边框使用到的图像进行分成九分`
   2. `A上 B右 C下 D左`
2. `使用border-image指定边框宽度`
   1. `border-image url(path) a b c d/border-width`
3. `图像显示方式`
   1. `border-image url(path) a b c d/border-width topbottom-type leftright-type`
   2. `type`
      1. `stretch:拉伸`
      2. `round`:平铺
         1. 与重复类似,区别在于
         2. 最后一张图如果 不能铺满一半,则将上张图拉伸处理,
         3. 如果能铺满一半,则将该图拉伸处理
      3. `repeat`:重复

