## GitHub首页

https://github.com/canonical/multipass/

## Find available images

```
$ multipass find
Image                       Aliases           Version          Description
core                        core16            20200213         Ubuntu Core 16
core18                                        20200210         Ubuntu Core 18
16.04                       xenial            20200721         Ubuntu 16.04 LTS
18.04                       bionic,lts        20200717         Ubuntu 18.04 LTS
20.04                       focal             20200720         Ubuntu 20.04 LTS
daily:20.10                 devel,groovy      20200721         Ubuntu 20.10
```





## Launch a fresh instance of the current Ubuntu LTS

```
$ multipass launch ubuntu
Launching dancing-chipmunk...
Downloading Ubuntu 18.04 LTS..........
Launched: dancing chipmunk
```



### 指定CPU内存磁盘

```shell
$ multipass launch --name myVM --mem 2G --disk 10G --cpus 2 impish
```



# How to share data with an instance

```nohighlight
$ multipass mount $HOME keen-yak
$ multipass info keen-yak
…
Mounts:         /home/michal => /home/ubuntu
```

从这一点开始，/home/ubuntu将在实例内部可用。使用umount再次卸载它，您可以通过在实例名称后传递指定的目标来更改目标:

```nohighlight
$ multipass umount keen-yak
$ multipass mount $HOME keen-yak:/some/path
$ multipass info keen-yak                
…
Mounts:         /home/michal => /some/path
```



## 传输文件

```nohighlight
$ multipass transfer keen-yak:/etc/crontab keen-yak:/etc/fstab /home/michal
$ ls -l /home/michal/crontab /home/michal/fstab
-rw-r--r-- 1 ubuntu ubuntu 722 Oct 18 12:13 /home/michal/crontab
-rw-r--r-- 1 ubuntu ubuntu  82 Oct 18 12:13 /home/michal/fstab
$ multipass transfer /home/michal/crontab /home/michal/fstab keen-yak:
$ multipass exec keen-yak -- ls -l crontab fstab
-rw-rw-r-- 1 multipass multipass 722 Oct 18 12:14 crontab
-rw-rw-r-- 1 multipass multipass  82 Oct 18 12:14 fstab
```



## Check out the running instances

```
$ multipass list
Name                    State             IPv4             Release
dancing-chipmunk        RUNNING           10.125.174.247   Ubuntu 18.04 LTS
live-naiad              RUNNING           10.125.174.243   Ubuntu 18.04 LTS
snapcraft-asciinema     STOPPED           --               Ubuntu Snapcraft builder for Core 18
```





## Learn more about the VM instance you just launched

```
$ multipass info dancing-chipmunk
Name:           dancing-chipmunk
State:          RUNNING
IPv4:           10.125.174.247
Release:        Ubuntu 18.04.1 LTS
Image hash:     19e9853d8267 (Ubuntu 18.04 LTS)
Load:           0.97 0.30 0.10
Disk usage:     1.1G out of 4.7G
Memory usage:   85.1M out of 985.4M
```



## Connect to a running instance

```
 multipass shell dancing-chipmunk
```





## Run commands inside an instance from outside

```
$ multipass exec dancing-chipmunk -- lsb_release -a
No LSB modules are available.
Distributor ID:  Ubuntu
Description:     Ubuntu 18.04.1 LTS
Release:         18.04
Codename:        bionic
```



## Stop an instance to save resources

```
$ multipass stop dancing-chipmunk
```



## Delete the instance

```
$ multipass delete dancing-chipmunk
```

And when you want to completely get rid of it:

```
$ multipass purge
```

