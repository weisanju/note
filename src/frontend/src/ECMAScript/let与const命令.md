{% raw %}
# let命令

## 语法

```
let variable_name
```

## 块级作用域

* 为什么需要块级作用域?

  ES5 只有全局作用域和函数作用域，没有块级作用域

* 只有子作用域可以访问父作用域的变量,其他均不能访问

* 只要块级作用域内存在`let`命令，它所声明的变量就“绑定”（binding）这个区域，不再受外部的影响。-- 暂时性死区 (temporal dead zone)

  ```js
  这样的typeof会报错
  let a = 1
          {
              typeof a
              let a = 1
          }
  ```

* 作用域可以嵌套

  ```javascript
  {{{{
    {let insane = 'Hello World'}
    console.log(insane); // 报错
  }}}};
  ```

* 重复申明
  * 类似java的变量作用域,不同点之出在于 不同作用域之间可以重复命名
  * 同一作用域之间 的同名变量不能重复申明



## 块级作用域与函数声明

**不支持在块级作用域中申明函数**

​	ES5 规定，函数只能在顶层作用域和函数作用域之中声明，不能在块级作用域声明。

```javascript
// 情况一
if (true) {
  function f() {}
}

// 情况二
try {
  function f() {}
} catch(e) {
  // ...
}
```

**ES6 中 在块级作用域函数声明**

​	明确允许在块级作用域之中声明函数。ES6 规定，块级作用域之中，函数声明语句的行为类似于`let`，在块级作用域之外不可引用。

```javascript
// 浏览器的 ES6 环境
function f() { console.log('I am outside!'); }

(function () {
  if (false) {
    // 重复声明一次函数f
    function f() { console.log('I am inside!'); }
  }

  f();
}());
// Uncaught TypeError: f is not a function
```

