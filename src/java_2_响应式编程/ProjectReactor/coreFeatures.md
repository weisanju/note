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











