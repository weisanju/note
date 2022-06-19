## Tutorial

### 获取五条数据

```
curl 'https://api.github.com/repos/stedolan/jq/commits?per_page=5'
```

### 格式化

```
curl 'https://api.github.com/repos/stedolan/jq/commits?per_page=5' | jq '.'
```

### 取数据组中的第一个数据

```
curl 'https://api.github.com/repos/stedolan/jq/commits?per_page=5' | jq '.[0]'
```

### 取第一条数据的message、name

```java
curl 'https://api.github.com/repos/stedolan/jq/commits?per_page=5' | jq '.[0] | {message: .commit.message, name: .commit.committer.name}'
```

**取所有的**

```
jq '.[] | {message: .commit.message, name: .commit.committer.name}'
```

## 嵌套数组访问

```
jq '[.[] | {message: .commit.message, name: .commit.committer.name, parents: [.parents[].html_url]}]'
```







## Basic filters

### Identity: `.`

绝对最简单的过滤器是。。这是一个过滤器，它接受它的输入，并以不变的方式产生它作为输出。也就是说，这是身份运算符。



### Object Identifier-Index: `.foo`, `.foo.bar`

https://stedolan.github.io/jq/manual/#Builtinoperatorsandfunctions







