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
* Sinks.many 终止，允许新的 subscribers 加入，并重放 终止信号

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



