# 步骤

## 给项目安装ts语言

```shell
# 先删除package.json 中的 dependencies中的 ts依赖
yarn add --dev typescript
```



## 使用tsc编译

```json
{
  // ...
  "scripts": {
    "build": "tsc",
    // ...
  },
  // ...
}
```

**配置 TypeScript 编译器**

```shell
yarn run tsc --init
```



**指定编译源代码路径以及 编译输出路径**

```json
// tsconfig.json

{
  "compilerOptions": {
    // ...
    "rootDir": "src",
    "outDir": "build"
    // ...
  },
}
```

**构建HTML**

```
yarn build
```



## 类型定义

为了能够显示来自其他包的错误和提示，编译器依赖于声明文件

声明文件提供有关库的所有类型信息

这样，我们的项目就可以用上像 npm 这样的平台提供的三方 JavaScript 库。



获取一个库的声明文件有两种方式：

**Bundled** - 该库包含了自己的声明文件。

这样很好，因为我们只需要安装这个库，就可以立即使用它了

要知道一个库是否包含类型，看库中是否有 `index.d.ts` 文件

有些库会在 `package.json` 文件的 `typings` 或 `types` 属性中指定类型文件。



**[DefinitelyTyped](https://github.com/DefinitelyTyped/DefinitelyTyped)** - DefinitelyTyped 是一个庞大的声明仓库，为没有声明文件的 JavaScript 库提供类型定义。这些类型定义通过众包的方式完成，并由微软和开源贡献者一起管理。例如，React 库并没有自己的声明文件。但我们可以从 DefinitelyTyped 获取它的声明文件。只要执行以下命令。



```sh
# yarn
yarn add --dev @types/react

# npm
npm i --save-dev @types/react
```



**局部声明** 有时，你要使用的包里没有声明文件，在 DefinitelyTyped 上也没有。在这种情况下，我们可以创建一个本地的定义文件。因此，在项目的根目录中创建一个 `declarations.d.ts` 文件。一个简单的声明可能是这样的：

```typescript
declare module 'querystring' {
  export function stringify(val: object): string
  export function parse(val: string): object
}
```

