# 什么是响应式编程

**异步回调地狱**

异步编程时，存在很多难题，比如典型的`回调地狱(Callback Hell)`，一层套一层的回调函数简直是个灾难，这里列出几个异步编程常见的问题：

1. 超时、异常处理困难
2. 难以重构
3. 多个异步任务协同处理

**编程范式**

就像面向对象编程，函数式编程一样，反应式编程也是另一种编程范式，响应式编程是一种新的编程范式，可以使用 申明式代码，类似函数式编程来构建异步处理管道，这是一个基于事件的模型，在数据可用时将数据推送到使用者

**标准制定**

当越来越多的开发人员使用这种编程思想时，自然而然需要一套统一的规范，2013年底Netflix，Pivotal和Lightbend中的工程师们，启动了Reactive Streams项目，希望为异步流(包含背压)处理提供标准，它包括针对运行时环境（JVM和JavaScript）以及网络协议的工作。



# 概念

处理数据流，特别是实时数据，其体积未预先确定

最突出的问题：需要严格控制资源消耗，以防快速的 数据源 不会压倒流目的地



Reactive Streams  的主要目标是 管理异步边界的流数据交换，可以认为是将元素传递到另一个线程或线程池，同时确保接收方不会被迫缓冲任意数量的数据

换句话说，背压是此模型的一个组成部分，以便允许在线程之间进行调解的队列被绑定。

