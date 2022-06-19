set -ex
which python
if [ $effectedBranch ];then 
    mdbook build src/$effectedBranch
fi