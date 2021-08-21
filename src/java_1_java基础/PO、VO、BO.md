# 见图

![](/images/po_vo_bo.jfif)

**PO（Persistant Object）-持久对象**

一个PO对应数据库表中的一条记录，等同于entity，一般PO仅用于表示数据，除了拥有get、set方法，没有操作数据的方法。

**BO（business object）-业务对象**

BO可以看成是PO的组合，例如：一个订单会有商品PO、购买人PO等，可以建立一个BO处理订单信息。这样处理业务逻辑时就可以针对BO来处理，对外就不会暴露数据表结构了。可以根据实际需要把业务处理方法放在BO里面。

**VO（value object /view object）-值对象/表现层对象**

主要对应前台页面显示的数据对象，例如常见的json。

**DTO（Data Transfer Object）-数据传输对象**

跨层级或者跨进程传输时用到的对象，例如微服务中服务于服务之间的调用传输的对象就是DTO。DTO和VO的比较相似但是还是有些差异，这些主要体现在设计上或对业务的解释上，例如：同一个gender属性，DTO中的值可能是“1”，VO为了更好理解则会转化为“男”。

**DAO（data access object）-数据访问对象**

用来访问数据库，封装对数据库的增删改查操作，PO一起使用。

**POJO（plain ordinary java object）-简单Java对象**

可以理解为最常用到的Java Bean，PO、VO、DTO都是典型的POJO，它是一个中间对象可以转化为PO、DTO、VO。

不同类型的“O”在不同的架构层级中扮演不同的角色，每种“O”都有不同的用途，目的就是为了更好的封装自己的服务及有效的控制数据的传播。







