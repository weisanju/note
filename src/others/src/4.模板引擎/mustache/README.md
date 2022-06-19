## NAME

`mustache` - Logic-less templates.

## SYNOPSIS

A typical Mustache template:

```
Hello {{name}}
You have just won {{value}} dollars!
{{#in_ca}}
Well, {{taxed_value}} dollars, after taxes.
{{/in_ca}}
```

Given the following hash:

```json
{
  "name": "Chris",
  "value": 10000,
  "taxed_value": 10000 - (10000 * 0.4),
  "in_ca": true
}
```

Will produce the following:

```
Hello Chris
You have just won 10000 dollars!
Well, 6000.0 dollars, after taxes.
```

## DESCRIPTION

Mustache can be used for HTML, config files, source code - anything. It works by expanding tags in a template using values provided in a hash or object.

We call it "logic-less" because there are no if statements, else clauses, or for loops. Instead there are only tags. Some tags are replaced with a value, some nothing, and others a series of values. This document explains the different types of Mustache tags.

*Mustache*可以用于HTML，配置文件，源代码。它通过使用哈希或对象中提供的值扩展模板中的标记来工作。
我们称其为 “无逻辑”，因为没有if语句，else子句或for循环。相反，只有标签。有些标签被替换为一个值，有些什么都没有，而另一些则是一系列值。本文档解释了不同类型的*Mustache*标签。



## TAG TYPES



标签由 *double-mustaches* 表示。{{person}} 是一个标签，和 {{# person}} 一样。在这两个示例中，我们都将person称为键或标签键。让我们谈谈不同类型的标签。



### Variables

最基本的标签类型是变量。一个基本模板中的` {{name}}` 标签会尝试在当前上下文中找到name键。如果没有名称键，则将递归检查父上下文。如果到达顶部上下文并且仍然找不到名称键，则不会呈现任何内容。

默认情况下，所有变量都是HTML转义的。如果要返回未转义的HTML，请使用三重胡子 :`{{{ name }}}`。

可以使用 &  反转义 一个变量 `{{& name}}`。这在更改分隔符时可能很有用 (请参阅下面的 “设置分隔符”)。

默认情况下，变量 *miss* 返回一个空字符串。这通常可以在你的小胡子库中配置。例如，Ruby版本的Mustache支持在这种情况下引发异常。

Template:

```
* {{name}}
* {{age}}
* {{company}}
* {{{company}}}
```

Hash:

```
{
  "name": "Chris",
  "company": "<b>GitHub</b>"
}
```

Output:

```
* Chris
*
* &lt;b&gt;GitHub&lt;/b&gt;
* <b>GitHub</b>
```

### Sections

Sections render blocks of text one or more times, depending on the value of the key in the current context.

A section begins with a pound and ends with a slash. That is, `{{#person}}` begins a "person" section while `{{/person}}` ends it.

The behavior of the section is determined by the value of the key.



**False Values or Empty Lists**

If the `person` key exists and has a value of false or an empty list, the HTML between the pound and slash will not be displayed.

Template:

```
Shown.
{{#person}}
  Never shown!
{{/person}}
```

Hash:

```
{
  "person": false
}
```

Output:

```
Shown.
```

**Non-Empty Lists**

If the `person` key exists and has a non-false value, the HTML between the pound and slash will be rendered and displayed one or more times.



When the value is a non-empty list, the text in the block will be displayed once for each item in the list. The context of the block will be set to the current item for each iteration. In this way we can loop over collections.

Template:

```
{{#repo}}
  <b>{{name}}</b>
{{/repo}}
```

Hash:

```
{
  "repo": [
    { "name": "resque" },
    { "name": "hub" },
    { "name": "rip" }
  ]
}
```

Output:

```
<b>resque</b>
<b>hub</b>
<b>rip</b>
```

**Lambdas**

When the value is a callable object, such as a function or lambda, the object will be invoked and passed the block of text. The text passed is the literal block, unrendered. `{{tags}}` will not have been expanded - the lambda should do that on its own. In this way you can implement filters or caching.

Template:

```
{{#wrapped}}
  {{name}} is awesome.
{{/wrapped}}
```

Hash:

```
{
  "name": "Willy",
  "wrapped": function() {
    return function(text, render) {
      return "<b>" + render(text) + "</b>"
    }
  }
}
```

Output:

```
<b>Willy is awesome.</b>
```

**Non-False Values**

When the value is non-false but not a list, it will be used as the context for a single rendering of the block.

Template:

```
{{#person?}}
  Hi {{name}}!
{{/person?}}
```

Hash:

```
{
  "person?": { "name": "Jon" }
}
```

Output:

```
Hi Jon!
```

### Inverted Sections

An inverted section begins with a caret (hat) and ends with a slash. That is `{{^person}}` begins a "person" inverted section while `{{/person}}` ends it.

While sections can be used to render text one or more times based on the value of the key, inverted sections may render text once based on the inverse value of the key. That is, they will be rendered if the key doesn't exist, is false, or is an empty list.

Template:

```
{{#repo}}
  <b>{{name}}</b>
{{/repo}}
{{^repo}}
  No repos :(
{{/repo}}
```

Hash:

```
{
  "repo": []
}
```

Output:

```
No repos :(
```

### Comments

Comments begin with a bang and are ignored. The following template:

```
<h1>Today{{! ignore me }}.</h1>
```

Will render as follows:

```
<h1>Today.</h1>
```

Comments may contain newlines.

### Partials

Partials begin with a greater than sign, like `{{> box}}`.

Partials are rendered at runtime (as opposed to compile time), so recursive partials are possible. Just avoid infinite loops.

They also inherit the calling context. Whereas in an [ERB](http://en.wikipedia.org/wiki/ERuby) file you may have this:

```
<%= partial :next_more, :start => start, :size => size %>
```

Mustache requires only this:

```
{{> next_more}}
```

Why? Because the `next_more.mustache` file will inherit the `size` and `start` methods from the calling context.

In this way you may want to think of partials as includes, imports, template expansion, nested templates, or subtemplates, even though those aren't literally the case here.

For example, this template and partial:

```
base.mustache:
<h2>Names</h2>
{{#names}}
  {{> user}}
{{/names}}

user.mustache:
<strong>{{name}}</strong>
```

Can be thought of as a single, expanded template:

```
<h2>Names</h2>
{{#names}}
  <strong>{{name}}</strong>
{{/names}}
```

### Set Delimiter

Set Delimiter tags start with an equal sign and change the tag delimiters from `{{` and `}}` to custom strings.

Consider the following contrived example:

```
* {{default_tags}}
{{=<% %>=}}
* <% erb_style_tags %>
<%={{ }}=%>
* {{ default_tags_again }}
```

Here we have a list with three items. The first item uses the default tag style, the second uses erb style as defined by the Set Delimiter tag, and the third returns to the default style after yet another Set Delimiter declaration.

According to [ctemplates](http://google-ctemplate.googlecode.com/svn/trunk/doc/howto.html), this "is useful for languages like TeX, where double-braces may occur in the text and are awkward to use for markup."

Custom delimiters may not contain whitespace or the equals sign.



## SEE ALSO

[mustache(1)](http://mustache.github.io/mustache.1.ron.html), http://mustache.github.io/