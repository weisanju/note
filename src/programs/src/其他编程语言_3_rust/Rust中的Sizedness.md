## 介绍

本文将会探讨 从  已知大小类型(`sized type`)、未知大小类型(`unsized type`)、在到 零大小类型(`zero type`)等各种类型的 sizedness，并同时对他们的优点 、缺点、痛点、以及解决方法进行评估

**以下是文中的术语表格**



| 术语                            | 含义                                |
| ------------------------------- | ----------------------------------- |
| sizedness                       | 不同大小类型的特性                  |
| sized type                      | 编译期可以确定大小的类型            |
| unsized type or DST             | 动态大小类型                        |
| ?sized type                     | 可能确定也可能不确定的大小类型      |
| unsized coercion                | 从确定大小类型转换为 不确定大小类型 |
| zst                             | 零大小类型                          |
| width                           | 指针的宽度的单位                    |
| thin point\|single width point  | 1个宽度的指针                       |
| fat point \| double-width point | 2个宽度的指针                       |
| slice                           | 数据的动态大小视图                  |



## Sizedness

* 如果一个类型 的大小能在编译期确定，也称作 sizedType,那确定类型大小的类型就能在 栈上分配空间。数据的传递也就能通过 值传递或者引用传递的方式
* 如果一个类型的大小不能在编译期确定，也叫作，dst、动态类型大小。无法在栈上分配空间。数据的传递只能通过引用传递

以下是一些 **sized type**或者 **unsized type**

* 基本类型
* 元祖类型
* 结构体类型
* 数组类型：固定数组类型、不固定数组类型
* 枚举类型
* 普通指针、字符串指针、数组指针
* trait类型
* 自定义 unsized类型

```rust
use std::mem::size_of;

fn main() {
    // primitives
    assert_eq!(4, size_of::<i32>());
    assert_eq!(8, size_of::<f64>());

    // tuples
    assert_eq!(8, size_of::<(i32, i32)>());

    // arrays
    assert_eq!(0, size_of::<[i32; 0]>());
    assert_eq!(12, size_of::<[i32; 3]>());

    struct Point {
        x: i32,
        y: i32,
    }

    // structs
    assert_eq!(8, size_of::<Point>());

    // enums
    assert_eq!(8, size_of::<Option<i32>>());

    // get pointer width, will be
    // 4 bytes wide on 32-bit targets or
    // 8 bytes wide on 64-bit targets
    const WIDTH: usize = size_of::<&()>();

    // pointers to sized types are 1 width
    assert_eq!(WIDTH, size_of::<&i32>());
    assert_eq!(WIDTH, size_of::<&mut i32>());
    assert_eq!(WIDTH, size_of::<Box<i32>>());
    assert_eq!(WIDTH, size_of::<fn(i32) -> i32>());

    const DOUBLE_WIDTH: usize = 2 * WIDTH;

    // unsized struct
    struct Unsized {
        unsized_field: [i32],
    }

    // pointers to unsized types are 2 widths
    assert_eq!(DOUBLE_WIDTH, size_of::<&str>()); // slice
    assert_eq!(DOUBLE_WIDTH, size_of::<&[i32]>()); // slice
    assert_eq!(DOUBLE_WIDTH, size_of::<&dyn ToString>()); // trait object
    assert_eq!(DOUBLE_WIDTH, size_of::<Box<dyn ToString>>()); // trait object
    assert_eq!(DOUBLE_WIDTH, size_of::<&Unsized>()); // user-defined unsized type

    // unsized types
    size_of::<str>(); // compile error
    size_of::<[i32]>(); // compile error
    size_of::<dyn ToString>(); // compile error
    size_of::<Unsized>(); // compile error
}
```

## proTIPS

* rust中 指向数组的动态大小视图 `dynamic sized view` 被称为 切片（*slice*）,&str是字符串切片、&[i32] 是数组切片
* 切片是双宽度的、因为它们存储了 指向 数组的 指针 和 数组中元素的个数
* trait对象是 双宽度的，因为它们 存储了 指向 数据的指针 和 指向  vnode的指针
* 不确定大小的结构体是 双宽的。因为 它拥有 指针指向 结构体的指针 和 结构体大小的size
* 不确定大小的结构体 只能拥有 一个 不确定大小的 字段 且 只能是 结构体最后一个字段



**总结**

* 确定大小的 类型可以 分配在 栈上。可以通过值传递
* 不确定大小的类型不能分配在栈上，且必须通过 引用传递
* 不确定大小的类型是双宽度的，除了要记录 指针位置外，还需要记录 数据量大小、或者 vnodetable等



