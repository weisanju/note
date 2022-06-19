export effectedBranch=java
if git diff head~1..head  --name-only | grep src/java/src/SUMMARY.md;then
    echo effectedBranch yes
    export effectedBranch=java
fi