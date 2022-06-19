set -ex
export effectedBranch=java
cd src/$effectedBranch && mdbook build
cd ../../

# if git diff head~1..head  --name-only | grep src/java/SUMMARY.md;then
#     echo success 
# fi