The benefits of asynchronous processing would be negated if the backpressure signals were synchronous (see also the [Reactive Manifesto](http://reactivemanifesto.org/)), 

如果背压信号是同步的（另见[反应宣言]（http://reactivemanifesto.org/），异步处理的好处将不存在

在 Reactive Streams 实现中，需要考虑到 完全的非阻塞和异步行为



本规范的目的是允许创建许多符合的实现，通过遵守规则就能顺利地互操作，在整个流应用程序的处理图中保留上述优势和特征。



应当指出，本规范不包括流操作的精确性质（转换、拆分、合并等）

Reactive Streams 只关心 在不同的 API组件中 调解 数据流

总之：Reactive Streams 是面向流的 JVM库的 标准和规范

- 处理可能不受限制的元素数量
- 顺序处理
- 异步传递组件之间的元素，
- 具有强制性的非阻塞背压

反应流规范包括以下部分：

***The API*** ：API组件定义

***The Technology Compatibility Kit (TCK)*** 是实现的符合性测试的标准测试套件

只要符合 API 要求并通过 TCK 中的测试，实施可以自由实现规范未涵盖的其他功能。

`Reactive Streams API`中仅仅包含了如下四个接口：

```java
//发布者
public  interface  Publisher < T > {
    public  void  subscribe（Subscriber <？super  T >  s）;
}
//订阅者
public  interface  Subscriber < T > {
    public  void  onSubscribe（Subscription  s）;
    public  void  onNext（T  t）;
    public  void  onError（Throwable  t）;
    public  void  onComplete（）;
}
//表示Subscriber消费Publisher发布的一个消息的生命周期
public interface Subscription {
    public void request(long n);
    public void cancel();
}
//处理器，表示一个处理阶段，它既是订阅者也是发布者，并且遵守两者的契约
public interface Processor<T, R> extends Subscriber<T>, Publisher<R> {}
```

**背压(back-pressure)**

背压是从流体动力学中借用的类比, 在维基百科的定义是：抵抗所需流体通过管道的阻力或力。

在软件环境中，可以调整定义：**通过软件抵抗所需数据流的阻力或力量。**

**解决问题**

背压是为了解决这个问题的：上游组件了过量的消息，导致下游组件无法及时处理，从而导致程序崩溃。

对于正遭受压力的组件来说，无论是灾难性地失败，还是不受控地丢弃消息，都是不可接受的。既然它既不能应对压力，又不能直接做失败处理，那么它就应该向其上游组件传达其正在遭受压力的事实，并让它们降低负载。

这种背压（back-pressure）是一种重要的反馈机制，使得系统得以优雅地响应负载，而不是在负载下崩溃。相反，如果下游组件比较空闲，则可以向上游组件发出信号，请求获得更多的调用。



# 事件发布

Publisher 是潜在无限数量的序列元素的提供者，根据订阅者的需求发布这些元素。

为了响应   `Publisher.subscribe(Subscriber)` 的呼叫，`Subscriber`上方法的可能调用顺序遵循以下协议

```
onSubscribe onNext* (onError | onComplete)?
```

This means that `onSubscribe` is always signalled, 

1. 这意味着 `onSubscribe` 总是发出信号，
2. 然后是可能未绑定的"OnNext"信号（as requested by `Subscriber`）
3. 然后是 `onError`（如果出现故障）或 `onComplete` 信号（只要 `Subscription` 未取消时 且没有更多元素可用）。







# 与Java1.8、Java1.9的关系

Reactive Streams不要求必须使用Java8，Reactive Streams也不是Java API的一部分。

但是使用Java8中lambda表达式的存在，可以发挥Reactive Streams规范的强大特性，比如Reactive Streams的实现`Project Reactor`项目的当前版本，就要求最低使用Java1.8。

# 具体实现框架

Reactive Streams的实现现在比较多了，David Karnok在Advanced Reactive Java这边文章中，将这些实现分解成几代，也可以侧面了解反应式编程的发展史。

**RxJava**

RxJava是ReactiveX项目中的Java实现。ReactiveX项目实现了很多语言，比如JavaScript，.NET（C＃），Scala，Clojure，C ++，Ruby，Python，PHP，Swift等。

RxJava早于Reactive Streams规范。虽然RxJava 2.0+确实实现了Reactive Streams API规范，单使用的术语略有不同。

**Reactor**

Reactor是Pivotal提供的Java实现，它作为Spring Framework 5的重要组成部分，是WebFlux采用的默认反应式框架。

**Akka Streams**

Akka Streams完全实现了Reactive Streams规范，但Akka Streams API与Reactive Streams API完全分离。

**Ratpack**

Ratpack是一组用于构建现代高性能HTTP应用程序的Java库。Ratpack使用Java 8，Netty和Reactive原则。可以将RxJava或Reactor与Ratpack一起使用。

**Vert.x**

Vert.x是一个Eclipse Foundation项目，它是JVM的多语言事件驱动的应用程序框架。Vert.x中的反应支持与Ratpack类似。Vert.x允许我们使用RxJava或其Reactive Streams API的实现。



**互操作性**

在Reactive Streams之前，各种反应库无法实现互操作性。早期版本的`RxJava`与`Project Reactor`的早期版本不兼容。

另外，反应式编程无法大规模普及，一个很重要的原因是并不是所有库都支持反应式编程，当一些类库只能同步调用时，就无法达到节约性能的作用了。

Reactive Streams的推出统一了反应式编程的规范，并且已经被Java9集成。由此，不同的库可以互操作了，互操作性是一个重要的多米诺骨牌。

例如，MongoDB实现了Reactive Streams驱动程序后，我们可以使用Reactor或RxJava来使用MongoDB中的数据



# SPECIFICATION

## Publisher

```java
public interface Publisher<T> {
    public void subscribe(Subscriber<? super T> s);
}
```

### **`OnNext`信号总数**

`Publisher` 向 `Subscriber` 发布的 *OnNext* 信号 总数 必须小于等于 该 *Subscriber* 的订阅要求的总数

此规则的主要目的是表明：发布者不能发出比订阅者要求的更多的元素信号

此规则有一个隐含但重要的后果：由于需求只有在收到需求后才能实现，因此请求元素和接收元素之间之间存在一种先发生的关系。

### *Publisher* 产生的信号总数

`Publisher` 可能发出的 `onNext` 信号比请求的要少，并通过 调用 `onComplete` or `onError` 来终止 `Subscription`

此规则的目的是要表明：*Publisher* 不能保证它 能够产生所需求的元素数量，他可能根本无法生产他们所有：它可能处于失败状态;它可能是空的或其他已经完成

### 信号触发的是串行的

`onSubscribe`, `onNext`, `onError` and `onComplete` signaled to a `Subscriber` MUST be signaled [serially](https://github.com/reactive-streams/reactive-streams-jvm/tree/v1.0.3#term_serially).

### Publisher失败触发信号

If a `Publisher` fails it MUST signal an `onError`.



此规则的目的是要明确说明，如果发布者发现无法继续订阅，则有责任通知其订阅者

订阅者必须有机会清理资源或以其他方式处理  *Publisher* 的失败

### Publisher成功触发完成信号

If a `Publisher` terminates successfully (finite stream) it MUST signal an `onComplete`.

此规则的目的是表明，发布者负责通知其订阅者，它已达到 [terminal state](https://github.com/reactive-streams/reactive-streams-jvm/tree/v1.0.3#term_terminal_state) ，订阅者可以根据此信息采取行动：清理资源

### Publisher失败或成功要取消订阅

如果"发布者"在"订阅者"上发出    `onError` or `onComplete` 的信号，则必须考虑取消"订阅者"的"订阅"。

此规则的目的是确保订阅无论是否被取消，发布者都受到相同的对待

### Publisher处于终止时不在触发信号

Once a [terminal state](https://github.com/reactive-streams/reactive-streams-jvm/tree/v1.0.3#term_terminal_state) has been signaled (`onError`, `onComplete`) it is REQUIRED that no further signals occur.

此规则的目的是确保在Error和Complete上是发布者和订阅者对之间交互的最终状态



### `Subscription`被取消则不再接收信号

If a `Subscription` is cancelled its `Subscriber` MUST eventually stop being signaled

此规则的目的是确保发布者尊重订阅者在调用订阅时取消订阅的请求。**最终**的原因是，由于异步，信号可能会有传播延迟



### Subscriber的onSubscribe最先调用

`Publisher.subscribe`方法,必须  在任何其他信号之前  调用 提供的  `Subscriber` 上的  `onSubscribe` 方法 ，MUST [return normally](https://github.com/reactive-streams/reactive-streams-jvm/tree/v1.0.3#term_return_normally)

如果提供的 *Subscriber* 是空 抛出 NPE给调用者

对于所有其他情况，发出故障信号（或拒绝"订阅者"）的唯一合法方式是 调用 `OnError` (在调用 `onSubscribe` )



此规则的目的是确保"订阅"  始终在任何其他信号之前发出信号，以便订阅者可以在收到信号时执行初始化逻辑。

此外， "订阅" 最多只能调用一次

如果提供的"订阅者"是"空"，除了向调用者 发出信号，别无他法，例如可能会抛出 NPE

可能的情况：一个有状态的 *Publisher*  可能会不堪重负，受有限数量的基础资源限制用尽，或者处于 [terminal state](https://github.com/reactive-streams/reactive-streams-jvm/tree/v1.0.3#term_terminal_state).





### Publisher.subscribe多次调用不同Subscribe

`Publisher.subscribe` 可以 随需 调用多次 但是每次必须与不同的  `Subscriber`

此规则的目的是让"订阅"调用者 了解到：a generic Publisher and a generic Subscriber 不能支持多次 附加

此外，它还要求无论 `subscribe` 的语义被调用多少次，都必须得到维护。



### 多个`Subscriber`的支持

A `Publisher` MAY support multiple `Subscriber`s and decides whether each `Subscription` is unicast or multicast.

发布者可能支持多个 订阅者，并决定每个`订阅` 是单波还是 多播

此规则的目的是让发布者实现灵活决定他们将支持多少（如果有的话）订阅者，以及如何分发元素



## Subscriber 

```java
public interface Subscriber<T> {
    public void onSubscribe(Subscription s);
    public void onNext(T t);
    public void onError(Throwable t);
    public void onComplete();
}
```



### *Subscriber* 通过*request*发出信号接收请求

*Subscriber* 必须通过  `Subscription.request(long n)` 发出 信号需求以 接收 `onNext` signals

此规则的目的是确定 `Subscriber`  有责任决定  何时以及能够和愿意接收多少元素

为避免重新加入订阅方法导致信号重新订购，

强烈建议同步订阅者实现在任何信号处理结束时调用订阅方法。

建议订阅者请求其 能够处理的内容的上限，因为一次只请求一个元素会导致固有的低效"停止和等待"协议



### 建议使用异步处理信号

如果"订阅者"怀疑其处理信号会对其"发布者"的责任产生负面影响，则建议其异步发送信号。

此规则的意图是，订阅者不应从执行角度阻止 发布者的 进程

换句话说：订阅者不应使发布者无法接收 CPU 周期。



### 终止状态的信号中 不应该调发布订阅中的方法

`Subscriber.onComplete()` and `Subscriber.onError(Throwable t)` MUST NOT call any methods on the `Subscription` or the `Publisher`.

此规则的目的是防止在处理完成信号期间 避免 发布者、订阅、订阅者 之间的出现循环 或者 竞争



### 收到终止信号后必须考虑取消状态

`Subscriber.onComplete()` and `Subscriber.onError(Throwable t)` MUST consider the Subscription cancelled after having received the signal

此规则的目的是：确保 Subscribers 尊重 Publisher的  [terminal state](https://github.com/reactive-streams/reactive-streams-jvm/tree/v1.0.3#term_terminal_state) 信号，一旦 收到 *onComplete* *onError*  信号，一个订阅过程 就无效了





### 阻止多个发布者订阅同一个订阅者

在 *onSubscribe* 信号之后， 在一个给定的 *Subscription*中 ，如果已经存在一个 活跃的 `Subscription` 则 `Subscriber` 必须 调用  `Subscription.cancel()` 

此规则的目的是防止两个或更多单独的发布者尝试与同一订阅者进行交互

Enforcing this rule means that resource leaks are prevented since extra Subscriptions will be cancelled

执行此规则意味着防止资源泄漏，因为额外的订阅将被取消。

如果不符合此规则，可能导致违反 Publisher rule 1。此类违规行为可能导致难以诊断的错误



### Subscription不用之后需要Cancel

一个 `Subscription` 如果不再需要了，则 *Subscriber*  必须 调用  `Subscription.cancel()` 



### Cancel有延迟

*Subscriber* 必须有能力处理下列情况

当 调用 *Subscription.cancel* 时，如果还存在 请求的元素 正 pending中，能够接收一个或多个 *onNext* 信号，Subscription.cancel() 不保证立即执行基础清洁操作

此规则的目的是强调，在调用 `cancel` 和  `publisher` 遵守 cancel之间可能会有延迟。



### 允许流提前完成

一个 *Subscriber* 必须能够 处理 `onComplete` 信号 无论是否前置调用 `Subscription.request(long n)` 

此规则的目的是确定 completion 与需求流无关，这允许流提前完成，并避免 *poll* 完成的需要。





### OnError与信号需求无关

订阅者 必须准备好接收 *OnError* 信号，无论是否事先发出 `Subscription.request(long n)`

此规则的目的是确定发布者故障可能与信号需求完全无关。这意味着订阅者不需要 *poll* 来了解发布者是否无法满足其请求





### 信号的异步处理

`订阅者` 必须确保在处理相关信号之前：所有 发生在其 *signal* 方法 的调用 

Subscriber必须确保 所有 信号方法的调用 发生于 信号处理之前

即订阅者必须注意正确发布信号以达到其处理逻辑。

此规则的目的是确定订阅者实现的责任，以确保其信号的异步处理是线程安全的

[JMM definition of Happens-Before in section 17.4.5](https://docs.oracle.com/javase/specs/jls/se8/html/jls-17.html#jls-17.4.5)





### Subscriber对于一个*Publisher*只能调用一次

`Subscriber.onSubscribe` 对于给定的 Subscriber（ (based on object equality)最多只能被调用一次

此规则的目的是确定必须假定最多只能订阅一次相同的订阅者



### 信号方法必须 ReturnNormal

调用  `onSubscribe`, `onNext`, `onError` or `onComplete`  必须 [return normally](https://github.com/reactive-streams/reactive-streams-jvm/tree/v1.0.3#term_return_normally) 

除非任何提供的参数是 "空"，在这种情况下 它 必须向 调用者 抛出 java.lang.NullPointerException

对于所有其他情况，"订阅者"发出故障信号的唯一合法方式是取消其 "订阅"。

如果违反此规则，任何与 *subscriber* 关联的 *Subscription* 必须 被取消，调用者必须以适应 运行时环境的方式 抛出此 错误状态



此规则的目的是确定 订阅者的方法的语义，以及允许发布者在违反此规则的情况下做什么的语义

«Raise this error condition in a fashion that is adequate for the runtime environment» 可能意味着记录错误

或者使某人某事 意识到这种情况，因为错误不能向有故障的订阅者发出信号



## Subscription

```java
public interface Subscription {
    public void request(long n);
    public void cancel();
}
```



### Subscriber控制请求

*Subscription.request* *Subscription.cancel* 必须 在 *Subscriber* *context* 中调用

此规则的目的是确定  Subscription  代表订阅者和发布者之间的独特关系

订阅者可以控制何时请求元素以及何时不再需要更多元素



### Request可重复调用

 `Subscription`  必须 允许 `Subscriber`   在`onNext` or `onSubscribe`   中  同步地调用   `Subscription.request`  

此规则的目的是明确  `request`  的实现 必须可重入，以避免在  `request` `onNext`  之间相互重复的情况下出现堆栈溢出（最终`onComplete` / `onError`）之间发生堆栈溢出

这意味着发布者可以是"同步的"，即在称为"请求"的线程上发出"onNext"的信号





### Request OnNext递归上限

`Subscription.request` 在  `Publisher` and `Subscriber`的递归调用之间 放置一个 上限

此规则的目的是通过对 `request` `onNext` 之间的递归调用 设置上限来补充（最终 *onComplete* *onError*）

Implementations are RECOMMENDED to limit this mutual recursion to a depth of `1` (ONE)—for the sake of conserving stack space.

为了节省堆栈空间，建议实现将这种相互递归限制为"1"（One）深度



### Request应该足够快

`Subscription.request`应该遵循 调用者的 职责 。应及时返回

此规则的目的是确定 *request* 被实现为 轻量级 非阻塞方法，能在调用线程中 尽快执行，避免 重计算，及其他拖慢调用线程的执行



### Cancel应该幂等线程安全快

 `Subscription.cancel`  必须 遵循 调用者的 责任，及时返回，必须是幂等、线程安全

此规则的目的是确定 *cancel* 旨在 设计为 非阻塞 方法，能够在调用线程尽快执行，避免重计算和其他会拖慢 调用者 线程执行的 事情。

此外，还必须可以多次调用它，而不会产生任何不利影响。



### 取消后的Request是 No-op的

Subscription被取消后，对  `Subscription.request(long n)`的调用 都应该是 [NOPs](https://github.com/reactive-streams/reactive-streams-jvm/tree/v1.0.3#term_nop).

此规则的目的是在取消订阅与随后 *non-operation* *request* 更多元素之间建立因果关系

此规则被  [3.5](https://github.com/reactive-streams/reactive-streams-jvm/tree/v1.0.3#3.5) 取代了



### Request is an additive operation

当 Subscription未被 取消时， `Subscription.request(long n)` 必须注册 给定数量的额外元素，这些元素将会产生给 相应的  *Subscriber*

此规则的目的是确保 *request* 是一个 附加的操作，也确保 元素的请求 被 递送到 *Publisher*



### Request Param 参数小于等于0时触发onError

当 *Subscription* 没有取消时，使用参数小于等于0 调用 `Subscription.request(long n)` 必须发出 *onError* 信号，带有java.lang.IllegalArgumentException 

Request Param

当订阅未被取消， `Subscription.request(long n)`   



### Request 可以同步调用onNext

虽然"订阅"未被取消，但 `Subscription.request(long n)`  可在此（或其他）订阅者上同步调用"onNext"

此规则的目的是确定允许创建同步发布者，即在 调用线程上执行逻辑的发布者







While the `Subscription` is not cancelled, `Subscription.request(long n)` MAY synchronously call `onComplete` or `onError` on this (or other) subscriber(s)



| ID                                                           | Rule                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| 11                                                           | While the `Subscription` is not cancelled, `Subscription.request(long n)` MAY synchronously call `onComplete` or `onError` on this (or other) subscriber(s). |
| [💡](https://github.com/reactive-streams/reactive-streams-jvm/tree/v1.0.3#3.11) | *The intent of this rule is to establish that it is allowed to create synchronous Publishers, i.e. Publishers who execute their logic on the calling thread.* |
| 12                                                           | While the `Subscription` is not cancelled, `Subscription.cancel()` MUST request the `Publisher` to eventually stop signaling its `Subscriber`. The operation is NOT REQUIRED to affect the `Subscription` immediately. |
| [💡](https://github.com/reactive-streams/reactive-streams-jvm/tree/v1.0.3#3.12) | *The intent of this rule is to establish that the desire to cancel a Subscription is eventually respected by the Publisher, acknowledging that it may take some time before the signal is received.* |
| 13                                                           | While the `Subscription` is not cancelled, `Subscription.cancel()` MUST request the `Publisher` to eventually drop any references to the corresponding subscriber. |
| [💡](https://github.com/reactive-streams/reactive-streams-jvm/tree/v1.0.3#3.13) | *The intent of this rule is to make sure that Subscribers can be properly garbage-collected after their subscription no longer being valid. Re-subscribing with the same Subscriber object is discouraged [see [2.12](https://github.com/reactive-streams/reactive-streams-jvm/tree/v1.0.3#2.12)], but this specification does not mandate that it is disallowed since that would mean having to store previously cancelled subscriptions indefinitely.* |
| 14                                                           | While the `Subscription` is not cancelled, calling `Subscription.cancel` MAY cause the `Publisher`, if stateful, to transition into the `shut-down` state if no other `Subscription` exists at this point [see [1.9](https://github.com/reactive-streams/reactive-streams-jvm/tree/v1.0.3#1.9)]. |
| [💡](https://github.com/reactive-streams/reactive-streams-jvm/tree/v1.0.3#3.14) | *The intent of this rule is to allow for Publishers to signal `onComplete` or `onError` following `onSubscribe` for new Subscribers in response to a cancellation signal from an existing Subscriber.* |
| 15                                                           | Calling `Subscription.cancel` MUST [return normally](https://github.com/reactive-streams/reactive-streams-jvm/tree/v1.0.3#term_return_normally). |
| [💡](https://github.com/reactive-streams/reactive-streams-jvm/tree/v1.0.3#3.15) | *The intent of this rule is to disallow implementations to throw exceptions in response to `cancel` being called.* |
| 16                                                           | Calling `Subscription.request` MUST [return normally](https://github.com/reactive-streams/reactive-streams-jvm/tree/v1.0.3#term_return_normally). |
| [💡](https://github.com/reactive-streams/reactive-streams-jvm/tree/v1.0.3#3.16) | *The intent of this rule is to disallow implementations to throw exceptions in response to `request` being called.* |
| 17                                                           | A `Subscription` MUST support an unbounded number of calls to `request` and MUST support a demand up to 2^63-1 (`java.lang.Long.MAX_VALUE`). A demand equal or greater than 2^63-1 (`java.lang.Long.MAX_VALUE`) MAY be considered by the `Publisher` as “effectively unbounded”. |
| [💡](https://github.com/reactive-streams/reactive-streams-jvm/tree/v1.0.3#3.17) | *The intent of this rule is to establish that the Subscriber can request an unbounded number of elements, in any increment above 0 [see [3.9](https://github.com/reactive-streams/reactive-streams-jvm/tree/v1.0.3#3.9)], in any number of invocations of `request`. As it is not feasibly reachable with current or foreseen hardware within a reasonable amount of time (1 element per nanosecond would take 292 years) to fulfill a demand of 2^63-1, it is allowed for a Publisher to stop tracking demand beyond this point.* |
