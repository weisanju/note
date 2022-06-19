# 活动图

## 开始/结束

```
start 
statement...
end
```

## 条件语句

```
start
if (condition A) then (yes)
  :Text 1;
elseif (condition B) then (yes)
  :Text 2;
  stop
elseif (condition C) then (yes)
  :Text 3;
elseif (condition D) then (yes)
  :Text 4;
else (nothing)
  :Text else;
endif
stop
```

## 循环

```
repeat循环
start
repeat
  :read data;
  :generate diagrams;
repeat while (more data?)
stop

while循环
start
while (data available?)
  :read data;
  :generate diagrams;
endwhile
stop
```

## 并行处理

```
start

if (multiprocessor?) then (yes)
  fork
    :Treatment 1;
  fork again
    :Treatment 2;
  end fork
else (monoproc)
  :Treatment 1;
  :Treatment 2;
endif
```

## 注释

```
start
:foo1;
floating note left: This is a note
:foo2;
note right
  This note is on several
  //lines// and can
  contain <b>HTML</b>
  ====
  * Calling the method ""foo()"" is prohibited
end note
stop
```

## 箭头样式

```
:foo1;
-> You can put text on arrows;
if (test) then
  -[#blue]->
  :foo2;
  -[#green,dashed]-> The text can
  also be on several lines
  and **very** long...;
  :foo3;
else
  -[#black,dotted]->
  :foo4;
endif
-[#gray,bold]->
:foo5;
```

## 连接器

```
start
:Some activity;
(A)
detach
(A)
:Other activity;
```

## 组合(*Grouping*)

```
start
partition Initialization {
    :read config file;
    :init internal variable;
}
partition Running {
    :wait for user interaction;
    :print information;
}

stop
```

## 泳道(*Swimlanes*)

```
可以改变泳道颜色
|Swimlane1|
start
:foo1;
|#AntiqueWhite|Swimlane2|
:foo2;
:foo3;
|Swimlane1|
:foo4;
|Swimlane2|
:foo5;
stop
```

## 分离(detach)

```
在fork中分离
:start;
 fork
   :foo1;
   :foo2;
 fork again
   :foo3;
   detach
 endfork
 if (foo4) then
   :foo5;
   detach
 endif
 :foo6;
 detach
 :foo7;
 stop
```



# 类图

## 类之间的关系

| **Type**            | **Symbol** | **Drawing**                                      |
| ------------------- | ---------- | ------------------------------------------------ |
| Extension（扩展）   | `<|--`     | ![img](https://s.plantuml.com/img/extends01.png) |
| Composition（组合） | `*--`      | ![img](https://s.plantuml.com/img/sym03.png)     |
| Aggregation（聚合） | `o--`      | ![img](https://s.plantuml.com/img/sym01.png)     |

## 箭头样式

```
[arrow_style]line_style
arrow_style=*|o|#|x|}|+|^
line_style= --|..
```

## 箭头标识

```
"":位于 某个类的左右,
: 位于连接线中间
@startuml

Class01 "1" *-- "many" Class02 : contains

Class03 o-- Class04 : aggregation

Class05 --> "1" Class06

@enduml
```

