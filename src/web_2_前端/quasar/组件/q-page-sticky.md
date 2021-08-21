# 简介

QPageSticky组件有助于将由它包裹的DOM元素/组件放置到QPage内容区域中的静态位置，无论用户在哪里滚动。



这样做的最大好处是，即使未配置为固定，此组件包裹的元素也不会与布局页眉、页脚或侧滑菜单重叠。 在后一种情况下，位置将偏移，因此不会发生重叠。 例如，尝试使用非固定页脚。 当用户触及屏幕底部并进入视图时，组件将向上移动，因此它不会与页脚重叠。



# 基本使用

```vue
   <!-- place QPageSticky at end of page -->
          <q-page-sticky position="top-left" :offset="[18, 18]">
            <q-btn round color="accent" icon="arrow_back" class="rotate-45" />
          </q-page-sticky>
```







# API

| name     | type    | description                                            | defaultValue   | 可选值                                                       | example                         |
| -------- | ------- | ------------------------------------------------------ | -------------- | ------------------------------------------------------------ | ------------------------------- |
| position | String  | 决定位于Page的位置                                     | "bottom-right" | top-right<br />top-left<br />bottom-right<br />bottom-left<br />top<br />right<br />bottom<br />left | :to="{ name: 'my-route-name' }" |
| offset   | Array   | 偏移量                                                 |                |                                                              | [8, 8]                          |
| expand   | Boolean | 默认情况下，该组件自动收缩，如果使用该属性，则充分扩展 |                |                                                              |                                 |

