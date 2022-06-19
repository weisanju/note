## Summary
允许将一般函数和 继承过来的函数 标记为 const,使它们能够在常量上下文中调用，并带有常量参数

## Motivation

```rust

#[lang="unsafe_cell"]
struct UnsafeCell<T> { pub value: T }
struct AtomicUsize { v: UnsafeCell<usize> }
const ATOMIC_USIZE_INIT: AtomicUsize = AtomicUsize {
    v: UnsafeCell { value: 0 }
};

```

* 为了能在 const 字段中 直接初始化结构体，上述例子只能 将 字段 设置为 pub
* 有了const函数后，可以避免上述的情况


```rust
pub mod unsafe_cell {
    pub struct UnsafeCell<T> {  value: T }
    pub struct AtomicUsize {  v: UnsafeCell<usize> }
    pub const  fn new(a:usize)->AtomicUsize{
        AtomicUsize {
            v:UnsafeCell{
                value:a
            }
        }
    }
}
use unsafe_cell::*;
const ATOMIC_USIZE_INIT: AtomicUsize = unsafe_cell::new(0);
```


## Detailed design
* 函数和继承方法可以标记为 const：
* 只允许 简单的 参数 按值传递
* const 函数体会当做 一个 const 代码块
```rust
const FOO: Foo = {
    // Currently, only item "statements" are allowed here.
    stmts;
    // The function's arguments and constant expressions can be freely combined.
    expr
}
```

## const规则
* 当前支持的 `expr`是
### expr
* 基础类型字面量
* ADTS（tuples、arrays、structs、enum variants
* 基础类型的一元操作、二元操作。unary/binary 
* 强制转换、字段访问、索引
* 无捕获闭包
* 引用和块（只 item statmts,和 tail expression）
###  no side-effects 
* 赋值语句
* non-const function  调用
* inline assembly)
### struct/enum values not allowded for copy trait
struct/enum values are not allowed if their type implements Drop, but this is not transitive, allowing the (perfectly harmless) creation of, e.g. `None::<Vec<T>> `(as an aside, this rule could be used to allow [x; N] even for non-Copy types of x, but that is out of the scope of this RFC)

### references are truly immutable,
* no value with interior mutability can be placed behind a reference,
* mutable references can only be created from zero-sized values (e.g. &mut || {}) -  this allows a reference to be represented just by its value, with no guarantees for the actual address in memory


### raw pointer
* raw pointers can only be created from an integer, a reference or another raw pointer
* cannot be dereferenced or cast back to an integer, which means any constant raw pointer can be represented by either a constant integer or references

### loops
* as a result of not having any side-effects, loops would only affect termination, which has no practical value, thus remaining unimplemented


### conditional control flow 
* although more useful than loops, conditional control flow (if/else and match) also remains unimplemented and only match would pose a challenge


### immutable let bindings
* immutable let bindings in blocks have the same status and implementation difficulty as if/else and they both suffer from a lack of demand (blocks were originally introduced to const/static for scoping items used only in the initializer of a global).

### 可以从任何常量表达式调用 const 函数和方法：
```rust

// Standalone example.
struct Point { x: i32, y: i32 }

impl Point {
    const fn new(x: i32, y: i32) -> Point {
        Point { x: x, y: y }
    }

    const fn add(self, other: Point) -> Point {
        Point::new(self.x + other.x, self.y + other.y)
    }
}

const ORIGIN: Point = Point::new(0, 0);

const fn sum_test(xs: [Point; 3]) -> Point {
    xs[0].add(xs[1]).add(xs[2])
}

const A: Point = Point::new(1, 0);
const B: Point = Point::new(0, 1);
const C: Point = A.add(B);
const D: Point = sum_test([A, B, C]);

// Assuming the Foo::new methods used here are const.
static FLAG: AtomicBool = AtomicBool::new(true);
static COUNTDOWN: AtomicUsize = AtomicUsize::new(10);
#[thread_local]
static TLS_COUNTER: Cell<u32> = Cell::new(1);

```

Type parameters and their bounds are not restricted, though trait methods cannot be called, as they are never const in this design. Accessing trait methods can still be useful - for example, they can be turned into function pointers:

```rust
const fn arithmetic_ops<T: Int>() -> [fn(T, T) -> T; 4] {
    [Add::add, Sub::sub, Mul::mul, Div::div]
}
```

const functions can also be unsafe, allowing construction of types that require invariants to be maintained (e.g. std::ptr::Unique requires a non-null pointer)


```rust
struct OptionalInt(u32);
impl OptionalInt {
    /// Value must be non-zero
    const unsafe fn new(val: u32) -> OptionalInt {
        OptionalInt(val)
    }
}
```