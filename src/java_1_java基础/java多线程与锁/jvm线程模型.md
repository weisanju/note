# 虚拟机运行时的java线程模型

# 线程创生纪

* **线程模型描述了Java虚拟机中的执行单元，是所有虚拟机组件的最终使能的对象。**
* 了解Java线程模型有助于了解虚拟机运行的概况。
* Java程序可以轻松创建线程，虚拟机本身也需要创建线程。
* 解释器、JIT编译器、GC是抽象出来执行某一具体任务的组件，这些组件执行任务时都需要依托线程。

所以，为了管理这些五花八门的线程，虚拟机将它们的公有特性抽象出来构成一个**线程模型**，如图4-1所示。



![](/images/java_thread_structure.png)

1）Thread：线程基类，定义所有线程都具有的功能。

2）JavaThread：Java线程在虚拟机层的实现。

3）NonJavaThread：相比Thread只多了一个可以遍历所有NonJavaThread的能力。

4）ServiceThread：服务线程，会处理一些杂项任务，如检查内存过低、JVMTI事件发生。

5）JvmtiAgentThread：JVMTI的RunAgentThread()方法启动的线程。

6）CompilerThread：JIT编译器线程。

7）CodeCacheSweeperThread：清理Code Cache的线程。

8）WatcherThread：计时器（Timer）线程。

9）JfrThreadSampler：JFR数据采样线程。

10）VMThread：虚拟机线程，会创建其他线程的线程，也会执行GC、退优化等。

11）ConcurrentGCThread：与WorkerThread及其子类一样，都是为GC服务的线程。



当使用命令行工具java启动应用程序时，**操作系统会定位到java启动器的main函数**，

java启动器调用JavaMain完成一个程序的生命周期，如代码清单4-1所示，这其中涉及各种线程的创建与销毁：

......

[建议了解文章](https://www.toutiao.com/i6934925749275394573/)





