# 为了类型安全和抽象而使用 newtype 模式

## newType的功能

* 使用newType替代 基本类型 可以确保某值不被混淆或者用来表示一个值的单元

* 抽象掉一些类型的实现细节

    * 封装类型可以暴露出与直接使用其内部私有类型时所不同的公有 API，以便限制其功能。
    * newtype 也可以隐藏其内部的泛型类型。

    例如，可以提供一个封装了 `HashMap<i32, String>` 的 `People` 类型，用来储存人名以及相应的 ID。使用 `People` 的代码只需与提供的公有 API 交互即可，比如向 `People` 集合增加名字字符串的方法，这样这些代码就无需知道在内部我们将一个 `i32` ID 赋予了这个名字了。newtype 模式是一种实现第十七章 [“封装隐藏了实现细节”](https://kaisery.github.io/trpl-zh-cn/ch17-01-what-is-oo.html#encapsulation-that-hides-implementation-details) 部分所讨论的隐藏实现细节的封装的轻量级方法。



## 类型别名

#### 什么是类型别名

连同 newtype 模式，Rust 还提供了声明 **类型别名**（*type alias*）的能力，使用 `type` 关键字来给予现有类型另一个名字。例如，可以像这样创建 `i32` 的别名 `Kilometers`：

```rust
type Kilometers = i32;
```

这意味着 `Kilometers` 是 `i32` 的 **同义词**（*synonym*）；不同于示例 19-23 中创建的 `Millimeters` 和 `Meters` 类型。`Kilometers` 不是一个新的、单独的类型。**`Kilometers` 类型的值将被完全当作 `i32` 类型值来对待**：

### 类型别名的使用

类型别名的主要用途是减少重复。例如，可能会有这样很长的类型：

```rust
let f: Box<dyn Fn() + Send + 'static> = Box::new(|| println!("hi"));

fn takes_long_type(f: Box<dyn Fn() + Send + 'static>) {
    // --snip--
}

fn returns_long_type() -> Box<dyn Fn() + Send + 'static> {
    // --snip--
}

//使用类型别名
type Thunk = Box<dyn Fn() + Send + 'static>;

let f: Thunk = Box::new(|| println!("hi"));

fn takes_long_type(f: Thunk) {
    // --snip--
}

fn returns_long_type() -> Thunk {
    // --snip--
}
```

### **泛型化类型别名**

```rust
type Result<T> = std::result::Result<T, std::io::Error>;
```



# 从不返回的 never type

* Rust 有一个叫做 `!` 的特殊类型。在类型理论术语中，它被称为 *empty type*，因为它没有值。我们更倾向于称之为 *never type*。

* 从不返回的函数被称为 发散函数（*diverging functions*）

    

## Continue返回 !

```rust
let guess: u32 = match guess.trim().parse() {
    Ok(num) => num,
    Err(_) => continue,
};
```

## Panic返回!

```rust
impl<T> Option<T> {
    pub fn unwrap(self) -> T {
        match self {
            Some(val) => val,
            None => panic!("called `Option::unwrap()` on a `None` value"),
        }
    }
}
```



## Loop返回!

```rust
print!("forever ");

loop {
    print!("and ever ");
}
```



# 动态大小类型和 Sized trait

因为 Rust 需要知道例如应该为特定类型的值分配多少空间这样的信息其类型系统的一个特定的角落可能令人迷惑：这就是 **动态大小类型**（*dynamically sized types*）的概念。这有时被称为 “DST” 或 “unsized types”，这些类型允许我们处理只有在运行时才知道大小的类型。

让我们深入研究一个贯穿本书都在使用的动态大小类型的细节：`str`。没错，不是 `&str`，而是 `str` 本身。`str` 是一个 DST；直到运行时我们都不知道字符串有多长。因为直到运行时都不能知道大其小，也就意味着不能创建 `str` 类型的变量，也不能获取 `str` 类型的参数。考虑一下这些代码，他们不能工作：

```rust
let s1: str = "Hello there!";
let s2: str = "How's it going?";
```

Rust 需要知道应该为特定类型的值分配多少内存，同时所有同一类型的值必须使用相同数量的内存。如果允许编写这样的代码，也就意味着这两个 `str` 需要占用完全相同大小的空间，不过它们有着不同的长度。这也就是为什么不可能创建一个存放动态大小类型的变量的原因。

那么该怎么办呢？你已经知道了这种问题的答案：`s1` 和 `s2` 的类型是 `&str` 而不是 `str`。如果你回想第四章 [“字符串 slice”](https://kaisery.github.io/trpl-zh-cn/ch04-03-slices.html#string-slices) 部分，slice 数据结储存了开始位置和 slice 的长度。

所以虽然 `&T` 是一个储存了 `T` 所在的内存位置的单个值，`&str` 则是 **两个** 值：`str` 的地址和其长度。这样，`&str` 就有了一个在编译时可以知道的大小：它是 `usize` 长度的两倍。也就是说，我们总是知道 `&str` 的大小，而无论其引用的字符串是多长。这里是 Rust 中动态大小类型的常规用法：他们有一些额外的元信息来储存动态信息的大小。这引出了动态大小类型的黄金规则：必须将动态大小类型的值置于某种指针之后。

可以将 `str` 与所有类型的指针结合：比如 `Box<str>` 或 `Rc<str>`。事实上，之前我们已经见过了，不过是另一个动态大小类型：trait。每一个 trait 都是一个可以通过 trait 名称来引用的动态大小类型。在第十七章 [“为使用不同类型的值而设计的 trait 对象”](https://kaisery.github.io/trpl-zh-cn/ch17-02-trait-objects.html#using-trait-objects-that-allow-for-values-of-different-types) 部分，我们提到了为了将 trait 用于 trait 对象，必须将他们放入指针之后，比如 `&dyn Trait` 或 `Box<dyn Trait>`（`Rc<dyn Trait>` 也可以）。

为了处理 DST，Rust 有一个特定的 trait 来决定一个类型的大小是否在编译时可知：这就是 `Sized` trait。这个 trait 自动为编译器在编译时就知道大小的类型实现。另外，**Rust 隐式的为每一个泛型函数增加了 `Sized` bound**。也就是说，对于如下泛型函数定义：

```rust
fn generic<T>(t: T) {
    // --snip--
}
```

实际上被当作如下处理：

```rust
fn generic<T: Sized>(t: T) {
    // --snip--
}
```

泛型函数默认只能用于在编译时已知大小的类型。然而可以使用如下特殊语法来放宽这个限制：

```rust
fn generic<T: ?Sized>(t: &T) {
    // --snip--
}
```

`?Sized` trait bound 与 `Sized` 相对；也就是说，它可以读作 “`T` 可能是也可能不是 `Sized` 的”。这个语法只能用于 `Sized` ，而不能用于其他 trait。

另外注意我们将 `t` 参数的类型从 `T` 变为了 `&T`：因为其类型可能不是 `Sized` 的，所以需要将其置于某种指针之后。在这个例子中选择了引用。



**例如**

**闭包的存储空间是动态的**

**通过指定包装返回闭包**

```rust
fn returns_closure() -> Box<dyn Fn(i32) -> i32> {
    Box::new(|x| x + 1)
}
```

