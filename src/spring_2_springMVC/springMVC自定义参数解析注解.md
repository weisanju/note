# 自定义参数解析器

## 自定义注解

```java
@Target(ElementType.PARAMETER)
@Retention(RetentionPolicy.RUNTIME)
public @interface JsonParam {
    String value();
}
```

## Controller

```java
@Controller
public class UserController {
    @RequestMapping("/users/add")
    @ResponseBody
    public Map<String, Object> addUser(Integer userid, @JsonParam("dept") Dept userDept) {
        Map<String, Object> data = new HashMap<>(2);
        data.put("userId", userid);
        data.put("userDept", userDept);
        return data;
    }
}
```



## 自定义参数解析器

```java
public class JsonParamProvider implements HandlerMethodArgumentResolver {
    /**
     * 判断是否是需要我们解析的参数类型
     */
    @Override
    public boolean supportsParameter(MethodParameter methodParameter) {
        return methodParameter.hasParameterAnnotation(JsonParam.class);
    }

    /**
     * 真正解析的方法
     */
    @Override
    public Object resolveArgument(MethodParameter methodParameter, ModelAndViewContainer modelAndViewContainer, NativeWebRequest nativeWebRequest, WebDataBinderFactory webDataBinderFactory) throws Exception {
        HttpServletRequest request = nativeWebRequest.getNativeRequest(HttpServletRequest.class);
        Map<String, String[]> parameterMap = request.getParameterMap();

        JsonParam jsonParam = methodParameter.getParameterAnnotation(JsonParam.class);
        String paramName = jsonParam.value();
        //注解没有给定参数名字，默认取参数类型的小写
        if (StringUtils.isEmpty(paramName)) {
            String parmTypeName = methodParameter.getParameterType().getSimpleName();
            paramName = parmTypeName.substring(0, 1).toLowerCase() + parmTypeName.substring(1);
        }

        //从request中能拿到参数值
        if (parameterMap.containsKey(paramName)) {
            String paramVal = parameterMap.get(paramName)[0];
            //解析json
            ObjectMapper objectMapper = new ObjectMapper();
            Dept dept = objectMapper.readValue(paramVal.getBytes("UTF-8"), Dept.class);
            return dept;
        } else {
            return new Dept();
        }
    }
}
```





# 配置自定义参数解析器

## XML方式

```xml
<?xml version="1.0" encoding="UTF-8"?>

	<context:component-scan base-package="com.demo" />
	
	<mvc:annotation-driven>
		<mvc:argument-resolvers>
			<bean class="com.demo.JsonParamProvider"/>
		</mvc:argument-resolvers>
	</mvc:annotation-driven>
</beans>  
```

## 注解方式配置

### xml

```xml
spring-mvc.xml内容
<?xml version="1.0" encoding="UTF-8"?>

	<context:component-scan base-package="com.demo" />
	
	<!--不能添加该标签，否则注解类不生效-->
	<!--<mvc:annotation-driven/>-->
</beans>
```

### Java配置

```java
@Configuration
@EnableWebMvc
public class MyConfiguration extends WebMvcConfigurerAdapter {
    @Override
    public void addArgumentResolvers(List<HandlerMethodArgumentResolver> argumentResolvers) {
        argumentResolvers.add(new JsonParamProvider());
        }
}
```



## springBoot配置

```java
@Configuration
public class MyConfiguration extends WebMvcConfigurationSupport {
    @Override
    public void addArgumentResolvers(List<HandlerMethodArgumentResolver> argumentResolvers) {
        argumentResolvers.add(new JsonParamProvider());
    }
}
```


