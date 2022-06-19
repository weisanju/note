set -ex
if [[ $effectedBranch != "" ]];then 
    mdbook build src/$effectedBranch
fi
