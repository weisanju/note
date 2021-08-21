# Shell test 命令

> Shell中的 test 命令用于检查某个条件是否成立，它可以进行数值、字符和文件三个方面的测试。

## 数值测试

| 参数 | 说明           |
| :--- | :------------- |
| -eq  | 等于则为真     |
| -ne  | 不等于则为真   |
| -gt  | 大于则为真     |
| -ge  | 大于等于则为真 |
| -lt  | 小于则为真     |
| -le  | 小于等于则为真 |

## 实例

### 实例1

```
num1=100
num2=100
if test $[num1] -eq $[num2]
then
    echo '两个数相等！'
else
    echo '两个数不相等！'
fi
```

代码中的 **[]** 执行基本的算数运算，如：

### 实例2

```
#!/bin/bash
a=5
b=6
result=$[a+b]# 注意等号两边不能有空格
echo "result 为： $result"
```

## 字符串测试

| 参数      | 说明                     |
| :-------- | :----------------------- |
| =         | 等于则为真               |
| !=        | 不相等则为真             |
| -z 字符串 | 字符串的长度为零则为真   |
| -n 字符串 | 字符串的长度不为零则为真 |

### 实例

```sh
num1="ru1noob"
num2="runoob"
if test $num1 = $num2
then
    echo '两个字符串相等!'
else
    echo '两个字符串不相等!'
fi
```

## 文件测试

| -b filename     | 当filename 存在并且是块文件时返回真(返回0)                   |
| --------------- | ------------------------------------------------------------ |
| -c filename     | 当filename 存在并且是字符文件时返回真                        |
| -d pathname     | 当pathname 存在并且是一个目录时返回真                        |
| -e pathname     | 当由pathname 指定的文件或目录存在时返回真                    |
| -f filename     | 当filename 存在并且是正规文件时返回真                        |
| -g pathname     | 当由pathname 指定的文件或目录存在并且设置了SGID 位时返回真   |
| -h filename     | 当filename 存在并且是符号链接文件时返回真 (或 -L filename)   |
| -k pathname     | 当由pathname 指定的文件或目录存在并且设置了"粘滞"位时返回真  |
| -p filename     | 当filename 存在并且是命名管道时返回真                        |
| -r pathname     | 当由pathname 指定的文件或目录存在并且可读时返回真            |
| -s filename     | 当filename 存在并且文件大小大于0 时返回真                    |
| -S filename     | 当filename 存在并且是socket 时返回真                         |
| -t fd           | 当fd 是与终端设备相关联的文件描述符时返回真                  |
| -u pathname     | 当由pathname 指定的文件或目录存在并且设置了SUID 位时返回真   |
| -w pathname     | 当由pathname 指定的文件或目录存在并且可写时返回真            |
| -x pathname     | 当由pathname 指定的文件或目录存在并且可执行时返回真          |
| -O pathname     | 当由pathname 存在并且被当前进程的有效用户id 的用户拥有时返回真(字母O 大写) |
| -G pathname     | 当由pathname 存在并且属于当前进程的有效用户id 的用户的用户组时返回真 |
| file1 -nt file2 | file1 比file2 新时返回真                                     |
| file1 -ot file2 | file1 比file2 旧时返回真                                     |
| f1 -ef f2       | files f1 and f2 are hard links to the same file              |



```sh
cd /bin
if test -e ./bash
then
    echo '文件已存在!'
else
    echo '文件不存在!'
fi
```

## 条件组合

另外，Shell 还提供了与( -a )、或( -o )、非( ! )三个逻辑操作符用于将测试条件连接起来，其优先级为： **!** 最高， **-a** 次之， **-o** 最低

```sh
cd /bin
if test -e ./notFile -o -e ./bash
then
    echo '至少有一个文件存在!'
else
    echo '两个文件都不存在'
fi
```



## 简写形式

```sh
[ expression ]
```

注意`[]`和`expression`之间的空格，这两个空格是必须的，否则会导致语法错误。`[]`的写法更加简洁，比 test 使用频率高。

## 在 test 中使用变量建议用双引号包围起来

test 和 [] 都是命令，一个命令本质上对应一个程序或者一个函数。即使是一个程序，它也有入口函数，例如C语言程序的入口函数是 main()，运行C语言程序就从 main() 函数开始，所以也可以将一个程序等效为一个函数，这样我们就不用再区分函数和程序了，直接将一个命令和一个函数对应起来即可。

有了以上认知，就很容易看透命令的本质了：使用一个命令其实就是调用一个函数，命令后面附带的选项和参数最终都会作为实参传递给函数。

假设 test 命令对应的函数是 func()，使用`test -z $str1`命令时，会先将变量 $str1 替换成字符串：

- 如果 $str1 是一个正常的字符串，比如 abc123，那么替换后的效果就是`test -z abc123`，调用 func() 函数的形式就是`func("-z abc123")`。test 命令后面附带的所有选项和参数会被看成一个整体，并作为实参传递进函数。
- 如果 $str1 是一个空字符串，那么替换后的效果就是`test -z`，调用 func() 函数的形式就是`func("-z ")`，这就比较奇怪了，因为`-z`选项没有和参数成对出现，func() 在分析时就会出错。


