# 概述

字符串（String）就是一系列字符的组合。字符串是 Shell 编程中最常用的数据类型之一（除了数字和字符串，也没有其他类型了）。

## 三种形式的区别：

1) 由单引号`' '`包围的字符串：

- 任何字符都会原样输出，在其中使用变量是无效的。
- 字符串中不能出现单引号，即使对单引号进行转义也不行。


2) 由双引号`" "`包围的字符串：

- 如果其中包含了某个变量，那么该变量会被解析（得到该变量的值），而不是原样输出。
- 字符串中可以出现双引号，只要它被转义了就行。


3) 不被引号包围的字符串

- 不被引号包围的字符串中出现变量时也会被解析，这一点和双引号`" "`包围的字符串一样。
- **字符串中不能出现空格，否则空格后边的字符串会作为其他变量或者命令解析。**



## 获取字符串长度

```sh
${#string_name}
```

# Shell字符串拼接（连接、合并）

在 Shell 中你不需要使用任何运算符，将两个字符串并排放在一起就能实现拼接，非常简单粗暴

```sh
#!/bin/bash
name="Shell"
url="http://c.biancheng.net/shell/"
str1=$name$url  #中间不能有空格
str2="$name $url"  #如果被双引号包围，那么中间可以有空格
str3=$name": "$url  #中间可以出现别的字符串
str4="$name: $url"  #这样写也可以
str5="${name}Script: ${url}index.html"  #这个时候需要给变量名加上大括号
echo $str1
echo $str2
echo $str3
echo $str4
echo $str5
```



# Shell字符串截取

## 从指定位置开始截取

* 这种方式需要两个参数：除了指定起始位置，还需要截取长度，才能最终确定要截取的字符串。

* 既然需要指定起始位置，那么就涉及到计数方向的问题，到底是从字符串左边开始计数，还是从字符串右边开始计数。答案是 Shell 同时支持两种计数方式。

### 从字符串左边开始计数

```sh
# 如果想从字符串的左边开始计数，那么截取字符串的具体格式如下：
${string: start :length}

url="c.biancheng.net"
echo ${url: 2: 9}

url="c.biancheng.net"
echo ${url: 2}  #省略 length，截取到字符串末尾
```

## 从右边开始计数

```sh
${string: 0-start :length}
# 同第 1) 种格式相比，第 2) 种格式仅仅多了0-，这是固定的写法，专门用来表示从字符串右边开始计数。
```

这里需要强调两点：

- 从左边开始计数时，起始数字是 0（这符合程序员思维）；从右边开始计数时，起始数字是 1（这符合常人思维）。计数方向不同，起始数字也不同。
- **不管从哪边开始计数，截取方向都是从左到右。**

```sh
url="c.biancheng.net"
echo ${url: 0-13: 9}
#结果为biancheng。从右边数，b是第 13 个字符。
```

## 从指定字符（子字符串）开始截取

这种截取方式无法指定字符串长度，只能从指定字符（子字符串）截取到字符串末尾。Shell 可以截取指定字符（子字符串）右边的所有字符，也可以截取左边的所有字符。

### **使用`#`号可以截取指定字符**

（或者子字符串）右边的所有字符，具体格式如下：

```sh
${string#*chars}
```

其中，string 表示要截取的字符，chars 是指定的字符（或者子字符串），`*`是通配符的一种，表示任意长度的字符串。`*chars`连起来使用的意思是：忽略左边的所有字符，直到遇见 chars（chars 不会被截取）。

```sh
url="http://c.biancheng.net/index.html"
echo ${url#*:}
```

如果希望直到最后一个指定字符（子字符串）再匹配结束，那么可以使用`##`，具体格式为：

```sh
${string##*chars}
```

```sh
#!/bin/bash
url="http://c.biancheng.net/index.html"
echo ${url#*/}    #结果为 /c.biancheng.net/index.html
echo ${url##*/}   #结果为 index.html
str="---aa+++aa@@@"
echo ${str#*aa}   #结果为 +++aa@@@
echo ${str##*aa}  #结果为 @@@
```

### 使用 % 截取左边字符

```sh
使用%号可以截取指定字符（或者子字符串）左边的所有字符，具体格式如下：
${string%chars*}
```

请注意`*`的位置，因为要截取 chars 左边的字符，而忽略 chars 右边的字符，所以`*`应该位于 chars 的右侧。其他方面`%`和`#`的用法相同，这里不再赘述，仅举例说明：

```sh
#!/bin/bash
url="http://c.biancheng.net/index.html"
echo ${url%/*}  #结果为 http://c.biancheng.net
echo ${url%%/*}  #结果为 http:
str="---aa+++aa@@@"
echo ${str%aa*}  #结果为 ---aa+++
echo ${str%%aa*}  #结果为 ---
```

## 汇总

最后，我们对以上 8 种格式做一个汇总，请看下表：

| 格式                       | 说明                                                         |
| -------------------------- | ------------------------------------------------------------ |
| ${string: start :length}   | 从 string 字符串的左边第 start 个字符开始，向右截取 length 个字符。 |
| ${string: start}           | 从 string 字符串的左边第 start 个字符开始截取，直到最后。    |
| ${string: 0-start :length} | 从 string 字符串的右边第 start 个字符开始，向右截取 length 个字符。 |
| ${string: 0-start}         | 从 string 字符串的右边第 start 个字符开始截取，直到最后。    |
| ${string#*chars}           | 从 string 字符串第一次出现 *chars 的位置开始，截取 *chars 右边的所有字符。 |
| ${string##*chars}          | 从 string 字符串最后一次出现 *chars 的位置开始，截取 *chars 右边的所有字符。 |
| ${string%*chars}           | 从 string 字符串第一次出现 *chars 的位置开始，截取 *chars 左边的所有字符。 |
| ${string%%*chars}          | 从 string 字符串最后一次出现 *chars 的位置开始，截取 *chars 左边的所有字符。 |



