# CSV Layouts

此布局创建逗号分隔值 (CSV) 记录并需要 Apache Commons CSV 1.4。



可以通过两种方式使用 CSV 布局：

1. 使用 CsvParameterLayout 记录事件参数以创建自定义数据库
2. using CsvParameterLayout to log event parameters to create a custom database, usually to a logger and file appender uniquely configured for this purpose.
3. 其次，使用 CsvLogEventLayout 记录事件以创建数据库，作为使用完整 DBMS 或使用支持 CSV 格式的 JDBC 驱动程序的替代方法。

CsvParameterLayout 将事件的参数转换为 CSV 记录，忽略消息。要记录 CSV 记录，您可以使用常用的 Logger 方法 info()、debug() 等：

```
logger.info("Ignored", value1, value2, value3);
```

Which will create the CSV record:

```
value1, value2, value3
```



或者，您可以使用仅携带参数的 ObjectArrayMessage：

```
logger.info(new ObjectArrayMessage(value1, value2, value3));
```



CsvParameterLayout and CsvLogEventLayout

| Parameter Name  | Type                                                  | Description                                                  |
| :-------------- | :---------------------------------------------------- | :----------------------------------------------------------- |
| format          | String                                                | One of the predefined formats: Default, Excel, MySQL, RFC4180, TDF. See [CSVFormat.Predefined](https://commons.apache.org/proper/commons-csv/archives/1.4/apidocs/org/apache/commons/csv/CSVFormat.Predefined.html). |
| delimiter       | Character                                             | 分隔符                                                       |
| escape          | Character                                             | 转义字符                                                     |
| quote           | Character                                             | quoteChar                                                    |
| quoteMode       | String                                                | Sets the output quote policy of the format to the specified value. One of: ALL, MINIMAL, NON_NUMERIC, NONE. |
| nullString      | String                                                | Writes null as the given nullString when writing records.    |
| recordSeparator | String                                                | Sets the record separator of the format to the specified String. |
| charset         | Charset                                               | The output Charset.                                          |
| header          | Sets the header to include when the stream is opened. | Desc.                                                        |
| footer          | Sets the footer to include when the stream is closed. | Desc.                                                        |

Logging as a CSV events looks like this:

```
logger.debug("one={}, two={}, three={}", 1, 2, 3);
```

生成包含以下字段的 CSV 记录：

1. Time Nanos
2. Time Millis
3. Level
4. Thread ID
5. Thread Name
6. Thread Priority
7. Formatted Message
8. Logger FQCN
9. Logger Name
10. Marker
11. Thrown Proxy
12. Source
13. Context Map
14. Context Stack

```
0,1441617184044,DEBUG,main,"one=1, two=2, three=3",org.apache.logging.log4j.spi.AbstractLogger,,,,org.apache.logging.log4j.core.layout.CsvLogEventLayoutTest.testLayout(CsvLogEventLayoutTest.java:98),{},[]
```

Additional [runtime dependencies](https://logging.apache.org/log4j/2.x/runtime-dependencies.html) are required for using CSV layouts.

