# Reactor Core Features

The Reactor project 主要构件 是 `reactor-core`  

专注于 reactive library 基于 java8

Reactor 引入 composable reactive types  实现 Publisher，

同时提供了丰富的 操作词汇：`Flux` and `Mono`

Flux 对象表示 响应式的 序列 从  0..N，而 *Mono* 对象表示 单值或空 结果

这种区别携带一些语义信息到类型，表示异步处理的粗糙基数（ the rough cardinality）



例如，HTTP 请求只生成一个响应，所以做 "计数" 操作没有多大意义

因此，将 HTTP 调用的结果 表示为`Mono<HttpResponse>`  比 `Flux<HttpResponse>` 更有意义

因为它只提供与零或一个item 上下文相关的Operator。

Operators that change the maximum cardinality of the processing also switch to the relevant type. For instance, the `count` operator exists in `Flux`, but it returns a `Mono<Long>`.

改变 最大的基数 的处理   的Operator   同样 切换到了 相应的类别 

例如 count 计数 返回 `Mono<Long>`

# `Flux`

> an Asynchronous Sequence of 0-N Items



The following image shows how a `Flux` transforms items:

![](../../images/flux.svg)

1. Flux发出的元素
2. 水平线：从左到右的  flux 时间流 
3. 垂直线表明：Flux成功完成
4. 虚点线和 box 表明 正在对 Flux执行 转换
5. 盒子中的 文字 展示了 转换规则
6. 下方是 Flux的转换后的结果 
7. 如果 处于某种原因 转换失败 ，垂直线 会被 x替代



`Flux<T>` 是 标准的 *Publiser<T>* 表明 异步 0~N 的  发出项的 异步序列，可选的被 完成 或 error 总结



As in the Reactive Streams spec， 这三种类型的信号转换为 对 下游 订阅者的 `onNext onComplete onError`的调用



在这种大范围的可能信号下，"Flux"是通用反应类型。

请注意：所有事件 甚至 终止 是 可选的

no `onNext` event but an `onComplete` event represents an *empty* finite sequence, 

but remove the `onComplete` and you have an *infinite* empty sequence (not particularly useful, except for tests around cancellation). 

Similarly, infinite sequences are not necessarily empty. For example, `Flux.interval(Duration)` produces a `Flux<Long>` that is infinite and emits regular ticks from a clock.

# `Mono`

> an Asynchronous 0-1 Result

![Mono](..\..\images\mono.svg)

A `Mono<T>` is a specialized `Publisher<T>` that emits at most one item *via* the `onNext` signal then terminates with an `onComplete` signal (successful `Mono`, with or without value), or only emits a single `onError` signal (failed `Mono`).



`Mono<T>` 是 `Publisher<T>` 的特化 ，通过 *onNext*  最多产生 一个 *item*  然后 使用 *onComplete* 终止，或者产生 *onEror* 信号



在调用 *onNext* 之后 *Mono* 的实现应该 立即调用 *onComplete*



`Mono.never()` 是一个 outlier。不发出任何信号



请注意，您可以使用"Mono"表示只有完成概念的无值异步过程（类似于"runnable"）。

To create one, you can use an empty `Mono<Void>`.



# Flux Mono使用

## `subscribe` Method `Examples`

**No-op订阅**

```java
Flux<Integer> ints = Flux.range(1, 3); 
ints.subscribe(); 
```

**处理元素订阅**

```java
Flux<Integer> ints = Flux.range(1, 3); 
ints.subscribe(i -> System.out.println(i)); 
```

**异常处理**

```java
Flux<Integer> ints = Flux.range(1, 4) 
      .map(i -> { 
        if (i <= 3) return i; 
        throw new RuntimeException("Got to 4"); 
      });
ints.subscribe(i -> System.out.println(i), 
      error -> System.err.println("Error: " + error));
```

**完成处理**

```java
Flux<Integer> ints = Flux.range(1, 4); 
ints.subscribe(i -> System.out.println(i),
    error -> System.err.println("Error " + error),
    () -> System.out.println("Done")); 
```

**Subscription消费**

这个变体 方法可以 使你 对 Subscription做一些事情（request(n)） 或者取消它， Otherwise the `Flux` hangs.

```java
Flux<Integer> ints = Flux.range(1, 4);
ints.subscribe(i -> System.out.println(i),
    error -> System.err.println("Error " + error),
    () -> System.out.println("Done"),
    sub -> sub.request(10)); 
```



```java
Flux<Integer> ints = Flux.range(1, 4);
ints.subscribe(i -> System.out.println(i),
    error -> System.err.println("Error " + error),
    () -> System.out.println("Done"),
    sub -> sub.request(10)); 
```

## Cancelling a `subscribe()` with Its `Disposable`

所有以上的 Lambada 变体的 *subscribe* 都有 Disposable 返回值

在这种情况下，the `Disposable`接口 表示 可以 通过 调用 `dispost()` 取消 订阅的事实

对于 Flux or Mono 取消是一个 信号，源应该停止生产元素 ，但是不能立即保证，某些源可能会产生 如此之快的元素  以至于 在收到取消指令前已完成

Disposable的一些工具方法 也是可用的。

`Disposables.swap()`  创建一个  Disposable 包装器 ，让你自动取消 并 替换具体的 *Disposable* 

在UI场景下，当用户单击按钮时，您需要取消请求，然后用新请求替换请求

Disposing the wrapper itself closes it. Doing so disposes the current concrete value and all future attempted replacements.

