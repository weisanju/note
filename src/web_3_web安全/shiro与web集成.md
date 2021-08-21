# 单机spring应用的整合

配置*LifecycleBeanPostProcessor*

* shiro生命周期 bean后置处理器
* 主要是为了处理 实现了 init,destroy方法的初始化和销毁
* *Initializable*
* *Destroyable*

自定义Realm

```xml
<bean id="myRealm" class="...">
    ...
</bean>

```

获取securityManager

```xml
<bean id="securityManager" class="org.apache.shiro.mgt.DefaultSecurityManager">
    <!-- Single realm app.  If you have multiple realms, use the 'realms' property instead. -->
    <property name="realm" ref="myRealm"/>
</bean>

```

给SecurityUtils绑定 securityManager

```xml
<bean class="org.springframework.beans.factory.config.MethodInvokingFactoryBean">
    <property name="staticMethod" value="org.apache.shiro.SecurityUtils.setSecurityManager"/>
    <property name="arguments" ref="securityManager"/>
</bean>
```



# web应用整合

1. 在web.xml中配置 过滤器代理类 DelegatingFilterProxy

   默认被代理的过滤器 取 与filter-name同名的bean

   ```xml
   <filter>
       <filter-name>shiroFilter</filter-name>
       <filter-class>org.springframework.web.filter.DelegatingFilterProxy</filter-class>
       <init-param>
           <param-name>targetFilterLifecycle</param-name>
           <param-value>true</param-value>
       </init-param>
   </filter>
   <filter-mapping>
       <filter-name>shiroFilter</filter-name>
       <url-pattern>/*</url-pattern>
   </filter-mapping>
   ```

   

2. 配置 ShiroFilterFactoryBean

   

   

   







SecurityUtils:代理 SecurityManager,方便调用

*ShiroFilter* 针对shiro的web过滤器

* *WebSecurityManager*  实现权限认证的对象
* *FilterChainResolver* 用来判断哪个过滤器链来处理

WebSecurityManager

* *DefaultSecurityManager*

  * 基本功能

    * *Subject login(Subject subject, AuthenticationToken authenticationToken)*
    * *void logout(Subject subject);*
    * *Subject createSubject(SubjectContext context);*

  * *RememberMeManager*

    * 

  * Subject

    * 基本功能

      * 登录登出,鉴权

      * runas支持

      * 会话

        

    * 对象构建

      * 解析原则
        * 先从ContextMap缓存中取
        * 如果不存在则从 设置的相应对象中取
        * 如果不存在则从 session中取

      * *SecurityManager*
      * *Session*:与该实例关联的session
      * host
      * *AuthenticationToken*
      * *AuthenticationInfo*
      * *PrincipalCollection*
      * web端功能
        * *ServletRequest*
          * 取认证前的request
        * *ServletResponse*
          * 取认证前的response
        * 重写取host方法

  * *SubjectDAO*

    * 基本功能
      * *Subject save(Subject subject);* :保存subject的认证授权状态
      * *void delete(Subject subject);* 删除subject的认证授权状态
    * 是否支持 session保存 (身份信息,认证状态)
    * 保存逻辑
      * 与之前的比对, 如果有变化则保存,没变化则不保存

    * web端
      * *ServletRequest*
      * *ServletResponse*
      * host

  * *SubjectFactory*

    * 根据 SubjectContext构建 Subject

  * *SessionManager*
    * *session*
      * 标识符
      * 创立时间
      * 最近访问时间
      * 空闲的保持时间
      * touch:更新session时间
      * stop:清楚登录信息
      * 其具体实现 委托给 SecruityManager
    * *HttpServletSession*
      * 基于HttpSession的实现
    * *SessionContext*
      * host
      * sessionid
    * *WebSessionContext*
      * *ServletRequest*
      * *ServletResponse*
    * *SessionKey*
    * *SessionListener*
      * session监听
        * onstart
        * onstop
        * onExpiration
  * *Authorizer*
  * *Authenticator*
  * `Collection<Realm> realms` 
  * *CacheManager*
  * *EventBus* 事件注册,发布器





*SessionManager*

* 基本功能

  * 根据上下文创建session
  * 获取session

* *AbstractSessionManager*

  * 新增全局 session超时时间

* *NativeSessionManager*

  * session中的时间管理
    * 创建时间
    * 最近访问时间
    * 空闲超时时间

  

  * session操作
    * stop(新增)
    * checkValid
    * session属性管理

  

