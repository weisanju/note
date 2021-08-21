[GIT文档官网](https://git-scm.com/book/zh/)

# 获取GIT仓库

一般有两种方式获取git仓库

* 将尚未进行版本控制的本地目录转换为 Git 仓库；
* 从其它服务器 **克隆** 一个已存在的 Git 仓库。

## 已存在目录初始化仓库

```shell
#初始化
git init
#添加文件
git add *.c
#提交
git commit -m
```

## 远程库克隆

```shell
git clone <url>
#这会在当前目录下创建一个名为 “libgit2” 的目录
git clone https://github.com/libgit2/libgit2
#自定义目录名
git clone https://github.com/libgit2/libgit2 mylibgit
#使用git协议或者 ssh协议
user@server:path/to/repo.git
```



# 记录文件更新到仓库

## git文件状态

工作目录下的每一个文件都不外乎这两种状态: 已跟踪,未跟踪

* untracked
* unmodified
* modified
* staged

查看状态

```shell
git status
#查看紧凑的状态
git status -s
```

## 忽略文件.gitignore

### gitingore格式规范

* 所有空行或者以 `#` 开头的行都会被 Git 忽略。
* 可以使用标准的 glob 模式匹配，它会递归地应用在整个工作区中。
* 匹配模式可以以（`/`）开头防止递归。
* 匹配模式可以以（`/`）结尾指定目录。
* 要忽略指定模式以外的文件或目录，可以在模式前加上叹号（`!`）取反。

### gitignore pattern

所谓的 glob 模式是指 shell 所使用的简化了的正则表达式。

* 星号匹配任意多个字符
* [abc]匹配任何在方括号的字符
* ? 号一个任意字符
* [0-9]表示范围
* ** 表示匹配任意中间目录 a/**/z



## 对比差异

```shell
#只显示尚未暂存的改动
git diff
#查看已经暂存的变化
git diff --staged
```



## 提交

```
git commit

//跳过使用暂存区域,自动把所有已经跟踪过的文件暂存起来一并提交
git commit -A
```



## 移除文件

```shell
#删除工作区的改动文件
rm file #暂存区-> untracked
#删除git记录
git rm file 

#保留文件不想让git继续跟踪
git rm --cached file # tracked -> untracked

#强制删除文件
git rm -f filename
```



## 重命名

```
git mv README.md README
命令等价于三个命令

 mv README.md README
 git rm README.md
 git add README
```



# 查看提交历史

## git log常用选项

| 选项              | 说明                                                         |
| :---------------- | :----------------------------------------------------------- |
| `-p`              | 按补丁格式显示每个提交引入的差异。                           |
| `--stat`          | 显示每次提交的文件修改统计信息。                             |
| `--shortstat`     | 只显示 --stat 中最后的行数修改添加移除统计。                 |
| `--name-only`     | 仅在提交信息后显示已修改的文件清单。                         |
| `--name-status`   | 显示新增、修改、删除的文件清单。                             |
| `--abbrev-commit` | 仅显示 SHA-1 校验和所有 40 个字符中的前几个字符。            |
| `--relative-date` | 使用较短的相对时间而不是完整格式显示日期（比如“2 weeks ago”）。 |
| `--graph`         | 在日志旁以 ASCII 图形显示分支与合并历史。                    |
| `--pretty`        | 使用其他格式显示历史提交信息。可用的选项包括 oneline、short、full、fuller 和 format（用来定义自己的格式）。 |
| `--oneline`       | `--pretty=oneline --abbrev-commit` 合用的简写。              |

## pretty format格式

```console
 git log --pretty=format:"%h - %an, %ar : %s"
```

| 选项  | 说明                                          |
| :---- | :-------------------------------------------- |
| `%H`  | 提交的完整哈希值                              |
| `%h`  | 提交的简写哈希值                              |
| `%T`  | 树的完整哈希值                                |
| `%t`  | 树的简写哈希值                                |
| `%P`  | 父提交的完整哈希值                            |
| `%p`  | 父提交的简写哈希值                            |
| `%an` | 作者名字                                      |
| `%ae` | 作者的电子邮件地址                            |
| `%ad` | 作者修订日期（可以用 --date=选项 来定制格式） |
| `%ar` | 作者修订日期，按多久以前的方式显示            |
| `%cn` | 提交者的名字                                  |
| `%ce` | 提交者的电子邮件地址                          |
| `%cd` | 提交日期                                      |
| `%cr` | 提交日期（距今多长时间）                      |
| `%s`  | 提交说明                                      |



## 形象展示分支合并历史

```console
 git log --pretty=format:"%h %s" --graph
```



## 限制输出内容与长度

### 查看最近的2条

*git log -2* 

### 时间限制



* 绝对日期

可以使用 这个日期 2008-01-15

* 相对日期

也可以是类似 `"2 years 1 day 3 minutes ago"` 的相对日期

* 案例

```shell
git log --since=2.weeks 最近两周
```



### 作者过滤

*--author*

### 提交说明搜索

*--grep*



### pickAXE搜索

只会显示那些添加或删除了该字符串的提交。 

假设你想找出添加或删除了对某一个特定函数的引用的提交，可以调用：

```shell
git log -S function_name
```

### 常用限制输出选项

| 选项                | 说明                                       |
| ------------------- | ------------------------------------------ |
| `-<n>`              | 最近的n条提交                              |
| --since`, `--after  | 仅显示指定时间之后的提交。                 |
| --until`, `--before | 仅显示指定时间之前的提交。                 |
| --author            | 仅显示作者匹配指定字符串的提交。           |
| `--committer`       | 仅显示提交者匹配指定字符串的提交。         |
| `--grep`            | 仅显示提交说明中包含指定字符串的提交。     |
| `-S`                | 仅显示添加或删除内容匹配指定字符串的提交。 |



# 撤销操作

## 补漏

* 使用场景
  * 有时候我们提交完了才发现漏掉了几个文件没有添加
  * 或者提交信息写错了
* 可以运行带有 `--amend` 选项的提交命令来重新提交：

* 这个命令会将暂存区中的文件提交。 如果自上次提交以来你还未做任何修改
* 并替换之前的提交日志

```shell
git commit --amend -m ""
```

## 取消暂存

```shell
#这个命令后续在了解
git reset HEAD CONTRIBUTING.md
#正规用法
git restore --staged <file>...
```

## 撤消对文件的修改

```shell
git restore <file>...

#用版本库最近的版本去替换当前的文件
git checkout file
```



# 远程仓库的使用

## 查看已配置的远程仓库

```shell
git remote  #默认的名称为 origin
git remote -v #详细
```

## 添加远程仓库

```shell
#配置远程库
git remote add <shortname> <url>
git remote add pb https://github.com/paulboone/ticgit
#拉取代码
git fetch pb

现在 Paul 的 master 分支可以在本地通过 pb/master 访问到
```

## clone命令的行为

如果你使用 `clone` 命令克隆了一个仓库，命令会自动将其添加为远程仓库并默认以 “origin” 为简写

`git clone` 命令会自动设置本地 master 分支跟踪克隆的远程仓库的 `master` 分支



## gitpull

如果你的当前分支设置了跟踪远程分支

那么可以用 `git pull` 命令来自动抓取后合并该远程分支到当前分支

运行 `git pull` 通常会从最初克隆的服务器上抓取数据并自动尝试合并到当前所在的分支。



## 推送远程分支

```shell
git push <remote> <branch>
 git push origin master
```

限制

* 只有当你有所克隆服务器的写入权限,并且之前没有人推送过时，这条命令才能生效
* 当已经有推送时,你必须先抓取他们的工作并将其合并进你的工作后才能推送

## 查看某个远程仓库

```shell
git remote show origin
```

## 远程仓库的重命名与移除

```shell
 git remote rename pb paul
 git remote remove paul
```



# 打标签

## 查看标签

Git 可以给仓库历史中的某一个提交打上标签,使用这个功能来标记发布结点（ `v1.0` 、 `v2.0` 等等）

```shell
#这个命令以字母顺序列出标签，但是它们显示的顺序并不重要。
git tag
#可以模糊匹配
git tag -l "v1.8.5*"
```

## 创建标签

git有两种标签

## 轻量标签

轻量标签很像一个不会改变的分支——它只是某个特定提交的引用。

```shell
#不要使用-a, -m -s 等
git tag v1.4-lw
git show v1.4-lw
```

## 附注标签

​	而附注标签是存储在 Git 数据库中的一个完整对象， 它们是可以被校验的，其中包含打标签者的名字、电子邮件地址、日期时间， 此外还有一个标签信息，并且可以使用 GNU Privacy Guard （GPG）签名并验证。 通常会建议创建附注标签，这样你可以拥有以上所有信息。但是如果你只是想用一个临时的标签， 或者因为某些原因不想要保存这些信息，那么也可以用轻量标签。

```shell
git tag -a v1.4 -m "my version 1.4"

git show v1.4
```

## 提交历史打标签

```
git log --pretty=oneline
git tag -a v1.2 9fceb02 #部分校验和足够区分
```

## 共享标签

`git push` 命令并不会传送标签到远程仓库服务器上,在创建完标签后你必须显式地推送标签到共享服务器上。

```shell
 git push origin <tagname>
 #把所有不在远程仓库服务器上的标签全部传送到那里。
 git push origin --tags
```

## 删除标签

```
 git tag -d v1.4-lw
 
 #推送远程仓库
 git push origin :refs/tags/v1.4-lw
  git push origin --delete <tagname>
```



## 检出标签

```shell
git checkout 2.0.0

```

仓库处于“分离头指针（detached HEAD）”的状态,如果你做了某些更改然后提交它们，标签不会发生变化， 但你的新提交将不属于任何分支，并且将无法访问，除非通过确切的提交哈希才能访问。 因此，如果你需要进行更改，比如你要修复旧版本中的错误，那么通常需要创建一个新分支：



# Git别名

通过 `git config` 文件来轻松地为每一个命令设置一个别名

```shell
$ git config --global alias.co checkout
$ git config --global alias.br branch
$ git config --global alias.ci commit
$ git config --global alias.st status
$ git config --global alias.unstage 'reset HEAD --'

$ git unstage fileA
$ git reset HEAD -- fileA
git config --global alias.last 'log -1 HEAD'

#执行外部命令 加上!
git config --global alias.visual '!gitk'
```

