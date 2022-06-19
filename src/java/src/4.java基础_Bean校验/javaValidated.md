# 校验框架介绍

JSR303 是一套JavaBean参数校验的标准，它定义了很多常用的校验注解，我们可以直接将这些注解加在我们JavaBean的属性上面，就可以在需要校验的时候进行校验了。注解如下：

## JSR303注解

## 空检查

| 注解名        | 验证对象       | 说明                                                         |
| ------------- | -------------- | ------------------------------------------------------------ |
| **@NotEmpty** | 集合类         | ；不能为null，而且长度必须大于0                              |
| **@NotBlank** | String         | 只能作用在String上，不能为null，而且调用trim()后，长度必须大于0 |
| **@NotNull**  | 用在基本类型上 | 不能为null，但可以为empty。                                  |
| **@Null**     | 用在基本类型上 | 被注释的元素必须为null                                       |

## **长度检查**

| 注解名                  | 验证对象                    | 说明                     |
| ----------------------- | --------------------------- | ------------------------ |
| **@Size(min=,max=)**    | Array,Collection,Map,String | 长度是否在给定的范围之内 |
| **@Length(min=, max=)** | String 类型                 | 长度是否在给定的范围之内 |

## Booelan检查

| 注解名           | 验证对象 | 说明                          |
| ---------------- | -------- | ----------------------------- |
| **@AssertTrue**  | bool     | 验证 Boolean 对象是否为 true  |
| **@AssertFalse** | bool     | 验证 Boolean 对象是否为 false |

## 日期检查

| 注解名       | 验证对象      | 说明                                     |
| ------------ | ------------- | ---------------------------------------- |
| **@Past**    | date,Calendar | 验证是否在当前时间之前                   |
| **@Future**  | date,Calendar | 验证 String 对象是否符合正则表达式的规则 |
| **@Pattern** | String 对象   | 是否符合正则表达式的规则                 |

## 数值检查

| 注解名                          | 验证对象              | 说明                                                         |
| ------------------------------- | --------------------- | ------------------------------------------------------------ |
| **@Min**,**@Max**               | Number 和 String 对象 | 是否大等于指定的值                                           |
| **@DecimalMax**,**@DecimalMin** | BigDecimal,数值类型   |                                                              |
| **@Digits**                     | Number 和 String      | 验证 Number 和 String 的构成是否合法                         |
| **@Digits(integer=,fraction=)** | 字符串                | 验证字符串是否是符合指定格式的数字，interger指定整数精度，fraction指定小数精度。 |



Hibernate validator 在JSR303的基础上对校验注解进行了扩展，扩展注解如下：

## Hibernate扩展注解

| 注解名    | 说明                       |
| --------- | -------------------------- |
| @Email    | 被注释的元素必须是电子邮箱 |
| @Length   | 字符串的长度               |
| @NotEmpty | 字符串非空                 |
| @Range    | 指定范围                   |

Spring validtor 同样扩展了jsr303,并实现了方法参数和返回值的校验

Spring 提供了MethodValidationPostProcessor类，用于对方法的校验



# 声明 Java Bean 约束

## 字段级别约束

```java
@NotNull
private String manufacturer;
```

## 属性级别约束

```java
@NotNull
public String getManufacturer(){
  return manufacturer;
}
```

## 容器级别约束

```java
private Map<@NotNull FuelConsumption, @MaxAllowedFuelConsumption Integer> fuelConsumption = new HashMap<>();
```

## 类级别约束

在这种情况下，验证的对象不是单个属性，而是完整的对象。如果验证依赖于对象的多个属性之间的相关性，则类级约束非常有用。
如：汽车中，乘客数量不能大于座椅数量，否则超载

```java
@ValidPassengerCount
public class Car {

    private int seatCount;

    private List<Person> passengers;

    //...
}
```

## 约束继承

当一个类继承/实现另一个类时，父类声明的所有约束也会应用在子类继承的对应属性上。
如果方法`重写`，约束注解将会聚合，也就是此方法父类和子类声明的约束都会起作用。



## 级联验证

`Bean Validation API` 不仅允许验证单个类实例，也支持级联验证。

只需使用 `@Valid` 修饰对象属性的引用，则对象属性中声明的所有约束也会起作用。

```java
public class Car {
    @NotNull
    @Valid
    private Person driver;
    //...
}
public class Person {
    @NotNull
    private String name;
    //...
}
```

# 声明方法约束

## 参数约束

通过向方法或构造函数的参数添加约束注解来指定方法或构造函数的`前置条件`，官方示例如下：

