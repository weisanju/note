# 源文件划分

cmake语言 将工程源文件划分为

- [Directories](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#directories) (`CMakeLists.txt`)
- [Scripts](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#scripts) (`<script>.cmake`)
- [Modules](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#modules) (`<module>.cmake`)



## Directories

* `CMakeLists.txt` 位于目录顶层, 是工程的入口, 最终会被解析成 构建树
* 这个文件中 可以 添加  `entire build specification`   也可以添加子目录 `add_subdirectory()`,.子目录也会产生构建树
* 每个被添加的子目录必须包括 一个  `CMakeLists.txt` 的入口点文件 





## Scripts

* 独立 .*cmake* 源文件 

* 按照*Cmake*语法执行脚本 通过 `-P选项`

## Modules

* 在 `Scripts`  `Directories` 中可以使用 `include()` 命令,  

* 导入`<module>`.cmake 脚本命令





# 语法

## SourceFile

```
file         ::=  file_element*
file_element ::=  command_invocation line_ending |
                  (bracket_comment|space)* line_ending
line_ending  ::=  line_comment? newline
space        ::=  <match '[ \t]+'>
newline      ::=  <match '\n'>
```



## 命令调用

**Command Invocations**

```
command_invocation  ::=  space* identifier space* '(' arguments ')'
identifier          ::=  <match '[A-Za-z_][A-Za-z0-9_]*'>
arguments           ::=  argument? separated_arguments*
separated_arguments ::=  separation+ argument? |
                         separation* '(' arguments ')'
separation          ::=  space | line_ending
```

**Command Arguments**

```
argument ::=  bracket_argument | quoted_argument | unquoted_argument
```



## 参数形式

**Bracket Argument**

```
bracket_argument ::=  bracket_open bracket_content bracket_close
bracket_open     ::=  '[' '='* '['
bracket_content  ::=  <any text not containing a bracket_close with
                       the same number of '=' as the bracket_open>
bracket_close    ::=  ']' '='* ']'
```

**Quoted Argument**

```
quoted_argument     ::=  '"' quoted_element* '"'
quoted_element      ::=  <any character except '\' or '"'> |
                         escape_sequence |
                         quoted_continuation
quoted_continuation ::=  '\' newline
```

**Unquoted Argument**

```
unquoted_argument ::=  unquoted_element+ | unquoted_legacy
unquoted_element  ::=  <any character except whitespace or one of '()#"\'> |
                       escape_sequence
unquoted_legacy   ::=  <see note in text>
```

例子

```
foreach(arg
    NoSpace
    Escaped\ Space
    This;Divides;Into;Five;Arguments
    Escaped\;Semicolon
    )
  message("${arg}")
endforeach()
```

**Escape Sequences**

```
escape_sequence  ::=  escape_identity | escape_encoded | escape_semicolon
escape_identity  ::=  '\' <match '[^A-Za-z0-9;]'>  //字符串本身
escape_encoded   ::=  '\t' | '\r' | '\n'  // tab,回车,换行
escape_semicolon ::=  '\;' //;
```

## 变量引用

**Variable References**

* 形如`${<variable>}`  是被解析为  `Quoted Argument`,`Unquoted Argument` 
* 变量可以嵌套 从内而外. `${outer_${inner_variable}_variable}`
* 详见 [Variables](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#variables) 变量作用域以及 如何设置值
* 环境变量 形如  `$ENV{<variable>} `  详见[Environment Variables](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#environment-variables)
* 缓存变量 形如 `$CACHE{<variable>}`,详见 [`CACHE`](https://cmake.org/cmake/help/latest/variable/CACHE.html#variable:CACHE)
* `if()` 命令 中的 变量 可以  `<variable>` instead of `${<variable>}`., environment and cache variables always need to be referenced as `$ENV{<variable>}` or `$CACHE{<variable>}`.

## 注释

**Bracket Comment**

```
#[[This is a bracket comment.
It runs until the close bracket.]]
message("First Argument\n" #[[Bracket Comment]] "Second Argument")
```



**Line Comment**

```
# This is a line comment.
message("First Argument\n" # This is a line comment :)
        "Second Argument") # This is a line comment.
```

## 控制流程

**条件块**

```
if()/elseif()/else()/endif()
```

**LOOP**

```
The foreach()/endforeach() and while()/endwhile() commands delimit code blocks to be executed in a loop. 
Inside such blocks the break() command may be used to terminate the loop early whereas the continue() command may be used to start with the next iteration immediately.
```

**命令定义**

The [`macro()`](https://cmake.org/cmake/help/latest/command/macro.html#command:macro)/[`endmacro()`](https://cmake.org/cmake/help/latest/command/endmacro.html#command:endmacro), and [`function()`](https://cmake.org/cmake/help/latest/command/function.html#command:function)/[`endfunction()`](https://cmake.org/cmake/help/latest/command/endfunction.html#command:endfunction) 

## 变量

* 变量是 Cmake的  基本存储单元
* 都是string类型
*  [`set()`](https://cmake.org/cmake/help/latest/command/set.html#command:set) and [`unset()`](https://cmake.org/cmake/help/latest/command/unset.html#command:unset)  设置或取消变量
* 变量名大小写敏感,可以包含任何文本
* 变量有作用域  ,set,unset只在当前作用域

**Function Scope**

*function* 内部

**Directory Scope**

* 整个  `CMakeLists.txt`文件中,也继承父级 作用域的 变量

**Persistent Cache**

CMake stores a separate set of “cache” variables, or “cache entries”, whose values persist across multiple runs within a project build tree. Cache entries have an isolated binding scope modified only by explicit request, such as by the `CACHE` option of the [`set()`](https://cmake.org/cmake/help/latest/command/set.html#command:set) and [`unset()`](https://cmake.org/cmake/help/latest/command/unset.html#command:unset) commands.

**Cmake保留命令**

```
CMake reserves identifiers that:
begin with CMAKE_ (upper-, lower-, or mixed-case), or
begin with _CMAKE_ (upper-, lower-, or mixed-case), or
begin with _ followed by the name of any
```

**Environment Variables**

与普通变量一样,除了以下几点

Scope

Environment variables have global scope in a CMake process. They are never cached.

References

[Variable References](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#variable-references) have the form `$ENV{<variable>}`.

## list

尽管所有值 存储为 string, 但在某些场合 也可以当作 *list*

例如在  `Unquoted Argument` 的解析时, 参数以 ';'分隔

数组元素 作为string展示时 以 ';' 连接

```
set(srcs a.c b.c c.c) # sets "srcs" to "a.c;b.c;c.c"
set(x a "b;c") 
```

