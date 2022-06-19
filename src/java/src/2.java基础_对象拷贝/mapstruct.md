# MapStruct介绍

* mapStructut是一个基于注解的,用来生成类型安全的bean映射类
* 在编译时期Mapstruct会生成接口的实现,基于普通的方法调用,没有反射

* 主要包含了两个组件
  * *org.mapstruct:mapstruct*: 注解
  * *org.mapstruct:mapstruct-processor*: 生成实现类的处理器

# 使用方式

## *Maven configuration*

```
<properties>
    <org.mapstruct.version>1.3.1.Final</org.mapstruct.version>
</properties>
...
<dependencies>
    <dependency>
        <groupId>org.mapstruct</groupId>
        <artifactId>mapstruct</artifactId>
        <version>${org.mapstruct.version}</version>
    </dependency>
</dependencies>

<plugin>
    <groupId>org.apache.maven.plugins</groupId>
    <artifactId>maven-compiler-plugin</artifactId>
    <version>3.8.1</version>
<configuration>
        <source>1.8</source>
        <target>1.8</target>
        <annotationProcessorPaths>
            <path>
                <groupId>org.mapstruct</groupId>
                <artifactId>mapstruct-processor</artifactId>
                <version>${org.mapstruct.version}</version>
            </path>
        </annotationProcessorPaths>
    </configuration>
</plugin>
```



# 入门

## 案例

**源类型**

* 转换的类要符合JavaBean定义
* 同名会自动转换
* @BeanMapping(ignoreByDefault = true) 不会自动匹配,只能显示指定名字字段的对应关系

```java
public class Car {
 
    private String make;
    private int numberOfSeats;
    private CarType type;
}
public enum  CarType {
    BAIDU,
    ALI,
    TENXUN
}
```

**目标类型**

```java
public class CarDto {
 
    private String make;
    private int seatCount;
    private String type;
 }
```

**中间转换类**

```java
@Mapper
public interface CarMapper {
 
    CarMapper INSTANCE = Mappers.getMapper( CarMapper.class );
 
    @Mapping(source = "numberOfSeats", target = "seatCount") // 可以有多个 @Repeatable(Mappings.class)
    @Mapping(source = "numberOfSeats", target = "seatCount")
    CarDto carToCarDto(Car car);
}
```

**测试类**

```java
{
    //given
    Car car = new Car( "Morris", 5, CarType.ALI );

    //when
    CarDto carDto = CarMapper.INSTANCE.carToCarDto( car );

    //then
    assertThat(carDto, notNullValue());
    assertThat(carDto.getSeatCount(),is(5));
    assertThat(carDto.getType(),is("ALI"));
}
```



# [类型转换](https://mapstruct.org/documentation/stable/reference/html/#implicit-type-conversions)

## 基本数据类型转换

如果源类型与 目标类型 不同,会进行隐式转换,或者调用或者创建另一个映射方法

### 包装类型转基本类型

* 所有Java的数值类型会自动转换
* long 转 int会造成精度丢失,*MapperConfig* 注解 的 typeConversionPolicy 方法控制 警告与错误由于向后兼容,默认`ReportingPolicy.IGNORE`



### 所有基本数据类型 转string

* 自动调用相应包装类型的 parse与valueOf

```
int to  string string to int
会自动调用 如下方法
String#valueOf(int) and Integer#parseInt(String)
```

* 可以识别 java.text.DecimalFormat 的

  ```vue
  @Mapper
  public interface CarMapper {
  
      @Mapping(source = "price", numberFormat = "$#.00")
      CarDto carToCarDto(Car car);
  
      @IterableMapping(numberFormat = "$#.00")
      List<String> prices(List<Integer> prices);
  }
  ```



### 枚举转string

默认取 枚举方法名



### 日期类型转换

各种日期之间的转换



```
@Mapper
public interface CarMapper {

    @Mapping(source = "manufacturingDate", dateFormat = "dd.MM.yyyy")
    CarDto carToCarDto(Car car);

    @IterableMapping(dateFormat = "dd.MM.yyyy")
    List<String> stringListToDateList(List<Date> dates);
}
```

### 货币与string

`java.util.Currency` and `String`.

## 引用数据类型转换

