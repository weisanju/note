# Welcome to Invoke!



该网站涵盖了Invoke的项目信息，例如变更日志，贡献指南，开发路线图，新闻/博客等。详细的用法和API文档可以在我们的代码文档网站 docs.pyinvoke.org 上找到。此外，项目维护者在他的网站上保留了路线图。
请参阅下面的高级介绍，或左侧的导航以获取网站的其余内容。



## What is Invoke?

Invoke是一个Python (2.7和3.4) 任务执行工具和库，从各种来源汲取灵感，得出一个强大而干净的功能集。

- 像Ruby的Rake工具和Invoke自己的前身Fabric 1.x一样，它提供了一个干净的高级API，用于运行shell命令并从tasks.py文件中定义/组织任务函数:

  ```python
  from invoke import task
  
  @task
  def clean(c, docs=False, bytecode=False, extra=''):
      patterns = ['build']
      if docs:
          patterns.append('docs/_build')
      if bytecode:
          patterns.append('**/*.pyc')
      if extra:
          patterns.append(extra)
      for pattern in patterns:
          c.run("rm -rf {}".format(pattern))
  
  @task
  def build(c, docs=False):
      c.run("python setup.py build")
      if docs:
          c.run("sphinx-build docs docs/_build")
  ```

- 从GNU Make开始，它继承了对通用模式的最小样板的强调，并能够在一次调用中运行多个任务:

  ```python
  $ invoke clean build
  ```

- 其中Fabric 1.x认为命令行方法的默认使用模式，Invoke (和建立在它上面的工具) 同样在家里嵌入你自己的Python代码或REPL::

  ```python
  >>> from invoke import run
  >>> cmd = "pip install -r requirements.txt"
  >>> result = run(cmd, hide=True, warn=True)
  >>> print(result.ok)
  True
  >>> print(result.stdout.splitlines()[-1])
  Successfully installed invocations-0.13.0 pep8-1.5.7 spec-1.3.1
  ```

- 在大多数Unix CLI应用程序的领导下，它提供了一种传统的基于标志的命令行解析风格，从任务签名中导出标志名称和值类型 (当然是可选的!):

  ```
  $ invoke clean --docs --bytecode build --docs --extra='**/*.pyo'
  $ invoke clean -d -b build --docs -e '**/*.pyo'
  $ invoke clean -db build -de '**/*.pyo'
  ```

- 像它的许多前辈一样，它也提供了高级功能– namespacing, task aliasing, before/after hooks, parallel execution and more.





# Getting started

教程/入门文档中解释了许多核心思想和API调用:Getting started

