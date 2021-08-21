# 智能指针简介

1. **指针** （*pointer*）是一个包含内存地址的变量

2. 这个地址 "引用"，或  “指向”（points at）一些其他数据

3. Rust 中最常见的指针是 **引用**（*reference*）引用以 `&` 符号为标志并借用了他们所指向的值

4. 除了引用数据没有任何其他特殊功能。它们也没有任何额外开销，所以应用得最多。

5. **智能指针**（*smart pointers*）是一类数据结构，他们的表现类似指针，但是拥有额外的元数据和功能
6. 智能指针的概念并不为 Rust 所独有；其起源于 C++ 并存在于其他语言中
7. Rust 标准库中不同的智能指针提供了多于引用的额外功能
8. 在 Rust 中，普通引用和智能指针的一个额外的区别是  **引用是一类只借用数据的指针**,智能指针 **拥有** 他们指向的数据。
9. 实际上本书中已经出现过一些智能指针，比如第八章的 `String` 和 `Vec<T>`，虽然当时我们并不这么称呼它们。这些类型都属于智能指针因为它们拥有一些数据并允许你修改它们
10. 它们也带有元数据（比如他们的容量）和额外的功能或保证（`String` 的数据总是有效的 UTF-8 编码）。
11. 智能指针通常使用结构体实现。智能指针区别于常规结构体的显著特性在于其实现了 `Deref` 和 `Drop` trait
12. `Deref` trait 允许智能指针结构体实例表现的像引用一样，这样就可以编写既用于引用、又用于智能指针的代码
13. `Drop` trait 允许我们自定义当智能指针离开作用域时运行的代码。**本章会讨论这些 trait 以及为什么对于智能指针来说他们很重要。**
14. 常见的智能指针

- `Box<T>`，用于在堆上分配值
- `Rc<T>`，一个引用计数类型，其数据可以有多个所有者
- `Ref<T>` 和 `RefMut<T>`，通过 `RefCell<T>` 访问。（ `RefCell<T>` 是一个在运行时而不是在编译时执行借用规则的类型）。

1. 另外我们会涉及 **内部可变性**（*interior mutability*）模式，这是不可变类型暴露出改变其内部值的 API
2.  **引用循环**（*reference cycles*）会如何泄漏内存，以及如何避免。





# 最简单的智能指针：Box`<T>`

## 简介

1. 最简单直接的智能指针是 *box*，其类型是 `Box<T>`

2.  box 允许你将一个值放在堆上而不是栈上。

3. 留在栈上的则是指向堆数据的指针

除了数据被储存在堆上而不是栈上之外，box 没有性能损失。不过也没有很多额外的功能。它们多用于如下场景：

1. 当有一个在编译时未知大小的类型，而又想要在需要确切大小的上下文中使用这个类型值的时候

   box 允许创建递归类型

2. 当有大量数据并希望在确保数据不被拷贝的情况下转移所有权的时候

   转移大量数据的所有权可能会花费很长的时间，因为数据在栈上进行了拷贝。为了改善这种情况下的性能，可以通过 box 将这些数据储存在堆上。接着，只有少量的指针数据在栈上被拷贝

3. 当希望拥有一个值并只关心它的类型是否实现了特定 trait 而不是其具体类型的时候

    **trait 对象**（*trait object*）



## Box 允许创建递归类型

### 简介

1. Rust 需要在编译时知道类型占用多少空间。一种无法在编译时知道大小的类型是 **递归类型**（*recursive type*）

2. 其值的一部分可以是相同类型的另一个值。这种值的嵌套理论上可以无限的进行下去，
3. 所以 Rust 不知道递归类型需要多少空间。
4. 不过 box 有一个已知的大小，所以通过在循环类型定义中插入 box，就可以创建递归类型了。

```rust
enum List {
    Cons(i32, List),
    Nil,
}

use crate::List::{Cons, Nil};
fn main() {
    let list = Cons(1, Cons(2, Cons(3, Nil)));
}
```