## Trait对象(Trait Objects)

Traits默认是`?Sized`。

```rust
trait Trait: ?Sized {}


trait Trait where Self: ?Sized {}
```

1. 默认情况下，trait允许`self`可能是一个不确定大小类型(unsized type)

2. 不确定大小类型无法 通过 值传递，所以没法以传值的方式 接收 或返回 **self** 

3. 但是是 可以编译，如果一旦为这个方法 提供默认的实现，或者实现其他的 都会编译错误

4. 可以通过 引用传递方式 传递 `self`

   ```rust
   trait Trait {
       fn method(&self) {} // compiles
   }
   
   impl Trait for str {
       fn method(&self) {} // compiles
   }
   ```

5. 可以有更细粒度和更精确的选择 来标记 单个方法 为 `Sized`

   ```rust
   trait Trait {
       fn method(self) where Self: Sized {}
   }
   
   impl Trait for str {} // compiles!?
   
   fn main() {
       "str".method(); // compile error
   }
   ```



## Trait对象的限制(Trait Object Limitations)

即使一个trait是对象安全的，仍然存在sizeness相关的边界情况，这些情况限制了什么类型可以转成trait对象以及多少种trait和什么样的trait可以通过一个trait对象来表示。

### 不能把不确定大小类型(unsized type)转成trait对象

| 类型                      | 指向数据的指针 | 数据长度 | 指向Vtable的指针 | 总长度 |
| ------------------------- | -------------- | -------- | ---------------- | ------ |
| &String                   | 有             | 没有     | 没有             | 1w     |
| &str                      | 有             | 有       | 没有             | 2w     |
| &String as & dyn ToString | 有             | 没有     | 有               | 2w     |
| &str as & &dyn ToString   | 有             | 有       | 有               | 3w     |

### 不能创建多Trait的对象(Cannot create Multi-Trait Objects)

```rust
trait Trait {}
trait Trait2 {}

fn function(t: &(dyn Trait + Trait2)) {}
```

* 一个trait对象指针是双宽度的:存储一个指向数据的指针、和指向vttable的指针
* 这里有 两个 Trait 就存在 指向 两个vtable的 指针  &(dyn Trait+Trait2) 就是三宽度 ，rust最多支持 两个宽度的指针
* 像 Sync 与 Send 这样的 Trait不存在 方法，所以可以有多个
* 解决办法是，借助第三个 Trait实现 上述 两个 Trait,但是也会存在 无法自动向上转型

```rust
trait Trait {
    fn method(&self) {}
}

trait Trait2 {
    fn method2(&self) {}
}

trait Trait3: Trait + Trait2 {}

impl<T: Trait + Trait2> Trait3 for T {}

struct Struct;
impl Trait for Struct {}
impl Trait2 for Struct {}

fn takes_trait(t: &dyn Trait) {}
fn takes_trait2(t: &dyn Trait2) {}

fn main() {
    let t: &dyn Trait3 = &Struct;
    takes_trait(t); // compile error
    takes_trait2(t); // compile error
}
```

* 无法自动向上转型：只能显示向上转型

```rust
trait Trait {}
trait Trait2 {}

trait Trait3: Trait + Trait2 {
    fn as_trait(&self) -> &dyn Trait;
    fn as_trait2(&self) -> &dyn Trait2;
}

impl<T: Trait + Trait2> Trait3 for T {
    fn as_trait(&self) -> &dyn Trait {
        self
    }
    fn as_trait2(&self) -> &dyn Trait2 {
        self
    }
}

struct Struct;
impl Trait for Struct {}
impl Trait2 for Struct {}

fn takes_trait(t: &dyn Trait) {}
fn takes_trait2(t: &dyn Trait2) {}

fn main() {
    let t: &dyn Trait3 = &Struct;
    takes_trait(t.as_trait()); // compiles
    takes_trait2(t.as_trait2()); // compiles
}
```

### 关键点(Key Takeaway)

- Rust不支持超过2个宽度的指针，所以
- 我们不能够把不确定大小类型(unsized type)转换为trait对象
- 我们不能有多trait对象，但是我们可以通过把多个trait合并到一个trait里来解决



## 用户定义的不确定大小类型

```rust
struct Unsized {
  unsized_field: [i32],
}
```

* 可以给结构体 定义一个不确定大小的字段 来定义一个不确定大小的类型
* 不确定大小的结构体 只能有一个不确定大小字段
* 使用一个双宽度 最多只能 追踪 一个 不确定大小字段

