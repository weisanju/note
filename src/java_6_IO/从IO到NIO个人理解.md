

# 阻塞IO有什么弊端

1. 建立连接与 读取连接输入 这两个过程 需要时间
2. 在单线程情况下：连接需要一个个处理，效率低下
3. 在多线程情况 下：如果海量连接 来到 会严重消耗 服务器线程
4. 在多线程情况下，海量连接，中只有一小部分是活跃连接，大部分是无效连接，为每个连接维护一个 线程浪费服务器性能



# 非阻塞IO如何改变上述情况的？

**实现思路**

1. 首先所有的读写都是异步的
2. 然后 把所有连接统一管理，并轮询操作系统内核 某一个 连接 是否可读可写

**好处**

1. 使用单一线程 就能管理 海量 连接
2. 还可针对指定连接 指定读或写事件

**不足之处**

1. 当连接数过多时 在用户态 轮询 会造成 过多的系统调用而 响应延时提高

   解决办法：让系统自己轮询：然后通知 上层用户。这就是 Selector模式 在Linux 中 `select poll`

2. 每次传送海量 连接FD给 内核  数据复制 也会造成延时

   解决办法：在内核中开辟一段空间，使得所有连接FD存放至此 ：省去了 COPY FD的时间

3. 每次连接可读之后：仍 需要把 数据从 网卡 到 内核内存，到用户内存 ，copy仍需要时间





# 客户端

```java
package com.weisanju.ioStudy;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.charset.StandardCharsets;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class SelfNioTimeClient {
    public static void main(String[] args) throws IOException {
        SocketChannel open = SocketChannel.open();
        open.connect(new InetSocketAddress(8080));
        ByteBuffer allocate = ByteBuffer.allocate(1024);

        while (true) {
            Scanner scanner = new Scanner(System.in);
            String yourName = scanner.next();
            if ("quit".equals(yourName)) {
                open.close();
                System.out.println("client exit");
                return;
            }
            open.write(ByteBuffer.wrap(yourName.getBytes(StandardCharsets.UTF_8)));
            int read = open.read(allocate);
            allocate.flip();
            System.out.println(StandardCharsets.UTF_8.decode(allocate));
            allocate.clear();
        }
    }
}
```

# 普通IO复用实现思路

```java
package com.weisanju.ioStudy;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

public class SelfNioTimeServer {
    public static void main(String[] args) throws IOException, InterruptedException {
        List<SocketChannel> objects = new LinkedList<>();
        ByteBuffer allocate = ByteBuffer.allocate(1024);

        ServerSocketChannel open = ServerSocketChannel.open();

        open.bind(new InetSocketAddress(8080));
        open.configureBlocking(false);


        //设置为阻塞
        while (true) {
            SocketChannel accept = open.accept();

            System.out.println(String.format("currentConnections:%d",objects.size()));

            if (accept!=null) {
                accept.configureBlocking(false);
                objects.add(accept);
            }else{
                Thread.sleep(2000);
            }

            for (SocketChannel object : objects) {
                if (object.isConnected()) {
                    int read = 0;
                    try {
                        read = object.read(allocate);
                    } catch (IOException e) {
                        System.out.println("client exception exits");
                        objects.remove(object);
                    }
                    System.out.println("already read："+read);
                    if (read>0) {
                        allocate.flip();
                        String name = Charset.defaultCharset().decode(allocate).toString();
                        String format = String.format("hello:%s,now time is  %s", name, LocalDateTime.now());
                        object.write(StandardCharsets.UTF_8.encode(format));
                        allocate.clear();
                    }else if(read < 0){
                        System.out.println("client normal exits");
                        objects.remove(object);
                    }
                }else{
                    System.out.println("client normal exits");
                    objects.remove(object);
                }
            }
        }

    }
}
```

# NIOSelector实现思路

```java
package com.weisanju.ioStudy;


import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.*;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.time.LocalDateTime;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;

public class NioTimeServer {
    public static void main(String[] args) throws IOException, InterruptedException {
        ByteBuffer allocate = ByteBuffer.allocate(1024);
        Selector selector = Selector.open();
        ServerSocketChannel open = ServerSocketChannel.open();
        open.bind(new InetSocketAddress(8080));
        open.configureBlocking(false);

        open.register(selector, SelectionKey.OP_ACCEPT);
        //设置为阻塞
        while (selector.select() >= 0) {
            System.out.println(String.format("currentConnections:%d", selector.keys().size() - 1));
            Set<SelectionKey> selectionKeys = selector.selectedKeys();
            for (SelectionKey selectedKey : selectionKeys) {
                if (selectedKey.isAcceptable()) {
                    SocketChannel accept = ((ServerSocketChannel)selectedKey.channel()).accept();
                    accept.configureBlocking(false);
                    accept.register(selector, SelectionKey.OP_READ);
                } else {
                    SocketChannel channel = (SocketChannel) selectedKey.channel();
                    int read = 0;
                    try {
                        read = channel.read(allocate);
                    } catch (IOException e) {
                        System.out.println("client exception exits");
                        read = -1;
                        selectedKey.cancel();
                    }
                    System.out.println("already read：" + read);
                    if (read > 0) {
                        allocate.flip();
                        String name = Charset.defaultCharset().decode(allocate).toString();
                        String format = String.format("hello:%s,now time is  %s", name, LocalDateTime.now());
                        channel.write(StandardCharsets.UTF_8.encode(format));
                        allocate.clear();
                    } else if (read < 0) {
                        System.out.println("client normal exits");
                        selectedKey.cancel();
                    }
                }
            }
            selectionKeys.clear();
        }

    }
}
```

