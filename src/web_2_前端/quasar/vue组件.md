# [Ajax栏](http://www.quasarchs.com/vue-components/ajax-bar#Introduction)

* ajax调用反馈

* QAjaxBar组件自动捕获Ajax调用（除非被告知不这样做）。

* 手动触发

  ```js
  <script>
  export default {
    methods: {
      // we manually trigger it (this is not needed if we
      // don't skip Ajax calls hijacking)
      trigger () {
        const bar = this.$refs.bar
  
        bar.start()
  
        this.timer = setTimeout(() => {
          if (this.$refs.bar) {
            this.$refs.bar.stop()
          }
        }, Math.random() * 3000 + 1000)
      }
    }
  }
  </script>
  ```

```vue
<template>
  <div class="q-pa-md">
    <q-ajax-bar
      ref="bar"
      position="bottom"
      color="accent"
      size="10px"
      skip-hijack
    />

    <q-btn color="primary" label="Trigger" @click="trigger" />
  </div>
</template>
```



# [头像](http://www.quasarchs.com/vue-components/avatar#Introduction)

QAvatar组件创建一个可缩放的、可着色的元素，其形状内可以包含文本、图标或图像。 默认情况下，它是圆形的，但也可以是正方形的，也可以应用边框半径为正方形提供圆角。

它通常与插槽中的其他组件一起使用。

```vue
<template>
  <div class="q-pa-md q-gutter-sm">
    <q-avatar color="red" text-color="white" icon="directions" />
    <q-avatar color="primary" text-color="white">J</q-avatar>
    <q-avatar size="100px" font-size="52px" color="teal" text-color="white" icon="directions" />
    <q-avatar size="24px" color="orange">J</q-avatar>
    <q-avatar>
      <img src="https://cdn.quasar.dev/img/avatar.png">
    </q-avatar>
  </div>
</template>
```



# [标记](http://www.quasarchs.com/vue-components/badge#Introduction)

使用QBadge组件，您可以创建一个小型标记，以添加需要突出和引起注意的信息（如上下文数据）。 与其他元素（例如用户头像）结合使用以显示大量新消息时，通常也很有用。

```html
<template>
  <div class="q-pa-md q-gutter-md">
    <q-badge color="blue">
      #4D96F2
    </q-badge>

    <q-badge color="orange" text-color="black" label="2" />

    <q-badge color="purple">
      <q-icon name="bluetooth" color="white" />
    </q-badge>

    <q-badge color="red">
      12 <q-icon name="warning" color="white" class="q-ml-xs" />
    </q-badge>

    <div class="text-h6">
      Badge <q-badge color="primary">v1.0.0+</q-badge>
    </div>

    <div>
      Feature <q-badge color="primary">v1.0.0+</q-badge>
    </div>

    <q-item clickable v-ripple class="bg-grey-2">
      <q-item-section avatar>
        <q-avatar rounded>
          <img src="https://cdn.quasar.dev/img/chaosmonkey.png">
        </q-avatar>
      </q-item-section>

      <q-item-section>
        <q-item-label>
          Ganglia
        </q-item-label>
        <q-item-label caption>
          <q-badge color="yellow-6" text-color="black">
            3
            <q-icon
              name="warning"
              size="14px"
              class="q-ml-xs"
            />
          </q-badge>
        </q-item-label>
      </q-item-section>

      <q-item-section side>
        <span>2 min ago</span>
      </q-item-section>
    </q-item>
  </div>
</template>
```

# [横幅](http://www.quasarchs.com/vue-components/banner#Introduction)

QBanner组件创建横幅元素以显示突出的消息和相关的可选操作。

根据Material Design规范，横幅应“显示在屏幕顶部，顶部应用栏下方”-但是您当然可以在任何有意义的地方放置一个横幅，即使在QDialog中也可以。



# [栏](http://www.quasarchs.com/vue-components/bar#Introduction)

```html
 <q-bar>
      <q-btn dense flat :icon="fabApple" />
      <div class="text-weight-bold">
        App
      </div>
      <div class="cursor-pointer gt-md">File</div>
      <div class="cursor-pointer gt-md">Edit</div>
      <div class="cursor-pointer gt-md">View</div>
      <div class="cursor-pointer gt-md">Window</div>
      <div class="cursor-pointer gt-md">Help</div>
      <q-space />
      <q-btn dense flat icon="airplay" class="gt-xs" />
      <q-btn dense flat icon="battery_charging_full" />
      <q-btn dense flat icon="wifi" />
      <div>9:41</div>
      <q-btn dense flat icon="search" />
      <q-btn dense flat icon="list" />
    </q-bar>
```



