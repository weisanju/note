# 安装

## script引入

`Vue` 会被注册为一个全局变量。

[开发版本](https://cn.vuejs.org/js/vue.js):包含完整的警告和调试模式

[生产版本](https://cn.vuejs.org/js/vue.min.js):删除了警告，33.30KB min+gzip

### 使用CDN

```
<script src="https://cdn.jsdelivr.net/npm/vue/dist/vue.js"></script>
```

指定版本号

```
<script src="https://cdn.jsdelivr.net/npm/vue@2.6.11"></script>
```

### 兼容ESModule的构建文件

```
<script type="module">   import Vue from 'https://cdn.jsdelivr.net/npm/vue@2.6.11/dist/vue.esm.browser.js' </script>
```

### NPM安装

```shell
npm install vue
```

## 对不同构建版本的解析

 [NPM 包的 `dist/` 目录](https://cdn.jsdelivr.net/npm/vue/dist/)你将会找到很多不同的 Vue.js 构建版本

|                               | UMD                | CommonJS              | ES Module (基于构建工具使用) | ES Module (直接用于浏览器) |
| :---------------------------- | :----------------- | :-------------------- | :--------------------------- | -------------------------- |
| **完整版**                    | vue.js             | vue.common.js         | vue.esm.js                   | vue.esm.browser.js         |
| **只包含运行时版**            | vue.runtime.js     | vue.runtime.common.js | vue.runtime.esm.js           | -                          |
| **完整版 (生产环境)**         | vue.min.js         | -                     | -                            | vue.esm.browser.min.js     |
| **只包含运行时版 (生产环境)** | vue.runtime.min.js | -                     | -                            | -                          |



# 入门

## 声明式渲染

Vue.js 的核心是一个允许采用简洁的模板语法来声明式地将数据渲染进 DOM 的系统

```js
<div id="app">   {{ message }} </div>

var app = new Vue({
  el: '#app',
  data: {
    message: 'Hello Vue!'
  }
})
```

## 动态绑定

语法:v-bind:attributename

```js
var app2 = new Vue({
		  el: '#app-2',
		  data: {
		    message: '页面加载于 ' + new Date().toLocaleString()
		  }
		})
		
		<div id="app-2">
		  <span v-bind:title="message">
		    鼠标悬停几秒钟查看此处动态绑定的提示信息！
		  </span>
		</div>
```

## 条件

```js
<div id="app-3">
  <p v-if="seen">现在你看到我了</p>
</div>
var app3 = new Vue({
  el: '#app-3',
  data: {
    seen: true
  }
})
```

## 循环

```js
div id="app-4">
  <ol>
    <li v-for="todo in todos">
      {{ todo.text }}
    </li>
  </ol>
</div>
var app4 = new Vue({
  el: '#app-4',
  data: {
    todos: [
      { text: '学习 JavaScript' },
      { text: '学习 Vue' },
      { text: '整个牛项目' }
    ]
  }
})
```

## 处理用户输入

```html
<div id="app-5">
  <p>{{ message }}</p>
  <button v-on:click="reverseMessage">反转消息</button>
</div>
```

```js
var app5 = new Vue({
  el: '#app-5',
  data: {
    message: 'Hello Vue.js!'
  },
  methods: {
    reverseMessage: function () {
      this.message = this.message.split('').reverse().join('')
    }
  }
})
```

## 双向绑定

```html
<div id="app-6">
  <p>{{ message }}</p>
  <input v-model="message">
</div>
```

```js
var app6 = new Vue({
  el: '#app-6',
  data: {
    message: 'Hello Vue!'
  }
})
```

## 组件化应用构建

一个组件本质上是一个拥有预定义选项的一个 Vue 实例

人话:自定义标签

自定义todoItem

```js
// 定义名为 todo-item 的新组件
Vue.component('todo-item', {
  template: '<li>这是个待办项</li>'
})

var app = new Vue(...)
```

```html
		<div id="app">
			<todo-item/>
		</div>
```

todoitem 自定义文本

```js
Vue.component('todo-item', {
  props: ['todo'],
  template: '<li>{{ todo.text }}</li>'
})

var app7 = new Vue({
  el: '#app-7',
  data: {
    groceryList: [
      { id: 0, text: '蔬菜' },
      { id: 1, text: '奶酪' },
      { id: 2, text: '随便其它什么人吃的东西' }
    ]
  }
})
```

```html
<div id="app-7">
  <ol>
    <!--
      现在我们为每个 todo-item 提供 todo 对象
      todo 对象是变量，即其内容可以是动态的。
      我们也需要为每个组件提供一个“key”，稍后再
      作详细解释。
    -->
    <todo-item
      v-for="item in groceryList"
      v-bind:todo="item"
      v-bind:key="item.id"
    ></todo-item>
  </ol>
</div>
```

# VUE实例

```js
var vm = new Vue({
  // 选项
})
```

## VUE root实例

​	上述代码创建了一个VUE实例,一个 Vue 应用由一个通过 new Vue 创建的

* **根 Vue 实例**
* 以及可选的嵌套的、可复用的组件树组成。

一个 todo 应用的组件树可以是这样的：

```
根实例
└─ TodoList
   ├─ TodoItem
   │  ├─ DeleteTodoButton
   │  └─ EditTodoButton
   └─ TodoListFooter
      ├─ ClearTodosButton
      └─ TodoListStatistics
```

## 数据与方法

​	当一个 Vue 实例被创建时，它将 `data` 对象中的所有的 property 加入到 Vue 的**响应式系统**中。当这些 property 的值发生改变时，视图将会产生“响应”，即匹配更新为新的值。

```js
// 我们的数据对象
var data = { a: 1 }

// 该对象被加入到一个 Vue 实例中
var vm = new Vue({
  data: data
})

// 获得这个实例上的 property
// 返回源数据中对应的字段
vm.a == data.a // => true

// 设置 property 也会影响到原始数据
vm.a = 2
data.a // => 2

// ……反之亦然
data.a = 3
vm.a // => 3
```

​	只有当实例被创建时就已经存在于 `data` 中的 property 才是**响应式**的

```js
data: {
  newTodoText: '',
  visitCount: 0,
  hideCompletedTodos: false,
  todos: [],
  error: null
}
```

​	Object.freeze()阻止 属性响应

```js
var obj = {
  foo: 'bar'
}

Object.freeze(obj)

new Vue({
  el: '#app',
  data: obj
})
```

​	Vue 实例还暴露了一些有用的实例 property 与方法,它们都有前缀 `$`，以便与用户定义的 property 区分开来

```js
var data = { a: 1 }
var vm = new Vue({
  el: '#example',
  data: data
})

vm.$data === data // => true
vm.$el === document.getElementById('example') // => true

// $watch 是一个实例方法
vm.$watch('a', function (newValue, oldValue) {
  // 这个回调将在 `vm.a` 改变后调用
})
```

## 实例生命周期钩子

每个 Vue 实例在被创建时都要经过一系列的初始化过程—例如，

* 需要设置数据监听
* 编译模板
* 将实例挂载到 DOM 
* 并在数据变化时更新 DOM 等

```js
new Vue({
  data: {
    a: 1
  },
  created: function () {
    // `this` 指向 vm 实例
    console.log('a is: ' + this.a)
  }
})
// => "a is: 1"
```

​	也有一些其它的钩子，在实例生命周期的不同阶段被调用，如 [`mounted`](https://vuejs.bootcss.com/api/#mounted)、[`updated`](https://vuejs.bootcss.com/api/#updated) 和 [`destroyed`](https://vuejs.bootcss.com/api/#destroyed)。生命周期钩子的 `this` 上下文指向调用它的 Vue 实例。

# 其他

[仓库托管](https://github.com/vuejs/vue/releases)

[浏览器插件](https://github.com/vuejs/vue-devtools#vue-devtools)



