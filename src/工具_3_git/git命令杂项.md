## 拉取远程分支合并到本地分支

```shell
git pull origin feature/develop-4.4.0-public
git pull upstream feature/develop-4.4.0-public
```



## 删除未加入到版本管理的文件

```shell
git clean -fd
```



## 将版本库里的head替换工作区与暂存库

```shell
git checkout head . 
```





### 不分页

```
git config --global core.pager cat 
```

### 分页

```
git config --global core.pager less
```





## bare repo

```
# 只有版本库，没有工作区的仓库。专门用于中心化存储
git init --bare

# 克隆仓库，并作为裸仓库
git clone --mirror https://xxxx
```



### 使用SSH克隆

```
 git clone git+ssh://hap@192.168.1.2/~/working 
  sudo git clone username@12.345.67.891:/home/path/to/repo.git 

```





### 远程跟踪分支信息

```
git branch -vv
```





### GIT支持的协议

```
$ git clone http[s]://example.com/path/to/repo.git
$ git clone http://git.oschina.net/yiibai/sample.git
$ git clone ssh://example.com/path/to/repo.git
$ git clone git://example.com/path/to/repo.git
$ git clone /opt/git/project.git 
$ git clone file:///opt/git/project.git
$ git clone ftp[s]://example.com/path/to/repo.git
$ git clone rsync://example.com/path/to/repo.git
```







### GIT显示两个分支的提交差异

```
# 在 newBranch不在 oldBranch的提交
git log oldBranch..newBranch
git branch oldbranch..newbranch
```





### GIT DIFF

```
# diff工具
git difftool
# 比较文件名
git difftool location\filename

## diff 当前版本的文件和某个commit的某个文件
git difftool 3693493981a35c07f2bee7cae71f8e8bd95be625 -- filename


## 
git difftool [start commit]..[end commit] filename

##
git log filename  # 查看某个文件的提交记录

git difftool 6cde26245763dd43f9505c7578a1f7be44b7fad1..8d5336398  -- filename


git diff HEAD^ -- filePath


git diff：是查看 workspace 与 index 的差别的。
git diff --cached：是查看 index 与 local repositorty 的差别的。
git diff HEAD：是查看 workspace 和 local repository 的差别的。（HEAD 指向的是 local repository 中最新提交的版本）
```

注：git diff 后跟两个参数，如果只写一个参数，表示默认跟 workspace中的代码作比较。git diff 显示的结果为 第二个参数所指的代码在第一个参数所指代码基础上的修改。如，git diff HEAD 表示 workspace 在 最新commit的基础上所做的修改。







### git difftool

```shell
git difftool [<options>] [<commit> [<commit>]] [--] [<path>…​]
```

1. `git difftool`是一个 Git 命令，允许您使用常见差异工具在修订之间比较和编辑文件。`git difftool`是前端`git diff`并接受相同的选项和参数。参见 git-diff [1]。

   -d   --dir-diff   : 将修改后的文件复制到临时位置，然后对它们执行一个目录 diff。该模式在启动 diff 工具之前从不提示。

   -y   --no-prompt   :启动 diff 工具前不要提示。

   --prompt   :在每次调用 diff 工具前提示。这是默认行为; 该选项用于覆盖任何配置设置。

```
-t <tool>   --tool=<tool>   
   
使用<tool>指定的 diff 工具。有效值包括 emerge，kompare，meld 和 vimdiff。运行git difftool --tool-help有效的<工具>设置列表。
   
如果没有指定 diff 工具，git difftool将使用配置变量diff.tool。
如果配置变量diff.tool没有设置，git difftool会选择一个合适的默认值。

您可以通过设置配置变量明确提供工具的完整路径difftool.<tool>.path。例如，您可以通过设置配置 kdiff3 的绝对路径difftool.kdiff3.path。否则，git difftool假定该工具在 PATH 中可用。
```







### 不合并特定文件

```shell
echo 'index.php merge=ours' >> .gitattributes
git add .gitattributes
```





### git 只合并某个目录/文件

```
git checkout 分支名 目录/** 目录2/**

比如：git checkout pmc dist/**

(目录下可能还有多个目录所以用/** 不用/*，单独只合并某个文件的话，路径准确就行)
```

### git 设置 合并分支时 忽略某个文件

