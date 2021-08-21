# 描述

ApacheShiro 是*java*的一个安全框架,提供认证,授权,加密,会话管理的解决方案

 

# 功能特点

## *primary* *concerns*

*authentication*:认证

*authorization*:授权

*sessionManager*:会话管理

*cryptography* :加密

## *supporting* *features*

*websupport* *web*:集成到*web*环境

*caching*:缓存用户信息

*concurrency*:并发验证,在多线程应用中 ,在一个线程中开启另一个线程,能把权限自动传播过去

*testing* :提供测试支持

*run as* : 允许用户假装为另外一个用户 去访问

*remember* *me* :记住



# 抽象概念

## *subject* 

* 访问资源对象的主体
* 通过 `Subject currentUser = SecurityUtils.getSubject();`

## *SecurityManager*

* *shiro*核心,总控,负责管理所有*subject*,进行认证授权,及会话缓存的管理

* 实例化

  * web程序通常通过  *Shiro Servlet Filter* 来实例化
  * 通常每个应用只有一个 *manager*
  * 可以通过 ini配置文件,*spring XML*去实例化它

* 配置示例

  ```
  [main]
  cm = org.apache.shiro.authc.credential.HashedCredentialsMatcher
  cm.hashAlgorithm = SHA-512
  cm.hashIterations = 1024
  # Base64 encoding (less text):
  cm.storedCredentialsHexEncoded = false
  [users]
  jdoe = TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJpcyByZWFzb2
  asmith = IHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbXNoZWQsIG5vdCB
  ```

* 更多*INI*配置方法 详见 http://shiro.apache.org/documentation.html

  

* *API*调用

  ```java
  Factory<SecurityManager> factory = new IniSecurityManagerFactory("classpath:shiro.ini");
  SecurityManager manager = factory.getInstance();
  SecurityUtils.setSecurityManager(manager);
  ```

## *Realm*

* 有一个或多个*realm* 可以认为是安全实体数据源,用于获取安全实体的对象

* Realm配置 LDAP

  ```
  [main]
  ldapRealm = org.apache.shiro.realm.ldap.JndiLdapRealm
  ldapRealm.userDnTemplate = uid={0},ou=users,dc=mycompany,dc=com
  ldapRealm.contextFactory.url = ldap://ldapHost:389
  ldapRealm.contextFactory.authenticationMechanism = DIGEST-MD5
  ```

  

## *Authenticator* 

* 默认实现的认证器

* 可以自定义实现,指定*AuthenticationStrategy*

* 认证有三步

  * 收集用户信息,(*principals*) 和认证信息(*credentials*)
  * 提交 *principals* 和 *credentials*
  * 返回结果

* *API*

  ```
  AuthenticationToken token = new UsernamePasswordToken(username, password);
  Subject currentUser = SecurityUtils.getSubject();
  currentUser.login(token);
  ```

* 当调用 登录方法时,*securityManager* 会将 *token* 发送各个 *realm* 去验证,验证失败后可以 捕获下面异常

  ```
  //3. Login:
  try {
      currentUser.login(token);
  } catch (IncorrectCredentialsException ice) { …
  } catch (LockedAccountException lae) { …
  }
  …
  catch (AuthenticationException ae) {…
  } 
  ```

  

## *authrizer*

* 授权器