如何实例化该 不确定大小类型：

* 尽管如此，根据定义，`Unsized`总是不确定大小的，没有办法构造一个它的确定性大小版本。
* 唯一的解决方法是把这个结构体变成泛型(generic)的，这样它就可以存在于确定性大小和不确定性大小的版本里。

* 申明一个该 确定 大小类型 
* 然后将其转化为 不确定大小类型

```rust
struct MaybeSized<T: ?Sized> {
    maybe_sized: T,
}

fn main() {
    // unsized coercion from MaybeSized<[i32; 3]> to MaybeSized<[i32]>
    let ms: &MaybeSized<[i32]> = &MaybeSized { maybe_sized: [1, 2, 3] };
}
```

* 用户 定义 不确定大小的类型，目前没有什么使用场景，是一个不成熟的特性
* `std::ffi::OsStr`和`std::path::Path`是标准库里的两个不确定大小结构体

## 零大小类型(Zero-Sized Types)

### 单元类型(Unit Type)

最常见的ZST 是单元类型，也见空元祖

* 所空块 {} 的计算结果为 `()` 

* 所有以 分号结尾的 也返回 `()`

* 没有明确返回类型的 也 返回 `()`

* 所有 ()都相等

* 单元类型的 标准Trait实现

  ```rust
  use std::cmp::Ordering;
  
  impl Default for () {
      fn default() {}
  }
  
  impl PartialEq for () {
      fn eq(&self, _other: &()) -> bool {
          true
      }
      fn ne(&self, _other: &()) -> bool {
          false
      }
  }
  
  impl Ord for () {
      fn cmp(&self, _other: &()) -> Ordering {
          Ordering::Equal
      }
  }
  ```

* 编译器理解`()`是零大小类型并且会优化和`()`实例有关的交互。例如:一个`Vec<()>`永远不会执行堆分配，从`Vec`里推进(push)和弹出(pop)`()`只是对它里面的`len`字段进行增加或减少。



### 用户定义的单元结构体(User-Defined Unit Structs)

```rust
struct Struct;
```

单元结构比`()`更有用的一些属性:

*  可以为单元结构体实现 *trait*，而 空元祖 由于 孤儿规则 阻止
* 单元结构体 可以赋予更有意义的名字
* 单元结构体 默认是非 copy类型的

### Never Type

！：它被叫做never类型是因为它表示永远不会产生任何值的计算。

never类型不同于 `()` 它有一些有趣的属性

* ! 可以被强制转化到任意类型
* 无法创建 ！ 类型的实例

```rust
// nice for quick prototyping
fn example<T>(t: &[T]) -> Vec<T> {
    unimplemented!() // ! coerced to Vec<T>
}

fn example2() -> i32 {
    // we know this parse call will never fail
    match "123".parse::<i32>() {
        Some(num) => num,
        None => unreachable!(), // ! coerced to i32
    }
}

fn example3(bool: someCondition) -> &'static str {
    if (!someCondition) {
        panic!() // ! coerced to &str
    } else {
        "str"
    }
}
```

`break`，`continue`，和`return`表达式也有`!`类型：

```rust
fn example() -> i32 {
    // we can set the type of x to anything here
    // since the block never evaluates to any value
    let x: String = {
        return 123 // ! coerced to String
    };
}

fn example2(nums: &[i32]) -> Vec<i32> {
    let mut filtered = Vec::new();
    for num in nums {
        filtered.push(
            if *num < 0 {
                break // ! coerced to i32
            } else if *num % 2 == 0 {
                *num
            } else {
                continue // ! coerced to i32
            }
        );
    }
    filtered
}
```

`!`的第二个有趣的属性让我们能够让我们在类型级别把特定的状态标记为不可能。让我们看看下面的函数:

```rust
fn function() -> Result<Success, Error>;
```





```rust
//永远也不会失败
fn function() -> Result<Success, !>;
//永远也不会成功
fn function() -> Result<!, Error>;
```

**keypoint**

- `!`可以被强制转到到任何其他的类型
- 无法创建`!`的实例，我们可以使用这一点在类型级别把一个状态标记为不可能的



### 用户定义的伪Never类型(User-Defined Pseudo Never Types)

尽管定义一个**能够强制转换到任意其他类型的类型是不可能的**

但是定义一个无法创建实例的类型是有可能的，例如一个没有任何variant的`enum`:

```rust
enum Void {}
```

