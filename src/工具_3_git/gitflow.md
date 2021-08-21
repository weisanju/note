# gitflow分支概念

| 分支名称                | 作用                                     | 生命周期           | 提交or合并                                                   | 起止点                                         |
| ----------------------- | ---------------------------------------- | ------------------ | ------------------------------------------------------------ | ---------------------------------------------- |
| *Production* (*master*) | 记录历史发布版本                         | 贯穿整个项目       | 不能提交，由release分支合并                                  | 整个项目                                       |
| *Develop*分支           | 记录历史开发功能                         | 贯穿整个项目       | 不能提交，由*feature*分支，*Bugfix*分支，*Release*分支合并代码 | 整个项目                                       |
| *Hotfix*分支            | 解决线上bug                              | 临时分支，紧急修复 | 可提交                                                       | 由生产分支产生，最终合并进生产分支，与开发分支 |
| *Reslease*分支          | 用于本次的Release如文档，*bug*修复，测试 | 临时分支，发版阶段 | 可提交                                                       | 由开发分支产生，合并到开发分支与生产分支       |
| *Feature*分支           | 用于某个功能的开发                       | 临时分支，开发阶段 | 可提交                                                       | 由Develop分支产生，合并到Develop分支中         |



# 从不同角度理解分支

## 生命周期

* *production* 分支和*develop*分支 贯穿项目
  * *production* 分支 记录发布版本，由*release*分支合并而来
  * *develop* 分支记录各个功能点的开发进度
* 其他分支，均为承担特定功能的 临时分支
  * *hotfix* *bug*修复分支，由生产分支产生，合并到生产分支与开发分支
  * *Reslease*：用于本次的发布，由开发分支产生，合并到开发分支与生产分支
  * *feature*：用于某个功能的开发 

## 项目阶段

* 开发阶段主要涉及  *feature*分支 ，*develop*分支 
* 发布阶段主要涉及release分支，*production* 分支，*develop* 分支
* 紧急修复阶段：主要涉及*hotfix*分支，*production*分支，*develop*分支 

## 成员关注点

* 开发人员 关注*develop* 分支，*feature*分支，hotfix分支
* 测试人员关注 *release*分支，*hotfix*分支
* 项目经理关注*production*分支，*release*分支



# 实践⼀个完整的Git-Flow流程

```
git init
git add .
git commit -m "project init"
git checkout -b develop master //切换分支时，从master分支创建 develop分支
git checkout -b feature-login develop
git add .
git commit -m "add loginUser.html"
git checkout develop
git merge --no-ff feature-login
git branch -d feature-login
```