* *API*

  * *roleCheck*

    ```java
    if ( subject.hasRole(“administrator”) ) {
        //show the ‘Create User’ button
    } else {
        //grey-out the button?
    }
    ```

  * *Permission Check*

    ```java
    if ( subject.isPermitted(“user:create”) ) {
        //show the ‘Create User’ button
    } else {
        //grey-out the button?
    } 
    ```

    

  * *Instance-Level Permission Check* 实例级别的访问控制

    ```java
    if ( subject.isPermitted(“user:delete:jsmith”) ) {
        //delete the ‘jsmith’ user
    } else {
        //don’t delete ‘jsmith’
    }
    ```

  * [访问控制文档](http://shiro.apache.org/permissions.html)

    

## *SessionManager*

* 管理*session*的生命周期

* 提供 独立于容器的 分布式*session*解决方案

* *API*

  ```java
  Session session = subject.getSession();
  Session session = subject.getSession(boolean create);
  session.getAttribute(“key”, someValue);
  Date start = session.getStartTimestamp();
  Date timestamp = session.getLastAccessTime();
  session.setTimeout(millis);
  ```

## *Cryptography*

* 密码模块

## *sessionDao*

* 控制session的存放位置

## *cacheManager*

* 缓存管理器,缓存用户,角色,权限等

# Shiro过滤器

当 Shiro 被运用到 web 项目时，Shiro 会自动创建一些默认的过滤器对客户端请求进行过滤

| 过滤器简称        | 对应的 Java 类(父包头org.apache.shiro.web.filter) | 示例                           | 解析                                                         |
| :---------------- | :------------------------------------------------ | ------------------------------ | ------------------------------------------------------------ |
| anon              | authc.AnonymousFilter                             | */admins/**=anon*              | 表示该 uri 可以匿名访问                                      |
| authc             | authc.FormAuthenticationFilter                    | */admins/**=auth*              | 表示该 uri 需要认证才能访问                                  |
| authcBasic        | authc.BasicHttpAuthenticationFilter               | */admins/**=authcBasic*        | 表示该 uri 需要 httpBasic 认证                               |
| perms             | authz.PermissionsAuthorizationFilter              | */admins/**=perms[user:add:*]* | 需要认证用户拥有 user:add:* 权限才能访问                     |
| port              | authz.PortFilter                                  | */admins/**=port[8081]*        | 表示该 uri 需要使用 8081 端口                                |
| rest              | authz.HttpMethodPermissionFilter                  | */admins/**=rest[user]*        | 相当于 /admins/**=perms[user:method]，其中，method 表示  get、post、delete 等 |
| roles             | authz.RolesAuthorizationFilter                    | */admins/**=roles[admin]*      | 表示该 uri 需要认证用户拥有 admin 角色才能访问               |
| ssl               | authz.SslFilter                                   | */admins/**=ssl*               | 表示该 uri 需要使用 https 协议                               |
| user              | authc.UserFilter                                  | */admins/**=user*              | 表示该 uri 需要认证或通过记住我认证才能访问                  |
| logout            | authc.LogoutFilter                                | */logout=logout*               | 表示注销,可以当作固定配置                                    |
| noSessionCreation | session.NoSessionCreationFilter                   |                                |                                                              |



# 快速入门

## 添加依赖

```xml
<dependency>
    <groupId>commons-logging</groupId>
    <artifactId>commons-logging</artifactId>
    <version>1.1.3</version>
</dependency>

<dependency>
    <groupId>org.apache.shiro</groupId>
    <artifactId>shiro-core</artifactId>
    <version>1.4.0</version>
</dependency>
```

## 配置文件

```
[users]
# admin=admin 分别表示账号和密码，administrator 表示逗号前边的账号拥有 administrator 这个角色。
admin=admin,administrator
zhangsan=zhangsan,manager
lisi=lisi,guest

[roles]
# administrator 表示角色名称，* 表示这个角色拥有所有权限
administrator=*
manager=user:*,department:*
guest=user:query,department:query
其中，每个用户可以拥有多个角色，通过逗号分隔。每个角色可以拥有多个权限，同样通过逗号分隔。
```

## web支持

**ShiroFilter in web.xml**

```xml
<filter>
    <filter-name>ShiroFilter</filter-name>
    <filter-class>
        org.apache.shiro.web.servlet.IniShiroFilter
    </filter-class>
    <!-- no init-param means load the INI config
        from classpath:shiro.ini --> 
</filter>

<filter-mapping>
     <filter-name>ShiroFilter</filter-name>
     <url-pattern>/*</url-pattern>
</filter-mapping>
过滤器将会过滤每个 请求,只有满足
```

## *INI* *URL*过滤器指定

* 右边时过滤器的名字, 有序的逗号分隔, *(anon, user, perms, authc shiro提供的内置过滤器)* 可以自定义

## Web Session Management



# 身份认证

## 配置在*INI*文件里的 用户密码认证

```
        Factory<SecurityManager> factory = new IniSecurityManagerFactory("classpath:shiro.ini");
        SecurityManager manager = factory.getInstance();
        SecurityUtils.setSecurityManager(manager);
        Subject subject = SecurityUtils.getSubject();
        UsernamePasswordToken token = new UsernamePasswordToken("lisi", "lisi");
subject.login(token);
Assert.assertEquals(true, subject.isAuthenticated()); //断言用户已经登录
        
INI
        [users]
# admin=admin 分别表示账号和密码，administrator 表示逗号前边的账号拥有 administrator 这个角色。
admin=admin,administrator
zhangsan=zhangsan,manager
lisi=lisi,guest
```

自定义 Realm 实现

* 实现 *realm* 接口

```
import org.apache.shiro.authc.*;
import org.apache.shiro.realm.Realm;

public class MyRealm1 implements Realm {
    @Override
    public String getName() {
        return "myrealm1";
    }
    @Override
    public boolean supports(AuthenticationToken token) {
        //仅支持UsernamePasswordToken类型的Token
        return token instanceof UsernamePasswordToken;
    }
    @Override
    public AuthenticationInfo getAuthenticationInfo(AuthenticationToken token) throws AuthenticationException {
        String username = (String)token.getPrincipal();  //得到用户名
        String password = new String((char[])token.getCredentials()); //得到密码
        if(!"zhang".equals(username)) {
            throw new UnknownAccountException(); //如果用户名错误
        }
        if(!"123".equals(password)) {
            throw new IncorrectCredentialsException(); //如果密码错误
        }
        //如果身份认证验证成功，返回一个AuthenticationInfo实现；
        return new SimpleAuthenticationInfo(username, password, getName());
    }
}

实现Realm 接口
三个方法
得到域名
是否支持该 类型的token验证
验证:验证失败,抛异常,验证成功返回 AutenticationInfo
```

* 配置该*realm*

  ```
  [main]
  myRealm1=MyRealm1
  myRealm2=MyRealm2
  securityManager.realms=$myRealm1,$myRealm2
  [users]
  # admin=admin 分别表示账号和密码，administrator 表示逗号前边的账号拥有 administrator 这个角色。
  admin=admin,administrator
  zhangsan=zhangsan,manager
  lisi=lisi,guest
  ```

* relam的继承图
![realm继承图](https://i.loli.net/2020/06/02/8ivPcbkfhOa7ydE.png)

* 解析

  * **org.apache.shiro.realm.text.IniRealm**

    主要加载的时 [users] 部分指定用户名 / 密码及其角色；[roles] 部分指定角色即权限信息；

  * **org.apache.shiro.realm.text.PropertiesRealm**

    * user.username=password,role1,role2 指定用户名 / 密码及其角色；
    * role.role1=permission1,permission2 指定角色及权限信息；

  * **org.apache.shiro.realm.jdbc.JdbcRealm**

    * 通过 sql 查询相应的信息

    * select password from users where username = ?” 获取用户密码

    * “select password, password_salt from users where username = ?” 获取用户密码及盐

    * “select role_name from user_roles where username = ?” 获取用户角色

    * “select permission from roles_permissions where role_name = ?” 获取角色对应的权限信息；
  
    * 也可以调用相应的 api 进行自定义 sql；

    * jdbcRealm 数据库

      ```xml
          <dependency>
                  <groupId>mysql</groupId>
                  <artifactId>mysql-connector-java</artifactId>
                  <version>5.1.25</version>
              </dependency>
              <dependency>
                  <groupId>com.alibaba</groupId>
                  <artifactId>druid</artifactId>
                  <version>0.2.23</version>
              </dependency>
      ```

## Authenticator 及 AuthenticationStrategy

Authenticator 的职责是验证用户帐号

接口方法

```
public AuthenticationInfo authenticate(AuthenticationToken authenticationToken)            throws AuthenticationException
```

* 有一个 *ModularRealmAuthenticator*  实现,其委托给多个 *Relam*进行验证 

* 验证规则通过 *AuthenticationStrategy* 接口指定

  * *FirstSuccessfulStrategy* 

    只要有一个*realm* 验证成功即可,只返回第一个realm身份验证成功的认证信息,其他的忽略

  * *AtLeastOneSuccessfulStrategy* 

    只要有一个 Realm 验证成功即可,返回所有 Realm 身份验证成功的认证信息

  * *AllSuccessfulStrategy*

    所有 Realm 验证成功才算成功,且返回所有 Realm 身份验证成功的认证信息

  * *ModularRealmAuthenticator* 默认使用 *AtLeastOneSuccessfulStrategy* 策略
  * 返回时 若 *principal* 身份信息发生变化,则 可以通过  *subject.getPrincipals* 来获取

* 自定义 AuthenticationStrategy 实现

  * *beforeAllAttempts*
  * *beforeAttempt*
  * *afterAttempt*
  * *afterAllAttempts*

  * 因为每个 *AuthenticationStrategy* 实例都是无状态的，所有每次都通过接口将相应的认证信息(*Collection<? extends Realm>*)传入下一次流程；通过如上接口可以进行如合并 / 返回第一个验证成功的认证信息

  ```
  authenticator=org.apache.shiro.authc.pam.ModularRealmAuthenticator
  securityManager.authenticator=$authenticator
  \#指定securityManager.authenticator的authenticationStrategy
  allSuccessfulStrategy=org.apache.shiro.authc.pam.AllSuccessfulStrategy
  securityManager.authenticator.authenticationStrategy=$allSuccessfulStrategy
  ```
  
  

## 授权(access controll)

Shiro 支持三种方式 权限检查

* *checkRole/checkRoles 和 hasRole/hasAllRoles* , *isPermitted 和 isPermittedAll* 用于判断用户是否拥有某个权限或所有权限

* *@RequiresRoles("admin")*

* jsp标签:

  ```xml
  <shiro:hasRole name="admin">
  <!— 有权限 —>
  </shiro:hasRole>
  ```

角色,权限,资源,操作

用户名=密码,角色1，角色2  

角色=权限 1，权限 2权限1=资源:操作

*permission*

字符串通配符权限

资源规则: 资源标识符:操作:对象实例ID,即 对哪个资源的哪个实例可以进行什么操作

默认支持通配符

冒号表示资源/操作/实例的分割

逗号表示操作的分割

星号表示任意资源/操作/实例

单个资源单个权限

*subject().checkPermissions("system:user:update");*

单个资源多个权限

*role41=system:user:update,system:user:delete*

*subject().checkPermissions("system:user:update", "system:user:delete");*

简写

*role42="system:user:update,delete"*

*subject().checkPermissions("system:user:update,delete");*

授权流程

调用 `Subject.isPermitted*/hasRole*`接口 

->委托给 SecurityManager

->SecurityManager 接着会委托给 Authorizer；

->Authorizer 是真正的授权者，如果我们调用如 isPermitted(“user:view”),会通过 PermissionResolver 把字符串转换成相应的 Permission 实例

->在进行授权之前，其会调用相应的 Realm 获取 Subject 相应的角色/权限

->Authorizer 会判断 Realm 的角色/权限是否和传入的匹配，如果有多个 Realm，会委托给 ModularRealmAuthorizer 进行循环判断

->ModularRealmAuthorizer 进行多 Realm 匹配流程

​	->首先检查相应的 Realm 是否实现了实现了 Authorizer

​	->如果实现了 Authorizer，那么接着调用其相应的 `isPermitted*/hasRole*` 接口进行匹配

​	->如果有一个 Realm 匹配那么将返回 true，否则返回 false

->如果 Realm 进行授权的话，应该继承 AuthorizingRealm，其流程是


