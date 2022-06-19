# resultHandler使用说明

1. 返回值为Null

2. 实现ResultHandler接口

3. 定义Mapper时传入ResultHandler

   ```
     void selectByPrimaryKey(Integer id, ResultHandler<User> resultHandler);
   ```



# DefaultResultHandler

```java
public class DefaultResultHandler implements ResultHandler<Object> {

  /**
   * 集合
   */
  private final List<Object> list;

  public DefaultResultHandler() {
    list = new ArrayList<>();
  }

  @SuppressWarnings("unchecked")
  public DefaultResultHandler(ObjectFactory objectFactory) {
    list = objectFactory.create(List.class);
  }

  @Override
  public void handleResult(ResultContext<?> context) {
    list.add(context.getResultObject());
  }

  public List<Object> getResultList() {
    return list;
  }

}

```



# DefaultMapResultHandler

```java
/**
 *    Copyright 2009-2015 the original author or authors.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
package org.apache.ibatis.executor.result;

import java.util.Map;

import org.apache.ibatis.reflection.MetaObject;
import org.apache.ibatis.reflection.ReflectorFactory;
import org.apache.ibatis.reflection.factory.ObjectFactory;
import org.apache.ibatis.reflection.wrapper.ObjectWrapperFactory;
import org.apache.ibatis.session.ResultContext;
import org.apache.ibatis.session.ResultHandler;

/**
 * 默认Map的ResultHandler
 * @author Clinton Begin
 */
public class DefaultMapResultHandler<K, V> implements ResultHandler<V> {

  /**
   * 映射结果
   */
  private final Map<K, V> mappedResults;
  /**
   * 映射Key
   * <p>
   *   指定一个字段作为返回Map中的key，这里一般也就是使用唯一键来做key.
   * </p>
   * <p>
   *     参考博客：https://blog.csdn.net/u012734441/article/details/85861337
   * </p>
   */
  private final String mapKey;
  /**
   * 对象工厂
   */
  private final ObjectFactory objectFactory;
  /**
   * 对象包装工厂
   */
  private final ObjectWrapperFactory objectWrapperFactory;
  /**
   * 反射工厂
   */
  private final ReflectorFactory reflectorFactory;

  /**
   *
   * @param mapKey 指定一个字段作为返回Map中的key，这里一般也就是使用唯一键来做key.
   *               参考博客：https://blog.csdn.net/u012734441/article/details/85861337
   * @param objectFactory 对象工厂
   * @param objectWrapperFactory 对象包装工厂
   * @param reflectorFactory 反射工厂
   */
  @SuppressWarnings("unchecked")
  public DefaultMapResultHandler(String mapKey, ObjectFactory objectFactory, ObjectWrapperFactory objectWrapperFactory, ReflectorFactory reflectorFactory) {
    this.objectFactory = objectFactory;
    this.objectWrapperFactory = objectWrapperFactory;
    this.reflectorFactory = reflectorFactory;
    this.mappedResults = objectFactory.create(Map.class);
    this.mapKey = mapKey;
  }

  @Override
  public void handleResult(ResultContext<? extends V> context) {
    //获取结果对象
    final V value = context.getResultObject();
    //value元对象
    final MetaObject mo = MetaObject.forObject(value, objectFactory, objectWrapperFactory, reflectorFactory);
    // TODO is that assignment always true?
    //获取mapKey的key
    final K key = (K) mo.getValue(mapKey);
    mappedResults.put(key, value);
  }

  public Map<K, V> getMappedResults() {
    return mappedResults;
  }
}
```