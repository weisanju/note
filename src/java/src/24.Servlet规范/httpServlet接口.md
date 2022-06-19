# Servlet接口

## 请求处理

### 请求方法

#### doGet

一般用于查询,资源获取

#### doPost

用于修改服务器资源

#### doPut

用于文件上传

#### doDelete

删除资源

#### doHead

只返回DoGet请求的 头

#### doOptions

返回HttpServlet支持的 方法,通过 Allow 响应头返回支持的 HTTP 操作，如 GET、POST

#### doTrace 

返回的响应包含 TRACE 请求的所有头信息

#### 有条件 GET 支持

*HttpServlet* 定义了用于支持有条件 GET 操作的 *getLastModified* 方法。所谓的有条件 GET 操作是指客户端
通过 GET 请求获取资源时，当资源自第一次获取那个时间点发生更改后才再次发生数据，否则将使用客户
端缓存的数据。在一些适当的场合，实现此方法可以更有效的利用网络资源，减少不必要的数据发送



## Servlet实例数量

**单实例的Servlet**

* 通过注解描述的（第 8 章 注解和可插拔性）或者在 Web 应用程序的部署描述符（第 14 章 部署描述符）中描述的 servlet 声明，控制着 servlet 容器如何提供 servlet 实例

* 对于未托管在分布式环境中（默认）的 *servlet* 而言，*servlet* 容器对于每一个 *Servlet* 声明必须且只能产生一
  个实例。不过，如果 Servlet 实现了 *SingleThreadModel* 接口，servlet 容器可以选择实例化多个实例以便处
  理高负荷请求或者串行化请求到一个特定实例。
  如果 *servlet* 以分布式方式进行部署，容器可以为每个虚拟机（JVM）的每个 *Servlet* 声明产生一个实例。但
  是，如果在分布式环境中 servlet 实现了 SingleThreadModel 接口，此时容器可以为每个容器的 JVM 实例化
  多个 Servlet 实例

***SingleThreadModel***  

* 它保证在  同一时刻 只能由一个 线程执行 *service*方法 

* 实现的方式大致由两种

  * 针对 单实例 进行 同步锁定
  * 针对 多实例 维护实例池 , 分配空闲实例

* 但是 仍然避免不了 线程安全问题, 尤其是在 多个servlet针对  *session*的attribute时

* 最好的办法是 不要编写 有状态的 *servlet*

* 已经过时

  

## 生命周期

### 加载和实例化

Servlet 容器负责加载和实例化 Servlet。**加载和实例化可以发生在容器启动时，或者延迟初始化直到容器决定有请求需要处理时**。当 Servlet 引擎启动后，servlet 容器必须定位所需要的 Servlet 类。Servlet 容器使用普通的 Java 类加载设施加载 Servlet 类。可以从本地文件系统或远程文件系统或者其他网络服务加载。加载
完 Servlet 类后，容器就可以实例化它并使用了

### 初始化

一旦一个 Servlet 对象实例化完毕，容器接下来必须在处理客户端请求之前初始化该 Servlet 实例。初始化
的目的是以便 Servlet 能读取持久化配置数据，**初始化一些代价高的资源（比如 JDBC API 连接）**，或者执
行一些一次性的动作。

```java
init(ServletConfig config)
```

**初始化时的错误条件**
在初始化阶段，servlet 实现可能抛出 UnavailableException 或 ServletException 异常。在这种情况下，Servlet
不能放置到活动服务中，而且 Servlet 容器必须释放它。如果初始化没有成功，destroy 方法不应该被调用。
在实例初始化失败后容器可能再实例化和初始化一个新的实例。此规则的例外是，**当抛出的**
**UnavailableException 表示一个不可用的最小时间，容器在创建和初始化一个新的 servlet 实例之前必须等待**
**一段时间**。



### 请求处理

#### forward与include

* *forward*

    一旦调用forward,**除了保留forward前的response里的header外**，其它的都不保留

    ```java
    //该命令 会立马 分派到指定servlet
    request.getRequestDispatcher("/forwardDemo02").forward(request,response);
    ```

* *include* 

    包含 该URL的 内容,(*printer*) 其他 *header* 都不要

* forward include之间的数据共享可以通过  *request.attribute* 实现

#### **多线程问题**

不要编写有状态的 *Servlet*

#### 请求处理时的异常

*UnavailableException*

**永久性不可用**

* Servlet 容器必须从服务中移除这个 Servlet，调用它的 destroy 方法，并释放 Servlet 实例。
* 所有被容器拒绝的请求，都会返回一个 SC_NOT_FOUND (404) 响
  应。

**临时不可用**

* 返回一个 SC_SERVICE_UNAVAILABLE (503)
* 同时会返回一个 Retry-After 头指示此 Servlet 什么时候可用

容器可以选择忽略永久性和临时性不可用的区别，并把 UnavailableExceptions 视为永久性的，从而 Servlet 抛出 UnavailableException 后需要把它从服务中移除。



#### 异步处理

