# XML

## XML语法规则

* 必须有跟元素

* XML声明 可选

  `<?xml version="1.0" encoding="utf-8"?>`

* 所有的 XML 元素都必须有一个关闭标签

* XML 标签对大小写敏感

* XML **必须正确嵌套**

* XML 属性值必须加引号

* 实体引用：特殊字符用实体引用替代

  | 实体引用 | 符号 | 含义           |
  | -------- | ---- | -------------- |
  | `&lt;`   | <    | less than      |
  | `&gt;`   | >    | greater than   |
  | `&amp;`  | &    | ampersand      |
  | `&apos;` | '    | apostrophe     |
  | `&quot;` | "    | quotation mark |

* XML 中的注释

  `<!-- This is a comment -->`

* 空格会被保留

  HTML 会把多个连续的空格字符裁减（合并）为一个：

* XML 以 LF 存储换行

## XML元素

* XML 元素指的是从（且包括）开始标签直到（且包括）结束标签的部分。

* 一个元素可以包含：
  * 其他元素
  * 文本
  * 属性
  * 或混合以上所有...
* 元素命名规则
  * 字母数字其他字符
  * 不能以数字标点符号开始
  * 不能以字母xml开始
  * 不能有空格
  * 避免使用 : -  . 的字符

## XML属性

XML 中，您应该尽量避免使用属性。如果信息感觉起来很像数据，那么请使用元素吧

元数据（有关数据的数据）应当存储为属性，而数据本身应当存储为元素。

## XML验证

* 合法的 XML 文档是"形式良好"的 XML 文档，这也符合文档类型定义（DTD,documentTyoeDefinition ）的规则
* `<!DOCTYPE note system "note.dtd"`

* DTD 的目的是定义 XML 文档的结构

* W3C 支持一种基于 XML 的 DTD 代替者，它名为 XML Schema

## 使用 CSS /XSLT显示 XML

* `<?xml-stylesheet type="text/css" href="cd_catalog.css"?>`

* 使用 CSS 格式化 XML 不是常用的方法。W3C 推荐使用 XSLT。
* XSLT（eXtensible Stylesheet Language Transformations）远比 CSS 更加完善。
* 通过使用 XSLT，您可以把 XML 文档转换成 HTML 格式。

## XML 命名空间

* XML 命名空间提供避免元素命名冲突的方法。

* 在 XML 中的命名冲突可以通过使用名称前缀从而容易地避免。

  ```
  <h:table>
  <h:tr>
  <h:td>Apples</h:td>
  <h:td>Bananas</h:td>
  </h:tr>
  </h:table>
  ```

* 当在 XML 中使用前缀时，一个所谓的用于前缀的**命名空间**必须被定义。xmlns:*前缀*="*URI*"

  ```
  <h:table xmlns:h="http://www.w3.org/TR/html4/">
  <h:tr>
  <h:td>Apples</h:td>
  <h:td>Bananas</h:td>
  </h:tr>
  </h:table>
  ```

* 元素定义默认的命名空间可以让我们省去在所有的子元素中使用前缀的工作

  `<table xmlns="http://www.w3.org/TR/html4/">`

  

## XML CDATA

* XML 文档中的所有文本均会被解析器解析。只有 CDATA 区段中的文本会被解析器忽略。
* CDATA 部分由 `<![CDATA[` 开始，由 "**]]>**" 结束：

# 

# DTD

## 简介

DTD（文档类型定义）的作用是定义 XML 文档的合法构建模块。

DTD 可被成行地声明于 XML 文档中，也可作为一个外部引用。

## XML构建模块

* 所有的 XML 文档（以及 HTML 文档）均由以下简单的构建模块构成：
  * 元素
  * 属性
  * 实体：特殊字符的实体引用
  * PCDATA：parsed character data，**被解析器解析的文本**
  * CDATA：**不会被解析器解析的文本**

## DTD元素申明

声明一个元素

```
<!ELEMENT element-name category>
或
<!ELEMENT element-name (element-content)>
```

* 空元素：`<!ELEMENT br EMPTY>`
* PCDATA 的元素：`<!ELEMENT from (#PCDATA)>`
* 带有任何内容的元素:`<!ELEMENT note ANY>`
* 带有子元素（序列）的元素
  * 带有一个或多个子元素的元素通过圆括号中的子元素名进行声明：
  * `<!ELEMENT element-name (child1)>`
  * `<!ELEMENT element-name (child1,child2,...)>`
