# InstantiationAwareBeanPostProcessor

InstantiationAwareBeanPostProcessor接口是BeanPostProcessor的子接口，通过接口字面意思翻译该接口的作用是感知Bean实例话的处理器



| 方法                            | 描述                                                         |
| :------------------------------ | :----------------------------------------------------------- |
| postProcessBeforeInitialization | BeanPostProcessor接口中的方法,在Bean的自定义初始化方法之前执行 |
| postProcessAfterInitialization  | BeanPostProcessor接口中的方法 在Bean的自定义初始化方法执行完成之后执行 |
| postProcessBeforeInstantiation  | 自身方法，是最先执行的方法，它在目标对象实例化之前调用，该方法的返回值类型是Object，我们可以返回任何类型的值。由于这个时候目标对象还未实例化，所以这个返回值可以用来代替原本该生成的目标对象的实例(比如代理对象)。如果该方法的返回值代替原本该生成的目标对象，后续只有postProcessAfterInitialization方法会调用，其它方法不再调用；否则按照正常的流程走 |
| postProcessAfterInstantiation   | 在目标对象实例化之后调用，这个时候对象已经被实例化，但是该实例的属性还未被设置，都是null。因为它的返回值是决定要不要调用postProcessPropertyValues方法的其中一个因素（因为还有一个因素是mbd.getDependencyCheck()）；如果该方法返回false,并且不需要check，那么postProcessPropertyValues就会被忽略不执行；如果返回true，postProcessPropertyValues就会被执行 |
| postProcessPropertyValues       | 对属性值进行修改，如果postProcessAfterInstantiation方法返回false，该方法可能不会被调用。可以在该方法内对属性值进行修改 |



# postProcessBeforeInstantiation

在目标Bean实例化前执行此方法，返回的 *Bean* 对象可能是要使用的代理，而不是目标Bean

有效抑制目标Bean的默认实例化

如果通过此方法返回非空对象，则Bean创建过程 短路



# postProcessAfterInstantiation

* 在 bean通过构造器或工厂方法初始化后，执行指定的操作
* 发生于Spring属性填充（依赖注入）（显示属性引用或 自动注入）
* 对给定bean 执行自定义字段注入,这是一个理想的回调，
* 在Spring自动注入依赖之前、

**返回值**

* 如果属性应该设置在豆上， 则返回true
* 如果属性填充应该被跳过则返回 false
* 返回*false* 会阻止后续的 *InstantiationAwareBeanPostProcessor* 实例调用



# postProcessProperties

**pvs**

工厂即将应用的属性值

**bean**

已经实例化的bean，但是属性还未设置

**beanName** 

bean名称

**返回**

应用与bean的实际属性值

可以返回 *PropertyValues* 实例传递

或者返回 *null*



