## 安装 Elasticsearch

本节包括有关如何设置Elasticsearch并使其运行的信息，包括:

- Downloading
- Installing
- Starting
- Configuring





## Supported platforms

所有版本支持的： [Support Matrix](https://www.elastic.co/support/matrix)







## Java (JVM) Version

Elasticsearch is built using Java, and includes a bundled version of [OpenJDK](https://openjdk.java.net/) from the JDK maintainers (GPLv2+CE) within each distribution. The bundled JVM is the recommended JVM and is located within the `jdk` directory of the Elasticsearch home directory.



To use your own version of Java, set the `ES_JAVA_HOME` environment variable. If you must use a version of Java that is different from the bundled JVM, we recommend using a [supported](https://www.elastic.co/support/matrix) [LTS version of Java](https://www.oracle.com/technetwork/java/eol-135779.html). Elasticsearch will refuse to start if a known-bad version of Java is used. The bundled JVM directory may be removed when using your own JVM.

1. 内置JDK
2. 可以  使用 *ES_JAVA_HOME*  自定义JDK 