```java
public RentalStation(@NotNull String name){}

public void rentCar(@NotNull Customer customer,
                          @NotNull @Future Date startDate,
                          @Min(1) int durationInDays){}
```

## 返回值约束

通过在方法体上添加约束注解来给方法或构造函数指定`后置条件`，官方示例如下：

```java
public class RentalStation {
    @ValidRentalStation
    public RentalStation() {
        //...
    }
    @NotNull
    @Size(min = 1)
    public List<@NotNull Customer> getCustomers() {
        //...
        return null;
    }
}
```

此示例指定了三个约束：

- 任何新创建的 RentalStation 对象都必须满足 @validRentalStation 约束
- getCustomers() 返回的客户列表不能为空，并且必须至少包含 1 个元素
- getCustomers() 返回的客户列表不能包含空对象

## 级联约束

类似于 JavaBeans 属性的级联验证，`@Valid` 注解可用于标记方法参数和返回值的级联验证。

类似于 javabeans 属性的级联验证（参见第 2.1.6 节“对象图”），@valid 注释可用于标记可执行参数和级联验证的返回值。当验证用@valid 注释的参数或返回值时，也会验证在参数或返回值对象上声明的约束。
而且，也可用在容器元素中。

```java
public class Garage {
    public boolean checkCars(@NotNull List<@Valid Car> cars) {
        //...
        return false;
    }
}
```

## 继承验证

当在继承体系中声明方法约束时，必须了解两个规则：

- 方法调用方要满足前置条件不能在子类型中得到加强 参数
- 方法调用方要保证后置条件不能再子类型中被削弱 返回值

这些规则是由子类行为概念所决定的：在使用类型 T 的任何地方，也能在不改变程序行为的情况下使用 T 的子类。

当两个类分别有一个同名且形参列表相同的方法，而另一个类用一个方法重写/实现上述两个类的同名方法时，这两个父类的同名方法上不能有任何参数约束，因为不管怎样都会与上述规则冲突。
示例：

```java
public interface Vehicle {
  void drive(@Max(75) int speedInMph);
}
public interface Car {
  void drive(int speedInMph);
}

public class RacingCar implements Car, Vehicle {
  @Override
  public void drive(int speedInMph) {
      //...
  }
}
```

# 分组约束

`注意`：上述的 22 个约束注解都有 `groups` 属性。当不指定 groups 时，默认为 `Default` 分组。

`JSR` 规范支持手动校验，不直接支持使用注解校验，不过 `spring` 提供了分组校验注解扩展支持，即：`@Validated`，参数为 group 类集合

## 分组继承

在某些场景下，需要定义一个组，它包含其它组的约束，可以用分组继承。
如：

```java
public class SuperCar extends Car {
    @AssertTrue(
            message = "Race car must have a safety belt",
            groups = RaceCarChecks.class
    )
    private boolean safetyBelt;
    // getters and setters ...
}
public interface RaceCarChecks extends Default {}
```

但因为此处，是想 `Default` 分组一直都要校验

```java
public interface DefaultInherGroup extends Default {}
```



## 定义分组序列

默认情况下，不管约束是属于哪个分组，它们的计算是没有特定顺序的，而在某些场景下，控制约束的计算顺序是有用的。
如：先检查汽车的默认约束，再检查汽车的性能约束，最后在开车前，检查驾驶员的实际约束。
可以定义一个接口，并用 `@GroupSequence` 来定义需要验证的分组的序列。

```java
@GroupSequence({ Default.class, CarChecks.class, DriverChecks.class })
public interface OrderedChecks {}
```

此分组用法与其它分组一样，只是此分组拥有按分组顺序校验的功能

> 定义序列的组和组成序列的组不能通过级联序列定义或组继承直接或间接地参与循环依赖关系。如果对包含此类循环的组计算，则会引发 GroupDefinitionException。

## 重新定义默认分组序列

#### @GroupSequence

`@GroupSequence` 除了定义分组序列外，还允许重新定义指定类的默认分组。为此，只需将`@GroupSequence` 添加到类中，并在注解中用指定序列的分组替换 `Default` 默认分组。

```java
@GroupSequence({ RentalChecks.class, CarChecks.class, RentalCar.class })
public class RentalCar extends Car {}
```

在验证约束时，直接把其当做默认分组方式来验证

#### @GroupSequenceProvider

注意：此为 hibernate-validator 提供，JSR 规范不支持

可用于根据对象状态动态地重新定义默认分组序列。
需要做两步：

