# Kafka是如何保证数据的可靠性?

## 副本机制

单个*topic*可以有多个 分区, 每个分区 都可以设置 副本数量,只有一个*leader*副本进行外部数据的写入,然后由leader将数据转发给各个副本,保证集群间数据的一致.

## *acks*参数

三种选项 不等待答复|等待*leader*答复|等待所有节点答复

## ISR机制

*ISR*机制保证了leader写入数据成功并且至少有一个follower同步完成leader的数据，才会认为消息发送成功

每个partition中会维护着一个ISR列表包含leader，还有与它同步的follower

只要某个follower会同步leader的数据，那么肯定会在该列表中

如果某个follower因为自身发生问题，不能同步数据，那么会被认为“out of sync”，从ISR列表中删除

# Kafka是如何保证数据的顺序性?

*Kafka*只能保证 单个topics 的 单个*partition*, 的数据顺序

因为在*partition* 内维护了 *offset*



# Kafka的消息处理/消息交付语义

最多一次：消息可能丢失也可能被处理，但最多只会被处理一次。

至少一次：消息不会丢失，但可能被处理多次

精确一次：消息被处理且只会被处理一次



# Kafka如何判断节点是否还活着

1. 建立与zk的连接,在zk上建立一个连接节点
2. 如果是*follower*节点 能及时的同步*leader*写操作,不能延时太久

# *PULL*模式与*PUSH*模式的优缺点

*push* 

* 由*broker*决定消息推送速率,当*broker*推送的速率远大于*consumer* 消费的效率,*consumer*会崩溃
* 但能够及时的推送

*pull*

*consumer*能根据自己的消费能力去决定

需要不断轮询



# *TOPIC* partition

## 分配规则

接受 topic create 请求的 结点 为 0号分区,按照 集群ID 依次有序分配, 当*partitions*数量 > *brokers*数量,会轮回再次分配

## 命名规则

*paritions*名称为:*topic-name-index*,  *index*分区索引编号，从0开始依次递增



## 分区文件存储方式

* 将大文件切割成 *segment file*

* segment file组成：由2大部分组成，分别为*segment data file*和*segment index file*,此2个文件一一对应，成对出现.

* partition文件命名

  * partion全局的第一个segment从0开始，
  * 后续每个segment文件名为上一个segment文件最后一条消息的offset值
  * 数值最大为64位long大小，19位数字字符长度，没有数字用0填充

* *partition.log*文件 由 很多*message* 顺序存储

  *message*结构

  ![](C:\Users\weisanju\Desktop\实用图\kafkaMessage结构.png)

  

  参数说明

  | 关键字              | 解释说明                                                     |
  | :------------------ | :----------------------------------------------------------- |
  | 8 byte offset       | 在parition(分区)内的每条消息都有一个有序的id号，这个id号被称为偏移(offset),它可以唯一确定每条消息在parition(分区)内的位置。即offset表示partiion的第多少message |
  | 4 byte message size | message大小                                                  |
  | 4 byte CRC32        | 用crc32校验message                                           |
  | 1 byte “magic”      | 表示本次发布Kafka服务程序协议版本号                          |
  | 1 byte “attributes” | 表示为独立版本、或标识压缩类型、或编码类型。                 |
  | 4 byte key length   | 表示key的长度,当key为-1时，K byte key字段不填                |
  | K byte key          | 可选                                                         |
  | value bytes payload | 表示实际消息数据。                                           |

* 索引文件
  * 稀疏索引存储方式
  * 记录了 *message* 在 *log*文件中的 序号以及 对应的 字节偏移
* 如何通过 *offset* 查找相应的 *message*
  * 根据文件列表的 数字命名 二分查找 定位  相应的 *log*文件,与*index*文件
  * 通过索引文件定位  该 *offset*下的 物理字节偏移
  * 最后通过 去log文件中查找数据













# 消息队列优点

解耦,冗余,扩展,灵活,峰值处理,可恢复性,顺序保证,异步通信



# kafka架构

* 生产者

* 消费者

  * 消费者组 中的 多个客户端 不能重复消息
  * 同一个Topic的同一个分区的数据

* 集群

  集群 -> broker(节点) -> TOPIC 主题 -> 分区

* zookeeper

  ```
  systemctl stop firewalld && ./zookeeper/bin/zkServer.sh start && ./zookeeper/bin/zkServer.sh status
  ```

  

操作命令

