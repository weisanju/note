# 介绍

*Reactor* 是一个 JVM上 完全 非阻塞的 响应式 编程框架，它有着高效的 需求管理（以管理"背压"的形式），它直接与 Java 8 功能 API 集成，特别是 *CompletableFuture* *Stream* *Duration* ，它提供可组合的异步序列 API `Flux` (for [N] elements)   `Mono` (for [0|1] elements) 

广泛实现了  [Reactive Streams](https://www.reactive-streams.org/)  规范



Reactor-netty 还支持 非阻塞的 跨进程 通信，适合微服务架构，Reactor Netty 为 HTTP 包括 *websockets* TCP, and UDP  提供   backpressure-ready engines  完全支持反应编码和解码。





## Prerequisites

### 传递依赖引用org.reactivestreams

It has a transitive dependency on `org.reactivestreams:reactive-streams:1.0.3`.

### 依赖Java1.8

## Understanding the BOM and versioning scheme

**BOM模型**

Reactor 3 采用 BOM 模型  （since `reactor-core 3.0.4`, with the `Aluminium` release train）

此精心策划的列表将旨在很好地协同工作的器件组。

Note the versioning scheme has changed between 3.3.x and 3.4.x (Dysprosium and Europium).

**版本命名规范**

构件采用 `MAJOR.MINOR.PATCH-QUALIFIER`  命名版本

BOM  is versioned using a CalVer inspired scheme of `YYYY.MINOR.PATCH-QUALIFIER`, 

- `MAJOR` Reactor的 generation, 每一代都能给项目结构带来根本性的变化 (这可能意外着 更重大的迁移工作)
- `YYYY` is the year of the first GA release in a given release cycle (like 3.4.0 for 3.4.x)
- `.MINOR` is a 0-based number incrementing with each new release cycle(是每个新发布版本周期的基于 0 的数字增量)
  - 就构建而言，它通常反映了更广泛的变化，可以表明只需要 适度的迁移工作
  - 在 BOM 的情况下，它允许辨别 同一年的两个 首次发布周期
- `.PATCH` 是每个服务版本的基于 0 的数字增量
- `-QUALIFIER` 是文本限定符，在 GA 版本的情况下省略（见下文）





遵循该约定的第一个发布周期是 `2020.0.x` 研发代码 `Europium` 该计划使用以下限定符（注意使用破折号分割），顺序如下：

- `-M1`..`-M9`: 里程碑（我们预计每次服务发布不超过 9 个）
- `-RC1`..`-RC9`: 发布候选项（我们预计每个服务版本不会超过 9 个）
- `-SNAPSHOT`: snapshots
- *no qualifier* for GA releases

快照在上面的顺序中显示得更高，因为从概念上讲，它们总是任何给定的 PATCH 的"最新鲜的预发布"。

即使补丁周期中的第一个部署的工件永远是 - 快照

类似命名但更新的快照也将在例如之后发布。例如：里程碑或 发布候选者之间。



**版本周期代号**

每个版本周期也给出一个代号，与以前的基于代号的方案保持连续性

可用于更非正式地引用它（比如在讨论、博客文章等。）

代号代表传统上 *MAJOR.MINOR*

它们（大部分）来自 [Periodic Table of Elements](https://en.wikipedia.org/wiki/Periodic_table#Overview),以增加字母顺序。





Up until Dysprosium the BOM 使用  release train 模式： codename跟着 qualifier,qualifier略有不同

For example: 

Aluminium-RELEASE (first GA release, would now be something like YYYY.0.0), 

Bismuth-M1, Californium-SR1 (service release would now be something like YYYY.0.1), 

Dysprosium-RC1, Dysprosium-BUILD-SNAPSHOT (after each patch, we’d go back to the same snapshot version. would now be something like YYYY.0.X-SNAPSHOT so we get 1 snapshot per PATCH)





# 获取Reactor

使用 Reactor最简单的方法是 使用 BOM 并将相关的依赖 添加到 您的项目中

注意：添加此类依赖时，必须省略版本，以便从 BOM 获取版本

As of this version (reactor-core 3.4.10), the latest stable BOM in the associated release train line is `2020.0.11`, 



which is what is used in snippets below. There might be newer versions since then (including snapshots, milestones and new release train lines), see https://projectreactor.io/docs for the latest artifacts and BOMs.

## Maven

**引入BOM**

```xml
<dependencyManagement> 
    <dependencies>
        <dependency>
            <groupId>io.projectreactor</groupId>
            <artifactId>reactor-bom</artifactId>
            <version>2020.0.11</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement>
```

**引入核心依赖**

```xml
<dependencies>
    <dependency>
        <groupId>io.projectreactor</groupId>
        <artifactId>reactor-core</artifactId> 
        
    </dependency>
    <dependency>
        <groupId>io.projectreactor</groupId>
        <artifactId>reactor-test</artifactId> 
        <scope>test</scope>
    </dependency>
</dependencies>
```

## Milestones and Snapshots

里程碑和开发人员预览 通过 Spring Milestones 仓库 分发，而不是 Maven Central.

要将其添加到构建配置文件，请使用以下片段：

```xml
<repositories>
	<repository>
		<id>spring-milestones</id>
		<name>Spring Milestones Repository</name>
		<url>https://repo.spring.io/milestone</url>
	</repository>
</repositories>

<repositories>
	<repository>
		<id>spring-snapshots</id>
		<name>Spring Snapshot Repository</name>
		<url>https://repo.spring.io/snapshot</url>
	</repository>
</repositories>
```

## Support and policies

### Stack Overflow first

Search Stack Overflow first; discuss if necessary

使用我们为此目的监控的标签中的相关标签：

- [`reactor-netty`](https://stackoverflow.com/questions/tagged/reactor-netty) for specific reactor-netty questions
- [`project-reactor`](https://stackoverflow.com/questions/tagged/project-reactor) for generic reactor questions

如果您喜欢实时讨论，我们还有几个 Gitter 频道：

- [`reactor`](https://gitter.im/reactor/reactor) 是历史上最活跃的一个, 社区的大部分可以帮助
- [`reactor-core`](https://gitter.im/reactor/reactor-core) is intended for more advanced pinpointed discussions around the inner workings of the library
- [`reactor-netty`](https://gitter.im/reactor/reactor-netty) is intended for netty-specific questions

有关潜在的其他信息来源，请参阅每个项目的 README。

我们通常不鼓励打开 Github 问题的问题， 赞成上述两个渠道。

## Our policy on **deprecations**

在处理弃用时，如果提供 A .B.C 版本，我们将确保：

- deprecations introduced in version `A`.`B`.`0` will be removed **no sooner than** version `A`.**`B+1`**.`0`

  A.B.0的弃用  在 A.B+1.0 最早移除

- deprecations introduced in version `A`.`B`.`1+` will be removed **no sooner than** version `A`.**`B+2`**.`0`

  A.B.1+的 弃用 在A.B+2 最早移除

- we’ll strive to mention the following in the deprecation javadoc:
  - target minimum version for removal：目标弃用版本
  - pointers to replacements for the deprecated method：替代方法
  - version in which method was deprecated：声明 国企的版本

## Active Development

The following table summarises the development status of the various Reactor release trains:

| Version                                                | Supported          |
| :----------------------------------------------------- | :----------------- |
| 2020.0.0 (codename Europium) (core 3.4.x, netty 1.0.x) | :white_check_mark: |
| Dysprosium Train (core 3.3.x, netty 0.9.x)             | :white_check_mark: |
| Califonium and below (core < 3.3, netty < 0.9)         | :x:                |
| Reactor 1.x and 2.x Generations                        | :x:                |





# 响应式编程介绍

Reactor 是反应编程范式的 实现：可以概括为

> 响应式编程 是一个 异步编程范式，关注 数据流的 以及 变化的 传播
>
> 这意味着 编程语言 轻松表达 静态 （例如 数据） 或者 动态（例如 事件产生器） 数据流 

作为响应式编程的第一步，微软在.NET生态系统中实现了 `Reactive Extensions` 库

然后 Rxjava 在 Jvm 上实现了反应性编程 ，随着时间的推移，通过 Reactive Streams  的努力 实现了  Java的标准化, 这个规范为 JVM库  定义了 一系列接口 和 交互规则，这些接口已集成到 Java9的 *flow* 类



响应式编程范式 通常是 作为面向对象语言中的 观察者模式的 扩展



你还可以 将 主 反应式 流 模式 和 熟悉的 迭代器模式 比较，因为对于 Interable-Iterator 对  具有二元性。一个主要的区别是  迭代器是 拉模式，reactive streams是基于 推模式

使用 迭代器 是一个 必不可少的 设计模式，即使 访问值的方法完全由 *Iterable* 负责

事实上，它是 由 开发人员选择 何时 访问序列中的  *next*，在 reactive stream中 等价的  对 是 *publisher* -*subscriber*  但是  是由 *Publisher*  通知 订阅者 最新可用的值 ,这种 *push* aspect   被称为 *reactive* 

此外：应用 push值 的 操作 是 声明式 而不是 命令式，程序员声明计算逻辑。而不是准确描述 其控制流



除了推值外， 错误处理，完成后处理也以明确的方式定义

A `Publisher` can push new values to its `Subscriber` (by calling `onNext`) but can also signal an error (by calling `onError`) or completion (by calling `onComplete`). Both errors and completion terminate the sequence. This can be summed up as follows:

*Publisher* 能够 推 新值 给 它的*Subscriber*  （通过调用 *onNext*） 但是 也可以  发出 Error信号（通过 调用 *onError*） 或者 完成 （通过调用 *onComplete*）

*error* 或 *complete* 都会 终止 序列 ，这可以概括为

```none
onNext x 0..N [onError | onComplete]
```

## Blocking Can Be Wasteful

现在应用 可会有 大量并发用户 ，尽管 现代硬件能力 得到了长足的发展，现代软件的性能 仍然是一个关键问题



大致而言，有两种方法可以改进程序的性能：

- **parallelize**  使用更多的线程和更多的硬件资源。
- **seek more efficiency** 当前资源的使用方式

通常，Java开发人员使用 阻塞式编程，这种做法 存在性能 瓶颈，然后引入额外线程 ，运行 类似的 阻塞代码 

这种资源利用规模的扩大 可以迅速 引入 数据竞争 和并发问题



更遭的是，阻塞 浪费资源,如果 你仔细观察 只要一个程序 涉及一些延迟 (特别是 I/O 例如数据库请求 或网络请求)

资源被浪费了，因为很多线程处于闲置状态，等待数据

因此 并发化并不是一颗银弹，有必要 利用硬件的 全部能力

##  Asynchronicity to the Rescue

> 异步节省资源



通过编写 异步 非阻塞代码，您可以让执行 切换到 使用相同基础资源的已活动任务，并在 异步处理完成后返回 当前 过程

但是：如何在JVM上生成 异步代码呢，Java提供了两种 异步编程模式

**Callbacks**：

异步方法没有返回值，需要有额外的回调参数

**Future**

异步方法立即 返回 `Future<T>`

异步计算过程返回 T，通过Future对象包装

该值不可立即获得，对象可以进行 *polled*，直到该值可用。

例如运行：`ExecutorService`  `Callable<T>` tasks  返回 Future队形



这些技术够好吗？并非针对每个用例，两种方法都有局限性。

回调很难组合在一起，导致难以读取 和维护 代码（known as “Callback Hell”）

考虑一个例子：展示来自 用户UI上的 TOP 5 的收藏夹 或者如果没有收藏夹就建议

这需求经过三项服务

第一个提供 favorite IDs

第二个取 favorite details

第三个 提供 建议



**传统回调**

这是很多代码，它有点难以阅读，具有重复部分

```java
userService.getFavorites(userId, new Callback<List<String>>() { 
  public void onSuccess(List<String> list) { 
    if (list.isEmpty()) { 
      suggestionService.getSuggestions(new Callback<List<Favorite>>() {
        public void onSuccess(List<Favorite> list) { 
          UiUtils.submitOnUiThread(() -> { 
            list.stream()
                .limit(5)
                .forEach(uiList::show); 
            });
        }

        public void onError(Throwable error) { 
          UiUtils.errorPopup(error);
        }
      });
    } else {
      list.stream() 
          .limit(5)
          .forEach(favId -> favoriteService.getDetails(favId, 
            new Callback<Favorite>() {
              public void onSuccess(Favorite details) {
                UiUtils.submitOnUiThread(() -> uiList.show(details));
              }

              public void onError(Throwable error) {
                UiUtils.errorPopup(error);
              }
            }
          ));
    }
  }

  public void onError(Throwable error) {
    UiUtils.errorPopup(error);
  }
});
```

**响应式**

```java
userService.getFavorites(userId) 
           .flatMap(favoriteService::getDetails) 
           .switchIfEmpty(suggestionService.getSuggestions()) 
           .take(5) 
           .publishOn(UiUtils.uiThreadScheduler()) 
           .subscribe(uiList::show, UiUtils::errorPopup); 
```

如果你想确保在不到 800 毫秒内检索到最喜欢的 ID， 该怎么办？

如果需要更长的时间，从缓存中获取它们？

在基于回调的代码中，这是一项复杂的任务

在 *Reactor* 中 它变得像在链条中添加超时操作员一样简单

```java
userService.getFavorites(userId)
           .timeout(Duration.ofMillis(800)) 
           .onErrorResume(cacheService.cachedFavoritesFor(userId)) 
           .flatMap(favoriteService::getDetails) 
           .switchIfEmpty(suggestionService.getSuggestions())
           .take(5)
           .publishOn(UiUtils.uiThreadScheduler())
           .subscribe(uiList::show, UiUtils::errorPopup);
```



Future objects 比 回调要好，但是 不能很好的组合，另外 Java8的 **CompletableFuture**  做出了改善



将多个Future对象协调在一起是可行的，但并不容易。此外，Future还有其他问题：

* get方法会阻塞
* 不支持懒 计算
* 缺乏对 多个值的支持 和高级错误处理



考虑另一个例子：

我们得到一个ID列表，我们希望从中获取一个名称和一个统计数据，并结合这些配对，所有这些都是异步的

```java
CompletableFuture<List<String>> ids = ifhIds(); 

CompletableFuture<List<String>> result = ids.thenComposeAsync(l -> { 
	Stream<CompletableFuture<String>> zip =
			l.stream().map(i -> { 
				CompletableFuture<String> nameTask = ifhName(i); 
				CompletableFuture<Integer> statTask = ifhStat(i); 

				return nameTask.thenCombineAsync(statTask, (name, stat) -> "Name " + name + " has stats " + stat); 
			});
	List<CompletableFuture<String>> combinationList = zip.collect(Collectors.toList()); 
	CompletableFuture<String>[] combinationArray = combinationList.toArray(new CompletableFuture[combinationList.size()]);

	CompletableFuture<Void> allDone = CompletableFuture.allOf(combinationArray); 
	return allDone.thenApply(v -> combinationList.stream()
			.map(CompletableFuture::join) 
			.collect(Collectors.toList()));
});

List<String> results = result.join(); 
assertThat(results).contains(
		"Name NameJoe has stats 103",
		"Name NameBart has stats 104",
		"Name NameHenry has stats 105",
		"Name NameNicole has stats 106",
		"Name NameABSLAJNFOAJNFOANFANSF has stats 121");
```

由于 Reactor拥有 更多 开箱即用的 组合操作，因此过程可以 简化如下

```java
Flux<String> ids = ifhrIds(); 

Flux<String> combinations =
		ids.flatMap(id -> { 
			Mono<String> nameTask = ifhrName(id); 
			Mono<Integer> statTask = ifhrStat(id); 

			return nameTask.zipWith(statTask, 
					(name, stat) -> "Name " + name + " has stats " + stat);
		});

Mono<List<String>> result = combinations.collectList(); 

List<String> results = result.block(); 
assertThat(results).containsExactly( 
		"Name NameJoe has stats 103",
		"Name NameBart has stats 104",
		"Name NameHenry has stats 105",
		"Name NameNicole has stats 106",
		"Name NameABSLAJNFOAJNFOANFANSF has stats 121"
);
```



## From Imperative to Reactive Programming

Reactive 库例如 Reactor 旨在 解决 经典异步方法的缺点 同时关注其他几个方面

1. 组合型跟可读性
2. 数据流动，丰富的操作方法
3. 懒加载
4. 被压：消费者向 生产者 发出 生产速率 过高的信号的能力
5. 高度抽象，高度价值 抽象   *concurrency-agnostic*

### Composability and Readability

通过 可组合性，我们有能力能 协调多个 异步任务，我们使用 以前任务的结果输入到 后续的调用中 或者 我们可以 fork-join style 运行任务 ，此外在更高级的组件中，作为 离散组件  我们可以重用 异步任务 



协调任务的能力与 可读性 和可维护性的代码 紧密结合

随着异步 过程 层在数量和 复杂性上 都有所增加 

能够编写 和阅读代码 变得越来越困难，正如所看到的  回调模型很简单 

主要缺点之一是：回调地狱

Reactor提供了丰富的选项组合，其中 代码反映了 抽象过程的组织，并且所有内容通常 保持在同一水平 （嵌套最小化）

### The Assembly Line Analogy

> 装配线类比





您可以将响应式应用程序 处理的数据 视为 通过装配线移动

Reactor 既是 传送带 也是 工作站 ，原料 从源头（原始 *Publisher* ）倾泻而出 

最终成为成品 准备将推给 消费者（订阅者）



原材料 可以经历 各种转换 和其他中间步骤 或成为  将中间件聚合在一起的 大型装配线 的一部分 

如果某时刻 出现故障 或阻塞 （业务 boxing 产品 花费很长的时间）

负载严重的工作站 可以向上游发出信号，以限制原材料的流动







### Operators

Reactor中，操作就是工作站，每一个操作 都会 将行为添加到 *Publisher* 中

并将前一步的 Publisher 包装成一个 新的 实例

因此整个链条是相连的

这样 数据源自 第一个 *Publisher* 沿着 链条向下移动，由每个链节点 转换，最终订阅者 完成了该过程，如果没有 Subscriber没有订阅 ，

了解 operators 创建 新实例 帮助您 避免 常见的误区，这个误区 会导致您相信 一个操作 在链条中没有被应用



虽然 Reactive Streams 规范 没有 规定操作 

one of the best added values of reactive libraries, such as Reactor, is the rich vocabulary of operators that they provide. 

这些覆盖了很多面，从简单的 转换 和过滤 到复杂的 编排 和错误处理

### Nothing Happens Until You `subscribe()`

In Reactor, 当你 编写 *Publisher* 链时，默认情况下 数据不会 泵入该链条

相反：您可以创建 异步过程的 抽象描述 （这有助于重用和组合）

通过订阅的 行为 您将 发布者 与 订阅者 联系起来 ，从而触发整个链中的数据流。

这是通过 从上游 传播的 Subscriber 的单个 request请求 信号内部实现的，一直传回源  发布者



### Backpressure

上游传播信号也用于实现**背压**，我们在装配线类比中 描述为当 工作站比上游处理得慢时，向线路发送发聩信号

 Reactive Streams specification定义的 真实机制 非常接近类比

订阅者可以 在 无限制 的模式 工作 ，并让源 以最快的 速度 推送数据

或者 它可以使用 请求 机制 向 源 发出信号，表明 它已准备好 处理最多 n个元素



中间操作 同样可以更改 在途请求，想象 一个缓冲操作员将元素 分十批分组 

如果订阅者 请求一个缓冲 则 源可以生成 是个元素

一些操作 同样实现了 预取 策略 这避免了 request(1) 的 *round-trips* 

如果在要求 之前 生产 元素不是 太昂贵  则是有益的



这将 推送模型 将 转化为  **push-pull hybrid** ，如果上游随时可用，下游可以从上游拉n元素

如果 元素尚未准备好 ，他们一产生就会 被 上游推

### Hot vs Cold

Rx 家族的 反应式库 区分了 两大类反应式 序列库 **hot** and **cold**.

这种区别 主要与  reactive stream 如何与 subscribers 相关联起来

- A **Cold** 序列 为每一个 Subscriber开启一个新的 包括数据源. 如果源包住Http调用 则为 每个订阅提出 新的 HTTP 请求

- **hot** 序列 不会为 每个 *subscriber* 从头开始

  收到 他们订阅后 ， 延迟订阅 者 会发出的信号 

  但是：请注意，某些热 reactive stream 可以完全 或部分的缓存或重播 安排的历史

  从一般角度 来看，当没有subscriber监听时   热序列 可以 发出 信号（订阅前不做任何事的 规则的 例外）



# 快速开始

`Flux<T>`  是 `Reactive Streams` 体系中的  `Publisher`，它有许多 *operator* ,可用于生成，转换或编排Flux序列

它可以发出0到n个元素（*onNext*事件），要么完成或者出错（*onComplete*和*onError*终止事件）。
如果未触发任何终止事件，则 *Flux*是无限的。

- Flux上的静态工厂允许创建源，或从几种回调类型生成 *Publisher*
- 实例方法，operators,使您可以构建异步处理管道，该管道将产生异步序列
- 每个`Flux＃subscribe()`或*multicasting* (多播操作)（例如`Flux#publish`和`Flux#publishNext`）都会具体化管道的专用实例并触发其中的数据流。



## 订阅

### 订阅

```java
subscribe(Consumer<? super T> consumer,
          Consumer<? super Throwable> errorConsumer,
          Runnable completeConsumer,
          Consumer<? super Subscription> subscriptionConsumer); 
```

处理值 以及 异常 或者成功，同样 触发订阅事件的处理

### 取消订阅

> disposable

**disposable**

所有这些基于 *lambda* 的 `subscribe（）`变体都具有*Disposable*返回类型。
在这种情况下，Disposable接口表示可以通过调用其`dispose（）`方法来取消订阅的事实。

对于 *Flux* or *Mono*，取消 是 信号源 应停止产生元素的信号。
但是，并不能保证立即执行：某些源可能会产生如此快的元素，以至于甚至在接收到取消指令之前它们也可以完成

**swap**

Disposables类中提供了一些有关Disposable的实用程序。
其中，`Disposables.swap（）`创建一个Disposable包装器，使您可以原子地取消和替换一个具体的Disposable。

```
例如，这在UI场景中很有用，在UI场景中，您希望在用户单击按钮时取消请求并将其替换为新的请求。
关闭 包装器本身也会 关闭 它
这样做会 处理 当前的 具体价值 以及将来所有尝试的替代产品。
```

**composite**

您可以收集多个Disposable（例如，与服务调用关联的多个进行中的请求），并在以后一次将所有这些都处置。
在调用组合的dispose（）方法后，任何尝试添加另一个Disposable的尝试都会立即将其处置。

**BaseSubscriber**

额外的订阅方法，该方法更通用并且采用成熟的订阅服务器，而不是从一个lambda中组成一个。
为了帮助编写这样的订阅服务器，我们提供了一个称为 *BaseSubscriber* 的可扩展类。

BaseSubscriber（或其子类）的实例是一次性的，这意味着，

如果BaseSubscriber 同时 订阅两个发布者，只有一个能成功，需要CAS 操作去 争抢，失败的则会自行 取消

这是因为使用实例两次将违反“Reactive Stream”规则，即不得并行调用订阅服务器的onNext方法。



## StepVerifier

> 步骤验证器

使用 `StepVerifier` 来 定义一个测试单元 来检查 每个测试的结果

这个类来自 `reactor-test` 构件，能够订阅任何 `Publisher` ，然后针对改序列 申明 一些列用户定义的 期望

如果触发了任何与当前期望不符的事件，则`StepVerifier`将产生一个“ AssertionError”。

您可以从静态工厂`create`获取`StepVerifier`的实例。它提供了一个DSL来设置数据部分的期望值，并以单个终端期望值（完成，错误，取消...）结束。

获取到实例后，必须调用*verify* 方法，或者  结合 *termination* 期望和验证的快捷方式之一 例如：`.verifyErrorMessage(String)` 

```java
StepVerifier.create(T<Publisher>).{expectations...}.verify()
```