### 计算非递归类型的大小

```rust

enum Message {
    Quit,
    Move { x: i32, y: i32 },
    Write(String),
    ChangeColor(i32, i32, i32),
}
```

当 Rust 需要知道要为 `Message` 值分配多少空间时，它可以检查每一个成员并发现 `Message::Quit` 并不需要任何空间，`Message::Move` 需要足够储存两个 `i32` 值的空间，依此类推。因此，`Message` **值所需的空间等于储存其最大成员的空间大小。**

### 使用 Box\<T>给递归类型一个已知的大小

```rust
enum List {
    Cons(i32, Box<List>),
    Nil,
}

use crate::List::{Cons, Nil};

fn main() {
    let list = Cons(1,
        Box::new(Cons(2,
            Box::new(Cons(3,
                Box::new(Nil))))));
}
```

# 通过 Deref trait 将智能指针当作常规引用处理

> 实现 `Deref` trait 允许我们重载 **解引用运算符**（*dereference operator*）`*`（与乘法运算符或通配符相区别）

> 通过这种方式实现 `Deref` trait 的智能指针可以被当作常规引用来对待，



1. 让我们首先看看解引用运算符如何处理常规引用
2. 接着尝试定义我们自己的类似 `Box<T>` 的类型并看看为何解引用运算符不能像引用一样工作
3. 我们会探索如何实现 `Deref` trait 使得智能指针以类似引用的方式工作变为可能
4. 最后，我们会讨论 Rust 的 **Deref 强制转换**（*deref coercions*）功能以及它是如何处理引用或智能指针的

我们将要构建的 `MyBox<T>` 类型与真正的 `Box<T>` 有一个很大的区别

1. 我们的版本不会在堆上储存数据
2. 这个例子重点关注 `Deref`，所以其数据实际存放在何处，相比其类似指针的行为来说不算重要。



## 通过解引用运算符追踪指针的值

>  常规引用是一个指针类型，一种理解指针的方式是将其看成指向储存在其他某处值的箭头

```rust
fn main() {
    let x = 5;
    let y = &x;

    assert_eq!(5, x);
    assert_eq!(5, *y);
}
```

不能 将 5 与 y比较 因为 y是 引用类型（指针类型）

```sh
6 |   assert_eq!(5, y);

 |   ^^^^^^^^^^^^^^^^^ no implementation for `{integer} == &{integer}`
```

## 像引用一样使用 Box\<T>

```rust
fn main() {
    let x = 5;
    let y = Box::new(x);

    assert_eq!(5, x);
    assert_eq!(5, *y);
}
```

## 自定义智能指针

```rust

use std::ops::Deref;


impl<T> Deref for MyBox<T> {
    type Target = T;

    fn deref(&self) -> &T {
        //返回内部数据的引用
        &self.0
    }
}

//等价于
*(y.deref()) 
//等价于
*(&T)
//等价于
T
```

1. `deref`方法返回值的引用，以及 `*(y.deref())` 括号外边的普通解引用仍为必须的原因在于**所有权**。

2. 如果 `deref` 方法直接返回值而不是值的引用，其值（的所有权）将被移出 `self`
3. 在这里以及大部分使用解引用运算符的情况下我们并不希望获取 `MyBox<T>` 内部值的所有权
4. 每次当我们在代码中使用 `*` 时， `*` 运算符都被替换成了先调用 `deref` 方法再接着使用 `*` 解引用的操作，且只会发生一次，不会对 `*` 操作符无限递归替换
5. 解引用出上面 `i32` 类型的值就停止了



# 函数和方法的隐式 Deref 强制转换

> **Deref 强制转换**（*deref coercions*）是 Rust 在函数或方法传参上的一种便利

1. 其将实现了 `Deref` 的类型的引用  转换为原始类型

2. 通过 `Deref` 所能够转换的类型的引用
3. 当这种特定类型的引用作为实参传递给和形参类型不同的函数或方法时，Deref 强制转换将自动发生，这时会有一系列的 `deref` 方法被调用，把我们提供的类型转换成了参数所需的类型。

