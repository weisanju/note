# 概念

scp是secure copy的简写， 是 linux 系统下基于 ssh 登陆进行安全的远程文件拷贝命令。scp 是加密的，rcp 是不加密的，scp 是 rcp 的加强版。

因为scp传输是加密的,可能会稍微影响一下速度。另外，scp还非常不占资源，不会提高多少系统负荷，在这一点上，rsync就远远不及它了。虽然 rsync比scp会快一点，但当小文件众多的情况下，rsync会导致硬盘I/O非常高，而scp基本不影响系统正常使用。



# **语法**

```
scp [-1246BCpqrv] [-c cipher] [-F ssh_config] [-i identity_file]
[-l limit] [-o ssh_option] [-P port] [-S program]
[[user@]host1:]file1 [...] [[user@]host2:]file2
```

**简易写法**

```
scp [可选参数] file_source... file_target 
```

## **参数说明：**

```text
-1： 强制scp命令使用协议ssh1
-2： 强制scp命令使用协议ssh2
-4： 强制scp命令只使用IPv4寻址
-6： 强制scp命令只使用IPv6寻址
-B： 使用批处理模式（传输过程中不询问传输口令或短语）
-C： 允许压缩。（将-C标志传递给ssh，从而打开压缩功能）
-p： 保留原文件的修改时间，访问时间和访问权限。
-q： 不显示传输进度条。
-r： 递归复制整个目录。
-v： 详细方式显示输出。scp和ssh(1)会显示出整个过程的调试信息。这些信息用于调试连接，验证和配置问题。
-c cipher：        以cipher将数据传输进行加密，这个选项将直接传递给ssh。
-F ssh_config：    指定一个替代的ssh配置文件，此参数直接传递给ssh。
-i identity_file： 从指定文件中读取传输时使用的密钥文件，此参数直接传递给ssh。
-l limit：         限定用户所能使用的带宽，以Kbit/s为单位。
-o ssh_option：    如果习惯于使用ssh_config(5)中的参数传递方式，
-P port：          注意是大写的P, port是指定数据传输用到的端口号
-S program：       指定加密传输时所使用的程序。此程序必须能够理解ssh(1)的选项。
```



# 注意事项

**复制目录时 最低级目录可以存在和不存在**

```sh
#如果 存在c目录，则 拷贝的 目标路径为   /d/c/b
scp /a/b  /d/c/
#如果 不存在目录c 则拷贝的 目录路径为 /d/c
```

**只复制文件时目录必须先存在**

```
#复制文件但不复制目录
scp /a/b* /d/c/d
#复制目录
scp -r /a/b* /d/c/d
```