如果我们给 $str1 变量加上双引号，当 $str1 是空字符串时，`test -z "$str1"`就会被替换为`test -z ""`，调用 func() 函数的形式就是`func("-z \"\"")`，很显然，`-z`选项后面跟的是一个空字符串（`\"`表示转义字符），这样 func() 在分析时就不会出错了。

所以，当你在 test 命令中使用变量时，我强烈建议将变量用双引号`""`包围起来，这样能避免变量为空值时导致的很多奇葩问题







# ifelse语句

## 语法格式

```sh
if  condition
then
    statement(s)
fi
```

`condition`是判断条件，如果 condition 成立（返回“真”），那么 then 后边的语句将会被执行；如果 condition 不成立（返回“假”），那么不会执行任何语句。

**从本质上讲，if 检测的是命令的退出状态**，我们将在下节《[Shell退出状态](http://c.biancheng.net/view/2735.html)》中深入讲解。

注意，最后必须以`fi`来闭合，fi 就是 if 倒过来拼写。也正是有了 fi 来结尾，所以即使有多条语句也不需要用`{ }`包围起来。

如果你喜欢，也可以将 then 和 if 写在一行：

```sh
if condition;  then
  statement(s)
fi


if  condition
then
   statement1
else
   statement2
fi


if  condition1
then
   statement1
elif condition2
then
    statement2
elif condition3
then
    statement3
……
else
   statementn
fi
```

请注意 condition 后边的分号`;`，当 if 和 then 位于同一行的时候，这个分号是必须的，否则会有语法错误。





# 双括号与条件测试

> **使用双括号进行四则运算、逻辑运算等**

## 比较两个数字的大小

```sh
#!/bin/bash
read a
read b
if (( $a == $b ))
then
    echo "a和b相等"
fi
```

在《[Shell (())](http://c.biancheng.net/view/2480.html)》一节中我们讲到，`(())`是一种数学计算命令，它除了可以进行最基本的加减乘除运算，**还可以进行大于、小于、等于等关系运算，以及与、或、非逻辑运算**。当 a 和 b 相等时，`(( $a == $b ))`判断条件成立，进入 if，执行 then 后边的 echo 语句。

## 逻辑运算

```sh
#!/bin/bash

read age
read iq

if (( $age > 18 && $iq < 60 ))
then
    echo "你都成年了，智商怎么还不及格！"
    echo "来C语言中文网（http://c.biancheng.net/）学习编程吧，能迅速提高你的智商。"
fi
```





# [[ ]]条件测试

> `[[ ]]`是 Shell 内置关键字，它和 [test 命令](http://c.biancheng.net/view/2742.html)类似，也用来检测某个条件是否成立。

test 能做到的，[[ ]] 也能做到，而且 [[ ]] 做的更好；test 做不到的，[[ ]] 还能做到。可以认为 [[ ]] 是 **test 的升级版**，对细节进行了优化，并且扩展了一些功能。

## 语法

```sh
[[ expression ]]
当 [[ ]] 判断 expression 成立时，退出状态为 0，否则为非 0 值。注意[[ ]]和expression之间的空格，这两个空格是必须的，否则会导致语法错误。
```

## 逻辑运算

对多个表达式进行逻辑运算时，可以使用逻辑运算符将多个 test 命令连接起来，例如：

```sh
[ -z "$str1" ] || [ -z "$str2" ]
```

你也可以借助选项把多个表达式写在一个 test 命令中，例如：

```sh
[ -z "$str1" -o -z "$str2" ]
```

但是，这两种写法都有点“别扭”，完美的写法是在一个命令中使用逻辑运算符将多个表达式连接起来。我们的这个愿望在 [[ ]] 中实现了，[[ ]] 支持 &&、|| 和 ! 三种逻辑运算符。

**使用 [[ ]] 对上面的语句进行改进：**

```sh
[[ -z $str1 || -z $str2 ]]
```

这种写法就比较简洁漂亮了。

**注意，[[ ]] 剔除了 test 命令的`-o`和`-a`选项，你只能使用 || 和 &&。这意味着，你不能写成下面的形式：**

```sh
[[ -z $str1 -o -z $str2 ]]
```

当然，使用逻辑运算符将多个 [[ ]] 连接起来依然是可以的，因为这是 Shell 本身提供的功能，跟 [[ ]] 或者 test 没有关系，如下所示：

```sh
[[ -z $str1 ]] || [[ -z $str2 ]]
```



## 实例

### 字符串比较

```sh
#!/bin/bash
read str1
read str2
if [[ -z $str1 ]] || [[ -z $str2 ]]  #不需要对变量名加双引号
then
    echo "字符串不能为空"
elif [[ $str1 < $str2 ]]  #不需要也不能对 < 进行转义
then
    echo "str1 < str2"
else
    echo "str1 >= str2"
fi
```



