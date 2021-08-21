# 何为位图（bitmap）

位图不是特殊的数据结构，它本身是一个普通字符串，即 byte 数组。一个 byte 使用 8 个二进制位存储。redis 提供一系列操作二进制位的指令操作，这些操作称为「位图操作」。

* 使用 getbit、setbit 可以直接对 value 的位进行操作，在处理数亿级别的**业务标示位存储**时能够节约很大的空间。

* redis 中一个 String 类型的 value 能存储最大的值是 512MB、那么能存储大约 **40亿** 个bit。

# 基本使用

零存零取 setbit / getbit

我们可以使用 bitset 指令以「位存储」的方式存储一个 `he` 字符串，注意上面获取的是无符号表示，存储的时候需要添加第一位为符号位。

```
127.0.0.1:6379> setbit he 0 0 # 添加符号位
(integer) 0
127.0.0.1:6379> setbit he 1 1
(integer) 0
127.0.0.1:6379> setbit he 2 1
(integer) 0
127.0.0.1:6379> setbit he 3 0
(integer) 0
127.0.0.1:6379> setbit he 4 1
(integer) 0
127.0.0.1:6379> setbit he 5 0
(integer) 0
127.0.0.1:6379> setbit he 6 0
(integer) 0
127.0.0.1:6379> setbit he 7 0
(integer) 0
127.0.0.1:6379> get he # “整取”
"h" # 设置了第一个字符
127.0.0.1:6379> setbit he 8 0 # 添加符号位
(integer) 0
127.0.0.1:6379> setbit he 9 1
(integer) 0
127.0.0.1:6379> setbit he 10 1
(integer) 0
127.0.0.1:6379> setbit he 11 0
(integer) 0
127.0.0.1:6379> setbit he 12 0
(integer) 0
127.0.0.1:6379> setbit he 13 1
(integer) 0
127.0.0.1:6379> setbit he 14 0
(integer) 0
127.0.0.1:6379> setbit he 15 1
(integer) 0
127.0.0.1:6379> get he # “整取”
"he" # 设置了两个字符
127.0.0.1:6379> getbit he 14 # “零取”
(integer) 0
```



## 统计和查找 bitcount / bitpos

- bitcount key [start end]：统计指定范围内 1 出现的次数；
- bitpos key bit [start] [end]：统计指定范围内 0 或 1 第一次出现的位置。

需要注意的是，这里的索引值是以 byte 为单位的，不是以 bit 为单位。即是以字节为范围查找的，如果需要以位范围查找必须是 8 的倍数。

![](/images/redis_bit_map_example.png)

## 批量操作 bitfield

bitfield 有三个子指令 set、get、incrby。他们可以对执行片段进行读写自增，最多处理 64 个连续位。

# 使用场景

bitmap 非常合适存储标识字段，因为位存储的就是 0 和 1。假设存储一个 1 的标识字段，用 String 类型存储需要 1 个字节，八个位。如果直接用位存储则能存储 8 个标识字段。在大数据量的标示位存储下，bit 能节省很大空间。

## 【场景1】统计用户某个时间段的登陆次数

```
key：时间段，如 1月
offset：用户ID，必须是整数
value：是否上线标识位
```

前面以时间为主体，或者还可以以用户为主体，具体设计方式取决于取数据的方式。

```
key：用户ID
offset：日期，如 200214
value：是否上线标识位
```

【场景2】存储用户一年的签到次数

```
key：用户ID
offset：天数，1 代表今年的第一天，一年就只占用了 365 个位
value：是否签到标识
```



