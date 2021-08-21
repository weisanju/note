### 可重入锁

1. 重入锁可以完全替代 synchronized 关键字, jdk1.5 之间重入锁性能远远好于 synchronized 从 1.6 开始,jdk 在 synchronized 做了大量优化,使得两者性能差距并不大

2. 特性

    1. 可重入性质: 一个线程可以连续两次获得锁, 但相应的得释放两次锁

    ```java
    ReentryantLock lock1 = new ReentryantLock();
    lock1.lock()
    lock1.lock()
    lock1.unlock()
    lock1.unlock()
    ```

    2. 可中断性质
        1. 线程在尝试获取锁时,可被打断,并被打断后,释放相应的锁,让其他线程获取锁
        2. 案例 : 线程 a, 线程 b ,a 先得到锁 1,然后请求锁 2,b 先得到锁 2,然后请求锁 1
        3. 代码

    ```java
    package com.weisanju;
    
    import java.util.concurrent.locks.ReentrantLock;
    
    public class DeadLock {
        private static ReentrantLock lock1= new ReentrantLock();
        private static ReentrantLock lock2= new ReentrantLock();
    
        public static class  ThreadTest implements  Runnable{
            private char name;
    
            public ThreadTest(char name) {
                this.name = name;
            }
    
            @Override
            public void run() {
                if(name == 'A'){
                    try {
                        lock1.lockInterruptibly();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    try {
                        Thread.sleep(1000);
                        lock2.lockInterruptibly();
                        System.out.println("A 得到锁了");
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }finally {
                        if(lock1.isHeldByCurrentThread()){
                            lock1.unlock();
                        }
                        if(lock2.isHeldByCurrentThread()){
                            lock2.unlock();
                        }
                    }
    
                }else{
                    try {
                        lock2.lockInterruptibly();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    try {
                        Thread.sleep(1000);
                        lock1.lockInterruptibly();
                        System.out.println("B 得到锁了");
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }finally {
                        if(lock1.isHeldByCurrentThread()){
                            lock1.unlock();
                        }
                        if(lock2.isHeldByCurrentThread()){
                            lock2.unlock();
                        }
                    }
                }
            }
        }
    
        public static void main(String[] args) {
            Thread ta = new Thread(new ThreadTest('A'));
            Thread tb = new Thread(new ThreadTest('B'));
    
            ta.start();
            tb.start();
    
            try {
                Thread.sleep(4000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
    
            tb.interrupt();
    
        }
    }
    
    ```

    3. 超时性质
        1. `tryLock()`:尝试获取锁,获取不成功则马上返回
        2. `tryLock(long mili)`:尝试获取锁,并等待指定时间段
    4. 公平锁
        1. 锁的申请遵循 先到先到,支持排队
        2. `public ReentrantLock(boolean fair)`
        3. 实现公平锁,系统需要维护一个有序队列,实现成本较高,性能太低
        4. 根据系统的调度,一个线程会倾向于再次获取已经持有的锁,这种锁分配是高效的

### Conditional 条件等待

1. 与 synchronized 配合 wait,notify 使用类似 , condition 配合与 Reentryant 锁使用实现线程间通信
2. 代码

```java
package com.weisanju;

import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class ConditionalTest {
    private  static  int flag =0;
    private static ReentrantLock lock = new ReentrantLock();
    private static Condition condition= lock.newCondition();
    private  static class  AThread implements Runnable{
        @Override
        public void run() {
            lock.lock();
            System.out.println("正等待条件发生");
            try {
                condition.await();
                System.out.println(flag);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }finally {
                lock.unlock();
            }

        }
    }

    public static void main(String[] args) throws InterruptedException {
        new Thread(new AThread()).start();
        flag = 666;
        Thread.sleep(200);
        lock.lock();
        System.out.println("已经获取锁");
        Thread.sleep(1000);

        condition.signal();
        lock.unlock();
    }
}
```

### 信号量

1. API
    1. 构造函数:`public Semaphore(int premits)`
    2. 逻辑方法
        1. acquire|acquireUninterruptible()|tryAcquire()
        2. release
2. 例子:省略

### 读写锁

1. 读写操作互斥表

    |      | 读     | 写   |
    | ---- | ------ | ---- |
    | 读   | 不阻塞 | 阻塞 |
    | 写   | 阻塞   | 阻塞 |

2. API

    1. `ReentrantReadWriteLock`
    2. `lock.readLock(),lock.writeLock()`

### 倒计时

1. API
    1. 构造函数:`public CountDownLatch(int count)`
    2. 逻辑操作
        1. 计时器减 1:`CountDownLatch.countDown()`
        2. 等待计时器归 0:``CountDownLatch.await();`
        3. 获取计数器:`CountDownLatch.getCount()`

### CyclicBarrier 循环栅栏

0. 每当有 `parties` 个 到达 `wait` 点时, 则执行 barrierAction

1. APi

    1. 构造函数:`public CyclicBarrier(int parties, Runnable barrierAction)`

    2. `await`:等待

    3. 一个线程在等待时被打断, 则其他线程抛出`BrokenBarrierException`,该线程抛出:`InterruptedException`

    4. code

        ```java
        package com.weisanju;
        
        import java.util.concurrent.BrokenBarrierException;
        import java.util.concurrent.CyclicBarrier;
        
        public class CyclicBarrierTest {
            private static CyclicBarrier barrier = new CyclicBarrier(5,new BarrierRun(false));
            public  static  class Solider implements  Runnable{
                private int i;
        
                public Solider(int i) {
                    this.i = i;
                }
        
                @Override
                public void run() {
                    try {
                        barrier.await();
        
                        Thread.sleep(1000);
                        System.out.println("士兵"+i+"完成任务");
                        barrier.await();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    } catch (BrokenBarrierException e) {
                        e.printStackTrace();
                    }
                }
            }
            public  static  class BarrierRun implements  Runnable{
                private boolean flag ;
        
                public BarrierRun(boolean flag) {
                    this.flag = flag;
                }
        
                @Override
                public void run() {
                    if (flag) {
                        System.out.println("任务完成");
                    }else{
                        System.out.println("集合完毕");
                        flag = true;
                    }
                }
            }
        
            public static void main(String[] args) {
                int n = 5;
        
                for (int i = 0; i < n; i++) {
                    System.out.println("士兵报数:"+i);
                    new  Thread(new Solider(i)).start();
                }
            }
        }
        
        ```

### 线程阻塞工具类

**API**

1. `LockSupport.unpack(Object)`,`LockSupport.pack(Thread)`
2. 类似于 值为 1 的信号量 操作
3. unpack 操作发生在 pack 操作之前,unpack 使得许可可用,pack 消耗许可
4. 不需要获取锁
5. 为每一个线程都拥有一个许可证
6. 被打断之后正常返回,可以通过 `Thread.isInterrputed`
7. `unpack(Object)`:object 为日志打印时的对象

**code**

```java
package com.weisanju;

import java.util.concurrent.locks.LockSupport;

public class LockSupportTest {
    public static  class AThread implements Runnable{
        @Override
        public void run() {
            LockSupport.park();
            if(Thread.currentThread().isInterrupted()){
                System.out.println("被打断了");
                return;
            }
            System.out.println("正常运行");
        }
    }

    public static void main(String[] args) {
        Thread t1 = new Thread(new AThread());
        Thread t2 = new Thread(new AThread());

        t1.start();
        t2.start();
        t1.interrupt();
        LockSupport.unpark(t2);
    }
}
```