Deref 强制转换的加入使得 Rust 程序员编写函数和方法调用时无需增加过多显式使用 `&` 和 `*` 的引用和解引用。这个功能也使得我们可以编写更多同时作用于引用或智能指针的代码。

## **example**

> **对于 str的解引用**

```rust
mod smart_box;
use smart_box::MyBox;
fn main() {
    let my_box = MyBox::new(1);
    let my_box1 = MyBox::new(String::from("xjq"));
    //普通手动解引用
		//*mybox1 为 String
    	//**mybox1 为 str
    	//&**mybox1 为 &str
    print_str(&**my_box1);
    
    //自动强制解引用
    print_str(&my_box1);
}

fn print_str(str: &str){
    print!("{}",str);
}
let my_box1 = MyBox::new(String::from("xjq"));

print_str(&**my_box1);
```

1. Rust 可以通过 `deref` 调用将 `&MyBox<String>` 变为 `&String`
2. Rust 再次调用 `deref` 将 `&String` 变为 `&str`



```rust
fn main() {
    let m = MyBox::new(String::from("Rust"));
    hello(&(*m)[..]);
}
```



## Deref 强制转换如何与可变性交互

1. 类似于如何使用 `Deref` trait 重载不可变引用的 `*` 运算符

2. Rust 提供了 `DerefMut` trait 用于重载可变引用的 `*` 运算符。

   

Rust 在发现类型和 trait 实现满足三种情况时会进行 Deref 强制转换：

1. 当 T: Deref<Target=U> 时从 &T 到 &U。
2. 当 T: DerefMut<Target=U> 时从 &mut T 到 &mut U。
3. 当 T: Deref<Target=U> 时从 &mut T 到 &U。

将一个可变引用转换为不可变引用永远也不会打破借用规则。

# 使用 Drop Trait 运行清理代码

1. 对于智能指针模式来说第二个重要的 trait 是 `Drop`

2. 其允许我们在值要离开作用域时执行一些代码

3. 可以为任何类型提供 `Drop` trait 的实现

4. 同时所指定的代码被用于释放类似于文件或网络连接的资源

我们在智能指针上下文中讨论 `Drop` 是因为其功能几乎总是用于实现智能指针

1. `Drop` trait 包含在 prelude 中
2. `drop` 函数体是放置任何当类型实例离开作用域时期望运行的逻辑的地方

## 通过 std::mem::drop 提早丢弃值

1. 整个 `Drop` trait 存在的意义在于其是自动处理的
2. 你可能希望强制运行 `drop` 方法来释放锁以便作用域中的其他代码可以获取锁
3. Rust 并不允许我们主动调用 `Drop` trait 的 `drop` 方法；
4. 当我们希望在作用域结束之前就强制释放变量的话，我们应该使用的是由标准库提供的 `std::mem::drop`。
5. Rust 中的 `drop` 函数就是这么一个析构函数。
6. 因为不能禁用当值离开作用域时自动插入的 `drop`，并且不能显式调用 `drop`，如果我们需要强制提早清理值，可以使用 `std::mem::drop` 函数。
7. `std::mem::drop` 函数不同于 `Drop` trait 中的 `drop` 方法。可以通过传递希望提早强制丢弃的值作为参数



# Rc\<T> 引用计数智能指针

1. 大部分情况下所有权是非常明确的:可以准确地知道哪个变量拥有某个值

2. 有些情况单个值可能会有多个所有者,例如图数据结构
3. 为了启用多所有权，Rust 有一个叫做 `Rc<T>` 的类型。其名称为 **引用计数**（*reference counting*）的缩写
4. 引用计数意味着记录一个值引用的数量来知晓这个值是否仍在被使用。如果某个值有零个引用，就代表没有任何有效引用并可以被清理。
5. `Rc<T>` 用于当我们希望在堆上分配一些内存供程序的多个部分读取，而且无法在编译时确定程序的哪一部分会最后结束使用它的时候
6. 如果确实知道哪部分是最后一个结束使用的话，就可以令其成为数据的所有者，正常的所有权规则就可以在编译时生效。

