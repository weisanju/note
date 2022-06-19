if git diff head~1..head  --name-only | grep src/java/SUMMARY.md;then
    export effectedBranch=java
    cd $effectedBranch && mdbook build $effectedBranch
    cd ../
fi