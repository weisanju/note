# 函数式接口

* 只有一个抽象方法的接口
* 可以通过 *@FunctionalInterface* 来注明这是一个函数式接口

# lambda 表达式

lambda 表达式的语法由参数列表、箭头符号 `->` 和函数体组成

# 目标类型

lambda表达式的类型是由其上下文推导而来

### 目标类型的上下文

- 变量声明

- 赋值

- 返回语句

- 数组初始化器

  ```
  new FileFilter[] {
      f -> f.exists(), f -> f.canRead(), f -> f.getName().startsWith("q")
    });
  ```

- 方法和构造方法的参数

- lambda 表达式函数体

- 条件表达式（`? :`）

- 转型（Cast）表达式

# 词法作用域

* 内部类与 lambda表达式作用域

  内部类的作用域在 类内部

  lambda表达式的作用域是在 类外部,类似于代码块的作用域

# 变量捕获

lambda 表达式对 *值* 封闭，对 *变量* 开放

lambda表达式 对局部变量 引用的原则是 *effective final*

lambda不支持 捕获变量的修改,因为容易在多线程环境下引起竞争

```
int sum = 0;
list.forEach(e -> { sum += e.size(); }); // Illegal, close over values

List<Integer> aList = new List<>();
list.forEach(e -> { aList.add(e); }); // Legal, open over variables
```



# 方法引用

* 方法引用时 lambda表达式的缩写
* *Person::getName* 等价于 *p -> p.getName()*

## 方法引用的种类

- 静态方法引用：`ClassName::methodName`
- 实例上的实例方法引用：`instanceReference::methodName`
- 超类上的实例方法引用：`super::methodName`
- 类型上的实例方法引用：`ClassName::methodName`
- 构造方法引用：`Class::new`
- 数组构造方法引用：`TypeName[]::new`