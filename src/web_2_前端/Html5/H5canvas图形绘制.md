# H5canvas图形绘制

## 基本使用

1. <canvas id="tutorial" width="300" height="300"></canvas>

2. 检测支持性: <canvas>    你的浏览器不支持canvas,请升级你的浏览器 </canvas>

   1. 支持的浏览器会只渲染标签，而忽略其中的替代内容。不支持 的浏览器则 会直接渲染替代内容。

3. 渲染上下文

   ```html
   if (canvas.getContext){
     var ctx = canvas.getContext('2d');
     // drawing code here
   } else {
     // canvas-unsupported code here
   }
   ```

   

## 绘图示例

```html
<html>

<head>
</head>
<script>
    function init() {
        var canvas = document.getElementById('canvas');
        if(canvas == null){
            return false
        }
        var context = canvas.getContext('2d');
        context.fillStyle = 'red';
        context.fillRect(0,0,50,50);
        context.strokeStyle='green';
        context.lineWidth= 1;
        context.strokeRect(50,50,400,300);
    }
</script>
<body onload="init()">
    <canvas id="canvas" width="400" height="300" />
</body>
</html>		
```



## 路径绘制步骤

1. 获取canvas对象 

   1.  document.getElementById('canvas');

2. 得到 上下文

   ```
   canvas.getContext('2d');
   ```

3. 填充与绘制边框

   1. 开始绘制 

      ```
      context.beginPath();	
      ```

   2. 以特定方式绘制形状 :以弧形为例

      ```
      context.arc(i*25,i*25,i*10,0,Math.PI*2,true);
      ```

   3. 关闭路径

      ```
      context.closePath() 路径闭合后会自动将终点与起点相连
      ```

   4. 开始绘制

      ```
      context.fill() 填充
      context.stroke() 绘制边框
      ```

   5. ```html
      <html>
      
      <head>
      </head>
      <script>
          function init() {
              var canvas = document.getElementById('canvas');
              if(canvas == null){
                  return false
              }
              var context = canvas.getContext('2d');
              draw_circle(context);
          }
          function draw_circle(c){
              for(var i=0;i<10;i++){
                  c.beginPath();
                  c.arc(i*25,i*25,i*10,0,Math.PI*2,true);
                  c.closePath();
                  c.fillStyle='rgba(255,0,0,0.25)';
                  c.fill();
              }
          }
      </script>
      <body onload="init()">
          <canvas id="canvas" width="400" height="300" />
      </body>
      </html>
      ```

      

## 绘制图形方式

| 函数名                                            | 说明                                                         | 参数解析                                                     |
| ------------------------------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| arc(x,y,radius,startAngle,endAngle,anticlockwise) | 绘制弧形                                                     | x,y圆心,start,end,开始角度结束角度,是否顺时针                |
| arcTo(x1,y1,x2,y2,radius)                         | 根据给定的控制点和半径画一段圆弧，最后再以直线连接两个控制点 | x1,y1为控制点1,x2,y2为控制点2,radius为半径                   |
| moveTo(x,y),lineTo(x,y)                           | 绘制直线                                                     | move:x,y起点,line:x,y终点                                    |
| bezierCurveTo(cplx,cply,cp2x,cp2y,x,y)            | 贝济埃曲线                                                   | cp1x , y 第一个控制点横坐标;cp2x,y第二个控制点横坐标;x,y终点坐标 |
| quadraticCurveTo(cp1x, cp1y, x, y)                | 二次贝塞尔曲线                                               | 控制点坐标,结束点坐标                                        |
| bezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y)       | 三次贝塞尔曲线                                               | 控制点坐标,控制点坐标,结束点坐标                             |

## 添加样式和颜色

1. fillStyle 填充颜色, strokeStyle  边框颜色

2. 透明度

   1. globalAlpha = transparencyValue 全局设置图形透明度

3. line style 线条样式

   1. 线宽:`lineWidth` = value 

   2. 线条末端样式 `lineCap` = type,有三个值

      1. `butt`：线段末端以方形结束
      2. `round`：线段末端以圆形结束
      3. `square`：线段末端以方形结束，但是增加了一个宽度和线段相同，高度是线段厚度一半的矩形区域

   3. 线条与线条间接合处的样式

      1. `round`

         通过填充一个额外的，圆心在相连部分末端的扇形，绘制拐角的形状。 圆角的半径是线段的宽度

      2. `bevel`

         在相连部分的末端填充一个额外的以三角形为底的区域， 每个部分都有各自独立的矩形拐角。

      3. `miter`

         通过延伸相连部分的外边缘，使其相交于一点，形成一个额外的菱形区域。

   4. 虚线

      1. `setLineDash`(实线长度,间隙长度)
      2. `lineDashOffset`设置起始偏移量

## 绘制文本

### 渲染文本方式

1. `fillText(text, x, y [, maxWidth])`
   1. 在指定的(x,y)位置填充指定的文本，绘制的最大宽度是可选的
2. `strokeText(text, x, y [, maxWidth])`
   1. 在指定的(x,y)位置绘制文本边框，绘制的最大宽度是可选的.

