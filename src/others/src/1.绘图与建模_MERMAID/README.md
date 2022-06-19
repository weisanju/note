# 简介

## 介绍

Mermaid 是一个用于画流程图、状态图、时序图、甘特图的库，使用 JS 进行本地渲染，广泛集成于许多 Markdown 编辑器中。

官网：https://mermaidjs.github.io/

Github 项目地址：https://github.com/knsv/mermaid





## Mermaid is a Diagramming tool for everyone.

* 非编程人员可以 使用在线编辑器， [Mermaid Live Editor](https://github.com/mermaid-js/mermaid-live-editor)，访问 教程页 [Tutorials Page](https://mermaid-js.github.io/mermaid/#/./Tutorials) 
* 很多 应用也 集成了 *mermaid*  ，看[Integrations and Usages for Mermaid](https://mermaid-js.github.io/mermaid/#/./integrations).

🌐 [CDN](https://unpkg.com/mermaid/) | 📖 [Documentation](https://mermaidjs.github.io/) | 🙌 [Contribution](https://github.com/mermaid-js/mermaid/blob/develop/docs/development.md) | 📜 [Version Log](https://mermaid-js.github.io/mermaid/#/./CHANGELOG)





## 可以渲染的图像

### 简单流程图

```
graph TD;
    A-->B;
    A-->C;
    B-->D;
    C-->D;
```

### 时序图

```
sequenceDiagram
    participant Alice
    participant Bob
    Alice->>John: Hello John, how are you?
    loop Healthcheck
        John->>John: Fight against hypochondria
    end
    Note right of John: Rational thoughts <br/>prevail!
    John-->>Alice: Great!
    John->>Bob: How about you?
    Bob-->>John: Jolly good!
```

### 甘特图

```
gantt
dateFormat  YYYY-MM-DD
title Adding GANTT diagram to mermaid
excludes weekdays 2014-01-10

section A section
Completed task            :done,    des1, 2014-01-06,2014-01-08
Active task               :active,  des2, 2014-01-09, 3d
Future task               :         des3, after des2, 5d
Future task2               :         des4, after des3, 5d
```

### 类图

```
classDiagram
Class01 <|-- AveryLongClass : Cool
Class03 *-- Class04
Class05 o-- Class06
Class07 .. Class08
Class09 --> C2 : Where am i?
Class09 --* C3
Class09 --|> Class07
Class07 : equals()
Class07 : Object[] elementData
Class01 : size()
Class01 : int chimp
Class01 : int gorilla
Class08 <--> C2: Cool label
```

### Git提交图

```
gitGraph:
options
{
    "nodeSpacing": 150,
    "nodeRadius": 10
}
end
commit
branch newbranch
checkout newbranch
commit
commit
checkout master
commit
commit
merge newbranch
```

### ER图

```
erDiagram
    CUSTOMER ||--o{ ORDER : places
    ORDER ||--|{ LINE-ITEM : contains
    CUSTOMER }|..|{ DELIVERY-ADDRESS : uses
```

### 用户日志图

```
journey
    title My working day
    section Go to work
      Make tea: 5: Me
      Go upstairs: 3: Me
      Do work: 1: Me, Cat
    section Go home
      Go downstairs: 5: Me
      Sit down: 5: Me
```

## 安装

`https://unpkg.com/mermaid@<version>/dist/`

Latest Version: https://unpkg.com/browse/mermaid@8.8.0/



## 部署Mermaid

```
<script src="https://cdn.jsdelivr.net/npm/mermaid/dist/mermaid.min.js"></script>
<script>mermaid.initialize({startOnLoad:true});</script>
```

## 兄弟项目

- [Mermaid Live Editor](https://github.com/mermaid-js/mermaid-live-editor)
- [Mermaid CLI](https://github.com/mermaid-js/mermaid-cli)
- [Mermaid Webpack Demo](https://github.com/mermaidjs/mermaid-webpack-demo)
- [Mermaid Parcel Demo](https://github.com/mermaidjs/mermaid-parcel-demo)

**请求协助**

欢迎来共享源代码，我们以后的方向是

- Adding more types of diagrams like mindmaps, ert diagrams, etc. 更多的类型的图，例如 mindmap
- Improving existing diagrams 改进现有的图

## **代码贡献步骤**

* yarn install
* yarn build:watch
* yarn lint
* yarn test
* npm publish







# 使用步骤

有四种方式 使用*mermaid*

1. Using the mermaid [live editor](https://mermaid-js.github.io/mermaid-live-editor/). For some popular video tutorials on the live editor go to [Overview](https://mermaid-js.github.io/mermaid/#/./n00b-overview).
2. Using one of the many [mermaid plugins](https://mermaid-js.github.io/mermaid/#/../overview/integrations). 
3. Hosting mermaid on a webpage, with an absolute link.
4. Downloading mermaid and hosting it on your Web Page.

## 在线编辑器

## mermaid插件

**This is covered in greater detail in the [Usage section](https://mermaid-js.github.io/mermaid/#/usage)**

## 浏览器使用

a. A reference for fetching the online mermaid renderer, which is written in Javascript.

b. The mermaid code for the diagram we want to create.

c. The `mermaid.initialize()` call to start the rendering process.

```html
<body>
  <script src="https://cdn.jsdelivr.net/npm/mermaid/dist/mermaid.min.js"></script>
</body>

<body>
  Here is a mermaid diagram:
  <div class="mermaid">
    graph TD
    A[Client] --> B[Load Balancer]
    B --> C[Server01]
    B --> D[Server02]
  </div>
</body>

<body>
  <script>mermaid.initialize({startOnLoad:true});</script>
</body>
```

**相对链接**

```html
<html lang="en">
<head>
  <meta charset="utf-8">
</head>
<body>
  <div class="mermaid">
  graph LR
      A --- B
      B-->C[fa:fa-ban forbidden]
      B-->D(fa:fa-spinner);
  </div>
  <div class="mermaid">
     graph TD
     A[Client] --> B[Load Balancer]
     B --> C[Server1]
     B --> D[Server2]
  </div>
  <script src="C:\Users\MyPC\mermaid\dist\mermaid.js"></script>
  <script>mermaid.initialize({startOnLoad:true});</script>
</body>
</html>
```

