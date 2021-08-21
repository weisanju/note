# LinuxFor语句总结

## **第一类：数字性循环**

## **显示递增**

```sh
#!/bin/bash  
  
for((i=1;i<=10;i++));  
do   
echo $(expr $i \* 3 + 1);  
done  
```

## SEQ命令

```sh
#!/bin/bash  
  
for i in $(seq 1 10)  
do   
echo $(expr $i \* 3 + 1);  
done   
```

**选项**

```
Usage: seq [OPTION]... LAST
  or:  seq [OPTION]... FIRST LAST
  or:  seq [OPTION]... FIRST INCREMENT LAST
```



## 首尾范围

```sh
#!/bin/bash  
  
for i in {1..10}  
do  
echo $(expr $i \* 3 + 1);  
done  
```

## AWK递增

```
awk 'BEGIN{for(i=1; i<=10; i++) print i}'  
```

## 字符性循环

**ls命令**

```
#!/bin/bash  
  
for i in `ls`;  
do   
echo $i is file name\! ;  
done   
```

**脚本参数**

```sh
#!/bin/bash  
  
for i in $* ;  
do  
echo $i is input chart\! ;  
done 
```

**字面量**

```sh
#!/bin/bash  
  
list="rootfs usr data data2"  
for i in $list;  
do  
echo $i is appoint ;  
done  
```

## **路径查找**

```sh
#!/bin/bash  
  
for file in /proc/*;  
do  
echo $file is file path \! ;  
done  
```

```sh
#!/bin/bash  
  
for file in $(ls *.sh)  
do  
echo $file is file path \! ;  
done  
```

