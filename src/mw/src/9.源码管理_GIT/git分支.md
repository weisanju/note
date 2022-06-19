# 分支简介

* 在git中,每一次提交都相当于从当前版本重做一次快照,而分支类似于指针, 指向不同的快照
* 当前工作目录所处的分支 则是用 head指针 标识
* 新建分支类似于新建一个指针 指向某一个快照
* 切换分支 将头指针 移向某一分支指针

# 操作命令

```shell
#新建分支
git branch testing
#分支切换,移动头指针在该分支指针上
git checkout testing
#新建分支自动切换
git checkout -b <newbranchname>
#删除分支
git branch -d hotfix
```



# 分支合并

## 命令

```shell
#检出
git checkout master
#合并
git merge iss53
```

* 快速合并

在合并的时候，你应该注意到了“快进（fast-forward）”这个词

如果master分支 跟 iss53 位于同一链条则 不用进行合并

直接 将 master分支 移动到 iss53分支

* 三方合并

  Git 会使用两个分支的末端所指的快照（`C4` 和 `C5`）以

  这两个分支的公共祖先（`C2`），做一个简单的三方合并,

  如果没有冲突,则会产生一个合并提交



## 冲突时的分支合并

* 对同一个文件的同一个部分进行了不同的修改,就会产生冲突此时 Git 做了合并，但是没有自动地创建一个新的合并提交

* `git status` 命令来查看那些因包含合并冲突而处于未合并（unmerged）状态的文件

* 冲突内容类似 下面

  ```
  <<<<<<< HEAD:index.html
  <div id="footer">contact : email.support@github.com</div>
  =======
  <div id="footer">
   please contact us at support@github.com
  </div>
  >>>>>>> iss53:index.html
  ```

  要选择 上面下面的一个然后提交

# 分支管理

```shell
#查看当前分支的最后一次提交
git branch -v
#查看哪些分支已合并到当前分支
git branch --merged
#查看合并的分支 未完成的合并工作
git branch --no-merged

```

# 分支开发工作流

gitflow

# 远程分支

## 远程跟踪分支

* 远程跟踪分支是远程分支状态的引用。

* 它们是你无法移动的本地引用。

* 一旦你进行了网络通信， Git 就会为你移动它们以精确反映远程仓库的状态。
* 请将它们看做书签， 这样可以提醒你该分支在远程仓库中的位置就是你最后一次连接到它们的位置。

* 它们以 `<remote>/<branch>` 的形式命名

## git clone 的操作

### 远程分支的开始

1. Git 的 `clone` 命令会为你自动将其命名为 `origin`
2. 拉取它的所有数据， 创建一个指向它的 `master` 分支的指针,并且在本地将其命名为 `origin/master`,这个即为远程跟踪分支
3. 也会创建一个本地分支 指向 origin/master

### 远程分支的分叉

* 你在本地的 `master` 分支做了一些工作，在同一段时间内有其他人推送提交到 `git.ourcompany.com` 并且更新了它的 `master` 分支

* 只要你保持不与 `origin` 服务器连接（并拉取数据），你的 `origin/master` 指针就不会移动。

```shell
#更新远程跟踪分支   
git fetch  <remote>
#将本地分支与远程分支合并
git merge origin/serverfix
#推送 本地分支 到远程分支
git push origin serverfix
#推送到远程 origin, 将本地serverfix推送到 远程servefix
git push origin serverfix:serverfix

```

## 分支跟踪

clone通常会自动地创建一个跟踪 `origin/master` 的 `master` 分支

### 手动跟踪某个分支

git checkout --track origin/serverfix

### checkout捷径

如果你尝试检出的分支 (a) 不存在且 (b) 刚好只有一个名字与之匹配的远程分支，那么 Git 就会为你创建一个跟踪分支：

```shell
git checkout serverfix
```

### 自定义分支名称

```shell
git checkout -b sf origin/serverfix
```

### 查看所有设置的跟踪分支

```shell
 git branch -vv
```

### 示例

```shell
$ git branch -vv
  iss53     7e424c3 [origin/iss53: ahead 2] forgot the brackets
  master    1ae2a45 [origin/master] deploying index fix
* serverfix f8674d9 [teamone/server-fix-good: ahead 3, behind 1] this should do it
  testing   5ea463a trying something new
```

`iss53` 

正在跟踪 `origin/iss53` 并且 “ahead” 是 2，意味着本地有两个提交还没有推送到服务器上。 

`master` 

正在跟踪 `origin/master` 分支并且是最新的。 

serverfix

 正在跟踪 `teamone` 服务器上的 `server-fix-good` 分支并且领先 3 落后 1， 意味着服务器上有一次提交还没有合并入同时本地有三次提交还没有推送。 

`testing` 

分支并没有跟踪任何远程分支。

​	需要重点注意的一点是这些数字的值来自于你从每个服务器上最后一次抓取的数据。 这个命令并没有连接服务器，它只会告诉你关于本地缓存的服务器数据。 如果想要统计最新的领先与落后数字，需要在运行此命令前抓取所有的远程仓库。 可以像这样做：

`git pull` 的魔法经常令人困惑所以通常单独显式地使用 `fetch` 与 `merge` 命令会更好一些

```shell
$ git fetch --all; git branch -vv
```

## 删除远程分支

```shell
 git push origin --delete serverfix
```

# 变基

## merge与rebase原理

个人理解:将该分支的基底基于 某个你想合并的分支

​	在 Git 中整合来自不同分支的修改主要有两种方法：`merge` 以及 `rebase`。

​	它的原理是首先找到这两个分支（即当前分支 `experiment`、变基操作的目标基底分支 `master`） 的最近共同祖先 `C2`，然后对比当前分支相对于该祖先的历次提交，提取相应的修改并存为临时文件， 然后将当前分支指向目标基底 `C3`, 最后以此将之前另存为临时文件的修改依序应用。 （译注：写明了 commit id，以便理解，下同）

​	Merge是基于三方快照 ,合并提交生成最新的一个快照

​	变基:可以使用 `rebase` 命令将提交到某一分支上的所有修改都移至另一分支上，就好像“重新播放”一样。





rebase的过程

```shell
git checkout experiment
git rebase master
git checkout master
git merge experiment
```



[变基文档](https://git-scm.com/book/zh/v2/Git-%E5%88%86%E6%94%AF-%E5%8F%98%E5%9F%BA)