# 概述

**JMH只适合细粒度的方法测试，并不适用于系统之间的链路测试！**





# 使用

## **引包**

```xml
        <dependency>
            <groupId>org.openjdk.jmh</groupId>
            <artifactId>jmh-core</artifactId>
            <version>1.20</version>
        </dependency>
        <dependency>
            <groupId>org.openjdk.jmh</groupId>
            <artifactId>jmh-generator-annprocess</artifactId>
            <version>1.20</version>
            <scope>provided</scope>
        </dependency>
```

## HelloWorldTest

```java
package com.weisanju.logger;

import org.openjdk.jmh.annotations.*;
import org.openjdk.jmh.runner.Runner;
import org.openjdk.jmh.runner.RunnerException;
import org.openjdk.jmh.runner.options.Options;
import org.openjdk.jmh.runner.options.OptionsBuilder;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.TimeUnit;

@Warmup(iterations = 5, time = 1, timeUnit = TimeUnit.SECONDS) //每次都进行 五次预热执行，每隔1秒进行一次预热操作
@Measurement(iterations = 5, time = 1, timeUnit = TimeUnit.SECONDS) //预热结束后，进行五次实际 执行，每隔一秒执行
public class JMHSample_01_HelloWorld {
    static class Demo {
        int id;
        String name;

        public Demo(int id, String name) {
            this.id = id;
            this.name = name;
        }
    }

    static List<Demo> demoList;

    static {
        demoList = new ArrayList();
        for (int i = 0; i < 10000; i++) {
            demoList.add(new Demo(i, "test"));
        }
    }

    @Benchmark //定义基准测试方法单元
    @BenchmarkMode(Mode.AverageTime) //取方法平均值
    @OutputTimeUnit(TimeUnit.MICROSECONDS) //输出单元 us
    public void testHashMapWithoutSize() {
        Map map = new HashMap();
        for (Demo demo : demoList) {
            map.put(demo.id, demo.name);
        }
    }

    @Benchmark //定义基准测试方法单元
    @BenchmarkMode(Mode.AverageTime)
    @OutputTimeUnit(TimeUnit.MICROSECONDS)
    public void testHashMap() {
        Map map = new HashMap((int) (demoList.size() / 0.75f) + 1);
        for (Demo demo : demoList) {
            map.put(demo.id, demo.name);
        }
    }

    public static void main(String[] args) throws RunnerException {
        Options opt = new OptionsBuilder()
                .include(JMHSample_01_HelloWorld.class.getSimpleName())
                .forks(1)
                .build();
        new Runner(opt).run();
    }
}
```



上面的代码翻译一下：分别定义两个基准测试的方法testHashMapWithoutSize和 testHashMap，这两个基准测试方法执行流程是：每个方法执行前都进行5次预热执行，每隔1秒进行一次预热操作，预热执行结束之后进行5次实际测量执行，每隔1秒进行一次实际执行，我们此次基准测试测量的是平均响应时长，单位是us。

预热？为什么要预热？因为 JVM 的 JIT 机制的存在，如果某个函数被调用多次之后，JVM 会尝试将其编译成为机器码从而提高执行速度。为了让 benchmark 的结果更加接近真实情况就需要进行预热。

从上面的执行结果我们看出，针对一个Map的初始化参数的给定其实有很大影响，当我们给定了初始化参数执行执行的速度是没给定参数的2/3，这个优化速度还是比较明显的，所以以后大家在初始化Map的时候能给定参数最好都给定了，代码是处处优化的，积少成多。

## 示例2

