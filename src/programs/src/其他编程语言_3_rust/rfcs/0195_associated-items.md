- Start Date: 2014-08-04
- RFC PR #: [rust-lang/rfcs#195](https://github.com/rust-lang/rfcs/pull/195)
- Rust Issue #: [rust-lang/rust#17307](https://github.com/rust-lang/rust/issues/17307)

# Summary

该RFC使用*关联项*扩展特性，从而实现泛型编程 更方便、可扩展和强大，特别的，traits将包含一系列方法

* Associated functions (already present as "static" functions)
* Associated consts
* Associated types
* Associated lifetimes

这个RFC还为 *multidispatch* traits 提供了一种机制，其中根据不同类型选择不同的 trait实现。

注意:在 RFC 246引入const和静态项之间的区别之前，这个RFC已经被接受了。该文本已经被 更新，以 澄清：关联常量而不是静态，并提供了对关联常量初始实现的限制 的摘要，除了这个修改之外，没有其他的新语法 影响这个提案的修改

# Motivation

> 动机

关联项发挥作用的一个典型例子是数据结构，比如 图，至少包含三种类型:节点、边和图本身。

在今天的《Rust》中，为了将 *graph* 设计为 通用的trait，你必须采用 与图关联的其他类型作为 参数

```rust
trait Graph<N, E> {
    fn has_edge(&self, &N, &N) -> bool;
    ...
}
```



把节点和边类型当做参数这令人困惑，因为任何 具体的图类型他们的边跟节点的类型都是确定的、唯一的

同时，这也带来了不遍，因为使用 泛型的 图的代码 也同样被迫 参数化，即使不是所有的类型都是相关的

```rust
//使用N、E参数化了,所以G必须也是参数化的
fn distance<N, E, G: Graph<N, E>>(graph: &G, start: &N, end: &N) -> uint { ... }
```

使用关联的类型，图trait 可以明确表明  节点和 边类型由impl决定:

```rust
trait Graph {
    type N;
    type E;
    fn has_edge(&self, &N, &N) -> bool;
}
```

客户端可以 直接使用关联类型 表示 图类型

```rust
fn distance<G: Graph>(graph: &G, start: &G::N, end: &G::N) -> uint { ... }
```

下面的小节扩展了 关联类型 的上述好处

## Associated types: engineering benefits for generics

> 泛型的工程好处

关联类型提供了几个工程上的好处

* **Readability and scalability**:可读性和可伸缩性

  关联类型可以一次性抽象整个类型族，而不需要分别命名它们

  这提高了 泛型代码的可读性 (就像上面的 `distance` 函数). 

  它还 使泛型更具“可伸缩性”:traits可以合并其他相关的特性 类型不会给不关心这些的客户带来额外的负担

  相比之下，在今天的Rust中，将额外的通用参数添加到 Trait经常感觉像是一个非常“重量级”的举动。

* **Ease of refactor ing/evolution**：易于重构

  因为trait的用户不必单独参数化它 相关的类型，可以添加新的关联类型而不破坏所有 现有的客户端代码。

  相反，在今天的Rust中，关联类型只能通过给 一个trait 添加更多的类型参数，这会破坏所有使用到这个trait的代码。

  

## Clearer trait matching

>  清晰的trait匹配

traits的类型参数 要么是 输入、输出

* **Inputs**. “input”类型参数用于确定使用哪个类型的实现

* **Outputs**. "output" 类型参数 在选择实现时没有作用。

输入和输出类型在类型推理和   trait 一致性规则 中起着重要作用，这后面会有更详细的描述

在目前绝大多数的库中，唯一的输入类型是' Self ' 类型实现trait，所有其他特征类型参数都是输出

例如，`trait  Iterator<A>` 接受元素的类型形参' A ' 但这种类型总是由具体的“Self”决定的 类型(e.g. `Items<u8>`) ，A类型通常是输出类型


 

Additional input type parameters are useful for cases like binary operators,
where you may want the `impl` to depend on the types of *both*
arguments. For example, you might want a trait

```rust
trait Add<Rhs, Sum> {
    fn add(&self, rhs: &Rhs) -> Sum;
}
```

将' Self '和' Rhs '类型视为输入，将' Sum '类型视为输出 (因为它是由参数类型唯一决定的)。这将允许 ' impl ' s取决于' Rhs '类型，即使' Self '类型是相同的:

```rust
impl Add<int, int> for int { ... }
impl Add<Complex, Complex> for int { ... }
```

 今天的Rust没有明确区分输入类型和输出类型 参数特征。如果你试图提供上面的两个impl，你 会收到如下错误

```
error: conflicting implementations for trait `Add`
```

这个RFC通过

* 将所有trait类型参数视为输入类型，并且
* 提供关联类型，即输出类型



在这个设计中，“Add”trait将会像下面这样写和实现:

```rust
// Self and Rhs are *inputs*
trait Add<Rhs> {
    type Sum; // Sum is an *output*
    fn add(&self, &Rhs) -> Sum;
}

impl Add<int> for int {
    type Sum = int;
    fn add(&self, rhs: &int) -> int { ... }
}

impl Add<Complex> for int {
    type Sum = Complex;
    fn add(&self, rhs: &Complex) -> Complex { ... }
}
```

通过这种方法，一个trait声明像`trait Add<Rhs>{…}` 定义了一个“家族”特征，每个“Rhs”选择一类家族



## Expressiveness

> 善于表现；表情丰富

今天的rust  已经可以表达 Associated types、lifetimes、functions，尽管这样做很笨拙(如上所述)。

但associated _consts_不能表达。

