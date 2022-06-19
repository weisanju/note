export effectedBranch=java
git status 
git branch
git log -1

git log --pretty=format:“%s” -1

git diff head^..head --  --name-only | grep SUMMARY.md>