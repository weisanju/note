

# Pattern Layout

使用模式字符串可配置的灵活布局。此类的目标是格式化 LogEvent 并返回结果。结果的格式取决于转换模式。

转换模式与 C 中 printf 函数的转换模式密切相关。转换模式由称为转换说明符的文字文本和格式控制表达式组成。

请注意，任何文字文本，包括特殊字符，都可以包含在转换模式中。特殊字符包括\t、\n、\r、\f。使用 \\ 在输出中插入一个反斜杠。

每个转换说明符都以百分号 (%) 开头，后跟可选的格式修饰符和转换字符。转换字符指定数据的类型，例如类别、优先级、日期、线程名称。格式修饰符控制诸如字段宽度、填充、左右对齐等内容。下面是一个简单的例子。



让转换模式为 "%-5p [%t]: %m%n" 并假设 Log4j 环境设置为使用 PatternLayout。然后声明



```
Logger logger = LogManager.getLogger("MyLogger");
logger.debug("Message 1");
logger.warn("Message 2");

DEBUG [main]: Message 1
WARN  [main]: Message 2
```

请注意，文本和转换说明符之间没有明确的分隔符。模式解析器在读取转换字符时知道它何时到达转换说明符的末尾。在上面的例子中，转换说明符 %-5p 意味着记录事件的优先级应该左对齐到五个字符的宽度。



如果模式字符串不包含处理正在记录的 Throwable 的说明符，则模式的解析将表现为“%xEx”说明符已添加到字符串的末尾。要完全抑制 Throwable 的格式，只需在模式字符串中添加“%ex{0}”作为说明符。

# PatternLayout Parameters

