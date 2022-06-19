export effectedBranch=java
git status 
git branch
git log -1

git log --pretty=format:“%s” -1

if git diff head^..head --   --name-only | grep src/java/src/SUMMARY.md;then
    echo effectedBranch yes
    export effectedBranch=java
fi