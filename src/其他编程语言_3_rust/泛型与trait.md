# 泛型的使用

## **函数泛型定义与使用**

```rust
//定义,写在函数名后面, 使用在函数的任何地方
fn largest<T>(list: &[T]) -> T {
    let mut largest = list[0];
    for &item in list.iter() {
        if item > largest {
            largest = item;
        }
    }
    largest
}
```

## **结构体泛型定义与使用**

```rust
struct Point<T> {
    x: T,
    y: T,
}

fn main() {
    let wont_work = Point { x: 5, y: 4.0 };
}
```

## 枚举定义的泛型

```rust
enum Option<T> {
    Some(T),
    None,
}
enum Result<T, E> {
    Ok(T),
    Err(E),
}
```

## 方法实现中的泛型定义

```rust
struct Point<T> {
x: T,
y: T,
}


impl Point<f32> {
    fn distance_from_origin(&self) -> f32 {
        (self.x.powi(2) + self.y.powi(2)).sqrt()
    }
}

impl<T> Point<T> {
    fn x(&self) -> &T {
        &self.x
    }
}

//产生新泛型
struct Point<T, U> {
    x: T,
    y: U,
}

impl<T, U> Point<T, U> {
    fn mixup<V, W>(self, other: Point<V, W>) -> Point<T, W> {
        Point {
            x: self.x,
            y: other.y,
        }
    }
}
```

## 泛型代码的性能

* Rust 实现了泛型，使得使用泛型类型参数的代码相比使用具体类型并没有任何速度上的损失。
* Rust 通过在编译时进行泛型代码的 **单态化**（*monomorphization*）来保证效率。单态化是一个通过填充编译时使用的具体类型，将通用代码转换为特定代码的过程。
* 编译器寻找所有泛型代码被调用的位置并使用泛型代码针对具体类型生成代码。



让我们看看一个使用标准库中 `Option` 枚举的例子：

```rust
let integer = Some(5);
let float = Some(5.0);
```

编译器会读取传递给 `Option<T>` 的值并发现有两种 `Option<T>`：一个对应 `i32` 另一个对应 `f64`

为此，它会将泛型定义 `Option<T>` 展开为 `Option_i32` 和 `Option_f64`，接着将泛型定义替换为这两个具体的定义。









# Trait(特性)

## 定义

- 一个类型的行为由其可供调用的方法构成。

- 如果可以**对不同类型调用相同的方法**的话，这些类型就可以共享相同的行为了。

- trait 定义是一种将方法签名组合起来的方法，目的是定义一个实现某些目的所必需的行为的集合。

```rust
//trait 体中可以有多个方法
pub trait Summary {
    fn summarize(&self) -> String;
}
```

## 为结构体实现*Trait*

```rust
pub struct NewsArticle {
    pub headline: String,
    pub location: String,
    pub author: String,
    pub content: String,
}

impl Summary for NewsArticle {
    fn summarize(&self) -> String {
        format!("{}, by {} ({})", self.headline, self.author, self.location)
    }
}

pub struct Tweet {
    pub username: String,
    pub content: String,
    pub reply: bool,
    pub retweet: bool,
}

impl Summary for Tweet {
    fn summarize(&self) -> String {
        format!("{}: {}", self.username, self.content)
    }
}
```

## trait 的相干性

- 不能为 外部类型(第三方) 实现 外部 （第三方）trait
  - 例如 _不能在 `aggregator` crate 中为 `Vec<T>` 实现 `Display` trait_
  - 因为 `Display` 和 `Vec<T>` 都定义于标准库中
  - 并不位于 `aggregator` crate 本地作用域中
  - 这个限制是被称为 **相干性**（_coherence_） 更具体的说是 **孤儿规则**（_orphan rule_）
  - 这条规则确保了其他人编写的代码不会破坏你代码

## Trait 的默认实现

```rust
pub trait Summary {
    fn summarize(&self) -> String {
        String::from("(Read more...)")
    }
}
```

**默认方法与抽象方法共存**

```rust

pub trait Summary {
    fn summarize_author(&self) -> String;

    fn summarize(&self) -> String {
        format!("(Read more from {}...)", self.summarize_author())
    }
}
```

```rust
trait Summary{
     fn summary(&self) -> String{
        String::from("read more")
    }
}
impl Summary for Book{

}
```





## Trait 作为参数

### TraitBound 语法糖

