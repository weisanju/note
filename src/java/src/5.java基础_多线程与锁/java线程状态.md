# 线程状态

java中Thread有6种状态，分别是：

1. NEW - 新创建的Thread，还没有开始执行
2. RUNNABLE - 可运行状态的Thread，包括准备运行和正在运行的。
3. BLOCKED - 正在等待资源锁的线程
4. WAITING - 正在无限期等待其他线程来执行某个特定操作
5. TIMED_WAITING - 在一定的时间内等待其他线程来执行某个特定操作
6. TERMINATED - 线程执行完毕





# 线程生命周期

![](/images/thread_state.png)

