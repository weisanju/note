# 概述

JVM参数有很多，其实我们直接使用默认的JVM参数，不去修改都可以满足大多数情况。但是如果你想在有限的硬件资源下，部署的系统达到最大的运行效率，那么进行相关的JVM参数设置是必不可少的。下面我们就来对这些JVM参数进行详细的介绍。

　　JVM参数主要分为以下三种（可以根据书写形式来区分）：





# 标准参数

* 标准参数中包括功能以及输出的结果都是很稳定的，基本上**不会随着JVM版本的变化而变化**。

* 以 - 开头

**模块操作**

```
-p <模块路径>
--module-path <模块路径>...
              用 ; 分隔的目录列表, 每个目录
              都是一个包含模块的目录。
--upgrade-module-path <模块路径>...
              用 ; 分隔的目录列表, 每个目录
              都是一个包含模块的目录, 这些模块
              用于替换运行时映像中的可升级模块
--add-modules <模块名称>[,<模块名称>...]
              除了初始模块之外要解析的根模块。
              <模块名称> 还可以为 ALL-DEFAULT, ALL-SYSTEM,
              ALL-MODULE-PATH.
--list-modules
              列出可观察模块并退出
-d <module name>
--describe-module <模块名称>
              描述模块并退出
--validate-modules
              验证所有模块并退出
              --validate-modules 选项对于查找
              模块路径中模块的冲突及其他错误可能非常有用。
--show-module-resolution
              在启动过程中显示模块解析输出
```

**类路径搜索**

```
-cp <目录和 zip/jar 文件的类搜索路径>
-classpath <目录和 zip/jar 文件的类搜索路径>
--class-path <目录和 zip/jar 文件的类搜索路径>
              使用 ; 分隔的, 用于搜索类文件的目录, JAR 档案
              和 ZIP 档案列表。
              
```

**版本、详细、帮助**

```
-verbose:[class|module|gc|jni]
              启用详细输出
-version      将产品版本输出到错误流并退出
--version     将产品版本输出到输出流并退出
-showversion  将产品版本输出到错误流并继续
--show-version
              将产品版本输出到输出流并继续

-? -h -help
              将此帮助消息输出到错误流
--help        将此帮助消息输出到输出流
-X            将额外选项的帮助输出到错误流
--help-extra  将额外选项的帮助输出到输出流
```

**属性与参数**

```
-D<名称>=<值>
              设置系统属性
@argument 文件
              一个或多个包含选项的参数文件
```

**断言**

```
-ea[:<程序包名称>...|:<类名>]
-enableassertions[:<程序包名称>...|:<类名>]
              按指定的粒度启用断言
-da[:<程序包名称>...|:<类名>]
-disableassertions[:<程序包名称>...|:<类名>]
              按指定的粒度禁用断言
-esa | -enablesystemassertions
              启用系统断言
-dsa | -disablesystemassertions
              禁用系统断言
              

```

**代理库**

```
-agentlib:<库名>[=<选项>]
              加载本机代理库 <库名>, 例如 -agentlib:jdwp
              另请参阅 -agentlib:jdwp=help
-agentpath:<路径名>[=<选项>]
              按完整路径名加载本机代理库
-javaagent:<jar 路径>[=<选项>]
              加载 Java 编程语言代理, 请参阅 java.lang.instrument
```

**其他**

    
    --dry-run     创建 VM 并加载主类, 但不执行 main 方法。
                  此 --dry-run 选项对于验证诸如
                  模块系统配置这样的命令行选项可能非常有用。
    
    
    -splash:<图像路径>
                  使用指定的图像显示启动屏幕
                  自动支持和使用 HiDPI 缩放图像
                  (如果可用)。应始终将未缩放的图像文件名 (例如, image.ext)
                  作为参数传递给 -splash 选项。
                  将自动选取提供的最合适的缩放
                  图像。
                  有关详细信息, 请参阅 SplashScreen API 文档
    
    -disable-@files
                  阻止进一步扩展参数文件
    --enable-preview
                  允许类依赖于此发行版的预览功能
# X 参数

非标准化参数。表示在将来的JVM版本中可能会发生改变，

**常用**

