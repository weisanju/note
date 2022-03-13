## Fabric 是什么？

Python部署工具

> Fabric 是一个 Python (2.5-2.7) 的库和命令行工具，用来提高基于 SSH 的应用部署和系统管理效率。





## Hello, `fab`

```python
def hello():
    print("Hello world!")
```

把上述代码放在你当前的工作目录中一个名为 `fabfile.py` 的 Python 模块文件中。

然后这个 `hello` 函数就可以用 `fab` 工具（随 Fabric 一并安装的命令）来执行了，输出的结果会是这样：

```shell
$ fab hello
Hello world!

Done.
```



把上述代码放在你当前的工作目录中一个名为 `fabfile.py` 的 Python 模块文件中。然后这个 `hello` 函数就可以用 `fab` 工具（随 Fabric 一并安装的命令）来执行了，输出的结果会是这样：



`fab` 工具所做的只是导入 fabfile 并执行了相应一个或多个的函数，这里并没有任何魔法——任何你能在一个普通 Python 模块中做的事情同样可以在一个 fabfile 中完成。



## 任务参数

和你平时的 Python 编程一样，给任务函数传递参数很有必要``。Fabric 支持 Shell 兼容的参数用法： `<任务名>:<参数>, <关键字参数名>=<参数值>,...` 用起来就是这样，下面我们用一个 say hello 的实例来展开说明一下：

```
def hello(name="world"):
    print("Hello %s!" % name)
```

```python
$ fab hello:name=Jeff
Hello Jeff!

Done.
```



## 本地命令

Fabric 的设计目的更是为了使用它自己的 API，包括执行 Shell 命令、传送文件等函数（或操作）接口。

假设我们需要为一个 web 应用创建 fabfile 。具体的情景如下：这个 web 应用的代码使用 git 托管在一台远程服务器 `vcshost` 上，我们把它的代码库克隆到了本地 `localhost` 中。

我们希望在我们把修改后的代码 push 回 vcshost 时，自动把新的版本安装到另一台远程服务器 `my_server` 上

我们将通过自动化本地和远程 git 命令来完成这些工作。



关于 fabfile 文件放置位置的最佳时间是项目的根目录：

```python
.
|-- __init__.py
|-- app.wsgi
|-- fabfile.py <-- our fabfile!
|-- manage.py
`-- my_app
    |-- __init__.py
    |-- models.py
    |-- templates
    |   `-- index.html
    |-- tests.py
    |-- urls.py
    `-- views.py
```

作为起步，我们希望先执行测试准备好部署后，再提交到 VCS（版本控制系统）：

```python
from fabric.api import local

def prepare_deploy():
    local("./manage.py test my_app")
    local("git add -p && git commit")
    local("git push")
```

这段代码很简单，导入一个 Fabric API： [`local`](https://fabric-chs.readthedocs.io/zh_CN/chs/api/core/operations.html#fabric.operations.local) ，然后用它执行本地 Shell 命令并与之交互，剩下的 Fabric API 也都类似——它们都只是 Python。

## 用你的方式来组织

比如说，把任务分割成多个子任务：

```Python
from fabric.api import local

def test():
    local("./manage.py test my_app")

def commit():
    local("git add -p && git commit")

def push():
    local("git push")

def prepare_deploy():
    test()
    commit()
    push()
```

这个 `prepare_deploy` 任务仍可以像之前那样调用，但现在只要你愿意，就可以调用更细粒度的子任务。

## 故障

我们的基本案例已经可以正常工作了，但如果测试失败了会怎样？我们应该抓住机会即使停下任务，并在部署之前修复这些失败的测试。

Fabric 会检查被调用程序的返回值，如果这些程序没有干净地退出，Fabric 会终止操作。下面我们就来看看如果一个测试用例遇到错误时会发生什么：

```shell
$ fab prepare_deploy
[localhost] run: ./manage.py test my_app
Creating test database...
Creating tables
Creating indexes
.............E............................
======================================================================
ERROR: testSomething (my_project.my_app.tests.MainTests)
----------------------------------------------------------------------
Traceback (most recent call last):
[...]

----------------------------------------------------------------------
Ran 42 tests in 9.138s

FAILED (errors=1)
Destroying test database...

Fatal error: local() encountered an error (return code 2) while executing './manage.py test my_app'

Aborting.
```

但如果我们想更加灵活，给用户另一个选择，该怎么办？一个名为 [warn_only](https://fabric-chs.readthedocs.io/zh_CN/chs/usage/env.html#warn-only) 的设置（或着说 **环境变量** ，通常缩写为 **env var** ）可以把退出换为警告，以提供更灵活的错误处理。

让我们把这个设置丢到 `test` 函数中，然后注意这个 [`local`](https://fabric-chs.readthedocs.io/zh_CN/chs/api/core/operations.html#fabric.operations.local) 调用的结果：



```python
from __future__ import with_statement
from fabric.api import local, settings, abort
from fabric.contrib.console import confirm

