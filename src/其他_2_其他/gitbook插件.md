# sectionx插件

## 引入

```json
{
    "plugins": ["sectionx"],
    "pluginsConfig": {
        "sectionx": {
          "tag": "b"       // 设置标题的标签，可选值：h1, h2, h3, h4, h5, h6, b,标签的 tag 最好是使用 b 标签，如果使用 h1-h6 可能会和其他插件冲突。
        }
     }
}
```

## 使用

**基本使用**

```
<!--sec data-title="这里写标题" data-id="section0" data-show=true ces-->
这里是内容   
dsadsa    
dadsa
<!--endsec-->
```

**按钮使用**

```
<button class="section" target="showCode" show="显示文案" hide="隐藏文案"></button>
<!--sec data-title="这里写标题" data-id="showCode" data-show=true ces-->
这里是内容   
dsadsa     
dadsa
<!--endsec-->
```

**参数解析**

class：类名必须是`section`
target：需要隐藏的模块名，名字与`data-id`一致
show：模块隐藏时，按钮显示的文案
hide：模块显示时，**按钮显示的文案**



# CSV插件

```json
{
    "plugins": ["include-csv"]
}
```

**使用**

```json
{% includeCsv  src="./assets/csv/test.csv", useHeader="true" %} {% endincludeCsv %}
```



# 模板渲染冲突解决

```
{% raw %}

{% endraw %}
```





# GitBook命令行维护

## 版本维护

```
gitbook  uninstall version_tag
gitbook fetch version_tag
```

