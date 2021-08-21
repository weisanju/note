# git-merge简介

git-merge命令是用于从指定的commit(s)合并到当前分支的操作。

这里的指定commit(s)是指从这些历史commit节点开始，一直到当前分开的时候。





# 合并方式

Git merge的时候，有几种合并方式可以选择

## --ff

如果能从一个分支的*commit*  直接 移动 到 被合并分支。则直接 更新 分支的 指针，而不会创建一个合并的提交

这是默认行为，*fast-forwar模式*

## --no-ff

即使能 fast-forward也 要 创建一个*commit*

## --squash

将待合并的 分支与当前分支的 最近共同 的祖先结点 到  待合并的分支 的 head节点的所有 提交压缩成一个

## --no-squash

