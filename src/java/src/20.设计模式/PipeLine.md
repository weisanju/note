# Pipeline 设计模式

## 定义

**解释**

Pipeline 翻译过来就是水管的意思，Pipeline 设计模式其实很简单，就像是我们常用的 CI/CD 的 Pipeline 一样，一个环节做一件事情，最终串联成一个完整的 Pipeline。

**概念**

Pipeline 设计模式有三个概念：Pipeline、Valve、Context。它们的关系大概是这样：

![20201017160821](https://i.loli.net/2020/10/17/HUp52SCEkmXcaZy.png)

**对象定义**

```java
public interface Pipeline {
    void init(PipelineConfig config);
    void start();
    Context getContext();
}

public class Context {

}

public interface Valve {
    void invoke(Context context);
    void invokeNext(Context context);
    String getValveName();
}
```

**Tomcat 也广泛使用了 Pipeline 设计模式**

![20201017161509](https://i.loli.net/2020/10/17/7TUKzpjCcgBLQ2M.png)



**value配置**

```json
{
    "scene_a": {
        "valves": [
            "checkOrder",
            "checkPayment",
            "checkDiscount",
            "computeMount",
            "payment",
            "DeductInventory"
        ],
        "config": {
            "sendEmail": true,
            "supportAlipay": true
        }
    }
}
```



## Pipeline变种与演化

> Pipeline不是一成不变的，根据你的需要，它可以有很多变种和演化。



### 设计模式

>Pipeline其实是使用了责任链模式的思想。但它也可以和其它设计模式很好地结合起来。



**策略模式**

可以在配置里面写上当前这个业务线要发送的渠道,然后在Valve里面通过策略模式去决定使用什么渠道发送

**模板方法模式**

这个时候就可以使用模板方法模式，定义一个抽象的Valve，把公共逻辑抽取出来，把每个Valve差异的逻辑做成抽象方法，由Valve自己去实现。

**工厂方法模式**

```java
Pipeline pipeline = PipelineFactory.create(pipelineConfig);
pipeline.start();
```

**组合**

虽然我们说一个Valve只做一件简单的事。但这是相对于整个流程来说的。有时候太过细化也不好，不方便管理。正确的做法应该是做好抽象和分组。比如我们会有一个“校验”阶段，就不用把具体每个字段的校验都单独抽成Valve放进主流程。我们可以就在主流程放一个“校验”的Valve，然后在这个“校验”的Valve里面专门生成一条“校验Pipeline”。这样主流程也比较清晰明了，每个Valve的职责也比较清晰。

> 注意，子Pipeline应该有它单独的Context，但是它同时也应该具有主Pipeline的Context，是不是应该通过继承来实现?



## **树与图**

上面我们介绍的Pipeline，本质上是一个链。但如果往更通用（同时也更复杂）的方向去设计，它还可以做成一个图或者树。

假设我们在某个环节有一个条件分支，通过当时的context里面的数据状态，来判断下一步要走哪个Valve，形成一个树。最后可能又归拢到一个Valve，那就形成了一个图。



## **并行执行**

我们在前面看到Valve都是链式一个一个执行的。但有时候可能多个Valve彼此之间并不依赖，可以同时并行地去跑。比如发消息，可能多个Valve并行地去发

这个时候我们可以把Pipeline改造一下，就像Jenkins设计Pipeline那样，把一个完整的Pipeline分成Phase、Stage、Step等，我们可以对某个Phase或者某个Step设置成可以并行执行的。这需要另外写一个并行执行的Pipeline，用CountDownLatch等工具来等待所有Valve执行完，往下走。



## **日志和可视化**

日志和可视化是有必要的。对于一条Pipeline来说，推荐在Context里面生成一个traceId，然后用AOP等技术打印日志或者落库，最后通过可视化的方式在界面展现每次调用经过了哪些Valve，时间，在每个Valve执行前和执行后的Context等等信息。

异常也很重要。如果使用Pipeline设计模式，推荐专门定义一套异常，可以区分为“可中断Pipeline异常”和“不可中断Pipeline”异常。这个根据实际的业务需求，来决定是否需要中断Pipeline。以我们前面的例子来说，我们在校验阶段如果不通过，就应该抛出一个可以中断Pipeline的异常，让它不往下走。但如果在发送邮件的时候发生了异常，只需要catch住异常，打印一下warn日志，继续往下走。中不中断Pipeline，是业务来决定的。



## **使用ThreadLocal**

不要把零散的一个个属性放进ThreadLocal，因为同一种类型，一个线程只能在一个ThreadLocal里面放一个值。而我们的上下文可能会有多个String、boolean等值。如果使用ThreadLocal，可以把所有属性都包成一个Context类，放进ThreadLocal。



## **Pipe的缺点**

* 第一个缺点是可读性不强。因为它是可配置化的，且配置经常在外部（比如数据库里的一个JSON）。所以可读性不好。尤其是我们在读Valve代码的时候，**如果不对照配置，其实是不知道它的前后调用关系的**。

* 第二个缺点是Pipeline之间传递数据是通过Context，而不是简单的函数调用。所以一条Pipeline是有状态的，而且**方法调用内部修改Context**，而不是通过返回值，是有副作用的。



[**参考链接**](https://www.toutiao.com/i6872495007941526020/)



 