```
解压
tar -xf kafka_2.13-2.5.0.tgz

启动
bin/kafka-server-start.sh config/server.properties
--daemon  后台
创建topic
bin/kafka-topics.sh --create --zookeeper 192.168.3.8:2181 --partitions 2 --replication-factor 3 --topic second
Created topic first.

查看topic
bin/kafka-topics.sh --list --zookeeper 192.168.3.8:2181

生产者
bin/kafka-console-producer.sh --broker-list 192.168.3.8:9092 --topic second 

控制台消费者-消费
kafka_2.13-2.5.0/bin/kafka-console-consumer.sh --broker-list 192.168.3.8:9092  --topic second
从开始消费数据 --from-begining

新版本,offset维护在本地,
--bootstrap-server 192.168.3.7:9092

描述卡夫卡
bin/kafka-topics.sh --zookeeper 192.168.3.7:2181 --describe --topic first
partitionCount:2 分区2,relicationFactor:2 复本2
partition0,leader:0,replicas:0,2, ISR:0,2 ,分区0 有两块副本,broker0为使用分区,ISR为分区不可用时的副本使用顺序

```

# 分区原则

1. 指定了分区 直接使用分区
2. 未指定partition 但指定key,通过key 的valuehash分区
3. key未指定,轮询选出 一个partition



# 副本(replication)

## 描述

* 同一个分区的有多个副本
* 需要在 这几个副本中选出 *leader*, *producer*与*consumer* 只与*leader*交互 ,其他replication作为follower 从leader中复制数据
* 副本创建好之后,会有一个 可用序列, 决定了 *leader* 不可用时,下一个可用的副本*broker*节点
* *replication* 副本之间不可能在同一个 broker

## 副本之间的同步机制

*ALL*

*producer*向*leader*写入 数据, 等待所有follower同步完成

1

*producer* 本地写到日志中后 就立刻返回

0

*producer* 不等待任何确认即返回,一般确认数为 -1



# JavaAPI

## 生产者

1. 生产者配置信息

   * *bootstrap.servers*  :

     *Kafka*集群地址,*host:port,host:port*

   * *acks*:应答级别

   * *retries*:重试次数

   * 生产者提交数据的 阈值

     * *batch.size* :批量大小
     * *linger.ms*:提交延时

   * *buffer.memory*

     * 缓存区大小

   * *KV*的序列化类

     * key.serializer*:

   * 位于*ProducerConfig*

2. 实例化生产者

   ```java
   KafkaProducer<String, String> producer = new KafkaProducer<>(p);
   ```

3. 发送消息

   ```java
     producer.send(new ProducerRecord<String, String>("first", i + ""),(metadata, exception) -> {
                   System.out.println("offset:"+metadata.offset()+";partition:"+metadata.partition());
               });
               producer.flush();
   ```

自定义分区

* 实现*Partitioner*接口

* 关键方法

  *public int partition*

  *(String topic, Object key, byte[] keyBytes,* 

  *Object value, byte[] valueBytes,* 

  *Cluster cluster);*

  根据,*topic*,*key*,序列化的*keybytes*,值,序列化的值,集群配置等决定分区

* 默认方法*onNewBatch* 
  * 创建新的批次时通知
  * API默认一个批次一个批次的推送消息过去

* 配置属性 *PARTITIONER_CLASS_CONFIG* 批次类

## 消费者

属性

* *bootstrap.servers* 集群

* 消费组ID *group.id*

* *enable.auto.commit*  自动提交*offset*

* *auto.commit.interval.ms* 提交延时

* *KV*反序列化 

* *AUTO_OFFSET_RESET_CONFIG*

  *earliest* , *lastest*,每次连接到 Kafka集群 自动 *offset*

对象实例化

*KafkaConsumer*

订阅主题

*subscribe*

拉取消息

*poll*

直接定位*offset*

*seek*



## 消费者细化API

步骤

1. 根据指定分区 从主题元数据中找到 主副本

   *findLeader*

2. 获取分区最新消费进度

   *getLastOffset*

3. 从主副本拉去分区信息

   *run*

4. 识别主副本的变化 重试

   *findNewLeader*

# 







# 拦截器

*ProducerInterceptor* 

方法

*configure*

获取配置信息和初始化数据时调用

*onsend(ProducerRecord)*

发送前,序列化前 调用

*onAcknowledement(RecordMetaData,Exception)*

在*producer*的回调 之前调用

*close* 

关闭 

设置拦截器

*Interceptor_classes_config*







## Kafka EOS 语义

*exactly once semantics*

精确一次处理语义

## **Kafka 幂等性** *Idempotent*

