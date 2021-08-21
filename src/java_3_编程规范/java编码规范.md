# 属性拷贝

禁用 *Apache Beanutils* 可以使用  cglib的 beancopier 或者 *mapstruct*

```java
for (int i = 0; i < times; i++) {
PersonDTO personDTO = new PersonDTO();
BeanCopier copier = BeanCopier.create(PersonDO.class, PersonDTO.
class, false);
copier.copy(personDO, personDTO, null);
}
```





# 自动装箱 与 类型转换之间会带来空指针异常

要避免











# 日志

在各个业务 步骤之间一定要打印日志