def test():
    with settings(warn_only=True):
        result = local('./manage.py test my_app', capture=True)
    if result.failed and not confirm("Tests failed. Continue anyway?"):
        abort("Aborting at user request.")

[...]
```

```python
from __future__ import with_statement
from fabric.api import local, settings, abort
from fabric.contrib.console import confirm

def test():
    with settings(warn_only=True):
        result = local('./manage.py test my_app', capture=True)
    if result.failed and not confirm("Tests failed. Continue anyway?"):
        abort("Aborting at user request.")

[...]
```

为了引入这个新特性，我们需要添加一些新东西：

- 在 Python 2.5 中，需要从 `__future__` 中导入 `with` ；
- Fabric [`contrib.console`](https://fabric-chs.readthedocs.io/zh_CN/chs/api/contrib/console.html#module-fabric.contrib.console) 子模块提供了 [`confirm`](https://fabric-chs.readthedocs.io/zh_CN/chs/api/contrib/console.html#fabric.contrib.console.confirm) 函数，用于简单的 yes/no 提示。
- [`settings`](https://fabric-chs.readthedocs.io/zh_CN/chs/api/core/context_managers.html#fabric.context_managers.settings) 上下文管理器提供了特定代码块特殊设置的功能。
- [`local`](https://fabric-chs.readthedocs.io/zh_CN/chs/api/core/operations.html#fabric.operations.local) 这样运行命令的操作会返回一个包含执行结果（ `.failed` 或 `.return_code` 属性）的对象。
- [`abort`](https://fabric-chs.readthedocs.io/zh_CN/chs/api/core/utils.html#fabric.utils.abort) 函数用于手动停止任务的执行。

## 建立连接

让我们回到 fabfile 的主旨：定义一个 `deploy` 任务，让它在一台或多台远程服务器上运行，并保证代码是最新的：

```python
def deploy():
    code_dir = '/srv/django/myproject'
    with cd(code_dir):
        run("git pull")
        run("touch app.wsgi")
```

这里再次引入了一些新的概念：

- Fabric 是 Python——所以我们可以自由地使用变量、字符串等常规的 Python 代码；
- [`cd`](https://fabric-chs.readthedocs.io/zh_CN/chs/api/core/context_managers.html#fabric.context_managers.cd) 函数是一个简易的前缀命令，相当于运行 `cd /to/some/directory` ，和 [`lcd`](https://fabric-chs.readthedocs.io/zh_CN/chs/api/core/context_managers.html#fabric.context_managers.lcd) 函数类似，只不过后者是在本地执行。
- ~fabric.operations.run和 [`local`](https://fabric-chs.readthedocs.io/zh_CN/chs/api/core/operations.html#fabric.operations.local) 类似，不过是在 **远程** 而非本地执行。



我们还需要保证在文件顶部导入了这些新函数：

```python
from __future__ import with_statement
from fabric.api import local, settings, abort, run, cd
from fabric.contrib.console import confirm
```

```python
$ fab deploy
No hosts found. Please specify (single) host string for connection: my_server
[my_server] run: git pull
[my_server] out: Already up-to-date.
[my_server] out:
[my_server] run: touch app.wsgi

Done.
```

我们并没有在 fabfile 中指定任何连接信息，所以 Fabric 依旧不知道该在哪里运行这些远程命令。遇到这种情况时，Fabric 会在运行时提示我们。连接的定义使用 SSH 风格的“主机串”（例如： [user@host](mailto:user@host):port ），默认使用你的本地用户名——所以在这个例子中，我们只需要指定主机名 `my_server` 。



### 与远程交互

如果你已经得到了代码，说明 `git pull` 执行非常顺利——但如果这是第一次部署呢？最好也能应付这样的情况，这时应该使用 `git clone` 来初始化代码库：

```python
def deploy():
    code_dir = '/srv/django/myproject'
    with settings(warn_only=True):
        if run("test -d %s" % code_dir).failed:
            run("git clone user@vcshost:/path/to/repo/.git %s" % code_dir)
    with cd(code_dir):
        run("git pull")
        run("touch app.wsgi")
```



和上面调用 [`local`](https://fabric-chs.readthedocs.io/zh_CN/chs/api/core/operations.html#fabric.operations.local) 一样， [`run`](https://fabric-chs.readthedocs.io/zh_CN/chs/api/core/operations.html#fabric.operations.run) 也提供基于 Shell 命令构建干净的 Python 逻辑。

**git交互**

这里最有趣的部分是 `git clone` ：因为我们是用 git 的 SSH 方法来访问 git 服务器上的代码库，这意味着我们远程执行的 [`run`](https://fabric-chs.readthedocs.io/zh_CN/chs/api/core/operations.html#fabric.operations.run) 需要自己提供身份验证。

旧版本的 Fabric（和其他类似的高层次 SSH 库）像在监狱里一样运行远程命令，无法提供本地交互。当你迫切需要输入密码或者与远程程序交互时，这就很成问题。

Fabric 1.0 和后续的版本突破了这个限制，并保证你和另一端的会话交互。让我们看看当我们在一台没有 git checkout 的新服务器上运行更新后的 deploy 任务时会发生什么：

```shell
$ fab deploy
No hosts found. Please specify (single) host string for connection: my_server
[my_server] run: test -d /srv/django/myproject