Another interesting utility is `Disposables.composite(…)`. This composite lets you collect several `Disposable` — for instance, multiple in-flight requests associated with a service call — and dispose all of them at once later on. Once the composite’s `dispose()` method has been called, any attempt to add another `Disposable` immediately disposes it.



## An Alternative to Lambdas: `BaseSubscriber`

还有一个额外的订阅方法,更通用，采取成熟的订阅者,而不是通过Lamba组合。为了帮助 编写 *Subscriber* 我们提供了 可扩展的类  `BaseSubscriber` 

`BaseSubscriber` 的实例 是一次性的，意味着：

* `BaseSubscriber`  在订阅第二个 *Publisher* 时 会 取消第一个
* 这是因为 多次使用 实例，会 违反  Reactive Streams rule ：一个 *Subscriber* 的 *onNext* 方法  不能被并行调用
* 因此 只有直接声明在 `Publisher#subscribe(Subscriber)` 的调用中 匿名实现 才行



```java
SampleSubscriber<Integer> ss = new SampleSubscriber<Integer>();
Flux<Integer> ints = Flux.range(1, 4);
ints.subscribe(ss);

package io.projectreactor.samples;

import org.reactivestreams.Subscription;

import reactor.core.publisher.BaseSubscriber;

public class SampleSubscriber<T> extends BaseSubscriber<T> {

	public void hookOnSubscribe(Subscription subscription) {
		System.out.println("Subscribed");
		request(1);
	}

	public void hookOnNext(T value) {
		System.out.println(value);
		request(1);
	}
}
```

该类提供可以覆盖的 *hook* ，以调整 subscriber的行为

默认情况下 会触发  无界请求 与 `subscribe()` 行为一致

但是，当您想要自定义请求数量时，扩展 BaseSubscriber 更有用

对于自定义 请求数量，最小限度是 实现： `hookOnSubscribe(Subscription subscription)`  `hookOnNext(T value)` 

在上述例子中：the `hookOnSubscribe` 方法 发出第一个 请求，然后：`hookOnNext` 放出额外的 *request* 

`BaseSubscriber` 同样 提供了   `requestUnbounded()` 方法 请求 无界模式 等价于 `request(Long.MAX_VALUE)`) 

和 `cancel方法`

它也有额外的  *hooks*   `hookOnComplete`, `hookOnError`, `hookOnCancel` `hookFinally` （当序列终止时，总是调用，终止类型作为  `SignalType`  参数传递）

## On Backpressure and Ways to Reshape Requests

Reactor 在 实现 背压时，消费者压力传回源头的 方式是 ：发送 *request* 请求给 上游

当前请求的 总数 有时被引用为 当前 *demand* 或者 pending request

Long.MAX_VALUE 的demand 表示 无限制的 请求（意味着：尽可能快的生产）



第一个请求来自 订阅时 最终的订阅者

所有最直接的 订阅方式 立即 触发了  无界的  `Long.MAX_VALUE` 的 *request*

* **subscribe()**，以及大部分 lambada 变体（除了：具有 `Consumer<Subscription>` 的变体） 
* `block()`, `blockFirst()` and `blockLast()`
* iterating over a `toIterable()` or `toStream()`



最简单的 自定义 原始 *request*的方式是： 使用 `BaseSubscriber` *subcribe* ，覆盖 `hookOnSubscribe`  方法

```java
Flux.range(1, 10)
    .doOnRequest(r -> System.out.println("request of " + r))
    .subscribe(new BaseSubscriber<Integer>() {

      @Override
      public void hookOnSubscribe(Subscription subscription) {
        request(1);
      }

      @Override
      public void hookOnNext(Integer integer) {
        System.out.println("Cancelling after having received " + integer);
        cancel();
      }
    });
```



当修改 request时，你必须 小心，产生足够的 *demand*  以进行推进，否则 你的 *Flux* 会被卡住

这也是为什么  `BaseSubscriber`  默认 在 `hookOnSubscribe` 中 请求 无限制的 *request*

当覆盖此钩子函数时，你必须 至少 调用一次 *request*

## **Operators that Change the Demand from Downstream**

### buffer

需求记住的是：上游链条 中 每个运营商 都可以 在 订阅级别上 重塑  表达的需求

一个教科书案例是： `buffer(N)`  操作：

如果它收到 `request(2)`  ，它被解释为 填满  **two full buffers**.

因此 缓冲区需要 n个元素填满，而需要2个缓冲区，所以 buffer操作将 元素个数 重塑为 `mxn` m是请求的缓冲区的个数，n是缓冲区的大小



### prefetch

你可能会注意到： `prefetch(int)`，这是修改下游请求的另一个 操作，

这通常是 处理内部序列，从每个 输入的元素中  提取 *Publisher* 像flatMap



*Prefetch* 是一种   微调 初始化请求的方式，在其内部完成

如果未指定，大多数 操作start with a demand of `32`

这些操作的通常 实现了 **replenishing optimization**: 

一旦操作员看到 75%的 prefetch 填充完毕，它将重新向上游请求 75%

这是一个 启发式优化，使得操作 能够 主动预测 即将到来的请求



### limit

最后，几个操作允许您直接调整请求： `limitRate` and `limitRequest`

`limitRate(N)`   将下游请求 拆分，以便以较小的 批次向下游传播



例如：`request(100)`，和 `limitRate(10)` 会导致：最多 10个请求被传播到上游