* 声明只出现一次的元素
  * `<!ELEMENT note (message)>`
* 声明最少出现一次的元素
  * `<!ELEMENT note (message+)>`
* 声明出现零次或多次的元素
  * `<!ELEMENT note (message*)>`
* 声明出现零次或一次的元素
  * `<!ELEMENT note (message?)>`

* 声明"非.../即..."类型的内容
  * `<!ELEMENT note (to,from,header,(message|body))>`
* 声明混合型的内容
  * `<!ELEMENT note (#PCDATA|to|from|header|message)*>`



## DTD属性申明

在 DTD 中，属性通过 ATTLIST 声明来进行声明。

* 语法：

  `<!ATTLIST element-name attribute-name attribute-type attribute-value>`

* 类型选项

  | 类型               | 描述                          |
  | :----------------- | :---------------------------- |
  | CDATA              | 值为字符数据 (character data) |
  | (*en1*\|*en2*\|..) | 此值是枚举列表中的一个值      |
  | ID                 | 值为唯一的 id                 |
  | IDREF              | 值为另外一个元素的 id         |
  | IDREFS             | 值为其他 id 的列表            |
  | NMTOKEN            | 值为合法的 XML 名称           |
  | NMTOKENS           | 值为合法的 XML 名称的列表     |
  | ENTITY             | 值是一个实体                  |
  | ENTITIES           | 值是一个实体列表              |
  | NOTATION           | 此值是符号的名称              |
  | xml:               | 值是一个预定义的 XML 值       |

* 默认**属性值**

  | 值           | 属性的默认值   |
  | ------------ | -------------- |
  |              | 属性值         |
  | #REQUIRED    | 属性值是必需的 |
  | #IMPLIED     | 属性不是必需的 |
  | #FIXED value | 属性值是固定的 |

## DTD实体

* 实体是用于定义引用普通文本或特殊字符的快捷方式的变量。

* 实体引用是对实体的引用
* 实体可在内部或外部进行声明。

* 一个内部实体声明

  * `<!ENTITY entity-name "entity-value">`

    ```
    <!ENTITY writer "Donald Duck.">
    <!ENTITY copyright "Copyright runoob.com">
    
    XML 实例：
    
    <author>&writer;&copyright;</author>
    ```

    

  * 一个实体由三部分构成: 一个和号 (&), 一个实体名称, 以及一个分号 (;)。

* 一个外部实体声明

  ```
  DTD 实例:
  
  <!ENTITY writer SYSTEM "http://www.runoob.com/entities.dtd">
  <!ENTITY copyright SYSTEM "http://www.runoob.com/entities.dtd">
  
  XML example:
  
  <author>&writer;&copyright;</author>
  ```

## EXAMPLE

### 电视节目表 DTD

```
由 David Moisan 创造。拷贝自： http://www.davidmoisan.org/

<!DOCTYPE TVSCHEDULE [

<!ELEMENT TVSCHEDULE (CHANNEL+)>
<!ELEMENT CHANNEL (BANNER,DAY+)>
<!ELEMENT BANNER (#PCDATA)>
<!ELEMENT DAY (DATE,(HOLIDAY|PROGRAMSLOT+)+)>
<!ELEMENT HOLIDAY (#PCDATA)>
<!ELEMENT DATE (#PCDATA)>
<!ELEMENT PROGRAMSLOT (TIME,TITLE,DESCRIPTION?)>
<!ELEMENT TIME (#PCDATA)>
<!ELEMENT TITLE (#PCDATA)> 
<!ELEMENT DESCRIPTION (#PCDATA)>

<!ATTLIST TVSCHEDULE NAME CDATA #REQUIRED>
<!ATTLIST CHANNEL CHAN CDATA #REQUIRED>
<!ATTLIST PROGRAMSLOT VTR CDATA #IMPLIED>
<!ATTLIST TITLE RATING CDATA #IMPLIED>
<!ATTLIST TITLE LANGUAGE CDATA #IMPLIED>
]>


```



------

### 报纸文章 DTD

