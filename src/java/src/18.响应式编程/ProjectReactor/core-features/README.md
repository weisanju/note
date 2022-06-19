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

改变 最大的基数 的处理   的Operator   同样 切换到了 相应的类别 

例如 count 计数 返回 `Mono<Long>`



包括以下内容

* [FluxAndMono](FluxAndMono.md)
* [编程式创建序列](编程式创建序列.md)
* [错误处理](错误处理.md)
* [线程和调度](线程和调度.md)
* [ProcessorOrSinks](ProcessorOrSinks.md)