* *Kafka*的幂等性实现 引入了 *PID(producerID)* 和 sequenceNumber
* *pid*:对于用户透明,每个*producer*在初始化的时候会被分配一个唯一的*PID*
* *sequenceNumber* ,对于每一个PID,该*producer*发送到每个*partition*的数据都有对应的序列号,这些序列号时从0开始单调递增,broker只接收序号大于其缓存中 1 的,否则就丢弃
* 涉及的参数是 *enable.idempotence* = true
* 只能保证同个 *producer* 单会话,单个*partition* 的exactlyOnce语义

# Kafka事务

* 正是因为 Kafka幂等性不提供跨多个partition和 跨会话场景下的保证能够原子的处理多个*partition* 的写入操作

## 使用事务API注意事项

* 需要消费者的自动模式设置为 false
* 不能手动执行*consumer#commitSync*或者*consumer#commitAsyc*
* 生产者配置 *transactional*.*id* 属性
* 生产者不需要再配置 *enable.idempotence*，因为如果配置了*transaction.id*，则此时 *enable.idempotence* 会被设置为*true*
* 消费者需要配置 *isolation.level* 属性，有两个可选值："*read_committed*"，"*read_uncommitted*"，默认"*read_uncommitted*"



## 事务API

为producer提供了

*initTransactions*

*beginTransaction*

*sendOffsetsToTransaction*

*commitTransaction*

*abortTransaction* 



```
 Properties props = new Properties();
        props.put("key.serializer", "org.apache.kafka.common.serialization.StringSerializer");
        props.put("value.serializer", "org.apache.kafka.common.serialization.StringSerializer");
        props.put("client.id", "ProducerTranscationnalExample");
        props.put("bootstrap.servers", "192.168.3.7:9092");
        props.put("transactional.id", "test-transactional");
        props.put("acks", "all");

        KafkaProducer producer = new KafkaProducer(props);

        producer.initTransactions();

        try {

            String msg = "matt test";
            producer.beginTransaction();
            producer.send(new ProducerRecord("first", "1", msg.toString()));
            producer.send(new ProducerRecord("first", "1", msg.toString()));
            producer.send(new ProducerRecord("first", "1", msg.toString()));
            if(1 == 1)
            throw  new ProducerFencedException("自定义异常");

            producer.commitTransaction();
        } catch (ProducerFencedException e1) {

            e1.printStackTrace();
            producer.close();
        } catch (KafkaException e2) {
            e2.printStackTrace();
            producer.abortTransaction();

        }

        producer.close();
```



## 事务具体实现

寻找TC

* Transaction Coordinator 运行在 Kafka 服务端，下面简称 TC 服务。

* Kafka 有个特殊的事务 topic，名称为*__transaction_state*  负责持久化事务消息，有50个分区，每个分区负责一部分事务。事务划分是根据 *transaction id*，计算出该事务属于哪个分区。这个分区的 leader 所在的机器，负责这个事务的TC 服务地址

* Producer 会首先从 Kafka 集群中选择任意一台机器，然后向其发送请求，获取 TC 服务的地址

初始化事务

* Producer 在使用事务功能，必须先自定义一个唯一的 *transaction* id。有了 *transaction* id，即使客户端挂掉了，它重启后也能继续处理未完成的事务。
* Kafka 实现事务需要依靠幂等性，而幂等性需要指定 producer id 。所以Producer在启动事务之前，需要向 TC 服务申请 producer id。TC 服务在分配 producer id 后，会将它持久化到事务 topic。

发送消息

* Producer 在接收到 producer id 后，就可以正常的发送消息了。不过发送消息之前，需要先将这些消息的分区地址，上传到 TC 服务，TC 服务会将这些分区地址持久化到事务 topic

* 然后 Producer 才会真正的发送消息，这些消息与普通消息不同，它们会有一个字段，表示自身是事务消息。

发送提交请求

* TC 服务收到事务提交请求后，会先将提交信息先持久化到事务 topic
* 持久化成功后，服务端就立即发送成功响应给 Producer
* TC服务找到该事务涉及到的所有分区，为每 个分区生成提交请求，存到队列里等待发送

发送事务结果信息给分区

* 后台线程会不停的从队列里，拉取请求并且发送到分区。当一个分区收到事务结果消息后，会将结果保存到分区里，并且返回成功响应到 TC服务。当 TC 服务收到所有分区的成功响应后，会持久化一条事务完成的消息到事务 topic。至此，一个完整的事务流程就完成了







