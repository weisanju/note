
# 简介

* *springEL* 是一个强大的表达式语言, 支持在运行时查询,修改对象图
* 与其他 *Unified EL* 相比提供其他特性,最值得注意的是 方法调用 字符串模板

# springEL的特性

- Literal expressions 字面量表达式
- Boolean and relational operators 布尔操作
- Regular expressions 正则
- Class expressions 类表达式
- Accessing properties, arrays, lists, and maps 访问 集合。数组，列表，map
- Method invocation 方法调用
- Relational operators 关联操作
- Assignment 赋值
- Calling constructors 调用构造
- Bean references bean引用
- Array construction 数组构造
- Inline lists  内联list
- Inline maps 内联map
- Ternary operator 三目表达式
- Variables 变量
- User-defined functions 用户定义函数
- Collection projection 集合投影
- Collection selection 集合选择
- Templated expressions 模板表达式



# Evaluation

## 简介

**解析字符串字面量**

```java
ExpressionParser parser = new SpelExpressionParser();
Expression exp = parser.parseExpression("'Hello World'"); 
String message = (String) exp.getValue();
```

* 最可能会使用的 SpEL 类与接口 位于 `org.springframework.expression`  包下
*  `ExpressionParser` 接口 负责 表达式解析，在上面的例子中，表达式是 一个被单引号引起来的字符串字面量
* `Expression`  接口 负责 对定义的表达式 求值，调用 `parser.parseExpression`  会抛出`ParseException` ，调用`exp.getValue`  会抛出`EvaluationException`,
* SpEL支持 很多特性，例如 方法调用，访问属性，调用构造函数



**字符串的 *concat***

```java
ExpressionParser parser = new SpelExpressionParser();
Expression exp = parser.parseExpression("'Hello World'.concat('!')"); 
String message = (String) exp.getValue();
```

**字符串 的 *getBytes***

```java
ExpressionParser parser = new SpelExpressionParser();

// invokes 'getBytes()'
Expression exp = parser.parseExpression("'Hello World'.bytes"); 
byte[] bytes = (byte[]) exp.getValue();
```

**bytes.length**

```java
ExpressionParser parser = new SpelExpressionParser();

// invokes 'getBytes().length'
Expression exp = parser.parseExpression("'Hello World'.bytes.length"); 
int length = (Integer) exp.getValue();
```

**使用 Class类指定返回值**

```java
ExpressionParser parser = new SpelExpressionParser();
Expression exp = parser.parseExpression("new String('hello world').toUpperCase()"); 
String message = exp.getValue(String.class);
```



* 使用泛型方法 求值，`public <T> T getValue(Class<T> desiredResultType)`，可以自动类型转换

* 使用注册的 Converter ，类型转换仍然失败 会抛出 `EvaluationException`

* SpEL的更常见用法是提供一个针对特定对象实例（称为根对象）进行求值的表达式字符串。

    下面例子展示了如何从示例中 取属性

    ```java
    SpelExpressionParser parser = new SpelExpressionParser();
    Person person = new Person();
    person.age = 10;
    person.name = "肖佳权";
    Expression age = parser.parseExpression("age");
    int value = age.getValue(person, int.class);
    System.out.println(value);
    ```



## Understanding EvaluationContext

**简介**

在 对表达式 求值以解析 属性，方法，或字段并帮助执行类型转换时，将使用 `EvaluationContext`  接口 

**有两个实现**

- `SimpleEvaluationContext`: 

    针对不需要 完整SpEL语言 语法且 应受到有意义限制的表达式类别，公开了SPEL 基本语言功能和配置选项的子集。示例包括但不限于数据绑定，基于属性的过滤器

- `StandardEvaluationContext`:

    公开了完整的SpEL语言功能和配置选项，您可以使用它来指定默认的跟对象，并配置每个可用的 求值策略

**SimpleEvaluationContext**

* 设计为 只提供 SpEL语言语法的子集，它不包括Java类型引用，构造函数和Bean引用