* *AbstractNativeSessionManager*

  抽象实现

  * 创建session
    * 根据上下文创建session (子类实现)
    * 应用全局超时时间
    * onStart给子类回调
    * 通知listener start监听 
  * 查询session
    * 根据 sessionKey查询 子类实现

* *AbstractValidatingSessionManager*

  对上面的类新增 session定时校验功能

* *DefaultSessionManager*
  * *SessionFactory* 创建session
  * *SessionDAO*  session的存取,与状态的同步过更新
    * session create -> sessionDao create
    * *session* onStop ->sessionDao change
    * afterStopped -> delete
    * onExpiration -> onChange
    * 'afterExpired' ->delete
* *DefaultWebSessionManager*
  * *Cookie* 默认名:JSESSIONID



SessionDao

* 基本功能:增删改查
  * 存:create
  * 取:read
  * 更新:update
  * 删除:delete

* 抽象实现*AbstractSessionDAO*
  * 对存取session的时候进行完整性检查
  * 提供 *SessionIdGenerator* 给子类使用
* *MemorySessionDAO*
  * 基于内存的Session存取
  * *ConcurrentMap*

* *CachingSessionDAO*
  * 注入缓存  *CacheManager*
  * *Cache<Serializable, Session> activeSessions;* 
  * 缓存名 *为shiro-activeSessionCache*

*Cookie*

简单的与HttpCookie的交互

* name
* value
* comment
* domain
* maxage
* path
* isSecure
* version
* httpOnly



SecurityManager登录过程

1. 客户端生成自己的 登录信息 :例如 用户名密码

2. 调用 Authenticator.authenticate(AuthenticationToken token)方法

   1. 单Realm:以JDBC为例

      1. 判断该域是否支持 该token认证

      2. 尝试从缓存中取

      3. 从缓存中没有取到 则根据用户名从 数据库查询,生成 *认证后信息AuthenticationInfo*,包含用户密码

         *doGetAuthenticationInfo* 

      4. 尝试缓存该 token对应的 info

         *cacheAuthenticationInfoIfPossible*

      5. 将 token 与 info的密码 做比较

         使用 *CredentialsMatcher* 比较

      6. 

   2. 多Realm

      1. 获取securityManager的 认证策略
      2. strategy.beforeAttempt
      3. 循环多个Realm
      4. beforeAttempt
      5. 上述单Realm的认证流程
      6. afterAttempt
      7. 结束循环
      8. afterAllAttempts

3. 认证结果

   1. 认证成功返回 AuthenticationInfo
      1. 通知 *AuthenticationListener*监听
      2. 根据token,info,当前subject生成 新的subject
      3. 根据 rememberMe的管理器的配置记住登录信息

Subject登录过程

1. 清除runAs信息
2. 调用SecurityManager登录过程
3. 得到登录成功后的Subject
4. 取其中的 principals,认证状态,host
5. 默认不建session



*AnnotationResolver* 根据拦截的方法 获取方法上的注解

```
*Annotation getAnnotation(MethodInvocation mi, Class<? extends Annotation> clazz);*
```



*AnnotationHandler*

注解处理器

1. 注解类
2. *Subject*

*AuthorizingAnnotationHandler*

​		确保已授权

*AuthenticatedAnnotationHandler*:已登录

*GuestAnnotationHandler*:最低需要访客权限

*UserAnnotationHandler*:有用户已登录

*PermissionAnnotationHandler*:需要特定权限

*RoleAnnotationHandler*:指定角色



*MethodInterceptor*

方法拦截

```
Object invoke(MethodInvocation methodInvocation)
Subject getSubject()
```

*AuthorizingMethodInterceptor* :拦截:执行前授权

拦截逻辑: 执行前授权

```java
  assertAuthorized(methodInvocation);
  return methodInvocation.proceed();
```

*AnnotationMethodInterceptor* :注解方法拦截

*AuthorizingAnnotationMethodInterceptor*



注解解析:Reslover

注解处理:Handler

权限注解方法拦截: MethodInterceptor

* 解析器
* 权限处理器



shiroweb总结

* SecurityManager

  * 授权对象管理
  * 会话管理
  * 缓存管理
  * 凭证保存
  * 事件发布
  * 授权认证
    * 认证
    * 授权
    * 凭证域

