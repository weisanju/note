# Layouts

1. Appender 使用 Layout 将 LogEvent 格式化为满足消费日志事件需求的形式

2. 在 Log4j 2 Layouts 中返回一个字节数组。这允许 Layout 的结果在更多类型的 Appender 中有用。但是，这意味着您需要 在大多数布局中配置 Charset ，以确保字节数组包含正确的值。

3. 使用字符集的布局的根类是 org.apache.logging.log4j.core.layout.AbstractStringLayout，其中默认值为 UTF-8。每个扩展 AbstractStringLayout 的布局都可以提供自己的默认值。请参阅下面的每个布局。





Log4j 2.4.1 中为 ISO-8859-1 和 US-ASCII 字符集添加了一个自定义字符编码器

以将 Java 8 内置的一些性能改进带到 Log4j 以便在 Java 7 上使用

对于仅记录日志的应用程序ISO-8859-1 字符，指定此字符集将显着提高性能。











