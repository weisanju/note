## PropertySource

代表名称/值属性对的源的抽象基类。基础源对象可以是封装属性的任何类型T。

示例包括*java.util.Properties*对象，*java.util.Map*对象，*ServletContext*和*ServletConfig*对象 (用于访问init参数)。探索PropertySource类型层次结构以查看提供的实现。



PropertySource对象通常不是隔离使用，而是通过一个PropertySources对象使用，该对象聚合属性源，并与可以在PropertySources集合中执行基于优先级的搜索的PropertyResolver实现结合使用。





PropertySource标识不是基于封装属性的内容来确定的，而是仅基于PropertySource的名称来确定的。这对于在集合上下文中操纵PropertySource对象很有用。有关详细信息，请参见MutablePropertySources中的操作以及named(String) 和toString() 方法。



请注意，在使用 @ Configuration类时，@ PropertySource注释提供了一种方便且声明性的方式将属性源添加到封闭环境中。

