# 类定义

```c++
class Test   {
    private: int i;int j;int k;
    public: Test(){ i=j=k=0; } //构造函数
}
```

# 类实现

```c++
class Array {
private:
    int length;
    int *data;
public:
    Array(int length) {
        std::cout << "initailizer"<<std::endl;
        data = new int[length];
        this->length = 0;
    }
    void setData(int val);
    int getData(int index);
};

int Array::getData(int index) {
    return data[index];
}
void Array::setData(int val) {
    data[length++] = val;
}

```

# 无参与拷贝构造

```c++
//两个特殊的构造函数
无参构造,跟拷贝构造 :简单的对成员变量进行复制
// C++默认提供,这两个
    public:
        Test(){ printf("Test()\n")}
        Test(const Test &obj){ printf("Test(const Test &obj)")}
//一旦自己写了 以上构造函数之一的话,编译器就会不提供
```

# 成员变量

1. 可以定义静态成员变量和静态成员函数

2. 静态成员属于整个类,不需要实例化

3. 可以通过类名直接访问 public 静态成员

4. 可以通过对象名访问 public 静态成员

5. 静态成员函数可以直接访问静态成员变量

6. 使用静态成员

   1.分配空间在外部 int Test::cI=0;

   2.使用通过类名: Test::SetI(5)

7. 从命名空将的角度看,类的静态成员只是类这个命名空间中的全局变量,和全局函数,不同之处是 类可以对静态成员进行访问权限的控制,而命名空间不行
8. 静态成员会进行 默认初始化 0
9. 静态成员可以用来统计 该类 实例化了多少个成员变量

# 成员函数与成员变量的空间

- 静态成员变量在全局数据区中存放着,不在类的空间

- 普通成员变量:存储与对象中,与 struct 变量有相同的内存布局和直接对齐方式

- 成员函数是 存储于代码段中
- C++中的 class 从面向对象理论出发,将变量与函数集定义在一起, 在计算机内部,程序依然由 数据段和代码段组成

# C++Class 内部处理

![20210102102643](https://i.loli.net/2021/01/02/clemQG6JXf2TzrD.png)





C++标准库涵盖 C库的功能 <name.h> 头文件对应 C++中的 `<cname>`

C++标准库预定义了多数常用的数据结构

`bitset deque list map queue set stack vector`



# 友元类

private声明使得类的成员不能被外部访问,但是通过friend 关键字可以例外的开放权限



# 操作符重载

什么时候 用全局 ,什么时候 用 成员函数  来 重载操作符

```c++
Array & Array::operator= (const Array &obj){
 	delete [] mspace;
 	mlength = obj.length;
 	mspace = new int[mlength];
 	for(int i=0;i<mlength;i++){
 		mspace[i] = obj.mspace[i];
 	}
}


// C++中通过一个占位参数来区分前置运算符和 后置运算
Complex operator++ (int) // obj++;
{
    Complex ret = *this;
    a++;
    b++;
    return ret;
}

Complex opeator++(){ // ++obj
    ++a;
    ++b;
    return *this;
}


//重载左操作符
ostream& operator<< (ostream&out,const Complex&c){
    out<<c.a<<" + "<<c.b<<"i";
    return out;
}
```

**不要重载 && || 操作符 , 因为会 违背 内定义的 短路法则**

**为了支持 链式调用, 返回其自身的引用**