> 与 _impl Summary_ 是一样

```rust
pub fn notify<T: Summary>(item: T) {
    println!("Breaking news! {}", item.summarize());
}
```

```
pub fn notify(item1: impl Summary, item2: impl Summary) {
与
pub fn notify<T: Summary>(item1: T, item2: T) {
```

**通过 + 号 指定多个**

```rust
pub fn notify(item: impl Summary + Display) {
与
pub fn notify<T: Summary + Display>(item: T) {
```

**通过 where 简化 trait bound**

```rust
fn some_function<T: Display + Clone, U: Clone + Debug>(t: T, u: U) -> i32 {
与

fn some_function<T, U>(t: T, u: U) -> i32
    where T: Display + Clone,
          U: Clone + Debug
{
```

### 函数参数

```rust
pub fn notify(item: impl Summary) {
    println!("Breaking news! {}", item.summarize());
}
```

### 返回类型

```rust
fn returns_summarizable() -> impl Summary {
    Tweet {
        username: String::from("horse_ebooks"),
        content: String::from("of course, as you probably already know, people"),
        reply: false,
        retweet: false,
    }
}
```

## 使用 trait bound 有条件地实现方法

> 限定泛型的实现 类型

```rust
use std::fmt::Display;

struct Pair<T> {
    x: T,
    y: T,
}

impl<T> Pair<T> {
    fn new(x: T, y: T) -> Self {
        Self {
            x,
            y,
        }
    }
}

impl<T: Display + PartialOrd> Pair<T> {
    fn cmp_display(&self) {
        if self.x >= self.y {
            println!("The largest member is x = {}", self.x);
        } else {
            println!("The largest member is y = {}", self.y);
        }
    }
}
```

**例如**

```rust
//标准库为任何实现了 Display trait 的类型实现了 ToString trait。这个 impl 块看起来像这样：
impl<T: Display> ToString for T {
    // --snip--
}
//因为标准库有了这些 blanket implementation，我们可以对任何实现了 Display trait 的类型调用由 ToString 定义的 to_string 方法。例如，可以将整型转换为对应的 String 值，因为整型实现了 Display：
```

## dyn Trait trait 对象

`dyn Trait` 是使用 trait 对象的新语法，简而言之：

- `Box<Trait>` becomes `Box<dyn Trait>`
- `&Trait` and `&mut Trait` become `&dyn Trait` and `&mut dyn Trait`







# 标准库中的 _Trait_

## Debug详细打印

`Debug` trait 用于开启格式化字符串中的调试格式，其通过在 `{}` 占位符中增加 `:?` 表明。

需要实现 `Debug` 的 `fmt`

```
impl Debug for Address{
    fn fmt(&self, f: &mut Formatter<'_>) -> Result<(), Error> {
        write!(f,"a={}",self.home)
    }
}
```

## 等值比较