​	理论上面的代码在 ES6 浏览器中，都会报错。因为兼容问题,ES6 在[附录 B](http://www.ecma-international.org/ecma-262/6.0/index.html#sec-block-level-function-declarations-web-legacy-compatibility-semantics)里面规定，浏览器的实现可以不遵守上面的规定，有自己的[行为方式](http://stackoverflow.com/questions/31419897/what-are-the-precise-semantics-of-block-level-functions-in-es6)。

- 允许在块级作用域内声明函数。
- 函数声明类似于`var`，即会提升到全局作用域或函数作用域的头部。
- 同时，函数声明还会提升到所在的块级作用域的头部。

应该避免在块级作用域内声明函数。如果确实需要，也应该写成函数表达式，而不是函数声明语句。

**变量的申明必须要有 *{}***

```javascript
// 第一种写法，报错
if (true) let x = 1;

// 第二种写法，不报错
if (true) {
  let x = 1;
}
```



# const命令

* `const`声明一个只读的常量。一旦声明，常量的值就不能改变。
* 只是引用不可变, 引用所指向的内容可以变

## ES6中声明变量的六种方法

* ES5: `var`命令和`function`命令
* ES6: let const , import,class



# 顶层对象的属性

​	顶层对象，在浏览器环境指的是`window`对象，在 Node 指的是`global`对象。ES5 之中，顶层对象的属性与全局变量是等价的。

```
window.a = 1;
a // 1

a = 2;
window.a // 2
```

**顶层对象的属性与全局变量挂钩 败笔**

* 没法在编译时就报出变量未声明的错误

  只有运行时才能知道（因为全局变量可能是顶层对象的属性创造的，而属性的创造是动态的）

* 程序员很容易不知不觉地就创建了全局变量

* 顶层对象的属性是到处可以读写的，这非常不利于模块化编程

* `window`对象有实体含义，指的是浏览器的窗口对象，顶层对象是一个有实体含义的对象，也是不合适的。

## ES6对顶层对象的改变

* 为了保持兼容性，`var`命令和`function`命令声明的全局变量，依旧是顶层对象的属性
* `let`命令、`const`命令、`class`命令声明的全局变量，不属于顶层对象的属性(从 ES6 开始，全局变量将逐步与顶层对象的属性脱钩。)

## globalThis 对象 

**顶层对象不统一的实现**

​	JavaScript 语言存在一个顶层对象，它提供全局环境（即全局作用域），所有代码都是在这个环境中运行,但是，顶层对象在各种实现里面是不统一的。

- 浏览器里面，顶层对象是`window`，但 Node 和 Web Worker 没有`window`。
- 浏览器和 Web Worker 里面，`self`也指向顶层对象，但是 Node 没有`self`。
- Node 里面，顶层对象是`global`，但其他环境都不支持。

**一般使用*this* 返回顶层对象 但是有局限性**

* 全局环境中，`this`会返回顶层对象。但是，Node 模块和 ES6 模块中，`this`返回的是当前模块。
* 函数里面的`this`，如果函数不是作为对象的方法运行，而是单纯作为函数运行，`this`会指向顶层对象。但是，严格模式下，这时`this`会返回`undefined`。
* 不管是严格模式，还是普通模式，`new Function('return this')()`，总是会返回全局对象。但是，如果浏览器用了 CSP（Content Security Policy，内容安全策略），那么`eval`、`new Function`这些方法都可能无法使用。

**很难找到一种方法，可以在所有情况下，都取到顶层对象**

```javascript
// 方法一
(typeof window !== 'undefined'
   ? window
   : (typeof process === 'object' &&
      typeof require === 'function' &&
      typeof global === 'object')
     ? global
     : this);

// 方法二
var getGlobal = function () {
  if (typeof self !== 'undefined') { return self; }
  if (typeof window !== 'undefined') { return window; }
  if (typeof global !== 'undefined') { return global; }
  throw new Error('unable to locate global object');
};
```

[ES2020](https://github.com/tc39/proposal-global) 在语言标准的层面，引入`globalThis`作为顶层对象。也就是说，任何环境下，`globalThis`都是存在的，都可以从它拿到顶层对象，指向全局环境下的`this`。

垫片库[`global-this`](https://github.com/ungap/global-this)模拟了这个提案，可以在所有环境拿到`globalThis`。



# 变量的解构赋值

## 数组的解构赋值

**什么是结构赋值**

ES6 允许按照一定模式，从数组和对象中提取值，对变量进行赋值，这被称为解构

**案例**

```javascript
let a = 1;
let b = 2;
let c = 3;

ES6 允许写成下面这样。

let [a, b, c] = [1, 2, 3];

let [foo, [[bar], baz]] = [1, [[2], 3]];
foo // 1
bar // 2
baz // 3

let [ , , third] = ["foo", "bar", "baz"];
third // "baz"

let [x, , y] = [1, 2, 3];
x // 1
y // 3

let [head, ...tail] = [1, 2, 3, 4];
head // 1
tail // [2, 3, 4]

let [x, y, ...z] = ['a'];
x // "a"
y // undefined
z // []


```

**总结**

* 本质上，这种写法属于“模式匹配”，只要等号两边的模式相同，左边的变量就会被赋予对应的值。

* 如果解构不成功，变量的值就等于`undefined`。

* 另一种情况是不完全解构，即等号左边的模式，只匹配一部分的等号右边的数组。这种情况下，解构依然可以成功。

* 如果等号的右边不是数组（或者严格地说，不是可遍历的结构，参见《Iterator》一章），那么将会报错。事实上，只要某种数据结构具有 Iterator 接口，都可以采用数组形式的解构赋值。

  (参见《Generator 函数》一章)

```
//全都报错,因为不可迭代
let [foo] = 1;
let [foo] = false;
let [foo] = NaN;
let [foo] = undefined;
let [foo] = null;
let [foo] = {};
```

**默认值**

```javascript
let [foo = true] = [];
foo // true

let [x, y = 'b'] = ['a']; // x='a', y='b'
let [x, y = 'b'] = ['a', undefined]; // x='a', y='b'
```

> 注意，ES6 内部使用严格相等运算符（`===`），判断一个位置是否有值。所以，只有当一个数组成员严格等于`undefined`，默认值才会生效。

上面代码中，如果一个数组成员是`null`，默认值就不会生效，因为`null`不严格等于`undefined`

```javascript
let [x = 1] = [undefined];
x // 1

let [x = 1] = [null];
x // null
```

**惰性求值**

如果默认值是一个表达式，那么这个表达式是惰性求值的，即只有在用到的时候，才会求值。

```javascript
function f() {
  console.log('aaa');
}

let [x = f()] = [1];
```

**引用解构赋值的其他变量**

>  该变量必须已经声明。

```javascript
let [x = 1, y = x] = [];     // x=1; y=1
let [x = 1, y = x] = [2];    // x=2; y=2
let [x = 1, y = x] = [1, 2]; // x=1; y=2
let [x = y, y = 1] = [];     // ReferenceError: y is not defined
```



## 对象结构赋值

**案例**

```javascript
let { foo, bar } = { foo: 'aaa', bar: 'bbb' };
foo // "aaa"
bar // "bbb"
```



**变量赋值是根据名称来取得**

​	的解构与数组有一个重要的不同。数组的元素是按次序排列的，变量的取值由它的位置决定；而对象的属性没有次序，变量必须与属性同名，才能取到正确的值。



**解构失败,值为undefined**

```javascript
let {foo} = {bar: 'baz'};
foo // undefined
```



**已有对象赋值**

```javascript
// 例一, 将Math对象得 三个方法拿出来赋值
let { log, sin, cos } = Math;

// 例二,将console得 log方法拿出来
const { log } = console;
log('hello') // hello
```



**对象赋值原理**

```javascript
let { foo: baz } = { foo: 'aaa', bar: 'bbb' };
baz // "aaa"
foo // error: foo is not defined
```

这实际上说明，对象的解构赋值是下面形式的简写（参见《对象的扩展》一章）。

```javascript
let { foo: foo, bar: bar } = { foo: 'aaa', bar: 'bbb' };
```

上面代码中，`foo`是匹配的模式，`baz`才是变量。真正被赋值的是变量`baz`



**嵌套解构**

```javascript
let obj = {
  p: [
    'Hello',
    { y: 'World' }
  ]
};

let { p: [x, { y }] } = obj;
x // "Hello"
y // "World"
```

```javascript
let obj = {
  p: [
    'Hello',
    { y: 'World' }
  ]
};

let { p, p: [x, { y }] } = obj;
x // "Hello"
y // "World"
p // ["Hello", {y: "World"}]
```

**个人理解**

模式得写法 就是将 原本对象存在值得地方替换成变量即可

```javascript
    const node = {
        loc: {
            start: {
                line: 1,
                column: 5
            }
        }
    };

    let {
        
        loc: {
            start: {
                line: x,
                column: y
            }
        },
    
        loc: {
            start: e
        }
    } = node;

    console.log(x,y);
    console.log(e.column,e.line);
```



对象中继承的属性 可以解构

```javascript
const obj1 = {};
const obj2 = { foo: 'bar' };
Object.setPrototypeOf(obj1, obj2);

const { foo } = obj1;
foo // "bar"
```

**默认值**

```javascript
var {x = 3} = {};
x // 3

var {x, y = 5} = {x: 1};
x // 1
y // 5

var {x: y = 3} = {};
y // 3

var {x: y = 3} = {x: 5};
y // 5

var { message: msg = 'Something went wrong' } = {};
msg // "Something went wrong"
```

必须是*undefined*

```javascript
var {x = 3} = {x: undefined};
x // 3

var {x = 3} = {x: null};
x // null
```



**对于已经申明过的变量的解构赋值**

```javascript
// 错误的写法
let x;
{x} = {x: 1};
// SyntaxError: syntax error
```

​	因为这会使解析引擎错误的认为是代码块,正确的赋值方式是使用 *()* 括号包裹

**解构左边没有任何变量名**

解构赋值允许等号左边的模式之中，不放置任何变量名。因此，可以写出非常古怪的赋值表达式。

```javascript
({} = [true, false]);
({} = 'abc');
({} = []);
```

  **数组本质是特殊的对象**

 可以对数组进行对象属性的解构,根据索引 进行模式匹配

```javascript
let arr = [1, 2, 3];
let {0 : first, [arr.length - 1] : last} = arr;
first // 1
last // 3
```



## 字符串解构赋值

字符串解构有两种方式

**示例**

```javascript
//字符串被转换成了一个类似数组的对象
const [a, b, c, d, e] = 'hello';
a // "h"
b // "e"
c // "l"
d // "l"
e // "o"
//对象解构赋值
let {length : len} = 'hello';
len // 5

```



## 数值和布尔值的解构赋值

解构赋值时，如果等号右边是数值和布尔值，则会先转为对象。

```javascript
let {toString: s} = 123;
s === Number.prototype.toString // true

let {toString: s} = true;
s === Boolean.prototype.toString // true
```

解构赋值的规则是，只要等号右边的值不是对象或数组，就先将其转为对象。由于`undefined`和`null`无法转为对象，所以对它们进行解构赋值，都会报错。

```javascript
let { prop: x } = undefined; // TypeError
let { prop: y } = null; // TypeError
```

## 函数参数的解构赋值

**一般的参数解构**

```javascript
function add([x, y]){
  return x + y;
}

add([1, 2]); // 3

[[1, 2], [3, 4]].map(([a, b]) => a + b);
```

**内建函数的参数解构**

```javascript
[[1, 2], [3, 4]].map(([a, b]) => a + b);
// [ 3, 7 ]
```

**函数参数的解构也可以使用默认**

```javascript
//参数串:  {x = 0, y = 0} = {}  表明该函数参数是一个对象, 且默认值为{} , 且会被自动解构成x,y,且解构有默认值
function move({x = 0, y = 0} = {}) {
  return [x, y];
}

move({x: 3, y: 8}); // [3, 8]
move({x: 3}); // [3, 0]
move({}); // [0, 0]
move(); // [0, 0]
```

**`undefined`就会触发函数参数的默认值。**

```javascript
[1, undefined, 3].map((x = 'yes') => x);
// [ 1, 'yes', 3 ]
```



## 圆括号问题

### 以下三种解构赋值不得使用圆括号。

* **变量声明语句**

  ```javascript
  // 全部报错
  let [(a)] = [1];
  
  let {x: (c)} = {};
  let ({x: c}) = {};
  let {(x: c)} = {};
  let {(x): c} = {};
  
  let { o: ({ p: p }) } = { o: { p: 2 } };
  ```

* **函数参数**

  > 函数参数也属于变量声明，因此不能带有圆括号。

```javascript
// 报错
function f([(z)]) { return z; }
// 报错
function f([z,(x)]) { return x; }
```

* **赋值语句的模式**

  ```javascript
  // 全部报错
  ({ p: a }) = { p: 42 };
  ([a]) = [5];
  ```

### 可以使用圆括号的情况

可以使用圆括号的情况只有一种：赋值语句的非模式部分，可以使用圆括号。

```javascript
[(b)] = [3]; // 正确
({ p: (d) } = {}); // 正确
[(parseInt.prop)] = [3]; // 正确
```

## 解构的用途

### **交换变量的值**

```javascript
[x, y] = [y, x];
```

### **从函数返回多个值**

```javascript
// 返回一个数组

function example() {
  return [1, 2, 3];
}
let [a, b, c] = example();

// 返回一个对象

function example() {
  return {
    foo: 1,
    bar: 2
  };
}
let { foo, bar } = example();
```

### **函数参数的定义**

解构赋值可以方便地将一组参数与变量名对应起来。

```javascript
// 参数是一组有次序的值
function f([x, y, z]) { ... }
f([1, 2, 3]);

// 参数是一组无次序的值
function f({x, y, z}) { ... }
f({z: 3, y: 2, x: 1});
```

### **提取 JSON 数据**

```javascript
let jsonData = {
  id: 42,
  status: "OK",
  data: [867, 5309]
};

let { id, status, data: number } = jsonData;

console.log(id, status, number);
// 42, "OK", [867, 5309]
```

### **函数参数的默认值**

​	指定参数的默认值，就避免了在函数体内部再写`var foo = config.foo || 'default foo';`这样的语句。

```javascript
jQuery.ajax = function (url, {
  async = true,
  beforeSend = function () {},
  cache = true,
  complete = function () {},
  crossDomain = false,
  global = true,
  // ... more config
} = {}) {
  // ... do stuff
};
```

### **遍历 Map 结构**

任何部署了 Iterator 接口的对象，都可以用`for...of`循环遍历。Map 结构原生支持 Iterator 接口，配合变量的解构赋值，获取键名和键值就非常方便。

```javascript
const map = new Map();
map.set('first', 'hello');
map.set('second', 'world');

for (let [key, value] of map) {
  console.log(key + " is " + value);
}
// first is hello
// second is world
```

如果只想获取键名，或者只想获取键值，可以写成下面这样。

```javascript
// 获取键名
for (let [key] of map) {
  // ...
}

// 获取键值
for (let [,value] of map) {
  // ...
}
```

**输入模块的指定方法**

```javascript
const { SourceMapConsumer, SourceNode } = require("source-map");
```



{% raw %}