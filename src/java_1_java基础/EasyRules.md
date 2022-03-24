# 上手

## **依赖**

```xml
<dependency>
    <groupId>org.jeasy</groupId>
    <artifactId>easy-rules-core</artifactId>
    <version>4.0.0</version>
</dependency>
```

## **定义规则**

### 注解定义

```java
@Rule(name = "weather rule", description = "if it rains then take an umbrella")
public class WeatherRule {

    @Condition
    public boolean itRains(@Fact("rain") boolean rain) {
        return rain;
    }
    
    @Action
    public void takeAnUmbrella() {
        System.out.println("It rains, take an umbrella!");
    }
}
```

### Fluent API定义

```java
Rule weatherRule = new RuleBuilder()
        .name("weather rule")
        .description("if it rains then take an umbrella")
        .when(facts -> facts.get("rain").equals(true))
        .then(facts -> System.out.println("It rains, take an umbrella!"))
        .build();
```



### 表达式语言

```java
Rule weatherRule = new MVELRule()
        .name("weather rule")
        .description("if it rains then take an umbrella")
        .when("rain == true")
        .then("System.out.println(\"It rains, take an umbrella!\");");
```



### YAML的规则描述器

```yml
name: "weather rule"
description: "if it rains then take an umbrella"
condition: "rain == true"
actions:
  - "System.out.println(\"It rains, take an umbrella!\");"
```

```java
MVELRuleFactory ruleFactory = new MVELRuleFactory(new YamlRuleDefinitionReader());
Rule weatherRule = ruleFactory.createRule(new FileReader("weather-rule.yml"));
```

## 使用

```java
  // 定义事实
        Facts facts = new Facts();
        facts.put("rain", true);

        // 定义规则
        Rule weatherRule = ...
        Rules rules = new Rules();
        rules.register(weatherRule);

        // 使用规则引擎执行
        RulesEngine rulesEngine = new DefaultRulesEngine();
        rulesEngine.fire(rules, facts);
```



# 对规则的抽象

## 规则的定义

包括：名字、描述、优先级、事实集合、条件集合和行动集合

## 规则的比较

*UnitRuleGroup*

要么应用所有规则,要么都不应用, 规则组里的规则 是一个整体

*ActivationRuleGroup*

第一个满足条件的会被触发,其他的会被忽略 ,规则首先会被以优先级排序

ConditionalRuleGroup

以优先级排序,当前规则返回true 则触发余下规则

## 规则引擎

**DefaultRulesEngine**

（以规则的自然顺序执行）和

**InferenceRulesEngine**

（一直执行直到没有可用的规则），且接受包括优先级阈值等的参数。