## 并发相关知识

#### 并发(Concurrency),并行(Parallelism)

并发:多项任务,交替执行

并行:多项任务,同时执行

#### 同步(Synchronous),异步(Asynchronous)

描述的是针对某个调用 获取返回结果的方式:是同步等待,还是异步通知

同步:调用某项方法时,等待方法返回结果

异步:调用后马上返回,结果计算完后,通知调用者

#### 阻塞(blocking),非阻塞(non-blocking)

描述的是多线程之间的相互影响

阻塞:一个线程占用了临界资源,其他线程必须等待这个线程释放资源

非阻塞:访问被其他线程占用的临界资源时, 不会阻塞等待,而立即返回

#### 临界区

表示公共资源,多个线程访问或修改同一个资源

#### 多线程竞争锁导致会问题

死锁:所有线程都不能动

饥饿锁:某个线程一直无法获取所需的资源

活锁:线程秉承谦让的原则,主动释放给他人使用,这样可能会导致资源在两个线程中跳动,而没有一个线程正常执行

### 并发级别

#### 阻塞

一个线程会阻塞在 获取资源的步骤中,直到其他线程释放该资源,synchronized 的锁为阻塞级别

#### 无饥饿

如果获取锁是公平的,各个线程排队获取锁,则该锁是无饥饿的

#### 无障碍

最弱的非阻塞调度

两个线程访问同一个临界区,都不会被对方所阻塞,一旦检测到某一方把数据改动了,则所有线程操作全部回滚

阻塞的控制方式是 悲观策略,假定两个线程之间很可能发生冲突,而非阻塞的调度是乐观的策略,认为多个线程不会发生冲突,或者概率不大,一旦发生冲突,就应该回滚

#### 无锁

要求有一个线程可以在有限步内完成操作

当所有线程都能尝试对临界区访问,但只有一个线程能 进入临界区,其他的线程会不断尝试

#### 无等待

1. 要求所有线程必须在有限步内完成
2. 典型的无等待结构是 RCU(read-copy-update),读无等待,更新时,先取得副本更新,然后适时写回

### 并行的两个重要定律

#### Amdahl 定律

1. 定义了串行系统并行化的加速比的计算公式,和理论上限

$$
加速比 = 优化前系统耗时 / 优化后系统耗时\\F:为系统串行比例\\T_1:为一个处理器的耗时\\T_n:为n个处理器优化后的耗时\\T_n = T_1(F+\frac{1}{n}*(1-F))\\加速比 = \frac{T_1}{T_n} = \frac{1}{F+\frac{1}{n}*(1-F)}
$$

2. 由公式可分析出
   1. CPU 处理器数量趋近于无穷,那么加速比与系统串行率成反比
   2. 如果系统串行率为 50%,则系统最大加速比为 2

#### Gustafson 定律

$$
a:串行时间,b:并行时间,n处理器个数\\
   实际执行时间 = a+b\\
   总执行时间 = a+n*b\\
   加速比= \frac{a+n*b}{a+b}\\
   串行比例 = F = \frac{a}{a+b}\\
   加速比 = \frac{a+n*b}{a+b} = \frac{a}{a+b}+\frac{n*(a+b-a)}{a+b}=F+n*(1-F)=n-F*(n-1)
$$

3. 两个定律的不同点

   1. Amdahl 定律侧重于 当 总任务一定时, 当串行比例一定时,加速比是有上线的
   2. Gustafson 定律侧重于 不管 F 的值有多高,只要 n 足够大,有足够的时间和 工作量,就能达到某个加速比

### java 多线程并发原则

#### 原子性 `Atomicity`

函数调用过程中 不可被其他线程打断,要么成功,要么失败

#### 可见性 `visibility`

对某一线程修改了某一个共享变量,其他线程能够立刻知道

#### 有序性 `ordering`

1. 在程序编译时可能 有指令重排:通过指令重排 减少 CPU 流水线指令的停顿
2. 线程重排原则
   1. 程序顺序原则:一个线程内保证语义的串行性,不保证并行性
   2. volatile 变量的写 先发生于读
   3. 锁规则:解锁必然发生在 加锁前
   4. 传递性: a 先于 b,b 先于 c,a 必然先于 c
   5. 线程 start 方法优先于它的每一个动作
   6. 所有操作先于 线程的终结
   7. 中断先于 被中断线程的代码
   8. 对象的构造函数执行,结束先于 finalize 方法

## java 并行程序基础

### 线程状态变更图

![20201024111905](https://i.loli.net/2020/10/24/3zNEiyP9eDkvdBa.png)

### 线程基本操作

[线程操作链接](线程操作.md)

### volatile 关键字

修饰变量

告知各个线程,取变量值时,从主内存中取,不要从副本取

### 线程组

```java
package com.weisanju;

public class ThreadGroupTest {
    public static class AThread implements  Runnable{
        @Override
        public void run() {
            System.out.println(Thread.currentThread().getName());
        }
    }

    public static void main(String[] args) {
        ThreadGroup threadGroup = new ThreadGroup("xjq");
        Thread t1 = new Thread(threadGroup,new AThread(),"t1");
        Thread t2 = new Thread(threadGroup,new AThread(),"t2");
        t1.start();
        t2.start();
        threadGroup.list();
        System.out.println(threadGroup.activeCount());
        System.out.println(threadGroup.activeGroupCount());
    }
}

```

### 守护线程

1. 线程分为用户线程 ,守护线程

2. 当用户线程执行完毕之后, 守护线程会自行退出

3. 守护线程一般完成系统性服务,例如垃圾回收,JIT 线程

4. 代码

   ```java
   package com.weisanju;

   public class DeamonTest {
       public static class  Athread implements  Runnable{

           @Override
           public void run() {
               while(true){
                   System.out.println(1);
                   try {
                       Thread.sleep(500);
                   } catch (InterruptedException e) {
                       e.printStackTrace();
                   }
               }
           }
       }

       public static void main(String[] args) throws InterruptedException {
           Thread t = new Thread(new Athread());
           t.setDaemon(true);
           t.start();
           Thread.sleep(2000);
       }
   }

   ```

### 线程优先级

1. `Thread.MAX_PRIORITY` = 10
2. `Thread.NORM_PRIORITY` = 5
3. `Thread.MIN_PRIORITY = 1`

## Java 锁

[java 锁](java锁.md)

## jdk 并发包

[JUC线程同步工具](JUC线程同步工具.md)

