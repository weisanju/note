# java注解

详见 [java注解](../java基础/注解.md)





# spring组合注解

* @AliasFor是用于为注解属性声明别名的注解，从Spring Framework 4.2开始，核心Spring中的几个注释已更新为使用@AliasFor配置其内部属性别名。
* 通过  @AliasFor 注解 实现了 Spring的组合注解功能



# 别名  `@AliasFor`

在 Spring 中别名可以分为以下几类：

* 显式别名（xplicit Aliases） 

    如果一个注解中的两个成员通过 @AliasFor声明后互为别名，那么它们是显式别名

    ![显示别名](/images/xplicit_aliases.png)

    

    

* 隐式别名（Implicit Aliases）

    如果一个注解中的两个或者更多成员通过@AliasFor声明去覆盖同一个元注解的成员值，它们就是隐式别名

    ![隐式别名](/images/implicit_aliaes.png)

* 传递隐式别名（Transitive Implicit Aliases）

    如果一个注解中的两个或者更多成员通过@AliasFor声明去覆盖元注解中的不同成员，但是实际上因为[覆盖的传递性](https://en.wikipedia.org/wiki/Transitive_relation)导致最终覆盖的是元注解中的同一个成员，那么它们就是传递隐式别名

    ![传递隐式别名](/images/transitive_Implicit_Aliases.jpg)

    

以上三类都需要满足以下条件：

* 属性类型相同

* 属性方法必须存在默认值

* 属性默认值必须相同



# 属性覆盖原理

> java本身不会执行 @AliasFor语义，通过AnnotatedElementUtils 支持实现

* 与Java中的任何注释一样，仅仅是@AliasFor本身的存在不会强制执行别名语义。要强制执行别名语义，必须通过AnnotationUtils等类 中的方法加载注解。

* 在幕后，Spring将通过将注释包装在一个动态代理中来合成注解，该代理透明地为使用@AliasFor注解的注解属性强制执行属性别名语义。

* 类似地，当在注解层次结构中使用@AliasFor时，AnnotatedElementUtils**支持显式元注解属性重写**。

* 通常，您不需要自己手动合成注解，因为当在Spring管理的组件上查找注解时，Spring将透明地为您合成注解。到了Spring5.2则通过MergedAnnotations加载注解。

```java
@Test
@GetMapping(value = "/GetMapping", consumes = MediaType.APPLICATION_JSON_VALUE)
public void test() throws NoSuchMethodException {
    Method method = ReflectUtils.findDeclaredMethod(
            AliasForTest.class, "test", null);

    // AnnotationUtils 不支持注解属性覆盖
    RequestMapping requestMappingAnn1 = AnnotationUtils.getAnnotation(method, RequestMapping.class);
    Assert.assertEquals(new String[]{}, requestMappingAnn1.value());
    Assert.assertEquals(new String[]{}, requestMappingAnn1.consumes());

    // AnnotatedElementUtils 支持注解属性覆盖
    RequestMapping requestMappingAnn2 = AnnotatedElementUtils.getMergedAnnotation(method, RequestMapping.class);
    Assert.assertEquals(new String[]{"/GetMapping"}, requestMappingAnn2.value());
    Assert.assertEquals(new String[]{MediaType.APPLICATION_JSON_VALUE}, requestMappingAnn2.consumes());
}
```

# AnnotationUtils 源码分析

AnnotationUtils 解决 **注解别名**，**包括显式别名**、**隐式别名**、**传递的隐式别名**，还可以查的指定注解的属性信息。AnnotationUtils 底层使用动态代理的方式处理注解别名的问题。

### get* 系列注解查找

> get 遵循 JDK 的注解查找语义，只是增加了一级元注解的查找。

```java
public static <A extends Annotation> A getAnnotation(Annotation annotation, Class<A> annotationType) {
    // 1. 直接查找本地注解
    if (annotationType.isInstance(annotation)) {
        return synthesizeAnnotation((A) annotation);
    }
    // 2. 元注解上查找，注意相对于 find* 而言，这里只查找一级元注解
    Class<? extends Annotation> annotatedElement = annotation.annotationType();
    try {
        A metaAnn = annotatedElement.getAnnotation(annotationType);
        return (metaAnn != null ? synthesizeAnnotation(metaAnn, annotatedElement) : null);
    }
    catch (Throwable ex) {
        handleIntrospectionFailure(annotatedElement, ex);
        return null;
    }
}
```

##  find* 系列注解查找

> 遵循 JDK 的注解查找语义，只是增加了多级元注解的查找。

```java
// visited 表示已经查找的元素，Spring 的递归很多都用到了这个参数
private static <A extends Annotation> A findAnnotation(
        AnnotatedElement annotatedElement, Class<A> annotationType, Set<Annotation> visited) {
    try {
        // 1. 本地注解查找
        A annotation = annotatedElement.getDeclaredAnnotation(annotationType);
        if (annotation != null) {
            return annotation;
        }
        // 2. 元注解上查找
        for (Annotation declaredAnn : getDeclaredAnnotations(annotatedElement)) {
            Class<? extends Annotation> declaredType = declaredAnn.annotationType();
            if (!isInJavaLangAnnotationPackage(declaredType) && visited.add(declaredAnn)) {
                // 3. 元注解上递归查找
                annotation = findAnnotation((AnnotatedElement) declaredType, annotationType, visited);
                if (annotation != null) {
                    return annotation;
                }
            }
        }
    }
    catch (Throwable ex) {
        handleIntrospectionFailure(annotatedElement, ex);
    }
    return null;
}
```

## synthesizeAnnotation 动态代理解决别名问题

```java
static <A extends Annotation> A synthesizeAnnotation(A annotation, @Nullable Object annotatedElement) {
    // 1. SynthesizedAnnotation 为一个标记，表示已经动态代理过了
    //    hasPlainJavaAnnotationsOnly 如果是 java 中的注解不可能有注解别名，直接返回
    if (annotation instanceof SynthesizedAnnotation || hasPlainJavaAnnotationsOnly(annotatedElement)) {
        return annotation;
    }

    // 2. 判断是否需要进行动态代理，即注解中存在别名，包括显示别名、隐式别名、传递的隐式别名
    Class<? extends Annotation> annotationType = annotation.annotationType();
    if (!isSynthesizable(annotationType)) {
        return annotation;
    }

    // 3. AnnotationAttributeExtractor 用于从注解 annotation 中提取属性的值
    DefaultAnnotationAttributeExtractor attributeExtractor =
            new DefaultAnnotationAttributeExtractor(annotation, annotatedElement);
    // 4. SynthesizedAnnotationInvocationHandler 动态代理的类
    InvocationHandler handler = new SynthesizedAnnotationInvocationHandler(attributeExtractor);

    // 5. 接口中有 SynthesizedAnnotation，并返回动态代理的对象
    Class<?>[] exposedInterfaces = new Class<?>[] {annotationType, SynthesizedAnnotation.class};
    return (A) Proxy.newProxyInstance(annotation.getClass().getClassLoader(), exposedInterfaces, handler);
}
```

```java
public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
    if (ReflectionUtils.isEqualsMethod(method)) {
        return annotationEquals(args[0]);
    }
    if (ReflectionUtils.isHashCodeMethod(method)) {
        return annotationHashCode();
    }
    if (ReflectionUtils.isToStringMethod(method)) {
        return annotationToString();
    }
    // 注解的 annotationType 返回注解的 Class 类型
    if (AnnotationUtils.isAnnotationTypeMethod(method)) {
        return annotationType();
    }
    if (!AnnotationUtils.isAttributeMethod(method)) {
        throw new AnnotationConfigurationException(String.format(
                "Method [%s] is unsupported for synthesized annotation type [%s]", method, annotationType()));
    }
    // 真正获取注解的属性值
    return getAttributeValue(method);
}
```



# 示例

```java
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.ANNOTATION_TYPE,ElementType.TYPE})
public @interface DemoEnum01 {
    String value() default "111";

    String name() default "";
}
```

```java
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.ANNOTATION_TYPE,ElementType.TYPE})
@DemoEnum01()
public @interface Demo222 {
    @AliasFor(annotation = DemoEnum01.class)
    String value() default "3333";
    @AliasFor(annotation = DemoEnum01.class)
    String name();
}
```

```java
public class MainTest {
    public static void main(String[] args) {
        Annotation annotation = AnnotatedElementUtils.getMergedAnnotation(PersonEntity.class, DemoEnum01.class);
        System.out.println(annotation);
    }
}

```