```
拷贝自：http://www.vervet.com/

<!DOCTYPE NEWSPAPER [

<!ELEMENT NEWSPAPER (ARTICLE+)>
<!ELEMENT ARTICLE (HEADLINE,BYLINE,LEAD,BODY,NOTES)>
<!ELEMENT HEADLINE (#PCDATA)>
<!ELEMENT BYLINE (#PCDATA)>
<!ELEMENT LEAD (#PCDATA)>
<!ELEMENT BODY (#PCDATA)>
<!ELEMENT NOTES (#PCDATA)>

<!ATTLIST ARTICLE AUTHOR CDATA #REQUIRED>
<!ATTLIST ARTICLE EDITOR CDATA #IMPLIED>
<!ATTLIST ARTICLE DATE CDATA #IMPLIED>
<!ATTLIST ARTICLE EDITION CDATA #IMPLIED>

<!ENTITY NEWSPAPER "Vervet Logic Times">
<!ENTITY PUBLISHER "Vervet Logic Press">
<!ENTITY COPYRIGHT "Copyright 1998 Vervet Logic Press">

]>
```



------

### 产品目录 DTD

```
拷贝自： http://www.vervet.com/

<!DOCTYPE CATALOG [

<!ENTITY AUTHOR "John Doe">
<!ENTITY COMPANY "JD Power Tools, Inc.">
<!ENTITY EMAIL "jd@jd-tools.com">

<!ELEMENT CATALOG (PRODUCT+)>

<!ELEMENT PRODUCT
(SPECIFICATIONS+,OPTIONS?,PRICE+,NOTES?)>
<!ATTLIST PRODUCT
NAME CDATA #IMPLIED
CATEGORY (HandTool|Table|Shop-Professional) "HandTool"
PARTNUM CDATA #IMPLIED
PLANT (Pittsburgh|Milwaukee|Chicago) "Chicago"
INVENTORY (InStock|Backordered|Discontinued) "InStock">

<!ELEMENT SPECIFICATIONS (#PCDATA)>
<!ATTLIST SPECIFICATIONS
WEIGHT CDATA #IMPLIED
POWER CDATA #IMPLIED>

<!ELEMENT OPTIONS (#PCDATA)>
<!ATTLIST OPTIONS
FINISH (Metal|Polished|Matte) "Matte"
ADAPTER (Included|Optional|NotApplicable) "Included"
CASE (HardShell|Soft|NotApplicable) "HardShell">

<!ELEMENT PRICE (#PCDATA)>
<!ATTLIST PRICE
MSRP CDATA #IMPLIED
WHOLESALE CDATA #IMPLIED
STREET CDATA #IMPLIED
SHIPPING CDATA #IMPLIED>

<!ELEMENT NOTES (#PCDATA)>

]>
```

# XMLSchema

描述

* XML Schema 是基于 XML 的 DTD 替代者。

* XML Schema 可描述 XML 文档的结构。

* XML Schema 语言也可作为 XSD（XML Schema Definition）来引用。

* XML Schema 的作用是定义 XML 文档的合法构建模块，

## XMLSchema定义

* 存在性问题定义

​	定义了文档中可以出现的元素和属性

* 层次结构

  定义了子元素，子元素的次序，数目

* 元素和属性的值

  数据类型，是否为空，是否可包含文本，默认值，固定值

## XSDschema元素

`<schema>`  为root元素

```
<?xml version="1.0"?>

<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema"
targetNamespace="http://www.runoob.com"
xmlns="http://www.runoob.com"
elementFormDefault="qualified">
...
...
</xs:schema>

xmlns:xs="http://www.w3.org/2001/XMLSchema" 申明一个名称空间使用xs前缀
xmlns="http://www.runoob.com" ：申明默认的名称空间
xmlns="http://www.runoob.com" ：被此schema定义的元素的名称空间
xs:schemaLocation="http://www.runoob.com note.xsd"：xs引用的名称空间的 XML schema 的位置
```

## XSD 简易元素

* 简易元素指那些只包含文本的元素。它不会包含任何其他的元素或属性。

* 它可以是 XML Schema 定义中包括的类型中的一种（布尔、字符串、数据等等）

定义简易元素

​	`<xs:element name="xxx" type="yyy"/>`

常见数据类型

- xs:string
- xs:decimal
- xs:integer
- xs:boolean
- xs:date
- xs:time

简易元素的默认值和固定值

`<xs:element name="color" type="xs:string" default="red"/>`

`<xs:element name="color" type="xs:string" fixed="red"/>`

## XSD属性

定义属性

`<xs:attribute name="xxx" type="yyy"/>`

- xs:string
- xs:decimal
- xs:integer
- xs:boolean
- xs:date
- xs:time

