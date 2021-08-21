# Stat命令

**用法**： `stat [OPTION]... FILE...`







# 选项

 **-L**, --dereference     跟随引用
 **-f**, --file-system     展示文件系统状态而不是文件状态
 **-c**  --format=FORMAT   使用格式化，对于每一个 FORMAT的后面会应用一个换行

   --printf=FORMAT   like --format, 会解析转义符。不会强制应用换行符

  **-t**, --terse       以简洁的形式打印信息
      --help     显示此帮助并退出
      --version  输出版本信息并退出



# 文件状态格式符

The valid format sequences for files (without --file-system):

| 格式符 | 说明                                         | 英文                                                         |
| ------ | -------------------------------------------- | ------------------------------------------------------------ |
| %a     | 八进制的访问权限                             | access rights in octal                                       |
| %A     | 人类可读的访问权限                           | access rights in human readable form                         |
| %b     | 分配的 块                                    | number of blocks allocated (see %B)                          |
| %B     | 每个块有多少个字节                           | the size in bytes of each block reported by %b               |
| %C     | SELinux security 上下文                      | SELinux security context string                              |
| %d/%D  | 十进制/十六进制的设备号                      | device number in decimal/device number in hex                |
| %f     | raw mode in hex                              | raw mode in hex                                              |
| %F     | 文件类型                                     | file type                                                    |
| %g/%G  | 拥有者的 群组编号/名称                       | group ID of owner/group name of owner                        |
| %h     | 硬链接的个数                                 | number of hard links                                         |
| %i     | i节点的个数                                  | inode number                                                 |
| %m     | 挂载点                                       | mount point                                                  |
| %n     | 文件名                                       | file name                                                    |
| %N     | 文件名 跟随了符号链接                        | quoted file name with dereference if symbolic link           |
| %o     |                                              | optimal I/O transfer size hint                               |
| %s     | 文件大小                                     | total size, in bytes                                         |
| %t     | 主设备被号                                   | major device type in hex, for character/block device special files |
| %T     | 次设备号                                     | minor device type in hex, for character/block device special files |
| %u/%U  | 用户id/用户名                                | user ID of owner/user name of owner                          |
| %w     | 创建时间，可读的，如果未知则是-              | time of file birth, human-readable; - if unknown             |
| %W     | 创建时间，从 Epoch 开始的秒数，如果未知则为0 | time of file birth, seconds since Epoch; 0 if unknown        |
| %x     | 访问时间，可读，如果未知则是-                | time of last access, human-readable                          |
| %X     | 访问时间，从 Epoch 开始的秒数                | time of last access, seconds since Epoch                     |
| %y     | 修改时间，可读                               | time of last modification, human-readable                    |
| %Y     | 修改时间，从 Epoch 开始的秒数                | time of last modification, seconds since Epoch               |
| %z     | 文件状态修改时间，人类可读的                 | time of last change, human-readable                          |
| %Z     | 文件状态修改时间，从 Epoch 开始的秒数        | time of last change, seconds since Epoch                     |

​    

# 文件系统状态格式符

| 格式符 | 说明                          | 英文                                      |
| ------ | ----------------------------- | ----------------------------------------- |
| %a     | 非超级用户来说。可用的块      | free blocks available to non-superuser    |
| %b     | 文件系统总共的块              | total data blocks in file system          |
| %c     | 总共i结点的 个数              | total file nodes in file system           |
| %d     | 剩余i节点的个数               | free file nodes in file system            |
| %f     | 剩余的块个数                  | free blocks in file system                |
| %i     | 文件系统id 十六进制           | file system ID in hex                     |
| %l     | 最长的文件名                  | maximum length of filenames               |
| %n     | 文件名                        | file name                                 |
| %s     | 推荐的 用于更快的传输的块大小 | block size (for faster transfers)         |
| %S     | 基础的块大小                  | fundamental block size (for block counts) |
| %t     | 十六进制的 文件系统类型       | file system type in hex                   |
| %T     | 文件系统类型，可读的          | file system type in human readable form   |


