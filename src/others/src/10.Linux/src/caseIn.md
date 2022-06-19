# 概述

和其它编程语言类似，Shell 也支持两种分支结构（选择结构），分别是 if else 语句和 case in 语句

当分支较多，并且判断条件比较简单时，使用 case in 语句就比较方便了。

**语法**

```sh
case expression in
    pattern1)
        statement1
        ;;
    pattern2)
        statement2
        ;;
    pattern3)
        statement3
        ;;
    ……
    *)
        statementn
esac
```



```sh
#!/bin/bash
printf "Input integer number: "
read num
case $num in
    1)
        echo "Monday"
        ;;
    2)
        echo "Tuesday"
        ;;
    3)
        echo "Wednesday"
        ;;
    4)
        echo "Thursday"
        ;;
    5)
        echo "Friday"
        ;;
    6)
        echo "Saturday"
        ;;
    7)
        echo "Sunday"
        ;;
    *)
        echo "error"
esac
```



case、in 和 esac 都是 Shell 关键字，expression 表示表达式，pattern 表示匹配模式。

- expression 既可以是一个变量、一个数字、一个字符串，还可以是一个数学计算表达式，或者是命令的执行结果，只要能够得到 expression 的值就可以。
- pattern 可以是一个数字、一个字符串，甚至是一个简单的正则表达式。


case 会将 expression 的值与 pattern1、pattern2、pattern3 逐个进行匹配：

- 如果 expression 和某个模式（比如 pattern2）匹配成功，就会执行这模式（比如 pattern2）后面对应的所有语句（该语句可以有一条，也可以有多条），直到遇见双分号`;;`才停止；然后整个 case 语句就执行完了，程序会跳出整个 case 语句，执行 esac 后面的其它语句。
- 如果 expression 没有匹配到任何一个模式，那么就执行`*)`后面的语句（`*`表示其它所有值），直到遇见双分号`;;`或者`esac`才结束。`*)`相当于多个 if 分支语句中最后的 else 部分。



# case in 和正则表达式

case in 的 pattern 部分支持简单的正则表达式，具体来说，可以使用以下几种格式：

| 格式  | 说明                                                         |
| ----- | ------------------------------------------------------------ |
| *     | 表示任意字符串。                                             |
| [abc] | 表示 a、b、c 三个字符中的任意一个。比如，[15ZH] 表示 1、5、Z、H 四个字符中的任意一个。 |
| [m-n] | 表示从 m 到 n 的任意一个字符。比如，[0-9] 表示任意一个数字，[0-9a-zA-Z] 表示字母或数字。 |
| \|    | 表示多重选择，类似逻辑运算中的或运算。比如，abc \| xyz 表示匹配字符串 "abc" 或者 "xyz"。 |



如果不加以说明，Shell 的值都是字符串，expression 和 pattern 也是按照字符串的方式来匹配的；本节第一段代码看起来是判断数字是否相等，其实是判断字符串是否相等。



**正则使用**

```sh
#!/bin/bash
printf "Input a character: "
read -n 1 char
case $char in
    [a-zA-Z])
        printf "\nletter\n"
        ;;
    [0-9])
        printf "\nDigit\n"
        ;;
    [0-9])
        printf "\nDigit\n"
        ;;
    [,.?!])
        printf "\nPunctuation\n"
        ;;
    *)
        printf "\nerror\n"
esac
```