```rust
enum Void {}

// example 1
impl FromStr for String {
    type Err = Void;
    fn from_str(s: &str) -> Result<String, Self::Err> {
        Ok(String::from(s))
    }
}

// example 2
fn run_server() -> Result<Void, ConnectionError> {
    loop {
        let (request, response) = get_request()?;
        let result = request.process();
        response.send(result);
    }
}
```

这是Rust标准库里使用的技术，因为`String`的`FromStr`实现里的`Err`类型是`std::convert::Infallible`， 其定义如下:

```rust
pub enum Infallible {}
```

### PhantomData

`PhantomData`是一个零大小标记结构体



如果不想实现 Send Sync 这项的自动 trait 要么使用 feature 的 !  功能

```rust
#![feature(negative_impls)]

// this type is Send and Sync
struct Struct;

// opt-out of Send trait
impl !Send for Struct {}

// opt-out of Sync trait
impl !Sync for Struct {}
```

要么增加一个成员变量：是非 send或 sync的

```rust
use std::rc::Rc;

// this type is not Send or Sync
struct Struct {
    // adds 8 bytes to every instance
    _not_send_or_sync: Rc<()>,
}
```

但是这增加了 trait的大小

针对上述的场景，可以使用 *PhantomData*

```rust
use std::rc::Rc;
use std::marker::PhantomData;

type NotSendOrSyncPhantom = PhantomData<Rc<()>>;

// this type is not Send or Sync
struct Struct {
    // adds no additional size to instances
    _not_send_or_sync: NotSendOrSyncPhantom,
}
```

**关键点(Key Takeaway)**

- `PhantomData`是一个零大小标记结构体，可以用于标记一个包含结构体为拥有特定的属性





## 总结(Conclusion)



- 只有确定大小类型(sized type)的实例才可以放到栈上，也就是，可以通过传值的方式传递
- 不确定大小类型(unsized tpe)的实例不能放到栈上而且必须通过传引用的方式传递
- 指向不确定大小类型(unsized tpe)的指针是双宽度的，因为除了保存指向数据的指针外，还需要额外的比特位来追踪数据的长度或者指向一个vtable
- `Sized`是一个"自动(auto)"标记trait
- 所有的泛型类型参数默认是被`Sized`自动约束
- 如果我们有一个泛型函数，它接收隐于指针后的类型`T`为参数，例如`&T`，`Box<T>`，`Rc<T>`等，那么我们总是选择退出默认的`Sized`约束而选用`T:?Sized`约束
- 利用切片和Rust的自动类型强制转换能够让我们写出灵活的API
- 所有的trait默认都是`?Sized`
- 对于`impl Trait for dyn Trait`，要求`Trait: ?Sized`
- 我们可以在每个方法上要求`Self:Sized`
- 由`Sized`约束的trait不能转为trait对象
- Rust不支持超过2个宽度的指针，因此
- 我们不能把不确定大小类型转为trait对象
- 我们不能有多trait对象，但是我们可以通过把多个trait合并到一个trait里来解决这个问题
- 用户定义的不确定类型大小类型是个不成熟的特性，现在其局限性超过所能带来的益处
- ZST的所有实例都相等
- Rust编译器会去优化和ZST相关的交互
- `!`可以被强制转换为其他类型
- 无法创建一个`!`的实例，我们可以利用这一点在类型级别把特定状态标记为不可能
- `PhantomData`是一个零大小标记结构体，可以用于把一个包含结构体标记为含有特定属性

[原文链接](https://zhuanlan.zhihu.com/p/189353352)





## [Rust-dyn 关键字](https://www.cnblogs.com/johnnyzhao/p/15385113.html)

**dyn是trait对象类型的前缀**

dyn关键字用于强调相关trait的方法是动态分配的。要以这种方式使用trait，它必须是“对象安全”的。

与泛型参数或植入型特质不同，编译器不知道被传递的具体类型。也就是说，该类型已经被抹去

因此，一个dyn Trait引用包含两个指针

一个指针指向数据（例如，一个结构的实例）

另一个指针指向方法调用名称与函数指针的映射（被称为虚拟方法表各vtable）

impl trait 和 dyn trait 在Rust分别被称为静态分发和动态分发，即当代码涉及多态时，需要某种机制决定实际调动类型。

每当在堆上分配内存时，Rust都会尝试尽可能明确。因此，如果你的函数以这种方式返回指向堆的trait指针，则需要使用dyn关键字编写返回类型，如示例2:

```rust
fn random_animal(random_number: f64) -> Box<dyn Animal> {
    if random_number < 0.5 {
        Box::new(Sheep {})
    } else {
        Box::new(Cow {})
    }
}
```





