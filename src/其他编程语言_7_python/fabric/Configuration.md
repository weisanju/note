## Introduction



Invoke提供了一种多方面的配置机制，允许您通过配置文件、环境变量、任务名称空间和CLI标志的层次结构来配置核心行为和任务的行为。



配置搜索，加载，解析和合并的最终结果是一个Config对象，它的行为就像一个 (嵌套的) Python字典。Invoke在运行时引用此对象 (确定诸如Context.run之类的方法的默认行为)，并将其作为Context.config或 Context 本身的速记属性访问方式公开给用户的任务。





## The configuration hierarchy

简而言之，配置值相互覆盖的顺序如下:

1. **Internal default values** ：默认值

2. **Collection-driven configurations**   通过 [`Collection.configure`](https://docs.pyinvoke.org/en/latest/api/collection.html#invoke.collection.Collection.configure) 定义，Sub-collections的配置被合并到顶级集合中，最终结果构成了整个配置设置的基础。

3. **System-level configuration file** stored in `/etc/`, such as `/etc/invoke.yaml`

4. **User-level configuration file** found in the running user’s home directory, e.g. `~/.invoke.yaml`.

5. **Project-level configuration file** living next to your top level `tasks.py`. For example, if your run of Invoke loads `/home/user/myproject/tasks.py` (see our docs on [the load process](https://docs.pyinvoke.org/en/latest/concepts/loading.html)), this might be `/home/user/myproject/invoke.yaml`.

6. **Environment variables** found in the invoking shell environment.

   > - These aren’t as strongly hierarchical as the rest, nor is the shell environment namespace owned wholly by Invoke, so we must rely on slightly verbose prefixing instead - see [Environment variables](https://docs.pyinvoke.org/en/latest/concepts/configuration.html#env-vars) for details.

7. **Runtime configuration file** whose path is given to [`-f`](https://docs.pyinvoke.org/en/latest/invoke.html#cmdoption-f), e.g. `inv -f /random/path/to/config_file.yaml`. This path may also be set via the `INVOKE_RUNTIME_CONFIG` env var.

8. **Command-line flags** for certain core settings, such as [`-e`](https://docs.pyinvoke.org/en/latest/invoke.html#cmdoption-e).

9. **Modifications made by user code** at runtime.



## Default configuration values

下面列出了所有配置值和/或section Invoke本身用于控制行为的列表，例如Context.run的echo和pty标志，任务重复数据删除等。



这些值的存储位置在Config类内部，特别是Config.global_defaults的返回值; 有关更多详细信息，请参见其API文档。



For convenience, we refer to nested setting names with a dotted syntax, so e.g. `foo.bar` refers to what would be (in a Python config context) `{'foo': {'bar': <value here>}}`. Typically, these can be read or set on [`Config`](https://docs.pyinvoke.org/en/latest/api/config.html#invoke.config.Config) and [`Context`](https://docs.pyinvoke.org/en/latest/api/context.html#invoke.context.Context) objects using attribute syntax, which looks nearly identical: `c.foo.bar`.

为了方便起见，我们引用带有点语法的嵌套设置名称，例如foo.bar指的是 (在Python config上下文中) `{'foo': {'bar': <value here >}}`。通常，可以使用属性语法在Config和Context对象上读取或设置这些语法，这些语法看起来几乎相同: c.foo.bar。





任务配置树保存与任务执行相关的设置。

* `tasks.dedupe` controls [Task deduplication](https://docs.pyinvoke.org/en/latest/concepts/invoking-tasks.html#deduping) and defaults to `True`. It can also be overridden at runtime via [`--no-dedupe`](https://docs.pyinvoke.org/en/latest/invoke.html#cmdoption-no-dedupe).

* `tasks.auto_dash_names` 控制任务名称和集合名称是否已将下划线转到CLI上的破折号。 Default: `True`. See also [Dashes vs underscores](https://docs.pyinvoke.org/en/latest/concepts/namespaces.html#dashes-vs-underscores).

* `tasks.collection_name` controls the Python import name sought out by [collection discovery](https://docs.pyinvoke.org/en/latest/concepts/loading.html#collection-discovery), and defaults to `"tasks"`.

* `tasks.executor_class` allows users to override the class instantiated and used for task execution.

Must be a fully-qualified dotted path of the form `module(.submodule...).class`, where all but `.class` will be handed to [`importlib.import_module`](https://docs.python.org/2.7/library/importlib.html#importlib.import_module), and `class` is expected to be an attribute on that resulting module object.

Defaults to `None`, meaning to use the running [`Program`](https://docs.pyinvoke.org/en/latest/api/program.html#invoke.program.Program) object’s `executor_class` attribute.

Warning

Take care if using this setting in tandem with [custom program binaries](https://docs.pyinvoke.org/en/latest/concepts/library.html#reusing-as-a-binary), since custom programs may specify their own default executor class (which your use of this setting will override!) and assume certain behaviors stemming from that.

`tasks.search_root` allows overriding the default [collection discovery](https://docs.pyinvoke.org/en/latest/concepts/loading.html#collection-discovery) root search location. It defaults to `None`, which indicates to use the executing process’ current working directory.

- The `run` tree controls the behavior of [`Runner.run`](https://docs.pyinvoke.org/en/latest/api/runners.html#invoke.runners.Runner.run). Each member of this tree (such as `run.echo` or `run.pty`) maps directly to a [`Runner.run`](https://docs.pyinvoke.org/en/latest/api/runners.html#invoke.runners.Runner.run) keyword argument of the same name; see that method’s docstring for details on what these settings do & what their default values are.

- • 运行 • 树控制 • Runner.ru n • 的行为。此树的每个成员 (例如 • run.echo • 或 • run.pt y •) 直接映射到具有相同名称的 • Runner.ru n • 关键字参数; 有关这些设置的功能和默认值的详细信息，请参阅该方法的docstring。

- The `runners` tree controls _which_ runner classes map to which execution contexts; if you’re using Invoke by itself, this will only tend to have a single member, `runners.local`. Client libraries may extend it with additional key/value pairs, such as `runners.remote`.

- The `sudo` tree controls the behavior of [`Context.sudo`](https://docs.pyinvoke.org/en/latest/api/context.html#invoke.context.Context.sudo):

  > - `sudo.password` controls the autoresponse password submitted to sudo’s password prompt. Default: `None`.
  >
  >   Warning
  >
  >   While it’s possible to store this setting, like any other, in [configuration files](https://docs.pyinvoke.org/en/latest/concepts/configuration.html#) – doing so is inherently insecure. We highly recommend filling this config value in at runtime from a secrets management system of some kind.
  >
  > - `sudo.prompt` holds the sudo password prompt text, which is both supplied to `sudo -p`, and searched for when performing [auto-response](https://docs.pyinvoke.org/en/latest/concepts/watchers.html). Default: `[sudo] password:`.

- A top level config setting, `debug`, controls whether debug-level output is logged; it defaults to `False`.

  `debug` can be toggled via the [`-d`](https://docs.pyinvoke.org/en/latest/invoke.html#cmdoption-d) CLI flag, which enables debugging after CLI parsing runs. It can also be toggled via the `INVOKE_DEBUG` environment variable which - unlike regular env vars - is honored from the start of execution and is thus useful for troubleshooting parsing and/or config loading.

- A small config tree, `timeouts`, holds various kinds of timeout controls. At present, for Invoke, this only holds a `command` subkey, which controls subprocess execution timeouts.

  > - Client code often adds more to this tree, and Invoke itself may add more in the future as well.



## Configuration files

### Loading

For each configuration file location mentioned in the previous section, we search for files ending in `.yaml`, `.yml`, `.json` or `.py` (**in that order!**), load the first one we find, and ignore any others that might exist.

For example, if Invoke is run on a system containing both `/etc/invoke.yml` *and* `/etc/invoke.json`, **only the YAML file will be loaded**. This helps keep things simple, both conceptually and in the implementation.

### Format

Invoke’s configuration allows arbitrary nesting, and thus so do our config file formats. All three of the below examples result in a configuration equivalent to `{'debug': True, 'run': {'echo': True}}`:

- **YAML**

  ```
  debug: true
  run:
      echo: true
  ```

- **JSON**

  ```
  {
      "debug": true,
      "run": {
          "echo": true
      }
  }
  ```

- **Python**:

  ```
  debug = True
  run = {
      "echo": True
  }
  ```

For further details, see these languages’ own documentation.



## Environment variables

Environment variables are a bit different from other configuration-setting methods, since they don’t provide a clean way to nest configuration keys, and are also implicitly shared amongst the entire system’s installed application base.

In addition, due to implementation concerns, env vars must be pre-determined by the levels below them in the config hierarchy (in other words - env vars may only be used to override existing config values). If you need Invoke to understand a `FOOBAR` environment variable, you must first declare a `foobar` setting in a configuration file or in your task collections.

### Basic rules

To mitigate the shell namespace problem, we simply prefix all our env vars with `INVOKE_`.

Nesting is performed via underscore separation, so a setting that looks like e.g. `{'run': {'echo': True}}` at the Python level becomes `INVOKE_RUN_ECHO=1` in a typical shell. See [Nesting vs underscored names](https://docs.pyinvoke.org/en/latest/concepts/configuration.html#env-var-nesting) below for more on this.

### Type casting

Since env vars can only be used to override existing settings, the previous value of a given setting is used as a guide in casting the strings we get back from the shell:

- If the current value is a string or Unicode object, it is replaced with the value from the environment, with no casting whatsoever;

  > - Depending on interpreter and environment, this means that a setting defaulting to a non-Unicode string type (eg a `str` on Python 2) may end up replaced with a Unicode string, or vice versa. This is intentional as it prevents users from accidentally limiting themselves to non-Unicode strings.

- If the current value is `None`, it too is replaced with the string from the environment;

- Booleans are set as follows: `0` and the empty value/string (e.g. `SETTING=`, or `unset SETTING`, or etc) evaluate to `False`, and any other value evaluates to `True`.

- Lists and tuples are currently unsupported and will raise an exception;

  > - In the future we may implement convenience transformations, such as splitting on commas to form a list; however since users can always perform such operations themselves, it may not be a high priority.

- All other types - integers, longs, floats, etc - are simply used as constructors for the incoming value.

  > - For example, a `foobar` setting whose default value is the integer `1` will run all env var inputs through [`int`](https://docs.python.org/2.7/library/functions.html#int), and thus `FOOBAR=5` will result in the Python value `5`, not `"5"`.



### Nesting vs underscored names

Since environment variable keys are single strings, we must use some form of string parsing to allow access to nested configuration settings. As mentioned above, in basic use cases this just means using an underscore character: `{'run': {'echo': True}}` becomes `INVOKE_RUN_ECHO=1`.

However, ambiguity is introduced when the settings names themselves contain underscores: is `INVOKE_FOO_BAR=baz` equivalent to `{'foo': {'bar': 'baz'}}`, or to `{'foo_bar': 'baz'}`? Thankfully, because env vars can only be used to modify settings declared at the Python level or in config files, we look at the current state of the config to determine the answer.

There is still a corner case where *both* possible interpretations exist as valid config paths (e.g. `{'foo': {'bar': 'default'}, 'foo_bar': 'otherdefault'}`). In this situation, we honor the [Zen of Python](http://zen-of-python.info/in-the-face-of-ambiguity-refuse-the-temptation-to-guess.html#12) and refuse to guess; an error is raised instead, counseling users to modify their configuration layout or avoid using env vars for the setting in question.



## [`Collection`](https://docs.pyinvoke.org/en/latest/api/collection.html#invoke.collection.Collection)-based configuration

[`Collection`](https://docs.pyinvoke.org/en/latest/api/collection.html#invoke.collection.Collection) objects may contain a config mapping, set via [`Collection.configure`](https://docs.pyinvoke.org/en/latest/api/collection.html#invoke.collection.Collection.configure), and (as per [the hierarchy](https://docs.pyinvoke.org/en/latest/concepts/configuration.html#config-hierarchy)) this typically forms the lowest level of configuration in the system.

When collections are [nested](https://docs.pyinvoke.org/en/latest/concepts/namespaces.html), configuration is merged ‘downwards’ by default: when conflicts arise, outer namespaces closer to the root will win, versus inner ones closer to the task being invoked.

Note

‘Inner’ tasks here are specifically those on the path from the root to the one housing the invoked task. ‘Sibling’ subcollections are ignored.

A quick example of what this means:

```
from invoke import Collection, task

# This task & collection could just as easily come from
# another module somewhere.
@task
def mytask(c):
    print(c['conflicted'])
inner = Collection('inner', mytask)
inner.configure({'conflicted': 'default value'})

# Our project's root namespace.
ns = Collection(inner)
ns.configure({'conflicted': 'override value'})
```

The result of calling `inner.mytask`:

```
$ inv inner.mytask
override value
```

## Example of real-world config use

The previous sections had small examples within them; this section provides a more realistic-looking set of examples showing how the config system works.

### Setup

We’ll start out with semi-realistic tasks that hardcode their values, and build up to using the various configuration mechanisms. A small module for building [Sphinx](http://sphinx-doc.org/) docs might begin like this:

```
from invoke import task

@task
def clean(c):
    c.run("rm -rf docs/_build")

@task
def build(c):
    c.run("sphinx-build docs docs/_build")
```

Then maybe you refactor the build target:

```
target = "docs/_build"

@task
def clean(c):
    c.run("rm -rf {}".format(target))

@task
def build(c):
    c.run("sphinx-build docs {}".format(target))
```

We can also allow runtime parameterization:

```
default_target = "docs/_build"

@task
def clean(c, target=default_target):
    c.run("rm -rf {}".format(target))

@task
def build(c, target=default_target):
    c.run("sphinx-build docs {}".format(target))
```

This task module works for a single set of users, but what if we want to allow reuse? Somebody may want to use this module with a different default target. Using the configuration data (made available via the context arg) to configure these settings is usually the better solution [[1\]](https://docs.pyinvoke.org/en/latest/concepts/configuration.html#id3).

### Configuring via task collection

The configuration [`setting`](https://docs.pyinvoke.org/en/latest/api/collection.html#invoke.collection.Collection.configure) and [`getting`](https://docs.pyinvoke.org/en/latest/api/context.html#invoke.context.Context.config) APIs enable moving otherwise ‘hardcoded’ default values into a config structure which downstream users are free to redefine. Let’s apply this to our example. First we add an explicit namespace object:

```
from invoke import Collection, task

default_target = "docs/_build"

@task
def clean(c, target=default_target):
    c.run("rm -rf {}".format(target))

@task
def build(c, target=default_target):
    c.run("sphinx-build docs {}".format(target))

ns = Collection(clean, build)
```

Then we can move the default build target value into the collection’s default configuration, and refer to it via the context. At this point we also change our kwarg default value to be `None` so we can determine whether or not a runtime value was given. The result:

```
@task
def clean(c, target=None):
    if target is None:
        target = c.sphinx.target
    c.run("rm -rf {}".format(target))

@task
def build(c, target=None):
    if target is None:
        target = c.sphinx.target
    c.run("sphinx-build docs {}".format(target))

ns = Collection(clean, build)
ns.configure({'sphinx': {'target': "docs/_build"}})
```

The result isn’t significantly more complex than what we began with, and as we’ll see next, it’s now trivial for users to override your defaults in various ways.

### Configuration overriding

The lowest-level override is, of course, just modifying the local [`Collection`](https://docs.pyinvoke.org/en/latest/api/collection.html#invoke.collection.Collection) tree into which a distributed module has been imported. E.g. if the above module is distributed as `myproject.docs`, someone can define a `tasks.py` that does this:

```
from invoke import Collection, task
from myproject import docs

@task
def mylocaltask(c):
    # Some local stuff goes here
    pass

# Add 'docs' to our local root namespace, plus our own task
ns = Collection(mylocaltask, docs)
```

And then they can add this to the bottom:

```
# Our docs live in 'built_docs', not 'docs/_build'
ns.configure({'sphinx': {'target': "built_docs"}})
```

Now we have a `docs` sub-namespace whose build target defaults to `built_docs` instead of `docs/_build`. Runtime users can still override this via flags (e.g. `inv docs.build --target='some/other/dir'`) just as before.

If you prefer configuration files over in-Python tweaking of your namespace tree, that works just as well; instead of adding the line above to the previous snippet, instead drop this into a file next to `tasks.py` named `invoke.yaml`:

```
sphinx:
    target: built_docs
```

For this example, that sort of local-to-project conf file makes the most sense, but don’t forget that the [config hierarchy](https://docs.pyinvoke.org/en/latest/concepts/configuration.html#config-hierarchy) offers additional configuration methods which may be suitable depending on your needs.