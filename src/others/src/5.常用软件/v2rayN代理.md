[GITHUB](https://github.com/2dust/v2rayN/releases)

## Geo文件

```
Geo文件即路由规则文件：

"geosite.dat"：提供一个预定义好的 「全球域名」 列表;  

"geoip.dat" ：提供一个预定义好的 「全球 ip-地区」 列表.
```

```
.dat文件里面有无数个分类，比如,中国的域名和IP都在 geosite:cn 和 geoip:cn*

gfwlist的网址(也就是经典的PAC)在 geosite:gfw | [点击跳转](https://youtu.be/jjpBvUYotDc)

广告域名在 geosite:category-ads-all
国外域名在 geosite:geolocation-!cn
本地IP在 geoip:private里;
还有一千多种分类细分，比如 geosite:steam geosite:google 等，
```



**路由规则输入的格式**

```
domian:jamesdailylife.com
```

**相关域名和IP分类名的解释**

```
category-ads：包含了常见的广告域名。
category-ads-all：包含了常见的广告域名，以及广告提供商的域名。
cn：相当于 geolocation-cn 和 tld-cn 的合集。
apple：包含了 Apple 旗下绝大部分域名。
google：包含了 Google 旗下绝大部分域名。
microsoft：包含了 Microsoft 旗下绝大部分域名。
facebook：包含了 Facebook 旗下绝大部分域名。
twitter：包含了 Twitter 旗下绝大部分域名。
telegram：包含了 Telegram 旗下绝大部分域名。
geolocation-cn：包含了常见的大陆站点域名。
geolocation-!cn：包含了常见的非大陆站点域名，同时包含了 tld-!cn。
tld-cn：包含了 CNNIC 管理的用于中国大陆的顶级域名，如以 .cn、.中国 结尾的域名。
tld-!cn：包含了非中国大陆使用的顶级域名，如以 .hk（香港）、.tw（台湾）、.jp（日本）、.sg（新加坡）、.us（美国）.ca（加拿大）等结尾的域名。

category-games： 包含了 steam、ea、blizzard、epicgames 和 nintendo 等常见的游戏厂商。
更多域名类别，请查看 data 目录 。
```



**OutBoundTag**

* *proxy*: 代理
* *direct*：直连
* *block*: 阻止



#### 越靠前的规则，优先级越高

**路由规则集范本**

**白名单范例**：https://raw.githubusercontent.com/2dust/v2rayCustomRoutingList/master/custom_routing_rules_whitelist

**黑名单范例**：https://raw.githubusercontent.com/2dust/v2rayCustomRoutingList/master/custom_routing_rules_blacklist





[参考链接-1](https://xtrojan.cc/client/new-v2rayn-c-4-12.html)

[官方文档](https://www.v2fly.org/config/overview.html)