注意：`limitRate` 也实现了 `replenishing optimization` 该 操作也有一个变体，允许调整 请求数量，称作：*lowTide* : `limitRate(highTide, lowTide)`. 

如果 lowTide为 0，则在每批次中 使用 严格的  *highTide* 请求数 。而不是 根据补充策略，批次进一步返工

`limitRequest(N)`, 在另一个方面， 将下游请求限制为最大总需求。

它加起来 请求 高达 N

如果单个 请求 没有超过 N的 需求，则该请求  完全传播到 上游，在源发出该数量之后，

 `limitRequest` 认为序列已完成，向下游发 *onComplete* 信号，并取消源



# Programmatically creating a sequence

在此章节，我们将 介绍，Flux、Mono的编程式创建 ，定义关联事件（`onNext`, `onError`, and `onComplete`）

所有这些方法都 体现了一个事实： 它们暴露了 API，去触发事件，我们称之为 *sink*

实际上有几个 *sink* 的变体，

## Synchronous `generate`

最简单的编程式 创建 *Flux*的 方式是 ：通过 *generate* 方法

这是用于同步 的 一个个的  产生，这意味着 sink 是一个 SynchronousSink 他的 next 方法 在每次回调中只能被调用 一次，你可以 调用 `error(Throwable)`  或者 `complete`  这是可选的



最有用的变体可能是 让你保持 一个状态，你可以 在你的 sink 使用中 引用这个 状态，以决定 下一步 产生什么

generator 函数 形式为： `BiFunction<S, SynchronousSink<T>, S>` s是状态对象，必须提供初始化状态，`Supplier<S>` ，你的 generator 函数 现在 返回每轮的 新状态



```java
Flux<String> flux = Flux.generate(
    () -> 0, 
    (state, sink) -> {
      sink.next("3 x " + state + " = " + 3*state); 
      if (state == 10) sink.complete(); 
      return state + 1; 
    });
```

```java
Flux<String> flux = Flux.generate(
    AtomicLong::new, 
    (state, sink) -> {
      long i = state.getAndIncrement(); 
      sink.next("3 x " + i + " = " + 3*i);
      if (i == 10) sink.complete();
      return state; 
    });
```



如果状态包含数据库连接或其他资源,需要在流程结束时处理,the *consumer* lamba 可以关闭连接，或者其他 在进程结束时，需要处理的任务



## Asynchronous and Multi-threaded:create

> 异步跟多线程 create



*create* 是更高级的 编程创建 *Flux* 的形式， 适合 每轮产生多个元素，甚至来自多线程

它暴露  *FluxSink* 对象，这个对象有 next、error、complete 方法 

与 *generate* 相反，它没有基于状态的变体，另一方面，它可以触发 回调中的多线程事件

*create* 对于 桥接 已有API到 响应式 世界 很有用，例如基于监听器的 异步API

`create` 不会串行化 你的 代码，也不会使之异步,但他能用于异步API

如果你 在 *create* lamba 阻塞，你可能会面临 死锁或类似的副作用

甚至使用 *subscribeOn* 

对于长阻塞的 *create* *lamba* 方法（例如 无限循环调用 sink.next(t) ）会锁住 *pipeline* 

请求永远不会执行 ，因为循环饿死了他们应该运行的同一个线程。

 Use the `subscribeOn(Scheduler, false)` variant: `requestOnSeparateThread = false` will use the `Scheduler` thread for the `create` and still let data flow by performing `request` in the original thread.



你有一个基于 监听器的 API，它按块处理数据，并具有两个事件在 `MyEventListener`  接口：

1. a chunk of data is ready and 
2. the processing is complete 

```java
interface MyEventListener<T> {
    void onDataChunk(List<T> chunk);
    void processComplete();
}
```

**使用create桥接**

```java
Flux<String> bridge = Flux.create(sink -> {
    myEventProcessor.register( 
      new MyEventListener<String>() { 

        public void onDataChunk(List<String> chunk) {
          for(String s : chunk) {
            sink.next(s); 
          }
        }

        public void processComplete() {
            sink.complete(); 
        }
    });
});
```

此外 由于 `create`  可以 桥接 异步API 并管理 背压

您可以通过 指示 `OverflowStrategy` 来改进 如何 进行背压

