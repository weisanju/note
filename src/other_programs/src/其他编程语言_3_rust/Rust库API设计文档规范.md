## 什么是优雅的 API

* 方法名清晰易懂，以让调用了这个 API 的代码易于阅读。
* 有规律、可猜测的方法名在使用 API 时也很有用，可以减少阅读文档的需求。
* 每个 API 都有至少要有文档和一小段示例代码。
* 用户几乎不需要编写样板代码（boilerplate code）来使用这个 API，因为
  * 它广泛接受各种输入类型（当然类型转换是显式的）
  * 并且也有足以应付大部分常用情况的一键 API

* 充分利用类型来防止逻辑错误，但不会太妨碍使用。
* 返回有意义的错误，并且在文档中注明会导致 panic 的情况。




## 技术
> 有一些 Rust RFC 描述了标准库的命名方案。你也应该遵循它们，以让用户能迅速上手使用你的库。

* RFC 199 解释说应该使用 mut、move 或 ref 作为后缀，来根据参数的可变性区分方法。
* RFC 344 定义了一些有意思的约定，比如：
  * 如何在方法名称中引用类型名称（如 &mut [T] 变成 mut_slice、*mut T 变成 mut ptr），
  * 如何命名返回迭代器的方法，
  * getter 方法应该被命名为 field_name 而 setter 方法应该被命名为 set_field_name，
  * 如何命名 trait：“优先选择（及物）动词、名词，然后是形容词；避免语法后缀（如 able）”，而且“如果这个 trait 只有一个主要方法，可以考虑用方法名称来命名 trait 本身”，

* RFC 430 描述了一些通用的大小写约定（总结：CamelCase 用于类型级别，snake_case 用于变量级别）。
* RFC 445 希望你为扩展 trait（extension trait）添加 Ext 后缀。


