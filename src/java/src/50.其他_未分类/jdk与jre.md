### JDK中rt.jar、tools.jar和dt.jar作用

1. dt.jar和tools.jar位于：{Java_Home}/lib/下，而rt.jar位于：{Java_Home}/jre/lib/下,其中：
2. rt.jar是JAVA基础类库，也就是你在java doc里面看到的所有的类的class文件
3. dt.jar是关于运行环境的类库
4. tools.jar是工具类库,编译和运行需要的都是toos.jar里面的类分别是sun.tools.java.*; sun.tols.javac.*;
5. 在Classpath设置这几个变量，是为了方便在程序中 import；Web系统都用到tool.jar。

### rt.jar

rt.jar 默认就在Root Classloader的加载路径里面的，而在Claspath配置该变量是不需要的；

同时jre/lib目录下的 其他jar:jce.jar、jsse.jar、charsets.jar、resources.jar都在Root Classloader中

### tools.jar

1. tools.jar 是系统用来编译一个类的时候用到的，即执行javac的时候用到
2. javac XXX.java  实际上就是运行  java -Calsspath=%JAVA_HOME%\lib\tools.jar xx.xxx.Main XXX.java
3. javac就是对上面命令的封装 所以tools.jar 也不用加到classpath里面

### dt.jar

dt.jar是关于运行环境的类库,主要是swing的包  在用到swing时最好加上。