```java
package benchmark;

import org.openjdk.jmh.annotations.*;
import org.openjdk.jmh.infra.Blackhole;
import org.openjdk.jmh.runner.Runner;
import org.openjdk.jmh.runner.RunnerException;
import org.openjdk.jmh.runner.options.Options;
import org.openjdk.jmh.runner.options.OptionsBuilder;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.TimeUnit;

@State(Scope.Benchmark)
@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.NANOSECONDS)
@Fork(value = 1, jvmArgsPrepend = "-XX:+PrintStringTableStatistics")
@Warmup(iterations = 5)
@Measurement(iterations = 5)
public class StringInternBenchMark {

    @Param({"1", "100", "10000", "1000000"})
    private int size;

    private StringInterner str;
    private ConcurrentHashMapInterner chm;
    private HashMapInterner hm;

    @Setup
    public void setup() {
        str = new StringInterner();
        chm = new ConcurrentHashMapInterner();
        hm = new HashMapInterner();
    }

    public static class StringInterner {
        public String intern(String s) {
            return s.intern();
        }
    }

    @Benchmark
    public void useIntern(Blackhole bh) {
        for (int c = 0; c < size; c++) {
            bh.consume(str.intern("doit" + c));
        }
    }

    public static class ConcurrentHashMapInterner {
        private final Map<String, String> map;

        public ConcurrentHashMapInterner() {
            map = new ConcurrentHashMap<>();
        }

        public String intern(String s) {
            String exist = map.putIfAbsent(s, s);
            return (exist == null) ? s : exist;
        }
    }

    @Benchmark
    public void useCurrentHashMap(Blackhole bh) {
        for (int c = 0; c < size; c++) {
            bh.consume(chm.intern("doit" + c));
        }
    }

    public static class HashMapInterner {
        private final Map<String, String> map;

        public HashMapInterner() {
            map = new HashMap<>();
        }

        public String intern(String s) {
            String exist = map.putIfAbsent(s, s);
            return (exist == null) ? s : exist;
        }
    }

    @Benchmark
    public void useHashMap(Blackhole bh) {
        for (int c = 0; c < size; c++) {
            bh.consume(hm.intern("doit" + c));
        }
    }

    public static void main(String[] args) throws RunnerException {
        Options opt = new OptionsBuilder()
                .include(StringInternBenchMark.class.getSimpleName())
                .build();
        new Runner(opt).run();
    }
}
```







# **@Benchmark**

* @Benchmark标签是用来标记测试方法的，只有被这个注解标记的话，该方法才会参与基准测试，但是有一个基本的原则就是被
* @Benchmark标记的方法必须是public的。



# **@Warmup**

@Warmup用来配置预热的内容，可用于类或者方法上，越靠近执行方法的地方越准确。一般配置warmup的参数有这些：

- iterations：预热的次数。
- time：每次预热的时间。
- timeUnit：时间单位，默认是s。
- **batchSize：批处理大小，每次操作调用几次方法。（后面用到）**

# **@Measurement**

用来控制实际执行的内容，配置的选项本warmup一样。

# @BenchmarkMode

主要是表示测量的纬度，有以下这些纬度可供选择：

- Mode.Throughput 吞吐量纬度
- Mode.AverageTime 平均时间
- Mode.SampleTime 抽样检测
- Mode.SingleShotTime 检测一次调用

**Mode.All 运用所有的检测模式 在方法级别指定@BenchmarkMode的时候可以一定指定多个纬度，例如： @BenchmarkMode({Mode.Throughput, Mode.AverageTime, Mode.SampleTime, Mode.SingleShotTime})，代表同时在多个纬度对目标方法进行测量。**

| 名称                | 描述                             |
| ------------------- | -------------------------------- |
| Mode.Throughput     | 计算吞吐量                       |
| Mode.AverageTime    | 计算平均运行时间                 |
| Mode.SampleTime     | 在测试中，随机进行采样执行的时间 |
| Mode.SingleShotTime | 测量单次操作的时间               |
| Mode.All            | 所有模式依次运行                 |

**每个维度 默认运行 十轮**

# **@OutputTimeUnit**

@OutputTimeUnit代表测量的单位，比如秒级别，毫秒级别，微妙级别等等。一般都使用微妙和毫秒级别的稍微多一点。该注解可以用在方法级别和类级别，当用在类级别的时候会被更加精确的方法级别的注解覆盖，原则就是离目标更近的注解更容易生效。





# **@State**

在很多时候我们需要维护一些状态内容，比如在多线程的时候我们会维护一个共享的状态，这个状态值可能会在每隔线程中都一样，也有可能是每个线程都有自己的状态，JMH为我们提供了状态的支持。该注解只能用来标注在类上，因为类作为一个属性的载体。 @State的状态值主要有以下几种：

- Scope.Benchmark 该状态的意思是会在所有的Benchmark的工作线程中共享变量内容。

- Scope.Group 同一个Group的线程可以享有同样的变量
- Scope.Thread 每隔线程都享有一份变量的副本，线程之间对于变量的修改不会相互影响。 下面看两个常见的@State的写法：

## 要求

这个类必须遵循以下四条规则：

- 有无参构造函数(默认构造函数)
- 必须公共类
- 如果是内部类，需要是静态内部类
- 必须使用 @State 注解

## **配置方式**

第一种是 Benchmark 不在 State 的类里。这时需要在测试方法的入参列表里显式注入该 State。