1. 实现接口：DefaultGroupSequenceProvider
2. 在指定类上使用 @GroupSequenceProvider，并指定 value 为上一步的类

```java
public class RentalCarGroupSequenceProvider
        implements DefaultGroupSequenceProvider<RentalCar> {
    @Override
    public List<Class<?>> getValidationGroups(RentalCar car) {
        List<Class<?>> defaultGroupSequence = new ArrayList<Class<?>>();
        defaultGroupSequence.add( RentalCar.class );
        if ( car != null && !car.isRented() ) {
            defaultGroupSequence.add( CarChecks.class );
        }
        return defaultGroupSequence;
    }
}
@GroupSequenceProvider(RentalCarGroupSequenceProvider.class)
public class RentalCar extends Car {
    @AssertFalse(message = "The car is currently rented out", groups = RentalChecks.class)
    private boolean rented;
    public RentalCar(String manufacturer, String licencePlate, int seatCount) {
        super( manufacturer, licencePlate, seatCount );
    }
    public boolean isRented() {
        return rented;
    }
    public void setRented(boolean rented) {
        this.rented = rented;
    }
}
```

## 分组转换

如果你想把与汽车相关的检查和驾驶员检查一起验证呢？当然，您可以显式地指定验证多个组，但是如果您希望将这些验证作为默认组验证的一部分进行，该怎么办？这里@ConvertGroup 开始使用，它允许您在级联验证期间使用与最初请求的组不同的组。

在可以使用 @Valid 的任何地方，都能定义分组转换，也可以在同一个元素上定义多个分组转换
必须满足以下限制：

- @ConvertGroup 只能与 @Valid 结合使用。如果不是，则抛出 ConstraintDeclarationException。
- 在同一元素上有多个 from 值相同的转换规则是不合法的。在这种情况下，将抛出 ConstraintDeclarationException。
- from 属性不能引用分组序列。在这种情况下会抛出 ConstraintDeclarationException



```java
// 当 driver 为 null 时，不会级联验证，使用的是默认分组，当级联验证时，使用的是 DriverChecks 分组 @Valid @ConvertGroup(from = Default.class, to = DriverChecks.class) private Driver driver;
```







# 全局异常处理

## **引包**

```xml
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-validation</artifactId>
    </dependency>
```
## **全局异常处理**

```java
@ControllerAdvice
public class WebExceptionHandler {
　　 //处理Get请求中 使用@Valid 验证路径中请求实体校验失败后抛出的异常，详情继续往下看代码
    @ExceptionHandler(BindException.class)
    @ResponseBody
    public ResponseVO BindExceptionHandler(BindException e) {
        String message = e.getBindingResult().getAllErrors().stream().map(DefaultMessageSourceResolvable::getDefaultMessage).collect(Collectors.joining());
        return new ResponseVO(message);
    }

    //处理请求参数格式错误 @RequestParam上validate失败后抛出的异常是javax.validation.ConstraintViolationException
    @ExceptionHandler(ConstraintViolationException.class)
    @ResponseBody
    public ResponseVO ConstraintViolationExceptionHandler(ConstraintViolationException e) {
        String message = e.getConstraintViolations().stream().map(ConstraintViolation::getMessage).collect(Collectors.joining());
        return new ResponseVO(message);
    }

    //处理请求参数格式错误 @RequestBody上validate失败后抛出的异常是MethodArgumentNotValidException异常。
    @ExceptionHandler(MethodArgumentNotValidException.class)
    @ResponseBody
    public ResponseVO MethodArgumentNotValidExceptionHandler(MethodArgumentNotValidException e) {
        String message = e.getBindingResult().getAllErrors().stream().map(DefaultMessageSourceResolvable::getDefaultMessage).collect(Collectors.joining());
        return new ResponseVO(message);
    }
}
```

**多异常统一处理**

```java
// @RestControllerAdvice

    /*  数据校验处理 */
    @ExceptionHandler({BindException.class, ConstraintViolationException.class})
    public String validatorExceptionHandler(Exception e) {
        String msg = e instanceof BindException ? msgConvertor(((BindException) e).getBindingResult())
            : msgConvertor(((ConstraintViolationException) e).getConstraintViolations());

        return msg;
    }

    /**
     * 校验消息转换拼接
     *
     * @param bindingResult
     * @return
     */
    public static String msgConvertor(BindingResult bindingResult) {
        List<FieldError> fieldErrors = bindingResult.getFieldErrors();
        StringBuilder sb = new StringBuilder();
        fieldErrors.forEach(fieldError -> sb.append(fieldError.getDefaultMessage()).append(","));

        return sb.deleteCharAt(sb.length() - 1).toString().toLowerCase();
    }

    private String msgConvertor(Set<ConstraintViolation<?>> constraintViolations) {
        StringBuilder sb = new StringBuilder();
        constraintViolations.forEach(violation -> sb.append(violation.getMessage()).append(","));

        return sb.deleteCharAt(sb.length() - 1).toString().toLowerCase();
    }

```