>  Filter 及/或 Servlet 在生成响应之前必须等待一些资源或事件以便完成请求处理,比如，Servlet 在进行生成一个响应之前可能等待一个可用的 JDBC 连接，或者一个远程 web 服务的响应，或者一个 JMS 消息，或者一个应用程序事件, 在 Servlet 中等待是一个低效的操作，因为这是阻塞操作，从而白白占用一个
> 线程或其他一些受限资源

**异步请求事件顺序**

* 收到请求, 通过一系列的 *filter*
* 处理请求参数
* 发出请求去获取一些资源或数据 例如获取JDBC连接,发起Web远程服务
* servlet 不产生响应并返回
* 过了一段时间后，所请求的资源变为可用，此时处理线程继续处理事件，要么在同一个线程，要么通过
  AsyncContext 分派到容器中的一个资源上

**异步的操作**

```
startAsync() //使用原生的未经包装的 request对象
startAsync(req,resp)//包装的 req,resp对象
complete() //完成
```

**异步的限制**

* 所有涉及到 异步调用链的  filter必须是支持异步的

* 当 从一个异步servlet 分派到 同步servlet 时, servlet结束后,会自动调用 complete

* 同步 servlet 不能 分派到 异步servlet

* 异步servlet 的响应 等到 调用 complete 才返回

* 异步超时之后 会自动 提交响应

    

```java
{
        System.out.println("servlet1");
        System.out.println(req.getClass());
        System.out.println(response.getClass());
        AsyncContext asyncContext = req.startAsync();
        if(asyncContext.hasOriginalRequestAndResponse()){
            asyncContext.start(()->{
                try {
                    Thread.sleep(2000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                // 设置响应内容类型
                response.setContentType("text/html;charset=UTF-8");
                //格式化输出
                PrintWriter out;
                try {
                    out = response.getWriter();
                    String title = "自动刷新 Header 设置 - 菜鸟教程实例";
                    String docType =
                            "<!DOCTYPE html>\n";
                    out.println(docType +
                            "<html>\n" +
                            "<head><title>" + title + "</title></head>\n"+
                            "<body bgcolor=\"#f0f0f0\">\n" +
                            "<h1 align=\"center\">" + title + "</h1>\n" +
                            "<p>当前时间是：" + LocalDateTime.now() + "</p>\n");
                } catch (IOException e) {
                    e.printStackTrace();
                }
                asyncContext.dispatch();
            });
        }else{
            //开始干另一件事
            System.out.println("异步处理完毕了哈哈哈");
            asyncContext.complete();
        }
        System.out.println("立即返回");
    }
```

**升级处理**

* 在 HTTP/1.1，Upgrade 通用头（general-header）允许客户端指定其支持和希望使用的其他通信协议。如果
    服务器找到合适的切换协议，那么新的协议将在之后的通信中使用。

* Servlet 容器提供了 HTTP 升级机制。不过，Servlet 容器本身不知道任何升级协议。协议处理封装在 *HttpUpgradeHandler* 协议处理器。**在容器和HttpUpgradeHandler 协议处理器之间通过字节流进行数据读取或写入**
* 流程
    * 当收到一个升级（upgrade）请求，servlet 可以调用 HttpServletRequest.upgrade 方法启动升级处理。该方法
        实例化给定的 HttpUpgradeHandler 类，返回的 HttpUpgradeHandler 实例可以被进一步的定制。
    * 应用准备发送一个合适的响应到客户端。
    * 退出 servlet service 方法之后，servlet 容器完成所有过滤器的处理并标记连接已交给 HttpUpgradeHandler 协议处理器处理。
    * 然后调用 HttpUpgradeHandler 协议处理器的 init 方法，传入一个 WebConnection 以允许 HttpUpgradeHandler 协议处理器访问数据流。
    * Servlet 过滤器仅处理初始的 HTTP 请求和响应，然后它们将不会再参与到后续的通信中。换句话说，一旦请求被升级，它们将不会被调用。
    * 协议处理器（ProtocolHandler）可以使用非阻塞 IO（non blocking IO）消费和生产消息。当处理 HTTP 升级时，开发人员负责线程安全的访问 ServletInputStream 和 ServletOutputStream。当升级处理已经完成，将调用 HttpUpgradeHandler.destroy 方法。

### **终止服务**

* Servlet 容器没必要保持装载的 Servlet 持续任何特定的一段时间。

* 一个 Servlet 实例可能会在 servlet 容器内保持活跃（active）持续一段时间（以毫秒为单位），Servlet 容器的寿命可能是几天，几个月，或几年，或者是任何之间的时间。当 Servlet 容器确定 servlet 应该从服务中移除时，将调用 Servlet 接口的 destroy 方法以允许 Servlet 释放它使
    用的任何资源和保存任何持久化的状态。例如，当想要节省内存资源或它被关闭时，容器可以做这个。

* 在 servlet 容器调用 destroy 方法之前，它必须让当前正在执行 service 方法的任何线程完成执行，或者超过
    了服务器定义的时间限制。一旦调用了 servlet 实例的 destroy 方法，容器无法再路由其他请求到该 servlet 实例了。如果容器需要再次使用该 servlet，它必须用该 servlet 类的一个新的实例。