```java
@Mapper
public interface CarMapper {

    CarDto carToCarDto(Car car);

    PersonDto personToPersonDto(Person person);
}
```

**遵循原则**

* 如果源和目标 有同样的类型, 值只会简单的从源copy到目的
* 如果类型不一致,查看是否还有另一个映射方法, **参数与源类型相同,返回值与目标类型相同**,则会自动调用这个方法
* 如果没有上述类型方法,则查找内置的转换器,如果有则应用
* 如果没有则 尝试自动生成 转换器
* 如果无法生成转换器 则编译时报错

**扩展**

`@Mapper( disableSubMappingMethodsGeneration = true )`. 禁止自动生成 子映射



## 嵌套bean的映射

```java
//使用 . 可以指定嵌套的映射
@Mapper
public interface FishTankMapper {

    @Mapping(target = "fish.kind", source = "fish.type")
    @Mapping(target = "fish.name", ignore = true) //忽略
    @Mapping(target = "ornament", source = "interior.ornament")
    @Mapping(target = "material.materialType", source = "material")
    @Mapping(target = "quality.report.organisation.name", source = "quality.report.organisationName")
    FishTankDto map( FishTank source );
}
```

```java
@Mapper
public interface FishTankMapperWithDocument {

    @Mapping(target = "fish.kind", source = "fish.type")
    @Mapping(target = "fish.name", expression = "java(\"Jaws\")")
    @Mapping(target = "plant", ignore = true )
    @Mapping(target = "ornament", ignore = true )
    @Mapping(target = "material", ignore = true)
    @Mapping(target = "quality.document", source = "quality.report")
    @Mapping(target = "quality.document.organisation.name", constant = "NoIdeaInc" )
    FishTankWithNestedDocumentDto map( FishTank source );

}
```















## Mapping Composition (experimental)

组合注解:用来处理 多个不同种类的bean可能存在 相同的字段

```
定义在注解上
@Retention(RetentionPolicy.CLASS)
@Mapping(target = "id", ignore = true)
@Mapping(target = "creationDate", expression = "java(new java.util.Date())")
@Mapping(target = "name", source = "groupName")
public @interface ToEntity { }

使用定义的注解
@Mapper
public interface StorageMapper {

    StorageMapper INSTANCE = Mappers.getMapper( StorageMapper.class );

    @ToEntity
    @Mapping( target = "weightLimit", source = "maxWeight")
    ShelveEntity map(ShelveDto source);

    @ToEntity
    @Mapping( target = "label", source = "designation")
    BoxEntity map(BoxDto source);
}

```



## 增加自定义方法

### 使用接口默认方法

```
@Mapper
public interface CarMapper {

    @Mapping(...)
    ...
    CarDto carToCarDto(Car car);

    default PersonDto personToPersonDto(Person person) {
        //hand-written mapping logic
    }
}
```

### 使用抽象类继承

```
@Mapper
public abstract class CarMapper {

    @Mapping(...)
    ...
    public abstract CarDto carToCarDto(Car car);

    public PersonDto personToPersonDto(Person person) {
        //hand-written mapping logic
    }
}
```

## 来自多源

如果多源中字段名有歧义就会报错

```
@Mapper
public interface AddressMapper {

    @Mapping(source = "person.description", target = "description")
    @Mapping(source = "address.houseNo", target = "houseNumber")
    DeliveryAddressDto personAndAddressToDeliveryAddressDto(Person person, Address address);
}
```

## 嵌套的bean属性处理

使用. 表明 将 record对象中所有的属性映射到target

```
@Mapper
 public interface CustomerMapper {

     @Mapping( target = "name", source = "record.name" )
     @Mapping( target = ".", source = "record" )
     @Mapping( target = ".", source = "account" )
     Customer customerDtoToCustomer(CustomerDto customerDto);
 }
```

## 更新使用 源类型 更新目标类型

```
@Mapper
public interface CarMapper {

    void updateCarFromDto(CarDto carDto, @MappingTarget Car car);
}
```



## 直接字段访问映射

支持 public的 字段,没有getter,setter

```
    @InheritInverseConfiguration
    CustomerDto fromCustomer(Customer customer);
```

## 使用构建器





