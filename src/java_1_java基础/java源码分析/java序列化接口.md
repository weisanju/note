# 前言

Java 序列化是 JDK 1.1 时引入的一组开创性的特性，用于将 Java 对象转换为字节数组，便于存储或传输。此后，仍然可以将字节数组转换回 Java 对象原有的状态。



序列化的思想是“冻结”对象状态，然后写到磁盘或者在网络中传输；反序列化的思想是“解冻”对象状态，重新获得可用的 Java 对象。



**接口定义**

```java
public interface Serializable {
}
```



**序列化方法**

**写入**

```
ObjectOutputStream#
writeObject()→writeObject0()→writeOrdinaryObject()→writeSerialData()→invokeWriteObject()→defaultWriteFields()。
```

**读取**

```
readObject()→readObject0()→readOrdinaryObject()→readSerialData()→defaultReadFields()。
```



```
Serializable 接口之所以定义为空，是因为它只起到了一个标识的作用，告诉程序实现了它的对象是可以被序列化的，但真正序列化和反序列化的操作并不需要它来完成。
```



# 注意事项

static 和 transient 修饰的字段是不会被序列化的。





# Externalizable

> 除了 Serializable 之外，Java 还提供了一个序列化接口 Externalizable（念起来有点拗口）



```java
@Override
public void writeExternal(ObjectOutput out) throws IOException {
    out.writeObject(name);
    out.writeObject(address);
    out.writeInt(age);
}
@Override
public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
    name = (String) in.readObject();
    address = (String) in.readObject();
    age = in.readInt();
}
```