[*相关解释*](https://rustcc.cn/article?id=9a1990b9-e86a-46df-a3c3-fcfaff3c8533) 

## `PartitalEq` 

> 派生的 `PartialEq` 实现了 `eq` 方法。

* 当 `PartialEq` 在结构体上派生时，只有*所有* 的字段都相等时两个实例才相等。
* 当在枚举上派生时，每一个成员都和其自身相等，且和其他成员都不相等。

## `Eq` 

Eq 相比 PartialEq 需要额外满足反身性，即 `a == a`，对于浮点类型，Rust 只实现了 PartialEq 而不是 Eq，原因就是 `NaN != NaN`。



## `Ord & PartialOrd`

类似于 Eq，Ord 指的是 [Total Order](https://en.wikipedia.org/wiki/Total_order)，需要满足以下三个性质：

- 反对称性（Antisymmetry）：`a <= b` 且 `a >= b` 可推出 `a == b`
- 传递性（Transitivity）：`a <= b` 且 `b <= c` 可推出 `a <= c`
- 连通性（Connexity）：`a <= b` 或 `a >= b`

而 PartialOrd 无需满足连通性，只满足反对称性和传递性即可。

- 反对称性：`a < b` 则有 `!(a > b)`，反之亦然
- 传递性：`a < b` 且 `b < c` 可推出 `a < c`，`==` 和 `>` 同理



## 复制值的 `Clone` 和 `Copy`

* 可以明确地创建一个值的深拷贝（deep copy），复制过程可能包含任意代码的执行以及堆上数据的复制
* 派生 `Clone` 实现了 `clone` 方法，其为整个的类型实现时，在类型的每一部分上调用了 `clone` 方法。这意味着类型中所有字段或值也必须实现了 `Clone`，这样才能够派生 `Clone` 。

**切片转集合时需要clone**

当在一个切片（slice）上调用 to_vec 方法时，Clone 是必须的。切片并不拥有其所包含实例的类型，但是从 to_vec 中返回的 vector 需要拥有其实例，因此，to_vec 在每个元素上调用 clone。因此，存储在切片中的类型必须实现 Clone。

**拷贝存储在栈上的数据不需要额外代码**

`Copy` trait 允许你通过只拷贝存储在栈上的位来复制值而不需要额外的代码。查阅第四章 [“只在栈上的数据：拷贝”](https://kaisery.github.io/trpl-zh-cn/ch04-01-what-is-ownership.html#stack-only-data-copy) 的部分来获取有关 `Copy` 的更多信息。



## 固定大小的值到值映射的 Hash

* `Hash` trait 可以实例化一个任意大小的类型，并且能够用哈希（hash）函数将该实例映射到一个固定大小的值上。

* 派生 `Hash` 实现了 `hash` 方法。`hash` 方法的派生实现结合了在类型的每部分调用 `hash` 的结果，这意味着所有的字段或值也必须实现了 `Hash`，这样才能够派生 `Hash`。

>  例如，在 `HashMap<K, V>` 上存储数据，存放 key 的时候，`Hash` 是必须的。

## 默认值的 `Default`

*　`Default` trait 使你创建一个类型的默认值

**使用**

```rust
pub fn  test_default(){
    let person = Person {..Default::default()};
    println!("{:?}",person);
}
#[derive(Debug)]
struct Person{
    age:i32,
    name:String,
    address:String
}

impl Default for Person{
    fn default() -> Self {
        Person{
            age:18,
            name:"ssss".to_owned(),
            address: "".to_owned()
        }
    }
}
```



## trait 对象执行动态分发

1. 当对泛型使用 trait bound 时编译器所进行单态化处理：编译器为每一个被泛型类型参数代替的具体类型生成了非泛型的函数和方法实现。

2. 单态化所产生的代码进行 **静态分发**（*static dispatch*）

3. 静态分发发生于编译器在编译时就知晓调用了什么方法的时候。这与 **动态分发** （*dynamic dispatch*）相对，这时编译器在编译时无法知晓调用了什么方法。在动态分发的情况下，编译器会生成在运行时确定调用了什么方法的代码。

**当使用 trait 对象时，Rust 必须使用动态分发**

## Trait 对象要求对象安全

只有 **对象安全**（*object safe*）的 trait 才可以组成 trait 对象

如果一个 trait 中所有的方法有如下属性时，则该 trait 是对象安全的：

- 返回值类型不为 `Self`
- 方法没有任何泛型类型参数

一个 trait 的方法不是对象安全的例子是**标准库中的 `Clone` trait**。`Clone` trait 的 `clone` 方法的参数签名看起来像这样：

```rust
pub trait Clone {
    fn clone(&self) -> Self;
}
```









# 高级Trait

## trait中的关联类型

**干什么用的?**

* 提供类似泛型的作用
* 提供迭代的子类型
* `Item` 是一个占位类型,在编译时期,会根据具体的实现类去替换

**怎么使用的?**

**定义**

```rust

pub trait Iterator {
    type Item;

    fn next(&mut self) -> Option<Self::Item>;
}
```

**使用**

```rust
mpl Iterator for Counter {
    type Item = u32;

    fn next(&mut self) -> Option<Self::Item> {
```

### **为什么会要有定义这个而不使用泛型?**

如果使用泛型 的话,每次调用 next方法都得指定泛型具体类型



## 默认类型参数

### **干嘛用的?**

* 当使用泛型类型参数时，可以为泛型指定一个默认的具体类型。如果默认类型就足够的话，这消除了为具体类型实现 trait 的需要

### **如何使用**

**定义默认类型参数**

```rust
//定义,RHS参数默认使用 实现该Trait 的 类型 
trait Add<RHS=Self> {
    type Output;
    fn add(self, rhs: RHS) -> Self::Output;
}

```

**使用默认类型参数**

```rust

#[derive(Debug, PartialEq)]
struct Point {
    x: i32,
    y: i32,
}

impl Add for Point {
    type Output = Point;

    fn add(self, other: Point) -> Point {
        Point {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}
```

**使用自定义类型参数**

```rust
use std::ops::Add;

struct Millimeters(u32);
struct Meters(u32);

impl Add<Meters> for Millimeters {
    type Output = Millimeters;

    fn add(self, other: Meters) -> Millimeters {
        Millimeters(self.0 + (other.0 * 1000))
    }
}
```

**为什么要引入这个**

* 一小部分实现的样板代码是不必要的，这样使用 trait 就更容易了,减少不必要的泛型类型
* 扩展类型而不破坏现有代码。

## 完全限定语法

### **干嘛用的?**

```rust

trait Pilot {
    fn fly(&self);
}

trait Wizard {
    fn fly(&self);
}

struct Human;

impl Pilot for Human {
    fn fly(&self) {
        println!("This is your captain speaking.");
    }
}

impl Wizard for Human {
    fn fly(&self) {
        println!("Up!");
    }
}

impl Human {
    fn fly(&self) {
        println!("*waving arms furiously*");
    }
}
```

* 当一个类上实现了多个方法,编译器默认调用 使用 `impl Type` 中的方法
* 使用 完全限定语法可以调用 其他 `Trait`的方法



### **如何使用?**

**全语法**

`<Type as Trait>::function(receiver_if_method, next_arg, ...);`

**使用(类型定义见上)**

```rust
fn main() {
    let person = Human;
    Pilot::fly(&person);
    Wizard::fly(&person);
    person.fly();
}
```

**使用2**

```rust
fn main() {
    println!("A baby dog is called a {}", <Dog as Animal>::baby_name());
}
```

## Trait中的继承

### **干什么用的?**

* 用于在另一个 trait 中使用某 trait 的功能

* `Trait` 定义指定了 要实现它就 必须先 实现 `Display`

### 如何使用

**定义**

```rust
use std::fmt;

trait OutlinePrint: fmt::Display {
    fn outline_print(&self) {
        let output = self.to_string();
        let len = output.len();
        println!("{}", "*".repeat(len + 4));
        println!("*{}*", " ".repeat(len + 2));
        println!("* {} *", output);
        println!("*{}*", " ".repeat(len + 2));
        println!("{}", "*".repeat(len + 4));
    }
}
```

**使用**

```rust
struct Point {
    x: i32,
    y: i32,
}
impl OutlinePrint for Point {}
```

### 为什么要定义这个

* **广义上来说是为了 重用代码**
* 狭义上 就是实现某一trait 需要依赖另一个 *trait*
* 类似于继承的概念



## 解决为 外部类型 实现外部 Trait

### 背景

**外部类型或Trait**

即非本地的, 例如标准库,第三方库的类型或Trait

**孤儿规则** (orphan rule)

* 不能为外部类型,实现外部trait
* 避免本地库 影响第三方库的行为

**什么是*newType* 模式**

**举例**

```rust
//常规定义
struct Person{
	age:u32,
	address:String
}
//newType定义
struct Person{
	age:Year,
	address:Address
}

struct Year(u32);
struct Address(String);
```

**说明**

* **使用这个模式没有运行时性能惩罚，这个封装类型在编译时就被省略了。**
* 在编写代码时,很快就能知道 某个字段的具体 业务含义

* **nwType模式可以在外部类型上实现外部Trait**

### **如何使用**

```rust
use std::fmt;

struct Wrapper(Vec<String>);

impl fmt::Display for Wrapper {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "[{}]", self.0.join(", "))
    }
}

fn main() {
    let w = Wrapper(vec![String::from("hello"), String::from("world")]);
    println!("w = {}", w);
}
```

* `Display` 的实现使用 **`self.0` 来访问其内部的 `Vec<T>`**

* 此方法的缺点是，因为 **`Wrapper` 是一个新类型，它没有定义于其值之上的方法**；必须直接在 `Wrapper` 上实现 `Vec<T>` 的所有方法，这样就可以代理到`self.0` 上 

* 如果希望**新类型拥有其内部类型的每一个方法**，为封装类型实现 `Deref` trait（第十五章 [“通过 `Deref` trait 将智能指针当作常规引用处理”](https://kaisery.github.io/trpl-zh-cn/ch15-02-deref.html#treating-smart-pointers-like-regular-references-with-the-deref-trait) 部分讨论过）并返回其内部类型是一种解决方案。

    