- 定义或运行任务：[Defining and running task functions](https://docs.pyinvoke.org/en/stable/getting-started.html#defining-and-running-task-functions)
- 任务参数：[Task parameters](https://docs.pyinvoke.org/en/stable/getting-started.html#task-parameters)
- 列出任务：[Listing tasks](https://docs.pyinvoke.org/en/stable/getting-started.html#listing-tasks)
- 运行shell任务：[Running shell commands](https://docs.pyinvoke.org/en/stable/getting-started.html#running-shell-commands)
- 申明任务：[Declaring pre-tasks](https://docs.pyinvoke.org/en/stable/getting-started.html#declaring-pre-tasks)
- 创建名称空间：[Creating namespaces](https://docs.pyinvoke.org/en/stable/getting-started.html#creating-namespaces)

## The `invoke` CLI tool

有关要调用的CLI接口、可用的核心标志和TAB补全：选项的详细信息。

- `inv[oke]` core usage
  - 核心标志：[Core options and flags](https://docs.pyinvoke.org/en/stable/invoke.html#core-options-and-flags)
  - Shell tab completion
    - [Generating a completion script](https://docs.pyinvoke.org/en/stable/invoke.html#generating-a-completion-script)
    - [Sourcing the script](https://docs.pyinvoke.org/en/stable/invoke.html#sourcing-the-script)
    - [Utilizing tab completion itself](https://docs.pyinvoke.org/en/stable/invoke.html#utilizing-tab-completion-itself)

## Concepts

**深入挖掘**

- Configuration
  - [Introduction](https://docs.pyinvoke.org/en/stable/concepts/configuration.html#introduction)
  - [The configuration hierarchy](https://docs.pyinvoke.org/en/stable/concepts/configuration.html#the-configuration-hierarchy)
  - [Default configuration values](https://docs.pyinvoke.org/en/stable/concepts/configuration.html#default-configuration-values)
  - [Configuration files](https://docs.pyinvoke.org/en/stable/concepts/configuration.html#configuration-files)
  - [Environment variables](https://docs.pyinvoke.org/en/stable/concepts/configuration.html#environment-variables)
  - [`Collection`-based configuration](https://docs.pyinvoke.org/en/stable/concepts/configuration.html#collection-based-configuration)
  - [Example of real-world config use](https://docs.pyinvoke.org/en/stable/concepts/configuration.html#example-of-real-world-config-use)
- Invoking tasks
  - [Basic command line layout](https://docs.pyinvoke.org/en/stable/concepts/invoking-tasks.html#basic-command-line-layout)
  - [Task command-line arguments](https://docs.pyinvoke.org/en/stable/concepts/invoking-tasks.html#task-command-line-arguments)
  - [How tasks run](https://docs.pyinvoke.org/en/stable/concepts/invoking-tasks.html#how-tasks-run)
- Using Invoke as a library
  - [Reusing Invoke’s CLI module as a distinct binary](https://docs.pyinvoke.org/en/stable/concepts/library.html#reusing-invoke-s-cli-module-as-a-distinct-binary)
  - [Customizing the configuration system’s defaults](https://docs.pyinvoke.org/en/stable/concepts/library.html#customizing-the-configuration-system-s-defaults)
- Loading collections
  - [Task module discovery](https://docs.pyinvoke.org/en/stable/concepts/loading.html#task-module-discovery)
  - [Configuring the loading process](https://docs.pyinvoke.org/en/stable/concepts/loading.html#configuring-the-loading-process)
- Constructing namespaces
  - [Starting out](https://docs.pyinvoke.org/en/stable/concepts/namespaces.html#starting-out)
  - [Naming your tasks](https://docs.pyinvoke.org/en/stable/concepts/namespaces.html#naming-your-tasks)
  - [Nesting collections](https://docs.pyinvoke.org/en/stable/concepts/namespaces.html#nesting-collections)
  - [Importing modules as collections](https://docs.pyinvoke.org/en/stable/concepts/namespaces.html#importing-modules-as-collections)
  - [Default tasks](https://docs.pyinvoke.org/en/stable/concepts/namespaces.html#default-tasks)
  - [Mix and match](https://docs.pyinvoke.org/en/stable/concepts/namespaces.html#mix-and-match)
  - [More shortcuts](https://docs.pyinvoke.org/en/stable/concepts/namespaces.html#more-shortcuts)
- Testing Invoke-using codebases
  - [Subclass & modify Invoke ‘internals’](https://docs.pyinvoke.org/en/stable/concepts/testing.html#subclass-modify-invoke-internals)
  - [Use `MockContext`](https://docs.pyinvoke.org/en/stable/concepts/testing.html#use-mockcontext)
  - [Expect `Results`](https://docs.pyinvoke.org/en/stable/concepts/testing.html#expect-results)
  - [Avoid mocking dependency code paths altogether](https://docs.pyinvoke.org/en/stable/concepts/testing.html#avoid-mocking-dependency-code-paths-altogether)
- Automatically responding to program output
  - [Background](https://docs.pyinvoke.org/en/stable/concepts/watchers.html#background)
  - [Basic use](https://docs.pyinvoke.org/en/stable/concepts/watchers.html#basic-use)



## API

- [`__init__`](https://docs.pyinvoke.org/en/stable/api/__init__.html)
- [`collection`](https://docs.pyinvoke.org/en/stable/api/collection.html)
- [`config`](https://docs.pyinvoke.org/en/stable/api/config.html)
- [`context`](https://docs.pyinvoke.org/en/stable/api/context.html)
- [`exceptions`](https://docs.pyinvoke.org/en/stable/api/exceptions.html)
- [`executor`](https://docs.pyinvoke.org/en/stable/api/executor.html)
- [`loader`](https://docs.pyinvoke.org/en/stable/api/loader.html)
- [`parser`](https://docs.pyinvoke.org/en/stable/api/parser.html)
- [`program`](https://docs.pyinvoke.org/en/stable/api/program.html)
- [`runners`](https://docs.pyinvoke.org/en/stable/api/runners.html)
- [`tasks`](https://docs.pyinvoke.org/en/stable/api/tasks.html)
- [`terminals`](https://docs.pyinvoke.org/en/stable/api/terminals.html)
- [`util`](https://docs.pyinvoke.org/en/stable/api/util.html)
- [`watchers`](https://docs.pyinvoke.org/en/stable/api/watchers.html)