```
-Xmn<大小>        为年轻代（新生代）设置初始和最大堆大小
                  （以字节为单位）
-Xms<大小>        设置初始 Java 堆大小
-Xmx<大小>        设置最大 Java 堆大小
-Xss<大小>        设置 Java 线程栈大小
```



    -Xbatch           禁用后台编译
    -Xbootclasspath/a:<以 ; 分隔的目录和 zip/jar 文件>
                      附加在引导类路径末尾
    -Xcheck:jni       对 JNI 函数执行其他检查
    -Xcomp            在首次调用时强制编译方法
    -Xdebug           为实现向后兼容而提供
    -Xdiag            显示附加诊断消息
    -Xfuture          启用最严格的检查，预期将来的默认值
    -Xint             仅解释模式执行
    -Xinternalversion
                      显示比 -version 选项更详细的 JVM
                      版本信息
    -Xloggc:<文件>    将 GC 状态记录在文件中（带时间戳）
    -Xmixed           混合模式执行（默认值）
    
    -Xnoclassgc       禁用类垃圾收集
    -Xrs              减少 Java/VM 对操作系统信号的使用（请参见文档）
    -Xshare:auto      在可能的情况下使用共享类数据（默认值）
    -Xshare:off       不尝试使用共享类数据
    -Xshare:on        要求使用共享类数据，否则将失败。
    -XshowSettings    显示所有设置并继续
    -XshowSettings:all
                      显示所有设置并继续
    -XshowSettings:locale
                      显示所有与区域设置相关的设置并继续
    -XshowSettings:properties
                      显示所有属性设置并继续
    -XshowSettings:vm
                      显示所有与 vm 相关的设置并继续
    -XshowSettings:system
                      （仅 Linux）显示主机系统或容器
                      配置并继续
    -Xverify          设置字节码验证器的模式
    --add-reads <模块>=<目标模块>(,<目标模块>)*
                      更新 <模块> 以读取 <目标模块>，而无论
                      模块声明如何。
                      <目标模块> 可以是 ALL-UNNAMED 以读取所有未命名
                      模块。
    --add-exports <模块>/<程序包>=<目标模块>(,<目标模块>)*
                      更新 <模块> 以将 <程序包> 导出到 <目标模块>，
                      而无论模块声明如何。
                      <目标模块> 可以是 ALL-UNNAMED 以导出到所有
                      未命名模块。
    --add-opens <模块>/<程序包>=<目标模块>(,<目标模块>)*
                      更新 <模块> 以在 <目标模块> 中打开
                      <程序包>，而无论模块声明如何。
    --illegal-access=<值>
                      允许或拒绝通过未命名模块中的代码对命名模块中的
                      类型成员进行访问。
                      <值> 为 "deny"、"permit"、"warn" 或 "debug" 之一
                      此选项将在未来发行版中删除。
    --limit-modules <模块名>[,<模块名>...]
                      限制可观察模块的领域
    --patch-module <模块>=<文件>(;<文件>)*
                      使用 JAR 文件或目录中的类和资源
                      覆盖或增强模块。
    --disable-@files  禁止进一步扩展参数文件
    --source <版本>
                      设置源文件模式中源的版本。
# XX参数

这是我们日常开发中接触到最多的参数类型。这也是非标准化参数，相对来说不稳定，随着JVM版本的变化可能会发生变化，主要用于**JVM调优**和debug。

主要有两种类型，Boolean，与key-value

## Boolean类型

`-XX:[+-]<name> 表示启用或者禁用name属性。`

`-XX:+UseG1GC（表示启用G1垃圾收集器）`

## Key-Value类型

```
-XX:<name>=<value> 表示name的属性值为value。
-XX:MaxGCPauseMillis=500（表示设置GC的最大停顿时间是500ms）
```

# 参数详解

## 打印JVM参数

**打印已经被用户或者当前虚拟机设置过的参数**

```
-XX:+PrintCommandLineFlags
```

## **最大堆和最小堆内存设置**

```
-Xms512M：设置堆内存初始值为512M
-Xmx1024M：设置堆内存最大值为1024M

这里的ms是memory start的简称，mx是memory max的简称，分别代表最小堆容量和最大堆容量。但是别看这里是-X参数，其实这是-XX参数，等价于：
-XX:InitialHeapSize
-XX:MaxHeapSize
```

## **Dump异常快照**

```
-XX:+HeapDumpOnOutOfMemoryError
-XX:HeapDumpPath

-XX:+HeapDumpOnOutOfMemoryError -XX:HeapDumpPath=./
```

## **发送OOM后，执行一个脚本**

```
-XX:OnOutOfMemoryError
-XX:OnOutOfMemoryError="C:\Program Files\Java\jdk1.8.0_152\bin\jconsole.exe"

利用这个参数，我们可以在系统OOM后，自定义一个脚本，可以用来发送邮件告警信息，可以用来重启系统等等。
```

## **打印gc信息**

### **打印GC简单信息**

```
-verbose:gc
-XX:+PrintGC
```

一个是标准参数，一个是-XX参数，都是打印详细的gc信息。通常会打印如下信息：

```
[Full GC (Ergonomics)  12907K->11228K(19968K), 0.0541310 secs]

比如第一行，表示GC回收之前有12195K的内存，回收之后剩余1088K，总共内存为125951K
```

### **打印详细GC信息**

```
-XX:+PrintGCDetails
-XX:+PrintGCTimeStamps
```

## **指定GC日志以文件输出**

```
-Xloggc:./gc.log

　　这个在参数用于将gc日志以文件的形式输出，更方便我们去查看日志，定位问题。
```

## **垃圾收集器常用参数**