* 它还要求您 显示的 选择 对表达式中的属性和 方法的 支持级别，默认 *create* 静态工厂方法 仅允许对属性的读取访问
* 你可以获取构建器来配置 准确的支持级别 针对以下一种或某些组合
    * Custom `PropertyAccessor` only (no reflection) 
    * Data binding properties for read-only access
    * Data binding properties for read and write

**Type Conversion**

* 默认情况下，SpEL 使用  `org.springframework.core.convert.ConversionService` 的转换服务
* 这个服务内置了很多通用类型转换器，同样可扩展，具备泛型

使用 *setValue* 赋值时，会自动将 字符串类型的 *string* 转成 Boolean

```java
    static class Simple {
        public List<Boolean> booleanList = new ArrayList<Boolean>();
    }

    public static void main(String[] args) {
        Simple simple = new Simple();
        simple.booleanList.add(true);

        EvaluationContext context = SimpleEvaluationContext.forReadOnlyDataBinding().build();

// "false" is passed in here as a String. SpEL and the conversion service
// will recognize that it needs to be a Boolean and convert it accordingly.
        SpelExpressionParser parser = new SpelExpressionParser();
        parser.parseExpression("booleanList[0]").setValue(context, simple, "false");

// b is false
        Boolean b = simple.booleanList.get(0);
        System.out.println(b);
    }
```

## Parser Configuration

> 语法解析配置

* 可以通过 使用 解析器配置对象 配置 SpEL的 表达式解析 `org.springframework.expression.spel.SpelParserConfiguration`
* 配置对象控制某些表达式组件的行为
    * 例如，使用索引 访问数组或集合 遇到 *NULL* 值，SpEL可以自动创建元素，这对于链式属性引用很有用
    * 还可以当 集合访问超出容量之后，*SpEL* 能够自动扩容
    * 往指定索引处放值，使用默认的构造器设置值，如果没有默认的构造器，则填充*NULL*
    * 如果没有 内置的或 自定义的转换器 则会填充 *NULL* 

```java
class Demo {
    public List<String> list;
}

// Turn on:
// - auto null reference initialization
// - auto collection growing
SpelParserConfiguration config = new SpelParserConfiguration(true,true);

ExpressionParser parser = new SpelExpressionParser(config);

Expression expression = parser.parseExpression("list[3]");

Demo demo = new Demo();

Object o = expression.getValue(demo);

// demo.list will now be a real collection of 4 entries
// Each entry is a new empty String
```

## SpEL Compilation

> SpEL表达式编译

**编译执行与解析执行**

Spring Framework 4.1 包含一个基本的 表达式编译器。表达式通常是被解析执行，这在求值阶段提供了很大的灵活性，但不会提供最佳性能。偶尔使用表达式 这已经够了，但是与其他组件 （例如 Spring Integration) 性能很重要，并且不需要动态性

**编译执行的使用场景**

​	SpEL编译器 旨在满足 这一需求：在求值过程中 生成一个Java类，该类在运行时体现了表达式的行为，并使用该类来实现更快的表达式解析

由于缺少在表达式周围 输入的信息，编译器 在执行编译时会使用 在表达式的解释求值过程收集的 信息

例如，它不只是 从表达式中就知道属性引用的类型，而是在第一次解释求值时知道的类型

当然，如果表达式元素的类型 随着时间变化，则基于此类派生信息进行编译会在以后引起麻烦

**因此 编译最适合类型信息 在重复求值过程 不会改变的表达式**

```java
someArray[0].someProperty.someOtherProperty < 0.1
```

由于前面的这个例子 涉及数组访问，一些属性的解引用 和数字运算，因此性能提升很明显，在 基准测试中，50000个迭代 ，

解释求值版本：75ms，编译求值版本：3ms

**Compiler Configuration**

默认情况下不打开编译器，但是您可以通过两种不同的方式之一来打开它。

* 您可以通过使用解析器配置过程来打开它
* 或者 当SpEL 嵌入到某个组件时，使用 spring属性控制打开它



