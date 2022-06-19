# java

```shell
function split_1()
{
    x="a,b,c,d"
 
    OLD_IFS="$IFS"
    IFS=","
    array=($x)
    IFS="$OLD_IFS"
 
    for each in ${array[*]}
    do
        echo $each
    done
}
```