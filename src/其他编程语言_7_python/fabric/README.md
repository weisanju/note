## 关于导入

Fabric由几个库组成，提供统一的接口层

用户代码 可以从 fabric中导入包。也可以直接从 *invoke* *paramiko* 库中导入



* [Invoke](https://www.pyinvoke.org/) 实现命令行参数解析,组织任务，shell命令执行 (一个通用框架，且实现了本地命令执行.)
  * Anything that isn’t specific to remote systems tends to live in Invoke, and it is often used standalone by programmers who don’t need any remote functionality.
  * Fabric users will frequently import Invoke objects, in cases where Fabric itself has no need to subclass or otherwise modify what Invoke provides.

- [Paramiko](https://www.paramiko.org/) implements low/mid level SSH functionality - SSH and SFTP sessions, key management, etc.
  * Fabric mostly uses this under the hood; users will only rarely import from Paramiko directly.
- Fabric将其他 库 粘合起来 提供 高层的抽象
  - Subclassing Invoke’s context and command-runner classes, wrapping them around Paramiko-level primitives;
  - Extending Invoke’s configuration system by using Paramiko’s `ssh_config` parsing machinery;
  - Implementing new high-level primitives of its own, such as port-forwarding context managers. (These may, in time, migrate downwards into Paramiko.)







## Run commands via Connections and `run`

Fabric 最基本的用途是通过 SSH 在远程系统上执行 shell 命令，然后（可选）询问结果。默认情况下，远程程序的输出直接打印到终端并捕获。一个基本示例：

```python
>>> from fabric import Connection
>>> c = Connection('web1')
>>> result = c.run('uname -s')
Linux
>>> result.stdout.strip() == 'Linux'
True
>>> result.exited
0
>>> result.ok
True
>>> result.command
'uname -s'
>>> result.connection
<Connection host=web1>
>>> result.connection.host
'web1'
```

```shell
Connection(host='web1', user='deploy', port=2202)

Connection('deploy@web1:2202')
```

*Connection* 对象的run方法通常返回  `invoke.runners.Result` 或其子类

**注意**：Many lower-level SSH connection arguments (such as private keys and timeouts) can be given directly to the SSH backend by using the [connect_kwargs argument](https://docs.fabfile.org/en/2.6/api/connection.html#connect-kwargs-arg).



## Superuser privileges via auto-response

>  超级管理员

需要以远程系统的超级用户身份运行操作？您可以通过运行调用 sudo 程序，并且（如果您的远程系统未配置无密码 sudo）手动响应密码提示，如下所示。（请注意我们需要如何请求远程伪终端;否则，大多数 sudo 实现在密码提示时会变得脾气暴躁。)



```python
>>> from fabric import Connection
>>> c = Connection('db1')
>>> c.run('sudo useradd mydbuser', pty=True)
[sudo] password:
<Result cmd='sudo useradd mydbuser' exited=0>
>>> c.run('id -u mydbuser')
1001
<Result cmd='id -u mydbuser' exited=0>
```



每次手动提供密码已经过时了

值得庆幸的是，Invoke强大的命令执行功能包括使用预定义输入自动响应程序输出的功能。

我们可以将其用于sudo：

```python
>>> from invoke import Responder
>>> from fabric import Connection
>>> c = Connection('host')
>>> sudopass = Responder(
...     pattern=r'\[sudo\] password:',
...     response='mypassword\n',
... )
>>> c.run('sudo whoami', pty=True, watchers=[sudopass])
[sudo] password:
root
<Result cmd='sudo whoami' exited=0>
```

### The `sudo` helper

使用观察器/响应程序在这里效果很好，但每次都需要设置很多样板 - 特别是因为实际用例需要更多的工作来检测失败/不正确的密码。



Invoke提供了一个Context.sudo方法

用户需要做的就是确保填写 sudo.password 配置值（通过配置文件、环境变量或 --prompt-for-sudo-password），然后 Connection.sudo 处理其余部分。为清楚起见，下面是一个示例，其中库/shell 用户执行自己的基于 getpass 的密码提示：

```python
>>> import getpass
>>> from fabric import Connection, Config
>>> sudo_pass = getpass.getpass("What's your sudo password?")
What's your sudo password?
>>> config = Config(overrides={'sudo': {'password': sudo_pass}})
>>> c = Connection('db1', config=config)
>>> c.sudo('whoami', hide='stderr')
root
<Result cmd="...whoami" exited=0>
>>> c.sudo('useradd mydbuser')
<Result cmd="...useradd mydbuser" exited=0>
>>> c.run('id -u mydbuser')
1001
<Result cmd='id -u mydbuser' exited=0>
```

## Transfer files

除了 shell 命令执行之外，SSH 连接的另一个常见用途是文件传输;Connection.put 和 Connection.get exist 可以满足这一需求。例如，假设您有一个要上传的归档文件：

```python
>>> from fabric import Connection
>>> result = Connection('web1').put('myfiles.tgz', remote='/opt/mydata/')
>>> print("Uploaded {0.local} to {0.remote}".format(result))
Uploaded /local/myfiles.tgz to /opt/mydata/
```

这些方法通常在参数评估方面遵循 cp 和 scp/sftp 的行为 - 例如，在上面的代码片段中，我们省略了远程路径参数的文件名部分。



## Multiple actions

单行线是很好的例子，但并不总是现实的用例 - 通常需要多个步骤来做任何有趣的事情。在最基本的级别上，您可以通过多次调用 Connection 方法来执行此操作：



```python
from fabric import Connection
c = Connection('web1')
c.put('myfiles.tgz', '/opt/mydata')
c.run('tar -C /opt/mydata -xzvf /opt/mydata/myfiles.tgz')
```

您可以（但不必）将此类代码块转换为函数，并使用调用方的 Connection 对象进行参数化，以鼓励重用：

```python
def upload_and_unpack(c):
    c.put('myfiles.tgz', '/opt/mydata')
    c.run('tar -C /opt/mydata -xzvf /opt/mydata/myfiles.tgz')
```

正如您将在下面看到的，这些函数可以交给其他API方法，以实现更复杂的用例。

## Multiple servers

大多数实际用例都涉及在多个服务器上执行操作。简单的方法可以循环访问连接参数的列表或元组（或连接对象本身，也许通过map）：

```python
>>> from fabric import Connection
>>> for host in ('web1', 'web2', 'mac1'):
...     result = Connection(host).run('uname -s')
...     print("{}: {}".format(host, result.stdout.strip()))
...
...
web1: Linux
web2: Linux
mac1: Darwin
```

这种方法是有效的，但随着用例变得越来越复杂，将主机集合视为单个对象会很有用。

输入 Group，这是一个包装一个或多个连接对象并提供类似 API 的类;

具体来说，您将需要使用其具体的子类之一，如 SerialGroup 或 ThreadingGroup。

The previous example, using [`Group`](https://docs.fabfile.org/en/2.6/api/group.html#fabric.group.Group) ([`SerialGroup`](https://docs.fabfile.org/en/2.6/api/group.html#fabric.group.SerialGroup) specifically), looks like this:

```python
>>> from fabric import SerialGroup as Group
>>> results = Group('web1', 'web2', 'mac1').run('uname -s')
>>> print(results)
<GroupResult: {
    <Connection 'web1'>: <CommandResult 'uname -s'>,
    <Connection 'web2'>: <CommandResult 'uname -s'>,
    <Connection 'mac1'>: <CommandResult 'uname -s'>,
}>
>>> for connection, result in results.items():
...     print("{0.host}: {1.stdout}".format(connection, result))
...
...
web1: Linux
web2: Linux
mac1: Darwin
```

如果连接方法返回单个 Result 对象（例如 fabric.runners.Result），则 Group 方法返回 GroupResult -类似 dict 的对象，提供对单个每个连接结果的访问权限以及有关整个运行的元数据。



当组内的任何单个连接遇到错误时，GroupResult 会略微包装在组异常中，该异常将引发。因此，聚合行为类似于各个 Connection 方法的行为，在成功时返回值或在失败时引发异常。

## Bringing it all together

最后，我们得出了最实际的用例：您有一堆命令和/或文件传输，并且希望将其应用于多个服务器。您可以使用多个 Group 方法调用来执行此操作：



```python
from fabric import SerialGroup as Group
pool = Group('web1', 'web2', 'web3')
pool.put('myfiles.tgz', '/opt/mydata')
pool.run('tar -C /opt/mydata -xzvf /opt/mydata/myfiles.tgz')
```

一旦逻辑变得必要，这种方法就会失效 - 例如，如果您只想在 /opt/mydata 为空时执行上面的 copy-and-untar。执行此类检查需要基于每个服务器执行。

您可以通过使用连接对象的可迭代对象来满足该需求（尽管这放弃了使用组的一些好处）：

```python
from fabric import Connection
for host in ('web1', 'web2', 'web3'):
    c = Connection(host)
    if c.run('test -f /opt/mydata/myfile', warn=True).failed:
        c.put('myfiles.tgz', '/opt/mydata')
        c.run('tar -C /opt/mydata -xzvf /opt/mydata/myfiles.tgz')
```

或者，还记得我们在前面的示例中如何使用函数吗？您可以改为走这条路：

```python
from fabric import SerialGroup as Group

def upload_and_unpack(c):
    if c.run('test -f /opt/mydata/myfile', warn=True).failed:
        c.put('myfiles.tgz', '/opt/mydata')
        c.run('tar -C /opt/mydata -xzvf /opt/mydata/myfiles.tgz')

for connection in Group('web1', 'web2', 'web3'):
    upload_and_unpack(connection)
```

最后一种方法缺乏的唯一便利性是 Group.run 的有用类似物 - 如果要将所有upload_and_unpack调用的结果作为聚合进行跟踪，则必须自己执行此操作。期待未来的功能版本，以获取有关此领域的更多内容！





## Addendum: the `fab` command-line tool

> 附录：

从 shell 运行 Fabric 代码通常很有用，例如，在任意服务器上部署应用程序或运行 sysadmin 作业。您可以使用带有Fabric库代码的常规Invine任务，但另一个选项是Fabric自己的"面向网络"工具fab。



fab 将 Invoke 的 CLI 机制与主机选择等功能相结合，让您在各种服务器上快速运行任务 - 无需在所有任务或类似任务上定义主机 kwargs。



对于最后一个代码示例，让我们将前面的示例改编为一个名为 fabfile.py 的 fab 任务模块：

```python
from fabric import task

@task
def upload_and_unpack(c):
    if c.run('test -f /opt/mydata/myfile', warn=True).failed:
        c.put('myfiles.tgz', '/opt/mydata')
        c.run('tar -C /opt/mydata -xzvf /opt/mydata/myfiles.tgz')
```

这并不难 - 我们所做的只是将临时任务函数复制到一个文件中，并在其上打上装饰器。任务告诉 CLI 机器在命令行上公开任务：

```python
$ fab --list
Available tasks:

  upload_and_unpack
```

然后，当fab实际调用任务时，它知道如何将控制目标服务器的参数拼接在一起，并在每个服务器上运行一次任务。在单个服务器上运行一次任务：



```shell
发生这种情况时，任务内部的 c 将有效地设置为 Connection（"web1"） - 如前面的示例所示。同样，您可以为多个主机提供多个主机，该主机多次运行任务，每次都交出不同的连接实例：


fab -H web1 upload_and_unpack
```

```
$ fab -H web1,web2,web3 upload_and_unpack
```