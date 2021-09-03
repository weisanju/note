# HTML Layout

The HtmlLayout generates an HTML page and adds each LogEvent to a row in a table.

# HtmlLayout Parameters

| Parameter Name | Type    | Description                                                  |
| :------------- | :------ | :----------------------------------------------------------- |
| charset        | String  | The character set to use when converting the HTML String to a byte array. The value must be a valid [Charset](http://docs.oracle.com/javase/6/docs/api/java/nio/charset/Charset.html). If not specified, this layout uses UTF-8. |
| contentType    | String  | The value to assign to the Content-Type header. The default is "text/html". |
| locationInfo   | boolean | If true, the filename and line number will be included in the HTML output. The default value is false.Generating [location information](https://logging.apache.org/log4j/2.x/manual/layouts.html#LocationInformation) is an expensive operation and may impact performance. Use with caution. |
| title          | String  | A String that will appear as the HTML title.                 |
| fontName       | String  | The font-family to use. The default is "arial,sans-serif".   |
| fontSize       | String  | The font-size to use. The default is "small".                |
| datePattern    | String  | The date format of the logging event. The default is "JVM_ELAPSE_TIME", which outputs the milliseconds since JVM started. For other valid values, refer to the [date pattern](https://logging.apache.org/log4j/2.x/manual/layouts.html#PatternDate) of PatternLayout. |
| timezone       | String  | The timezone id of the logging event. If not specified, this layout uses the [java.util.TimeZone.getDefault](http://docs.oracle.com/javase/6/docs/api/java/util/TimeZone.html#getDefault()) as default timezone. Like [date pattern](https://logging.apache.org/log4j/2.x/manual/layouts.html#PatternDate) of PatternLayout, you can use timezone id from [java.util.TimeZone.getTimeZone](http://docs.oracle.com/javase/6/docs/api/java/util/TimeZone.html#getTimeZone(java.lang.String)). |

Configure as follows to use dataPattern and timezone in HtmlLayout:

```xml
<Appenders>
  <Console name="console">
    <HtmlLayout datePattern="ISO8601" timezone="GMT+0"/>
  </Console>
</Appenders>
```







