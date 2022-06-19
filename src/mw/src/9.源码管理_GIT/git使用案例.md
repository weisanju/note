# GIT设置

# 设置客户端中文不显示数字

```
git config --global core.quotepath false
```



# 储藏本地修改

```
1. *git fetch && git merge* 
2. *git stash*储藏本地修改
3. *git stash pop*恢复储藏
```





# COMMIT操作

## 执行回退

```sh
#回退上一版本
git reset --hard HEAD^
git push -f origin master

#回退上上个版本
git reset --hard HEAD^1
```

## 保持跟远程一致

```sh
git fetch --all 

git reset --hard origin/dev

git pull
```



## 项目过大时拉取不了GIT

```
当项目过大时，git clone时会出现error: RPC failed; HTTP 504 curl 22 The requested URL returned error: 504 Gateway Time-out的问题，此时我们可以只下载远程仓库中的最新的一个版本，而不下载其他老版本的内容，这样会大大减小存储与传输压力。


```

```
我们可以在克隆时指定--depth 1，--depth后面的阿拉伯数字代表克隆仓库的最新几个版本，为1代表只克隆远程仓库的最新的一个版本。

```

**示例：**

```sh
git clone --depth 1 https://github.com/dogescript/xxxxxxx.git
```





