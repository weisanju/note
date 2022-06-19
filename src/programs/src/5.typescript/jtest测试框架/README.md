# Installation

```shell
yarn add --dev jest typescript ts-jest @types/jest
```



### [Jest config file](https://kulshekhar.github.io/ts-jest/docs/getting-started/installation#jest-config-file)

#### Creating

默认情况下，Jest可以在没有任何配置文件的情况下运行，但不会编译.ts文件。为了使它与ts-jest转换TypeScript，我们将需要创建一个配置文件，该文件将告诉Jest使用ts-jest预设。

`ts-jest` can create the configuration file for you automatically:

```sh
yarn ts-jest config:init
```

这将创建一个基本的Jest配置文件，该文件将通知Jest如何处理.ts文件正确。

您还可以使用`jest -- init`命令 (根据您使用的内容，以npx或yarn为前缀) 来提供更多与Jest相关的选项。

但是，对于有关是否启用TypeScript的Jtest问题，请回答 “否”。

相反，在之后的jest.config.js文件中添加一行，预设: `ts-jest`

