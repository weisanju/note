# mybatis多数据源配置

## 继承数据源路由类

*AbstractRoutingDataSource*

```java
    @Override
    protected Object determineCurrentLookupKey() {
        return DataSourceContextHolder.getDatasourceType();
    }
数据源的路由策略是:同一线程内的 数据源一致

package com.weisanju.hikaricp.datasource;

public class DataSourceContextHolder {
    public static final ThreadLocal<String> contextHolder=new ThreadLocal<>();

    public static void setDataSource(String type){
        contextHolder.set(type);
    }

    public static String getDatasourceType(){
        return contextHolder.get();
    }

    public static void clearDataSourceType(){
        contextHolder.remove();
    }
}

```

## 使用自定义注解+AOP切换数据源

```java
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.METHOD,ElementType.TYPE})
public @interface DataSourceSwitch {
    String value()default "test";
}
```

```java
@Aspect
@Component
public class DataSourceAspect {
    @Before("@annotation(ds)")
    public void beforeDataSource(DataSourceSwitch ds) {
        DataSourceContextHolder.setDataSource(ds.value());
    }
    @After("@annotation(ds)")
    public void afterDataSource(DataSourceSwitch ds){
        DataSourceContextHolder.clearDataSourceType();
    }
}
```



## 使用配置类配置mybatis

* 配置 原始 数据源

* 配置 动态数据源(代理类)

* 配置*sqlsessionFactory*

  需要指定的东西

  * 动态数据源
  * xml文件(原本在配置文件种指定的)

* 配置事务管理器

  ```java
  @Configuration
  @MapperScan(basePackages="com.weisanju.hikaricp.mapper")
  public class MybatisConfig {
  
      @Bean("ase")
      @ConfigurationProperties(prefix = "spring.datasource.ase")
      public DataSource ase(){
          return DataSourceBuilder.create().build();
      }
      @Bean("test")
      @ConfigurationProperties(prefix = "spring.datasource.test")
      public DataSource test(){
          return DataSourceBuilder.create().build();
      }
  
      @Bean
      public DynamicDataSource dataSource(@Qualifier("ase") DataSource ase,
                                          @Qualifier("test") DataSource test) {
          DynamicDataSource.datasouceMap.put(DataSourceType.ASE.getDbname(), ase);
          DynamicDataSource.datasouceMap.put(DataSourceType.TEST.getDbname(), test);
          DynamicDataSource.instance.setTargetDataSources(DynamicDataSource.datasouceMap);
          DynamicDataSource.instance.setDefaultTargetDataSource(test);
          return DynamicDataSource.instance;
      }
      @Bean
      public SqlSessionFactory sqlSessionFactory(DynamicDataSource dynamicDataSource) throws Exception {
          SqlSessionFactoryBean factoryBean = new SqlSessionFactoryBean();
          factoryBean.setDataSource(dynamicDataSource);
  //        factoryBean.setTypeAliasesPackage();
          // 设置mapper.xml的位置路径
          Resource[] resources = new PathMatchingResourcePatternResolver().getResources("classpath:xml/*.xml");
          factoryBean.setMapperLocations(resources);
          return factoryBean.getObject();
      }
  
      @Bean
      public PlatformTransactionManager transactionManager(DynamicDataSource dynamicDataSource){
          return new DataSourceTransactionManager(dynamicDataSource);
      }
  }
  ```

## 从数据库实例化数据源

```java
public class DynamicDataSource extends AbstractRoutingDataSource {
    public static final DynamicDataSource instance=new DynamicDataSource();
    public static final Map<Object,Object> datasouceMap = new HashMap<>();
    @Override
    public void setTargetDataSources(Map<Object, Object> targetDataSources) {
        super.setTargetDataSources(targetDataSources);
        super.afterPropertiesSet();
    }

    @Override
    protected Object determineCurrentLookupKey() {
        return DataSourceContextHolder.getDatasourceType();
    }

}
    要调用 afterPropertiesSet才会对 数据源的datasource Map生效
```

从数据库查数据源

```java
{
        DataSourceExample dataSourceExample = new DataSourceExample();
        String db = "ase";
        if(DynamicDataSource.datasouceMap.get(db) ==null){
            dataSourceExample.createCriteria().andDsnameEqualTo(db);
            List<DataSource> dataSources = dataSourceMapper.selectByExample(dataSourceExample);
            DataSource dataSource = dataSources.get(0);
            HikariDataSource hikariDataSource = new HikariDataSource();

            hikariDataSource.setJdbcUrl(dataSource.getUrl());
            hikariDataSource.setUsername(dataSource.getUsername());
            hikariDataSource.setPassword(dataSource.getPassword());
            hikariDataSource.setDriverClassName(dataSource.getDrivername());

            DynamicDataSource.datasouceMap.put(db,hikariDataSource);
            DynamicDataSource.instance.setTargetDataSources(DynamicDataSource.datasouceMap);
        }
        DataSourceContextHolder.setDataSource(db);
        //更新
        ZunshiRecord zunshiRecord = new ZunshiRecord();
        zunshiRecord.setId(1L);
        zunshiRecord.setCompanyCode("666");
        mapper.updateByPrimaryKeySelective(zunshiRecord);
    }
```



