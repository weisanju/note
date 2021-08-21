{% raw %}

# 绑定 class

## 对象语法

```html
<div v-bind:class="{ active: isActive }"></div>

<div
  class="static"
  v-bind:class="{ active: isActive, 'text-danger': hasError }"
></div>
```

isActive,hasError 决定类的存在与否

class 对象:多个 class 的集合:绑定的数据对象不必内联定义在模板里：

```
<div v-bind:class="classObject"></div>
data: {
  classObject: {
    active: true,
    'text-danger': false
  }
}
```

## 绑定计算属性

```
<div v-bind:class="classObject"></div>
data: {
  isActive: true,
  error: null
},
computed: {
  classObject: function () {
    return {
      active: this.isActive && !this.error,
      'text-danger': this.error && this.error.type === 'fatal'
    }
  }
}
```

## 绑定数组

```
<div v-bind:class="[activeClass, errorClass]"></div>

data: {
  activeClass: 'active',
  errorClass: 'text-danger'
}

<div v-bind:class="[isActive ? activeClass : '', errorClass]"></div>

<div v-bind:class="[{ active: isActive }, errorClass]"></div>
```

# 绑定内联样式

## 内联对象语法

```html
<div v-bind:style="{ color: activeColor, fontSize: fontSize + 'px' }"></div>

data: { activeColor: 'red', fontSize: 30 }
```

## 对象语法

```
data: {
  styleObject: {
    color: 'red',
    fontSize: '13px'
  }
}
<div v-bind:style="styleObject"></div>
```

## 数组语法

```
<div v-bind:style="[baseStyles, overridingStyles]"></div>
```

## 自动添加前缀

```
当 v-bind:style 使用需要添加浏览器引擎前缀的 CSS property 时，如 transform，Vue.js 会自动侦测并添加相应的前缀
```

## 多重值

从 2.3.0 起你可以为 `style` 绑定中的 property 提供一个包含多个值的数组，常用于提供多个带前缀的值，例如：

```
<div :style="{ display: ['-webkit-box', '-ms-flexbox', 'flex'] }"></div>
```

这样写只会渲染数组中最后一个被浏览器支持的值。在本例中，如果浏览器支持不带浏览器前缀的 flexbox，那么就只会渲染 `display: flex`。

{% endraw %}
