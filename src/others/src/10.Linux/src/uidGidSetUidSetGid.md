## 程序和进程

一般而言，程序是一个可执行的文件，而进程是程序执行的实例，

一个程序可以产生任意多个进程，这些进程具有相同的代码。



## 用户名/组名和uid/gid

1. 每一个用户具有一个用户名和组名以及对应的uid/gid

2. 通常用户名和组名是人类可读的词，而uid/gid是一个整数，系统其实只认识uid/gid, 而不关心具体的username和groupname是什么

3. linux的权限校验机制也都是通过uid和gid来控制的。
4.  多个不同的username可以对应同一个uid, 同理多个不同的groupname可对应同一个gid, 反之则不可。



## 主要gid和次要gid

每个用户都有一个主要gid, 可以加入多个次要gid

1. 当通过当前用户创建文件时，文件的gid就是用户的主要gid
2. 次要gid主要用于访问权限验证，当用户属于某个次要gid，且要访问的文件的gid是该gid，且文件权限是group可访问的，则用户可访问该文件。

## 程序uid, gid

1. 前面提到，程序就是一个可执行文件，每个文件同样有一个uid和gid，默认为创建该文件的用户的uid/gid
2. 可通过命令chown修改
3. 通过ls -l命名默认看到的是username和groupname, 实际上对应的应该是uid和gid
4. 如果修改/etc/passwd, 删除掉username, 则可以看到看到文件对应的user列变成了uid了。



## 进程uid和gid

linux访问权限控制是对进程进行控制的，每个进程也有一个uid和gid, 默认是运行进程的用户的uid和gid

## setuid和setgid

* 前面说到，linux是对进程进行权限验证，例如某个进程要访问一个文件 则需要验证进程的uid/gid是否满足该文件的访问权限
* 默认情况下进程的uid,gid是运行进程的用户的uid和gid
* 通过设置程序(进程对应的可执行文件) 的setuid和setgid，可以修改进程的uid和gid为程序的uid和gid
  * 例如修改某个user为root的程序的setuid位
  * 则任意用户执行该程序时，对应的进程的uid都是root uid, 也就是任何用户都可以root权限执行程序。

```
修改setuid和setgid:
chmod u+s filename
chmod g+s filename
```

