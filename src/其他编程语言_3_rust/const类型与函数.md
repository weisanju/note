## const types,traits and implementations in rust
    Rust 允许以  const 和 const fn 的形式 的有限形式 编译时函数 执行
虽然最初，const似乎 是一个 合理的简单功能，但它最终提出了大量有趣而复杂得设计问题，
const fn  是一种非常受限的函数
   ：不允许带trait bound的泛型参数：这是因为需要考虑 const代码与 运行时代码 交互的问题

但是很难确定一个满足所有要求，又尽可能简约的设计，


## Proposed design
> 建议设计
* 最要的概念是如何将 const函数 视为 运行时函数或转换为运行时函数
* 始终能够 在运行时调用 const函数

```rust
const fn foo<A: T>(A) -> A;
```
这个cost函数将会以如下方式解析
* 在编译时调用 foo时，必须有一个 类型为A的const常量，且实现了trait T，且 实现 T for A 必须也是 const的实现
* 在运行时调 foo时，没有特殊要求

```rust
fn bar<A: T>(A) -> A;
```
* bar不能在编译时调用
* 在运行时调 foo时，没有特殊要求




## const implementations
> cost实现

* 对于 trait T ，类型A的实现必须要满足：每个函数都必须是 const函数
* 如果视线中有一个不是 const函数，则该实现 不是 const实现
* 如果存在 默认方法，必须也得覆盖，除非默认方法本来就是 const


  
## const functions with generic trait bound types
> 具有泛型trait绑定类型的 const 函数

```rust
const fn baz<A: T>(A) -> A;
```
只接受 tarit T的 const实现的 类型A

## Explicitly-const trait bounds
> 显示const trait 绑定

```rust
fn baz<A: const T>(A) -> A {
    // We can only call a `T` method of `A`
    // in a `const` variable declaration
    // if we know `A` `const`-implements `T`,
    // so the trait bound must explicitly
    // be `const`.
    const X: bool = <A as T>::choice();
    ...
}
```
如果 在函数内部 显示调用了 trait的方法。则必须声明为 impl T for A is const 


## const in traits
```rust
//要求所有实现都必须 const实现 choice
trait T {
    const fn choice() -> bool;
    ...
}
fn baz<A: T>(A) -> A {
    // Now, `<A: const T>` is not needed, since
    // `choice` is always const in any implementation
    // of `T`.
    const X: bool = <A as T>::choice();
    ...
}
```
## Opting out of const trait bounds with ?const
> 选择性退出 const实现

```rust
trait T {
    const fn choice() -> bool;

    fn validate(u8) -> bool;
}

struct S;

impl T for S {
    const fn choice() -> bool {
        ...
    }

    fn validate(u8) -> bool {
        ...
    }
}

const fn bar<A: T>(A) -> A {
    let x: bool = <A as T>::choice();
    ...
}
```
* 如果某个函数 中只使用到了 某个 trait的某个 const函数
* 但是在函数申明时 必须要求 const 实现： trait实现里的所有方法都必须是 const
* 可以通过  显式 const trait bounds 选择退出 来放宽 此要求 ：`?const`

```rust
const fn bar_opt_ct<A: ?const T>(A) -> A {
    let x: bool = <A as T>::choice();
    ...
}
```
* 默认的  const fn  需要 const trait bounds ，而对于运行时没有要求
* 以 `?const` 为前缀的 trait bounds 不需要 const traits bounds 。在编译时、或者运行时


## Removal of the const keyword

由于任何 const 函数都可以在运行时调用，因此它也必须是有效的非 const 函数（在适当的翻译之后）：这就是我们定义的直觉和动机。转换只是修改函数签名，而不更改正文。这种转换非常简单，只需从函数中删除 const 前缀并删除任何 ？const 边界即可。


## Syntactic sugar for const on traits and impls
> const 语法糖

可以将 trait 申明为 const 或者 将 实现 声明为 trait
```rust
const trait V {
    fn foo(C) -> D;
    fn bar(E) -> F;
}
// ...desugars to...
trait V {
    const fn foo(C) -> D;
    const fn bar(E) -> F;
}

struct P;

const impl V for P {
    fn foo(C) -> D;
    fn bar(E) -> F;
}
// ...desugars to...
impl V for P {
    const fn foo(C) -> D;
    const fn bar(E) -> F;
}

```


[参考文章](https://varkor.github.io/blog/2019/01/11/const-types-traits-and-implementations-in-Rust.html)



