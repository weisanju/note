# 简介

read 是 [Shell 内置命令](http://c.biancheng.net/view/1136.html)，用来从标准输入中读取数据并赋值给变量。如果没有进行重定向，默认就是从键盘读取用户输入的数据；如果进行了重定向，那么可以从文件中读取数据

**语法**

```sh
read [-options] [variables]
```

`options`表示选项，如下表所示；`variables`表示用来存储数据的变量，可以有一个，也可以有多个。

`options`和`variables`都是可选的，如果没有提供变量名，那么读取的数据将存放到环境变量 REPLY 中。



# 选项

| 选项         | 说明                                                         |
| ------------ | ------------------------------------------------------------ |
| -a array     | 把读取的数据赋值给数组 array，从下标 0 开始。                |
| -d delimiter | 用字符串 delimiter 指定读取结束的位置，而不是一个换行符（读取到的数据不包括 delimiter）。 |
| -e           | 在获取用户输入的时候，对功能键进行编码转换，不会直接显式功能键对应的字符。 |
| -n num       | 读取 num 个字符，而不是整行字符。                            |
| -p prompt    | 显示提示信息，提示内容为 prompt。                            |
| -r           | 原样读取（Raw mode），不把反斜杠字符解释为转义字符。         |
| -s           | 静默模式（Silent mode），不会在屏幕上显示输入的字符。当输入密码和其它确认信息的时候，这是很有必要的。 |
| -t seconds   | 设置超时时间，单位为秒。如果用户没有在指定时间内输入完成，那么 read 将会返回一个非 0 的退出状态，表示读取失败。 |
| -u fd        | 使用文件描述符 fd 作为输入源，而不是标准输入，类似于重定向。 |



# 示例

## 多个变量

```sh
#!/bin/bash
read -p "Enter some information > " name url age
echo "网站名字：$name"
echo "网址：$url"
echo "年龄：$age"
```

## 只读取一个字符

```sh
#!/bin/bash
read -n 1 -p "Enter a char > " char
printf "\n"  #换行
echo $char
```

## 在指定时间内输入密码

```sh
#!/bin/bash
if
    read -t 20 -sp "Enter password in 20 seconds(once) > " pass1 && printf "\n" &&  #第一次输入密码
    read -t 20 -sp "Enter password in 20 seconds(again)> " pass2 && printf "\n" &&  #第二次输入密码
    [ $pass1 == $pass2 ]  #判断两次输入的密码是否相等
then
    echo "Valid password"
else
    echo "Invalid password"
fi
```