**编译模式**

定义在枚举 `org.springframework.expression.spel.SpelCompilerMode`

- `OFF` (default): The compiler is switched off.
- `IMMEDIATE`: 尽快编译.通常是在第一次解释求值之后. 如果编译的表达式失败(通常是因为类型变化) 会抛异常
- `MIXED`: 在混合模式下，表达式会随着时间静默在解释模式和编译模式之间切换. 经过一些解释后的运行, 他们切换到已编译的表单，如果已编译的表单出了问题（例如，如前面所述的类型更改）表达式自动再次切换回解释形式.

`IMMEDIATE` mode exists because `MIXED` mode could cause issues for expressions that have side effects. If a compiled expression blows up after partially succeeding

**存在IMMEDIATE模式的原因**

存在IMMEDIATE模式是因为MIXED模式可能会引起具有副作用的表达式问题。如果部分成功后已编译的表达式运行失败，切换到解释模式重新执行，那部分可能会执行两遍

```java
SpelParserConfiguration config = new SpelParserConfiguration(SpelCompilerMode.IMMEDIATE,
    this.getClass().getClassLoader());

SpelExpressionParser parser = new SpelExpressionParser(config);

Expression expr = parser.parseExpression("payload");

MyMessage message = new MyMessage();

Object payload = expr.getValue(message);
```

当指定编译器模式时，还可以指定一个类加载器（允许传递null）。

编译表达式是在提供的任何子类加载器中创建的子类加载器中定义的类

重要的是要确保，如果指定了类加载器，则它可以查看表达式求值过程中涉及的所有类型。

如果未指定类加载器，则使用默认的类加载器（通常是在表达式求值期间运行的线程的上下文类加载器）。

