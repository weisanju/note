# 类型变换

**类型变换指的是在一种类型的基础上构造 / 映射 / 变换出另一种新类型，是质的转变，即原类型与新类型不在一个抽象层面上。**

### 具体公式

有`X`、`Y`2种类型，而符号`≤`表示子类型关系（比如：`X ≤ Y`即类型`X`是类型`Y`的子类型），`f`表示类型变换，
假设`X ≤ Y`，并且`X`和`Y`经过同一类型变换`f`后构造出对应更复杂的类型`f(X)`和`f(Y)`，那么就可以得出如下这些结论：

- **如果`f(X) ≤ f(Y)`，即保持`X`和`Y`的关系，那么类型变换`f`是协变的（covariant），或具有协变性；**
- **如果`f(Y) ≤ f(X)`，即逆转`X`和`Y`的关系，那么类型变换`f`是逆变的（contravariant），或具有逆变性；**
- **如果即是`f(X) ≤ f(Y)`也是`f(Y) ≤ f(X)`，那么类型变换`f`是双变的（bivariant），或具有双变性；**
- **如果既不是`f(X) ≤ f(Y)`也不是`f(Y) ≤ f(X)`，那么类型变换`f`是不可变的（invariant），或具有不可变**



# 数组的协变

如果一只猫是一只动物，那一群猫是一群动物吗？一群狗是一群动物吗？Java数组认为是的。于是你可以这样写：

```java
Animal[] animals = new Cat[2];

Animal[] animals = new Cat[2];
animals[0] = new Cat();
// 下面这行代码会抛运行时异常
animals[1] = new Dog();
Animal animal = animal[0];
```

这种情况，编译器100%过，而运行时100%抛异常

如果Cat是Animal的子类型，那么Cat[]也是Animal[]的子类型，我们称这种性质为**协变**（covariance）。**Java中，数组是协变的**。



# 泛型的不变性

**Java中的泛型是不变（invariance）**

也就是说，`List<Cat>`并不是`List<Animal>`的子类型



# 消费场景的协变

比如，我希望有一个Animal的集合，我不用去管它里面存的具体类型是什么，但我每次从这个集合取出来的，一定是**一个Animal或其子类**。这是一种典型的消费场景，从集合中取出元素来消费。

在消费场景，Java提供了通配符和extends关键字来支持泛型的协变。

```java
List<? extends Animal> animals = new LinkedList<Cat>();
// 以下四行代码都不能编译通过
// animals.add(new Dog());
// animals.add(new Cat());
// animals.add(new Animal());
// animals.add(new Object());
// 可以添加null，但没意义
animals.add(null);
// 可以安全地取出来
Animal animal = animals.get(0);
```

为什么协变下不能写入呢？因为**协变下写入是不安全的**，



# 生产场景的逆变

我们希望有一个集合，可以往里面写入Animal及其子类。那可以通过super关键字来定义泛型集合：

```java
// 下面这行代码编译不通过
// List<? super Animal> animals = new LinkedList<Cat>();
// 下面都是OK的写法
// List<? super Animal> animals = new LinkedList<Object>();
// List<? super Animal> animals = new LinkedList<Animal>();
// 等价于上面一行的写法
List<? super Animal> animals = new LinkedList<>();
animals.add(new Cat());
animals.add(new Dog());
// 取出来一定是Object
Object object = animals.get(0);

// 这样写是OK的
List<? super Cat> cats = new LinkedList<Animal>();

```

逆变（contravariance），也称逆协变，从名字可以看出来，它与协变的性质是相反的。也就是说，`List<Animal>` 是`List<? super Cat>`的子类型。

# 任意类型通配符

也就是说，它是“无界”的，对于任意类型X，`List<X>`都是`List<?>`的子类型。但`List<?>`不能`add`，get出来也是Object类型。它同时具有协变和逆变的两种性质，上界是Object，但不能调用add方法。

那它与List<Object>有什么区别呢？根据前面的推断，有两个比较明显的区别：

- List<Object>可以调用add方法，但List<?>不能。
- `List<?>`可以协变，上界是Object，但List<Object>不能协变。





### 何时限制通配符的上界或下界？

**PECS表示producer-extends，consumer-super。**

**更加通俗具体地理解就是参数化类型是只读的，那就用`extends`限制通配符的上界；参数化类型是只写的，那就用`super`限制通配符的下界。**





# Java泛型中的协变

```
(? extend Animal) 是 Animal的子类类型，但是不确定是哪种
//泛型协变原则：子类类型				父类类型
Collection<? extend Animal>  <= Collection<Animal>
//面向对象：子类类型可以转换成父类类型


? super Animal 是 Animal的 父类类型：
//逆变原则：如何匹配具体的类型

? super Animal <= Animal

//面向对象：子类类型可以转换成父类类型
```