- `IGNORE` 完全忽略下游背压请求，当 排队者占满 下游 ，可能会产生`IllegalStateException`  
- `ERROR` 当下游跟不上时， 发出 `IllegalStateException` 的信号
- `DROP` 如果下游未准备好接收来信，则丢弃传入信号。.
- `LATEST 让下游只从上游获得最新信号.
- `BUFFER` （默认值）缓冲所有信号，如果下游跟不上 (这确实有无限制的缓冲， 并可能导致 `OutOfMemoryError`).

`Mono` also has a `create` generator. The `MonoSink` of Mono’s create doesn’t allow several emissions. It will drop all signals after the first one.



## Asynchronous but single-threaded: `push`

`push` 是 *generate* 跟 *create*的 中间地带，适合于 来自单个 生产者的 事件处理

与 `create` 类似 ，它同样可以 异步、使用  overflow strategies  管理背压，然后，只能来自一个生产线程，可能会 同时调用 `next` `complete` `error` 

```java
Flux<String> bridge = Flux.push(sink -> {
    myEventProcessor.register(
      new SingleThreadEventListener<String>() { 

        public void onDataChunk(List<String> chunk) {
          for(String s : chunk) {
            sink.next(s); 
          }
        }

        public void processComplete() {
            sink.complete(); 
        }

        public void processError(Throwable e) {
            sink.error(e); 
        }
    });
});
```

**A hybrid push/pull model**

Most Reactor operators, like `create`, follow a hybrid **push/pull** model. 

大多数 Reactor 操作，比如 *create* 遵循 混合的 `push/pull` 模型，

意思是：尽管大多是处理是异步的（建议采用 push方法），有一个小的 *pull*  组件：*request*

*consumer* 从源 pull 数据，直到第一次请求，它才会产生任何东西

*source* 推送数据 给 *consumer* 在其请求的数量范围内

Note that `push()` and `create()` both allow to set up an `onRequest` consumer in order to manage the request amount and to ensure that data is pushed through the sink only when there is pending request.

注意：`push() creat()` 都 允许 设置 onRequest consumer 为了 管理请求数量，以确保 存在 pending 的request 时 数据被推送

```java
Flux<String> bridge = Flux.create(sink -> {
    myMessageProcessor.register(
      new MyMessageListener<String>() {

        public void onMessage(List<String> messages) {
            //The remaining messages that arrive asynchronously later are also delivered.
          for(String s : messages) {
            sink.next(s); 
          }
        }
    });
    sink.onRequest(n -> {
        //	Poll for messages when requests are made.
        List<String> messages = myMessageProcessor.getHistory(n); 
        //If messages are available immediately, push them to the sink.
        for(String s : messages) {
           sink.next(s); 
        }
    });
});
```



**Cleaning up after `push()` or `create()`**

Two callbacks, `onDispose` and `onCancel`, perform any cleanup on cancellation or termination. 

`onCancel` can be used to perform any action specific to cancellation prior to cleanup with `onDispose`.

两个回调 *onDispose* *onCancel* 在 终止或 取消时 执行清理动作，

当 Flux completes,、错误或取消时，OnDispose 可用于执行清理

"onCancel" 可用于在   `onDispose` 之前执行任何特定于 cancellation   的"操作"。

```java
Flux<String> bridge = Flux.create(sink -> {
    sink.onRequest(n -> channel.poll(n))
        //onCancel is invoked first, for cancel signal only.
        .onCancel(() -> channel.cancel()) 
        //onDispose is invoked for complete, error, or cancel signals.
        .onDispose(() -> channel.close())  
    });
```

## Handle

*handle* 方法 有一点不同，实例 方法，意思是他被 链接在 现有源上（普通操作也是）在 `Mono Flux` 都有

与 *generate* 相近，使用 `SynchronousSink`  只允许 一个个的产生,*handle* 可用于从每个 元素 中生成 任意值，

可以跳过 元素，它可以看作是 map 和 filter 的 组合

```java
Flux<R> handle(BiConsumer<T, SynchronousSink<R>>);
```



The reactive streams specification 不允许 null值，你想要执行 map，但是想要使用预存在的 方法作为 map function 这个方法有时返回null,例如 以下方法 可以 安全的 应用于 整数源

```java
public String alphabet(int letterNumber) {
	if (letterNumber < 1 || letterNumber > 26) {
		return null;
	}
	int letterIndexAscii = 'A' + letterNumber - 1;
	return "" + (char) letterIndexAscii;
}
```

这时我们可以 *handle* 处理null值

```java
Flux<String> alphabet = Flux.just(-1, 30, 13, 9, 20)
    .handle((i, sink) -> {
        String letter = alphabet(i); 
        if (letter != null) 
            sink.next(letter); 
    });