第二种配置编译器的方法是将SpEL嵌入到其他组件中，并且可能无法通过配置对象进行配置。
在这种情况下，可以通过JVM系统属性 `spring.expression.compiler.mode` 

 [`SpringProperties`](https://docs.spring.io/spring-framework/docs/current/reference/html/appendix.html#appendix-spring-properties) 机制 *SpelCompilerMode* (`off`, `immediate`, or `mixed`)

**Compiler Limitations**

从Spring Framework 4.1开始，已经有了基本的编译框架

但是，该框架尚不支持编译每种表达式

最初的重点是可能在性能关键型上下文中使用的通用表达式。

目前无法编译以下类型的表达式：

- Expressions involving assignment：涉及赋值的表达
- Expressions relying on the conversion service：依赖转换服务的表达式
- Expressions using custom resolvers or accessors：使用自定义解析器或访问器的表达式
- Expressions using selection or projection：使用选择或投影的表达式



# Expressions in Bean Definitions

您可以将SpEL表达式与基于XML或基于注释的配置元数据一起使用，用来定义“ BeanDefinition”实例。
在这两种情况下，用于定义表达式的语法均采用“＃{<表达式字符串>}”的形式。

## XML Configuration

```java
<bean id="numberGuess" class="org.spring.samples.NumberGuess">
    <property name="randomNumber" value="#{ T(java.lang.Math).random() * 100.0 }"/>
    <!-- other properties -->
</bean>
```

*ApplicationContext* 的所有 bean 都可以通过 bean名引用，包括内置的 bean，例如 访问运行时的环境变量`org.springframework.core.env.Environment`  以及 *systemProperties* 和 *systemEnvironment*

通过属性名 访问属性

```java
<bean id="taxCalculator" class="org.spring.samples.TaxCalculator">
    <property name="defaultLocale" value="#{ systemProperties['user.region'] }"/>

    <!-- other properties -->
</bean>
```

```java
<bean id="numberGuess" class="org.spring.samples.NumberGuess">
    <property name="randomNumber" value="#{ T(java.lang.Math).random() * 100.0 }"/>

    <!-- other properties -->
</bean>

<bean id="shapeGuess" class="org.spring.samples.ShapeGuess">
    <property name="initialShapeSeed" value="#{ numberGuess.randomNumber }"/>

    <!-- other properties -->
</bean>
```

##  Annotation Configuration

**属性**

```java
public class FieldValueTestBean {

    @Value("#{ systemProperties['user.region'] }")
    private String defaultLocale;

    public void setDefaultLocale(String defaultLocale) {
        this.defaultLocale = defaultLocale;
    }

    public String getDefaultLocale() {
        return this.defaultLocale;
    }
}
```

**setterMethod**

```java
public class PropertyValueTestBean {

    private String defaultLocale;

    @Value("#{ systemProperties['user.region'] }")
    public void setDefaultLocale(String defaultLocale) {
        this.defaultLocale = defaultLocale;
    }

    public String getDefaultLocale() {
        return this.defaultLocale;
    }
}
```

Autowired methods and constructors can also use the `@Value` annotation

```java
public class SimpleMovieLister {

    private MovieFinder movieFinder;
    private String defaultLocale;

    @Autowired
    public void configure(MovieFinder movieFinder,
            @Value("#{ systemProperties['user.region'] }") String defaultLocale) {
        this.movieFinder = movieFinder;
        this.defaultLocale = defaultLocale;
    }

    // ...
}
```

```java
public class MovieRecommender {

    private String defaultLocale;

    private CustomerPreferenceDao customerPreferenceDao;

    public MovieRecommender(CustomerPreferenceDao customerPreferenceDao,
            @Value("#{systemProperties['user.country']}") String defaultLocale) {
        this.customerPreferenceDao = customerPreferenceDao;
        this.defaultLocale = defaultLocale;
    }

    // ...
}
```



# Language Reference

> 语法参考

## Literal Expressions

字面量包括：字符串单引号包裹，数值类型：整数，实数，十六进制，boolean，的

```java
ExpressionParser parser = new SpelExpressionParser();

// evals to "Hello World"
String helloWorld = (String) parser.parseExpression("'Hello World'").getValue();

double avogadrosNumber = (Double) parser.parseExpression("6.0221415E+23").getValue();

// evals to 2147483647
int maxValue = (Integer) parser.parseExpression("0x7FFFFFFF").getValue();

boolean trueValue = (Boolean) parser.parseExpression("true").getValue();

Object nullValue = parser.parseExpression("null").getValue();
```

数字支持使用负号，指数符号和小数点。
默认情况下，使用Double.parseDouble（）解析实数。



## Properties, Arrays, Lists, Maps, and Indexers

### 使用属性名访问属性

```java
// evals to 1856
int year = (Integer) parser.parseExpression("birthdate.year + 1900").getValue(context);

String city = (String) parser.parseExpression("placeOfBirth.city").getValue(context);
```

* 属性名称的首字母允许不区分大小写
* 上述的表达式 可以写作 `Birthdate.Year + 1900` and `PlaceOfBirth.City`
* 属性的访问 也可能通过方法调用 ``getPlaceOfBirth().getCity()` instead of `placeOfBirth.city`

### 使用方括号访问数组

```java
ExpressionParser parser = new SpelExpressionParser();
EvaluationContext context = SimpleEvaluationContext.forReadOnlyDataBinding().build();

// Inventions Array

// evaluates to "Induction motor"
String invention = parser.parseExpression("inventions[3]").getValue(
        context, tesla, String.class);

// Members List

// evaluates to "Nikola Tesla"
String name = parser.parseExpression("members[0].name").getValue(
        context, ieee, String.class);

// List and Array navigation
// evaluates to "Wireless communication"
String invention = parser.parseExpression("members[0].inventions[6]").getValue(
        context, ieee, String.class);
```

### 访问Map

```java
// Officer's Dictionary

Inventor pupin = parser.parseExpression("officers['president']").getValue(
        societyContext, Inventor.class);

// evaluates to "Idvor"
String city = parser.parseExpression("officers['president'].placeOfBirth.city").getValue(
        societyContext, String.class);

// setting values
parser.parseExpression("officers['advisors'][0].placeOfBirth.country").setValue(
        societyContext, "Croatia");
```

## 内联List

```java
// evaluates to a Java list containing the four numbers
List numbers = (List) parser.parseExpression("{1,2,3,4}").getValue(context);

List listOfLists = (List) parser.parseExpression("{{'a','b'},{'x','y'}}").getValue(context);
```

{}本身表示一个空列表。
出于性能原因，如果列表本身完全由固定文字组成，则会创建一个常量列表来表示该表达式（而不是在每次求值时都构建一个新列表）。

## Inline Maps

`{key:value}`

```java
// evaluates to a Java map containing the two entries
Map inventorInfo = (Map) parser.parseExpression("{name:'Nikola',dob:'10-July-1856'}").getValue(context);

Map mapOfMaps = (Map) parser.parseExpression("{name:{first:'Nikola',last:'Tesla'},dob:{day:10,month:'July',year:1856}}").getValue(context);
```

`{：}`表示一个空的map
出于性能原因，如果映射表本身由固定的文字或其他嵌套的常量结构（列表或映射表）组成，则会创建一个常量映射表来表示该表达式（而不是在每次求值时都构建一个新的映射表）。映射键的引号是可选的。



## Array Construction

您可以使用熟悉的Java语法来构建数组，可以选择提供一个初始化以在构造时填充该数组。
以下示例显示了如何执行此操作：

```java
int[] numbers1 = (int[]) parser.parseExpression("new int[4]").getValue(context);

// Array with initializer
int[] numbers2 = (int[]) parser.parseExpression("new int[]{1,2,3}").getValue(context);

// Multi dimensional array
int[][] numbers3 = (int[][]) parser.parseExpression("new int[4][5]").getValue(context);
```

构造多维数组时，当前无法提供初始化

## Methods

```java
// string literal, evaluates to "bc"
String bc = parser.parseExpression("'abc'.substring(1, 3)").getValue(String.class);

// evaluates to true
boolean isMember = parser.parseExpression("isMember('Mihajlo Pupin')").getValue(
        societyContext, Boolean.class);
```

## Operators

> 操作符
**支持四种运算符**
- Relational Operators 关系运算符
- Logical Operators 逻辑运算符
- Mathematical Operators 数学运算符
- The Assignment Operator 赋值运算符

### Relational Operators

支持 equal, not equal, less than, less than or equal, greater than, and greater than or equal  这几种 关系运算符

```java
// evaluates to true
boolean trueValue = parser.parseExpression("2 == 2").getValue(Boolean.class);

// evaluates to false
boolean falseValue = parser.parseExpression("2 < -5.0").getValue(Boolean.class);

// evaluates to true
boolean trueValue = parser.parseExpression("'black' < 'block'").getValue(Boolean.class);
```

**对于NULL值的处理**

`X > null` is always `true` 

`X < null` is always `false`

**InstanceOf与Match**

```java
// evaluates to false
boolean falseValue = parser.parseExpression(
        "'xyz' instanceof T(Integer)").getValue(Boolean.class);

// evaluates to true
boolean trueValue = parser.parseExpression(
        "'5.00' matches '^-?\\d+(\\.\\d{2})?$'").getValue(Boolean.class);

//evaluates to false
boolean falseValue = parser.parseExpression(
        "'5.0067' matches '^-?\\d+(\\.\\d{2})?$'").getValue(Boolean.class);
```

请注意基本类型，因为它们会立即被包装为包装类型，因此，如预期的那样，1 instanceof T（int）的计算结果为false，而1 instanceofT（Integer）的计算结果为true。

**支持字母的运算符**

- `lt` (`<`)
- `gt` (`>`)
- `le` (`<=`)
- `ge` (`>=`)
- `eq` (`==`)
- `ne` (`!=`)
- `div` (`/`)
- `mod` (`%`)
- `not` (`!`).

### Logical Operators

SpEL supports the following logical operators:

- `and` (`&&`)
- `or` (`||`)
- `not` (`!`)

```java
// -- AND --

// evaluates to false
boolean falseValue = parser.parseExpression("true and false").getValue(Boolean.class);

// evaluates to true
String expression = "isMember('Nikola Tesla') and isMember('Mihajlo Pupin')";
boolean trueValue = parser.parseExpression(expression).getValue(societyContext, Boolean.class);

// -- OR --

// evaluates to true
boolean trueValue = parser.parseExpression("true or false").getValue(Boolean.class);

// evaluates to true
String expression = "isMember('Nikola Tesla') or isMember('Albert Einstein')";
boolean trueValue = parser.parseExpression(expression).getValue(societyContext, Boolean.class);

// -- NOT --

// evaluates to false
boolean falseValue = parser.parseExpression("!true").getValue(Boolean.class);

// -- AND and NOT --
String expression = "isMember('Nikola Tesla') and !isMember('Mihajlo Pupin')";
boolean falseValue = parser.parseExpression(expression).getValue(societyContext, Boolean.class);
```

### Mathematical Operators

```java
// Addition
int two = parser.parseExpression("1 + 1").getValue(Integer.class);  // 2

String testString = parser.parseExpression(
        "'test' + ' ' + 'string'").getValue(String.class);  // 'test string'

// Subtraction
int four = parser.parseExpression("1 - -3").getValue(Integer.class);  // 4

double d = parser.parseExpression("1000.00 - 1e4").getValue(Double.class);  // -9000

// Multiplication
int six = parser.parseExpression("-2 * -3").getValue(Integer.class);  // 6

double twentyFour = parser.parseExpression("2.0 * 3e0 * 4").getValue(Double.class);  // 24.0

// Division
int minusTwo = parser.parseExpression("6 / -3").getValue(Integer.class);  // -2

double one = parser.parseExpression("8.0 / 4e0 / 2").getValue(Double.class);  // 1.0

// Modulus
int three = parser.parseExpression("7 % 4").getValue(Integer.class);  // 3

int one = parser.parseExpression("8 / 5 % 2").getValue(Integer.class);  // 1

// Operator precedence
int minusTwentyOne = parser.parseExpression("1+2-3*8").getValue(Integer.class);  // -21
```

### The Assignment Operator

要设置属性，请使用赋值运算符（=）。 这通常在对 `setValue`的调用内完成，但也可以在对getValue的调用内完成。
下面的清单显示了使用赋值运算符的两种方法：

```java
Inventor inventor = new Inventor();
EvaluationContext context = SimpleEvaluationContext.forReadWriteDataBinding().build();

parser.parseExpression("name").setValue(context, inventor, "Aleksandar Seovic");

// alternatively
String aleks = parser.parseExpression(
        "name = 'Aleksandar Seovic'").getValue(context, inventor, String.class);
```

## Types

* 你可以使用 特殊的 T运算符 来执行 `java.lang.Class` 类示例
* 也可以通过此运算符 来调用 静态方法
* `StandardEvaluationContext`  使用  `TypeLocator`  查找类型， `StandardTypeLocator`  （可以替换） 会自动查找 Java.lang包，不需要全限定类名，而其他包需要

```java
Class dateClass = parser.parseExpression("T(java.util.Date)").getValue(Class.class);

Class stringClass = parser.parseExpression("T(String)").getValue(Class.class);

boolean trueValue = parser.parseExpression(
        "T(java.math.RoundingMode).CEILING < T(java.math.RoundingMode).FLOOR")
        .getValue(Boolean.class);
```



## Constructors

您可以使用`new`运算符来调用构造函数。
除了基本类型（`int`，`float`等）和String之外，您都应使用完全限定的类名。
以下示例显示了如何使用`new`运算符来调用构造函数：

```java
Inventor einstein = p.parseExpression(
        "new org.spring.samples.spel.inventor.Inventor('Albert Einstein', 'German')")
        .getValue(Inventor.class);

//create new inventor instance within add method of List
p.parseExpression(
        "Members.add(new org.spring.samples.spel.inventor.Inventor(
            'Albert Einstein', 'German'))").getValue(societyContext);