默认值和固定值

`<xs:attribute name="lang" type="xs:string" default="EN"/>`

`<xs:attribute name="lang" type="xs:string" fixed="EN"/>`

可选的和必需

在默认的情况下，属性是可选的

`<xs:attribute name="lang" type="xs:string" use="required"/>`

## XSD限定

### 对值的限定

限定 元素age 的值不能低于 0 或者高于 120

```
<xs:element name="age">
 <xs:simpleType>
  <xs:restriction base="xs:integer">
   <xs:minInclusive value="0"/>
   <xs:maxInclusive value="120"/>
  </xs:restriction>
 </xs:simpleType>
</xs:element>
```

### 对一组值的限定

使用枚举约束（enumeration constraint）	

```
<xs:element name="car">
  <xs:simpleType>
    <xs:restriction base="xs:string">
      <xs:enumeration value="Audi"/>
      <xs:enumeration value="Golf"/>
      <xs:enumeration value="BMW"/>
    </xs:restriction>
  </xs:simpleType>
</xs:element>
```

或者

```
<xs:element name="car" type="carType"/>

<xs:simpleType name="carType">
  <xs:restriction base="xs:string">
    <xs:enumeration value="Audi"/>
    <xs:enumeration value="Golf"/>
    <xs:enumeration value="BMW"/>
  </xs:restriction>
</xs:simpleType>
```

### 对一系列值的限定

使用模式约束（pattern constraint），符合正则表达式

```
<xs:element name="letter">
  <xs:simpleType>
    <xs:restriction base="xs:string">
      <xs:pattern value="[a-z]"/>
    </xs:restriction>
  </xs:simpleType>
</xs:element>
```

### 对空白字符的限定

```
<xs:whiteSpace value="preserve"/>：保留空格
<xs:whiteSpace value="replace"/>：移除所有空格
<xs:whiteSpace value="collapse"/>：合并空格
```

### 对长度的限定

```
<xs:length value="8"/>：值的长度
<xs:minLength value="5"/>：最小值
<xs:maxLength value="8"/>：最大值
```

### 数据类型限定表

| 限定           | 描述                                                      |
| :------------- | :-------------------------------------------------------- |
| enumeration    | 定义可接受值的一个列表                                    |
| fractionDigits | 定义所允许的最大的小数位数。必须大于等于0。               |
| length         | 定义所允许的字符或者列表项目的精确数目。必须大于或等于0。 |
| maxExclusive   | 定义数值的上限。所允许的值必须小于此值。                  |
| maxInclusive   | 定义数值的上限。所允许的值必须小于或等于此值。            |
| maxLength      | 定义所允许的字符或者列表项目的最大数目。必须大于或等于0。 |
| minExclusive   | 定义数值的下限。所允许的值必需大于此值。                  |
| minInclusive   | 定义数值的下限。所允许的值必需大于或等于此值。            |
| minLength      | 定义所允许的字符或者列表项目的最小数目。必须大于或等于0。 |
| pattern        | 定义可接受的字符的精确序列。                              |
| totalDigits    | 定义所允许的阿拉伯数字的精确位数。必须大于0。             |
| whiteSpace     | 定义空白字符（换行、回车、空格以及制表符）的处理方式。    |

## XSD复合元素

复合元素指包含其他元素及/或属性的 XML 元素。

### 四种类型的复合元素

- 空元素
- 包含其他元素的元素
- 仅包含文本的元素
- 包含元素和文本的元素

### 定义复合元素

* 直接对"employee"元素进行声明

  ```
  <xs:element name="employee">
    <xs:complexType>
      <xs:sequence>
        <xs:element name="firstname" type="xs:string"/>
        <xs:element name="lastname" type="xs:string"/>
      </xs:sequence>
    </xs:complexType>
  </xs:element>
  ```

*  "employee" 元素可以使用 type 属性，这个属性的作用是引用要使用的复合类型的名称

  ```
  <xs:element name="employee" type="personinfo"/>
  
  <xs:complexType name="personinfo">
    <xs:sequence>
      <xs:element name="firstname" type="xs:string"/>
      <xs:element name="lastname" type="xs:string"/>
    </xs:sequence>
  </xs:complexType>
  ```