```java
public class JMHSample_03_States {

    @State(Scope.Benchmark)
    public static class BenchmarkState {
        volatile double x = Math.PI;
    }

    @State(Scope.Thread)
    public static class ThreadState {
        volatile double x = Math.PI;
    }

    @Benchmark
    public void measureUnshared(ThreadState state) {
        state.x++;
    }

    @Benchmark
    public void measureShared(BenchmarkState state) {
        state.x++;
    }
}
```



第二种是 Benchmark 在 State 的类里。这时不需要在测试方法的入参列表里显式注入该 State。

```java
@State(Scope.Thread)
public class JMHSample_04_DefaultState {

    double x = Math.PI;

    @Benchmark
    public void measure() {
        x++;
    }

}
```

## Scope

| scope     | 描述                                                         |
| --------- | ------------------------------------------------------------ |
| Benchmark | Benchmark 中所有线程都使用同一个 State                       |
| Group     | Benchmark 中同一 Benchmark 组（使用@Group标识，后面再讲）使用一个 State |
| Thread    | Benchmark 中每个线程使用同一个 State                         |

# @Setup 和 @TearDown

这两个注解只能定义在注解了 State 里，其中，`@Setup`类似于 junit 的`@Before`，而`@TearDown`类似于 junit 的`@After`。

```java
@State(Scope.Thread)
public class JMHSample_05_StateFixtures {

    double x;

    @Setup(Level.Iteration)
    public void prepare() {
        System.err.println("init............");
        x = Math.PI;
    }

    @TearDown(Level.Iteration)
    public void check() {
        System.err.println("destroy............");
        assert x > Math.PI : "Nothing changed?";
    }


    @Benchmark
    public void measureRight() {
        x++;
    }

}
```

这两个注解注释的方法的调用时机，主要受 Level 的控制，JMH 提供了三种 Level，如下：

1. Trial

    Benchmark 开始前或结束后执行，如下。Level 为 Benchmark 的 Setup 和 TearDown 方法的开销不会计入到最终结果。

    ```java
    //Benchmark
    public void Benchmark01(){
        // call Setup method
        // 每个循环为一个iteration
        for(iterations){
            // 每个循环为一个invocation，这里会统计每次invocation的开销
            while(!timeout){
                // 调用我们的测试方法
            }
        }
        // call TearDown method
    }
    ```

2. Iteration

    Benchmark 里每个 Iteration 开始前或结束后执行，如下。Level 为 Iteration 的 Setup 和 TearDown 方法的开销不会计入到最终结果。

    ```java
    //Benchmark
    public void Benchmark01(){
        // 每个循环为一个iteration
        for(iterations){
            // call Setup method
            // 每个循环为一个invocation，这里会统计每次invocation的开销
            while(!timeout){
                // 调用我们的测试方法
            }
            // call TearDown method
        }
    }
    ```

3. Invocation

    Iteration 里每次方法调用开始前或结束后执行，如下。**Level 为 Invocation 的 Setup 和 TearDown 方法的开销将计入到最终结果**。









# JMH Maven

## 引入插件

```xml
<plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-shade-plugin</artifactId>
                <version>2.2</version>
                <executions>
                    <execution>
                        <phase>package</phase>
                        <goals>
                            <goal>shade</goal>
                        </goals>
                        <configuration>
                            <finalName>${uberjar.name}</finalName>
                            <transformers>
                                <transformer implementation="org.apache.maven.plugins.shade.resource.ManifestResourceTransformer">
                                    <mainClass>org.openjdk.jmh.Main</mainClass>
                                </transformer>
                            </transformers>
                            <filters>
                                <filter>
                                    <artifact>*:*</artifact>
                                    <excludes>
                                        <exclude>META-INF/*.SF</exclude>
                                        <exclude>META-INF/*.DSA</exclude>
                                        <exclude>META-INF/*.RSA</exclude>
                                    </excludes>
                                </filter>
                            </filters>
                        </configuration>
                    </execution>
                </executions>
            </plugin>
```

* 用 JMH 生成骨架

* ```powershell
    mvn archetype:generate ^
    -DinteractiveMode=false ^
    -DarchetypeGroupId=org.openjdk.jmh ^
    -DarchetypeArtifactId=jmh-java-benchmark-archetype ^
    -DarchetypeVersion=1.25 ^
    -DgroupId=cn.zzs.jmh ^
    -DartifactId=jmh-test01 ^
    -Dversion=1.0.0
    ```







# JMH Sample

[JMH样例代码](http://hg.openjdk.java.net/code-tools/jmh/file/tip/jmh-samples/src/main/java/org/openjdk/jmh/samples/)