# 嵌套对象的校验

```java
@Setter
@Getter
public class BuyFlowerRequest {

    @NotEmpty(field = "花名")
    private String name;

    @Min(field = "价格", value = 1)
    private int price;

    @NotNull
    private List<PayType> payTypeList;

} 

@Setter
@Getter
public class PayType {

    @Valid
    @Min(value = 1)
    private int payType;

    @Valid
    @Min(value = 1)
    private int payAmount;

}
```



# 三种校验方式

## 全局异常处理

在Controller方法参数前加@Valid注解——校验不通过时直接抛异常

## 用户自行判断并处理

在Controller方法参数前加@Valid注解，参数后面定义一个BindingResult类型参数——执行时会将校验结果放进bindingResult里面，用户自行判断并处理

```java
@PostMapping("/test2")
	public Object test2(@RequestBody @Valid User user, BindingResult bindingResult) {
		// 参数校验
		if (bindingResult.hasErrors()) {
			String messages = bindingResult.getAllErrors()
				.stream()
				.map(ObjectError::getDefaultMessage)
				.reduce((m1, m2) -> m1 + "；" + m2)
				.orElse("参数输入有误！");
			throw new IllegalArgumentException(messages);
		}
		return "操作成功！";


	}
```

##### BindingResult 的使用

`BindingResult`必须跟在被校验参数之后,若被校验参数之后没有`BindingResult`对象，将会抛出`BindException`。



## 手动验证

用户手动调用对应API执行校验——Validation.buildDefault ValidatorFactory().getValidator().validate(xxx)

```java
    /**
 * 用户手动调用对应API执行校验
 * @param user
 * @return
 */
@PostMapping("/test3")
public Object test3(@RequestBody User user) {
	// 参数校验
	validate(user);
	
	return "操作成功！";
}
 
private void validate(@Valid User user) {
	Set<ConstraintViolation<@Valid User>> validateSet = Validation.buildDefaultValidatorFactory()
			.getValidator()
			.validate(user, new Class[0]);
		if (!CollectionUtils.isEmpty(validateSet)) {
			String messages = validateSet.stream()
				.map(ConstraintViolation::getMessage)
				.reduce((m1, m2) -> m1 + "；" + m2)
				.orElse("参数输入有误！");
			throw new IllegalArgumentException(messages);
			
		}
}
```
**获取Validtor对象**

```java
import org.hibernate.validator.HibernateValidator;
import org.springframework.util.ClassUtils;
import org.springframework.validation.BindException;
import org.springframework.validation.DataBinder;
import org.springframework.validation.SmartValidator;
import org.springframework.validation.beanvalidation.SpringValidatorAdapter;

import javax.validation.ConstraintViolation;
import javax.validation.ConstraintViolationException;
import javax.validation.Validation;
import javax.validation.Validator;
import java.util.Set;

/**
 * hibernate-validator校验工具类
 */
public class ValidatorUtils {
    private static Validator validator;
    private static SmartValidator validatorAdapter;

    static {
        // 快速返回模式
        validator = Validation.byProvider(HibernateValidator.class)
            .configure()
            .failFast(true)
            .buildValidatorFactory()
            .getValidator();
    }

    public static Validator getValidator() {
        return validator;
    }

    private static SmartValidator getValidatorAdapter(Validator validator) {
        if (validatorAdapter == null) {
            validatorAdapter = new SpringValidatorAdapter(validator);
        }
        return validatorAdapter;
    }

    /**
     * 校验参数，用于普通参数校验 [未测试！]
     *
     * @param
     */
    public static void validateParams(Object... params) {
        Set<ConstraintViolation<Object>> constraintViolationSet = validator.validate(params);

        if (!constraintViolationSet.isEmpty()) {
            throw new ConstraintViolationException(constraintViolationSet);
        }
    }

    /**
     * 校验对象
     *
     * @param object
     * @param groups
     * @param <T>
     */
    public static <T> void validate(T object, Class<?>... groups) {
        Set<ConstraintViolation<T>> constraintViolationSet = validator.validate(object, groups);

        if (!constraintViolationSet.isEmpty()) {
            throw new ConstraintViolationException(constraintViolationSet);
        }
    }

    /**
     * 校验对象
     * 使用与 Spring 集成的校验方式。
     * 
     * @param object 待校验对象
     * @param groups 待校验的组
     * @throws BindException
     */
    public static <T> void validateBySpring(T object, Class<?>... groups)
        throws BindException {
        DataBinder dataBinder = getBinder(object);
        dataBinder.validate((Object[]) groups);

        if (dataBinder.getBindingResult().hasErrors()) {
            throw new BindException(dataBinder.getBindingResult());
        }
    }

    private static <T> DataBinder getBinder(T object) {
        DataBinder dataBinder = new DataBinder(object, ClassUtils.getShortName(object.getClass()));
        dataBinder.setValidator(getValidatorAdapter(validator));
        return dataBinder;
    }

}
```