# [面包屑](http://www.quasarchs.com/vue-components/breadcrumbs#Introduction)

```vue
<template>
  <div class="q-pa-md q-gutter-sm">
    <q-breadcrumbs>
      <q-breadcrumbs-el label="Home" />
      <q-breadcrumbs-el label="Components" />
      <q-breadcrumbs-el label="Breadcrumbs" />
    </q-breadcrumbs>

    <q-breadcrumbs>
      <q-breadcrumbs-el label="Home" icon="home" />
      <q-breadcrumbs-el label="Components" icon="widgets" />
      <q-breadcrumbs-el label="Breadcrumbs" />
    </q-breadcrumbs>

    <q-breadcrumbs class="text-grey">
      <q-breadcrumbs-el icon="home" />
      <q-breadcrumbs-el icon="widgets" />
      <q-breadcrumbs-el icon="navigation" />
    </q-breadcrumbs>
  </div>
</template>
```

# 按钮

图标

按钮形状

大小

内容对齐

进度

波纹

路由链接

tooltip

禁用

# [按钮组](http://www.quasarchs.com/vue-components/button-group#Introduction)

您可以使用QBtnGroup方便地将[QBtn](http://www.quasarchs.com/vue-components/button)和[QBtnDropdown](http://www.quasarchs.com/vue-components/button-dropdown)分组。 确保查阅那些组件的相应页面以查看其属性和方法。

# [按钮下拉](http://www.quasarchs.com/vue-components/button-dropdown#Introduction)



# [卡片](http://www.quasarchs.com/vue-components/card#Introduction)

QCard组件是显示重要分组内容的好方法。 这种模式正在迅速成为应用、网站预览和电子邮件内容的核心设计模式。 它通过包含和组织信息来帮助观看者，同时还设置可预测的期望。

卡片具有一次可显示的大量内容，而屏幕尺寸通常很少，因此，卡片已迅速成为许多公司（包括Google和Twitter之类）的首选设计模式。

QCard组件特意是轻巧的，并且实质上是一个包含元素，该元素能够“容纳”任何其他合适的组件。



* qcard

* qcard-selection

* qcard-actions

* 对齐

* 可以放置多媒体

* ```
        <q-parallax
          src="https://cdn.quasar.dev/img/parallax1.jpg"
          :height="150"
        />平行
        
  ```

  

* 水平布局垂直布局
* 可展开的

# 转盘

跑马灯

* q-carousel

* q-carousel-slide
* 

# 聊天消息

* q-chat-message

```html
<q-chat-message
        :text="['hey, how are you?']"
        sent
      />
      <q-chat-message
        :text="['doing fine, how r you?']"
      />
```



# 碎片

QChip组件基本上是一个简单的UI块实体，以紧凑的方式表示例如更高级的基础数据，就像联系人。

碎片可以包含诸如头像，文本或图标之类的实体，还可以选择具有指针。 如果进行了配置，它们也可以关闭或移除。

```html
 <div>
      <q-chip icon="event">Add to calendar</q-chip>
      <q-chip icon="bookmark">Bookmark</q-chip>
      <q-chip icon="alarm" label="Set alarm" />
      <q-chip class="glossy" icon="directions">Get directions</q-chip>
    </div>
```

# [循环进度](http://www.quasarchs.com/vue-components/circular-progress#Introduction)

QCircularProgress



# 选色器

```html
 <div class="q-pa-md row items-start q-gutter-md">
    <q-color v-model="hex" class="my-picker" />
    <q-color v-model="hexa" class="my-picker" />
    <q-color v-model="rgb" class="my-picker" />
    <q-color v-model="rgba" class="my-picker" />
  </div>
```

# 对话框

q-dialog



# 编辑器

QEditor



# 表单组件

* q-input

* q-select

* q-form

* QField(包裹器)

* q-radio

  ```
   <div class="q-gutter-sm">
        <q-radio v-model="shape" val="line" label="Line" />
        <q-radio v-model="shape" val="rectangle" label="Rectangle" />
        <q-radio v-model="shape" val="ellipse" label="Ellipse" />
        <q-radio v-model="shape" val="polygon" label="Polygon" />
      </div>
  ```

* q-option-group

  ```
   <div class="q-pa-md">
      <q-option-group
        :options="options"
        label="Notifications"
        type="radio"
        v-model="group"
      />
    </div>
  ```

  

* checkbox
* QToggle
* QBtnToggle
* q-option-group
* q-slider
* q-range
* q-time
* q-date

# [图标](http://www.quasarchs.com/vue-components/icon#Introduction)

# [图像](http://www.quasarchs.com/vue-components/img#Introduction)

qimage

# 无线滚动

q-infinite-scroll 

# 内部加载

QInnerLoading组件允许您在组件内添加进度动画。 与[加载插件](http://www.quasarchs.com/quasar-plugins/loading)非常相似，其目的是向用户提供视觉确认，表明某些进程正在后台进行，这会花费大量时间。 QInnerLoading将在延迟的元素以及[旋转器](http://www.quasarchs.com/vue-components/spinners)上添加一个不透明的覆盖层。



# [交叉](http://www.quasarchs.com/vue-components/intersection#Introduction)

QIntersection组件本质上是[Intersection指令](http://www.quasarchs.com/vue-directives/intersection)的封装，它的附加好处是它可以单独处理状态（不需要您手动添加状态），并且可以有显示/隐藏过渡效果。

但是，使用QIntersection的主要好处是，DOM树释放了隐藏的节点，因此使用了尽可能少的RAM内存，并使页面感觉非常活泼。

在幕后，它使用[Intersection Observer API](https://developer.mozilla.org/en-US/docs/Web/API/Intersection_Observer_API)。

# [旋钮](http://www.quasarchs.com/vue-components/knob#Introduction)

QKnob

# [线性进度](http://www.quasarchs.com/vue-components/linear-progress#Introduction)

QLinearProgress

# [列表和选项](http://www.quasarchs.com/vue-components/list-and-list-items#Introduction)

   'QList',      'QItem',      'QItemSection',      'QItemLabel'



# [标记表](http://www.quasarchs.com/vue-components/markup-table#Introduction)

使用QMarkupTable可以简单地包裹原生的`<table>`以使其看起来像Material Design表。



# [菜单](http://www.quasarchs.com/vue-components/menu#Introduction)

QMenu的想法是将其放置在您希望成为触发器的DOM元素/组件中, 作为直接子元素。 不必担心QMenu内容会从容器继承CSS，因为QMenu将通过Quasar Portal作为`<body>`的直接子元素注入。

> 如果您希望菜单自动关闭，请不要忘记在可单击的菜单项中使用指令`v-close-popup`。 另外，您可以使用QMenu的属性`auto-close`，也可以通过其v-model自行处理关闭菜单的操作。

# 大小调整侦听器 (对于元素)

QResizeObserver

# [滚动侦听器](http://www.quasarchs.com/vue-components/scroll-observer#Introduction)

QScrollObserver

# 分页

QPagination

# 视差

QParallax

# [弹出编辑](http://www.quasarchs.com/vue-components/popup-edit#Introduction)

QPopupEdit

# [弹出代理](http://www.quasarchs.com/vue-components/popup-proxy#Introduction)

# [拉动刷新](http://www.quasarchs.com/vue-components/pull-to-refresh#Introduction)

# [评分](http://www.quasarchs.com/vue-components/rating#Introduction)

# 滚动区域

# 骨架(Skeleton)

# 滑动项

QSlideItem

# 滑动过度

```html
   <q-slide-transition>
      <div v-show="visible">
        <img
          class="responsive"
          src="https://cdn.quasar.dev/img/quasar.jpg"
        >
      </div>
    </q-slide-transition>
```

# 间距填充

q-space

# 旋转器

QSpinner

# 分割条

QSplitter

# 步骤

QStep

# 表格

QTable是允许您以表格方式显示数据的组件。 通常称为数据表。 它包含以下主要功能：

- 过滤
- 排序
- 具有自定义选择操作的单行/多行选择
- 分页（如果需要，包括服务器端）
- 网格模式（例如，您可以使用QCard以非表格方式显示数据）
- 通过有限范围的插槽，对行和单元格进行全面定制
- 能够在数据行的顶部或底部添加额外的行
- 列选取器（通过本页其中一节中描述的QTableColumns组件）
- 自定义顶部或底部表格控件
- 响应式设计

# 选项卡面板

 'QTabPanels',      'QTabPanel'

# 时间线

​     'QTimeline',      'QTimelineEntry'

# 提示

QTooltip

# 树

QTree

# 上传器

QUploader

# 视频

QVideo

# 虚拟滚动

QVirtualScroll



