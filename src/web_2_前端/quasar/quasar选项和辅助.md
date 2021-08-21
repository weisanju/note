# Vue原型注入

Quasar用`$q`对象注入Vue原型：

| 注入           | 类型   | 说明                                                         |
| :------------- | :----- | :----------------------------------------------------------- |
| `$q.version`   | 字符串 | quasar版本。                                                 |
| `$q.platform`  | 对象   | 从Quasar导入与[平台](http://www.quasarchs.com/options/platform-detection)相同的对象。 |
| `$q.screen`    | 对象   | [屏幕插件](http://www.quasarchs.com/options/screen-plugin)提供的对象。 |
| `$q.lang`      | 对象   | Quasar语言包管理，包含标签等（[语言文件](https://github.com/quasarframework/quasar/tree/dev/ui/lang)之一）。 专为Quasar组件设计，但您也可以在您的应用组件中使用。更多信息: [Quasar语言包](http://www.quasarchs.com/options/quasar-language-packs). |
| `$q.iconSet`   | Object | Quasar图标集管理 ([图标集文件](https://github.com/quasarframework/quasar/tree/dev/ui/icon-set)之一). 专为Quasar组件设计，但您也可以在您的应用组件中使用。更多信息: [Quasar图标集](http://www.quasarchs.com/options/quasar-icon-sets). |
| `$q.cordova`   | 对象   | 引用Cordova全局对象。 只有在Cordova应用程序下运行时才可用。  |
| `$q.capacitor` | 对象   | (@quasar/app v1.2+) 引用Capacitor全局对象。 只有在Capacitor应用程序下运行时才可用。 |
| `$q.electron`  | 对象   | 参考Electron全局对象。 仅在Electron应用程序下运行和**如果[Node集成](http://www.quasarchs.com/quasar-cli/developing-electron-apps/node-integration)未关闭**时才可用。 |

## 例子

判断是否是IOS

```vue
<!-- 在Vue模板中 -->
<template>
  <div>
    <div v-if="$q.platform.is.ios">
      Gets rendered only on iOS platform.
    </div>
  </div>
</template>

<script>
// 在export以外不可用

export default {
  // 在一个Vue组件脚本中
  ...,

  // 显示一个方法的例子，但是
  // 可以是Vue脚本的任何部分
  methods: {
    show () {
      // 打印出Quasar版本
      console.log(this.$q.version)
    }
  }
}
</script>
```



# 应用图标

## Icon Genie

查看[Icon Genie存储库](https://github.com/quasarframework/app-extension-icon-genie/blob/dev/README.md)，了解有关工作原理的详细信息，或者深入了解并如[任何应用扩展](http://www.quasarchs.com/app-extensions/introduction)一样在你的项目安装它：

```bash
$ quasar ext add @quasar/icon-genie
```



它会要求您告诉它在哪里可以找到源图像（1240x1240）以及您要使用的缩小策略。 然后当你运行`$ quasar dev`时 - 它会生成正确的图标并将它们放在适合你的所有位置，无论你使用什么样的`--mode`; 如果您只是在本地提供HMR服务或使用`build`生成最终资产。



## 尚未使用Quasar CLI

您必须转换、调整大小、命名和放置文件(无论它们在哪里)，并且取决于您构建应用（可在任何地方）的方式。

对于这种情况下的开发人员，我们提供了一个选项，您可以全局安装Icon Genie作为npm模块，并使用它来生成您需要的图标：

```bash
$ npm install --global @quasar/app-extension-icon-genie
$ icongenie -p=kitchensink -s=icon-1280x1280.png -t=./outputFolder -m=pngquant
```

有关此类用法的完整详细信息，请访问app-extension存储库。



# Quasar框架的SEO

## Quasar Meta插件

[Quasar Meta插件](http://www.quasarchs.com/quasar-plugins/meta) 可以动态更改页面标题、管理`<meta>`标签、管理`<html>`和`<body>`DOM元素属性、添加/删除/更改文档头部的`<style>`和`<script>`标签（例如用于CDN样式表或json-ld标记）或管理`<noscript>`标签。

使用**Quasar CLI**，特别是**用于SSR（服务器端渲染）构建**时，充分利用此功能。 将它用于SPA（单页应用程序）是没有意义的，因为在这种情况下，元信息将在运行时添加，而不是由Web服务器直接提供（如在SSR构建中）。

>  这个Quasar插件与Quasar的集成最紧密，因此它具有与任何其他类似解决方案相比的最佳性能。



# Quasar语言包

Quasar语言包作用于Quasar自己组件的国际化，其中一些组件具有标签。

>应该注意，下面描述的仅是Quasar组件的国际化。 如果您需要国际化自己的组件，请阅读[应用国际化](http://www.quasarchs.com/options/app-internationalization) 文档页面。



## 配置默认语言包

除非另有配置（见下文），否则Quasar默认使用`en-us`语言包。

### 硬编码默认语言包

如果未动态确定默认的Quasar语言包（例如，不依赖于cookie），则可以：

#### Quasar CLI

编辑`/quasar.conf.js`:

```js
framework: {
  lang: 'de'
}
```

#### Vue CLI

编辑你的`main.js`:

```js
import langDe from 'quasar/lang/de'
// ...

// when not selecting to import all Quasar components:
import { Quasar } from 'quasar'
// OTHERWISE:
import Quasar from 'quasar'

// ...
Vue.use(Quasar, {
  // ...,
  lang: langDe
})
```

#### Quasar UMD

包含你的Quasar版本的语言包JS标签，并告诉Quasar使用它。 例：

```html
<!-- 在Quasar JS之后包含这个标签 -->
<script src="https://cdn.jsdelivr.net/npm/quasar@v1.0.0/dist/lang/de.umd.min.js"></script>
<script>
  Quasar.lang.set(Quasar.lang.de)
</script>
```

### 动态选择默认语言

Quasar CLI：如果必须动态选择所需的Quasar语言包（例如：依赖于cookie），则需要创建一个启动文件：`$ quasar new boot quasar-lang-pack`。 这将创建``/src/boot/quasar-lang-pack.js` 文件。 编辑为：

```js
// for when you don't specify quasar.conf.js > framework: 'all'
import { Quasar } from 'quasar'
// OTHERWISE:
import Quasar from 'quasar'

export default async () => {
  const langIso = 'de' // ... some logic to determine it (use Cookies Plugin?)

  try {
    await import(
      /* webpackInclude: /(de|en-us)\.js$/ */
      `quasar/lang/${langIso}`
      )
      .then(lang => {
        Quasar.lang.set(lang.default)
      })
  }
  catch (err) {
    // Requested Quasar Language Pack does not exist,
    // let's not break the app, so catching error
  }
}
```

然后将此启动文件注册到`/quasar.conf.js`：

```js
boot: [
  'quasar-lang-pack'
]
```

## 在运行时更改Quasar语言包

使用QSelect动态更改Quasar组件语言的示例：

```html
<template>
  <q-select
    v-model="lang"
    :options="langOptions"
    label="Quasar Language"
    dense
    borderless
    emit-value
    map-options
    options-dense
    style="min-width: 150px"
  />
</template>

<script>
import languages from 'quasar/lang/index.json'
const appLanguages = languages.filter(lang =>
  [ 'de', 'en-us' ].includes(lang.isoName)
)

export default {
  data () {
    return {
      lang: this.$q.lang.isoName
    }
  },

  watch: {
    lang (lang) {
      // dynamic import, so loading on demand only
      import(
        /* webpackInclude: /(de|en-us)\.js$/ */
        `quasar/lang/${lang}`
        ).then(lang => {
        this.$q.lang.set(lang.default)
      })
    }
  },

  created () {
    this.langOptions = appLanguages.map(lang => ({
      label: lang.nativeName, value: lang.isoName
    }))
  }
}
</script>
```

## 在App Space中使用Quasar语言包

虽然Quasar语言包**仅适用于Quasar组件内部使用**，但您仍可以将其标签用于您自己的网站/应用程序组件。

```html
当前Quasar语言包中的“Close”标签是：
{{ $q.lang.label.close }}
```

查看[GitHub]（https://github.com/quasarframework/quasar/tree/dev/ui/lang）上的Quasar语言包，查看`$q.lang`的结构。

## 检测区域设置

还有一种Quasar提供的开箱即用的方法可以确定用户区域设置：

```js
// outside of a Vue file

// for when you don't specify quasar.conf.js > framework: 'all'
import { Quasar } from 'quasar'
// OTHERWISE:
import Quasar from 'quasar'

Quasar.lang.getLocale() // returns a string

// inside of a Vue file
this.$q.lang.getLocale() // returns a string
```



# [应用国际化 (I18n)](http://www.quasarchs.com/options/app-internationalization#Introduction)

国际化是一个设计过程，可确保产品（网站或应用程序）可适应各种语言和地区，而无需对源代码进行工程更改。将国际化视为本土化的准备。



# [RTL](http://www.quasarchs.com/options/rtl-support#Introduction)支持

RTL指的是需要“从右到左”展示的语言的UI。

# [安装图标库](http://www.quasarchs.com/options/installing-icon-libraries#Introduction)

**此页面仅指使用[webfont图标](http://www.quasarchs.com/vue-components/icon#Webfont-icons)。** SVG图标不需要任何安装步骤。

您很可能想要在您的网站/应用中使用图标，而Quasar提供了一个开箱即用的简单方法，用于以下图标库： [Material Icons](https://material.io/icons/) 、 [Font Awesome](http://fontawesome.io/icons/)、 [Ionicons](http://ionicons.com/), [MDI](https://materialdesignicons.com/)、 [Eva Icons](https://akveo.github.io/eva-icons) 和 [Themify Icons](https://themify.me/themify-icons)。

TIP

关于webfont图标，您可以选择安装一个或多个这些图标库。



# [Quasar](http://www.quasarchs.com/options/quasar-icon-sets#Introduction)图标集

Quasar组件有自己的图标。 Quasar并不强迫您特别使用一个图标库（以便它们可以正确显示），而允许您选择**应该用于其组件的图标**。 这被称为“Quasar图标集”。

您可以安装多个图标库，但必须只选择一个用于Quasar组件的图标库。

Quasar当前支持: [Material Icons](https://material.io/icons/)、 [Font Awesome](http://fontawesome.io/icons/)、 [Ionicons](http://ionicons.com/)、 [MDI](https://materialdesignicons.com/)、 [Eva Icons](https://akveo.github.io/eva-icons)和[Themify Icons](https://themify.me/themify-icons)。

也可以将自己的图标（作为自定义svg或任何格式的图像）与任何Quasar组件一起使用，有关更多信息，请参见[QIcon](http://www.quasarchs.com/vue-components/icon#Image-icons)页面。

TIP

相关页面： [安装图标库](http://www.quasarchs.com/options/installing-icon-libraries) and [QIcon组件](http://www.quasarchs.com/vue-components/icon).



# [平台检测](http://www.quasarchs.com/options/platform-detection#Introduction)

辅助程序内置于Quasar中，用于在运行代码的上下文中检测平台（及其功能）。

TIP

根据您的需要，您可能还需要查看[风格&特性 > 可见性](http://www.quasarchs.com/style/visibility) 页面以了解如何仅使用CSS来实现相同的效果。后一种方法将渲染您的DOM元素或组件，而不管平台如何，所以基于应用程序的性能做出明智的选择 。

## 用法

Vue组件JS中的用法：

```js
this.$q.platform.is.mobile
```

Vue组件模板中的用法：

```js
$q.platform.is.cordova
```

在Vue组件之外使用它时必须导入它：

```js
import { Platform } from 'quasar'
```



`Platform.is`本身返回一个包含当前平台详细信息的对象。 例如，在MacOS桌面计算机上运行Chrome时，`Platform.is`会返回类似以下信息：

```js
{
  chrome: true,
  desktop: true,
  mac: true,
  name: "chrome",
  platform: "mac",
  version: "70.0.3538.110",
  versionNumber: 70,
  webkit: true
}
```

现在，假设我们想要根据代码运行的平台呈现不同的组件或DOM元素。 我们想在桌面上展示一些东西，在移动设备上展示其他东西，我们会这样做：

```html
<div v-if="$q.platform.is.desktop">
  I'm only rendered on desktop!
</div>

<div v-if="$q.platform.is.mobile">
  I'm only rendered on mobile!
</div>

<div v-if="$q.platform.is.electron">
  I'm only rendered on Electron!
</div>
```



## 属性

Platform对象可以使用以下属性。 尽管如此，这并不是一个详尽的清单。 有关详细信息，请参阅下面的API部分。

| 属性                     | 类型 | 含义                                                     |
| :----------------------- | :--- | :------------------------------------------------------- |
| `Platform.is.mobile`     | 布尔 | 代码是否在移动设备上运行？                               |
| `Platform.is.cordova`    | 布尔 | 代码是否在Cordova内运行？                                |
| `Platform.is.capacitor`  | 布尔 | 代码是否与Capacitor一起运行？ （需要@quasar/app v1.2 +） |
| `Platform.is.electron`   | 布尔 | 代码是否在Electron内运行？                               |
| `Platform.is.desktop`    | 布尔 | 代码是否在桌面浏览器上运行？                             |
| `Platform.is.bex`        | 布尔 | 代码是否在浏览器扩展中运行？ （需要@quasar/app v1.2 +）  |
| `Platform.is.android`    | 布尔 | 应用是否在Android设备上运行？                            |
| `Platform.is.blackberry` | 布尔 | 应用是否在Blackberry设备上运行？                         |
| `Platform.is.cros`       | 布尔 | 应用是否在具有Chrome OS操作系统的设备上运行？            |
| `Platform.is.ios`        | 布尔 | 应用是否在iOS设备上运行？                                |
| `Platform.is.ipad`       | 布尔 | 应用是否在iPad上运行？                                   |
| `Platform.is.iphone`     | 布尔 | 应用是否在iPhone上运行？                                 |
| `Platform.is.ipod`       | 布尔 | 应用是否在iPod上运行？                                   |
| `Platform.is.kindle`     | 布尔 | 应用是否在Kindle设备上运行？                             |
| `Platform.is.linux`      | 布尔 | 代码是否在具有Linux操作系统的设备上运行？                |
| `Platform.is.mac`        | 布尔 | 代码是否在具有MacOS操作系统的设备上运行？                |
| `Platform.is.win`        | 布尔 | 代码是否在具有Windows操作系统的设备上运行？              |
| `Platform.is.winphone`   | 布尔 | 代码是否在Windows Phone设备上运行？                      |
| `Platform.is.playbook`   | 布尔 | 代码是否在Blackberry Playbook设备上运行？                |
| `Platform.is.silk`       | 布尔 | 代码是否在Kindle Silk浏览器中运行？                      |
| `Platform.is.chrome`     | 布尔 | 代码是否在Google Chrome浏览器中运行？                    |
| `Platform.is.opera`      | 布尔 | 代码是否在Opera Phone浏览器中运行？                      |
| `Platform.is.safari`     | 布尔 | 代码是否在Apple Safari浏览器中运行？                     |
| `Platform.is.edge`       | 布尔 | 代码是否在Microsoft Edge浏览器中运行？                   |
| `Platform.is.ie`         | 布尔 | 代码是否在Microsoft Internet Explorer浏览器中运行？      |
| `Platform.has.touch`     | 布尔 | 代码是否在支持触摸的屏幕上运行？                         |
| `Platform.within.iframe` | 布尔 | 该应用是否在IFRAME内运行？                               |

# [屏幕插件](http://www.quasarchs.com/options/screen-plugin#Introduction)

Quasar屏幕插件允许您在处理Javascript代码时拥有动态且响应迅速的UI。 如果可能，出于性能原因，建议使用[响应式CSS类](http://www.quasarchs.com/style/visibility#Window-Width-Related) 。

## 安装

你不需要做任何事情。屏幕插件会自动安装。



## 用法

请注意下面的`$q.screen`。 这只是一个简单的用法示例。

```html
<q-list :dense="$q.screen.lt.md">
  <q-item>
    <q-item-section>John Doe</q-item-section>
  </q-item>

  <q-item>
    <q-item-section>Jane Doe</q-item-section>
  </q-item>
</q-list>
// Vue组件的脚本部分
export default {
  computed: {
    buttonColor () {
      return this.$q.screen.lt.md
        ? 'primary'
        : 'secondary'
    }
  }
}
```

我们也可以在Vue组件之外使用屏幕插件：

```js
import { Screen } from 'quasar'

// Screen.gt.md
// Screen.md
// Screen.name ('xs', 'sm', ...; Quasar v1.5.2+)
```

# [动画](http://www.quasarchs.com/options/animations#Introduction)

CSS过渡可以由[Vue过渡组件](https://vuejs.org/v2/guide/transitions.html)处理。 过渡效果用于展示输入（出现）或离开（消失）动画。

但是，Quasar可以提供大量即用型CSS动画。 动画效果来自[Animate.css](https://daneden.github.io/animate.css/)。 因此，目前有12个常规，32个输入（In）和32个离开（Out）动画类型可供您开箱即用。 查看Animate.css网站上的列表或此页面的展示的DEMO。

> 请参阅[Vue](https://vuejs.org/v2/guide/transitions.html)文档，了解如何使用Vue提供的`<transition>` 组件。

## 安装

编辑 `/quasar.conf.js`.

```js
// embedding all animations
animations: 'all'

// or embedding only specific animations
animations: [
  'bounceInLeft',
  'bounceOutRight'
]
```

如果您正在构建一个网站，您也可以跳过配置quasar.conf.js并使用指向Animate.css的CDN链接（以下仅为示例，Google为最新链接）。 请记住，这需要为您的用户提供Internet连接，而不是从quasar.conf.js中进行捆绑。

```html
<!-- src/index.template.html -->
<head>
  ...

  <!-- CDN example for Animate.css -->
  <link
    rel="stylesheet"
    href="https://cdnjs.cloudflare.com/ajax/libs/animate.css/3.5.2/animate.min.css"
  >
</head>
```

## 用法

注意实际动画名称前面的字符串“animated”。

```html
<!-- Example with wrapping only one DOM element / component -->
<transition
  appear
  enter-active-class="animated fadeIn"
  leave-active-class="animated fadeOut"
>
  <!-- Wrapping only one DOM element, defined by QBtn -->
  <q-btn
    color="secondary"
    icon="mail"
    label="Email"
  />
</transition>
```



### 包装多个元素

您还可以在过渡中对组件或DOM元素进行分组，以便同时将相同的效果应用于所有这些元素。

```html

<!-- Example with wrapping multiple DOM elements / components -->
<transition-group
  appear
  enter-active-class="animated fadeIn"
  leave-active-class="animated fadeOut"
>
  <!-- We wrap a "p" tag and a QBtn -->
  <p key="text">
     Lorem Ipsum
  </p>
  <q-btn
    key="email-button"
    color="secondary"
    icon="mail"
    label="Email"
  />
</transition-group>
```

请注意以上示例中的一些内容：

1. 注意使用`<transition-group>`而不是`<transition>`。
2. 必须键入组件和DOM元素，例如上面示例中的`key="text"`或`key="email-button"`。
3. 上面的两个例子都指定了布尔属性`appear`，这使得在渲染组件后立即进入动画。 此属性是可选的。

# [Quasar组件过渡效果](http://www.quasarchs.com/options/transitions#Introduction)

有一些Quasar组件通过 `transition-show`/`transition-hide` 或`transition-prev`/`transition-next`或简单的`transition`属性提到过渡。 我们将在这里展示这些过渡效果。

# 全局事件总线

有时您需要一个事件总线或发布/订阅通道。 Vue已经为每个组件提供了一个事件总线。 为了方便起见，您可以使用根Vue组件通过`this.$root`来注册并监听事件。

WARNING

不要与Quasar组件支持的事件混淆。 这些是由各个组件发出的Vue事件，并且不会干扰全局事件总线。

> 考虑使用[Vuex](https://vuex.vuejs.org/)而不是事件总线。

## Usage

请查看Vue的 [实例方法/事件](https://vuejs.org/v2/api/#Instance-Methods-Events)API页面。 然后让我们看看如何在应用程序的根Vue组件上注册一个事件：

```js
// callback
function cb (msg) {
  console.log(msg)
}

// listen for an event
this.$root.$on('event_name', cb)

// listen once (only) for an event
this.$root.$once('event_name', cb)

// Make sure you stop listening for an event
// when your respective component gets destroyed
this.$root.$off('event_name', cb)


// Emitting an event:
this.$root.$emit('event_name', 'some message')
```

Example using event to open drawer from another component or page. Not recommended – a better way would be through [Vuex](https://vuex.vuejs.org/), but the example below is for educational purposes only.

从另一个组件或页面打开侧滑菜单的使用事件的示例。 不推荐 - 更好的方法是通过[Vuex](https://vuex.vuejs.org/)，但下面的示例仅用于教育目的。

```js
// (1) This code is inside layout file that have a drawer
//     if this.leftDrawerOpen is true, drawer is displayed

// (2) Listen for an event in created
created () {
  this.$root.$on('openLeftDrawer', this.openLeftDrawerCallback)
},

beforeDestroy () {
  // Don't forget to turn the listener off before your component is destroyed
  this.$root.$off('openLeftDrawer', this.openLeftDrawerCallback)
}

methods: {
  // (3) Define the callback in methods
  openLeftDrawerCallback () {
    this.leftDrawerOpen = !this.leftDrawerOpen
  }
}

// (4) In another component or page, emit the event!
//     Call the method when clicking button etc.
methods: {
  openLeftDrawer () {
    this.$root.$emit('openLeftDrawer')
  }
}
```

### 用于QDialog和QMenu

这些组件使用Quasar Portals，从而可以在`<body>`标记的末尾呈现内容，以便： 1.避免css污染 2.避免z-index问题 3.避免可能的父CSS溢出 4.在iOS上正常工作

如果需要在这些组件中使用总线，则必须通过.js文件创建自己的全局总线：

```js
import Vue from 'vue'
const bus = new Vue()
export default bus
```

然后在需要访问此总线的任何位置导入此文件。