* 在已有的复合元素之上以某个复合元素为基础，然后添加一些元素

  ```
  <xs:element name="employee" type="fullpersoninfo"/>
  
  <xs:complexType name="personinfo">
    <xs:sequence>
      <xs:element name="firstname" type="xs:string"/>
      <xs:element name="lastname" type="xs:string"/>
    </xs:sequence>
  </xs:complexType>
  
  <xs:complexType name="fullpersoninfo">
    <xs:complexContent>
      <xs:extension base="personinfo">
        <xs:sequence>
          <xs:element name="address" type="xs:string"/>
          <xs:element name="city" type="xs:string"/>
          <xs:element name="country" type="xs:string"/>
        </xs:sequence>
      </xs:extension>
    </xs:complexContent>
  </xs:complexType>
  ```

### 复合空元素

一个空的 XML 元素：`<product prodid="1345" />`

```
<xs:element name="product">
  <xs:complexType>
    <xs:complexContent>
      <xs:restriction base="xs:integer">
        <xs:attribute name="prodid" type="xs:positiveInteger"/>
      </xs:restriction>
    </xs:complexContent>
  </xs:complexType>
</xs:element>
```

```
<xs:element name="product">
  <xs:complexType>
    <xs:attribute name="prodid" type="xs:positiveInteger"/>
  </xs:complexType>
</xs:element>
```

```
<xs:element name="product" type="prodtype"/>

<xs:complexType name="prodtype">
  <xs:attribute name="prodid" type="xs:positiveInteger"/>
</xs:complexType>
```

### 复合类型仅包含元素

```
<person>
 <firstname>John</firstname>
 <lastname>Smith</lastname>
</person>
```

```
<xs:element name="person">
 <xs:complexType>
  <xs:sequence>
   <xs:element name="firstname" type="xs:string"/>
   <xs:element name="lastname" type="xs:string"/>
  </xs:sequence>
 </xs:complexType>
</xs:element>
```

```
<xs:element name="person" type="persontype"/>

<xs:complexType name="persontype">
  <xs:sequence>
    <xs:element name="firstname" type="xs:string"/>
    <xs:element name="lastname" type="xs:string"/>
  </xs:sequence>
</xs:complexType>
```

### 仅含文本的复合元素

```
<xs:element name="shoesize">
  <xs:complexType>
    <xs:simpleContent>
      <xs:extension base="xs:integer">
        <xs:attribute name="country" type="xs:string" />
      </xs:extension>
    </xs:simpleContent>
  </xs:complexType>
</xs:element>
```

```
<xs:element name="shoesize" type="shoetype"/>

<xs:complexType name="shoetype">
  <xs:simpleContent>
    <xs:extension base="xs:integer">
      <xs:attribute name="country" type="xs:string" />
    </xs:extension>
  </xs:simpleContent>
</xs:complexType>
```

### 带有混合内容的复合类型

```
<letter>
  Dear Mr.<name>John Smith</name>.
  Your order <orderid>1032</orderid>
  will be shipped on <shipdate>2001-07-13</shipdate>.
</letter>
```

```
<xs:element name="letter">
  <xs:complexType mixed="true">
    <xs:sequence>
      <xs:element name="name" type="xs:string"/>
      <xs:element name="orderid" type="xs:positiveInteger"/>
      <xs:element name="shipdate" type="xs:date"/>
    </xs:sequence>
  </xs:complexType>
</xs:element>
```

## XSD 指示器

有七种指示器：

Order 指示器

- All:规定子元素可以按照任意顺序出现

  ```
  <xs:element name="person">
    <xs:complexType>
      <xs:all>
        <xs:element name="firstname" type="xs:string"/>
        <xs:element name="lastname" type="xs:string"/>
      </xs:all>
    </xs:complexType>
  </xs:element>
  ```

  

- Choice：只可选一个

  ```
  <xs:element name="person">
    <xs:complexType>
      <xs:choice>
        <xs:element name="employee" type="employee"/>
        <xs:element name="member" type="member"/>
      </xs:choice>
    </xs:complexType>
  </xs:element>
  ```

  

- Sequence:按顺序出现

  ```
  <xs:element name="person">
     <xs:complexType>
      <xs:sequence>
        <xs:element name="firstname" type="xs:string"/>
        <xs:element name="lastname" type="xs:string"/>
      </xs:sequence>
    </xs:complexType>
  </xs:element>
  ```

  

Occurrence 指示器

- maxOccurs
- minOccurs

Group 指示器

- Group name
- attributeGroup name