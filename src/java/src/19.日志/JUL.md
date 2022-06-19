{% raw %}

# JUL

## 使用

JUL 全称 java.util.logging.Logger，JDK 自带的日志系统，从 JDK1.4 就有了。因为 log4j 的存在，这个 logger 一直沉默着，其实在一些测试性的代码中，jdk自带的 logger 比 log4j 更方便。JUL是自带具体实现的，与 log4j、logback 等类似，而不是像 JCL、slf4j 那样的日志接口封装。

```
import java.util.logging.Level;
import java.util.logging.Logger;

private static final Logger LOGGER = Logger.getLogger(MyClass.class.getName());
```

## 日志级别

- 日志级别由高到低依次为：
    - **SEVERE**（严重）
    - **WARNING**（警告）
    - **INFO**（信息）
    - **CONFIG**（配置）
    - **FINE**（详细）
    - **FINER**（较详细）
    - **FINEST**（非常详细）

另外还有两个全局开关：OFF「关闭日志记录」和ALL「启用所有消息日志记录」。

## logging.properties文件

**默认日志级别**

默认日志级别可以通过.level= ALL来控制，也可以基于层次命名空间来控制，按照Logger名字进行前缀匹配，匹配度最高的优先采用，**日志级别只认大写；**

JUL通过handler来完成实际的日志输出，可以通过配置文件指定一个或者多个hanlder，多个handler之间使用逗号分隔；handler上也有一个日志级别，作为该handler可以接收的日志最低级别，低于该级别的日志，将不进行实际的输出；

handler上可以绑定日志格式化器，比如java.util.logging.ConsoleHandler就是使用的String.format来支持的；



## 关于构造函数中defaultBundle的解释

```java
// 默认资源包位置
private static final String defaultBundle = "sun.util.logging.resources.logging";
// 用于本地化级别名称的资源包名称
private final String resourceBundleName;
```

resourceBundleName是用来指定外部资源包的，如果不指定，会默认用defaultBundle指定的资源包，资源包是干嘛的呢，我在rt.jar包下找到了这个资源。

```java
package sun.util.logging.resources;

import java.util.ListResourceBundle;

public final class logging extends ListResourceBundle {
    public logging() {
    }

    protected final Object[][] getContents() {
        return new Object[][]{{"ALL", "All"}, {"CONFIG", "Config"}, {"FINE", "Fine"}, {"FINER", "Finer"}, {"FINEST", "Finest"}, {"INFO", "Info"}, {"OFF", "Off"}, {"SEVERE", "Severe"}, {"WARNING", "Warning"}};
    }
}

```

原来就是控制台输出日志时，**定义本地化后的级别名称**。或许你会有疑问，明明在控制台上看到的是警告、信息等中文的Level，这实际上是SimpleFormatter进行的处理。

```java
public synchronized String format(LogRecord record) {
	// 省略方法前半部分的代码
	return String.format(format,
                             dat,
                             source,
                             record.getLoggerName(),
                             record.getLevel().getLocalizedLevelName(),
                             message,
                             throwable);
}

```



## LogManager对象的初始化

* 从类加载日志配置文件

    从图中代码可以看出 *java.util.logging.config.class* 中实例化类，用户并从构造函数中加载 配置类，通过调用   *readConfiguration(InputStream)*

* 从指定的 系统环境变量中加载，*java.util.logging.config.file*

    注意这里的 路径是绝对路径，默认路径是 `${java.home}\lib\logging.properties`

```java
    public void readConfiguration() throws IOException, SecurityException {
        checkPermission();

        // if a configuration class is specified, load it and use it.
        String cname = System.getProperty("java.util.logging.config.class");
        if (cname != null) {
            try {
                // Instantiate the named class.  It is its constructor's
                // responsibility to initialize the logging configuration, by
                // calling readConfiguration(InputStream) with a suitable stream.
                try {
                    Class<?> clz = ClassLoader.getSystemClassLoader().loadClass(cname);
                    clz.newInstance();
                    return;
                } catch (ClassNotFoundException ex) {
                    Class<?> clz = Thread.currentThread().getContextClassLoader().loadClass(cname);
                    clz.newInstance();
                    return;
                }
            } catch (Exception ex) {
                System.err.println("Logging configuration class \"" + cname + "\" failed");
                System.err.println("" + ex);
                // keep going and useful config file.
            }
        }

        String fname = System.getProperty("java.util.logging.config.file");
        if (fname == null) {
            fname = System.getProperty("java.home");
            if (fname == null) {
                throw new Error("Can't find java.home ??");
            }
            File f = new File(fname, "lib");
            f = new File(f, "logging.properties");
            fname = f.getCanonicalPath();
        }
        try (final InputStream in = new FileInputStream(fname)) {
            final BufferedInputStream bin = new BufferedInputStream(in);
            readConfiguration(bin);
        }
    }
```

## LogManager中的LoggerContext

LoggerContext为每个context的Logger提供命名空间。

默认的LogManager对象有一个系统上下文SystemLoggerContext和一个用户上下文LoggerContext。

系统上下文用于维护所有系统Logger的命名空间，并由系统代码查询。

如果系统Logger不存在于用户上下文中，它也将被添加到用户上下文中。

用户代码查询用户上下文，并在用户上下文中添加所有Logger。

Logger对象维护了 直接*Parent*

```java
    /**
     * 日志之间存在父子关系，最顶层的日志类型为LogManager$RootLogger,命名为""
     */
    @Test
    public void test() throws IOException {
        Logger logger = Logger.getLogger("com.wuhao.log");
        Logger logger1 = Logger.getLogger("com.wuhao");
        Logger logger2 = Logger.getLogger("com");
        System.out.println(logger);
        System.out.println(logger1.equals(logger.getParent()));
        System.out.println(logger2.equals(logger1.getParent()));
        System.out.println(logger2.getParent());
    }
    
java.util.logging.Logger@7fbe847c
true
true
java.util.logging.LogManager$RootLogger@41975e01
```



## Handler

JUL提供多种日志处理器。

- StreamHandler：用于将格式化记录写入OutputStream的简单处理程序。
- ConsoleHandler：用于将格式化记录写入System.err的简单处理程序
- FileHandler：将格式化日志记录写入单个文件或一组旋转日志文件的处理程序。
- SocketHandler：将格式化日志记录写入远程TCP端口的处理程序。
- MemoryHandler：缓冲内存中日志记录的处理程序



## Formatter

JUL提供了2种日志格式处理器

- SimpleFormatter：写简短的“人类可读”日志记录摘要。
- XMLFormatter：写入详细的XML结构信息。

{% endraw %}