例如，今天的Rust包含了各种数字traits，包括 ' Float '，当前必须将常量公开为静态函数:

```rust
trait Float {
    fn nan() -> Self;
    fn infinity() -> Self;
    fn neg_infinity() -> Self;
    fn neg_zero() -> Self;
    fn pi() -> Self;
    fn two_pi() -> Self;
    ...
} 
```

因为这些函数不能用在常量表达式中，

float类型的模块 同样导出了一组单独的常量作为 const,而没有使用 traits



相关的常量将允许常量直接存在于特征上:

```rust
trait Float {
    const NAN: Self;
    const INFINITY: Self;
    const NEG_INFINITY: Self;
    const NEG_ZERO: Self;
    const PI: Self;
    const TWO_PI: Self;
    ...
}
```

## Why now?



撇开上述动机不谈，添加 associated types 的原因可能并不明显, *现在*(即1.0之前)很重要。主要有两个原因。





首先，这里展示的设计是“不”向后兼容的，因为它 为了trait实现匹配的目的，将trait类型参数重新解释为输入 匹配

输入/输出的区别对一致性规则、类型推断和解析都有区别，这些都将在后面的 RFC。

当然，也有可能给出一个不太理想的设计 关联类型可以稍后添加，而不需要更改 已有的特征类型参数解析

例如，类型参数可以是显式的 标记为输入，否则假定为输出。这将是 不幸的是，因为关联的类型*也*是输出

这会使语言 具有两种指定traits的输出类型的方法

但第二个原因是库的稳定过程:

* 由于trait类型参数的大多数现有用途都是作为输出，它们实际上应该是关联类型。对这些api做出承诺：他们目前面临的风险是，将库置于一个这样的设计中：关联类型 添加后 即原有代码就会被抛弃。这种风险可能会 可以通过不同的向后兼容的 相关项设计来缓解，但是 以牺牲语言本身为代价。

*  二元运算符的特征(例如。' Add ')应该是multidispatch
* It does not seem possible to stabilize them *now* in a way that will support moving to multidispatch later.
* 当前的库中还存在一些棘手的问题,
  * such as the `_equiv`methods accumulating in `HashMap`, 这可以通过关联类型来解决
  * (请参阅下面的“Defaults”以获得关于这个特定示例的更多信息。) 额外的 示例包括：错误传播的 trait、类型转换的 trait

# Detailed design

> 详细设计

## Trait headers

Trait头是根据以下语法写的

```
TRAIT_HEADER =
  'trait' IDENT [ '<' INPUT_PARAMS '>' ] [ ':' BOUNDS ] [ WHERE_CLAUSE ]

INPUT_PARAMS = INPUT_TY { ',' INPUT_TY }* [ ',' ]
INPUT_PARAM  = IDENT [ ':' BOUNDS ]

BOUNDS = BOUND { '+' BOUND }* [ '+' ]
BOUND  = IDENT [ '<' ARGS '>' ]

ARGS   = INPUT_ARGS
       | OUTPUT_CONSTRAINTS
       | INPUT_ARGS ',' OUTPUT_CONSTRAINTS

INPUT_ARGS = TYPE { ',' TYPE }*

OUTPUT_CONSTRAINTS = OUTPUT_CONSTRAINT { ',' OUTPUT_CONSTRAINT }*
OUTPUT_CONSTRAINT  = IDENT '=' TYPE
```

注意：`WHERE_CLAUSE` and `BOUND` 的语法 在下面的 "Constraining associated types" 节中详细解释

一个trait的所有类型参数都被认为是输入，可以用来选择 一个“impl”;

从概念上讲，每个不同实例的类型 都会产生一个 截然不同的trait。更多细节在"The input/output type
distinction"一节中给出 区别”。

## Trait bodies: defining associated items

Trait bodies扩展成包含 三种新事物：

consts, types and lifetimes:

```
TRAIT = TRAIT_HEADER '{' TRAIT_ITEM* '}'
TRAIT_ITEM =
  ... <existing productions>
  | 'const' IDENT ':' TYPE [ '=' CONST_EXP ] ';'
  | 'type' IDENT [ ':' BOUNDS ] [ WHERE_CLAUSE ] [ '=' TYPE ] ';'
  | 'lifetime' LIFETIME_IDENT ';'
```

Traits已经支持 关联函数，就是之前 称作：“static”的函数

关联类型上的 `BOUNDS` and `WHERE_CLAUSE`  是对 trait的挑选、假设

```rust
trait Graph {
    type N: Show + Hash;
    type E: Show + Hash;
    ...
}

impl Graph for MyGraph {
    // Both MyNode and MyEdge must implement Show and Hash
    type N = MyNode;
    type E = MyEdge;
    ...
}

fn print_nodes<G: Graph>(g: &G) {
    // here, can assume G::N implements Show
    ...
}
```

### Namespacing/shadowing for associated types

> 关联类型的命名空间/遮蔽

关联类型可以与作用域中现有类型具有相同的名称，除trait的类型参数之外:

```rust
struct Foo { ... }

trait Bar<Input> {
    type Foo; // this is allowed
    fn into_foo(self) -> Foo; // this refers to the trait's Foo

    type Input; // this is NOT allowed
}
```

By not allowing name clashes between input and output types,
keep open the possibility of later allowing syntax like:

通过不允许输入和输出类型之间的名称冲突， 保留以后允许 如下语法的可能性

```rust
Bar<Input=u8, Foo=uint>
```

where both input and output parameters are constrained by name. And anyway,
there is no use for clashing input/output names.

