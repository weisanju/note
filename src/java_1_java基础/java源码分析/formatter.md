# formatter

## 简介

formatter是一个抽象的基类用于格式化 对 区域 敏感的 地区,主要包括三类

1. 日期
2. 数值
3. 消息(语言)

## 子类必须实现以下三个方法

* *format(Object obj, StringBuffer toAppendTo, FieldPosition pos)*
* *formatToCharacterIterator(Object obj)*
* *parseObject(String source, ParsePosition pos)*

## 子类一般要实现的 工厂方法

*getInstance* //使用currentLocale

*getInstance(Locale)*

或者更具体的工厂方法

*getIntegerInstance, getCurrencyInstance*

## 还应能获取 所有支持的Locale

*public static Locale[] getAvailableLocales()*

子类应该以如下形式的定义 *FieldPosition*的字段

item_FIELD 的形式



# MessageFormat

MessageFormat提供了以自然语言的形式,产生一系列组合的字符串



## messageFormat的语法定义

```
定义: | 二者选一, [] 可选, 

pattern = string | pattern formatElement string

formatElemnt = {argumentindex\[,FormatType]\[,FormatType ]}

formatType = number | date |time |choice

style = short | medium | long | full| integer|currency|precent|SubformatPattern
```

## 一些规定

* pattern用 Java中的 "" 包裹
* 单引号 包裹的不会解析
* 两个连续的单引号可以表示 一个单引号字符
* 花括号必须成对
* 没有结束的 单引号 默认在 pattern末尾加上单引号

* argumentIndex表示的是 非负的整数 0~9,之后解析传递进来的参数,按照这个顺序去解析
* formatType,formatStyle 是用来创建Format实例

## type与style对应的 Java类