Warning: run() encountered an error (return code 1) while executing 'test -d /srv/django/myproject'

[my_server] run: git clone user@vcshost:/path/to/repo/.git /srv/django/myproject
[my_server] out: Cloning into /srv/django/myproject...
[my_server] out: Password: <enter password>
[my_server] out: remote: Counting objects: 6698, done.
[my_server] out: remote: Compressing objects: 100% (2237/2237), done.
[my_server] out: remote: Total 6698 (delta 4633), reused 6414 (delta 4412)
[my_server] out: Receiving objects: 100% (6698/6698), 1.28 MiB, done.
[my_server] out: Resolving deltas: 100% (4633/4633), done.
[my_server] out:
[my_server] run: git pull
[my_server] out: Already up-to-date.
[my_server] out:
[my_server] run: touch app.wsgi

Done.
```

注意那个 `Password:` 提示——那就是我们在 web 服务器上的远程 `git` 应用在请求 git 密码。我们可以在本地输入密码，然后像往常一样继续克隆。

参见

[*与远程程序集成*](https://fabric-chs.readthedocs.io/zh_CN/chs/usage/interactivity.html)



### 预定义连接

在运行输入连接信息已经是非常古老的做法了，Fabric 提供了一套在 fabfile 或命令行中指定服务器信息的简单方法

这里我们不展开说明，但是会展示最常用的方法：设置全局主机列表 [env.hosts](https://fabric-chs.readthedocs.io/zh_CN/chs/usage/env.html#hosts) 。



[*env*](https://fabric-chs.readthedocs.io/zh_CN/chs/usage/env.html) 是一个全局的类字典对象，是 Fabric 很多设置的基础，也能在 with 表达式中使用（事实上，前面见过的 `~fabric.context_managers.settings` 就是它的一个简单封装）。因此，我们可以在模块层次上，在 fabfile 的顶部附近修改它，就像这样：



```python
from __future__ import with_statement
from fabric.api import *
from fabric.contrib.console import confirm

env.hosts = ['my_server']

def test():
    do_test_stuff()
```



当 `fab` 加载 fabfile 时，将会执行我们对 `env` 的修改并保存设置的变化。最终结果如上所示：我们的 `deploy` 任务将在 `my_server` 上运行。



这就是如何指定 Fabric 一次性控制多台远程服务器的方法： `env.hosts` 是一个列表， `fab` 对它迭代，对每个连接运行指定的任务。



参见

[*环境字典 env*](https://fabric-chs.readthedocs.io/zh_CN/chs/usage/env.html), [How host lists are constructed](https://fabric-chs.readthedocs.io/zh_CN/chs/usage/execution.html#host-lists)



## 总结

虽然经历了很多，我们的 fabfile 文件仍然相当短。下面是它的完整内容：

```python
from __future__ import with_statement
from fabric.api import *
from fabric.contrib.console import confirm

env.hosts = ['my_server']

def test():
    with settings(warn_only=True):
        result = local('./manage.py test my_app', capture=True)
    if result.failed and not confirm("Tests failed. Continue anyway?"):
        abort("Aborting at user request.")

def commit():
    local("git add -p && git commit")

def push():
    local("git push")

def prepare_deploy():
    test()
    commit()
    push()

def deploy():
    code_dir = '/srv/django/myproject'
    with settings(warn_only=True):
        if run("test -d %s" % code_dir).failed:
            run("git clone user@vcshost:/path/to/repo/.git %s" % code_dir)
    with cd(code_dir):
        run("git pull")
        run("touch app.wsgi")
```



但它已经涉及到了 Fabric 中的很多功能：

- 定义 fabfile 任务，并用 [*fab*](https://fabric-chs.readthedocs.io/zh_CN/chs/usage/fab.html) 执行；
- 用 [`local`](https://fabric-chs.readthedocs.io/zh_CN/chs/api/core/operations.html#fabric.operations.local) 调用本地 shell 命令；
- 通过 [`settings`](https://fabric-chs.readthedocs.io/zh_CN/chs/api/core/context_managers.html#fabric.context_managers.settings) 修改 env 变量；
- 处理失败命令、提示用户、手动取消任务；
- 以及定义主机列表、使用 [`run`](https://fabric-chs.readthedocs.io/zh_CN/chs/api/core/operations.html#fabric.operations.run) 来执行远程命令。

还有更多这里没有涉及到的内容，你还可以看看所有“参见”中的链接，以及 [*索引页*](https://fabric-chs.readthedocs.io/zh_CN/chs/index.html) 的内容表。