其中输入和输出参数都由名称约束。无论如何, 输入/输出名称冲突是没有用的。

在名字冲突的情况下，如上面的' Foo '，由于某种原因 如果trait需要引用 外部 Foo，它总是可以通过使用' type 别名来做到这一点 在 trait 之外。



### Defaults



注意，关联的常量和类型都允许默认值，就像trait一样 方法和函数可以提供默认值。



作为一种代码重用机制和一种扩展方法，默认值都是有用的 



然而，关联类型的默认值提出了一个有趣的问题：默认方法可以采用默认类型吗

```rust
trait ContainerKey : Clone + Hash + Eq {
    type Query: Hash = Self;
    fn compare(&self, other: &Query) -> bool { self == other }
    fn query_to_key(q: &Query) -> Self { q.clone() };
}

impl ContainerKey for String {
    type Query = str;
    fn compare(&self, other: &str) -> bool {
        self.as_slice() == other
    }
    fn query_to_key(q: &str) -> String {
        q.into_string()
    }
}

impl<K,V> HashMap<K,V> where K: ContainerKey {
    fn find(&self, q: &K::Query) -> &V { ... }
}
```



在这个例子中，' ContainerKey ' trait被用来关联一个' Query ' '类型 (用于查找)具有拥有的键类型

这解决了 `HashMap`中棘手的问题:使用 &str索引，而不是 &String索引

```rust
// H: HashMap<String, SomeType>
H.find("some literal")
```

而不是写作：

```rust
H.find(&"some literal".to_string())`
```



当前的解决方案包括使用' _equiv '方法复制API表面 使用了一些微妙的“Equiv”特征，但也使用了关联类型方法 使得提供一个覆盖相同用例的简单、单一的API变得容易。

 ' ContainerKey '的默认值只是假设拥有键和查找键 类型是相同的

 但是默认方法必须假定默认值 关联类型，才能正常工作。

不可用 覆盖 Query类型、而保留默认方法。

我们用一种非常简单的方法来处理这个问题:

* 如果一个trait 覆盖了任何默认的关联类型，它们也必须覆盖 覆盖*所有*默认函数和方法。
* 否则，trait实现者可以有选择地覆盖单个默认值 方法/函数，就像今天一样。

## Trait implementations

**triat实现语法**

```
IMPL_ITEM =
  ... <existing productions>
  | 'const' IDENT ':' TYPE '=' CONST_EXP ';'
  | 'type' IDENT' '=' 'TYPE' ';'
  | 'lifetime' LIFETIME_IDENT '=' LIFETIME_REFERENCE ';'
```

类中的任何“type”实现必须满足所有边界和where子句



## Referencing associated items

> 引用关联项

关联项是通过路径引用的

 表达式路径语法为 作为[UFCS]的一部分更新(https://github.com/rust-lang/rfcs/pull/132)，

但 容纳相关的类型和生命周期，我们需要更新类型路径 语法。

完整的语法如下:

```
EXP_PATH
  = EXP_ID_SEGMENT { '::' EXP_ID_SEGMENT }*
  | TYPE_SEGMENT { '::' EXP_ID_SEGMENT }+
  | IMPL_SEGMENT { '::' EXP_ID_SEGMENT }+
EXP_ID_SEGMENT   = ID [ '::' '<' TYPE { ',' TYPE }* '>' ]

TY_PATH
  = TY_ID_SEGMENT { '::' TY_ID_SEGMENT }*
  | TYPE_SEGMENT { '::' TY_ID_SEGMENT }*
  | IMPL_SEGMENT { '::' TY_ID_SEGMENT }+

