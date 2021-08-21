# springMVC 有着多种不同风格的 路径映射方式

## **Ant风格的映射**

```java
@RequestMapping("/a?/aaa") //?代表一个字符
@RequestMapping("/a*/aaa") //*代表多个字符
@RequestMapping("/**/aaa") //**代表多个路径
```

## 占位符映射风格

```java
@RequestMapping("/show/{name}")
public ModelAndView test(@PathVariable("name")String name){

}
```



# 请求限制

## 限制请求的方法

略

## **限定请求参数的映射**

```
@RequestMapping(value=””,params=””)

①params=”id”

//请求参数中必须有id，如果没有id会报错。

//与之相反的是：如果params=”!id”表示请求参数中不能包含id，如果有id会报错。

②params=”id=1”

//请求参数中id必须为1，如果不为1会报错。

//与之相反的是：如果params=”id!=1”表示请求参数中id必须不为1，如果等于1会报错。

③params={“name”, ”age”}

//请求参数中必须有name，age参数，当然有多余的其它参数也行，但这两个必须要有。
```



# **混合注解**

```
@GetMapping
@PostMapping
@PostMapping、@PutMapping、@DeleteMapping
```





# 获取*Cookie*

```
@CookieValue
```

