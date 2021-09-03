# JSON Layout

将一系列 JSON 事件附加为序列化为字节的字符串。

# Complete well-formed JSON vs. fragment JSON

如果配置 complete="true"，appender 会输出格式良好的 JSON 文档。默认情况下，使用 complete="false" 时，您应该将输出作为外部文件包含在单独的文件中，以形成格式良好的 JSON 文档。



如果 complete="false"，appender 不会在文档的开头、“]”和结尾写入 JSON 开放数组字符“[”，也不会在记录之间写入逗号“,”。

```xml
{
  "instant" : {
    "epochSecond" : 1493121664,
    "nanoOfSecond" : 118000000
  },
  "thread" : "main",
  "level" : "INFO",
  "loggerName" : "HelloWorld",
  "marker" : {
    "name" : "child",
    "parents" : [ {
      "name" : "parent",
      "parents" : [ {
        "name" : "grandparent"
      } ]
    } ]
  },
  "message" : "Hello, world!",
  "thrown" : {
    "commonElementCount" : 0,
    "message" : "error message",
    "name" : "java.lang.RuntimeException",
    "extendedStackTrace" : [ {
      "class" : "logtest.Main",
      "method" : "main",
      "file" : "Main.java",
      "line" : 29,
      "exact" : true,
      "location" : "classes/",
      "version" : "?"
    } ]
  },
  "contextStack" : [ "one", "two" ],
  "endOfBatch" : false,
  "loggerFqcn" : "org.apache.logging.log4j.spi.AbstractLogger",
  "contextMap" : {
    "bar" : "BAR",
    "foo" : "FOO"
  },
  "threadId" : 1,
  "threadPriority" : 5,
  "source" : {
    "class" : "logtest.Main",
    "method" : "main",
    "file" : "Main.java",
    "line" : 29
  }
}
```

如果 complete="false"，appender 不会在文档的开头、“]”和结尾写入 JSON 开放数组字符“[”，也不会在记录之间写入逗号“,”。



# Pretty vs. compact JSON

compact 属性决定输出是否“pretty”。默认值为“false”，这意味着 appender 使用行尾字符和缩进行来格式化文本。如果 compact="true"，则不使用行尾或缩进，这将导致输出占用更少的空间。当然，消息内容可能包含转义的行尾。



# JsonLayout Parameters

| Parameter Name            | Type    | Description                                                  |
| :------------------------ | :------ | :----------------------------------------------------------- |
| charset                   | String  | The character set to use when converting to a byte array. The value must be a valid [Charset](http://docs.oracle.com/javase/6/docs/api/java/nio/charset/Charset.html). If not specified, UTF-8 will be used. |
| compact                   | boolean | If true, the appender does not use end-of-lines and indentation. Defaults to false. |
| eventEol                  | boolean | If true, the appender appends an end-of-line after each record. Defaults to false. Use with eventEol=true and compact=true to get one record per line. |
| endOfLine                 | String  | If set, overrides the default end-of-line string. E.g. set it to "\n" and use with eventEol=true and compact=true to have one record per line separated by "\n" instead of "\r\n". Defaults to null (i.e. not set). |
| complete                  | boolean | If true, the appender includes the JSON header and footer, and comma between records. Defaults to false. |
| properties                | boolean | If true, the appender includes the thread context map in the generated JSON. Defaults to false. |
| propertiesAsList          | boolean | If true, the thread context map is included as a list of map entry objects, where each entry has a "key" attribute (whose value is the key) and a "value" attribute (whose value is the value). Defaults to false, in which case the thread context map is included as a simple map of key-value pairs. |
| locationInfo              | boolean | If true, the appender includes the location information in the generated JSON. Defaults to false.Generating [location information](https://logging.apache.org/log4j/2.x/manual/layouts.html#LocationInformation) is an expensive operation and may impact performance. Use with caution. |
| includeStacktrace         | boolean | If true, include full stacktrace of any logged [Throwable](http://docs.oracle.com/javase/6/docs/api/java/lang/Throwable.html) (optional, default to true). |
| includeTimeMillis         | boolean | If true, the timeMillis attribute is included in the Json payload instead of the instant. timeMillis will contain the number of milliseconds since midnight, January 1, 1970 UTC. |
| stacktraceAsString        | boolean | Whether to format the stacktrace as a string, and not a nested object (optional, defaults to false). |
| includeNullDelimiter      | boolean | Whether to include NULL byte as delimiter after each event (optional, default to false). |
| objectMessageAsJsonObject | boolean | If true, ObjectMessage is serialized as JSON object to the "message" field of the output log. Defaults to false. |

To include any custom field in the output, use following syntax:

```
  <JsonLayout>    <KeyValuePair key="additionalField1" value="constant value"/>    <KeyValuePair key="additionalField2" value="$${ctx:key}"/>  </JsonLayout>
```

Custom fields are always last, in the order they are declared. The values support [lookups](https://logging.apache.org/log4j/2.x/manual/lookups.html).

Additional [runtime dependencies](https://logging.apache.org/log4j/2.x/runtime-dependencies.html) are required for using JsonLayout.