```

## Variables

* 您可以使用 `#variableName` 语法在表达式中引用变量。
* 通过在EvaluationContext实现上使用setVariable方法设置变量。

### 合法变量名：

 `A` to `Z` and `a` to `z`digits: `0` to `9`underscore: `_`dollar sign: `$`

```java
Inventor tesla = new Inventor("Nikola Tesla", "Serbian");

EvaluationContext context = SimpleEvaluationContext.forReadWriteDataBinding().build();
context.setVariable("newName", "Mike Tesla");

parser.parseExpression("name = #newName").getValue(context, tesla);
System.out.println(tesla.getName())  // "Mike Tesla"
```

### The `#this` and `#root` Variables

* `#this` 变量始终 定义，引用当前解析对象（不会解析不满足的变量） 
* `#root` 始终指向 root context object `#this` 随着 表达式组件部分 求值 的变化而变化

```java
// create an array of integers
List<Integer> primes = new ArrayList<Integer>();
primes.addAll(Arrays.asList(2,3,5,7,11,13,17));

// create parser and set variable 'primes' as the array of integers
ExpressionParser parser = new SpelExpressionParser();
EvaluationContext context = SimpleEvaluationContext.forReadOnlyDataAccess();
context.setVariable("primes", primes);

// all prime numbers > 10 from the list (using selection ?{...})
// evaluates to [11, 13, 17]
List<Integer> primesGreaterThanTen = (List<Integer>) parser.parseExpression(
        "#primes.?[#this>10]").getValue(context);
```

