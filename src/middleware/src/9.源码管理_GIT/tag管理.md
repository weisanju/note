# 增删改

## 删除本地tag

```
git tag -d tag_1.6.21
```

## 推送删除tag

```
git push origin :refs/tags/tag_1.6.21
git push <remote> :refs/tags/<tagname>
```

## 新建本地tag

```
git tag tag_1.6.23.1

//带注释的标签
git tag -a <tagname> -m "runoob.com标签"
```

## 推送tag	
```
git push origin tag_1.6.22.2
```

## 一次性推送所有

```
git push origin --tags
```



# 查询

## 模糊查询

```console
 git tag -l "v1.8.5*"
```

# 从Tag签出

如果你想查看一个标签指向的文件的版本，你可以对该标签执行 git checkout，尽管这会使你的存储库处于“detached HEAD”状态，这会产生一些不良的副作用：

```
git checkout tag_name
git switch -c <new-branch-name>
```



```console
git checkout -b <branch-name> <tag_name>
git checkout -b version2 v2.0.0
```