alphabet.subscribe(System.out::println);
```



# Threading and Schedulers

Reactor 并不强制使用 并发模型，但也提供 并发帮助

获取 Flux Mono 并不意味着 在专用 线程中 运行

相反，大多数操作 运行在前面 操作的线程中，如果没有特别指定，最顶层的 操作：source 自身 运行在 *subscribe*的 调用线程上。

以下示例显示 在 Mono中运行在新线程上

```java
public static void main(String[] args) throws InterruptedException {
    //主线程 组装
  final Mono<String> mono = Mono.just("hello "); 
                        //其他线程订阅
  Thread t = new Thread(() -> mono
                        //map onNext 回调 实际上 都在 其他线程执行
      .map(msg -> msg + "thread ")
      .subscribe(v -> 
          System.out.println(v + Thread.currentThread().getName()) 
      )
  )
  t.start();
  t.join();
}
```

在Reactor中，执行模型 执行线程 取决于使用的 `Scheduler` 

A [`Scheduler`](https://projectreactor.io/docs/core/release/api/reactor/core/scheduler/Scheduler.html)  承担 调度 职责，类似于 `ExecutorService`

但是有专门的抽象，让它做得更多。

特别是 作为 时钟 并使更广泛的实现：虚拟时间测试， trampolining ，立即调度 等



The [`Schedulers`](https://projectreactor.io/docs/core/release/api/reactor/core/scheduler/Schedulers.html) class 的静态方法 可以访问 以下 执行上下文

- No execution context (`Schedulers.immediate()`): 在 处理时，已提交的 *Runnable* 会 直接执行，有效的运行在 当前线程（可以视为 空对象，或无操作的 *Scheduler*）

- A single, reusable thread (`Schedulers.single()`). 对于所有调用者使用一个线程你想每一个调用一个线程 则 使用  `Schedulers.newSingle()`

- An unbounded elastic thread pool (`Schedulers.elastic()`). 引入 `Schedulers.boundedElastic()` 之后 不在首选, 因为它有隐藏背压问题并导致太多线程的倾向

- A bounded elastic thread pool (`Schedulers.boundedElastic()`). Like its predecessor `elastic()`, 

  它根据需要创建新的 工作线程池，重用 空闲线程，闲置线程闲置超过60s，会被回收，创建的线程有上限（默认是 CPU 核数 x10），线程池达到 上限后 ，提交多达 100 000 的任务 会被入队列等待，直到线程池可用（如果延迟调用，则延迟从 线程可用时开始计算）

  This is a better choice for I/O blocking work.

  对于 阻塞式 I/O 工作来说，这是一个更好的选择。

   `Schedulers.boundedElastic()` 是一种方便的方式，在自己的线程中阻塞，一遍不会绑定其他资源

   See [How Do I Wrap a Synchronous, Blocking Call?](https://projectreactor.io/docs/core/release/reference/#faq.wrap-blocking), 但不会给系统产生太多线程的压力

* A fixed pool of workers that is tuned for parallel work (`Schedulers.parallel()`). 固定工作线程池，线程的个数和CPU的 核心数一样多

另外，通过 `Schedulers.fromExecutorService(ExecutorService)`  从  `ExecutorService`  创建 Scheduler

也可以 从 *Executor* 创建，这并不推荐

也可以 通过 newXXX 方法   创建 不同调度类型的  Scheduler 实例

例如： `Schedulers.newParallel(yourScheduleName)`  创建一个 并行的 命名的  Scheduler 



`boundedElastic`  用来帮助 旧的阻塞式代码（如果没法避免）

 `single` and `parallel` are not. 

As a consequence, 

因此，Reactor blocing APis的使用 （`block()`, `blockFirst()`, `blockLast()` as well as iterating over `toIterable()` or `toStream()`） 

the use of Reactor blocking APIs (`block()`, `blockFirst()`, `blockLast()` (as well as iterating over `toIterable()` or `toStream()`) inside the default single and parallel schedulers) results in an `IllegalStateException` being thrown.

自定义 *Schedulers* 同样能 被标记为 "non blocking only" ，只要 创建的 Thread 实现了  *NonBlocking* 标记接口



一些操作 默认的 使用 来自 `Schedulers`  的指定 的*Scheduler* （这通常会提供不同的 选择 ）

例如 调用   `Flux.interval(Duration.ofMillis(300))`   工厂方法 产生 一个 `Flux<Long>` 每300ms 滴答一下。默认的 使用 `Schedulers.parallel()`  以下代码 修改 *Scheduler* 成 `Schedulers.single()`

```java
Flux.interval(Duration.ofMillis(300), Schedulers.newSingle("test"))`
```

Reactor 提供 两种办法 在reactive chain种  切换 执行上下文（Scheduler） ：`publishOn` and `subscribeOn`

都有一个 Scheduler参数

但是  `publishOn` 在链条中的位置很重要，而`subscribeOn` 不重要