## Functions

您可以通过注册可以在表达式字符串中调用的用户定义函数来扩展SpEL。
该函数通过EvaluationContext注册。
以下示例显示了如何注册用户定义的函数：

```java
Method method = ...;

EvaluationContext context = SimpleEvaluationContext.forReadOnlyDataBinding().build();
context.setVariable("myFunction", method);
```

```java
public abstract class StringUtils {

    public static String reverseString(String input) {
        StringBuilder backwards = new StringBuilder(input.length());
        for (int i = 0; i < input.length(); i++) {
            backwards.append(input.charAt(input.length() - 1 - i));
        }
        return backwards.toString();
    }
}

ExpressionParser parser = new SpelExpressionParser();

EvaluationContext context = SimpleEvaluationContext.forReadOnlyDataBinding().build();
context.setVariable("reverseString",
        StringUtils.class.getDeclaredMethod("reverseString", String.class));

String helloWorldReversed = parser.parseExpression(
        "#reverseString('hello')").getValue(context, String.class);
```

## Bean References

如果 EvaluationContext 已使用bean解析器配置，则可以使用@符号从表达式中查找bean。
以下示例显示了如何执行此操作：

```java
ExpressionParser parser = new SpelExpressionParser();
StandardEvaluationContext context = new StandardEvaluationContext();
context.setBeanResolver(new MyBeanResolver());

// This will end up calling resolve(context,"something") on MyBeanResolver during evaluation
Object bean = parser.parseExpression("@something").getValue(context);
```

