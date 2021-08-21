# 确认/启用服务器远程开机

```
Settings for eth0:

        --- 略 ---

       Supports Wake-on: pumbag

        Wake-on: d

        --- 略 ---
```

其他信息不用关注，重要的是上面列出的两项:

**Supports Wake-on: pumbag**    

- p Wake on phy activity
- u Wake on unicast messages
- m Wake on multicast messages
- b Wake on broadcast messages
- a Wake on ARP
- g Wake on MagicPacket(tm)         

wake-on 项值默认为 d，表示禁用wake on lan。需要把wake-on的值设为g以启用 wake on lan



# 设置WOL选项

**临时设置**

```
ethtool -s eth0 wol g
```

**永久设置**

```
vi /etc/sysconfig/network-scripts/ifcfg-eth0
ETHTOOL_OPTS="wol g"
```

