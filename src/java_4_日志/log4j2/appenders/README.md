# Appenders

1. Appenders 负责将 LogEvents 传送到目的地。

2. 每个 Appender 都必须实现 Appender 接口。

3. 大多数 Appender 将扩展 AbstractAppender，它添加 Lifecycle 和可过滤支持。

4. Lifecycle 允许组件在配置完成后完成初始化并在关闭期间执行清理。 
5. Filterable 允许组件附加过滤器，这些过滤器在事件处理期间进行过滤 。



Appender 通常只负责将事件数据写入目标目的地。

在大多数情况下，他们将事件格式化的责任委托给布局。

一些 appender 包装其他 appender，以便它们可以 **修改 LogEvent**，**处理 Appender 中的故障**，

根据高级过滤器标准将事件 路由 到下级 Appender，或提供类似功能但  不直接格式化事件

Appenders 总是有一个名称，以便它们可以从 Loggers 中引用。

在下表中，“类型”列对应于预期的 Java 类型。

对于非 JDK 类，除非另有说明，否则这些类通常应位于 Log4j Core 中。





# 预定义的Appenders

## AsyncAppender