要访问工厂bean本身，您应该在bean名称前加上＆符号。
以下示例显示了如何执行此操作：

```java
ExpressionParser parser = new SpelExpressionParser();
StandardEvaluationContext context = new StandardEvaluationContext();
context.setBeanResolver(new MyBeanResolver());

// This will end up calling resolve(context,"&foo") on MyBeanResolver during evaluation
Object bean = parser.parseExpression("&foo").getValue(context);
```

## Ternary Operator (If-Then-Else)

```java
String falseString = parser.parseExpression(
        "false ? 'trueExp' : 'falseExp'").getValue(String.class);
```

```java
parser.parseExpression("name").setValue(societyContext, "IEEE");
societyContext.setVariable("queryName", "Nikola Tesla");

expression = "isMember(#queryName)? #queryName + ' is a member of the ' " +
        "+ Name + ' Society' : #queryName + ' is not a member of the ' + Name + ' Society'";

String queryResultString = parser.parseExpression(expression)
        .getValue(societyContext, String.class);
// queryResultString = "Nikola Tesla is a member of the IEEE Society"
```

## The Elvis Operator

Elvis运算符是三元运算符语法的简化，并且在Groovy语言中使用。
使用三元运算符语法，通常必须将变量重复两次，如以下示例所示：

```java
String name = "Elvis Presley";
String displayName = (name != null ? name : "Unknown");
```

