# Alert 警告

用于页面中展示重要的提示信息。



# 基本用法

Alert 组件提供四种主题，由`type`属性指定，默认值为`info`。

```vue
<template>
  <el-alert
    title="成功提示的文案"
    type="success">
  </el-alert>
  <el-alert
    title="消息提示的文案"
    type="info">
  </el-alert>
  <el-alert
    title="警告提示的文案"
    type="warning">
  </el-alert>
  <el-alert
    title="错误提示的文案"
    type="error">
  </el-alert>
</template>
```



# 主题

Alert 组件提供了两个不同的主题：`light`和`dark`。

```vue
template>
  <el-alert
    title="成功提示的文案"
    type="success"
    effect="dark">
  </el-alert>
  <el-alert
    title="消息提示的文案"
    type="info"
    effect="dark">
  </el-alert>
  <el-alert
    title="警告提示的文案"
    type="warning"
    effect="dark">
  </el-alert>
  <el-alert
    title="错误提示的文案"
    type="error"
    effect="dark">
  </el-alert>
</template>
```

# 自定义关闭按钮

在 Alert 组件中，你可以设置是否可关闭，关闭按钮的文本以及关闭时的回调函数。`closable`属性决定是否可关闭，接受`boolean`，默认为`true`。你可以设置`close-text`属性来代替右侧的关闭图标，注意：`close-text`必须为文本。设置`close`事件来设置关闭时的回调。



```vue
template>
  <el-alert
    title="不可关闭的 alert"
    type="success"
    :closable="false">
  </el-alert>
  <el-alert
    title="自定义 close-text"
    type="info"
    close-text="知道了">
  </el-alert>
  <el-alert
    title="设置了回调的 alert"
    type="warning"
    @close="hello">
  </el-alert>
</template>

<script>
  export default {
    methods: {
      hello() {
        alert('Hello World!');
      }
    }
  }
</script>
```

# 带有 icon

```vue
通过设置show-icon属性来显示 Alert 的 icon，这能更有效地向用户展示你的显示意图。

<template>
  <el-alert
    title="成功提示的文案"
    type="success"
    show-icon>
  </el-alert>
  <el-alert
    title="消息提示的文案"
    type="info"
    show-icon>
  </el-alert>
  <el-alert
    title="警告提示的文案"
    type="warning"
    show-icon>
  </el-alert>
  <el-alert
    title="错误提示的文案"
    type="error"
    show-icon>
  </el-alert>
</template>
```



# 文字居中

```vue
<template>
  <el-alert
    title="成功提示的文案"
    type="success"
    center
    show-icon>
  </el-alert>
  <el-alert
    title="消息提示的文案"
    type="info"
    center
    show-icon>
  </el-alert>
  <el-alert
    title="警告提示的文案"
    type="warning"
    center
    show-icon>
  </el-alert>
  <el-alert
    title="错误提示的文案"
    type="error"
    center
    show-icon>
  </el-alert>
</template>
```

# 带有辅助性文字介绍

除了必填的`title`属性外，你可以设置`description`属性来帮助你更好地介绍，我们称之为辅助性文字。辅助性文字只能存放单行文本，会自动换行显示。

```vue
<template>
  <el-alert
    title="带辅助性文字介绍"
    type="success"
    description="这是一句绕口令：黑灰化肥会挥发发灰黑化肥挥发；灰黑化肥会挥发发黑灰化肥发挥。 黑灰化肥会挥发发灰黑化肥黑灰挥发化为灰……">
  </el-alert>
</template>
```

# 带有 icon 和辅助性文字介绍



# 属性事件和方法

## Attributes

| 参数        | 说明                               | 类型    | 可选值                     | 默认值 |
| :---------- | :--------------------------------- | :------ | :------------------------- | :----- |
| title       | 标题                               | string  | —                          | —      |
| type        | 主题                               | string  | success/warning/info/error | info   |
| description | 辅助性文字。也可通过默认 slot 传入 | string  | —                          | —      |
| closable    | 是否可关闭                         | boolean | —                          | true   |
| center      | 文字是否居中                       | boolean | —                          | true   |
| close-text  | 关闭按钮自定义文本                 | string  | —                          | —      |
| show-icon   | 是否显示图标                       | boolean | —                          | false  |
| effect      | 选择提供的主题                     | string  | light/dark                 | light  |

## [¶](https://element.eleme.cn/#/zh-CN/component/alert#slot)Slot

| Name  | Description |
| :---- | :---------- |
| —     | 描述        |
| title | 标题的内容  |

## [¶](https://element.eleme.cn/#/zh-CN/component/alert#events)Events

| 事件名称 | 说明                  | 回调参数 |
| :------- | :-------------------- | :------- |
| close    | 关闭alert时触发的事件 | —        |