### 给文本添加样式

1. 字体:`font = value`
2. 文本对齐:textAlign = value
   1. `start`, `end`, `left`, `right` ,`center`
3. 基线对齐
   1. textBaseline = value
4. 方向对齐
   1. direction = value
   2. `ltr`, `rtl`, `inherit`

## 绘制图片

### 绘制步骤

1. 创建`<img>`元素

   `var img = new Image();   *// 创建一个元素* img.src = 'myImage.png'; *// 设置图片源地址*`

2. 绘制img

   `ctx.drawImage(img,0,0);` //参数1：要绘制的img  参数2、3：绘制的img在canvas中的坐标

3. `img` 可以 `new` 也可以来源于我们页面的 ``标签

4. 如果 `drawImage` 的时候图片还没有完全加载完成，则什么都不做，个别浏览器会抛异常。所以我们应该保证在 `img` 绘制完成之后再 `drawImage`。

### 缩放图片

1. 函数 `drawImage(image, x, y, width, height)`

### 切片

1. `drawImage(image, sx, sy, sWidth, sHeight, dx, dy, dWidth, dHeight)`
2. 第一个参数是 图像, sx,sy,swidth,sheight 是源图像的切片, 后面是目标图像的切片

## 状态的保存和恢复

1. save()
   1. Canvas状态存储在栈中，每当`save()`方法被调用后，当前的状态就被推送到栈中保存
   2. 一个绘画状态包括：
      1. 当前应用的变形（即移动，旋转和缩放）
      2. 各种样式
      3. 当前的裁切路径（`clipping path`）
   3. 可以调用任意多次 `save`方法。(类似数组的`push()`)
2. restore()
   1. 每一次调用 `restore` 方法，上一个保存的状态就从栈中弹出，所有设定都恢复

## 变形

1. 移动

   1. `translate(x, y)`

   2. 用来移动 `canvas` 的**原点**到指定的位置

2. 旋转坐标系

   1. `rotate(angle)`
   2. 旋转的中心是坐标原点。
   3. 它是顺时针方向的，以弧度为单位的值。

3. 缩放

   1. `scale(x, y)`
   2. `x,y`分别是横轴和纵轴的缩放因子
   3. 它们都必须是正值。值比 1.0 小表示缩 小，比 1.0 大则表示放大

4. 变形矩阵

5. 合成

   1. globalCompositeOperation = type
   2. type `是下面 13 种字符串值之一：
      1. `source-over(default)` : 默认设置，新图像会覆盖在原有图像
      2. `source-in`:仅仅会出现新图像与原来图像重叠的部分，其他区域都变成透明的。(包括其他的老图像区域也会透明)
      3. `source-out`:仅仅显示新图像与老图像没有重叠的部分，其余部分全部透明。(老图像也不显示)
      4. `source-atop`:新图像仅仅显示与老图像重叠区域。老图像仍然可以显示。
      5. `destination-over`:新图像会在老图像的下面
      6. `destination-in`:仅仅新老图像重叠部分的老图像被显示，其他区域全部透明。
      7. `destination-out`:仅仅老图像与新图像没有重叠的部分。 注意显示的是老图像的部分区域。
      8. `destination-atop`:老图像仅仅仅仅显示重叠部分，新图像会显示在老图像的下面。
      9. `lighter`:新老图像都显示，但是重叠区域的颜色做加处理
      10. `darken`:保留重叠部分最黑的像素。(每个颜色位进行比较，得到最小的)
      11. `lighten`:保证重叠部分最量的像素。(每个颜色位进行比较，得到最大的)
      12. `xor`:重叠部分会变成透明
      13. `copy`只有新图像会被保留，其余的全部被清除(边透明)

   

## 裁剪路径

1. `clip()`把已经创建的路径转换成裁剪路径。
2. 裁剪路径的作用是遮罩。只显示裁剪路径内的区域，裁剪路径外的区域会被隐藏
3. `clip()`只能遮罩在这个方法调用之后绘制的图像，如果是`clip()`方法调用之前绘制的图像，则无法实现遮罩。

## 动画

### 动画的基本步骤

1. 清空`canvas`

2. 保存`canvas`状态

   如果在绘制的过程中会更改`canvas`的状态(颜色、移动了坐标原点等),又在绘制每一帧时都是原始状态的话，则最好保存下`canvas`的状态

3. 绘制动画图形

4. 恢复`canvas`状态

### 控制动画

1. `setInterval()`
2. `setTimeout()`
3. `requestAnimationFrame()`



# example

## 1.绘制五角星

```html
 c.beginPath(); 
 c.fillStyle='rgba(255,0,0,0.25)';
 var x=200,y=200;
 var r1 = 60;
  var r2 = 120;
 for(var i=0;i<5;i++){
  c.lineTo(x+r1*Math.cos( Math.PI*((54-i*72)/180)),y-r1*Math.sin(Math.PI*((54-i*72)/180)));
  c.lineTo(x+r2*Math.cos( Math.PI*((18-i*72)/180)),y-r2*Math.sin(Math.PI*((18-i*72)/180)));
}
 c.closePath();
 c.stroke();
```