| FormatType | FormatStyle        | Subformat Created                                            |
| ---------- | ------------------ | ------------------------------------------------------------ |
| *(none)*   | *(none)*           | `null`                                                       |
| `number`   | *(none)*           | [`NumberFormat.getInstance`](https://docs.oracle.com/javase/7/docs/api/java/text/NumberFormat.html#getInstance(java.util.Locale))`(getLocale())` |
|            | `integer`          | [`NumberFormat.getIntegerInstance`](https://docs.oracle.com/javase/7/docs/api/java/text/NumberFormat.html#getIntegerInstance(java.util.Locale))`(getLocale())` |
|            | `currency`         | [`NumberFormat.getCurrencyInstance`](https://docs.oracle.com/javase/7/docs/api/java/text/NumberFormat.html#getCurrencyInstance(java.util.Locale))`(getLocale())` |
|            | `percent`          | [`NumberFormat.getPercentInstance`](https://docs.oracle.com/javase/7/docs/api/java/text/NumberFormat.html#getPercentInstance(java.util.Locale))`(getLocale())` |
|            | *SubformatPattern* | `new` [`DecimalFormat`](https://docs.oracle.com/javase/7/docs/api/java/text/DecimalFormat.html#DecimalFormat(java.lang.String, java.text.DecimalFormatSymbols))`(subformatPattern,` [`DecimalFormatSymbols.getInstance`](https://docs.oracle.com/javase/7/docs/api/java/text/DecimalFormatSymbols.html#getInstance(java.util.Locale))`(getLocale()))` |
| `date`     | *(none)*           | [`DateFormat.getDateInstance`](https://docs.oracle.com/javase/7/docs/api/java/text/DateFormat.html#getDateInstance(int, java.util.Locale))`(`[`DateFormat.DEFAULT`](https://docs.oracle.com/javase/7/docs/api/java/text/DateFormat.html#DEFAULT)`, getLocale())` |
|            | `short`            | [`DateFormat.getDateInstance`](https://docs.oracle.com/javase/7/docs/api/java/text/DateFormat.html#getDateInstance(int, java.util.Locale))`(`[`DateFormat.SHORT`](https://docs.oracle.com/javase/7/docs/api/java/text/DateFormat.html#SHORT)`, getLocale())` |
|            | `medium`           | [`DateFormat.getDateInstance`](https://docs.oracle.com/javase/7/docs/api/java/text/DateFormat.html#getDateInstance(int, java.util.Locale))`(`[`DateFormat.DEFAULT`](https://docs.oracle.com/javase/7/docs/api/java/text/DateFormat.html#DEFAULT)`, getLocale())` |
|            | `long`             | [`DateFormat.getDateInstance`](https://docs.oracle.com/javase/7/docs/api/java/text/DateFormat.html#getDateInstance(int, java.util.Locale))`(`[`DateFormat.LONG`](https://docs.oracle.com/javase/7/docs/api/java/text/DateFormat.html#LONG)`, getLocale())` |
|            | `full`             | [`DateFormat.getDateInstance`](https://docs.oracle.com/javase/7/docs/api/java/text/DateFormat.html#getDateInstance(int, java.util.Locale))`(`[`DateFormat.FULL`](https://docs.oracle.com/javase/7/docs/api/java/text/DateFormat.html#FULL)`, getLocale())` |
|            | *SubformatPattern* | `new` [`SimpleDateFormat`](https://docs.oracle.com/javase/7/docs/api/java/text/SimpleDateFormat.html#SimpleDateFormat(java.lang.String, java.util.Locale))`(subformatPattern, getLocale())` |
| `time`     | *(none)*           | [`DateFormat.getTimeInstance`](https://docs.oracle.com/javase/7/docs/api/java/text/DateFormat.html#getTimeInstance(int, java.util.Locale))`(`[`DateFormat.DEFAULT`](https://docs.oracle.com/javase/7/docs/api/java/text/DateFormat.html#DEFAULT)`, getLocale())` |
|            | `short`            | [`DateFormat.getTimeInstance`](https://docs.oracle.com/javase/7/docs/api/java/text/DateFormat.html#getTimeInstance(int, java.util.Locale))`(`[`DateFormat.SHORT`](https://docs.oracle.com/javase/7/docs/api/java/text/DateFormat.html#SHORT)`, getLocale())` |
|            | `medium`           | [`DateFormat.getTimeInstance`](https://docs.oracle.com/javase/7/docs/api/java/text/DateFormat.html#getTimeInstance(int, java.util.Locale))`(`[`DateFormat.DEFAULT`](https://docs.oracle.com/javase/7/docs/api/java/text/DateFormat.html#DEFAULT)`, getLocale())` |
|            | `long`             | [`DateFormat.getTimeInstance`](https://docs.oracle.com/javase/7/docs/api/java/text/DateFormat.html#getTimeInstance(int, java.util.Locale))`(`[`DateFormat.LONG`](https://docs.oracle.com/javase/7/docs/api/java/text/DateFormat.html#LONG)`, getLocale())` |
|            | `full`             | [`DateFormat.getTimeInstance`](https://docs.oracle.com/javase/7/docs/api/java/text/DateFormat.html#getTimeInstance(int, java.util.Locale))`(`[`DateFormat.FULL`](https://docs.oracle.com/javase/7/docs/api/java/text/DateFormat.html#FULL)`, getLocale())` |
|            | *SubformatPattern* | `new` [`SimpleDateFormat`](https://docs.oracle.com/javase/7/docs/api/java/text/SimpleDateFormat.html#SimpleDateFormat(java.lang.String, java.util.Locale))`(subformatPattern, getLocale())` |
| `choice`   | *SubformatPattern* | `new` [`ChoiceFormat`](https://docs.oracle.com/javase/7/docs/api/java/text/ChoiceFormat.html#ChoiceFormat(java.lang.String))`(subformatPattern)` |



## example

```java
   String result = MessageFormat.format(
       "At {1,time} on {1,date}, there was {2} on planet {0,number,integer}.",
       planet, new Date(), event);
```

可重用的

```java
   int fileCount = 1273;
   String diskName = "MyDisk";
   Object[] testArgs = {new Long(fileCount), diskName};
  
   MessageFormat form = new MessageFormat(
       "The disk \"{1}\" contains {0} file(s).");
  
   System.out.println(form.format(testArgs));
```

ChoiceFormat

```java
  MessageFormat form = new MessageFormat("The disk \"{1}\" contains {0}.");
   double[] filelimits = {0,1,2};
   String[] filepart = {"no files","one file","{0,number} files"};
   ChoiceFormat fileform = new ChoiceFormat(filelimits, filepart);
   form.setFormatByArgumentIndex(0, fileform);
  
   int fileCount = 1273;
   String diskName = "MyDisk";
   Object[] testArgs = {new Long(fileCount), diskName};
  
   System.out.println(form.format(testArgs));
```

语义化的创建 choice

```java
 form.applyPattern(
    "There {0,choice,0#are no files|1#is one file|1<are {0,number,integer} files}.");
```

