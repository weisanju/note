# 概述

Java 序列化是 JDK 1.1 时引入的一组开创性的特性，用于将 Java 对象转换为字节数组，便于存储或传输。此后，仍然可以将字节数组转换回 Java 对象原有的状态。

序列化的思想是“冻结”对象状态，然后写到磁盘或者在网络中传输；反序列化的思想是“解冻”对象状态，重新获得可用的 Java 对象。

再来看看序列化 Serializbale 接口的定义：

```java
public interface Serializable {
}
```

序列化接口本身没有任何作用，起作用的是 *ObjectOutputStream*，ObjectInputStream





# OOS与OIS的序列化与反序列化

以 ObjectOutputStream 为例，它在序列化的时候会依次调用 writeObject()→writeObject0()→writeOrdinaryObject()→writeSerialData()→invokeWriteObject()→defaultWriteFields()。

**核心代码**

```
if (obj instanceof String) {
    writeString((String) obj, unshared);
} else if (cl.isArray()) {
    writeArray(obj, desc, unshared);
} else if (obj instanceof Enum) {
    writeEnum((Enum<?>) obj, desc, unshared);
} else if (obj instanceof Serializable) {
    writeOrdinaryObject(obj, desc, unshared);
} else {
    if (extendedDebugInfo) {
        throw new NotSerializableException(
            cl.getName() + "\n" + debugInfoStack.toString());
    } else {
        throw new NotSerializableException(cl.getName());
    }
}
```

也就是说，ObjectOutputStream 在序列化的时候，会判断被序列化的对象是哪一种类型，字符串？数组？枚举？还是 Serializable，如果全都不是的话，抛出 NotSerializableException。



# 字段选择

* 过滤 STATIC，TRANSIENT

```java
private static ObjectStreamField[] getDefaultSerialFields(Class<?> cl) {
    Field[] clFields = cl.getDeclaredFields();
    ArrayList<ObjectStreamField> list = new ArrayList<>();
    int mask = Modifier.STATIC | Modifier.TRANSIENT;

    int size = list.size();
    return (size == 0) ? NO_FIELDS :
        list.toArray(new ObjectStreamField[size]);
}
```





# Externalizable

除了 Serializable 之外，Java 还提供了一个序列化接口 Externalizable

```java
class Wanger implements Externalizable {
    private String name;
    private int age;

    public Wanger() {

    }

    public String getName() {
        return name;
    }


    @Override
    public String toString() {
        return "Wanger{" + "name=" + name + ",age=" + age + "}";
    }

    @Override
    public void writeExternal(ObjectOutput out) throws IOException {

    }

    @Override
    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {

    }

}
```

**新增了一个无参的构造方法。**

* 使用 Externalizable 进行反序列化的时候，会调用被序列化类的无参构造方法去创建一个新的对象，然后再将被保存对象的字段值复制过去

**新增了两个方法 writeExternal() 和 readExternal()**

1）调用 ObjectOutput 的 writeObject() 方法将字符串类型的 name 写入到输出流中；

2）调用 ObjectOutput 的 writeInt() 方法将整型的 age 写入到输出流中；

3）调用 ObjectInput 的 readObject() 方法将字符串类型的 name 读入到输入流中；

4）调用 ObjectInput 的 readInt() 方法将字符串类型的 age 读入到输入流中；





# serialVersionUID

serialVersionUID 被称为序列化 ID，它是决定 Java 对象能否反序列化成功的重要因子。在反序列化时，Java 虚拟机会把字节流中的 serialVersionUID 与被序列化类中的 serialVersionUID 进行比较，如果相同则可以进行反序列化，否则就会抛出序列化版本不一致的异常。

1）添加一个默认版本的序列化 ID：

```
private static final long serialVersionUID = 1L。
```

2）添加一个随机生成的不重复的序列化 ID。

```
private static final long serialVersionUID = -2095916884810199532L;
```

3）添加 @SuppressWarnings 注解。

```
@SuppressWarnings("serial")
```