* 注解处理

  * 注解解析 Resolver
  * 注解处理 Handler
  * 注解方法拦截 MethodInterceptor
    * 登录拦截器 : 登录校验的handler + Resolver
    * 角色拦截器: 角色校验的handler + Resolver
  * *AopAllianceAnnotationsAuthorizingMethodInterceptor* 综合方法

* url过滤

  * *ShiroFilterFactoryBean* 

    * 设置 过滤器的默认 loginUrl,successfulUrl,unauthorizedUrl
    * 设置过滤链定义
      * 通过 INI
      * 手动 指定
    * *FactoryBean* :产生这种类型的工厂
      * *getObject* :spring容器从该工厂方法 中获取过滤器
        * *判断是否是WebSecurityManager*
        * *DefaultFilterChainManager* 创建过滤链条管理器
          * 添加默认可用的过滤器
        * 将默认可用的过滤器统一设置 loginurl,successurl,unauthorizedurl
        * 获取用户设置的过滤器,统一设置 loginurl,successurl,unauthorizedurl
        * 将用户设置的过滤器 添加到 过滤器管理器中,同名会覆盖 默认提供的过滤器
        * 获取用户设置的过滤器链定义,并调用 *DefaultFilterChainManager*.createChain方法,加入到管理器中
        * 新建 基于 路径匹配的链条选择器, PathMatchingFilterChainResolver
        * 实例化 SpringShiroFilter
      * *getObjectType*

  * shiroFilter

    * *AbstractFilter*

      * *ServletContextSupport* 对 ServletContext 中的参数 属性进行管理
      * 实现了filter的初始化流程
        * 设置 FilterConfig
        * 回调 onFilterConfigSet

    * *NameableFilter* :带名称的过滤器:默认取 FilterConfig.getFilterName

    * *OncePerRequestFilter* : 实现了  **每个request只过滤一次 的逻辑**

      * 如果已经过滤了,则在request 中设置标记
      * 如果不存在该标记,说明没有过滤,则判断 该filter是否针对 该request response 需要过滤
      * 需要过滤则 执行真正的过滤方法 a留给子类实现
      * 请求完成后 清除标记

    * *AdviceFilter*:AOP风格的 preHandler,postHandler,afterCompletion

      实现过滤逻辑

      * *preHandle*
      * *chain.doFilter(request, response);*
      * *postHandle*
      * *cleanup* 处理异常,调用afterCompletion

    * *PathMatchingFilter*:基于 Ant风格的URL匹配模式

      * prehandler:判断是否满足 过滤规则
      * onPrehandler:prehandler处理完之后,回调子类

    * *NoSessionCreationFilter*

      * *onPreHandle* 实现
      * 不创建session的filter
      * 在过滤前 DefaultSubjectContext.SESSION_CREATION_ENABLED 设置false

    * *AnonymousFilter* 不过滤

      * *onPreHandle* 实现

      ```
      /user/signup/** = anon
      /user/** = authc
      设置 /user/所有都要认证
      但排除 /user/signup/**
      ```

    * *AccessControlFilter* :资源访问控制的 父类
      * *onPreHandle* 的实现
      * *isAccessAllowed*  || onAccessDenied 判断是否可访问,访问被拒绝该如何处理

    * *AuthenticationFilter*

      * 认证过滤器,实现isAccessAllowed
      * 'isAccessAllowed' : 判断Subject.isAuthenticated
      * 登陆成功默认URL
      * 登录成功后:从session中取之前 保存的URL,并重定向

    * *PassThruAuthenticationFilter*

      * 实现 onAccessDenied
      * 如果登录被拒绝 且请求的是登录页面则通过
      * 如果登录被拒绝 且请求的是登录页面不是login则 将请求的URL保存在 Subject.session中 shiroSavedRequest

    * *AuthenticatingFilter*

      * 执行登录过程
        * 根据req,resp 生成token(子类实现)
        * 根据req,resp SecurityManager 得到subject
        * 调用登录方法
        * *onLoginSuccess*回调
        * 或者*onLoginFailure*回调
      * 默认 增加了 username,passwordtoken 的方法
      * *isAccessAllowed* 覆盖 : 调用父类的 || 子类判断 是否有 PERMISSIVE字符串

    * *FormAuthenticationFilter*

      * 实现 onAccessDenied 拒绝逻辑
        * 判断是否是 loginURL, 是否用 post提交
        * 执行登录逻辑
        * 使用usernamepassword token 
          * 取默认参数名,username,password,rememberMe
        * 如果不是登录URL,则保存该URL到session中
        * 如果是登录URL,但不是post方法 则允许访问

    * BasicHttpAuthenticationFilter

      * 使用基于HTTP的认证协议,步骤如下
        1. 客户端请求到来时,相应401,状态,并设置 WWW-Authenticate 头,页面的内容是 通知用户需要认证
        2. 客户端会以 username:password 形式 的 base64 编码格式返回
        3. 最终请求头是这样: Authorization: Basic Base64_encoded_username_and_password

      * 实现isAccessAllowed

        * ```
          /basic/** = authcBasic[POST,PUT,DELETE]
          ```

        * 该请求方法在 配置的authcBasic中

        * 调用父类的

      * *onAccessDenied*

        * 判断是否是 登录请求,如果是执行登录

        * 如果登录失败,发送

          * 响应码 401

          * ```
            WWW-Authenticate=BASIC realm="application"
            ```

        * *createToken*

          * 从 req中 取 Authorization 头字段
          * 逗号分隔取 用户名密码 生成用户名密码token

    * *AuthorizationFilter* 授权过滤器

      * *unauthorizedUrl* 未授权的url
      * 实现onAccessDenied
        * 如果没有登录则保存该URL.重定向到登录
        * 如果没有权限 则重定向到unauthorizedUrl,303
        * 如果没有设置 unauthorizedUrl则 为401

    * *PermissionsAuthorizationFilter*

      * 权限授权过滤器
      * *isAccessAllowed*
        * 调用subject的授权方法

    * HttpMethodPermissionFilter

      * 支持rest风格,将 put,get,post,delete转换成对应的权限

      * | HTTP Method | Mapped Action | Example Permission | Runtime Check |
        | ----------- | ------------- | ------------------ | ------------- |
        | get         | read          | perm               | perm:read     |
        | put         | update        | perm               | perm:update   |
        | post        | cretae        | perm               | perm:create   |
        |             |               |                    |               |

    * *RolesAuthorizationFilter*

      * *isAccessAllowed* 校验subjectRoles

    * *HostFilter*

      * 根据主机名 过滤

    * *PortFilter*

      * 根据端口过滤
      * 如果端口不匹配则 重定向到该端口的该URL

    * *SslFilter*

      * 443端口, 协议:https

    * *UserFilter*

      * 只要有用户在 Subject里则 放行,

    * *AbstractShiroFilter*
      * 执行过滤逻辑doFilterInternal
        * 包装req,resp
        * 创建WebSubject
        * 在subject 上下中执行 更新session
        * 执行调用链
          * 根据req,resp获取调用链
          * 如果链条解析器没有则 返回原始链条
          * 如果解析到了,则返回解析的链条

  * *PathMatchingFilterChainResolver*

  * *FilterChainManager*d

    * *Map<String, Filter> filters* 过滤器管理

      ```
      [filters]
         port.port = 80
         
      
      [urls]
      /some/path/** = port
      # override for just this path:
      /another/path/** = port[8080]
      ```

    * *createChain*

      * *chainName*

      * *chainDefinition*

        * string格式,*被FilterChainResolver* 解析
        * `filter1[optional_config1], filter2[optional_config2], ..., filterN[optional_configN]`
        * 过滤器书写的顺序 即 链条调用顺序

      * example

        ```
        /remoting/** = authcBasic, roles[b2bClient], perms["remote:invoke:wan,lan"]
        
        定义了一条过滤器链,针对  /remoting/**的所有请求
        1. 首先需要 authcBasic认证
        2. 然后需要有 b2bClient角色
        3. 最后需要remote:invoke:wan,lan的权限
        ```

    * *filters*
      * 默认可用的过滤器
    * *filterChains*
      * 过滤器链条

# 总结

* 不同功能交给不同功能的类
  * 对象创建交给 Factory
  * 存取 交给 Dao层,在存取过程中可以设置缓存
  * 缓存交给 缓存管理器
  * 对象注入交给 Aware接口

  

* 使用接口标识 类的功能特性

  * 要向容器注册某个扩展类时, 实现某个注册接口,spring会自动扫描自动注册这个接口到扩展点,不用手动去注册