# 快速失败

**校验完后不继续校验**

```java
@Configuration
public class WebConfig {
    @Bean
    public Validator validator() {
        ValidatorFactory validatorFactory = Validation.byProvider(HibernateValidator.class)
                .configure()
                //failFast的意思只要出现校验失败的情况，就立即结束校验，不再进行后续的校验。
                .failFast(true)
                .buildValidatorFactory();

        return validatorFactory.getValidator();
    }

    @Bean
    public MethodValidationPostProcessor methodValidationPostProcessor() {
        MethodValidationPostProcessor methodValidationPostProcessor = new MethodValidationPostProcessor();
        methodValidationPostProcessor.setValidator(validator());
        return methodValidationPostProcessor;
    }
}
```

# 绑定多个校验对象

```java
@PostMapping("save")
public void v1(@RequestBody @Valid AppUser appUser,BindingResult result,@RequestBody @Valid AppUser appUser2,BindingResult result2){
      if(result.hasErrors()){
            for (ObjectError error : result.getAllErrors()) {
                System.out.println(error.getDefaultMessage());
            }
        }
}
```



# message属性国际化

## 指定properties

`message`中填写国际化消息的`code`，在抛出异常时根据`code`处理一下就好了。

```java
    @GetMapping("/room")
    @Validated
    public String validator(@NotNull(message = "demo.message.notnull") String name) {
        if (result.hasErrors()) {
            return result.getFieldError().getDefaultMessage();
        }
        return "ok";
    }
```

idea乱码 勾选 *transparent native-to-ascii conversion*

**在/resources的根目录下添加上ValidationMessages.properties文件**

​	国际化配置文件必须放在classpath的根目录下，即src/java/resources的根目录下。
 国际化配置文件必须以ValidationMessages开头，比如ValidationMessages.properties 或者 ValidationMessages_en.properties。

## 自定义properties文件

重写LocalValidatorFactoryBean

```java
@Configuration
public class ValidatorConfiguration extends WebMvcConfigurationSupport {
    @Autowired
    private MessageSource messageSource;

    @Override
    public Validator getValidator() {
        return validator();
    }

    @Bean
    public Validator validator() {
        LocalValidatorFactoryBean validator = new LocalValidatorFactoryBean();
        validator.setValidationMessageSource(messageSource);
        return validator;
    }
}
```







# 自定义validtor

## Bean Validation 注解规范

`Bean Validation API` 规范要求任何约束注解定义以下要求：

一个 `message` 属性：在违反约束的情况下返回一个默认 key 以用于创建错误消息

一个 `groups` 属性：允许指定此约束所属的验证分组。必须默认是一个空 Class 数组

一个 `payload` 属性：能被 Bean Validation API 客户端使用，以自定义一个注解的 payload 对象。API 本身不使用此属性。自定义 payload 可以是用来定义严重程度。如下：

```java
public class Severity{
  public interface Info extends Payload{}
  public interface Error extends Payload{}
}
public class ContactDetails{
  @NotNull(message="名字必填", payload=Severity.Error.class)
  private String name;
  
  @NotNull(message="手机号没有指定，但不是必填项", payload=Severity.Info.class)
  private String phoneNumber;
}
```

然后客户端在 ContactDetails 实例验证之后，可以通过 `ConstraintViolation.getConstraintDescriptor().getPayload()` 获取 severity ，然后根据 severity 调整其行为。

此外，约束注解上还修饰了一些元注解：

