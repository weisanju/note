![img](../images/iptables_process.png)

![img](../images/tables_and_chain.png)

1. filter表——三个链：INPUT、FORWARD、OUTPUT
   作用：过滤数据包 内核模块：iptables_filter.
2. Nat表——三个链：PREROUTING、POSTROUTING、OUTPUT
   作用：用于网络地址转换（IP、端口） 内核模块：iptable_nat
3. Mangle表——五个链：PREROUTING、POSTROUTING、INPUT、OUTPUT、FORWARD
   作用：修改数据包的服务类型、TTL、并且可以配置路由实现QOS内核模块：iptable_mangle(别看这个表这么麻烦，咱们设置策略时几乎都不会用到它)
4. Raw表——两个链：OUTPUT、PREROUTING
   作用：决定数据包是否被状态跟踪机制处理 内核模块：iptable_raw
   (这个是REHL4没有的，不过不用怕，用的不多)

**规则链：**

1. INPUT——进来的数据包应用此规则链中的策略
2. OUTPUT——外出的数据包应用此规则链中的策略
3. FORWARD——转发数据包时应用此规则链中的策略
4. PREROUTING——对数据包作路由选择前应用此链中的规则
   （记住！所有的数据包进来的时侯都先由这个链处理）
5. POSTROUTING——对数据包作路由选择后应用此链中的规则
   （所有的数据包出来的时侯都先由这个链处理）









## 常用命令：

```
-A 追加规则-->iptables -A INPUT
-D 删除规则-->iptables -D INPUT 1(编号)
-R 修改规则-->iptables -R INPUT 1 -s 192.168.12.0 -j DROP 取代现行规则，顺序不变(1是位置)
-I 插入规则-->iptables -I INPUT 1 --dport 80 -j ACCEPT 插入一条规则，原本位置上的规则将会往后移动一个顺位
-L 查看规则-->iptables -L INPUT 列出规则链中的所有规则
-N 新的规则-->iptables -N allowed 定义新的规则
```

通用参数：

```
-p 协议 例：iptables -A INPUT -p tcp
-s源地址 例：iptables -A INPUT -s 192.168.1.1
-d目的地址 例：iptables -A INPUT -d 192.168.12.1
-sport源端口 例:iptables -A INPUT -p tcp --sport 22
-dport目的端口 例:iptables -A INPUT -p tcp --dport 22
-i指定入口网卡 例:iptables -A INPUT -i eth0
-o指定出口网卡 例:iptables -A FORWARD -o eth0

```

**-j 指定要进行的处理动作**
常用的ACTION：

```
DROP：丢弃
REJECT：明示拒绝
ACCEPT：接受
SNAT基于原地址的转换
```

**source--指定原地址**
  比如我们现在要将所有192.168.10.0网段的IP在经过的时候全都转换成172.16.100.1这个假设出来的外网地址：

```
iptables -t nat -A POSTROUTING -s 192.168.10.0/24 -j SNAT --to-source 172.16.100.1(外网有效ip)
```

这样，只要是来自本地网络的试图通过网卡访问网络的，都会被统统转换成172.16.100.1这个IP.
**MASQUERADE(动态伪装）**--

家用带宽获取的外网ip，就是用到了动态伪装

```
iptables -t nat -A POSTROUTING -s 192.168.10.0/24 -j MASQUERADE
```

**DNAT目标地址转换**
**destination-指定目标地址**

```
iptables -t nat -A PREROUTING -d 192.168.10.18 -p tcp --dport 80 -j DNAT --to-destination 172.16.100.2
10.18访问80端口转换到100.2上
```

MASQUERADE：源地址伪装
REDIRECT：重定向：主要用于实现端口重定向
MARK：打防火墙标记的
RETURN：返回 在自定义链执行完毕后使用返回，来返回原规则链。



匹配(match)
每个iptables规则都包含一组匹配以及一个目标，iptables匹配指的是数据包必须匹配的条件，只有当
数据包满足所有的匹配条件时，iptables才能根据由该规则的目标所指定的动作来处理该数据包
匹配都在iptable的命令行中指定

```
source--匹配源ip地址或网络
destination (-d)--匹配目标ip地址或网络
protocol (-p)--匹配ip值
in-interface (-i)--流入接口(例如，eth0)
out-interface (-o)--流出接口
state--匹配一组连接状态
string--匹配应用层数据字节序列
comment--在内核内存中为一个规则关联多达256个字节的注释数据
```

**目标(target)**
iptables支持一组目标，用于数据包匹配一条规则时触发一个动作

```
ACCEPT--允许数据包通过
DROP--丢弃数据包，不对该数据包做进一步的处理，对接收栈而言，就好像该数据包从来没有被接收一样
LOG--将数据包信息记录到syslog
REJECT--丢弃数据包，同时发送适当的响应报文(针对TCP连接的TCP重要数据包或针对UDP数据包的ICMP端口不可达消息)
RETURN--在调用链中继续处理数据包
```

