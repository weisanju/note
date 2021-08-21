# 前言

为了分布式session共享，通常的做法是将session存储在redis中，实现多个节点获取同一个session。此实现可以实现session共享，但session的特点是内存存储，就是为了高速频繁访问，每个请求都必须验证session是否存在是否过期，也从session中获取数据，这样导致一个页面刷新过程中的数十个请求会同时访问redis,在几毫秒内同时操作session的获取，修改，更新，保存，删除等操作，从而造成redis的并发量飙升，刷新一个页面操作redis几十到几百次。



为了解决由于session共享造成的redis高并发问题，很明显需要在redis之前做一次短暂的session缓存，如果该缓存存在就不用从redis中获取，从而减少同时访问redis的次数。如果做session缓存，主要有两种种方案，其实原理都相同：



**重写sessionManager的retrieveSession方法**

首先从request中获取session,如果request中不存在再走原来的从redis中获取。这样可以让一个请求的多次访问redis问题得到解决，因为request的生命周期为浏览器发送一个请求到接收服务器的一次响应完成

因此，在一次请求中，request中的session是一直存在的，并且不用担心session超时过期等的问题。这样就可以达到有多少次请求就几乎有多少次访问redis,大大减少单次请求，频繁访问redis的问题。大大减少redis的并发数量

```java

import java.io.Serializable;

import javax.servlet.ServletRequest;

import org.apache.shiro.session.Session;
import org.apache.shiro.session.UnknownSessionException;
import org.apache.shiro.session.mgt.SessionKey;
import org.apache.shiro.web.session.mgt.DefaultWebSessionManager;
import org.apache.shiro.web.session.mgt.WebSessionKey;

public class ShiroSessionManager extends DefaultWebSessionManager {
     /**
     * 获取session
     * 优化单次请求需要多次访问redis的问题
     * @param sessionKey
     * @return
     * @throws UnknownSessionException
     */
    @Override
    protected Session retrieveSession(SessionKey sessionKey) throws UnknownSessionException {
        Serializable sessionId = getSessionId(sessionKey);

        ServletRequest request = null;
        if (sessionKey instanceof WebSessionKey) {
            request = ((WebSessionKey) sessionKey).getServletRequest();
        }

        if (request != null && null != sessionId) {
            Object sessionObj = request.getAttribute(sessionId.toString());
            if (sessionObj != null) {
                return (Session) sessionObj;
            }
        }

        Session session = super.retrieveSession(sessionKey);
        if (request != null && null != sessionId) {
            request.setAttribute(sessionId.toString(), session);
        }
        return session;
    }
}
```



**session缓存于本地内存中**

自定义cacheRedisSessionDao,该sessionDao中一方面注入cacheManager用于session缓存，另一方面注入redisManager用于session存储

当readSession先用cacheManager从cache中读取，如果不存在再用redisManager从redis中读取



**注意：该方法最大的特点是session缓存的存活时间必须小于redis中session的存活时间，**

**就是当redus的session死亡，cahe中的session一定死亡,为了保证这一特点，cache中的session的存活时间应该设置为s级，设置为1s比较合适，并且存活时间固定不能刷新，不能随着访问而延长存活。**





