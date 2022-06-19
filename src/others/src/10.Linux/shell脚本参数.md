# Positional parameter（bash中的位置参数）

A **positional parameter** is an argument specified on the command line, used to launch the current process in a shell.

Positional parameter values are stored in a special set of variables maintained by the shell



![](/images/shell_positional_parameters.png)





| 变量 | 值            |      |
| ---- | ------------- | ---- |
| $0   | 文件名        |      |
| $1   | one           |      |
| $2   | two           |      |
| $#   | 变量个数      |      |
| $@   | one two three |      |
| $*   | one tow three |      |

在不加双引号的时候$*和$@是一样，

如果加了双引号了，$*会被翻译成 $1c$2c$3c

where *c* is the first character of **$IFS**, bash's internal field separator variable. The IFS is used for word splitting, and its default value is "space, tab, or newline" — this is where bash sees the beginning and end of one word.  c是$IFS的第一个字母，bash的internal field separator变量（默认为<space><tab><newline>），内置字段分割变量。（所以c是空格）



