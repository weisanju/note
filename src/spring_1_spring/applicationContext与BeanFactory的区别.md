## BeanFactory与ApplicationContext的区别

### 功能范围

**BeanFactory**

读取bean配置文档，管理bean的加载、实例化，控制bean的生命周期，维护bean之间的依赖关系

**ApplicationContext**

* 国际化（*MessageSource*）
* 统一的资源文件访问方式，如URL和文件（*ResourceLoader*）
* 事件机制（*ApplicationEventPublisher*）
* 载入多个（有继承关系）上下文 ，使得每一个上下文都专注于一个特定的层次，比如应用的web层  
* AOP（拦截器）