除了 RFC 199 和 RFC 344 （见上）规定的以外，还有一些其他的关于如何选择方法名称的约定，目前还没有在 RFC 中提及。这些约定大部分都在旧的 [Rust 风格指南](https://doc.rust-lang.org/1.12.0/style/style/naming/conversions.html)和 @llogiq 的文章 Rustic Bits 以及 [clippy](https://github.com/Manishearth/rust-clippy) 的 wrong_self_convention 检测项中提到了。这里总结一下。

方法名称 | 参数 | 备注 | 举例
------- | ------- | ------- | -------
new | 无 self，通常 >= 1 [^1] | 构造器，另参见 Default | Box::new、std::net::Ipv4Addr::new
with_... | 无 self，>= 1 | 其他构造器 | Vec::with_capacity、regex::Regex::with_size_limit
from_... | 1 | 参见转换 trait（conversion traits） | String::from_utf8_lossy
as_... | &self | 无开销的转换，返回数据的一个视图（view） | str::as_bytes、uuid::Uuid::as_bytes
to_... | &self | 昂贵的转换 | str::to_string、std::path::Path::to_str
into_... | self（消耗） | 可能昂贵的转换，参见 转换 trait（conversion traits） | std::fs::File::into_raw_fd
is_... | &self（或无） | 期望返回 bool | slice::is_empty、Result::is_ok、std::path::Path::is_file
has_... | &self （或无） | 期望返回 bool | regex_syntax::Expr::has_bytes


## 文档测试
编写带有示例代码的文档可以展示 API 的用法而且还能获得自动测试
详见第一版 TRPL（The Rust Programming Language）的[文档](详见第一版 TRPL（The Rust Programming Language）的文档一节。)一节。
```rust
/// 使用魔法操作数字
///
/// # 示例
///
/// ```rust
/// assert_eq!(min( 0,   14),    0);
/// assert_eq!(min( 0, -127), -127);
/// assert_eq!(min(42,  666),   42);
/// ```(由于 hexo markdown 渲染辣鸡，此处加点文字避免被渲染为单独代码块）
fn min(lhs: i32, rhs: i32) -> i32 {
	if lhs < rhs { lhs } else { rhs }
}

```
你还可以使用 #![deny(missing_docs)] 来强制保证每个公开 API 都有文档。你可能也会对我的这篇提出了 [Rust 文档格式化约定](https://deterministic.space/machine-readable-inline-markdown-code-cocumentation.html)的文章感兴趣。



## 不要在 API 中使用 “字符串类型”
> 尽量使用枚举

```rust
enum Color { Red, Green, Blue, LightGoldenRodYellow }

fn color_me(input: &str, color: Color) { /* ... */ }

fn main() {
    color_me("surprised", Color::Blue);
}
```
## 全是常量的模块

或者，如果你想表达更复杂的值的话，则可以定义一个新的 struct，然后定义一堆公共常量。然后把这些常量放到模块中，用户就可以使用与 enum 类似的语法来访问它们了。


```rust
pub mod output_options {
    pub struct OutputOptions { /* ... */ }
    
    impl OutputOptions { fn new(/* ... */) -> OutputOptions { /* ... */ } }
    
    pub const DEFAULT: OutputOptions = OutputOptions { /* ... */ };
    pub const SLIM: OutputOptions = OutputOptions { /* ... */ };
    pub const PRETTY: OutputOptions = OutputOptions { /* ... */ };
}

fn output(f: &Foo, opts: OutputOptions) { /* ... */ }

fn main() {
    let foo = Foo::new();
    
    output(foo, output_options::PRETTY);
}
```
## 使用 FromStr 来解析字符串

在某些情况下，你的用户确实不得不使用字符串，比如：从环境变量中读取或者读取他们的用户的输入作为参数——也就是说，他们没办法在代码中编写（静态）字符串传递给你的 API（这个也是我们尝试阻止的）。这种情况下就需要使用 FromStr triat 了，它抽象了 “解析字符串到 Rust 数据类型” 的行为。


```rust
// 选择 A: 你来解析
fn output_a(f: &Foo, color: &str) -> Result<Bar, ParseError> {
    // 这里使用解析后的类型遮蔽掉了原来的 `color`
    let color: Color = try!(color.parse());

    f.to_bar(&color)
}

// 选择 B: 用户来解析
fn output_b(f: &Foo, color: &Color) -> Bar {
    f.to_bar(color)
}

fn main() {
    let foo = Foo::new();

    // 选择 A: 你来解析，用户来处理 API 错误
    output_a(foo, "Green").expect("Error :(");

    // 选择 B: 用户传入有效类型，所以不需要处理错误
    output_b(foo, Color::Green);

    // 选择 B: 用户使用字符串，需要自己解析并处理错误
    output_b(foo, "Green".parse().except("Parse error!"));
}
```

## 错误处理

TRPL 中对于错误处理[有一章](https://kaisery.gitbooks.io/rust-book-chinese/content/content/Error%20Handling%20%E9%94%99%E8%AF%AF%E5%A4%84%E7%90%86.html)写得很不错。

也有一些 crate 可以用来减少编写良好错误类型所需的样板代码，比如 [quick-error](https://crates.io/crates/quick-error) 和 [error-chain](https://crates.io/crates/error-chain)。


## 公共类型别名

如果你的内部代码常常使用某个参数相同的泛型类型，此时可以使用类型别名。如果你想把这些类型公开给你的用户，你也应该把这些别名同样公开给用户（当然记得文档）。

一个常见情况是 E 为固定值的 `Result<T, E>` 类型。比如 `std::io::Result<T> 是 Result<T, std::io::Error>` 的别名，`std::fmt::Result 是 Result<(), std::fmt::Error> `的别名，`serde_json::error::Result<T> 是 Result<T, serde_json::error::Error>` 的别名。

## 使用转换 trait

一个良好实践是永远也不要在参数中使用 &String 和 &Vec<T>，取而代之使用 &str 和 &[T]，后者允许传入更多类型。（基本上是所有能 deref 到字符串或切片（slice）的类型）

与其使用具体类型作为参数，不如使用拥有严格约束的泛型。这样做的缺点是文档的可读性会降低，因为它充满了大量复杂的泛型约束！


std::convert 为提供了一些方便的工具：
* AsMut：一个便宜的（低消耗）、可变引用到可变引用的转换。
* AsRef：一个便宜的，引用到引用的转换。
* From： 通过转换来构造自身
* Into：一个消耗会自身的转换，可能会比较昂贵（高开销）。
* TryFrom：尝试通过转换来构造自身
* TryInto：尝试消耗自身转的换，可能会比较昂贵。


你可能也会喜欢这篇关于[如何在 Rust 中进行方便地道的转换的文章](https://ricardomartins.cc/2016/08/03/convenient_and_idiomatic_conversions_in_rust).


## Cow
如果你需要处理很多不确定是否需要进行分配（allocate）的东西，你应该使用Cow<'a, B>，它可以让你抽象借用和拥有所有权的数据。

fn foo(p: PathBuf) | fn foo<P: Into<PathBuf>>(p: P)
------- | -------
用户需要把数据转为 PathBuf | 由库来调用 .into() 进行转换
用户进行分配 | 看不出：库可能进行分配
用户需要关心 PathBuf 是什么、如何创建 | 用户可以传递 String 、OsString，或者 PathBuf 都行


## Into<Option<_>>

[这个 PR](https://github.com/rust-lang/rust/pull/34828) 添加了一个 `impl<T> From<T> for Option<T>`，在 Rust 1.12 中正式实装。寥寥几行代码赋予了你编写可以被直接调用而不需要写一大堆 Some(...) 的 API 的能力。

**之前**
```rust
// 对于 API 作者来说很容易编写，文档也很易于阅读
fn foo(lorem: &str, ipsum: Option<i32>, dolor: Option<i32>, sit: Option<i32>) {
    println!("{}", lorem);
}

fn main() {
    foo("bar", None, None, None);               // 看起来有些奇怪
    foo("bar", Some(42), None, None);           // 还好
    foo("bar", Some(42), Some(1337), Some(-1)); // 停！太多…… Some 了……
}
```
**现在**
```rust
// 对于 API 作者来说得多打点字
// （而且遗憾的是，每个参数都需要被单独指定——否则 Rust 只会根据第一个参数推断类型。
// 这种写法阅读来不是很方便，文档可能也没那么好看）
fn foo<I, D, S>(lorem: &str, ipsum: I, dolor: D, sit: S) where
    I: Into<Option<i32>>,
    D: Into<Option<i32>>,
    S: Into<Option<i32>>,
{
    println!("{}", lorem);
}

fn main() {
    foo("bar", None, None, None); // 仍然奇怪
    foo("bar", 42, None, None);   // 不错
    foo("bar", 42, 1337, -1);     // Wow，棒棒！请务必这样编写 API！
}
```


## 关于可能较长的编译时间的说明
如果你有：
* 很多类型参数（比如用于转换 trait）
* 用在一个很复杂/大型的函数上面
* 这个函数用得还很多

然后 rustc 将会根据不同参数编译这个函数的大量排列组合（泛型函数的单态化），这会导致很长的编译时间。


[bluss](https://github.com/bluss) 在 [Reddit](https://www.reddit.com/r/rust/comments/556c0g/optional_arguments_in_rust_112/d8839pu?context=1) 上提到可以使用 “去泛型” 技术来规避这个问题：你的（公共）泛型函数只简单地调用另一个（私有）非泛型函数，这样这个私有函数就只会被编译一次。

bluss 给的例子是 std::fs::OpenOptions::open 的实现（来自 Rust 1.12 的[源码](https://doc.rust-lang.org/1.12.0/src/std/up/src/libstd/fs.rs.html#599-604)）和 image crate 的 [这个PR](https://github.com/PistonDevelopers/image/pull/518)，它将 open 函数修改成了这个样子：


```rust
pub fn open<P>(path: P) -> ImageResult<DynamicImage> where P: AsRef<Path> {
    // 简单的包装函数，在调用 open_impl 之前去掉泛型
    open_impl(path.as_ref())
}
```
## 惰性
尽管 Rust 不能像 Haskell 一样对表达式进行惰性计算，但是你仍然可以使用一些技术来优雅地省略不必要的计算和分配。

### 使用迭代器（Iterator）

标准库中最绝妙的构造之一是 Iterator，它是一个 trait，允许类似生成器的值迭代，而你只需要为此实现一个 next 方法[^3]。Rust 的迭代器是惰性的，你需要显式地调用一个消费函数才会开始迭代。只是编写 "hello".chars().filter(char::is_white_space) 不会对数据进行任何操作，直到你对它调用像 .collect::<String>() 这样的方法。


### 迭代器作为参数

使用迭代器作为输入可能会让你的 API 更加难以阅读（T: Iterator<Item=Thingy> vs &[Thingy]），但是可以让用户避免内存分配。


不过，事实上，你可能也并不想接受一个宽泛的 Iterator：而是使用 IntoIterator 。这样你就可以得到一个通过调用 .into_iter() 就能轻松转换为迭代器的类型。判断哪些类型实现了 IntoIterator 也很简单——就如文档中所说的：

### 类似 Iterator 的 trait

futures::Stream：如 futures 教程所说，类似 Iterator::next 返回 Option<Self::Item>，Stream::poll 返回一个 Option<Self::Item> 的异步结果（或者返回一个错误）。


### 接受闭包

如果有一个可能比较昂贵的值（暂称为类型 Value），而且它并不会在所有的分支中都被使用到，这时可以考虑使用一个返回这个值的闭包（Fn() -> Value）。


一个实际例子是 Result 中的 unwrap_or 和 unwrap_or_else：
```rust
let res: Result<i32, &str> = Err("oh noes");
res.unwrap_or(42); // 立即返回 `42`

let res: Result<i32, &str> = Err("oh noes");
res.unwrap_or_else(|msg| msg.len() as i32); // 将会在需要的时候调用闭包计算
```
#### 关于惰性的小技巧

让 Deref 完成所有的工作：为你的类型实现 Deref，让它来完成实际的计算逻辑。这个[crate lazy](https://crates.io/crates/lazy)实现了一个能为你完成这件事情的宏（不过需要 unstable 特性）。



## 提升易用性的 trait

这里列举了一些你应该试着为你的类型实现的 trait，它们可以让你的类型更加易用：
* 实现或者派生（derive）“常用” 的 trait 比如 Debug、Hash、PartialEq、PartialOrd、Eq、Ord
* 实现或者派生Default，而不是编写一个不接受任何参数的 new 方法。
* 如果你正在为一个类型实现一个可以将它的数据作为 Iterator 返回的方法，你也应该考虑为这个类型实现IntoIterator。（仅有一种迭代数据的主要方式时，才建议这么做。 另请参见上面有关迭代器的部分。）
* 如果你的自定义数据类型和 std 中的基本类型 T 很相似，请考虑为它实现 Deref<Target=T>，不过请不要滥用——Deref 不是用来模拟继承的！
* 不要编写一个接受字符串作为参数然后返回一个实例的构造方法，请使用FromStr



## 为输入参数实现自定义 trait
例：str::find
`str::find<P: Pattern>(p: P) `接受一个Pattern作为输入，`char、str、FnMut(char) -> bool `等类型都实现了这个 trait

```rust
"Lorem ipsum".find('L');
"Lorem ipsum".find("ipsum");
"Lorem ipsum".find(char::is_whitespace);
```



## 扩展 trait

尽量使用标准库中定义的类型和 trait，因为大部分 Rust 程序员都了解它们，它们经过了充分的测试并且有良好的文档。不过，由于 Rust 标准库倾向于提供有语义含义的类型[^4]，这些类型包含的方法可能对你的 API 来说还不够。幸运的是，Rust 的 “孤儿规则（orphan rules）” 赋予了为任何类型实现任何 trait 的能力——前提是类型和 trait 中的任意一个是在当前 crate 中定义的。



## 装饰结果

如 [Florian](https://twitter.com/Argorak) 在 [“Decorating Results”](http://yakshav.es/decorating-results/) 中写到的，你可以使用这种方法来编写并实现 trait 来为内置类型如 Result 实现自己的方法。举例：

```rust
pub trait GrandResultExt {
    fn party(self) -> Self;
}

impl GrandResultExt for Result<String, Box<Error>> {
    fn party(self) -> Result<String, Box<Error>> {
        if self.is_ok() {
          println!("Wooohoo! 🎉");
        }
        self
    }
}

// 用户代码
fn main() {
    let fortune = library_function()
        .method_returning_result()
        .party()
        .unwrap_or("Out of luck.".to_string());
}
```
Florian 在 lazers 的真实代码中使用了这样的模式装饰了 BoxFuture（来自 futures crate）以让代码更加可读：
```rust
let my_database = client
    .find_database("might_not_exist")
    .or_create();

```

## 扩展 trait

到目前为止，我们已经通过定义并实现自己的 trait 扩展了类型上的可用方法。但你还可以定义扩展其他 trait 的 trait（trait MyTrait: BufRead + Debug {}）。最突出的例子是 [itertools](https://crates.io/crates/itertools) crate，它为 std 的迭代器添加了一大堆方法。

## 建造者模式

通过将一堆小方法串联在一起你可以让复杂的 API 更加易于调用。这个和 Session Type 非常搭（稍后会提到）。derive_builder crate 可以用来为自定义的 struct 自动生成（简单的）Builder

例： std::fs::OpenOptions
```rust
use std::fs::OpenOptions;
let file = OpenOptions::new().read(true).write(true).open("foo.txt");
```

### Session Type

你可以在类型系统中编码一个状态机。
* 每个状态都有不同的类型。
* 每个状态类型都实现了不同的方法。
* 一些方法会消耗这个状态类型（获取所有权）并且返回另一个状态类型。

这个技巧在 Rust 中工作地非常良好，因为你的方法可以将数据移动到新的类型中，并且保证在之后你就无法访问旧状态了。

这是一个关于邮寄包裹的小例子：
```rust
let p: OpenPackage = Package::new();
let p: OpenPackage = package.insert([stuff, padding, padding]);

let p: ClosedPackage = package.seal_up();

// let p: OpenPackage = package.insert([more_stuff]);
//~^ ERROR: No method named `insert` on `ClosedPackage`

let p: DeliveryTracking = package.send(address, postage);

```
一个很好的实际例子是 /u/ssokolow 在 [/r/rust 的这个帖子 ](https://www.reddit.com/r/rust/comments/568yvh/typesafe_unions_in_c_and_rust/d8hcwfs)中给出的：

Hyper 使用这个方法来在编译时保证，你不可能做出诸如 “在请求/响应主体已经开始后又来设置 HTTP 头” 这种经常在 PHP 网站上看到的事。（编译器可以捕获这个错误，因为在该状态下的连接上没有 “set header” 方法，并且由于过时引用会失效，所以被引用的一定是正确的状态。）


[hyper::server 文档](http://hyper.rs/hyper/v0.9.10/hyper/server/index.html#an-aside-write-status)中更详细地解释了这是如何实现的。另一个有趣的想法可以在[ lazers-replicator crate](https://github.com/skade/lazers/blob/96efff493be9312ffc70eac5a04b441952e089eb/lazers-replicator/src/lib.md#verify-peers) 中找到：它使用 std::convert::From来在状态中转换。


## 更多信息：
* 文章 “Beyond Memory Safety With Types” 描述了这项技术如何被用来实现一个漂亮并且类型安全的 IMAP 协议。
* 论文 “Session types for Rust” (PDF)，作者 Thomas Bracht Laumann Jespersen, Philip Munksgaard, and Ken Friis Larsen (2015). DOI
* Andrew Hobden 的帖子 [“Pretty State Machine Patterns in Rust”](https://hoverbear.org/2016/10/12/rust-state-machine-pattern/) 展示了一些在 Rust 的类型系统中实现状态机的方法。





## 使用生命周期
在静态类型语言中，为你的 API 指定类型和 trait 约束是必不可少的，如前文所说的，它们可以帮助防止逻辑错误。此外，Rust 的类型系统还提供了另一个维度：你还可以描述你的数据的生命周期（并编写生命周期约束）。

这可以让你（作为开发者）更轻松地对待借用的数据（而不是使用开销更大的拥有所有权的数据）。尽可能地使用引用在 Rust 中是一个良好实践，因为高性能和 “零分配” 的库也是语言的卖点之一。

不过，你应该尽可能为此编写良好的文档，因为理解生命周期和处理引用对于你的库用户来说可能是个挑战，尤其是对于 Rust 新手来说。


由于某些原因（可能是比较简短），很多生命周期都被命名为 'a、'b或类似的无意义字符，不过如果你了解引用的生命周期对应的资源的话，你可以找到更好的名称。举例来说，如果你将文件读入到内存并且处理对这块内存的引用，可以将它的生命周期命名为 'file，或者如果你在处理一个 TCP 请求并且解析它的数据，则可以将生命周期命名为 'req。


## 将析构代码放在 drop 中

Rust 的所有权规则不仅能用于内存：如果你的数据类型表示着外部资源（比如 TCP 连接），则在超出作用域时，你可以使用 Drop trait 关闭、释放或清理该资源。你可以像在其他语言中使用析构函数（或者 try ... catch ... finally）一样使用它。
实际的例子有：
* 引用计数类型 Rc 和 Arc 使用 Drop 来减少引用计数（并且在计数归零的时候释放拥有的数据）。
* MutexGuard 使用 Drop 来释放它对 Mutex 的锁。
* diesel crate 用 Drop 来关闭数据库连接（比如 SQLite）。




## 案例学习

在 API 设计中使用了一些不错的技巧的 Rust 库：
* hyper：Session Type（见上文）
* diesel：使用拥有复杂的关联类型的 trait 将 SQL 查询编码为类型
* futures：高度抽象并且拥有良好文档的 crate

## 其他设计模式
我在这里介绍的是编写接口的设计模式，即面向用户的 API。虽然我认为其中的一些模式只适用于编写库，但许多模式也同样适用于编写通用应用程序的代码。


你可以在 [Rust Design Patterns](https://github.com/rust-unofficial/patterns) 仓库中找到更多信息



Update 2017-04-27：这篇文章发布以来，Rust 库团队的 @brson 已经发布了一个相当全面的 [Rust API Guidelines](https://github.com/brson/rust-api-guidelines) 文档，囊括了我的所有建议，并且内容更全面。


[^2]: 在其他强类型语言中有一句口号 “making illegal states unrepresentable”。我第一次听说这个是在人们谈论 Haskell 的时候，这也是 F# for fun and profit 的[这篇文章](http://fsharpforfunandprofit.com/posts/designing-with-types-making-illegal-states-unrepresentable/)的标题，和 Richard Feldman 在 elm-conf 2016 上的这篇演讲。


[本文链接](https://rustcc.cn/article?id=67cd8a70-8f32-4984-bdd5-4a8c6c969775)