| Parameter Name        | Type             | Description                                                  |
| :-------------------- | :--------------- | :----------------------------------------------------------- |
| charset               | String           | The character set to use when converting the syslog String to a byte array. The String must be a valid [Charset](http://docs.oracle.com/javase/6/docs/api/java/nio/charset/Charset.html). If not specified, this layout uses the platform default character set. |
| pattern               | String           | A composite pattern string of one or more conversion patterns from the table below. Cannot be specified with a PatternSelector. |
| patternSelector       | PatternSelector  | 一个组件，用于分析 LogEvent 中的信息并确定应使用哪种模式来格式化事件。 pattern 和 patternSelector 参数是互斥的 |
| replace               | RegexReplacement | 允许替换部分结果字符串。如果已配置，replace 元素必须指定要匹配的正则表达式和替换。这执行类似于 RegexReplacement 转换器的功能，但适用于整个消息，而转换器仅适用于其模式生成的字符串。 |
| alwaysWriteExceptions | boolean          | If true (it is by default) exceptions are always written even if the pattern contains no exception conversions. This means that if you do not include a way to output exceptions in your pattern, the default exception formatter will be added to the end of the pattern. Setting this to false disables this behavior and allows you to exclude exceptions from your pattern output. |
| header                | String           | The optional header string to include at the top of each log file. |
| footer                | String           | The optional footer string to include at the bottom of each log file. |
| disableAnsi           | boolean          | If true (default is false), do not output ANSI escape codes. |
| noConsoleNoAnsi       | boolean          | If true (default is false) and System.console() is null, do not output ANSI escape codes. |

# RegexReplacement Parameters

| Parameter Name | Type   | Description                                                  |
| :------------- | :----- | :----------------------------------------------------------- |
| regex          | String | A Java-compliant regular expression to match in the resulting string. See [Pattern](http://docs.oracle.com/javase/6/docs/api/java/util/regex/Pattern.html) . |
| replacement    | String | The string to replace any matched sub-strings with.          |

# Pattern表格汇总

| 转换符                                                       | 说明                                                     | 示例                                          |
| ------------------------------------------------------------ | -------------------------------------------------------- | --------------------------------------------- |
| **c**{precision}<br/>**logger**{precision}                   | 日志名                                                   | %c{10}                                        |
| **C**{precision}<br/>**class**{precision}                    | 调用者的全限定类名                                       |                                               |
| **d**{pattern}<br/>**date**{pattern}                         | 日志事件的日期                                           | %d{DEFAULT}、%d{HH:mm:ss,SSS}                 |
| **F**<br/>**file**                                           | 目标输出文件名                                           |                                               |
| **K**{key}<br/>**map**{key}<br/>**MAP**{key}                 | 输出 MapMessage 的 **entries**<br />**%K{clientNumber}** | 多key的输出格式为： {{key1,val1},{key2,val2}} |
| **L**<br/>**line**                                           | 调用者的行号                                             | 耗时操作                                      |
| **m**{nolookups}{ansi}<br/>**msg**{nolookups}{ansi}<br/>**message**{nolookups}{ansi} | 输出程序提供的消息                                       | 两个选项：nolookups、ansi                     |
| **M**<br/>**method**                                         | 发出日志请求的方法名                                     |                                               |
| **marker**                                                   | marker全名包括parentName                                 |                                               |
| **markerSimpleName**                                         | 简单名（不包括parentName）                               |                                               |
| **maxLen**<br/>**maxLength**                                 | 限定日志的内容的最大最小                                 | %maxLen{%m}{20}                               |
| n                                                            | 换行符                                                   |                                               |
| **N**<br/>**nano**                                           | 日志事件产生时间                                         |                                               |
| **pid**{[defaultValue]}<br/>**processId**{[defaultValue]}    | 进程Id                                                   |                                               |
| **p\|level**                                                 | 日志级别                                                 |                                               |
| **x**<br/>**NDC**                                            | NDC                                                      |                                               |
| **X**{key[,key2...]}<br/>**mdc**{key[,key2...]}<br/>**MDC**{key[,key2...]} | MDC                                                      |                                               |
| **u**{"RANDOM"\|"TIME"}<br />**uuid**                        | 随机数                                                   |                                               |
| **T**<br/>**tid**<br/>**threadId**                           | 线程ID                                                   |                                               |
| **t**<br/>**tn**<br/>**thread**<br/>**threadName**           | 线程名                                                   |                                               |



# Patterns详解

The conversions that are provided with Log4j are:

## 输出日志名称

**c**{precision}
**logger**{precision}

| Conversion Pattern | Logger Name                 | Result                 |
| :----------------- | :-------------------------- | :--------------------- |
| %c{1}              | org.apache.commons.Foo      | Foo                    |
| %c{2}              | org.apache.commons.Foo      | commons.Foo            |
| %c{10}             | org.apache.commons.Foo      | org.apache.commons.Foo |
| %c{-1}             | org.apache.commons.Foo      | apache.commons.Foo     |
| %c{-2}             | org.apache.commons.Foo      | commons.Foo            |
| %c{-10}            | org.apache.commons.Foo      | org.apache.commons.Foo |
| %c{1.}             | org.apache.commons.Foo      | o.a.c.Foo              |
| %c{1.1.~.~}        | org.apache.commons.test.Foo | o.a.~.~.Foo            |
| %c{.}              | org.apache.commons.test.Foo | ....Foo                |

## 输出调用类的全限定名

**C**{precision}
**class**{precision}

输出发出日志记录请求的调用者的完全限定类名。此转换说明符可以选择后跟精度说明符，其遵循与记录器名称转换器相同的规则。

生成调用者的类名（位置信息）是一项代价高昂的操作，可能会影响性能。谨慎使用。



## 输出日志事件的日期

**d**{pattern}
**date**{pattern}



输出日志事件的日期。日期转换说明符后面可以跟一组包含每个 SimpleDateFormat 的日期和时间模式字符串的大括号。

| Pattern                            | Example                         |
| :--------------------------------- | :------------------------------ |
| %d{DEFAULT}                        | 2012-11-02 14:34:02,123         |
| %d{DEFAULT_MICROS}                 | 2012-11-02 14:34:02,123456      |
| %d{DEFAULT_NANOS}                  | 2012-11-02 14:34:02,123456789   |
| %d{ISO8601}                        | 2012-11-02T14:34:02,781         |
| %d{ISO8601_BASIC}                  | 20121102T143402,781             |
| %d{ISO8601_OFFSET_DATE_TIME_HH}    | 2012-11-02'T'14:34:02,781-07    |
| %d{ISO8601_OFFSET_DATE_TIME_HHMM}  | 2012-11-02'T'14:34:02,781-0700  |
| %d{ISO8601_OFFSET_DATE_TIME_HHCMM} | 2012-11-02'T'14:34:02,781-07:00 |
| %d{ABSOLUTE}                       | 14:34:02,781                    |
| %d{ABSOLUTE_MICROS}                | 14:34:02,123456                 |
| %d{ABSOLUTE_NANOS}                 | 14:34:02,123456789              |
| %d{DATE}                           | 02 Nov 2012 14:34:02,781        |
| %d{COMPACT}                        | 20121102143402781               |
| %d{UNIX}                           | 1351866842                      |
| %d{UNIX_MILLIS}                    | 1351866842781                   |

您还可以使用一组包含每个 java.util.TimeZone.getTimeZone 的时区 ID 的大括号。如果未给出日期格式说明符，则使用 DEFAULT 格式。

### 预定义格式说明符

| Pattern                                                      | Example                                                     |
| :----------------------------------------------------------- | :---------------------------------------------------------- |
| %d{HH:mm:ss,SSS}                                             | 14:34:02,123                                                |
| %d{HH:mm:ss,nnnn} to %d{HH:mm:ss,nnnnnnnnn}                  | 14:34:02,1234 to 14:34:02,123456789                         |
| %d{dd MMM yyyy HH:mm:ss,SSS}                                 | 02 Nov 2012 14:34:02,123                                    |
| %d{dd MMM yyyy HH:mm:ss,nnnn} to %d{dd MMM yyyy HH:mm:ss,nnnnnnnnn} | 02 Nov 2012 14:34:02,1234 to 02 Nov 2012 14:34:02,123456789 |
| %d{HH:mm:ss}{GMT+0}                                          | 18:34:02                                                    |



%d{UNIX} 以秒为单位输出 UNIX 时间。

 %d{UNIX_MILLIS} 以毫秒为单位输出 UNIX 时间。 

UNIX 时间是当前时间与 UTC 时间 1970 年 1 月 1 日午夜之间的差异，UNIX 以秒为单位，UNIX_MILLIS 以毫秒为单位。虽然时间单位是毫秒，但粒度取决于操作系统 (Windows)。这是输出事件时间的有效方法，因为只发生从 long 到 String 的转换，不涉及日期格式。

在 Java 9 上运行时，Log4j 2.11 添加了对比毫秒更精确的时间戳的有限支持。请注意，并非所有 DateTimeFormatter 格式都受支持。只有上表中提到的格式的时间戳可以使用“nano-of-second”模式字母 n 而不是“fraction-of-second”模式字母 S。

## 编码和转义适合以特定标记语言输出的特殊字符。

**enc**{*pattern*}{[HTML|XML|JSON|CRLF]}
**encode**{*pattern*}{[HTML|XML|JSON|CRLF]}

默认情况下，如果只指定了一个选项，则此编码为 HTML。

第二个选项用于指定应使用哪种编码格式。该转换器对于对用户提供的数据进行编码特别有用，这样输出数据就不会被错误地或不安全地写入。

典型的用法是对消息 %enc{%m} 进行编码，但用户输入也可能来自其他位置，例如 MDC %enc{%mdc{key}}



使用HTML编码格式，替换如下字符：

| Character        | Replacement                                                 |
| :--------------- | :---------------------------------------------------------- |
| '\r', '\n'       | Converted into escaped strings "\\r" and "\\n" respectively |
| &, <, >, ", ', / | Replaced with the corresponding HTML entity                 |

使用 XML 编码格式，这遵循 XML 规范指定的转义规则：

| Character     | Replacement                                |
| :------------ | :----------------------------------------- |
| &, <, >, ", ' | Replaced with the corresponding XML entity |

使用 JSON 编码格式，这遵循 [RFC 4627 第 2.5 节](https://www.ietf.org/rfc/rfc4627.txt)指定的转义规则：

| Character                    | Replacement                                           |
| :--------------------------- | :---------------------------------------------------- |
| U+0000 - U+001F              | \u0000 - \u001F                                       |
| Any other control characters | Encoded into its \uABCD equivalent escaped code point |
| "                            | \"                                                    |
| \                            | \\                                                    |

例如，模式 {"message": "%enc{%m}{JSON}"} 可用于输出包含作为字符串值的日志消息的有效 JSON 文档。使用CRLF编码格式，替换如下字符：



| Character  | Replacement                                                 |
| :--------- | :---------------------------------------------------------- |
| '\r', '\n' | Converted into escaped strings "\\r" and "\\n" respectively |



## 等值替换

**equals**{pattern}{test}{substitution}
**equalsIgnoreCase**{pattern}{test}{substitution}



将字符串中出现的 'test' 替换为由模式评估产生的字符串中的替换 'substitution'。

例如，"%equals{[%marker]}{[]}{}" 将用空字符串替换由没有标记的事件产生的 '[]' 字符串。模式可以是任意复杂的，特别是可以包含多个转换关键字。



## Throwable

**ex**|**exception**|**throwable**
{
 [ "none"
  | "full"
  | depth
  | "short"
  | "short.className"
  | "short.fileName"
  | "short.lineNumber"
  | "short.methodName"
  | "short.message"
  | "short.localizedMessage"]
}
 {filters(package,package,...)}
 {suffix(*pattern*)}
 {separator(*separator*)}

默认情况下，这将输出完整的跟踪，就像通常通过调用 Throwable.printStackTrace() 找到的那样。

您可以使用 %throwable{option} 形式的选项跟随 throwable 转换词。

`%throwable{short}` outputs the first line of the Throwable.

**%throwable{short.className}** outputs the name of the class where the exception occurred.

**%throwable{short.methodName}** outputs the method name where the exception occurred.

**%throwable{short.fileName}** outputs the name of the class where the exception occurred.

**%throwable{short.lineNumber}** outputs the line number where the exception occurred.

**%throwable{short.message}** outputs the message.

**%throwable{short.localizedMessage}** outputs the localized message.

**%throwable{n}** outputs the first n lines of the stack trace.

**%throwable{none}** or **%throwable{0}** suppresses output of the exception.

使用 {filters(packages)} ，其中包是包名称列表，以抑制来自堆栈跟踪的匹配堆栈帧。使用 {suffix(pattern)} 在每个堆栈帧的末尾添加模式的输出。使用 {separator(...)} 作为行尾字符串。例如：分隔符(|)。默认值是 line.separator 系统属性，它依赖于操作系统。



## 输出发出日志记录请求的文件名。

**F**
**file**

生成文件信息（位置信息）是一项昂贵的操作，可能会影响性能。谨慎使用。

## 根据当前事件的日志记录级别将 ANSI 颜色添加到封闭模式的结果中。

The default colors for each level are:

| Level | ANSI color              |
| :---- | :---------------------- |
| FATAL | Bright red              |
| ERROR | Bright red              |
| WARN  | Yellow                  |
| INFO  | Green                   |
| DEBUG | Cyan                    |
| TRACE | Black (looks dark grey) |

The color and attribute names and are standard, but the exact shade, hue, or value.

| Intensity Code | 0     | 1    | 2     | 3      | 4    | 5       | 6    | 7     |
| :------------- | :---- | :--- | :---- | :----- | :--- | :------ | :--- | :---- |
| Normal         | Black | Red  | Green | Yellow | Blue | Magenta | Cyan | White |
| Bright         | Black | Red  | Green | Yellow | Blue | Magenta | Cyan | White |

You can use the default colors with:

```
%highlight{%d [%t] %-5level: %msg%n%throwable}
```

You can override the default colors in the optional {style} option. For example:

```
%highlight{%d [%t] %-5level: %msg%n%throwable}{FATAL=white, ERROR=red, WARN=blue, INFO=black, DEBUG=green, TRACE=blue}
```

You can highlight only the a portion of the log event:

```
%d [%t] %highlight{%-5level: %msg%n%throwable}
```

You can style one part of the message and highlight the rest the log event:

```
%style{%d [%t]}{black} %highlight{%-5level: %msg%n%throwable}
```

You can also use the STYLE key to use a predefined group of colors:

```
%highlight{%d [%t] %-5level: %msg%n%throwable}{STYLE=Logback}
```

The STYLE value can be one of:

| Style   | Description        |
| :------ | :----------------- |
| Default | See above          |
| Logback | logback desciption |

**logback desciption**

| Level | ANSI color          |
| :---- | :------------------ |
| FATAL | Blinking bright red |
| ERROR | Bright red          |
| WARN  | Red                 |
| INFO  | Blue                |
| DEBUG | Normal              |
| TRACE | Normal              |

## Outputs the entries in a [MapMessage](https://logging.apache.org/log4j/2.x/log4j-api/apidocs/org/apache/logging/log4j/message/MapMessage.html)



**K**{key}
**map**{key}
**MAP**{key}

如果没有指定额外的子选项，则使用格式 {{key1,val1},{key2,val2}} 输出 Map 键值对集的全部内容



## location information

**l**
**location**

位置信息取决于 JVM 实现，但通常由调用方法的完全限定名称和调用者源文件名和括号之间的行号组成。

Generating [location information](https://logging.apache.org/log4j/2.x/manual/layouts.html#LocationInformation) is an expensive operation and may impact performance. Use with caution.



## line number

**L**
**line**

Outputs the line number from where the logging request was issued.

Generating line number information ([location information](https://logging.apache.org/log4j/2.x/manual/layouts.html#LocationInformation)) is an expensive operation and may impact performance. Use with caution.



## application supplied message

**m**{nolookups}{ansi}
**msg**{nolookups}{ansi}
**message**{nolookups}{ansi}





Add {ansi} to render messages with ANSI escape codes (requires JAnsi, see [configuration](https://logging.apache.org/log4j/2.x/manual/layouts.html#enable-jansi).)

The default syntax for embedded ANSI codes is:

```
@|code(,code)* text|@
```

For example, to render the message "Hello" in green, use:

```
@|green Hello|@
```

To render the message "Hello" in bold and red, use:

```
@|bold,red Warning!|@
```

You can also define custom style names in the configuration with the syntax:

```
%message{ansi}{StyleName=value(,value)*( StyleName=value(,value)*)*}%n
```

For example:

```
%message{ansi}{WarningStyle=red,bold KeyStyle=white ValueStyle=blue}%n
```

The call site can look like this:

```
logger.info("@|KeyStyle {}|@ = @|ValueStyle {}|@", entry.getKey(), entry.getValue());
```

Use {nolookups} to log messages like "${date:YYYY-MM-dd}" without using any lookups. Normally calling logger.info("Try ${date:YYYY-MM-dd}") would replace the date template ${date:YYYY-MM-dd} with an actual date. Using nolookups disables this feature and logs the message string untouched.



## 调用方法名

**M**
**method**

生成调用者的方法名称（位置信息）是一项代价高昂的操作，可能会影响性能。谨慎使用。

## **marker**

The full name of the marker, including parents, if one is present.

## **markerSimpleName**

The simple name of the marker (not including parents), if one is present



## 限制输出内容

**maxLen**
**maxLength**



如果长度大于 20，则输出将包含尾随省略号。如果提供的长度无效，则使用默认值 100。示例语法：%maxLen{%p: %c{1} - %m%notEmpty{ =>%ex{short}}}{160} 将被限制为 160 个字符，并带有尾随省略号。

另一个示例： %maxLen{%m}{20} 将被限制为 20 个字符并且没有尾随省略号。

## 平台无关的换行符

n

## log event cratedTime

**N**
**nano**

Outputs the result of System.nanoTime() at the time the log event was created.



## 进程ID

**pid**{[defaultValue]}
**processId**{[defaultValue]}

Outputs the process ID if supported by the underlying platform. An optional default value may be specified to be shown if the platform does not support process IDs.

## 变量存在则打印

Outputs the result of evaluating the pattern if and only if all variables in the pattern are not empty.

```
%notEmpty{[%marker]}
```

## 日志级别

**p**|**level**{*level*=*label*, *level*=*label*, ...} 

**p**|**level**{length=*n*} 

**p**|**level**{lowerCase=*true*|*false*}



输出日志事件的级别。您以“级别=值，级别=值”的形式提供级别名称映射，其中级别是级别的名称，值是应显示的值而不是级别的名称。

```
%level{WARN=Warning, DEBUG=Debug, ERROR=Error, TRACE=Trace, INFO=Info}

```

Alternatively, for the compact-minded:

```
%level{WARN=W, DEBUG=D, ERROR=E, TRACE=T, INFO=I}
```

更简洁地说，对于与上面相同的结果，您可以定义级别标签的长度：

```
%level{length=1}
```

如果长度大于级别名称长度，则布局使用普通级别名称。

You can combine the two kinds of options:

```
%level{ERROR=Error, length=2}
```

这将为您提供错误级别名称和长度为 2 的所有其他级别名称。

您可以输出小写级别的名称（默认为大写）：

```
%level{lowerCase=true}
```

## 从**JVM**启动时  日志事件的创建时间

**r**
**relative**



Outputs the number of milliseconds elapsed since the JVM was started until the creation of the logging event.

## 内容替换

replace{pattern}{regex}{substitution}	



用它在模式评估产生的字符串中的替换 'substitution' 替换正则表达式 'regex' 的出现。例如，“%replace{%msg}{\s}{}”将删除事件消息中包含的所有空格

模式可以是任意复杂的，特别是可以包含多个转换关键字。例如，“%replace{%logger %msg}{\.}{/}”将用正斜杠替换记录器或事件消息中的所有点。



## RThrowable

**rEx**|**rException**|**rThrowable**
 {
  ["none" | "short" | "full" | depth]
  [,filters(package,package,...)]
  [,separator(*separator*)]
 }
 {ansi(
  Key=Value,Value,...
  Key=Value,Value,...
  ...)
 }
 {suffix(*pattern*)}





同 %throwable 转换字

但是堆栈跟踪从抛出的第一个异常开始打印，然后是每个后续的包装异常

**%rEx{short}** 

输出异常栈的第一行

**%rEx{n}**

输出栈的前几行

**%rEx{none} or %rEx{0}** 

禁用异常打印

**抑制栈帧**

 **filters(*packages*)** 

*packages* 是一个包名称列表，用于抑制来自堆栈跟踪的匹配堆栈帧。

**separator(*separator*)**

使用分隔符字符串来分隔堆栈跟踪的行。例如：分隔符(|)。默认值是 line.separator 系统属性，它依赖于操作系统。



**rEx{suffix(*pattern*)** 

to add the output of *pattern* to the output only when there is a throwable to print.

## 日志事件自增序列

包括将在每个事件中递增的序列号。计数器是一个静态变量，因此仅在共享相同转换器类对象的应用程序中是唯一的。

## 线程ID

**T**
**tid**
**threadId**

Outputs the ID of the thread that generated the logging event.

## 线程名

**t**
**tn**
**thread**
**threadName**



## 线程优先级

**tp**
**threadPriority**



## logger的全限定类名

**fqcn**

Outputs the fully qualified class name of the logger.

## EndOfBatch 

Outputs the EndOfBatch status of the logging event, as "true" or "false".

## NDC

**x**
**NDC**

Outputs the Thread Context Stack (also known as the Nested Diagnostic Context or NDC) associated with the thread that generated the logging event.

## MDC

**X**{key[,key2...]}
**mdc**{key[,key2...]}
**MDC**{key[,key2...]}





Outputs the Thread Context Map (also known as the Mapped Diagnostic Context or MDC) associated with the thread that generated the logging event. 

%X{clientNumber}

%X{name, number} using the format {name=val1, number=val2}

如果未指定子选项，则使用格式 {key1=val1, key2=val2} 输出 MDC 键值对集的全部内容。键/值对将按排序顺序打印。

See the [ThreadContext](https://logging.apache.org/log4j/2.x/log4j-api/apidocs/org/apache/logging/log4j/ThreadContext.html) class for more details.



## 随机值

**u**{"RANDOM" | "TIME"}
**uuid**



包括随机或基于时间的 UUID。

The time-based UUID is a Type 1 UUID 每毫秒最多可以生成 10,000 个唯一 ID，

将使用每个主机的 MAC 地址，

并尝试确保同一主机上多个 JVM 和/或类加载器的唯一性

 到 16,384 之间的随机数将与 UUID 生成器类的每个实例相关联，并包含在生成的每个基于时间的 UUID 中。

由于基于时间的 UUID 包含 MAC 地址和时间戳，因此应谨慎使用，因为它们可能导致安全漏洞。



# 格式修饰符

默认情况下，相关信息按原样输出。但是，借助格式修饰符，可以更改最小字段宽度、最大字段宽度和对齐方式。

可选的格式修饰符位于百分号和转换字符之间。

第一个可选的格式修饰符是*左对齐标志*，它只是减号 (-) 字符

然后是可选的 *minimum field width* 修饰符

这是一个十进制常量，表示要输出的最小字符数。

如果数据项为较少的字符，则在左侧或右侧进行填充，直到达到最小宽度

默认是在左侧填充（右对齐），但您可以使用左对齐标志指定右填充。

填充字符是空格

如果数据项大于最小字段宽度，则扩展字段以容纳数据该值永远不会被截断

要使用零作为填充字符，请在 *minimum field width* 前面加上零。

可以使用 *maximum field width* 修饰符更改此行为，该修饰符由句点后跟十进制常量指定。

如果数据项比最大字段长，则从数据项的*开头*而不是结尾删除多余的字符。

例如，最大字段宽度为 8，数据项长度为 10 个字符，则删除数据项的前两个字符。

此行为不同于 C 中的 printf 函数，其中从末尾开始截断。

通过在句点后附加一个减号，可以从末尾截断。

在这种情况下，如果最大字段宽度为 8 且数据项长度为 10 个字符，则删除数据项的最后两个字符。

以下是类别转换说明符的各种格式修饰符示例。

Pattern Converters

| Format modifier | left justify | minimum width | maximum width | comment                                                      |
| :-------------- | :----------- | :------------ | :------------ | :----------------------------------------------------------- |
| %20c            | false        | 20            | none          | Left pad with spaces if the category name is less than 20 characters long. |
| %-20c           | true         | 20            | none          | Right pad with spaces if the category name is less than 20 characters long. |
| %.30c           | NA           | none          | 30            | Truncate from the beginning if the category name is longer than 30 characters. |
| %20.30c         | false        | 20            | 30            | Left pad with spaces if the category name is shorter than 20 characters. However, if category name is longer than 30 characters, then truncate from the beginning. |
| %-20.30c        | true         | 20            | 30            | Right pad with spaces if the category name is shorter than 20 characters. However, if category name is longer than 30 characters, then truncate from the beginning. |
| %-20.-30c       | true         | 20            | 30            | Right pad with spaces if the category name is shorter than 20 characters. However, if category name is longer than 30 characters, then truncate from the end. |