- @Target：指定此注解支持的元素类型，比如：FIELD（属性）、METHOD（方法）等
- @Rentention(RUNTIME)：指定此类型的注解将在运行时通过反射方式可用
- @Constraint()：标记注解的类型为约束，指定注解所使用的验证器（写验证逻辑的类），如果约束可以用在多种数据类型中，则每种数据类型对应一个验证器。
- @Documented：用此注解会被包含在使用方的 JavaDoc 中
- @Repeatable(List.class)：指示注解可以在相同的位置重复多次，通常具有不同的配置。List 包含注解类型。

## 验证器

### 示例

```java
public class CheckCaseValidator implements ConstraintValidator<CheckCase, String> {
    private CaseMode caseMode;
    @Override
    public void initialize(CheckCase constraintAnnotation) {
        this.caseMode = constraintAnnotation.value();
    }
    @Override
    public boolean isValid(String object, ConstraintValidatorContext constraintContext) {
        if ( object == null ) {
            return true;
        }
        if ( caseMode == CaseMode.UPPER ) {
            return object.equals( object.toUpperCase() );
        }else {
            return object.equals( object.toLowerCase() );
        }
    }
}
```

### **`ConstraintValidator` 指定了两个泛型类型：**

1. 第一个是指定需要验证的注解类
2. 第二个是指定要验证的数据类型，当注解支持多种类型时，就要写多个实现类，并分别指定对应的类型

### **需要实现两个方法**：

- `initialize()` 让你可以获取到使用注解时所指定的参数（可以将它们保存起来以供下一步使用）
- `isValid()` 包含实际的校验逻辑。注意：Bean Validation 规范建议将 null 值视为有效值。如果一个元素 null 不是一个有效值，则应该显示的用 @NotNull 标注。

### **isValid() 方法中的 ConstraintValidatorContext 对象参数：**

当应用指定约束验证器时，提供上下文数据和操作。

此对象至少有一个 `ConstraintViolation`

**示例**

官方示例展示了禁用默认消息并自定义了一个错误消息提示。

```java
@Override
public boolean isValid(String object, ConstraintValidatorContext constraintContext) {
    if ( object == null ) {
        return true;
    }

    boolean isValid;
    if ( caseMode == CaseMode.UPPER ) {
        isValid = object.equals( object.toUpperCase() );
    }
    else {
        isValid = object.equals( object.toLowerCase() );
    }

    if ( !isValid ) {
    // 禁用默认 ConstraintViolation，并自定义一个
        constraintContext.disableDefaultConstraintViolation();
        constraintContext.buildConstraintViolationWithTemplate(
                "{org.hibernate.validator.referenceguide.chapter06." +
                "constraintvalidatorcontext.CheckCase.message}"
        )
        .addConstraintViolation();
    }

    return isValid;
}
```

### 传递 payload 参数给验证器

**官方示例**

```java
HibernateValidatorFactory hibernateValidatorFactory = Validation.byDefaultProvider()
        .configure()
        .buildValidatorFactory()
        .unwrap( HibernateValidatorFactory.class );

Validator validator = hibernateValidatorFactory.usingContext()
        .constraintValidatorPayload( "US" )
        .getValidator();

// [...] US specific validation checks
validator = hibernateValidatorFactory.usingContext()
        .constraintValidatorPayload( "FR" )
        .getValidator();


public class ZipCodeValidator implements ConstraintValidator<ZipCode, String> {

    public String countryCode;

    @Override
    public boolean isValid(String object, ConstraintValidatorContext constraintContext) {
        if ( object == null ) {
            return true;
        }

        boolean isValid = false;

        String countryCode = constraintContext
                .unwrap( HibernateConstraintValidatorContext.class )
                .getConstraintValidatorPayload( String.class );

        if ( "US".equals( countryCode ) ) {
            // checks specific to the United States
        }
        else if ( "FR".equals( countryCode ) ) {
            // checks specific to France
        }
        else {
            // ...
        }

        return isValid;
    }
}
```

### message

**当违反约束时，应该用到的消息**
需要定义一个 `ValidationMessages.properties`文件，并记录以下内容：

```properties
# org.hibernate.validator.referenceguide.chapter06.CheckCase 是注解 CheckCase 的全类名
org.hibernate.validator.referenceguide.chapter06.CheckCase.message=Case mode must be {value}.
```

## 类级别的约束