> 注意 `Rc<T>` 只能用于单线程场景



## 使用 Rc\<T> 共享数据

1. 不必像调用 `Rc::clone` 增加引用计数那样调用一个函数来减少计数；

2. `Drop` trait 的实现当 `Rc<T>` 值离开作用域时自动减少引用计数。
3. 使用 `Rc<T>` 允许一个值有多个所有者，引用计数则确保只要任何所有者依然存在其值也保持有效。
4. 通过不可变引用， `Rc<T>` 允许在程序的多个部分之间只读地共享数据。如果 `Rc<T>` 也允许多个可变引用,则会违反第四章讨论的借用规则之一：相同位置的多个可变借用可能造成数据竞争和不一致

## RefCell\<T> 和内部可变性模式

1. **内部可变性**（*Interior mutability*）是 Rust 中的一个设计模式

2. 它允许你即使在有不可变引用时也可以改变数据，这通常是借用规则所不允许的

3. 为了改变数据，该模式在数据结构中使用 `unsafe` 代码来模糊 Rust 通常的可变性和借用规则
4. 我们还未讲到不安全代码；第十九章会学习它们
5. 当可以确保代码在运行时会遵守借用规则，即使编译器不能保证的情况，可以选择使用那些运用内部可变性模式的类型
6. 所涉及的 `unsafe` 代码将被封装进安全的 API 中，而外部类型仍然是不可变的。

## 通过 RefCell\<T> 在运行时检查借用规则

如下为选择 `Box<T>`，`Rc<T>` 或 `RefCell<T>` 的理由：

- `Rc<T>` 允许相同数据有多个所有者；`Box<T>` 和 `RefCell<T>` 有单一所有者。
- `Box<T>` 允许在编译时执行不可变或可变借用检查；`Rc<T>`仅允许在编译时执行不可变借用检查；`RefCell<T>` 允许在运行时执行不可变或可变借用检查。
- 因为 `RefCell<T>` 允许在运行时执行可变借用检查，所以我们可以在即便 `RefCell<T>` 自身是不可变的情况下修改其内部的值。



在不可变值内部改变值就是 **内部可变性** 模式。让我们看看何时内部可变性是有用的，并讨论这是如何成为可能的。

## 内部可变性：不可变值的可变借用

```rust
let vec1 = vec![1, 2, 3];
let cell = RefCell::new(vec1);

{
    let mut ref_mut1 = cell.borrow_mut();
    ref_mut1.push(4);
    println!("{:?}", ref_mut1);
}
let mut ref_mut2= cell.borrow_mut();
```

## RefCell\<T> 在运行时记录借用

* 当创建不可变和可变引用时，我们分别使用 `&` 和 `&mut` 语法。对于 `RefCell<T>` 来说，则是 `borrow` 和 `borrow_mut` 方法，这属于 `RefCell<T>` 安全 API 的一部分
* `borrow` 方法返回 `Ref<T>` 类型的智能指针，`borrow_mut` 方法返回 `RefMut` 类型的智能指针
* 这两个类型都实现了 `Deref`，所以可以当作常规引用对待。
* `RefCell<T>` 记录当前有多少个活动的 `Ref<T>` 和 `RefMut<T>` 智能指针
* 每次调用 `borrow`，`RefCell<T>` 将活动的不可变借用计数加一，当 `Ref<T>` 值离开作用域时，不可变借用计数减一，就像编译时借用规则一样
* `RefCell<T>` 在任何时候只允许有多个不可变借用或一个可变借用。
* 如果我们尝试违反这些规则，相比引用时的编译时错误，`RefCell<T>` 的实现会在运行时出现 panic



## 结合 Rc\<T> 和 RefCell\<T> 来拥有多个可变数据所有者

