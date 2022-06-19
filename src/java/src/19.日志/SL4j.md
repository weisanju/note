# 概述

使用过 Log4J 和 LogBack 的同学肯定能发现，这两个框架的设计理念极为相似，使用方法也如出一辙。

其实这个两个框架的作者都是一个人，Ceki Gülcü，俄罗斯程序员。

Log4J 最初是基于Java开发的日志框架，发展一段时间后，作者Ceki Gülcü将 Log4j 捐献给了Apache软件基金会，使之成为了Apache日志服务的一个子项目。 又由于 Log4J 出色的表现，后续又被孵化出了支持C, C++, C#, Perl, Python, Ruby等语言的子框架。

然而，伟大的程序员好像都比较有个性。Ceki Gülcü由于不满Apache对 Log4J 的管理，决定不再参加 Log4J 的开发维护。“出走”后的Ceki Gülcü另起炉灶，开发出了 LogBack 这个框架（ SLF4J 是和 LogBack 一起开发出来的）。

LogBack 改进了很多 Log4J 的缺点，在性能上有了很大的提升，同时使用方式几乎和 Log4J 一样，许多用户开始慢慢开始使用 LogBack 。

由于受到 LogBack 的冲击， Log4J 开始式微。终于，2015年9月，Apache软件基金业宣布， Log4j 不在维护，建议所有相关项目升级到 Log4j2 。

Log4J2 是Apache开发的一个新的日志框架，改进了很多 Log4J 的缺点，同时也借鉴了 LogBack ，号称在性能上也是完胜 LogBack







# 门面模式如何动态替换日志实现 

## sl4j-api1.8以前

```java
public static ILoggerFactory getILoggerFactory() {
       //双重检查
       if (INITIALIZATION_STATE == UNINITIALIZED) {
           synchronized (LoggerFactory.class) {
               if (INITIALIZATION_STATE == UNINITIALIZED) {
                   INITIALIZATION_STATE = ONGOING_INITIALIZATION;
                   //初始化工厂类，进去后就会发现会用到下面几行的StaticLoggerBinder类
                   performInitialization();
               }
           }
       }
       switch (INITIALIZATION_STATE) {
       case SUCCESSFUL_INITIALIZATION:
           //关键所在，StaticLoggerBinder这个类是logback的（slf4j-log4j12中也有同名类）
           return StaticLoggerBinder.getSingleton().getLoggerFactory();
       。。。
   }
```

* 上述代码还是在slf4j-api中，说到这有个疑问，那么在slf4j-api这个包中，不存在这个StaticLoggerBinder类，是怎么打包出来的呢？查看源码，发现其实源码中有这个类。只是在pom打包时将这个impl包整个删掉了。非常粗暴（1.8之后不再使用这种方式了）

* **原理是** ：在代码中写死了，利用类加载机制，只会加载一个 同名的类，其余实现类则忽略 掉

    `org.slf4j.impl.StaticLoggerBinder`

```
if (!isAndroid()) {
            // We need to use the name of the StaticLoggerBinder class, but we can't
            // reference
            // the class itself.
            //private static String STATIC_LOGGER_BINDER_PATH = "org/slf4j/impl/StaticLoggerBinder.class";
            staticLoggerBinderPathSet = findPossibleStaticLoggerBinderPathSet();
            reportMultipleBindingAmbiguity(staticLoggerBinderPathSet);
        }
        // the next line does the binding
        StaticLoggerBinder.getSingleton();
```

## sl4j-api1.8之后

> **使用*SPI*机制**

## 核心绑定逻辑

* 根据SPI机制 进行类发现
* 如果发现多个实现类则 输出日志
* 默认选择 第一个实例化，并报告被实例化的 那个实现类
* 进行其他初始化动作
* 如果 SPI类发现机制 没有发现，则  使用 旧版本的 类发现机制，报告当前的 *StaticLoggerBinder* binder

```java
private final static void bind() {
    try {
        List<SLF4JServiceProvider> providersList = findServiceProviders();
        reportMultipleBindingAmbiguity(providersList);
        if (providersList != null && !providersList.isEmpty()) {
           PROVIDER = providersList.get(0);
           PROVIDER.initialize();
           INITIALIZATION_STATE = SUCCESSFUL_INITIALIZATION;
            reportActualBinding(providersList);
            fixSubstituteLoggers();
            replayEvents();
            // release all resources in SUBST_FACTORY
            SUBST_PROVIDER.getSubstituteLoggerFactory().clear();
        } else {
            INITIALIZATION_STATE = NOP_FALLBACK_INITIALIZATION;
            Util.report("No SLF4J providers were found.");
            Util.report("Defaulting to no-operation (NOP) logger implementation");
            Util.report("See " + NO_PROVIDERS_URL + " for further details.");

            Set<URL> staticLoggerBinderPathSet = findPossibleStaticLoggerBinderPathSet();
            reportIgnoredStaticLoggerBinders(staticLoggerBinderPathSet);
        }
    } catch (Exception e) {
        failedBinding(e);
        throw new IllegalStateException("Unexpected initialization failure", e);
    }
}
```



```java
private static List<SLF4JServiceProvider> findServiceProviders() {
    ServiceLoader<SLF4JServiceProvider> serviceLoader = ServiceLoader.load(SLF4JServiceProvider.class);
    List<SLF4JServiceProvider> providerList = new ArrayList<SLF4JServiceProvider>();
    for (SLF4JServiceProvider provider : serviceLoader) {
        providerList.add(provider);
    }
    return providerList;
}
```



# 父类委托机制

如果 当前 logger没有配置 *LEVEL* 或者没有配置 APPEND，则会往向上一级父级 寻找

```java
public void callAppenders(ILoggingEvent event) {
       int writes = 0;
       for (Logger l = this; l != null; l = l.parent) {
           writes += l.appendLoopOnAppenders(event);
           //这里能看到日志会不断寻找其父级logger，并且把logevent交给父级的appender，除非additive为false，这也和我们配置中的<logger additive>属性对应上了
           if (!l.additive) {
               break;
           }
       }
       // No appenders in hierarchy
       if (writes == 0) {
           loggerContext.noAppenderDefinedWarning(this);
       }
   }
```

所以这里我们明白一点：**未专门配置appender的logger，且additive为true**的（默认就是），实际上最终都是由root的appender完成的日志输出。