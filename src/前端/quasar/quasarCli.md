# QuasarCli

## 安装

```bash
npm install -g @quasar/cli
quasar create <folder_name>
 npx quasar dev
```

在你的`package.json`中添加几个npm脚本的例子：

```js
// package.json
"scripts": {
  "dev": "quasar dev",
  "build": "quasar build",
  "build:pwa": "quasar build -m pwa"
}
```





# 配置quasar.conf.js

那么你可以通过`/quasar.conf.js`来配置什么？

- 您将在您的网站/应用程序中使用的Quasar组件、指令和插件
- 默认的[Quasar语言包](http://www.quasarchs.com/options/quasar-language-packs)
- 你想使用的[图标库](http://www.quasarchs.com/options/installing-icon-libraries)
- Quasar组件的默认的[Quasar图标集](http://www.quasarchs.com/options/quasar-icon-sets)
- 开发服务器端口、HTTPS模式、主机名等
- 你想使用的[CSS动画](http://www.quasarchs.com/options/animations)
- [启动文件](http://www.quasarchs.com/quasar-cli/cli-documentation/boot-files) 列表（也决定了执行顺序) - 这是`/src/boot`中的文件，告诉你在安装根Vue组件之前如何初始化应用程序
- bundle中包含的全局CSS/Stylus/…文件
- PWA [manifest](http://www.quasarchs.com/quasar-cli/developing-pwa/configuring-pwa#配置Manifest文件) 和 [Workbox选项](http://www.quasarchs.com/quasar-cli/developing-pwa/configuring-pwa#Quasar.conf.js)
- [Electron打包器](http://www.quasarchs.com/quasar-cli/developing-electron-apps/configuring-electron#Quasar.conf.js) 和/或 [Electron构建器](http://www.quasarchs.com/quasar-cli/developing-electron-apps/configuring-electron#Quasar.conf.js)
- IE11 +支持
- 扩展的Webpack配置

> 您会注意到，更改任何这些设置不需要您手动重新加载开发服务器。 Quasar检测是否可以通过[热模块更换](https://webpack.js.org/concepts/hot-module-replacement/) 注入更改，如果不能，则会自动重新加载开发服务器。 您不会丢失开发流程，因为您只需坐等Quasar CLI快速重新加载更改的代码，甚至保持当前状态。 这节省了大量的时间！



WARNING

`/quasar.conf.js`由Quasar CLI构建系统运行，因此这些配置代码直接在Node下运行，而不是在应用程序的上下文中运行。这意味着你可以导入像’fs’，‘path’，'webpack’等模块。确保您在此文件编写的ES6功能受安装的Node版本（应该>=8.9.0)支持。



## 结构

您会注意到`/quasar.conf.js`会导出一个函数，该函数接受`ctx`(context)参数并返回一个对象。这使您可以根据此上下文动态更改您的网站/应用配置：

```js
module.exports = function (ctx) {
  console.log(ctx)

  // 输出到控制台的例子:
  {
    dev: true,
    prod: false,
    mode: { spa: true },
    modeName: 'spa',
    target: {},
    targetName: undefined,
    arch: {},
    archName: undefined,
    debug: undefined
  }

  // 根据这些参数上下文将会被创建
  // 当你运行"quasar dev"或"quasar build"时
}
```

这意味着，作为一个例子，您可以在构建特定模式（如PWA）时加载字体，并为其他模式选择另一个：

```js
module.exports = function (ctx) {
  extras: [
    ctx.mode.pwa // we're adding only if working on a PWA
      ? 'roboto-font'
      : null
  ]
}
```

或者，您可以使用一个全局CSS文件用于SPA模式，使用另一个用于Cordova模式，同时避免为其他模式加载任何此类文件。

```js
module.exports = function (ctx) {
  css: [
    ctx.mode.spa ? 'app-spa.styl' : null, // looks for /src/css/app-spa.styl
    ctx.mode.cordova ? 'app-cordova.styl' : null  // looks for /src/css/app-cordova.styl
  ]
}
```

或者，您可以将开发服务器配置为在端口8000上运行SPA模式，在端口9000上运行PWA模式或在端口9090上运行其他模式：

```js
module.exports = function (ctx) {
  devServer: {
    port: ctx.mode.spa
      ? 8000
      : (ctx.mode.pwa ? 9000 : 9090)
  }
}
```

## 配置选项

让我们逐个采取每个选项：

| 属性          | 类型          | 描述                                                         |
| :------------ | :------------ | :----------------------------------------------------------- |
| css           | Array         | 来自/src/css/的全局CSS/Stylus/…文件，默认包含的主题文件除外。 |
| preFetch      | Boolean       | 启用[PreFetch功能](http://www.quasarchs.com/quasar-cli/cli-documentation/prefetch-feature). |
| extras        | Array         | 从[@quasar/extras](https://github.com/quasarframework/quasar/tree/dev/extras)包中导入什么内容。 例： *[‘material-icons’, ‘roboto-font’, ‘ionicons-v4’]* |
| vendor        | Object        | 向vendor块添加/删除文件/第三方库： { add: […], remove: […] }. |
| supportIE     | Boolean       | 增加IE11+支持.                                               |
| htmlVariables | Object        | 添加可在index.template.html中使用的变量。                    |
| framework     | Object/String | 导入哪个Quasar组件/指令/插件，选择哪个Quasar语言包,使用Quasar组件的哪个Quasar图标集。 |
| animations    | Object/String | 导入哪个[CSS动画](http://www.quasarchs.com/options/animations)。 例： *[‘bounceInLeft’, ‘bounceOutRight’]* |
| devServer     | Object        | Webpack开发服务器[选项](https://webpack.js.org/configuration/dev-server/)。 根据您使用的Quasar模式覆盖某些属性，以确保正确的配置。注意：如果您要代理开发服务器（即使用云IDE），请将“public”设置为你的公共应用程序URL。 |
| build         | Object        | 构建配置。                                                   |
| sourceFiles   | Object        | 更改应用部分的默认名称.                                      |
| cordova       | Object        | Cordova特定[配置](http://www.quasarchs.com/quasar-cli/developing-cordova-apps/configuring-cordova)。 |
| capacitor     | Object        | Quasar CLI Capacitor特定[配置](http://www.quasarchs.com/quasar-cli/developing-capacitor-apps/configuring-capacitor)。 |
| pwa           | Object        | PWA特定[配置](http://www.quasarchs.com/quasar-cli/developing-pwa/configuring-pwa)。 |
| ssr           | Object        | SSR特定[配置](http://www.quasarchs.com/quasar-cli/developing-ssr/configuring-ssr). |
| electron      | Object        | Electron特定[配置](http://www.quasarchs.com/quasar-cli/developing-electron-apps/configuring-electron)。 |

## example

### 属性：css

来自`/src/css/`的全局CSS/Stylus/…文件，默认包含的主题文件除外。

```js
// quasar.conf.js
return {
  css: [
    'app.styl', // referring to /src/css/app.styl
    '~some-library/style.css' // referring to node_modules/some-library/style.css
  ]
}
```

### 属性：vendor

默认情况下，出于性能和缓存原因，来自`node_modules`的所有内容都将注入到vendor块中。 但是，如果您希望在此特殊块中添加或删除某些内容，您可以这样做：

```js
// quasar.conf.js
return {
  vendor: {
    /* 可选的; @quasar/app v1.4.2+; 
       禁用vendor块: */ disable: true,

    add: ['src/plugins/my-special-plugin'],
    remove: ['axios', 'vue$']
  }
}
```

### 属性：framework

告诉CLI要导入的Quasar组件/指令/插件，要使用的Quasar I18n语言包，用于Quasar组件的图标集等等。

```js
// quasar.conf.js
return {
  // a list with all options (all are optional)
  framework: {
    components: ['QBtn', 'QIcon' /* ... */],
    directives: ['TouchSwipe' /* ... */],
    plugins: ['Notify' /* ... */],

    // Quasar config
    // You'll see this mentioned for components/directives/plugins which use it
    config: { /* ... */ },

    iconSet: 'fontawesome', // requires icon library to be specified in "extras" section too,
    lang: 'de', // Tell Quasar which language pack to use for its own components

    cssAddon: true // Adds the flex responsive++ CSS classes (noticeable bump in footprint)
  }
}
```

更多关于cssAddon参考[这里](http://www.quasarchs.com/layout/grid/introduction-to-flexbox#Flex-Addons).

### 自动导入功能

@quasar/app v1.1.1+

quasar v1.1.2+

您还可以通过`framework：{all}`属性将Quasar CLI配置为自动导入正在使用的Quasar组件和指令：

```js
// quasar.conf.js
framework: {
  // Possible values for "all":
  // * 'auto' - Auto-import needed Quasar components & directives
  //            (slightly higher compile time; next to minimum bundle size; most convenient)
  // * false  - Manually specify what to import
  //            (fastest compile time; minimum bundle size; most tedious)
  // * true   - Import everything from Quasar
  //            (not treeshaking Quasar; biggest bundle size; convenient)
  all: 'auto',
```

如果您设置`all: 'auto'`， **那么Quasar将自动为您导入组件和指令**。 编译时间将略有增加，但是您无需在quasar.conf.js中指定组件和指令。 **请注意，仍需要指定Quasar插件。**

从`@quasar/app` v1.1.2（以及`quasar` v1.1.3 +）开始，使用自动导入功能时，您还可以配置编写组件的方式：

```js
// quasar.conf.js
framework: {
  all: 'auto',
  autoImportComponentCase: 'pascal' // or 'kebab' (default) or 'combined'
```

### 属性：devServer

**Webpack devServer 选项**. 看看[完整列表](https://webpack.js.org/configuration/dev-server/)的选项。 有些被Quasar CLI根据“Quasar dev”参数和Quasar模式覆盖，以确保正确的设置。 注意：如果您要代理开发服务器（即使用云IDE），请将“public”设置为公共应用程序URL。

大多数使用的属性是：

| 属性   | 类型           | 描述                                                         |
| :----- | :------------- | :----------------------------------------------------------- |
| port   | Number         | dev server端口                                               |
| host   | String         | dev server使用的本地IP/主机名                                |
| open   | Boolean/String | 除非将其设置为“false”，否则Quasar将打开一个自动指向开发服务器地址的浏览器。 适用于SPA，PWA和SSR模式。 如果指定String，请参阅下面的说明。 |
| public | String         | 应用程序的公共地址（用于反向代理）                           |

使用`open`属性打开特定浏览器，而不是使用操作系统的默认浏览器（基于主机操作系统检查[支持的值](https://github.com/sindresorhus/open/blob/master/test.js)）：

```js
// quasar.conf.js

devServer: {
  open: 'firefox'
}
```

在quasar.conf.js文件中设置`devServer > https: true`时，Quasar会自动为您生成SSL证书。 但是，如果您想自己为本地主机创建一个，请查看[Filippo](https://blog.filippo.io/mkcert-valid-https-certificates-for-localhost/)的博客文章。 然后你的`quasar.conf.js > devServer > https`应该看起来像这样：

```js
// quasar.conf.js

const fs = require('fs')
// ...

devServer: {
  https: {
    key: fs.readFileSync('/path/to/server.key'),
    cert: fs.readFileSync('/path/to/server.crt'),
    ca: fs.readFileSync('/path/to/ca.pem'),
  }
}
```

从 **@quasar/app v1.3.2** 开始，您还可以配置自动打开远程Vue Devtools：

```js
// quasar.conf.js

devServer: {
  vueDevtools: true
}
```

### 属性：build

| 属性                        | 类型           | 描述                                                         |
| :-------------------------- | :------------- | :----------------------------------------------------------- |
| transpileDependencies       | Array of Regex | 添加使用Babel进行转换的依赖项（来自node_modules，默认情况下不会被转换）。 例： `[ /my-dependency/, ...]` |
| transformAssetUrls          | Object         | (**@quasar/app 1.3.4+**) 添加对自定义标记属性的引用资源的支持。例如： `{ 'my-img-comp': 'src', 'my-avatar': [ 'src', 'placeholder-src' ]}` |
| showProgress                | Boolean        | 编译时显示进度条。                                           |
| extendWebpack(cfg)          | Function       | Quasar CLI生成的扩展Webpack配置。 等同于chainWebpack()，但您可以直接访问Webpack配置对象。 |
| chainWebpack(chain)         | Function       | Quasar CLI生成的扩展Webpack配置。 等同于extendWebpack()，但改为使用webpack-chain。 |
| beforeDev({ quasarConf })   | Function       | 在运行`$ quasar dev`命令之前准备外部服务，比如启动一些后端或应用所依赖的任何其他服务。 可以使用async/await或直接返回Promise。 |
| afterDev({ quasarConf })    | Function       | quasar开发服务器启动后（`$ quasar dev`）运行钩子。 此时，开发服务器已启动，如果您希望对其执行某些操作则可用这个方法。 可以使用async/await或直接返回Promise。 |
| beforeBuild({ quasarConf }) | Function       | 在Quasar构建用于生产环境的应用（`$ quasar build`）之前运行钩子。 此时，尚未创建redistributables文件夹。 可以使用async/await或直接返回Promise。 |
| afterBuild({ quasarConf })  | Function       | 在Quasar构建用于生产环境的应用（`$ quasar build`）之后运行钩子。 此时，distributables文件夹已创建，如果您希望对其执行某些操作，则可用。 可以使用async/await或直接返回Promise。 |
| onPublish(opts)             | Function       | 在Quasar构建用于生产环境的应用并执行afterBuild挂钩（如果指定）之后，如果请求发布（`$ quasar build -P`），则运行挂钩。 可以使用async/await或直接返回Promise。 `opts`是`{arg, distDir}`形式的对象，其中“arg”是提供给-P的参数（如果有的话）。 |
| publicPath                  | String         | 部署时的公共路径。                                           |
| forceDevPublicPath          | Boolean        | (**@quasar/app 1.0.6+**) 也在开发版本中强制使用自定义publicPath（仅适用于SPA和PWA模式）。 请确保这确实是您要查找的内容，并且您知道自己在做什么，否则不建议这样做。 |
| appBase                     | String         | (**@quasar/app 1.4.2+**) 使用您的自定义值强制应用基本标签；仅在您**确实**知道自己在做什么的情况下进行配置，否则您可以轻松破坏应用程序。 强烈建议您保留由quasar/app计算的结果。 |
| vueRouterBase               | String         | (**@quasar/app 1.4.2+**) 用您的自定义值强制应用vue router base；仅在您**确实**知道自己在做什么的情况下进行配置，否则您可以轻松破坏应用程序。 强烈建议您保留由quasar / app计算的结果。 |
| vueRouterMode               | String         | 设置[Vue路由器模式](https://router.vuejs.org/en/essentials/history-mode.html)：‘hash’或’history’。 请明智选择。 历史记录模式也需要在部署Web服务器上进行配置。 |
| htmlFilename                | String         | 默认是’index.html’.                                          |
| productName                 | String         | 默认值取自package.json> productName字段。                    |
| distDir                     | String         | Quasar CLI生成可分发包的目录，对应项目根目录的相对路径。 默认是’dist/{ctx.modeName}’。 适用于除Cordova (强制生成到src-cordova/www目录)以外的所有模式。 |
| devtool                     | String         | Source map[策略](https://webpack.js.org/configuration/devtool/)使用。 |
| env                         | Object         | 将属性添加到`process.env`，您可以在您的网站/应用程序JS代码中使用它。 每个属性都需要JSON编码。 例如：{SOMETHING：JSON.stringify(‘someValue’)}。 |
| gzip                        | Boolean        | 使用Gzip压缩可分发包。 当您提供内容的网络服务器没有gzip功能时很有用。 |
| scopeHoisting               | Boolean        | 默认值：“true”。 使用 Webpack范围提升功能 来获得稍微更好的运行时性能。 |
| analyze                     | Boolean/Object | 使用webpack-bundle-analyzer显示构建包的分析。 如果用作对象，则表示webpack-bundle-analyzer配置对象。 |
| vueCompiler                 | Boolean        | 包括vue runtime + compiler版本，而不是默认的Vue运行时版本    |
| uglifyOptions               | Object         | 缩小选项。 [完整清单](https://github.com/webpack-contrib/terser-webpack-plugin/#minify). |
| preloadChunks               | Boolean        | 默认为“true”。 浏览器空闲时预加载块以改善用户以后导航到其他页面的体验。 |
| scssLoaderOptions           | Object         | 为`.scss`文件提供`sass-loader`的选项。                       |
| sassLoaderOptions           | Object         | 为`.sass`文件提供`sass-loader`的选项。                       |
| stylusLoaderOptions         | Object         | 提供给’stylus-loader`的选项.                                 |
| lessLoaderOptions           | Object         | 提供给’less-loader`的选项。                                  |

Quasar CLI根据dev/build命令和Quasar模式自动配置`build`的以下属性。 但是如果你想重写一些（确保你知道你在做什么)，你可以这样做：

| 属性            | 类型    | 描述                                                         |
| :-------------- | :------ | :----------------------------------------------------------- |
| extractCSS      | Boolean | 从Vue文件中提取CSS                                           |
| sourceMap       | Boolean | 使用 source maps                                             |
| minify          | Boolean | 压缩代码（html，js，css）                                    |
| webpackManifest | Boolean | 改进缓存策略。 使用一个webpack清单文件来避免在每个版本的vendor块上更改散列导致缓存崩溃。 |

例如，如果运行“quasar build --debug”，则无论您配置了什么，sourceMap和extractCSS都将设置为“true”。

### 属性：htmlVariables

您可以在`src/index.template.html`中定义然后引用变量，如下所示：

```js
// quasar.conf.js
module.exports = function (ctx) {
  return {
    htmlVariables: { title: 'test name' }
```

然后（只是一个示例，向您展示如何引用上面定义的变量，在本例中为`title`）：

```html
<!-- src/index.template.html -->
<%= htmlWebpackPlugin.options.title %>
```

### 属性：sourceFiles

如果必须，请使用此属性更改网站/应用程序的某些文件的默认名称。 所有路径必须相对于项目的根文件夹。

```js
// default values:
sourceFiles: {
  rootComponent: 'src/App.vue',
  router: 'src/router',
  store: 'src/store',
  indexHtmlTemplate: 'src/index.template.html',
  registerServiceWorker: 'src-pwa/register-service-worker.js',
  serviceWorker: 'src-pwa/custom-service-worker.js',
  electronMainDev: 'src-electron/main-process/electron-main.dev.js',
  electronMainProd: 'src-electron/main-process/electron-main.js'
}
```

### 为dev/build设置env的示例

```js
build: {
  env: ctx.dev
    ? { // 在开发状态下我们拥有以下属性
      API: JSON.stringify('https://dev.api.com')
    }
    : { // 在构建状态（生产版本）下
      API: JSON.stringify('https://prod.api.com')
    }
}
```

然后，在您的网站/应用程序中，您可以访问`process.env.API`，它将根据开发或生产构建类型指向上述两个链接中的一个。

你甚至可以更进一步。 提供来自`quasar dev/build` env变量的值：

```js
# 我们在终端设置一个env变量
$ MY_API=api.com quasar build

# 然后我们在/quasar.conf.js获取它
build: {
  env: ctx.dev
    ? { // 在开发状态下我们拥有以下属性
      API: JSON.stringify('https://dev.'+ process.env.MY_API)
    }
    : { // 在构建状态（生产版本）下
      API: JSON.stringify('https://prod.'+ process.env.MY_API)
    }
}
```

> 或者你可以使用我们的[@quasar/dotenv](https://github.com/quasarframework/app-extension-dotenv)或[@quasar/qenv](https://github.com/quasarframework/app-extension-qenv)应用扩展。

TIP

另请参阅[处理process.env](http://www.quasarchs.com/quasar-cli/cli-documentation/handling-process-env)页面。

### 处理Webpack配置

深入分析[处理Webpack](http://www.quasarchs.com/quasar-cli/cli-documentation/handling-webpack)文档页面。



# [应用图标](http://www.quasarchs.com/quasar-cli/app-icons#Introduction)

如果您的目标是Quasar当前支持的所有平台，则需要制作大约80种不同的包含4种不同媒体类型（png，ico，icns和svg）的文件。 如果您只使用像Gimp，Photoshop或Affinity Designer这样的工具，您会发现这些文件相当大，制作它们并命名它们的过程容易出现操作错误。 您可能希望至少压缩PNG文件，并从SVG中删除不必要的应用元数据。

此页面记录了每个构建目标所需的所有图标



# [测试与审核](http://www.quasarchs.com/quasar-cli/testing-and-auditing#Introduction)

您的Quasar项目能够添加单元和e2e测试工具，以及不断增长的产品质量审计工具套件。这篇介绍不会详细介绍如何编写和使用测试，为此请参考[GitHub上的测试报告](https://github.com/quasarframework/quasar-testing)中特别准备和维护的文档。如果您是初学者，请考虑阅读“进一步阅读”章节中的其中一本书。



# [cli文档](http://www.quasarchs.com/quasar-cli/cli-documentation/directory-structure#Introduction)

## 目录结构

TIP

如果你是初学者，你需要关心的是 `/quasar.conf.js` (Quasar应用配置文件)、`/src/router`、 `/src/layouts`、 `/src/pages` 以及可选的 `/src/assets`。

```bash
.
├── src/
│   ├── assets/              # 动态资源（由webpack处理）
│   ├── statics/             # 纯静态资源（直接复制）
│   ├── components/          # 用于页面和布局的.vue组件
│   ├── css/                 # CSS/Stylus/Sass/...文件
|   |   ├── app.styl
|   │   └── quasar.variables.styl # 供您调整的Quasar Stylus变量
│   ├── layouts/             # 布局 .vue 文件
│   ├── pages/               # 页面 .vue 文件
│   ├── boot/                # 启动文件 (app initialization code) 
│   ├── router/              # Vue路由
|   |   ├── index.js         # Vue路由定义
|   │   └── routes.js        # App路由定义
│   ├── store/               # Vuex Store
|   |   ├── index.js         # Vuex Store 定义
|   │   ├── <folder>         # Vuex Store 模块...
|   │   └── <folder>         # Vuex Store 模块...
│   ├── App.vue              # APP的根Vue组件
│   └── index.template.html  # index.html模板
├── src-ssr/                 # SSR特定代码(就像生产环境的Node网页服务器)
├── src-pwa/                 # PWA特定代码（如Service Worker）
├── src-cordova/             # Cordova生成的文件夹用于创建移动APP
├── src-electron/            # Electron特定代码（如"main"线程)
├── dist/                    # 生产版本代码，用于部署
│   ├── spa/                 # 构建SPA的例子
│   ├── ssr/                 # 构建SSR的例子
│   ├── electron/            # 构建Electron的例子
│   └── ....
├── quasar.conf.js           # Quasar App配置文件
├── babel.config.js          # Babeljs配置
├── .editorconfig            # editor配置
├── .eslintignore            # ESlint忽略路径
├── .eslintrc.js             # ESlint配置
├── .postcssrc.js            # PostCSS配置
├── .stylintrc               # Stylus lint配置
├── .gitignore               # GIT忽略路径
├── package.json             # npm脚本和依赖项
└── README.md                # 您的网站/应用程序的自述文件
```

## [构建命令](http://www.quasarchs.com/quasar-cli/cli-documentation/build-commands#Introduction)



## CSS预处理



如果想使用**Sass**或**SCSS**（推荐这两种中的任何一种）和**Stylus**，它们是通过Quasar CLI开箱即用的css预处理器。

您无需安装任何其他软件包或扩展Webpack配置。

WARNING

为了获得完整的Sass/SCSS支持，您将需要@quasar/app v1.1.0+

### 怎么用

您的Vue文件可以通过`<style>`标签包含Sass/SCSS/Stylus代码。

```html
<!-- 注意lang="sass" -->
<style lang="sass">
div
  color: #444
  background-color: #dadada
</style>
<!-- 注意lang="scss" -->
<style lang="scss">
div {
  color: #444;
  background-color: #dadada;
}
</style>
<!-- 注意lang="stylus" -->
<style lang="stylus">
div
  color #444
  background-color #dadada
</style>
```

而且，当然，还支持标准CSS：

```html
<style>
div {
  color: #444;
  background-color: #dadada;
}
</style>
```

### 变量

Quasar还提供变量（`$primary`, `$grey-3`等），您可以直接使用它们。 阅读有关[Sass/SCSS变量](http://www.quasarchs.com/style/sass-scss-variables)和[Stylus变量](http://www.quasarchs.com/style/stylus-variables)的更多信息。



## [应用路由](http://www.quasarchs.com/quasar-cli/cli-documentation/routing#Introduction)

## 延迟加载

当您的网站/应用程序很小时，您可以将所有布局/页面/组件加载到初始包中，并在启动时提供所有内容。 但是，当您的代码变得复杂时，有大量的布局/页面/组件，这样做并不是最理想的，因为它会影响加载时间。 幸运的是，有一种方法可以解决这个问题。

我们将介绍如何延迟加载/编码拆分应用程序的部分，以便仅在需要时自动请求它们。 这是通过动态导入完成的。 让我们从一个例子开始，然后转换它，以便我们使用延迟加载 - 我们将聚焦这个加载一个页面的例子，但同样的原则可以应用于加载任何东西（资源、JSONs、…）：



### 延迟加载路由页面

使用Vue-Router调用静态组件是正常的。

```js
import SomePage from 'pages/SomePage'

const routes = [
  {
    path: '/some-page',
    component: SomePage
  }
]
```

现在让我们改变这种方式，并使用动态导入使页面按需加载：

```js
const routes = [
  {
    path: '/some-page',
    component: () => import('pages/SomePage')
  }
]
```

很简单，对吧？ 它所做的是为`/src/pages/SomePage.vue`创建一个单独的块，只有在需要时才加载。 在这个例子中，指当用户访问’/same-page’的路由的时候。

### 延迟加载组件

通常，您将导入一个组件，然后将其注册到页面、布局或组件。

```html
<script>
import SomeComponent from 'components/SomeComponent'

export default {
  components: {
    SomeComponent,
  }
}
</script>
```

现在让我们改变这种方式，使用动态导入使组件按需加载：

```html
<script>
export default {
  components: {
    SomeComponent: () => import('components/SomeComponent'),
  }
}
</script>
```

### 延迟加载即时生效

正如你在上面注意到的那样，我们使用动态导入（`import('.. resource ..')`）而不是常规导入（`import resource from './path/to/resource'`）。 动态导入基本上返回一个您可以使用的Promise：

```js
import('./categories.json')
  .then(categories => {
    // 嘿, 我们已经延迟加载了这个文件
    // 并且我们有了"categories"中的内容
  })
  .catch(() => {
    // 哦, 哪里出错了...
    // 不能加载资源
  })
```

使用动态导入而不是常规导入的一个优点是导入路径可以在运行时确定：

```js
import('pages/' + pageName + '/' + 'id')
```

### 注意动态导入

在前面的例子中使用可变部分的动态导入时有一点需要注意。 当网站/应用程序被打包，在编译时我们无法知道运行时确切的导入路径。 因此，将为每个可以匹配变量路径的文件创建块。 您可能会在构建日志中看到不必要的文件。

那么我们如何限制在这种情况下创建的块的数量呢？ 方法是尽可能地限制可变部分，因此匹配的路径尽可能少。 1.添加文件扩展名，即使它没有扩展名也能用。 这将仅为该文件类型创建块。 当该文件夹包含许多文件类型时很有用。

```js
// 糟糕
import('./folder/' + pageName)

// 这样更好
import('./folder/' + pageName + '.vue')
```

2.尝试创建一个文件夹结构，以限制可变路径中的文件。 尽可能具体说明：

```js
// 糟糕 -- 为在./folder中的所有JSON创建块 (递归查询)
const asset = 'my/jsons/categories.json'
import('./folder/' + asset)

// 很好 --仅为在./folder/my/jsons中的JSON创建块
const asset = 'categories.json'
import('./folder/my/jsons/' + asset)
```

3.尝试从仅包含文件的文件夹导入。 以前面的例子为例，假设./folder/my/jsons还包含子文件夹。 我们通过指定更具体的路径来使动态导入更好，但在这种情况下它仍然不是最优的。 最好是使用仅包含文件的终端文件夹，因此我们限制匹配路径的数量。

1. 使用[Webpack魔术注释](https://webpack.js.org/api/module-methods/#magic-comments)的`webpackInclude`和`webpackExclude`通过正则表达式约束捆绑的块，例如：

```js
await import(
  /* webpackInclude: /(ar|en-us|ro)\.js$/ */
  `quasar/lang/${langIso}`
)
  .then(lang => {
    Quasar.lang.set(lang.default)
  })
```

将导致仅捆绑您网站/应用程序所需的语言包，而不是捆绑所有语言包（超过40种！），这可能会妨碍`quasar dev`和`quasar build`.命令的性能。

请记住，匹配路径的数量等于正在生成的组块的数量。



## [资源处理](http://www.quasarchs.com/quasar-cli/cli-documentation/handling-assets#Introduction)



## 启动文件

**在实例化根Vue应用程序实例之前运行代码**

### 解剖一个启动文件

启动文件是一个简单的可以选择导出函数的JavaScript文件。 当启动应用程序时，Quasar将调用导出的函数，并将具有以下属性的**一个对象**传递给该函数：

| 属性名称     | 说明                                                         |
| :----------- | :----------------------------------------------------------- |
| `app`        | 根组件通过Vue实例化的对象                                    |
| `router`     | 来自’src/router/index.js’的Vue路由器实例                     |
| `store`      | 应用Vuex存储的实例 - **只有当您的项目使用Vuex（您有src/store）时才会传递store** |
| `Vue`        | 和`import Vue from 'vue'`一样，它在那里是为了方便            |
| `ssrContext` | 如果为SSR构建，则仅在服务器端可用                            |
| `urlPath`    | (**@quasar/app 1.0.7+**) URL的路径名（路径+搜索）部分；在客户端（仅在客户端），它也包含哈希值。 |
| `redirect`   | (**@quasar/app 1.0.7+**) 重定向到另一个URL的调用函数。       |

```js
export default ({ app, router, store, Vue }) => {
  // something to do
}
export default async ({ app, router, store, Vue }) => {
  // something to do
  await something()
}

注意我们正在使用ES6解构赋值。只分配你实际需要/使用的东西。
```

### 何时使用启动文件

WARNING

请确保您了解应用插件解决什么问题，以及何时适合使用它们，以避免在不需要它们的情况下应用它们。

- 你的Vue插件有安装说明，就像需要调用`Vue.use()`一样。
- 你的Vue插件需要实例化添加到根实例的数据 - 一个例子是[vue-i18n](https://github.com/kazupon/vue-i18n/)。
- 您想使用`Vue.mixin()`添加全局mixin。
- 您想添加一些东西到Vue原型以方便访问 - 一个例子是在Vue文件中方便地使用`this.$axios`而不是在每个这样的文件中导入Axios。
- 你想干涉路由器 - 一个例子是使用`router.beforeEach`进行认证
- 你想干涉Vuex存储实例 - 一个例子是使用`vuex-router-sync`软件包
- 配置库的方面 - 一个例子是创建一个带有基本URL的Axios实例;你可以将它注入到Vue原型中和/或导出它（这样你就可以从应用程序中的任何其他地方导入实例）

### 不需要使用启动文件的示例

- 对于像Lodash这样的普通JavaScript库，在使用之前不需要任何初始化。例如，Lodash可能只有在你想注入Vue原型时(例如可以在你的Vue文件中使用`this.$_`)用作启动文件才有意义。

### 使用启动文件

第一步总是使用Quasar CLI生成一个新的启动文件：

```bash
$ quasar new boot <name>
```



其中`<name>`应该替换为您的启动文件的合适名称。

这个命令创建一个新文件：`/src/boot/<name>.js`包含以下内容：

```js
// import something here

// "async" is optional
// remove it if you don't need it
export default async ({ /* app, router, store, Vue */ }) => {
  // something to do
}
```

You can also return a Promise:

```js
// import something here

export default ({ /* app, router, store, Vue */ }) => {
  return new Promise((resolve, reject) => {
    // do something
  })
}
```

最后一步是告诉Quasar使用你的新启动文件。 为了做到这一点，你需要在`/quasar.conf.js`中添加启动文件

```js
boot: [
  // references /src/boot/<name>.js
  '<name>'
]
```

构建SSR应用程序时，您可能希望某些启动文件仅在服务器上运行或仅在客户端上运行，在这种情况下，您可以执行以下操作：

```js
boot: [
  {
    server: false, // run on client-side only!
    path: '<name>' // references /src/boot/<name>.js
  },
  {
    client: false, // run on server-side only!
    path: '<name>' // references /src/boot/<name>.js
  }
]
```

如果要从node_modules指定启动文件，可以通过在路径前加上`~`（波浪号）字符来实现：

```js
boot: [
  // boot file from an npm package
  '~my-npm-package/some/file'
]
```

如果您希望仅针对特定的构建类型将启动文件注入您的应用程序：

```js
boot: [
  ctx.mode.electron ? 'some-file' : ''
]
```

### 重定向到另一个页面

@quasar/app 1.0.7+

```js
export default ({ urlPath, redirect }) => {
  // ...
  const isAuthorized = // ...
  if (!isAuthorized && !urlPath.startsWith('/login')) {
    redirect('/login')
    return
  }
  // ...
}
```



如前几节所述，引导文件的默认导出可以返回Promise。 如果此Promise被包含“url”属性的对象拒绝，则Quasar CLI会将用户重定向到该URL：

```js
export default ({ urlPath }) => {
  return new Promise((resolve, reject) => {
    // ...
    const isAuthorized = // ...
    if (!isAuthorized && !urlPath.startsWith('/login')) {
      reject({ url: '/login' })
      return
    }
    // ...
  })
}
```

或更简单的等效代码：

```js
export default () => {
  // ...
  const isAuthorized = // ...
    if (!isAuthorized && !urlPath.startsWith('/login')) {
    return Promise.reject({ url: '/login' })
  }
  // ...
}
```



### Quasar应用程序流程

为了更好地理解启动文件的功能和用途，您需要了解您的网站/应用程序是如何启动的：

1. Quasar已初始化（组件、指令、插件、Quasar i18n、Quasar图标集）
2. Quasar Extras被导入（Roboto字体 - 如果使用，图标，动画…）
3. Quasar CSS和您的应用程序的全局CSS已导入
4. App.vue被加载（尚未被使用）
5. Store被导入（如果在src/store中使用Vuex存储）
6. 启动文件已导入
7. 启动文件会执行其默认导出功能 7.（如果在Electron模式下）Electron 被导入并注入Vue原型 8.（如果在Cordova模式下）收听“deviceready”事件，然后继续执行以下步骤
8. 使用根组件实例化Vue并附加到DOM

### 启动文件的例子

#### Axios

```js
import Vue from 'vue'
import axios from 'axios'

// we add it to Vue prototype
// so we can reference it in Vue files
// without the need to import axios
Vue.prototype.$axios = axios

// Example: this.$axios will reference Axios now so you don't need stuff like vue-axios
```

#### vue-i18n

```js
import Vue from 'vue'
// 导入外部包
import VueI18n from 'vue-i18n'

// 包含语言包的/src/i18n中一个文件
import messages from 'src/i18n'


// 告诉Vue使用我们的Vue包:
Vue.use(VueI18n)
export default ({ app, Vue }) => {
  // 在应用中设置i18n实例;
  // 我们通过这样做将它注入到根组件;
  // new Vue({..., i18n: ... }).$mount(...)

  app.i18n = new VueI18n({
    locale: 'en',
    fallbackLocale: 'en',
    messages
  })
}
```



#### 路由验证

一些插件可能需要干涉Vue路由器配置：

```js
export default ({ router, store, Vue }) => {
  router.beforeEach((to, from, next) => {
    //现在您需要在这里添加验证逻辑，比如调用一个API
  })
}
```

### 从启动文件访问数据

有时，您想访问您在启动文件中配置的数据，这些数据在您无权访问根Vue实例的文件中。

幸运的是，因为启动文件只是普通的JavaScript文件，所以您可以根据需要将任意数量的导出添加到您的启动文件。

以Axios为例。 有时候你想要在你的JavaScript文件中访问你的Axios实例，但是你不能访问根Vue实例。 为了解决这个问题，你可以在你的启动文件中导出Axios实例并将其导入到别处。

考虑下面的axios启动文件：

```js
// axios启动文件(src/boot/axios.js)

import Vue from 'vue'
import axios from 'axios'

// 我们创建我们自己的axios实例并设置一个自定义的基本URL。
// 请注意，如果我们不在这里设置任何配置，我们不需要
// 一个命名的导出，因为我们可以`import axios from 'axios'`
const axiosInstance = axios.create({
  baseURL: 'https://api.example.com'
})

// 在Vue文件中通过this.$axios来使用
Vue.prototype.$axios = axiosInstance

// 这里我们定义一个命名的导出，
// 然后我们后面可以使用这个内部的.js文件:
export { axiosInstance }
```

在任何JavaScript文件中，您都可以像这样导入axios实例:

```js
// 我们从src/boot/axios.js中导入一个命名的导出
import { axiosInstance } from 'boot/axios'
```

## 预取（PreFetch）功能

预取是一项功能（**仅在使用Quasar CLI**时可用），它允许Vue路由(在`/src/router/routes.js`定义)获取的组件去：

- 预取数据
- 验证路由
- 当某些条件不满足时（如用户未登录），重定向到另一条路由
- 可以帮助初始化存储状态

以上所有内容都将在实际路由组件呈现之前运行。

**它适用于所有Quasar模式**（SPA、PWA、SSR、Cordova、Electron），但它对SSR构建特别有用。



### 安装

```js
// quasar.conf.js
return {
  preFetch: true
}
```

> WARNING
>
> 当您使用它来预取数据时，您需要使用Vuex存储，因此在创建项目时请确保您的项目文件夹具有`/src/store`文件夹，否则生成新项目并复制store文件夹内容到当前项目。

### 预取功能激活场景

`preFetch`钩子（在下一节中描述）由访问的路由决定 - 它也决定了渲染的组件。实际上，给定路由所需的数据也是在该路由上渲染的组件所需的数据。 **因此将钩子逻辑仅置于路由组件内是很自然的（也是必需的）**。 这包括`/src/App.vue`，在这种情况下，它只会在app启动时运行一次。

让我们举一个例子来了解何时调用钩子。假设我们有这些路由，并且我们为所有这些组件编写了`preFetch`钩子：



```js
// routes
[
  {
    path: '/',
    component: LandingPage
  },
  {
    path: '/shop',
    component: ShopLayout,
    children: [
      {
        path: 'all',
        component: ShopAll
      },
      {
        path: 'new',
        component: ShopNew
      },
      {
        path: 'product/:name',
        component: ShopProduct,
        children: [{
          path: 'overview',
          component: ShopProductOverview
        }]
      }
    ]
  }
]
```

现在，让我们看看当用户一个接一个地按照下面指定的顺序访问这些路由时如何调用钩子。



| 正在访问的路由                 | 调用的钩子                         | 观察                                                         |
| :----------------------------- | :--------------------------------- | :----------------------------------------------------------- |
| `/`                            | App.vue然后登陆页面                | 自我们的应用程序启动以来，就调用了App.vue挂钩。              |
| `/shop/all`                    | ShopLayout然后ShopAll              | -                                                            |
| `/shop/new`                    | ShopNew                            | ShopNew是ShopLayout的子项，ShopLayout已经渲染，因此不再调用ShopLayout。 |
| `/shop/product/pyjamas`        | ShopProduct                        | -                                                            |
| `/shop/product/shoes`          | ShopProduct                        | Quasar注意到已经渲染了相同的组件，但是路由已经更新并且它有路由参数，所以它再次调用了钩子。 |
| `/shop/product/shoes/overview` | ShopProduct然后ShopProductOverview | ShopProduct具有路由参数，因此即使已经渲染它也会被调用。      |
| `/`                            | 登陆页面                           |                                                              |

### 用法

钩子被定义为我们的路由组件上名为`preFetch`的自定义静态函数。请注意，因为在实例化组件之前将调用此函数，所以它无法访问`this`。

```html
<!-- some .vue component used as route -->
<template>
  <div>{{ item.title }}</div>
</template>

<script>
export default {
  // our hook here
  preFetch ({ store, currentRoute, previousRoute, redirect, ssrContext }) {
    // fetch data, validate route and optionally redirect to some other route...

    // ssrContext is available only server-side in SSR mode

    // No access to "this" here as preFetch() is called before
    // the component gets instantiated.

    // Return a Promise if you are running an async job
    // Example:
    return store.dispatch('fetchItem', currentRoute.params.id)
  },

  computed: {
    // display the item from store state.
    item () {
      return this.$store.state.items[this.$route.params.id]
    }
  }
}
</script>
// related action for Promise example
// ...

actions: {
  fetchItem ({ commit }, id) {
    return axiosInstance.get(url, id).then(({ data }) => {
      commit('mutation', data)
    })
  }
}

// ...
```



### 重定向示例

下面是在某些情况下重定向用户的示例，例如当他们尝试访问只有经过身份验证的用户应该看到的页面时。

```js
// We assume here we already wrote the authentication logic
// in the Vuex Store, so take as a high-level example only.
preFetch ({ store, redirect }) {
  if (!store.state.authenticated) {
    // IMPORTANT! Always use the String form of a
    // route if also building for SSR. The Object form
    // won't work on SSR builds.
    redirect('/login')
  }
}
```

### 使用预取功能初始化存储

当应用程序启动时，`preFetch`挂钩只运行一次，因此您可以利用此机会在此处初始化Vuex存储。

```js
// App.vue
export default {
  // ...
  preFetch ({ store }) {
    // initialize something in store here
  }
}
```

### 加载中

还可以使用[加载中](http://www.quasarchs.com/quasar-plugins/loading) 插件。 这是一个例子：

```js

// a route .vue component
import { Loading } from 'quasar'

export default {
  // ...
  preFetch ({ /* ... */ }) {
    Loading.show()

    return new Promise(resolve => {
      // do something async here
      // then call "resolve()"
    }).then(() => {
      Loading.hide()
    })
  }
}
```

## API代理

将项目文件夹（由Quasar CLI创建）与现有后端集成时，通常需要在使用开发服务器时访问后端API。 为此，我们可以并行（或远程）运行开发服务器和API后端，并让开发服务器将所有API请求代理到实际的后端。

如果您在API请求中访问相对路径，这很有用。 显然，这些相对路径可能在您开发时无法正常工作。 为了创建与您部署的网站/应用使用的环境类似的环境，您可以代理您的API请求。

要配置代理规则，编辑`/quasar.conf.js`中的`devServer.proxy`。 有关详细用法，请参阅[Webpack Dev Server Proxy](https://webpack.js.org/configuration/dev-server/#devserver-proxy)文档。 但是这里有一个简单的例子：

```js
// quasar.conf.js

devServer: {
  proxy: {
    // 将所有以/api开头的请求代理到jsonplaceholder
    '/api': {
      target: 'http://some.api.target.com:7070',
      changeOrigin: true,
      pathRewrite: {
        '^/api': ''
      }
    }
  }
}
```

上面的例子将代理请求 `/api/posts/1` 到 `http://some.api.target.com:7070/posts/1`.



## Webpack处理

构建系统使用Webpack创建您的网站/应用程序。 如果您不熟悉Webpack，请不要担心。 因为它开箱即用。您无需对其进行配置，因为它已经设置了一切。

### 与quasar.conf.js一起使用

对于需要调整默认Webpack配置的情况，可以通过编辑`/quasar.conf.js`和配置`build> extendWebpack(cfg)` 方法或 `build > chainWebpack (chain)`来实现。

向Webpack添加ESLint加载器的例子（假设你已经安装了它）：

对于需要调整默认Webpack配置的情况，可以通过编辑`/quasar.conf.js`和配置`build> extendWebpack(cfg)` 方法或 `build > chainWebpack (chain)`来实现。

向Webpack添加ESLint加载器的例子（假设你已经安装了它）：

```js
// quasar.conf.js
build: {
  extendWebpack (cfg, { isServer, isClient }) {
    cfg.module.rules.push({
      enforce: 'pre',
      test: /\.(js|vue)$/,
      loader: 'eslint-loader',
      exclude: /(node_modules|quasar)/,
      options: {
        formatter: require('eslint').CLIEngine.getFormatter('stylish')
      }
    })
  }
}
```

注意你不需要返回任何东西。 extendWebpack（cfg）的参数是由Quasar为您生成的Webpack配置对象。 假设你真的知道你在做什么，你可以添加/删除/替换任何东西。

chainWebpack()的等价quasar.conf：

```js
// quasar.conf.js
build: {
  chainWebpack (chain, { isServer, isClient }) {
    chain.module.rule('eslint')
      .test(/\.(js|vue)$/)
      .enforce('pre')
      .exclude
        .add((/[\\/]node_modules[\\/]/))
        .end()
      .use('eslint-loader')
        .loader('eslint-loader')
  }
}
```



### 检查Webpack配置

Quasar CLI为此提供了一个有用的命令：

```bash
$ quasar inspect -h

  Description
    Inspect Quasar generated Webpack config

  Usage
    $ quasar inspect
    $ quasar inspect -c build
    $ quasar inspect -m electron -p 'module.rules'

  Options
    --cmd, -c        Quasar command [dev|build] (default: dev)
    --mode, -m       App mode [spa|ssr|pwa|cordova|electron] (default: spa)
    --depth, -d      Number of levels deep (default: 5)
    --path, -p       Path of config in dot notation
                        Examples:
                          -p module.rules
                          -p plugins
    --help, -h       Displays this message
```



### Webpack别名

Quasar带有一些预先配置好的Webpack别名。 您可以在项目中的任何位置使用它们，webpack将解析为正确的路径。

| 别名         | 解析为          |
| :----------- | :-------------- |
| `src`        | /src            |
| `app`        | /               |
| `components` | /src/components |
| `layouts`    | /src/layouts    |
| `pages`      | /src/pages      |
| `assets`     | /src/assets     |
| `boot`       | /src/boot       |

另外，如果您配置为使用Vue编译器版本（quasar.conf > build > vueCompiler: true）进行构建，那么`vue$`会解析为`vue/dist/vue.esm.js`。

### 添加Webpack别名

要添加自己的别名，可以扩展webpack配置并将其与现有别名合并。 使用`path.resolve`辅助程序来解析目标别名的路径。

```js
// quasar.conf.js
const path = require('path')

module.exports = function (ctx) {
  return {
    build: {
      extendWebpack (cfg, { isServer, isClient }) {
        cfg.resolve.alias = {
          ...cfg.resolve.alias, // This adds the existing alias

          // Add your own alias like this
          myalias: path.resolve(__dirname, './src/somefolder'),
        }
      }
    }
  }
}
```

与chainWebpack()等效：

```js

// quasar.conf.js
const path = require('path')

module.exports = function (ctx) {
  return {
    build: {
      chainWebpack (chain, { isServer, isClient }) {
        chain.resolve.alias
          .set('myalias', path.resolve(__dirname, './src/somefolder'))
      }
    }
  }
}
```

### Webpack装载器

构建系统使用Webpack，所以它依靠使用webpack加载器来处理不同类型的文件（js，css，styl，scss，json等）。 默认情况下，最常用的加载程序是默认提供的。

### 安装装载器

我们举个例子吧。 你想能够导入`.json`文件。 **Quasar提供开箱即用的json支持，所以您实际上不需要执行这些步骤，但为了演示如何添加加载程序，我们将假装Quasar不提供它。**

所以，你需要一个装载机。 你搜索谷歌，看看你需要什么样的webpack loader。 在这种情况下，它是“json-loader”。 我们先安装它：

```bash
$ yarn add --dev json-loader
```

在安装新的加载器之后，我们想告诉Webpack使用它。 因此，我们编辑`/quasar.conf.js`并更改`build.extendWebpack()`为这个新的加载器添加条目到`module/rules`：

```js
// quasar.conf
build: {
  extendWebpack (cfg) {
    cfg.module.rules.push({
      test: /\.json$/,
      loader: 'json-loader'
    })
  }
}
```

与chainWebpack()等效:

```js
// quasar.conf
build: {
  chainWebpack (chain) {
    chain.module.rule('json')
      .test(/\.json$/)
      .use('json-loader')
        .loader('json-loader')
  }
}
```

你完成了。

### SASS/SCSS

所以你希望能够编写SASS/SCSS形式的CSS代码。 你需要一个装载机。 我们首先安装它。 请注意，对于这种特殊情况，您还需要安装node-sass，因为sass-loader依赖于它（作为对等依赖）。

```bash
$ yarn add --dev sass-loader node-sass
```

你完成了。 对于SCSS/SASS来说，这一切都是需要的。 你不需要进一步配置`/quasar.conf.js`。

安装完成后，您可以在`*.vue`组件中使用此预处理器(通过在`<style>`标签中使用lang属性)：

```html
<style lang="scss">
/* We can write SASS now! */
</style>
```

关于SASS语法的说明：

- lang="scss"对应于CSS超集语法（带花括号和分号）。
- lang="sass"对应于基于缩进的语法。

### PostCSS

`*.vue`文件（以及所有其他样式文件）中的样式默认通过PostCSS传送，因此您不需要使用特定的装载器。

默认情况下，PostCSS配置为使用Autoprefixer。看看`/.postcssrc.js’，你可以在那里调整它，如果你需要的话

### Pug

首先，您需要安装一些依赖项：

```bash
$ yarn add --dev pug pug-plain-loader
```

然后，您需要通过quasar.conf.js扩展webpack配置：

```js
// quasar.conf.js
build: {
  extendWebpack (cfg) {
    cfg.module.rules.push({
      test: /\.pug$/,
      loader: 'pug-plain-loader'
    })
  }
}
```

与chainWebpack()等效:

```js
// quasar.conf.js
build: {
  chainWebpack (chain) {
    chain.module.rule('pug')
      .test(/\.pug$/)
      .use('pug-plain-loader')
        .loader('pug-plain-loader')
  }
}
```

### Coffeescript

如果您使用Coffeescript，则需要禁用ESLint或告诉ESLint哪些Vue组件正在使用Coffeescript。

请注意`vue-loader`使用`lang='coffee'`来标识使用Coffeescript的组件，但是`lang='coffee'`不能识别ESLint。幸运的是，ESLint（遵循传统的HTML）使用`type=“xxx”`来标识脚本的类型。只要`<script>`标签有`javascript`之外的`type`，ESLint就会将该脚本标记为非javascript，并跳过它。 Coffeescript的约定是使用`type=“text/coffeescript”`来标识自己。因此，在使用Coffeescript的Vue组件中，同时使用`lang`和`type`来避免ESLint警告：

```html
<template>
  ...
</template>
<script lang="coffee" type="text/coffeescript">
  ...
</script>
```





## process.env处理

访问`process.env`可以在很多方面为您提供帮助：

- 根据Quasar模式 (SPA/PWA/Cordova/Electron)区分运行时程序
- 根据运行开发或生产构建，区分运行时程序
- 在构建时根据终端环境变量向其添加标志

### Quasar CLI提供的值

| 名称                   | 输入   | 意义                                        |
| :--------------------- | :----- | :------------------------------------------ |
| `process.env.DEV`      | 布尔   | 在开发模式下运行的代码                      |
| `process.env.PROD`     | 布尔   | 在生产模式下运行的代码                      |
| `process.env.CLIENT`   | 布尔   | 在客户端上（不在服务器上）运行的代码        |
| `process.env.SERVER`   | 布尔   | 在服务器上（不在客户端上）运行的代码        |
| `process.env.MODE`     | 字符串 | Quasar CLI 模式 (`spa`、 `pwa`、 …)         |
| `process.env.NODE_ENV` | 字符串 | 有两个可能的值：`production`或`development` |



### 例子

```js
if (process.env.DEV) {
  console.log(`I'm on a development build`)
}

// process.env.MODE is the <mode> in
// "quasar dev/build -m <mode>"
// (defaults to 'spa' if -m parameter is not specified)
if (process.env.MODE === 'electron') {
  const { remote } = require('electron')
  const win = remote.BrowserWindow.getFocusedWindow()

  if (win.isMaximized()) {
    win.unmaximize()
  }
  else {
    win.maximize()
  }
}
```

### 剥离代码

在编译您的网站/应用程序时，会根据process.env评估`if()`分支，如果表达式为“false”，则会将其从文件中删除。 例：

```js
if (process.env.DEV) {
  console.log('dev')
}
else {
  console.log('build')
}

// running with "quasar dev" will result in:
console.log('dev')
// while running with "quasar build" will result in:
console.log('build')
```

请注意上面的`if`在编译时被评估并完全剥离，导致更小的包。

### 基于process.env导入

您可以将上面学到的内容与动态导入结合起来：

```js
if (process.env.MODE === 'electron') {
  import('my-fancy-npm-package').then(package => {
    // notice "default" below, which is the prop with which
    // you can access what your npm imported package exports
    package.default.doSomething()
  })
}
```

### 添加到process.env

您可以通过`/ quasar.conf.js`文件将自己的定义添加到`process.env`：

```js
// quasar.conf.js

build: {
  env: ctx.dev
    ? { // so on dev we'll have
      API: JSON.stringify('https://dev.api.com')
    }
    : { // and on build (production):
      API: JSON.stringify('https://prod.api.com')
    }
}
```

然后在您的网站/应用程序中，您可以访问`process.env.API`，它将指向上面的两个链接之一，基于开发或生产构建类型。

你甚至可以更进一步。 提供来自`quasar dev/build`环境变量的值：

```js
# we set an env variable in terminal
$ MY_API=api.com quasar build

# then we pick it up in /quasar.conf.js
build: {
  env: ctx.dev
    ? { // so on dev we'll have
      API: JSON.stringify('https://dev.'+ process.env.MY_API)
    }
    : { // and on build (production):
      API: JSON.stringify('https://prod.'+ process.env.MY_API)
    }
}
```

或者你可以使用我们的[@quasar/dotenv](https://github.com/quasarframework/app-extension-dotenv) 或 [@quasar/qenv](https://github.com/quasarframework/app-extension-qenv)应用扩展。





## 应用Vuex存储(Vuex Store)

我们不会详细介绍如何配置或使用Vuex，因为它有很棒的文档。 相反，我们只是告诉你在Quasar项目中使用它时文件夹结构的样子。

```bash
.
└── src/
    └── store/               # Vuex Store
        ├── index.js         # Vuex Store 定义
        ├── <folder>         # Vuex Store 模块...
        └── <folder>         # Vuex Store 模块...
```

默认情况下，如果您在使用Quasar CLI创建项目文件夹时选择使用Vuex，它将设置使用Vuex模块。 `/src/store`的每个子文件夹代表一个Vuex模块。

TIP

如果在您的网站应用程序中Vuex模块太多，您可以更改`/src/store/index.js`并避免导入任何模块。

### 添加一个Vuex模块

Quasar CLI通过`$ quasar new`命令轻松添加Vuex模块。

```bash
$ quasar new store <store_name>
```

它会在上面的命令中创建一个名为“store_name”的`/src/store`文件夹。 它将包含您需要的所有样板。

假设您要创建一个“showcase”Vuex模块。 你运行`$ quasar new store showcase`。 然后您会注意到新创建的`/src/store/showcase`文件夹，其中包含以下文件：

```bash
.
└── src/
    └── store/
        ├── index.js         # Vuex Store定义
        └── showcase         # "showcase"模块
            ├── index.js     # 将模块粘合在一起
            ├── actions.js   # actions模块
            ├── getters.js   # getters模块
            ├── mutations.js # mutations模块
            └── state.js     # state模块
```

我们已经创建了新的Vuex模块，但我们还没有通知Vuex使用它。 所以我们编辑`/src/store/index.js`并添加一个引用：

```js
import Vue from 'vue'
import Vuex from 'vuex'

// 首先导入模块
import showcase from './showcase'

Vue.use(Vuex)

export default function (/* { ssrContext } */) {
  const Store = new Vuex.Store({
    modules: {
      // 然后我们引用它
      showcase
    },

    // 启用严格模式（增加开销！）
    // 仅适用于开发模式
    strict: process.env.DEV
  })

  /*
    如果我们需要一些HMR魔术，我们会处理
    下面的热点更新。 注意我们实现这个
    用“process.env.DEV”代码 - 所以这不会
    进入我们的生产版本（也不应该）。
  */

  if (process.env.DEV && module.hot) {
    module.hot.accept(['./showcase'], () => {
      const newShowcase = require('./showcase').default
      Store.hotUpdate({ modules: { showcase: newShowcase } })
    })
  }

  return Store
}
```

现在我们可以在我们的Vue文件中使用这个Vuex模块。 这是一个简单的例子。假设我们配置了state的 `drawerState`属性并增加了 `updateDrawerState`变动(mutation)。

```js
// src/store/showcase/mutations.js
export const updateDrawerState = (state, opened) => {
  state.drawerState = opened
}

// src/store/showcase/state.js
// 如果使用SSR，请务必使用函数返回状态
export default function () {
  return {
    drawerState: true
  }
}
```

在Vue文件中

```html
<template>
  <div>
    <q-toggle v-model="drawerState" />
  </div>
</template>

<script>
export default {
  computed: {
    drawerState: {
      get () {
        return this.$store.state.showcase.drawerState
      },
      set (val) {
        this.$store.commit('showcase/updateDrawerState', val)
      }
    }
  }
}
</script>
```

### 存储代码拆分

您可以利用[预取功能](http://www.quasarchs.com/quasar-cli/cli-documentation/prefetch-feature#Store-Code-Splitting)来对Vuex模块进行代码拆分。



