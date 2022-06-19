# `Flux`

> an Asynchronous Sequence of 0-N Items



The following image shows how a `Flux` transforms items:

![](\images\flux.svg)



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

![Mono](\images\mono.svg)

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





