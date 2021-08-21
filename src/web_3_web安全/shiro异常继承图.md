# shiro异常继承

```
Throwable (java.lang)
    Exception (java.lang)
        RuntimeException (java.lang)
            ShiroException (org.apache.shiro)
                AuthorizationException (org.apache.shiro.authz)
                    UnauthenticatedException (org.apache.shiro.authz)
                    UnauthorizedException (org.apache.shiro.authz)
                    
                AuthenticationException (org.apache.shiro.authc)
                    AccountException (org.apache.shiro.authc)
                        DisabledAccountException (org.apache.shiro.authc)
                            LockedAccountException (org.apache.shiro.authc)
                        ConcurrentAccessException (org.apache.shiro.authc)
                        ExcessiveAttemptsException (org.apache.shiro.authc)
                        UnknownAccountException (org.apache.shiro.authc)
                    CredentialsException (org.apache.shiro.authc)
                        IncorrectCredentialsException (org.apache.shiro.authc)
                        ExpiredCredentialsException (org.apache.shiro.authc)
                    UnsupportedTokenException (org.apache.shiro.authc.pam)


```

* *shiroException* 分为 认证异常 和授权异常
* 授权异常 包括 未认证,未授权
* 认证异常包括
  * 账户异常
    * 账户不可用-> 账户锁定
    * 单账户多访问
    * 多次尝试登录不成功
    * 未知账户
  * 密码凭证异常
    * 密码不正确
    * 密码过期
  * 不支持的该认证方式

```
AuthenticationToken
    RememberMeAuthenticationToken (org.apache.shiro.authc)
        UsernamePasswordToken (org.apache.shiro.authc)
    HostAuthenticationToken (org.apache.shiro.authc)
        UsernamePasswordToken (org.apache.shiro.authc)
        BearerToken (org.apache.shiro.authc)
```





# *AuthenticationToken*

## 类图



## token的消息体

* *username* (*pricipal*)
* *password* (*credentials*)
* *host*:(登录主机名)
* 是否 *记住我*

# *AuthorizationInfo*

