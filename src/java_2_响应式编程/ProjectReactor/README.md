# 介绍

*Reactor* 是一个 JVM上 完全 非阻塞的 响应式 编程框架，它有着高效的 需求管理（以管理"背压"的形式），它直接与 Java 8 功能 API 集成，特别是 *CompletableFuture* *Stream* *Duration* ，它提供可组合的异步序列 API `Flux` (for [N] elements)   `Mono` (for [0|1] elements) 

广泛实现了  [Reactive Streams](https://www.reactive-streams.org/)  规范



Reactor-netty 还支持 非阻塞的 跨进程 通信，适合微服务架构，Reactor Netty 为 HTTP 包括 *websockets* TCP, and UDP  提供   backpressure-ready engines  完全支持反应编码和解码。





## Prerequisites

### 传递依赖引用org.reactivestreams

It has a transitive dependency on `org.reactivestreams:reactive-streams:1.0.3`.

### 依赖Java1.8

## Understanding the BOM and versioning scheme

Reactor 3 采用 BOM 模型  （since `reactor-core 3.0.4`, with the `Aluminium` release train）

此精心策划的列表将旨在很好地协同工作的器件组。

Note the versioning scheme has changed between 3.3.x and 3.4.x (Dysprosium and Europium).



构件采用 `MAJOR.MINOR.PATCH-QUALIFIER`  命名版本

BOM  is versioned using a CalVer inspired scheme of `YYYY.MINOR.PATCH-QUALIFIER`, 

- `MAJOR` Reactor的 generation, 每一代都能给项目结构带来根本性的变化 (这可能意外着 更重大的迁移工作)
- `YYYY` is the year of the first GA release in a given release cycle (like 3.4.0 for 3.4.x)
- `.MINOR` is a 0-based number incrementing with each new release cycle(是每个新发布版本周期的基于 0 的数字增量)
  - 就构建而言，它通常反映了更广泛的变化，可以表明只需要 适度的迁移工作
  - 在 BOM 的情况下，它允许辨别 同一年的两个 首次发布周期
- `.PATCH` 是每个服务版本的基于 0 的数字增量
- `-QUALIFIER` 是文本限定符，在 GA 版本的情况下省略（见下文）



The scheme uses the following qualifiers (note the use of dash separator), in order:

遵循该约定的第一个发布周期是 `2020.0.x` 研发代码 `Europium` 该计划使用以下限定符（注意使用破折号分割），顺序如下：

- `-M1`..`-M9`: 里程碑（我们预计每次服务发布不超过 9 个）
- `-RC1`..`-RC9`: 发布候选项（我们预计每个服务版本不会超过 9 个）
- `-SNAPSHOT`: snapshots
- *no qualifier* for GA releases

snapshots appear higher in the order above because, conceptually, they’re always "the freshest pre-release" of any given PATCH. 

快照在上面的顺序中显示得更高，因为从概念上讲，它们总是任何给定的 PATCH 的"最新鲜的预发布"。

Even though the first deployed artifact of a PATCH cycle will always be a -SNAPSHOT

即使补丁周期中的第一个部署的工件永远是 - 快照

类似命名但更新的快照也将在例如之后发布。例如：里程碑或 发布候选者之间。



每个版本周期也给出一个代号，与以前的基于代号的方案保持连续性

可用于更非正式地引用它（比如在讨论、博客文章等。）

代号代表传统上 *MAJOR.MINOR*

它们（大部分）来自 [Periodic Table of Elements](https://en.wikipedia.org/wiki/Periodic_table#Overview),以增加字母顺序。



Up until Dysprosium the BOM 使用  release train 模式： codename跟着 qualifier,qualifier略有不同

For example: 

Aluminium-RELEASE (first GA release, would now be something like YYYY.0.0), 

Bismuth-M1, Californium-SR1 (service release would now be something like YYYY.0.1), 

Dysprosium-RC1, Dysprosium-BUILD-SNAPSHOT (after each patch, we’d go back to the same snapshot version. would now be something like YYYY.0.X-SNAPSHOT so we get 1 snapshot per PATCH)



# 快速开始

`Flux<T>`  是 `Reactive Streams` 体系中的  `Publisher`，它有许多 *operator* ,可用于生成，转换或编排Flux序列

它可以发出0到n个元素（*onNext*事件），要么完成或者出错（*onComplete*和*onError*终止事件）。
如果未触发任何终止事件，则 *Flux*是无限的。

- Flux上的静态工厂允许创建源，或从几种回调类型生成 *Publisher*
- 实例方法，operators,使您可以构建异步处理管道，该管道将产生异步序列
- 每个`Flux＃subscribe()`或*multicasting* (多播操作)（例如`Flux#publish`和`Flux#publishNext`）都会具体化管道的专用实例并触发其中的数据流。

## 创建FLux的几种方式



### Empty flux

```java
static <T> Flux<T> empty()
```

### Flux from values

```
static <T> Flux<T> just(T... data)
```

```java
Flux.just("12","34","56").subscribe(System.out::println);
```



### Flux from iterator

```
static <T> Flux<T> fromIterable(Iterable<? extends T> it)
```

```java
Flux<Integer> objectFlux = Flux.fromIterable(() -> new Iterator<Integer>() {
            int i = 0;

            @Override
            public boolean hasNext() {
                return i <= 100;
            }

            @Override
            public Integer next() {
                return i++;
            }
        });
        objectFlux.subscribe(System.out::println);
```



### Create a Flux that emits an IllegalStateException

```java
static <T> Flux<T> error(Throwable error)
    
Flux<Object> error = Flux.error(new RuntimeException("自定义异常")).onErrorStop();
error.subscribe(e-> System.out.println(e));
```

### 计数器

```java
static Flux<Long> interval(Duration period)
Flux.interval(Duration.ofMillis(100)).take(10).subscribe(e-> System.out.println(e));
```

## 创建Mono的几种方式

Mono 是Reactive Streams发布者，增加了几种运算符，可用于生成，转换或编排Mono序列。

它是Flux的特化，最多可发出1个元素：Mono的值

* 带元素 完成

* 空（不带元素的完成）
* 或失败（错误）。

`Mono<Void>`可以用于仅关注完成信号的情况（相当于可运行任务的Reactive Streams完成）。

Like for `Flux`, the operators can be used to define an asynchronous pipeline which will be materialized anew for each `Subscription`.

跟 *Flux*  *operator* 一样 ，可用于定义异步管道，该管道将针对每个“订阅”重新实现。

请注意，某些更改序列的个数的API会返回“ Flux”（反之亦然，将“ Flux”中的个数减小为1的API会返回“ Mono”）。

### Empty Mono

```java
static <T> Mono<T> empty()
Mono.empty()
```

### fromValue

```java
Mono.just(1);
```

### 永不触发的Mono

```java
Mono.never()
```

### 触发Error

```java
Mono.error(new IllegalStateException());
```



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

