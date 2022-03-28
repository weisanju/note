## 容器属性

### *display:grid*

对容器开启*grid*布局。使其子元素按grid布局



### *grid-template-columns*

> 指定栏数布局

> grid-template-columns: 100px 100px 100px 100px;

1. 设定容器的有多少列。以及每列的宽度
2. 可以设定 固定宽度 也可以是 百分比宽度 还可以是 fr 
3. fr 会根据所有元素的 fr值计算百分比
4. 百分比宽度 会优先分配前面的元素。后面的元素可能会分配不到指定的宽度

### *column-gap*

> 列间距

设定元素列之间的间距

### *row-gap*

> 行间距

设定元素行之间的间距

### *gap*

> 设定行列间距

### *grid-template-area*

> 排布方式。命名排布

**示例**

```html
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>布局方式</title>
    <style>
      .layout {
        display: grid;
        grid-template-areas:
          "header header header"
          "siderbar content content"
          "footer footer footer";
      }
      header {
        grid-area: header;
      }
      aside {
        grid-area: siderbar;
      }
      main {
        grid-area: content;
      }
      footer {
        grid-area: footer;
      }
    </style>
  </head>
  <body>
    <div class="layout">
      <header>头部</header>
      <aside>侧边栏</aside>
      <main>主体区域</main>
      <footer>尾部</footer>
    </div>
  </body>
</html>

```



### *justify-content*

> 元素沿着主轴对齐方式

*center*

1. 沿着主轴居中对齐
2. 如果没有指定宽高 元素大小会由内容撑开

*flex-start*

沿着主轴开始的地方对齐

*flex-end*

沿着主轴结束的地方对齐

*space-between*

每行的元素都会沿着主轴方向分散对齐。首尾元素与容器边缘不存在间隙

*space-around*

每行的元素沿着主轴方向分散对齐。首尾元素与容器边缘存在间隙，且其间隙是 元素之间间隙的1/2

*space-evenly*

每行的元素沿着主轴方向分散对齐。首尾元素与容器边缘存在间隙，且其间隙与 元素之间间隙的相等

*stretch*

默认值。撑满容器



### *align-items*

> 元素沿着交叉轴对齐方式

*center*

沿着交叉轴居中对齐

*flex-start*

沿着交叉轴开始的地方对齐

*flex-end*

沿着交叉轴结束的地方对齐

*start*

固定左边或者上边，与交叉轴方向无关

*end*

固定右边或者下边，与交叉轴方向无关