## 类图
![类图](https://i.loli.net/2020/06/13/hAyKi1al9pwmNZ7.png)

## 已授权信息

* 一般用作 登录后的访问控制
* 有两个属性
  * 角色集合
  * 对象的权限 是以下两者 集合的 相加
    * String类型集合
    * Permission类型的集合(通过 *PermissionResolver*解析得到的)




# *AuthenticationInfo*

## 类图
![类图](https://i.loli.net/2020/06/13/JRIdQvNhD2cTAnf.png)

## 已认证的信息

* 主要是登录的账户密码信息
* *AuthenticationInfo* 接口提供 *principals*与 *credentials* 信息
* *MergableAuthenticationInfo*可以合并多个 已认证信息
* *SaltedAuthenticationInfo*  :在对密码加密时用到的 *salt*
* *Account* 继承 *AuthenticationInfo* *AuthorizationInfo* 
* *SimpleAccount* 简单实现
* *SimpleAuthenticationInfo* 简单实现




# *Relam*

## 类图
![类图](https://i.loli.net/2020/06/13/Iz7mGTVQCib8F5j.png)

*Relam*解析

* Relam接口
  * 结构
    * 域名
    * 对象的已认证信息
  * 操作
    * 是否支持该token登录
* *CachingRealm*
  * 结构
    * 是否开启了缓存
    * 缓存管理者
  * 操作
    * 可命名的:*Nameable*
    * 可根据*principals*登出,登录后清理缓存:*LogoutAware*
    * 可注入缓存管理者,以及注入后回调:*CacheManagerAware* 
    * 开启关闭缓存



* *AuthenticatingRealm*

  * 密码匹配:*CredentialsMatcher*
    * 默认是简单密码匹配器:*SimpleCredentialsMatcher*
  * 缓存
    * 是否开启 认证缓存
    * 认证消息缓存
      * 默认开启缓存
      * 缓存*token*
    * 认证缓存的标识符
  * 该Realm支持的token认证
    * 默认支持 *UsernamePasswordToken* 

* *AuthorizingRealm*
  
  * 授权
    * 给定用户 是否允许某权限
    * 给定用户 是否拥有某角色
    * 检查权限
    * 检查角色
  
  * *权限解析PermissionResolver*
    * 将字符串的*permissionString* 转换为对象的 *Permission*
  * 角色权限解析
    * 将角色名 解析为对应角色,*RolePermissionResolver*
  
* *SimpleAccountRealm*

  * 保存 用户名-> *SimpleAccount* ,角色名 -> *SimpleRole* 可以支持认证和授权

* *TextConfigurationRealm*
  * 增加对 字符串定义的 *user,role* 解析

* *IniRealm* 解析INI配置文件

* *PropertiesRealm*
  * 支持解析 properties文件
  * 支持可重载配置文件
* *JdbcRealm*
  * sql语句
    * 根据用户名查询密码
    * 根据用户名查询密码和盐
    * 根据用户名查询角色名
    * 跟据用户名查询权限
  * 是否开启权限查询
  * 数据源
  * 盐类型与盐

# *PrincipalCollection*

* 域及其对应的 *principals*



*Subject*

* 认证与授权判断
* 关联 Runnable方法
* 登录登出
* runAs

*DelegatingSubject*

* *subject* 简单实现
* 依赖*SecurityManager*

*SubjectFactory*

* 根据 *SubjectContext*获取实例



*SubjectContext*

* 支持 构建 Subject对象的上下文
  * *SecurityManager*
  * *sessionID*
  * *PrincipalCollection*
  * *IsAuthenticated*
  * *isSessionCreationEnabled*
  * *host*

*SubjectDAO*

* 用来保存 Subject对象的状态
* *save,delete*

*SessionStorageEvaluator*

* 判断*shiro*是否通过一个对象的 *session* 去维持对象的状态

* *DefaultSessionStorageEvaluator*默认简单实现

*DefaultSubjectDAO*

* 需要开启 session状态存储功能
* 保存的状态主要有两个
  * *principals*
  * 登录状态



*SecurityManager*

* subject的登录登出
* 创建*Subject*
* 认证与授权
* 会话管理

*CachingSecurityManager*

* 缓存管理
* 事件总线

*RealmSecurityManager*

* 集成Realm域
  * 为Relam注入缓存管理
  * 为Relam注入事件总线

*AuthenticatingSecurityManager*

* 为SecurityManager加入认证器
* 默认为 *ModularRealmAuthenticator*

*AuthorizingSecurityManager*

* 为 *Realm*注入授权器
* 授权器由以下几个功能
  * 权限验证
  * 权限解析
  * 角色权限解析

*SessionsSecurityManager*

* 注入会话管理器
* 为会话管理器注入 缓存管理器
* 为会话管理器注入 事件管理器

*DefaultSecurityManager*



*Session*

* touch:客户端刷新缓存,以免超时
* *stop*:清理这个*session*
* *session key管理*

* 超时时间
* 最近访问时间
* 开始时间

*ValidatingSession*

* 通常会根据最近访问时间 - 开始时间 是否超过 持续时间

*SimpleSession*

* validatingSession的具体简单实现

*ProxiedSession*

* 代理*Session*

*ImmutableProxiedSession*

* 代理*session*
* 只准读,不准写

*StoppingAwareProxiedSession*

* 内部类 可停止的

*DelegatingSession*

* 默认的简单实现
* *sessionKey* session唯一标识



*SessionListener*

* *onStart*
* *onStop*
* *onExpiration*

*SessionDao*

* *create*:插入到*EIS(Enterprise Information System)*返回*identifier*
* *readSession*:根据 sessionID取*session*
* *update*:根据sessionID更新
* *delete*:删除*session*

* *getActiveSessions*:得到激活的session:没有过期 且没有停止

*AbstractSessionDAO*

* *SessionIdGenerator* id产生器

*CachingSessionDAO*

* 带缓存管理的 *CacheManager*

*EnterpriseCacheSessionDAO*

* 设置缓存管理者
* 默认将session 全部缓存,由缓存去管理

*MemorySessionDAO*

* 基于内存的*SessionDao*

*SessionFactory*

* 根据sessionContext创建 *session*

*SimpleSessionFactory*

* 创建简单*Session*

*SessionContxt*

* *session*上下下文,用来构建session实例
* 操作
  * *host*
    * 初始化*Subject*的源IP或主机名
    * 一般在局域网,不适合在NAT外网
  * *sessionID*
    * session的唯一标识
  * 继承了Map接口

*DefaultSessionContext*

* 使用*HashMap*实现了 *SessionContext*
* 实现了 Host与 Session_id
  * host默认为 *DefaultSessionContext.HOST*
  * *sessioin_id*默认为*DefaultSessionContext.SESSION_ID*

 *SessionValidationScheduler*

* *session*周期性校验接口
  * 是否开启
  * 开启
  * 禁用

*ExecutorServiceSessionValidationScheduler*

* 周期性校验接口的具体实现
  * 开启,禁用
  * 设置线程名
  * 设置校验周期

*SessionManager*

* start:根据session上下文开启一个*session*
* getSession:根据*session_id*取*session*

*ValidatingSessionManager*

* 验证被管理的会话

*AbstractSessionManager*

* 全局超时时间,默认是 30min

*NativeSessionManager*

* 直接在本地(指在该Manager中)管理 session的生命周期
  * 时间:*starttimestamp*,*lastAccessTime*,*getTimeOut*
  * 校验:*isValid*,*checkValid*
  * *touch*,*stop*
  * *sessionAttribute*管理

*AbstractNativeSessionManager*

* 本地*session*管理
* session生命周期事件
* 全局*session*超时设置

*AbstractValidatingSessionManager*

* *session*周期性验证 与手动验证

*DefaultSessionManager*

* *session*自身 存储更新删除
* 缓存管理







*CacheManager*

* 取缓存

*AbstractCacheManager*

* 由*ConcurrentHashMap* 实现取

*MemoryConstrainedCacheManager*

* 由软引用实现的*map*,更加适合内存缓存

*Cache*

* 存取,移除,清空 键值对
* 类似于map操作

*MapCache*

* 由Map实现



*CredentialsMatcher*

* 密码匹配器
* *AuthenticationToken*:外部提交的,*AuthenticationInfo*:内部存储的

*AllowAllCredentialsMatcher*

* 所有都返回true

*PasswordMatcher*

* 密码校验,由*hashingPasswordService*完成密码匹配

*SimpleCredentialsMatcher*

* 直接比较 *byte*

*HashedCredentialsMatcher*

* 比较hash后的byte



*PasswordService*

* 密码加密
* 密码匹配

*HashingPasswordService*

* 密码hash

*DefaultPasswordService*

* 



*CodecSupport*

* *string,file,char[],ByteSource,相互转换*



*ByteSource*

* 字节数组的操作
  * 得到字节数组
  * 转16进制字符串
  * 转base64字符串

*SimpleByteSource*

* 对*ByteSource*的简单实现

  

*Hash*

* 在*ByteSource*的基础上 增加了 算法名,盐值,迭代次数

*SimpleHash*

* 用来替代*AbstractHash* 
* 

*RandomNumberGenerator*

* 得到随机生成书



*HashRequest*

* *HashService*依据 request 得到 *hash*对象
  * 字节数组源
  * 盐值
  * 迭代次数
  * 算法名
  * 内置 Builder构建器

*SimpleHashRequest*

* 简单的*hashRequest*的实现



*HashService*

* 根据 hash请求 计算 得到hash对象

*ConfigurableHashService*

* 可配置的hash
  * 设置 私有盐值
  * 设置hash迭代次数
  * 设置hash算法
  * 设置随机数字生成器

*DefaultHashService*

* 计算*hash*的工作台
* 支持 *RandomNumberGenerator* 的 salty
* 继承了可配置的 *ConfigurableHashService*

*HashFormat*

* 将hash对象格式化成字符串
  * 格式化成十六进制:HexFormat
  * 格式化成 base64编码:Base64Format

*ParsableHashFormat*

* 可解析的*format*
* *parse string->Hash* 

*ModularCryptFormat*

* 模块化的加密组件

*HashFormatFactory*

* 工厂
* *DefaultHashFormatFactory* 
  * 根据*FormatClass* 加载实例类





*Authenticator*

* 根据token认证,*返回AuthenticationInfo*

*AbstractAuthenticator*

* 实现了 与认证有关的通用的工作
* 支持 *AuthenticationListener* 事件的通知
* 实际认证 交给子类来做

*ModularRealmAuthenticator*

* 将账户查找 委托给 可插拔的 Relam集合
* 这个 类只有在 多Relam时才会用到
* 操作
  * 单Relam认证
  * 多Realm认证
  * *doAuthenticate* 判断采用单Relam认证还是多Realm认证

*AuthenticationStrategy*

* 在 多Relam环境下的 认证策略
  * *beforeAllAttempts*
  * *beforeAttempt*
  * *afterAttempt*
  * *afterAllAttempts*

*AbstractAuthenticationStrategy*

* 抽象实现
  * 所有认证开始前:返回SimpleAuthenticationInfo
  * 每个认证开始后:合并*SimpleAuthenticationInfo*

*AtLeastOneSuccessfulStrategy*

* 重写了*afterAllAttempts* 
* 如果所有的都不成功 则报错

*AllSuccessfulStrategy*

* 有一个验证不成功就 抛出异常

*FirstSuccessfulStrategy*

* 返回第一个认证成功的
* 可以选择 认证成功后立即返回



*RememberMeManager*

* 用于单用户 跨*session*的记录
  * 登录成功
  * 登录失败
  * 登出

工具类

*  *SecurityUtils*

* *ThreadContext*



*Environment*

* *getSecurityManager* 获取 *securityManager*

*NamedObjectEnvironment*

* 支持通过名称查找对象的 环境

*DefaultEnvironment*

* 使用*Map*的简单实现

*BasicIniEnvironment*

* shiro入口方法