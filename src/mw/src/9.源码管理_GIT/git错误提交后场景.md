# **修改最后一次提交**

修补提交

```
git commit --amend
```

**它可以把我们这一次的修改合并到上一条历史记录当中**



# **修改多个信息**

--amend虽然好用，但是它只能修改最后一次的提交信息，如果我们想要修改的提交记录在那之前，我们应该怎么办呢？



git当中并没有提供直接的工具来实现这一点，不过我们可以使用**rebase**来达成。我们可以加上-i进行交互式地变基，我们可以在任何想要的修改完成之后停止，也可以添加文件或者是做其他想要做的事情。但是我们变基的目标不是某一个分支而是当前分支的某一个历史节点，所以我们**需要提供一个具体的commitid或者是指针位置**。



git rebase -i的功能非常强大，我们几乎可以使用它来完成所有一切我们想要完成的事情。



比如我们想要修改倒数第二次提交，我们可以执行git rebase -i HEAD~3。也就是以倒数第三个节点作为基准节点执行变基，这时候git会进入一个vim窗口，在这个窗口当中我们可以看到最近的三次提交记录。



首先我们可以看到上面的三行就是我们可以修改的三个commit，分别展示的是要执行的操作以及commitid以及commit message。这里的操作默认的是pick，也就是使用该commit。关于我们可以执行的操作git在下方也给了充分的提示，其中比较常用的有**pick、edit以及squash**。



这一次我们想要做的是修改提交记录，所以我们应该执行edit，我们把想要修改的commit前的pick改成edit。比如这样：

退出之后，git会自动带我们**回到我们选择edit的分支提交之后的版本**。我们进行我们想要的修改，这里我在第15篇文章当中加上了一行：尝试rebase。之后再使用git add以及git commit --amend进行修改提交结果。

再之后我们执行git rebase --continue，把剩下要应用的变更应用完成



一切都结束之后，我们可以使用一下git show命令查看一下我们修改的bee9ce3这个commit的记录。可以看到已经多了这一行，说明我们的修改成功了。





# 撤回提交

## revert撤回

>  revert 可以取消指定的某次提交内容，原理是生成相应的提交 抵消

## 概述

当讨论 revert 时，需要分两种情况，因为 commit 分为两种：一种是常规的 commit，也就是使用 `git commit` 提交的 commit；另一种是 merge commit，在使用 `git merge` 合并两个分支之后，你将会得到一个新的 merge commit。

merge commit 和普通 commit 的不同之处在于 merge commit 包含两个 parent commit，代表该 merge commit 是从哪两个 commit 合并过来的。

```sh
git show bd86846
```



**revert 常规 commit**

```
`git revert <commit id>` # 即可，git 会生成一个新的 commit，将指定的 commit 内容从当前分支上撤除。
```

**revert merge commit**



revert merge commit 有一些不同，这时需要添加 `-m` 选项以代表这次 revert 的是一个 merge commit

但如果直接使用 git revert ，git 也不知道到底要撤除哪一条分支上的内容，这时需要指定一个 parent number 标识出"主线"，主线的内容将会保留，而另一条分支的内容将被 revert。

如上面的例子中，从 `git show` 命令的结果中可以看到，merge commit 的 parent 分别为 ba25a9d 和 1c7036f，其中 ba25a9d 代表 master 分支（从图中可以看出），1c7036f 代表 will-be-revert 分支。需要注意的是 -m 选项接收的参数是一个数字，数字取值为 1 和 2，也就是 Merge 行里面列出来的第一个还是第二个。



## reset

记住 合并前的最后一个  commitId，









# **顺序变更、合并、拆分**

## 顺序变更

我们不仅可以修改某一次commit当中的内容，还可以修改这些commit的相对顺序，以及可以让它们合并以及拆分。



修改顺序其实很简单，我们只需要人为地修改rebase -i之后弹出的vim文件即可。比如说原本的记录是：

```
pick A change A
pick B change B
pick C change C
```

如果我们想要更换顺序，我们只需要修改这个文件即可。比如变成：

```
pick B change B
pick A change A
pick C change C
```

那么当我们在退出vim的时候，git会首先应用B commit的变更，再应用A最后应用C。



## **合并**

除此之外，我们还可以合并多个commit记录成一个。操作的方法也很简单，就是我们只需要把pick修改成squash。git会自动把所有squash的commit记录合并在一起。

```
pick A change A
squash B change B
squash C change C
```

## **拆分**

有的时候一个commit非常巨大，我们可能也会想要将它拆分，其实操作也很简单。比如我们想要把commit B拆分成两条，首先，我们在rebase的时候将commit B前面的pick修改成edit。

```
pick A change A
edit B change B
pick C change C
```



当我们退出的时候，我们会进入到B commit刚刚提交完的状态。由于我们要做的是拆分B这个提交，所以我们需要执行git reset HEAD^，把上一次提交重置。然后再分别add我们想要拆分开来提交的文件。

整个操作如下：

```
git reset HEAD^
git add test/*
git ci -m 'add test'
git add code/*
git ci -m 'update code'
git rebase --continue
```

这样我们就把commit B拆分成了两个commit插入到了历史记录当中了。





## 注意

最后的最后，大家需要注意，虽然这些手段在修改记录的时候非常好用。但是如果这些commit已经被提交到了远程，我们是不可以直接git push同步的。因为git会校验我们提交的hash值，发现对不上之后会禁止我们的提交。所以如果想要提交到远程的话，只能使用git push -f强制覆盖。但是**这是一个非常非常危险的操作**，如果你git push -f了，没有人会知道你到底修改了什么，只建议在自己独有的分支上如此操作，一定一定要谨慎使用。

