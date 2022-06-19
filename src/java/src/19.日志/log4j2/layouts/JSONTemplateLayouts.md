# JSON Template Layout

JsonTemplateLayout 是一种可定制、高效且无垃圾的 JSON  emitting layout。它根据提供的 JSON 模板描述的结构对 LogEvents 进行编码。

例如，给定以下 JSON 模板建模官方 Logstash JSONEventLayoutV1

```json
{
  "mdc": {
    "$resolver": "mdc"
  },
  "exception": {
    "exception_class": {
      "$resolver": "exception",
      "field": "className"
    },
    "exception_message": {
      "$resolver": "exception",
      "field": "message",
      "stringified": true
    },
    "stacktrace": {
      "$resolver": "exception",
      "field": "stackTrace",
      "stringified": true
    }
  },
  "line_number": {
    "$resolver": "source",
    "field": "lineNumber"
  },
  "class": {
    "$resolver": "source",
    "field": "className"
  },
  "@version": 1,
  "source_host": "${hostName}",
  "message": {
    "$resolver": "message",
    "stringified": true
  },
  "thread_name": {
    "$resolver": "thread",
    "field": "name"
  },
  "@timestamp": {
    "$resolver": "timestamp"
  },
  "level": {
    "$resolver": "level",
    "field": "name"
  },
  "file": {
    "$resolver": "source",
    "field": "fileName"
  },
  "method": {
    "$resolver": "source",
    "field": "methodName"
  },
  "logger_name": {
    "$resolver": "logger",
    "field": "name"
  }
}
```

in combination with the below Log4j configuration:

```
<JsonTemplateLayout eventTemplateUri="classpath:LogstashJsonEventLayoutV1.json"/>

```

JSON Template Layout will render JSON documents as follows:

```json
{
  "exception": {
    "exception_class": "java.lang.RuntimeException",
    "exception_message": "test",
    "stacktrace": "java.lang.RuntimeException: test\n\tat org.apache.logging.log4j.JsonTemplateLayoutDemo.main(JsonTemplateLayoutDemo.java:11)\n"
  },
  "line_number": 12,
  "class": "org.apache.logging.log4j.JsonTemplateLayoutDemo",
  "@version": 1,
  "source_host": "varlik",
  "message": "Hello, error!",
  "thread_name": "main",
  "@timestamp": "2017-05-25T19:56:23.370+02:00",
  "level": "ERROR",
  "file": "JsonTemplateLayoutDemo.java",
  "method": "main",
  "logger_name": "org.apache.logging.log4j.JsonTemplateLayoutDemo"
}
```

# 