```java
ExpressionParser parser = new SpelExpressionParser();

String name = parser.parseExpression("name?:'Unknown'").getValue(new Inventor(), String.class);
System.out.println(name);  // 'Unknown'
```

```java
@Value("#{systemProperties['pop3.port'] ?: 25}")
```

## Safe Navigation Operator

安全导航运算符用于避免NullPointerException，它来自Groovy语言。
通常，当您引用对象时，可能需要在访问对象的方法或属性之前验证其是否为null。
为了避免这种情况，安全导航运算符返回null而不是引发异常。
以下示例显示如何使用安全导航操作符：

```java
ExpressionParser parser = new SpelExpressionParser();
EvaluationContext context = SimpleEvaluationContext.forReadOnlyDataBinding().build();

Inventor tesla = new Inventor("Nikola Tesla", "Serbian");
tesla.setPlaceOfBirth(new PlaceOfBirth("Smiljan"));

String city = parser.parseExpression("placeOfBirth?.city").getValue(context, tesla, String.class);
System.out.println(city);  // Smiljan

tesla.setPlaceOfBirth(null);
city = parser.parseExpression("placeOfBirth?.city").getValue(context, tesla, String.class);
System.out.println(city);  // null - does not throw NullPointerException!!!
```

## Collection Selection

选择是一种强大的表达语言功能，可让您通过从源集合中选择条目来将其转换为另一个集合。

选择使用。`？[selectionExpression] ` 的语法。
它过滤该集合并返回一个包含原始元素子集的新集合。

```java
List<Inventor> list = (List<Inventor>) parser.parseExpression(
        "members.?[nationality == 'Serbian']").getValue(societyContext);
```

在List和Map上都可以选择。
对于List，将针对每个单独的列表元素评估选择标准。
针对Map，针对每个Map Entry（Java类型Map.Entry的对象）评估选择标准。
每个Map Entry都有其键和值，可作为属性进行访问，以供在选择中使用。

```java
Map newMap = parser.parseExpression("map.?[value<27]").getValue();
```

除了返回所有选定的元素外，您只能检索第一个或最后一个值。
为了获得与选择匹配的第一个条目，语法为。`^ [selectionExpression]`。
要获得最后的匹配选择，语法为。`$ [selectionExpression]`。

## Collection Projection

投影使集合可以驱动子表达式的求值，结果是一个新的集合。
投影的语法为。`！[projectionExpression]`。

```java
// returns ['Smiljan', 'Idvor' ]
List placesOfBirth = (List)parser.parseExpression("members.![placeOfBirth.city]");
```

使用map 投影的结果也是 *list*

## Expression templating

表达式模板允许将文字文本与一个或多个求值块混合。
每个求值块均由您可以定义的前缀和后缀字符定界。
常见的选择是使用＃{}作为分隔符，如以下示例所示：

```java
String randomPhrase = parser.parseExpression(
        "random number is #{T(java.lang.Math).random()}",
        new TemplateParserContext()).getValue(String.class);

// evaluates to "random number is 0.7038186818312008"
```

```java
    public static void main(String[] args) {
        SpelParserConfiguration configuration = new SpelParserConfiguration(true,true);
        SpelExpressionParser parser = new SpelExpressionParser(configuration);
        Expression expression = parser.parseExpression("12#{list[0]}12", new TemplateParserContext());
        Demo demo = new Demo();
        demo.list = new ArrayList<>();
        demo.list.add("sdfdsf");
        Object o = expression.getValue(demo,String.class);
        System.out.println(o);
    }
```
