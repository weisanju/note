## @WebFilter

标记一个过滤器

```java
@WebFilter(urlPatterns = "/*")
public class MyFilter implements Filter {

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {
        System.out.println("-----doFilter-----");
        chain.doFilter(request, response);
    }
}
```

需要标注:@ServletComponentScan 注解,因为这是 servlet的注解



## @Bean

```java
@Component
public class MyFilter implements Filter {

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {
        System.out.println("-----doFilter-----");
        chain.doFilter(request, response);
    }
}

指定优先级
@Component
@Order(-1)
public class MyFilter implements Filter {

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {
        System.out.println("-----doFilter-----");
        chain.doFilter(request, response);
    }
}
```



## FilterRegistrationBean

```java
@Configuration
public class FilterConfiguration {
    @Bean
    FilterRegistrationBean<MyFilter> myFilterFilterRegistrationBean() {
        FilterRegistrationBean<MyFilter> bean = new FilterRegistrationBean<>();
        bean.setFilter(new MyFilter());
        bean.setOrder(-1);
        bean.setUrlPatterns(Arrays.asList("/*"));
        return bean;
    }
    @Bean
    FilterRegistrationBean<MyFilter2> myFilterFilterRegistrationBean2() {
        FilterRegistrationBean<MyFilter2> bean = new FilterRegistrationBean<>();
        bean.setFilter(new MyFilter2());
        bean.setOrder(-2);
        bean.setUrlPatterns(Arrays.asList("/hello"));
        return bean;
    }
}
```



spring有很多注册*bean*,用来向spring容器注册特殊业务功能的bean

1. ServletListenerRegistrationBean 用来注册监听器。
2. ServletRegistrationBean 用来注册 Servlet。
3. DispatcherServletRegistrationBean 用来注册 DispatcherServlet。
4. FilterRegistrationBean 用来注册过滤器。
5. DelegatingFilterProxyRegistrationBean 则用来注册 DelegatingFilterProxy，DelegatingFilterProxy 在 Spring Security、Spring Session、Shiro 等整合时非常有用。



