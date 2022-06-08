

### 扫描类路径

```java
ClassPathScanningCandidateComponentProvider provider = new ClassPathScanningCandidateComponentProvider(false);
provider.addIncludeFilter(new AssignableTypeFilter(MyClass.class));

// scan in org.example.package
Set<BeanDefinition> components = provider.findCandidateComponents("org/example/package");
for (BeanDefinition component : components)
{
    Class cls = Class.forName(component.getBeanClassName());
    // use class cls found
}
```

