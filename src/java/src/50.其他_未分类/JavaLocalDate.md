# chrono

chrono包提供历法相关的接口与实现。Java中默认使用的历法是ISO 8601日历系统，它是世界民用历法，也就是我们所说的公历。平年有365天，闰年是366天。闰年的定义是：非世纪年，能被4整除；世纪年能被400整除。为了计算的一致性，公元1年的前一年被当做公元0年，以此类推。此外chrono包提供了四种其他历法，每种历法有自己的纪元（Era）类、日历类和日期类，分别是：

- 泰国佛教历：ThaiBuddhistEra、ThaiBuddhistChronology和ThaiBuddhistDate；
- 民国历：MinguoEra、MinguoChronology和MinguoDate；
- 日本历：JapaneseEra、JapaneseChronology和JapaneseDate
- 伊斯兰历：HijrahEra、HijrahChronology和HijrahDate：



# format

format包提供了日期格式化的方法。format包中定义了时区名称、日期解析和格式化的各种枚举，以及最为重要的格式化类DateTimeFormatter。需要注意的是，format包类中的类都是final的，都提供了线程安全的访问。在DateTimeFormatter类中提供了ofPattern的静态方法来获得一个DateTimeFormatter，但细看其实现，其实还是调用的DateTimeFormatterBuilder的静态方法：`DateTimeFormatterBuilder.appendPattern(pattern).toFormatter();`所以我们在实际格式化日期和时间的时候，是两种方式都可以使用的。



# temporal

temporal包中定义了整个日期时间框架的基础：各种时间单位、时间调节器，以及在年月日时分秒中用到的各种属性。Java8中的日期时间类都是实现了temporal包中的时间单位（Temporal）、时间调节器（TemporalAdjuster）和各种属性的接口，所以在后面的日期的操作方法中都是以最基本的时间单位和各种属性为参数的。



# zone

定义了时区转换的各种方法。







# Java 8日期/时间类

## Instant

* 时间戳

* Instant可以精确到纳秒,这超过了long的最大表示范围,实现中是分成了两部分来表示，一部分是`seconds`，表示从1970-01-01 00:00:00开始到现在的秒数，另一个部分是`nanos`，表示纳秒部分

* 创建方式

  ```java
  Instant now = Instant.now(); 
  Instant instant = Instant.ofEpochSecond(60, 100000);
  ```

## Duration

* Duration是两个时间戳的差值

* 包含两部分：`seconds`表示秒，`nanos`表示纳秒

```java
LocalDateTime from = LocalDateTime.of(2020, Month.JANUARY, 22, 16, 6, 0);    // 2020-01-22 16:06:00
LocalDateTime to = LocalDateTime.of(2020, Month.FEBRUARY, 22, 16, 6, 0);     // 2020-02-22 16:06:00
Duration duration = Duration.between(from, to);     // 表示从 2020-01-22 16:06:00到 2020-02-22 16:06:00 这段时间
```

```java
Duration duration1 = Duration.of(5, ChronoUnit.DAYS);       // 5天
Duration duration2 = Duration.of(1000, ChronoUnit.MILLIS);  // 1000毫秒
```



## Period

* 以年月日来衡量一个时间段 (比如1年2个月3天：`Period period = Period.of(1, 2, 3);` )

  ```java
  Period period = Period.between(
                  LocalDate.of(2020, 1, 22),
                  LocalDate.of(2020, 2, 22));
  ```

#### LocalDate/LocalTime/LocalDateTime

**简单的日期操作**

> 简单的日期操作，比如增加、减少一天、修改年月日等

```java
LocalDate date = LocalDate.of(2020, 2, 22);          // 2020-02-22
LocalDate date1 = date.withYear(2021);              // 修改为 2021-02-22
LocalDate date2 = date.withMonth(3);                // 修改为 2020-03-22
LocalDate date3 = date.withDayOfMonth(1);           // 修改为 2020-02-01
LocalDate date4 = date.plusYears(1);                // 增加一年 2021-02-22
LocalDate date5 = date.minusMonths(2);              // 减少两个月，到2019年的12月  2019-12-22
LocalDate date6 = date.plus(5, ChronoUnit.DAYS);    // 增加5天 2020-02-27
```

**复杂的日期操作**

>  比较复杂的日期操作，比如将时间调到下一个工作日，或者是下个月的最后一天，这时候我们可以使用with()方法的另一个重载方法，它接收一个TemporalAdjuster参数，可以使我们更加灵活的调整日期



```java
LocalDate date7 = date.with(TemporalAdjusters.nextOrSame(DayOfWeek.SUNDAY));      // 返回下一个距离当前时间最近的星期日 2020-02-23
LocalDate date9 = date.with(TemporalAdjusters.lastInMonth(DayOfWeek.SATURDAY));  // 返回本月最后一个周六 2020-02-29
```



`TemporalAdjuster`API

| 方法名                      | 描述                                                        |
| --------------------------- | ----------------------------------------------------------- |
| dayOfWeekInMonth            | 返回同一个月中每周的第几天                                  |
| firstDayOfMonth             | 返回当月的第一天                                            |
| firstDayOfNextMonth         | 返回下月的第一天                                            |
| firstDayOfNextYear          | 返回下一年的第一天                                          |
| firstDayOfYear              | 返回本年的第一天                                            |
| firstInMonth                | 返回同一个月中第一个星期几                                  |
| lastDayOfMonth              | 返回当月的最后一天                                          |
| lastDayOfNextMonth          | 返回下月的最后一天                                          |
| lastDayOfNextYear           | 返回下一年的最后一天                                        |
| lastDayOfYear               | 返回本年的最后一天                                          |
| lastInMonth                 | 返回同一个月中最后一个星期几                                |
| next / previous             | 返回后一个/前一个给定的星期几                               |
| nextOrSame / previousOrSame | 返回后一个/前一个给定的星期几，如果这个值满足条件，直接返回 |



## 时区

```java
//根据字符串获取时区
ZoneId shanghaiZoneId = ZoneId.of("Asia/Shanghai");
//获取系统默认时区
ZoneId systemZoneId = ZoneId.systemDefault();
//获取时区字符串
Set<String> zoneIds = ZoneId.getAvailableZoneIds();
//新旧时区转换
ZoneId oldToNewZoneId = TimeZone.getDefault().toZoneId();

//转换ZonedDateTime 对象
LocalDateTime localDateTime = LocalDateTime.now();
ZonedDateTime zonedDateTime = ZonedDateTime.of(localDateTime, shanghaiZoneId);
```

ZonedDateTime对象由两部分构成，LocalDateTime和ZoneId，其中2020-02-22T16:50:54.658部分为LocalDateTime，+08:00[Asia/Shanghai]部分为ZoneId。另一种表示时区的方式是使用ZoneOffset，它是以当前时间和世界标准时间（UTC）/格林威治时间（GMT）的偏差来计算，例如：

```java
ZoneOffset zoneOffset = ZoneOffset.of("+09:00"); 
LocalDateTime localDateTime = LocalDateTime.now(); 
OffsetDateTime offsetDateTime = OffsetDateTime.of(localDateTime, zoneOffset);
```