TYPE_SEGMENT = '<' TYPE '>'
IMPL_SEGMENT = '<' TYPE 'as' TRAIT_REFERENCE '>'
TRAIT_REFERENCE = ID [ '<' TYPE { ',' TYPE * '>' ]
```

下面是一些示例路径，以及它们可能引用的内容

```rust
// Expression paths ///////////////////////////////////////////////////////////////

a::b::c         // reference to a function `c` in module `a::b`
a::<T1, T2>     // the function `a` instantiated with type arguments `T1`, `T2`
Vec::<T>::new   // reference to the function `new` associated with `Vec<T>`
<Vec<T> as SomeTrait>::some_fn
                // reference to the function `some_fn` associated with `SomeTrait`,
                //   as implemented by `Vec<T>`
T::size_of      // the function `size_of` associated with the type or trait `T`
<T>::size_of    // the function `size_of` associated with `T` _viewed as a type_
<T as SizeOf>::size_of
                // the function `size_of` associated with `T`'s impl of `SizeOf`

// Type paths /////////////////////////////////////////////////////////////////////

a::b::C         // reference to a type `C` in module `a::b`
A<T1, T2>       // type A instantiated with type arguments `T1`, `T2`
Vec<T>::Iter    // reference to the type `Iter` associated with `Vec<T>
<Vec<T> as SomeTrait>::SomeType
                // reference to the type `SomeType` associated with `SomeTrait`,
                //   as implemented by `Vec<T>`
```

### Ways to reference items



接下来，我们将详细介绍每种路径的含义。  为了便于讨论，我们假设已经定义了一个trait类似于

```rust
trait Container {
    type E;
    fn empty() -> Self;
    fn insert(&mut self, E);
    fn contains(&self, &E) -> bool where E: PartialEq;
    ...
}

impl<T> Container for Vec<T> {
    type E = T;
    fn empty() -> Vec<T> { Vec::new() }
    ...
}
```

#### Via an `ID_SEGMENT` prefix

> 通过ID_SEGEMENT 前缀 访问

##### 当前缀解析为类型时

获取关联项的最常见方法是通过  带有特征限制类型参数 :

```rust
fn pick<C: Container>(c: &C) -> Option<&C::E> { ... }

fn mk_with_two<C>() -> C where C: Container, C::E = uint {
    let mut cont = C::empty();  // reference to associated function
    cont.insert(0);
    cont.insert(1);
    cont
}
```

要使这些引用有效，必须知道类型参数才能实现 相关的特征:

```rust
// Knowledge via bounds
fn pick<C: Container>(c: &C) -> Option<&C::E> { ... }

// ... or equivalently,  where clause
fn pick<C>(c: &C) -> Option<&C::E> where C: Container { ... }

// Knowledge via ambient constraints
struct TwoContainers<C1: Container, C2: Container>(C1, C2);
impl<C1: Container, C2: Container> TwoContainers<C1, C2> {
    fn pick_one(&self) -> Option<&C1::E> { ... }
    fn pick_other(&self) -> Option<&C2::E> { ... }
}
```

请注意`' Vec<T>::E '和' Vec::<T>::empty '`也是有效的类型和函数

对于像`' C::E '或' Vec<T>::E '`这样的情况，路径以' ID_SEGMENT '开头，前缀本身解析为_type: `' C '和' Vec<T> '`都是类型。在 一般情况下，路径`PREFIX::REST_OF_PATH`，其中`PREFIX`解析为类型是 等价于使用' TYPE_SEGMENT 前缀 `<PREFIX>::REST_OF_PATH` 。因此,对于 下面的例子都是等价的:

```rust
fn pick<C: Container>(c: &C) -> Option<&C::E> { ... }
fn pick<C: Container>(c: &C) -> Option<&<C>::E> { ... }
fn pick<C: Container>(c: &C) -> Option<&<<C>::E>> { ... }
```

`TYPE_SEGMENT`前缀的行为将在下一小节中描述。

##### 当前缀解析为trait时

 `ID_SEGMENT` 前缀可以解析为*trait*' 其行为不同于 ' TYPE_SEGMENT '的类型

如下:

```rust
// a reference Container::insert is roughly equivalent to:
fn trait_insert<C: Container>(c: &C, e: C::E);

// a reference <Container>::insert is roughly equivalent to:
fn object_insert<E>(c: &Container<E=E>, e: E);
```

trait前缀

* A path `PREFIX::REST` resolves to the item/path `REST` defined within
  `Trait`, while treating the type implementing the trait as a type parameter.

* A path `<PREFIX>::REST` treats `PREFIX` as a (DST-style) *type*, and is
  hence usable only with trait objects. See the
  [UFCS RFC](https://github.com/rust-lang/rfcs/pull/132) for more detail.



 请注意，像' Container::E '这样的路径虽然在语法上是有效的，但将失败 ，因为没有办法告诉使用哪个“impl”。 Container::empty '的函数大致相当于:

```rust
fn trait_empty<C: Container>() -> C;
```

#### Via a `TYPE_SEGMENT` prefix

> The following text is *slightly changed* from the
> [UFCS RFC](https://github.com/rust-lang/rfcs/pull/132).

 当路径以`' TYPE_SEGMENT '`开头时，它是一个类型相对路径

如果这已经是一个全部路径，则解析到指定的 类型。如果路径继续(例如，`' <int>::size_of '`)，那么下一个段是 使用以下过程进行解析。

该过程旨在模拟 方法查找，因此对方法查找的任何更改也可能会更改 此查找算法。

Given a path `<T>::m::...`:

1. Search for members of inherent impls defined on `T` (if any) with
   the name `m`. If any are found, the path resolves to that item.
2. Otherwise, let `IN_SCOPE_TRAITS` be the set of traits that are in
   scope and which contain a member named `m`:
   - Let `IMPLEMENTED_TRAITS` be those traits from `IN_SCOPE_TRAITS`
     for which an implementation exists that (may) apply to `T`.
     - There can be ambiguity in the case that `T` contains type inference
       variables.
   - If `IMPLEMENTED_TRAITS` is not a singleton set, report an ambiguity
     error. Otherwise, let `TRAIT` be the member of `IMPLEMENTED_TRAITS`.
   - If `TRAIT` is ambiguously implemented for `T`, report an
     ambiguity error and request further type information.
   - Otherwise, rewrite the path to `<T as Trait>::m::...` and
     continue.

#### Via a `IMPL_SEGMENT` prefix

> The following text is *somewhat different* from the
> [UFCS RFC](https://github.com/rust-lang/rfcs/pull/132).

When a path begins with an `IMPL_SEGMENT`, it is a reference to an item defined
from a trait. Note that such paths must always have a follow-on member `m` (that
is, `<T as Trait>` is not a complete path, but `<T as Trait>::m` is).

To resolve the path, first search for an applicable implementation of `Trait`
for `T`. If no implementation can be found -- or the result is ambiguous -- then
report an error.  Note that when `T` is a type parameter, a bound `T: Trait`
guarantees that there is such an implementation, but does not count for
ambiguity purposes.

Otherwise, resolve the path to the member of the trait with the substitution
`Self => T` and continue.

This apparently straightforward algorithm has some subtle consequences, as
illustrated by the following example:

```rust
trait Foo {
    type T;
    fn as_T(&self) -> &T;
}

// A blanket impl for any Show type T
impl<T: Show> Foo for T {
    type T = T;
    fn as_T(&self) -> &T { self }
}

fn bounded<U: Foo>(u: U) where U::T: Show {
    // Here, we just constrain the associated type directly
    println!("{}", u.as_T())
}

fn blanket<U: Show>(u: U) {
    // the blanket impl applies to U, so we know that `U: Foo` and
    // <U as Foo>::T = U (and, of course, U: Show)
    println!("{}", u.as_T())
}

fn not_allowed<U: Foo>(u: U) {
    // this will not compile, since <U as Trait>::T is not known to
    // implement Show
    println!("{}", u.as_T())
}
```

This example includes three generic functions that make use of an associated
type; the first two will typecheck, while the third will not.

* The first case, `bounded`, places a `Show` constraint directly on the
  otherwise-abstract associated type `U::T`. Hence, it is allowed to assume that
  `U::T: Show`, even though it does not know the concrete implementation of
  `Foo` for `U`.

* The second case, `blanket`, places a `Show` constraint on the type `U`, which
  means that the blanket `impl` of `Foo` applies even though we do not know the
  *concrete* type that `U` will be. That fact means, moreover, that we can
  compute exactly what the associated type `U::T` will be, and know that it will
  satisfy `Show`. Coherence guarantees that that the blanket `impl` is the only
  one that could apply to `U`. (See the section "Impl specialization" under
  "Unresolved questions" for a deeper discussion of this point.)

* The third case assumes only that `U: Foo`, and therefore nothing is known
  about the associated type `U::T`. In particular, the function cannot assume
  that `U::T: Show`.

The resolution rules also interact with instantiation of type parameters in an
intuitive way. For example:

```rust
trait Graph {
    type N;
    type E;
    ...
}

impl Graph for MyGraph {
    type N = MyNode;
    type E = MyEdge;
    ...
}

fn pick_node<G: Graph>(t: &G) -> &G::N {
    // the type G::N is abstract here
    ...
}

let G = MyGraph::new();
...
pick_node(G) // has type: <MyGraph as Graph>::N = MyNode
```

Assuming there are no blanket implementations of `Graph`, the `pick_node`
function knows nothing about the associated type `G::N`. However, a *client* of
`pick_node` that instantiates it with a particular concrete graph type will also
know the concrete type of the value returned from the function -- here, `MyNode`.

## Scoping of `trait` and `impl` items

Associated types are frequently referred to in the signatures of a trait's
methods and associated functions, and it is natural and convenient to refer to
them directly.

In other words, writing this:

```rust
trait Graph {
    type N;
    type E;
    fn has_edge(&self, &N, &N) -> bool;
    ...
}
```

is more appealing than writing this:

```rust
trait Graph {
    type N;
    type E;
    fn has_edge(&self, &Self::N, &Self::N) -> bool;
    ...
}
```

This RFC proposes to treat both `trait` and `impl` bodies (both
inherent and for traits) the same way we treat `mod` bodies: *all*
items being defined are in scope. In particular, methods are in scope
as UFCS-style functions:

```rust
trait Foo {
    type AssocType;
    lifetime 'assoc_lifetime;
    const ASSOC_CONST: uint;
    fn assoc_fn() -> Self;

    // Note: 'assoc_lifetime and AssocType in scope:
    fn method(&self, Self) -> &'assoc_lifetime AssocType;

    fn default_method(&self) -> uint {
        // method in scope UFCS-style, assoc_fn in scope
        let _ = method(self, assoc_fn());
        ASSOC_CONST // in scope
    }
}

// Same scoping rules for impls, including inherent impls:
struct Bar;
impl Bar {
    fn foo(&self) { ... }
    fn bar(&self) {
        foo(self); // foo in scope UFCS-style
        ...
    }
}
```

Items from super traits are *not* in scope, however. See
[the discussion on super traits below](#super-traits) for more detail.

These scope rules provide good ergonomics for associated types in
particular, and a consistent scope model for language constructs that
can contain items (like traits, impls, and modules). In the long run,
we should also explore imports for trait items, i.e. `use
Trait::some_method`, but that is out of scope for this RFC.

Note that, according to this proposal, associated types/lifetimes are *not* in
scope for the optional `where` clause on the trait header. For example:

```rust
trait Foo<Input>
    // type parameters in scope, but associated types are not:
    where Bar<Input, Self::Output>: Encodable {

    type Output;
    ...
}
```

This setup seems more intuitive than allowing the trait header to refer directly
to items defined within the trait body.

It's also worth noting that *trait-level* `where` clauses are never needed for
constraining associated types anyway, because associated types also have `where`
clauses. Thus, the above example could (and should) instead be written as
follows:

```rust
trait Foo<Input> {
    type Output where Bar<Input, Output>: Encodable;
    ...
}
```

## Constraining associated types

Associated types are not treated as parameters to a trait, but in some cases a
function will want to constrain associated types in some way. For example, as
explained in the Motivation section, the `Iterator` trait should treat the
element type as an output:

```rust
trait Iterator {
    type A;
    fn next(&mut self) -> Option<A>;
    ...
}
```

For code that works with iterators generically, there is no need to constrain
this type:

```rust
fn collect_into_vec<I: Iterator>(iter: I) -> Vec<I::A> { ... }
```

But other code may have requirements for the element type:

* That it implements some traits (bounds).
* That it unifies with a particular type.

These requirements can be imposed via `where` clauses:

```rust
fn print_iter<I>(iter: I) where I: Iterator, I::A: Show { ... }
fn sum_uints<I>(iter: I) where I: Iterator, I::A = uint { ... }
```

In addition, there is a shorthand for equality constraints:

```rust
fn sum_uints<I: Iterator<A = uint>>(iter: I) { ... }
```

In general, a trait like:

```rust
trait Foo<Input1, Input2> {
    type Output1;
    type Output2;
    lifetime 'a;
    const C: bool;
    ...
}
```

can be written in a bound like:

```
T: Foo<I1, I2>
T: Foo<I1, I2, Output1 = O1>
T: Foo<I1, I2, Output2 = O2>
T: Foo<I1, I2, Output1 = O1, Output2 = O2>
T: Foo<I1, I2, Output1 = O1, 'a = 'b, Output2 = O2>
T: Foo<I1, I2, Output1 = O1, 'a = 'b, C = true, Output2 = O2>
```

The output constraints must come after all input arguments, but can appear in
any order.

Note that output constraints are allowed when referencing a trait in a *type* or
a *bound*, but not in an `IMPL_SEGMENT` path:

* As a type: `fn foo(obj: Box<Iterator<A = uint>>` is allowed.
* In a bound: `fn foo<I: Iterator<A = uint>>(iter: I)` is allowed.
* In an `IMPL_SEGMENT`: `<I as Iterator<A = uint>>::next` is *not* allowed.

The reason not to allow output constraints in `IMPL_SEGMENT` is that such paths
are references to a trait implementation that has already been determined -- it
does not make sense to apply additional constraints to the implementation when
referencing it.

Output constraints are a handy shorthand when using trait bounds, but they are a
*necessity* for trait objects, which we discuss next.

## Trait objects

When using trait objects, the `Self` type is "erased", so different types
implementing the trait can be used under the same trait object type:

```rust
impl Show for Foo { ... }
impl Show for Bar { ... }

fn make_vec() -> Vec<Box<Show>> {
    let f = Foo { ... };
    let b = Bar { ... };
    let mut v = Vec::new();
    v.push(box f as Box<Show>);
    v.push(box b as Box<Show>);
    v
}
```

One consequence of erasing `Self` is that methods using the `Self` type as
arguments or return values cannot be used on trait objects, since their types
would differ for different choices of `Self`.

In the model presented in this RFC, traits have additional input parameters
beyond `Self`, as well as associated types that may vary depending on all of the
input parameters. This raises the question: which of these types, if any, are
erased in trait objects?

The approach we take here is the simplest and most conservative: when using a
trait as a *type* (i.e., as a trait object), *all* input and output types must
be provided as part of the type. In other words, *only* the `Self` type is
erased, and all other types are specified statically in the trait object type.

Consider again the following example:

```rust
trait Foo<Input1, Input2> {
    type Output1;
    type Output2;
    lifetime 'a;
    const C: bool;
    ...
}
```

Unlike the case for static trait bounds, which do not have to specify any of the
associated types, lifetimes, or consts, (but do have to specify the input types),
trait object types must specify all of the types:

```rust
fn consume_foo<T: Foo<I1, I2>>(t: T) // this is valid
fn consume_obj(t: Box<Foo<I1, I2>>)  // this is NOT valid

// but this IS valid:
fn consume_obj(t: Box<Foo<I1, I2, Output1 = O2, Output2 = O2, 'a = 'static, C = true>>)
```

With this design, it is clear that none of the non-`Self` types are erased as
part of trait objects. But it leaves wiggle room to relax this restriction
later on: trait object types that are not allowed under this design can be given
meaning in some later design.

## Inherent associated items

All associated items are also allowed in inherent `impl`s, so a definition like
the following is allowed:

```rust
struct MyGraph { ... }
struct MyNode { ... }
struct MyEdge { ... }

impl MyGraph {
    type N = MyNode;
    type E = MyEdge;

    // Note: associated types in scope, just as with trait bodies
    fn has_edge(&self, &N, &N) -> bool {
        ...
    }

    ...
}
```

Inherent associated items are referenced similarly to trait associated items:

```rust
fn distance(g: &MyGraph, from: &MyGraph::N, to: &MyGraph::N) -> uint { ... }
```

Note, however, that output constraints do not make sense for inherent outputs:

```rust
// This is *not* a legal type:
MyGraph<N = SomeNodeType>
```

## The input/output type distinction

When designing a trait that references some unknown type, you now have the
option of taking that type as an input parameter, or specifying it as an output
associated type. What are the ramifications of this decision?

### Coherence implications

Input types are used when determining which `impl` matches, even for the same
`Self` type:

```rust
trait Iterable1<A> {
    type I: Iterator<A>;
    fn iter(self) -> I;
}

// These impls have distinct input types, so are allowed
impl Iterable1<u8> for Foo { ... }
impl Iterable1<char> for Foo { ... }

trait Iterable2 {
    type A;
    type I: Iterator<A>;
    fn iter(self) -> I;
}

// These impls apply to a common input (Foo), so are NOT allowed
impl Iterable2 for Foo { ... }
impl Iterable2 for Foo { ... }
```

More formally, the *coherence* property is revised as follows:

- Given a trait and values for all its type parameters (inputs, including
  `Self`), there is at most one applicable `impl`.

In the [trait reform RFC](https://github.com/rust-lang/rfcs/pull/48), coherence
is guaranteed by maintaining two other key properties, which are revised as
follows:

*Orphan check*: Every implementation must meet one of
the following conditions:

1. The trait being implemented (if any) must be defined in the current crate.

2. At least one of the input type parameters (including but not
   necessarily `Self`) must meet the following grammar, where `C`
   is a struct or enum defined within the current crate:

       T = C
         | [T]
         | [T, ..n]
         | &T
         | &mut T
         | ~T
         | (..., T, ...)
         | X<..., T, ...> where X is not bivariant with respect to T

*Overlapping instances*: No two implementations can be instantiable
with the same set of types for the input type parameters.

See the [trait reform RFC](https://github.com/rust-lang/rfcs/pull/48) for more
discussion of these properties.

### Type inference implications

Finally, *output* type parameters can be inferred/resolved as soon as there is
a matching `impl` based on the input type parameters. Because of the
coherence property above, there can be at most one.

On the other hand, even if there is only one applicable `impl`, type inference
is *not* allowed to infer the input type parameters from it. This restriction
makes it possible to ensure *crate concatenation*: adding another crate may add
`impl`s for a given trait, and if type inference depended on the absence of such
`impl`s, importing a crate could break existing code.

In practice, these inference benefits can be quite valuable. For example, in the
`Add` trait given at the beginning of this RFC, the `Sum` output type is
immediately known once the input types are known, which can avoid the need for
type annotations.

## Limitations

The main limitation of associated items as presented here is about associated
*types* in particular. You might be tempted to write a trait like the following:

```rust
trait Iterable {
    type A;
    type I: Iterator<&'a A>; // what is the lifetime here?
    fn iter<'a>(&'a self) -> I;  // and how to connect it to self?
}
```

The problem is that, when implementing this trait, the return type `I` of `iter`
must generally depend on the *lifetime* of self. For example, the corresponding
method in `Vec` looks like the following:

```rust
impl<T> Vec<T> {
    fn iter(&'a self) -> Items<'a, T> { ... }
}
```

This means that, given a `Vec<T>`, there isn't a *single* type `Items<T>` for
iteration -- rather, there is a *family* of types, one for each input lifetime.
In other words, the associated type `I` in the `Iterable` needs to be
"higher-kinded": not just a single type, but rather a family:

```rust
trait Iterable {
    type A;
    type I<'a>: Iterator<&'a A>;
    fn iter<'a>(&self) -> I<'a>;
}
```

In this case, `I` is parameterized by a lifetime, but in other cases (like
`map`) an associated type needs to be parameterized by a type.

In general, such higher-kinded types (HKTs) are a much-requested feature for
Rust, and they would extend the reach of associated types. But the design and
implementation of higher-kinded types is, by itself, a significant investment.
The point of view of this RFC is that associated items bring the most important
changes needed to stabilize our existing traits (and add a few key others),
while HKTs will allow us to define important traits in the future but are not
necessary for 1.0.

### Encoding higher-kinded types

That said, it's worth pointing out that variants of higher-kinded types can be
encoded in the system being proposed here.

For example, the `Iterable` example above can be written in the following
somewhat contorted style:

```rust
trait IterableOwned {
    type A;
    type I: Iterator<A>;
    fn iter_owned(self) -> I;
}

trait Iterable {
    fn iter<'a>(&'a self) -> <&'a Self>::I where &'a Self: IterableOwned {
        IterableOwned::iter_owned(self)
    }
}
```

The idea here is to define a trait that takes, as input type/lifetimes
parameters, the parameters to any HKTs. In this case, the trait is implemented
on the type `&'a Self`, which includes the lifetime parameter.

We can in fact generalize this technique to encode arbitrary HKTs:

```rust
// The kind * -> *
trait TypeToType<Input> {
    type Output;
}
type Apply<Name, Elt> where Name: TypeToType<Elt> = Name::Output;

struct Vec_;
struct DList_;

impl<T> TypeToType<T> for Vec_ {
    type Output = Vec<T>;
}

impl<T> TypeToType<T> for DList_ {
    type Output = DList<T>;
}

trait Mappable
{
    type E;
    type HKT where Apply<HKT, E> = Self;

    fn map<F>(self, f: E -> F) -> Apply<HKT, F>;
}
```

While the above demonstrates the versatility of associated types and `where`
clauses, it is probably too much of a hack to be viable for use in `libstd`.

### Associated consts in generic code

If the value of an associated const depends on a type parameter (including
`Self`), it cannot be used in a constant expression. This restriction will
almost certainly be lifted in the future, but this raises questions outside the
scope of this RFC.

# Staging

Associated lifetimes are probably not necessary for the 1.0 timeframe. While we
currently have a few traits that are parameterized by lifetimes, most of these
can go away once DST lands.

On the other hand, associated lifetimes are probably trivial to implement once
associated types have been implemented.

# Other interactions

## Interaction with implied bounds

As part of the
[implied bounds](http://smallcultfollowing.com/babysteps/blog/2014/07/06/implied-bounds/)
idea, it may be desirable for this:

```rust
fn pick_node<G>(g: &G) -> &<G as Graph>::N
```

to be sugar for this:

```rust
fn pick_node<G: Graph>(g: &G) -> &<G as Graph>::N
```

But this feature can easily be added later, as part of a general implied bounds RFC.

## Future-proofing: specialization of `impl`s

In the future, we may wish to relax the "overlapping instances" rule so that one
can provide "blanket" trait implementations and then "specialize" them for
particular types. For example:

```rust
trait Sliceable {
    type Slice;
    // note: not using &self here to avoid need for HKT
    fn as_slice(self) -> Slice;
}

impl<'a, T> Sliceable for &'a T {
    type Slice = &'a T;
    fn as_slice(self) -> &'a T { self }
}

impl<'a, T> Sliceable for &'a Vec<T> {
    type Slice = &'a [T];
    fn as_slice(self) -> &'a [T] { self.as_slice() }
}
```

But then there's a difficult question:

```
fn dice<A>(a: &A) -> &A::Slice where &A: Slicable {
    a // is this allowed?
}
```

Here, the blanket and specialized implementations provide incompatible
associated types. When working with the trait generically, what can we assume
about the associated type? If we assume it is the blanket one, the type may
change during monomorphization (when specialization takes effect)!

The RFC *does* allow generic code to "see" associated types provided by blanket
implementations, so this is a potential problem.

Our suggested strategy is the following. If at some later point we wish to add
specialization, traits would have to *opt in* explicitly. For such traits, we
would *not* allow generic code to "see" associated types for blanket
implementations; instead, output types would only be visible when all input
types were concretely known. This approach is backwards-compatible with the RFC,
and is probably a good idea in any case.

# Alternatives

## Multidispatch through tuple types

This RFC clarifies trait matching by making trait type parameters inputs to
matching, and associated types outputs.

A more radical alternative would be to *remove type parameters from traits*, and
instead support multiple input types through a separate multidispatch mechanism.

In this design, the `Add` trait would be written and implemented as follows:

```rust
// Lhs and Rhs are *inputs*
trait Add for (Lhs, Rhs) {
    type Sum; // Sum is an *output*
    fn add(&Lhs, &Rhs) -> Sum;
}

impl Add for (int, int) {
    type Sum = int;
    fn add(left: &int, right: &int) -> int { ... }
}

impl Add for (int, Complex) {
    type Sum = Complex;
    fn add(left: &int, right: &Complex) -> Complex { ... }
}
```

The `for` syntax in the trait definition is used for multidispatch traits, here
saying that `impl`s must be for pairs of types which are bound to `Lhs` and
`Rhs` respectively. The `add` function can then be invoked in UFCS style by
writing

```rust
Add::add(some_int, some_complex)
```

*Advantages of the tuple approach*:

- It does not force a distinction between `Self` and other input types, which in
  some cases (including binary operators like `Add`) can be artificial.

- Makes it possible to specify input types without specifying the trait:
  `<(A, B)>::Sum` rather than `<A as Add<B>>::Sum`.

*Disadvantages of the tuple approach*:

- It's more painful when you *do* want a method rather than a function.

- Requires `where` clauses when used in bounds: `where (A, B): Trait` rather
  than `A: Trait<B>`.

- It gives two ways to write single dispatch: either without `for`, or using
  `for` with a single-element tuple.

- There's a somewhat jarring distinction between single/multiple dispatch
  traits, making the latter feel "bolted on".

- The tuple syntax is unusual in acting as a binder of its types, as opposed to
  the `Trait<A, B>` syntax.

- Relatedly, the generics syntax for traits is immediately understandable (a
  family of traits) based on other uses of generics in the language, while the
  tuple notation stands alone.

- Less clear story for trait objects (although the fact that `Self` is the only
  erased input type in this RFC may seem somewhat arbitrary).

On balance, the generics-based approach seems like a better fit for the language
design, especially in its interaction with methods and the object system.

## A backwards-compatible version

Yet another alternative would be to allow trait type parameters to be either
inputs or outputs, marking the inputs with a keyword `in`:

```rust
trait Add<in Rhs, Sum> {
    fn add(&Lhs, &Rhs) -> Sum;
}
```

This would provide a way of adding multidispatch now, and then adding associated
items later on without breakage. If, in addition, output types had to come after
all input types, it might even be possible to migrate output type parameters
like `Sum` above into associated types later.

This is perhaps a reasonable fallback, but it seems better to introduce a clean
design with both multidispatch and associated items together.

# Unresolved questions

## Super traits

This RFC largely ignores super traits.

Currently, the implementation of super traits treats them identically to a
`where` clause that bounds `Self`, and this RFC does not propose to change
that. However, a follow-up RFC should clarify that this is the intended
semantics for super traits.

Note that this treatment of super traits is, in particular, consistent with the
proposed scoping rules, which do not bring items from super traits into scope in
the body of a subtrait; they must be accessed via `Self::item_name`.

## Equality constraints in `where` clauses

This RFC allows equality constraints on types for associated types, but does not
propose a similar feature for `where` clauses. That will be the subject of a
follow-up RFC.

## Multiple trait object bounds for the same trait

The design here makes it possible to write bounds or trait objects that mention
the same trait, multiple times, with different inputs:

```rust
fn mulit_add<T: Add<int> + Add<Complex>>(t: T) -> T { ... }
fn mulit_add_obj(t: Box<Add<int> + Add<Complex>>) -> Box<Add<int> + Add<Complex>> { ... }
```

This seems like a potentially useful feature, and should be unproblematic for
bounds, but may have implications for vtables that make it problematic for trait
objects. Whether or not such trait combinations are allowed will likely depend
on implementation concerns, which are not yet clear.

## Generic associated consts in match patterns

It seems desirable to allow constants that depend on type parameters in match
patterns, but it's not clear how to do so while still checking exhaustiveness
and reachability of the match arms. Most likely this requires new forms of
where clause, to constrain associated constant values.

For now, we simply defer the question.

## Generic associated consts in array sizes

It would be useful to be able to use trait-associated constants in generic code.

```rust
// Shouldn't this be OK?
const ALIAS_N: usize = <T>::N;
let x: [u8; <T>::N] = [0u8; ALIAS_N];
// Or...
let x: [u8; T::N + 1] = [0u8; T::N + 1];
```

However, this causes some problems. What should we do with the following case in
type checking, where we need to prove that a generic is valid for any `T`?

```rust
let x: [u8; T::N + T::N] = [0u8; 2 * T::N];
```

We would like to handle at least some obvious cases (e.g. proving that
`T::N == T::N`), but without trying to prove arbitrary statements about
arithmetic. The question of how to do this is deferred.