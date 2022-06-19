# Python with语句

`with`语句究竟有哪些好处？

它有助于简化一些通用资源管理模式，抽象出其中的功能，将其分解并重用。



```python
with open('hello.txt', 'w') as f:
    f.write('hello, world!')
```

```python
f = open('hello.txt', 'w')
try:
    f.write('hello, world')
finally:
    f.close()
```



`threading.Lock`类是Python标准库中另一个比较好的示例，它有效地使用了`with`语句：



```python
some_lock = threading.Lock()

# 有问题:
some_lock.acquire()
try:
    # 执行某些操作……
finally:
    some_lock.release()

# 改进版:
with some_lock:
    # 执行某些操作……

```

在这两个例子中，使用`with`语句都可以抽象出大部分资源处理逻辑。不必每次都显式地写一个`try...finally`语句，`with`语句会自行处理。



`with`语句不仅让处理系统资源的代码更易读，而且由于绝对不会忘记清理或释放资源，因此还可以避免bug或资源泄漏。



## Python with语句 在自定义对象中支持`with`

无论是`open()`函数和`threading.Lock`类本身，还是它们与`with`语句一起使用，这些都没有什么特殊之处。只要实现所谓的**上下文管理器**（context manager），就可以在自定义的类和函数中获得相同的功能。



详见Python文档: “With Statement Context Managers”。



上下文管理器是什么？这是一个简单的“协议”（或接口），自定义对象需要遵循这个接口来支持`with`语句。

总的来说，如果想将一个对象作为上下文管理器，需要做的就是向其中添加`__enter__`和`__exit__`方法。



```
class ManagedFile:
    def __init__(self, name):
        self.name = name

    def __enter__(self):
        self.file = open(self.name, 'w')
        return self.file

    def __exit__(self, exc_type, exc_val, exc_tb):
        if self.file:
            self.file.close()
Python

```

其中的`ManagedFile`类遵循上下文管理器协议，所以与原来的`open()`例子一样，也支持`with`语句：

```
>>> with ManagedFile('hello.txt') as f:
f.write('hello, world!')
f.write('bye now')
```