```java
public class ValidPassengerCountValidator
        implements ConstraintValidator<ValidPassengerCount, Car> {

    @Override
    public void initialize(ValidPassengerCount constraintAnnotation) {}

    @Override
    public boolean isValid(Car car, ConstraintValidatorContext constraintValidatorContext) {
        if ( car == null ) {
            return true;
        }
        // 用来验证两个属性之间必须满足一种关系
        // 验证乘客数量不能大于座椅数量
        boolean isValid = car.getPassengers().size() <= car.getSeatCount();

        if ( !isValid ) {
            constraintValidatorContext.disableDefaultConstraintViolation();
            constraintValidatorContext
                    .buildConstraintViolationWithTemplate( "{my.custom.template}" )
                    .addPropertyNode( "passengers" ).addConstraintViolation();
        }

        return isValid;
    }
}
```

## 组合约束

### 示例

```java
@NotNull
@Size(min = 2, max = 14)
@CheckCase(CaseMode.UPPER)
@Target({ METHOD, FIELD, ANNOTATION_TYPE, TYPE_USE })
@Retention(RUNTIME)
@Constraint(validatedBy = { })
@Documented
public @interface ValidLicensePlate {
    String message() default "{org.hibernate.validator.referenceguide.chapter06." +
            "constraintcomposition.ValidLicensePlate.message}";

    Class<?>[] groups() default { };

    Class<? extends Payload>[] payload() default { };
}
```

### 遇到违反一个约束即返回

一个注解拥有多个注解的功能，而且此组合注解通常不需要再指定验证器。此注解验证之后会得到违反所有约束的集合，如果想违反其中一个约束之后就有对应的违约信息，可以使用 `@ReportAsSingleViolation`

```java
//...
@ReportAsSingleViolation
public @interface ValidLicensePlate {

    String message() default "{org.hibernate.validator.referenceguide.chapter06." +
            "constraintcomposition.reportassingle.ValidLicensePlate.message}";

    Class<?>[] groups() default { };

    Class<? extends Payload>[] payload() default { };
}
```





# 示例

## 自定义简单约束

三个步骤：

- 创建一个约束注解
- 实现一个验证器
- 定义一个默认的错误消息

1. 定义注解

   ```java
   
   @Target( { ElementType.METHOD, ElementType.FIELD, ElementType.ANNOTATION_TYPE, ElementType.CONSTRUCTOR, ElementType.PARAMETER })
   @Constraint(validatedBy = { NotNullValidator.class })
   @Retention(RetentionPolicy.RUNTIME)
   public @interface NotNull {
   
       String field() default "";
   
       String message() default "{field} can not be null";
   
       Class<?>[] groups() default {};
   
       Class<? extends Payload>[] payload() default {};
   }
   
   ```

2. 定义注解处理类

   ```java
   package com.weisanju.validtortest.common;
   
   import javax.validation.ConstraintValidator;
   import javax.validation.ConstraintValidatorContext;
   
   public class NotNullValidator implements ConstraintValidator<NotNull, Object> {
   
       @Override
       public void initialize(NotNull annotation) {
           System.out.println(annotation);
       }
   
       @Override
       public boolean isValid(Object str, ConstraintValidatorContext constraintValidatorContext) {
           System.out.println(str);
           return str != null;
       }
   
   }
   
   ```







# spring validation实现关键代码



## @**RequestBody**

```java
public Object resolveArgument(MethodParameter parameter, ModelAndViewContainer mavContainer, NativeWebRequest webRequest, WebDataBinderFactory binderFactory) throws Exception {
    Object arg = this.readWithMessageConverters(webRequest, parameter, parameter.getGenericParameterType());
    String name = Conventions.getVariableNameForParameter(parameter);
    WebDataBinder binder = binderFactory.createBinder(webRequest, arg, name);
    if (arg != null) {
        this.validateIfApplicable(binder, parameter);
        if (binder.getBindingResult().hasErrors() && this.isBindExceptionRequired(binder, parameter)) {
            throw new MethodArgumentNotValidException(parameter, binder.getBindingResult());
        }
    }

    mavContainer.addAttribute(BindingResult.MODEL_KEY_PREFIX + name, binder.getBindingResult());
    return arg;
}
```

## @ModelAttibute