To understand that difference, you first have to remember that [nothing happens until you subscribe](https://projectreactor.io/docs/core/release/reference/#reactive.subscribe).

在Reactor中，当你 链接操作时，你可以 将 尽可能多的 `Flux` and `Mono`  实现 包裹 在 彼此的内部 

订阅后，`Subscriber` objects 对象链 已经创建好，向后 到 第一个 *Publiser*

对用户是透明的，你能看见的是 外层的 Flux or Mono 和 *Subscription* 但是 这些中间 特定操作的 Subscribers 才是实际工作发生的地方，有了这些认知后，我们可以更仔细的了解 *publishOn* *subscription* 操作

## The `publishOn` Method



`publishOn`  同其他 操作 一样 以同样的方式 适用

它接收来自上游的信号，并 执行回调时 重播它们( 回调的执行是在 相关联的 *Scheduler* )

因此。它影响 后续 操作的  执行上下文（知道 另一个 publishOn 链入进来）

- Changes the execution context to one `Thread` picked by the `Scheduler`
- as per the specification, `onNext` calls happen in sequence, so this uses up a single thread
- unless they work on a specific `Scheduler`, operators after `publishOn` continue execution on that same thread

```java
//创建 新的 Scheduler
Scheduler s = Schedulers.newParallel("parallel-scheduler", 4); 


final Flux<String> flux = Flux
    .range(1, 2)
    .map(i -> 10 + i)   //The first map runs on the anonymous thread
    //切换到 Scheduler s
    .publishOn(s)  
    //继承上一个 Scheduler
    .map(i -> "value " + i);  

//订阅发生在该线程， print发生于 最近的执行上下文
new Thread(() -> flux.subscribe(System.out::println));  
```

## The `subscribeOn` Method

当构建 后向链条时，`subscribeOn`  用于订阅过程 

因此 无论您将 `subscribeOn`  放在链条的哪里，它总是 因是影响 源的产生的执行上下文

但是，这不会影响后续 `publishOn`  的行为，它们仍然会切换执行上下文的链条部分

- 改变 操作的整条链条的订阅的线程
- 从调度器 中选择一个线程 

Only the earliest `subscribeOn` call in the chain is actually taken into account.

```java
//创建 Scheduler
Scheduler s = Schedulers.newParallel("parallel-scheduler", 4); 

final Flux<String> flux = Flux
    .range(1, 2)
    //在 s中运行
    .map(i -> 10 + i)  
    //切换整个链条的上下文到 s中
    .subscribeOn(s)  
    .map(i -> "value " + i);  

new Thread(() -> flux.subscribe(System.out::println));  
```



# Handling Errors

>  要快速查看可用于错误处理的操作  see [the relevant operator decision tree](https://projectreactor.io/docs/core/release/reference/#which.errors).

In Reactive Streams,错误是 终止事件，一旦错误发生，它会终止序列，传播 到 操作链的最后一个、the `Subscriber` 、以及其 *onError* 方法。这类错误 应该 在应用级别处理

如果没有定义  onError 抛出一个 **UnsupportedOperationException**  可以通过  `Exceptions.isErrorCallbackNotImplemented` 检测跟 分类

作为错误处理 操作，Reactor 还提供了 在链条中 处理错误的 方法

```java
Flux.just(1, 2, 0)
    .map(i -> "100 / " + i + " = " + (100 / i)) //this triggers an error with 0
    .onErrorReturn("Divided by zero :("); // error handling example
```



错误处理操作 是一个终止序列，即使使用了 error-handling operator ，并不会让 序列继续，相反它将 onError信号 转换成 了 一个新序列的开始 ，换句话说：它替代了 原始序列

现在，我们可以逐一考虑各种错误处理方法。当相关时，我们与命令式编程的 try pattenr 进行并行。

## Error Handling Operators

**try catch的错误处理模型**

- Catch and return a static default value: catch 并返回静态默认值
- Catch and execute an alternative path with a fallback method：  catch，使用fallback 方法 执行替代路径
- Catch and dynamically compute a fallback value. catch 动态计算 fallback value
- Catch, wrap to a `BusinessException`, and re-throw.  catch,重新抛出
- Catch, log an error-specific message, and re-throw. catch 记录日志 重新抛出
- Use the `finally` block to clean up resources or a Java 7 “try-with-resource” construct. finally 执行资源清理

上述所有这些情况都在 Reactor中都有等价API，以 error-handling 操作形式

**try catch与 Reactor error-handling的对照**

订阅时，the `onError`  callback 回调 类似于 catch块，当异常抛出 会直接调到 catch块执行

```java
Flux<String> s = Flux.range(1, 10)
    .map(v -> doSomethingDangerous(v)) 
    .map(v -> doSecondTransform(v)); 
s.subscribe(value -> System.out.println("RECEIVED " + value), 
            error -> System.err.println("CAUGHT " + error) 
);
```

### Static Fallback Value

**trycatch模型**

```java
try {
  return doSomethingDangerous(10);
}
catch (Throwable error) {
  return "RECOVERED";
}
```

**Reactor模型**

```java
Flux.just(10)
    .map(this::doSomethingDangerous)
    .onErrorReturn("RECOVERED");
```

**Reactor Predict模型**

```java
//Recover only if the message of the exception is "boom10"

Flux.just(10)
    .map(this::doSomethingDangerous)
    .onErrorReturn(e -> e.getMessage().equals("boom10"), "recovered10"); 
```

### Fallback Method

**trycatch模型**

```java
String v1;
try {
  v1 = callExternalService("key1");
}
catch (Throwable error) {
  v1 = getFromCache("key1");
}

String v2;
try {
  v2 = callExternalService("key2");
}
catch (Throwable error) {
  v2 = getFromCache("key2");
}
```

**Reactor模型**

```java
Flux.just("key1", "key2")
    .flatMap(k -> callExternalService(k) 
        .onErrorResume(e -> getFromCache(k)) 
    );
```

**Reactor Predict模型**

```java
Flux.just("timeout1", "unknown", "key2")
    .flatMap(k -> callExternalService(k)
        .onErrorResume(error -> { 
            if (error instanceof TimeoutException) 
                return getFromCache(k);
            else if (error instanceof UnknownKeyException)  
                return registerNewEntry(k, "DEFAULT");
            else
                return Flux.error(error); 
        })
    );
```

### Dynamic Fallback Value

**trycatch模型**

```java
try {
  Value v = erroringMethod();
  return MyWrapper.fromValue(v);
}
catch (Throwable error) {
  return MyWrapper.fromError(error);
}
```

**Reactor模型**

```java
erroringFlux.onErrorResume(error -> Mono.just( 
        MyWrapper.fromError(error) 
));
```

### Catch and Rethrow

**trycatch模型**

```java
try {
  return callExternalService(k);
}
catch (Throwable error) {
  throw new BusinessException("oops, SLA exceeded", error);
}
```

**Reactor模型**

```java
Flux.just("timeout1")
    .flatMap(k -> callExternalService(k))
    .onErrorResume(original -> Flux.error(
            new BusinessException("oops, SLA exceeded", original))
    );

Flux.just("timeout1")
    .flatMap(k -> callExternalService(k))
    .onErrorMap(original -> new BusinessException("oops, SLA exceeded", original));
```

### Log or React on the Side

**trycatch模型**

```java
try {
  return callExternalService(k);
}
catch (RuntimeException error) {
  //make a record of the error
  log("uh oh, falling back, service failed for key " + k);
  throw error;
}
```

**Reactor模型**

```java
LongAdder failureStat = new LongAdder();
Flux<String> flux =
Flux.just("unknown")
    .flatMap(k -> callExternalService(k) 
        .doOnError(e -> {
            failureStat.increment();
            log("uh oh, falling back, service failed for key " + k); 
        })
        
    );
```

### Using Resources and the Finally Block

**trycatch模型**

```java
Stats stats = new Stats();
stats.startTimer();
try {
  doSomethingDangerous();
}
finally {
  stats.stopTimerAndRecordTiming();
}
try (SomeAutoCloseable disposableInstance = new SomeAutoCloseable()) {
  return disposableInstance.toString();
}

```

**Reactor模型**

Both have their Reactor equivalents: `doFinally` and `using`.

**doFinally()**

```java
Stats stats = new Stats();
LongAdder statsCancel = new LongAdder();

Flux<String> flux =
Flux.just("foo", "bar")
    .doOnSubscribe(s -> stats.startTimer())
    .doFinally(type -> { 
        stats.stopTimerAndRecordTiming();
        if (type == SignalType.CANCEL) 
          statsCancel.increment();
    })
    .take(1); 
```

**Reactive try-with-resource**

```java
Flux<String> flux =
Flux.using(
    //产生资源
        () -> disposableInstance, 
    //处理资源
        disposable -> Flux.just(disposable.toString()), 
    // 清理资源
        Disposable::dispose 
);
```

### 证明 *onError* 信号 导致终止

```java
Flux<String> flux =
Flux.interval(Duration.ofMillis(250))
    .map(input -> {
        if (input < 3) return "tick " + input;
        throw new RuntimeException("boom");
    })
    .onErrorReturn("Uh oh");

flux.subscribe(System.out::println);
Thread.sleep(2100); 
```

interval 在默认在 定时器上 执行 ，如果我们想在主类中运行该示例，

我们需要在此处添加 `Sleep`  调用，以便应用程序不会立即退出，而不产生任何元素

### Retrying

错误处理的另一种方式 `retry` 可以 重试 一个 正 产生错误的 序列

原理是：重新订阅 上游 *Flux*，原始的仍然终止了

```java
Flux.interval(Duration.ofMillis(250))
    .map(input -> {
        if (input < 3) return "tick " + input;
        throw new RuntimeException("boom");
    })
    .retry(1)
    .elapsed() //将每个值 与 自上一个值发出依赖的持续时间 关联在一起
    .subscribe(System.out::println, System.err::println); 

Thread.sleep(2100); 
```

```java
259,tick 0
249,tick 1
251,tick 2
506,tick 0 
248,tick 1
253,tick 2
java.lang.RuntimeException: boom
```

新的 interval 开始了，tick从0开始，在恢复的时候，需要额外等 250ms

`retry(1)`  只是 仅仅 重订阅上游  *interval* ,第二轮仍会 发生异常，再次发生异常会 将错误 传播给 下游



### RetryWhen

**Retry.from**

接收 `Flux<Retry.RetrySignal>` 返回 `Publisher<?>`

重试周期如下：

1. 当 error 发生时，会给 `Flux<RetrySignal>` 发送 信号，可以纵览所有 重试，RetrySignal 提供对 错误的访问和 相关的辕信息
2. 如果 `Flux<RetrySignal>`  产生一个 值，则重试发生
3. 如果 `Flux<RetrySignal>`   complete 完成了，则错误会被吞并，重试周期会 停止，结果序列 也会完成
4. 如果 `Flux<RetrySignal>`    产生错误，重试周期 停止，使得序列 产生错误

使用 retryWhen 模拟 retry(3)

```java
//这不断产生错误，要求重试尝试。
Flux<String> flux = Flux
    .<String>error(new IllegalArgumentException()) 
//doOnError before the retry lets us log and see all failures.
    .doOnError(System.out::println) 
//The Retry is adapted from a very simple Function lambda
    .retryWhen(Retry.from(companion -> 
//我们认为前三个错误是可重复尝试的（takle(3)，然后放弃
        companion.take(3))); 
```









# Processors and Sinks



Processors是 一种特殊的 Publisher ，同样也是 Subscriber

它们最初是 在Reactive Streams 的不同实现中 作为 中间步骤的 可能 表示

在Reactor中，这些步骤 相当由 Publiser 表示

*Processor* 的常见误区是 直接调用 *Subscriber* 上 暴露的 `onNext onComplete onError` 方法

这样直接调用需要小心：

特别是 关于 Reactive Streams specification 考虑到 调用之间的外部同步。Processors 可能会稍有用，除非遇到 基于 Reactive Streams的API，该API需要 Subscriber传入，而不是暴露发 *Publisher*

Sinks是更好的选择，在 Reactor中，sink是一个 安全的手动的 触发 信号的类，它既可以于 Subscription 关联（从操作内部） 也可以完全独立

Since `3.4.0`, sinks 成为 一等公民，`Processor` 淘汰了

- 抽象的  或 具体的 `FluxProcessor` and `MonoProcessor`  过时了，预计在3.5.0 中删除
- sink不是由 操作 产生的，而是通过 *Sinks* 的工厂方法 构建的



## Safely Produce from Multiple Threads by Using `Sinks.One` and `Sinks.Many`

reactor-core 暴露出来的  *Sinks*  确保 在多线程使用，不会违反规范 或者 未定义的行为，从下游的角度看

当使用 `tryEmit*`  API 时，并行调用 会 fail fast 

当使用 `emit*` API 时，提供的 `EmissionFailureHandler` 可以允许 在竞争中 重试（例如：忙碌轮询），否则 sink会以失败终止

这对于 `Processor.onNext` 是一个提升，它 必须外部同步，否则就会 导致未定义的行为，从下游Subscribers的视角来看

*Sinks* 构建起 为主要支持的 producer types 提供引导API

你将会发现 Flux中 的一些行为 例如：`onBackpressureBuffer`

```java
Sinks.Many<Integer> replaySink = Sinks.many().replay().all();
```



多个生产者线程可以通过以下工作同时 在 sink 上生成数据：

```
//thread1
sink.emitNext(1, FAIL_FAST);

//thread2, later
sink.emitNext(2, FAIL_FAST);

//thread3, concurrently with thread 2
EmitResult result = sink.tryEmitNext(3); //would return FAIL_NON_SERIALIZED
```



`Sinks.Many` 可以作为 *Flux*

```java
Flux<Integer> fluxView = replaySink.asFlux();
fluxView
	.takeWhile(i -> i < 10)
	.log()
	.blockLast();
```

Similarly, the `Sinks.Empty` and `Sinks.One` flavors can be viewed as a `Mono` with the `asMono()` method.



The `Sinks` categories are:

1. `many().multicast()`: 只传输 新推的 数据给其 subscribers，遵循背压，（newly pushed as in "after the subscriber’s subscription"）
2. `many().unicast()`: 与上述相同, with the twist that data pushed before the first subscriber registers is buffered.
3. `many().replay()`: a sink that will replay a specified history size of pushed data to new subscribers then continue pushing new data live.
4. `one()`: a sink that will play a single element to its subscribers
5. `empty()`: a sink that will play a terminal signal only to its subscribers (error or complete), but can still be viewed as a `Mono<T>` (notice the generic type `<T>`).

## Overview of Available Sinks

### Sinks.many().unicast().onBackpressureBuffer(args?)

Sinks.many().unicast() 使用 内部缓冲区 处理 背压，作为权衡：只能有一个 *Subscriber*

**Sinks.many().unicast().onBackpressureBuffer()** 是基本的 sink创建方法

还有其他变体微调

例如，默认情况下，它是无限制的：

如果您在其 Subscriber 尚未请求数据时，推送任意数量的 数据时，它会缓冲数据数据

你可以为内部 缓冲区 提供自定义队列的 实现来改变此情况 `Sinks.many().unicast().onBackpressureBuffer(Queue)` 

如果队列是有界的，缓冲区已满，未收到来自下游足够的请求，sink可能会 拒绝 数据的推送

### Sinks.many().multicast().onBackpressureBuffer(args?)

Sinks.many().multicast() 可以 触发 多个 subscribers ，同时处理 为每一个 subscriber 处理 背压

subscribers 订阅后，只接收 通过 sink的 push 信号

创建 sink的 基本方法  `Sinks.many().multicast().onBackpressureBuffer()`. 

默认情况下，所有 subscribers  取消了（这基本意味着 它们 都有未订阅）

它会 清除其 内部缓冲区，并停止接受新的 subscribers

你可以在 `multicast`  静态工厂方法中 使用`autoCancel`  参数微调，位于 `Sinks.many().multicast()`



### Sinks.many().multicast().directAllOrNothing()

处理背压很简单：如果有一个 subscribers  很慢例如 0 demand, 则 onNext 方法 会被 所有subscribers drop掉

但是，慢subscribers  没有终止， 一旦 慢 subscribers  再次开始请求，所有subscribers  都将恢复 接收 从那里推送的数据

一旦 Sinks.many 终止了 （通常是 调用`emitError(Throwable)` or `emitComplete()` ）

它仍允许 更多的 subscribers subscribe  但是 会立即重播 终止信号给他们

### Sinks.many().multicast().directBestEffort()

* 最大努力的处理背压：只 drop掉 慢 subscriber  的 *onNext*

* 如果慢 subscribers  恢复速度，会重新 push 元素
*  Sinks.many 终止，允许新的 subscribers 加入，并重放 终止信号

### Sinks.many().replay()

缓存 产生的元素，并重播给 后续的 订阅者

它有以下配置

- Caching a limited history (`Sinks.many().replay().limit(int)`) or an unbounded history (`Sinks.many().replay().all()`). 基于个数的缓存
- Caching a time-based replay window (`Sinks.many().replay().limit(Duration)`).  基于时间的缓存
- Caching a combination of history size and time window (`Sinks.many().replay().limit(int, Duration)`). 综合

还有其他的重载方法 微调

例如 `latest()` and `latestOrDefault(T)`

### Sinks.unsafe().many()

与 `Sinks.Many`  相比 没有额外的 producer 线程安全，也就意味着更小的开销

根据 the Reactive Streams specification.可以确保  `onNext`, `onComplete` and `onError`  在外部同步

### Sinks.one()

Sinks的使用 是 Mono的 视图 ，通过 `asMono()` 

and has slightly different `emit` methods to better convey this Mono-like semantics:

并有稍微不同的 `emit` 方法， 以更好地传达这种单一(Mono-like)的语义：

- `emitValue(T value)` generates an `onNext(value)` signal and - in most implementations - will also trigger an implicit `onComplete()`
- `emitEmpty()` generates an isolated `onComplete()` signal, intended as generating the equivalent of an empty `Mono`
- `emitError(Throwable t)` generates an `onError(t)` signal

`Sinks.one()` accepts *one* call of any of these methods, effectively generating a `Mono` that either completed with a value, completed empty or failed.

### Sinks.empty()

This flavor of `Sinks` is like `Sinks.One<T>`, except it doesn’t offer the `emitValue` method.

As a result, it can only generates a `Mono` that completes empty or fails.

The sink is still typed with a generic `<T>` despite being unable to trigger an `onNext`, because it allows easy composition and inclusion in chains of operators that require a specific type.