* `RefCell<T>` 的一个常见用法是与 `Rc<T>` 结合
* `Rc<T>` 允许对相同数据有多个所有者，不过只能提供数据的不可变访问
* 如果有一个储存了 `RefCell<T>` 的 `Rc<T>` 的话，就可以得到有多个所有者 **并且** 可以修改的值了！



```rust
pub fn test_ref_rc(){
    let vec1 = vec![1, 2, 3];
    let cell = RefCell::new(vec1);
    let rc = Rc::new(cell);
    let rc1 = rc.clone();
    let rc2 = rc.clone();
    rc1.borrow_mut().push(4);
    rc2.borrow_mut().push(5);

    println!("{:?}",rc);
}
```



# 引用循环与内存泄漏

1. Rust 的内存安全性保证使其难以意外地制造永远也不会被清理的内存（被称为 **内存泄漏**（*memory leak*））
2. 但并不是不可能。与在编译时拒绝数据竞争不同， Rust 并不保证完全地避免内存泄漏，这意味着内存泄漏在 Rust 被认为是内存安全的
3. 这一点可以通过 `Rc<T>` 和 `RefCell<T>` 看出：创建引用循环的可能性是存在的。这会造成内存泄漏，因为每一项的引用计数永远也到不了 0，其值也永远不会被丢弃。

## 制造引用循环

> 创建一个引用循环：两个 `List` 值互相指向彼此

```rust
fn main() {}
use std::rc::Rc;
use std::cell::RefCell;
use crate::List::{Cons, Nil};

#[derive(Debug)]
enum List {
    Cons(i32, RefCell<Rc<List>>),
    Nil,
}

impl List {
    fn tail(&self) -> Option<&RefCell<Rc<List>>> {
        match self {
            Cons(_, item) => Some(item),
            Nil => None,
        }
    }
}

pub fn test_recursive(){
    let a =  Rc::new( Cons(1, RefCell::new(Rc::new(Nil))) ) ;
    println!("a initial rc count = {}", Rc::strong_count(&a));
    println!("a next item = {:?}", a.tail());


    let b = Rc::new( Cons(2,RefCell::new(a.clone())) );
    println!("a rc count after b creation = {}", Rc::strong_count(&a));
    println!("b initial rc count = {}", Rc::strong_count(&b));
    println!("b next item = {:?}", b.tail());




    match  a.tail() {
        None => {}
        Some(item) => {
            *item.borrow_mut() = b.clone();
        }
    }

    println!("b rc count after changing a = {}", Rc::strong_count(&b));
    println!("a rc count after changing a = {}", Rc::strong_count(&a));
}
```



## WeakRef的使用

```rust
use std::rc::{Rc, Weak};
use std::cell::RefCell;
#[derive(Debug)]
struct Node {
    value: i32,
    child: RefCell<Vec<Rc<Node>>>,
    parent: RefCell<Weak<Node>>,
}

impl Node {
    fn new(value: i32, child: Vec<Rc<Node>>) -> Rc<Node> {
        Rc::new(Node {
            value,
            child: RefCell::new(vec![]),
            parent: RefCell::new(Weak::new()),
        }
        )
    }
}

pub fn test_weak_ref() {
    //创建节点1
    let node1 = Node::new(1, vec![]);

    println!("node1 parent = {:?}", node1.parent.borrow().upgrade());


    {
        //创建节点2
        let node2 = Node::new(2, vec![node1.clone()]);

        println!("strong_count:{:?},weak_count:{:?}",Rc::strong_count(&node2),Rc::weak_count(&node2));

        //将parent赋值为node2
        *node1.parent.borrow_mut() = Rc::downgrade(&node2);

        println!("strong_count:{:?},weak_count:{:?}",Rc::strong_count(&node2),Rc::weak_count(&node2));

        println!("node1 parent = {:?}", node1.parent.borrow().upgrade());
    }

    println!("node1 parent = {:?}", node1.parent.borrow().upgrade());
}
```