```java
public final Object resolveArgument(MethodParameter parameter, ModelAndViewContainer mavContainer, NativeWebRequest webRequest, WebDataBinderFactory binderFactory) throws Exception {
    String name = ModelFactory.getNameForParameter(parameter);
    Object attribute = mavContainer.containsAttribute(name) ? mavContainer.getModel().get(name) : this.createAttribute(name, parameter, binderFactory, webRequest);
    if (!mavContainer.isBindingDisabled(name)) {
        ModelAttribute ann = (ModelAttribute)parameter.getParameterAnnotation(ModelAttribute.class);
        if (ann != null && !ann.binding()) {
            mavContainer.setBindingDisabled(name);
        }
    }

    WebDataBinder binder = binderFactory.createBinder(webRequest, attribute, name);
    if (binder.getTarget() != null) {
        if (!mavContainer.isBindingDisabled(name)) {
            this.bindRequestParameters(binder, webRequest);
        }

        this.validateIfApplicable(binder, parameter);
        if (binder.getBindingResult().hasErrors() && this.isBindExceptionRequired(binder, parameter)) {
            throw new BindException(binder.getBindingResult());
        }
    }

    Map<String, Object> bindingResultModel = binder.getBindingResult().getModel();
    mavContainer.removeAttributes(bindingResultModel);
    mavContainer.addAllAttributes(bindingResultModel);
    return binder.convertIfNecessary(binder.getTarget(), parameter.getParameterType(), parameter);
}
```

## 为什么 `BindingResult` 接收不到简单对象的校验信息？

注入实体对象时使用`ModelAttributeMethodProcessor`而注入 String 对象使用`AbstractNamedValueMethodArgumentResolver`

而正是这个差异导致了`BindingResult`无法接受到简单对象(简单的入参参数类型)的校验信息。



```java
public final Object resolveArgument(MethodParameter parameter, @Nullable ModelAndViewContainer mavContainer,
            NativeWebRequest webRequest, @Nullable WebDataBinderFactory binderFactory) throws Exception {
            
        // bean 参数绑定和校验
        WebDataBinder binder = binderFactory.createBinder(webRequest, attribute, name);
        
        // 参数校验
        validateIfApplicable(binder, parameter);
        // 校验结果包含错误，并且该对象后不存在 BindingResult 对象，就抛出异常
        if (binder.getBindingResult().hasErrors() && isBindExceptionRequired(binder, parameter)) {
            throw new BindException(binder.getBindingResult());
        }

        // 在对象后注入 BindingResult 对象
        Map<String, Object> bindingResultModel = bindingResult.getModel();
        mavContainer.removeAttributes(bindingResultModel);
        mavContainer.addAllAttributes(bindingResultModel);
    }
```



```java
    // HandlerMethodArgumentResolverComposite.class
    public Object resolveArgument(MethodParameter parameter, @Nullable ModelAndViewContainer mavContainer,
            NativeWebRequest webRequest, @Nullable WebDataBinderFactory binderFactory) throws Exception {
        // 获取 parameter 参数的解析器
        HandlerMethodArgumentResolver resolver = getArgumentResolver(parameter);
        // 调用解析器获取参数
        return resolver.resolveArgument(parameter, mavContainer, webRequest, binderFactory);
    }
    
    // 获取 parameter 参数的解析器
    private HandlerMethodArgumentResolver getArgumentResolver(MethodParameter parameter) {
        // 从缓存中获取参数对应的解析器
        HandlerMethodArgumentResolver result = this.argumentResolverCache.get(parameter);
        for (HandlerMethodArgumentResolver methodArgumentResolver : this.argumentResolvers) {
            // 解析器是否支持该参数类型
            if (methodArgumentResolver.supportsParameter(parameter)) {
                result = methodArgumentResolver;
                this.argumentResolverCache.put(parameter, result);
                break;
            }
        }
        return result;
    }
```



**简单参数类型检验**

```java
// MethodValidationInterceptor.class

public Object invoke(MethodInvocation invocation) throws Throwable {
        ExecutableValidator execVal = this.validator.forExecutables();
        // 校验参数
        try {
            result = execVal.validateParameters(
                    invocation.getThis(), methodToValidate, invocation.getArguments(), groups);
        }
        catch (IllegalArgumentException ex) {
            // 解决参数错误异常、再次校验
            methodToValidate = BridgeMethodResolver.findBridgedMethod(
                    ClassUtils.getMostSpecificMethod(invocation.getMethod(), invocation.getThis().getClass()));
            result = execVal.validateParameters(
                    invocation.getThis(), methodToValidate, invocation.getArguments(), groups);
        }
        if (!result.isEmpty()) {
            throw new ConstraintViolationException(result);
        }
        
        // 执行结果
        Object returnValue = invocation.proceed();
        
        // 校验返回值
        result = execVal.validateReturnValue(invocation.getThis(), methodToValidate, returnValue, groups);
        if (!result.isEmpty()) {
            throw new ConstraintViolationException(result);
        }

        return returnValue;
    }
```