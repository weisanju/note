# 前言

这篇文章将研究如何处理和调试那些只发生在生产环境（或其他远程环境）而本地开发环境可能没办法重现的“问题”。



# Tomcat启用远程调试

> 传递特定的启动参数给 JVM，让它启用远程调试

JVM 激活远程调试的启动参数有 *JPDA_OPTS*, *CATALINA_OPTS* 和 *JAVA_OPTS*

JAVA_OPTS 是通常不建议使用的， 因为基于 JAVA_OPTS 的参数设定会暴露给所有的 JVM 应用， 而 CATALINA_OPTS 定义的设定值限制在Tomcat 内

## 使用JPDA_OPTS

### 设置Tomcat

在 *CATALINA_HOME/bin* 目录下创建可执行脚本文件 *setenv.sh* ( Windows 创建 setenv.bat ），加入内容：

```sh
export JPDA_OPTS="-agentlib:jdwp=transport=dt_socket,address=1043,server=y,suspend=n"
set JPDA_OPTS="-agentlib:jdwp=transport=dt_socket,address=1043,server=y,suspend=n"
```

这些参数要做的事情就是启用远程调试和配置有效的选项：

- 指定运行的被调试应用和调试者之间的通信协议，(ie: *transport=dt_socket*)
- 远程被调试应用开通的端口，(ie: address=1043)， 可定义其他端口，比如9999
- server=y 表示这个 JVM 即将被调试
- suspend=n 用来告知 JVM 立即执行，不要等待未来将要附着上/连上（attached）的调试者。如果设成 y, 则应用将暂停不运行，直到有调试者连接上

suspend=y的一个比较适用的场景是，当debug一个会阻止应用成功启动的问题时， 通过suspend=y可以确保调试者连上来之后再启动应用，否则应用已经启动报错了再调试也没意义了。

### 启动Tomcat

```sh
$CATALINA_HOME/bin/catalina.sh jpda start
```

## 使用 JAVA_OPTS / CATALINA_OPTS

 setenv.sh 中写入

```
set CATALINA_OPTS="-agentlib:jdwp=transport=dt_socket,address=1043,server=y,suspend=n"
export CATALINA_OPTS="-agentlib:jdwp=transport=dt_socket,address=1043,server=y,suspend=n"
```

### 启动方式

```sh
./startup.sh
或者
./catalina.sh start
```

## 使用JPDA启动

是用 JPDA 切换， 用如下的启动命令将使用默认值自动启用远程调试，

```sh
catalina jpda start
```

该命令默认使用的设置是

```bash
-agentlib:jdwp=transport=dt_socket,address=8000,server=y,suspend=n
```

修改 Tomcat 需要的这些环境变量

```bash
//JPDA_TRANSPORT: 指定 jpda 传输协议
//JPDA_ADDRESS: 指定远程调试端口
//JPDA_SUSPEND: 指定 jvm 启动暂缓

export JPDA_ADDRESS=0.0.0.0:8080
```

# 配置Intellj Idea

## Remote Tomcat 配置

```
//步骤1
Run ➝ Edit Configurations ➝ **+ **按钮 ➝ Tomcat Server ➝ Remote
//步骤2 填写 主机与IP
remote connection settings
//步骤3 Startup/Connection-> debug
Transport Socket 8000
```

## 使用 Remote 配置

第一个方法有个缺陷，你打开的工程源码必须是编译通过的工程，否则会启动会报错；
而介绍的这第二种方法可以在你的工程目录乱七八糟，不是一个完整的可以部署的工程，甚至是一个解压缩的 war/ jar 的情况下都可以调试。

**案例1**

我手里有一个可部署的war包，没有源码，在远程已经部署完毕。这时我想调试那个远程应用，怎么做呢？

解压缩war包到一个文件夹，然后用Intellij Idea打开这个文件夹，如图的结构，编译的Class都在 WEB-INF/classes 目录下

找到我要debug的那个class, 这里示例Handler.class, 通过Idea反编译出来的类代码，拷贝到一个新的文件Handler.java

虽然如图可以看到各种的编译错误，但是完全不影响你启动，代码中加断点和调试哦。



```
remote JVM Debug -> attach to remote JVM ->  socket -> host -> port -> moduleClassPath
```

# 远程JVM调试怎么工作的

一切源于被称作 Agents 的东西。

运行着各种编译过的 .class 文件的JVM， 有一种特性，可以允许外部的库（Java或C++写的libraries）在运行时注入到 JVM 中。这些外部的库就称作 Agents, 他们有能力修改运行中 .class 文件的内容。

这些 Agents 拥有的这些 JVM 的功能权限， 是在 JVM 内运行的 Java Code 所无法获取的， 他们能用来做一些有趣的事情，比如修改运行中的源码， 性能分析等。 像 JRebel 工具就是用了这些功能达到魔术般的效果。

传递一个 Agent Lib 给 JVM, 通过添加 agentlib:libname[=options] 格式的启动参数即可办到。像上面的远程调试我们用的就是 **-agentlib:jdwp=... **来引入 jdwp 这个 Agent 的。

jdwp 是一个 JVM 特定的 JDWP（Java Debug Wire Protocol） 可选实现，用来定义调试者与运行JVM之间的通讯，它的是通过 JVM 本地库的 jdwp.so 或者 jdwp.dll 支持实现的。

