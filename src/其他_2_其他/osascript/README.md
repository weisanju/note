## AppleScript 基础

### 1. 「告诉 xxx 做某事」的俄罗斯套娃结构

AppleScript 的语法非常接近自然语言，想要操控一个应用（application）做某件事，只要直接「告诉」它就好了。

```
tell application "Money Pro"
    activate -- 告诉 Money Pro，让它激活自己
end tell
```



### UI层级

然而，如果想要用 `tell` 访问某一个 UI 元素，必须按下图中的层级结构，一层一层按顺序进行访问

```
System Events 是最外围的框架 → Money Pro（具体某个应用）→ window 1（该应用的第1个窗口）→ button 1（窗口中的第1个按钮）
```

**理解和掌握这种层级关系，是进行 GUI Scripting 非常关键的一步。**

![img](../../../../images/apply_ui_element.png)





System Events 是系统应用（application），要「告诉」它做某事，AppleScript 要这么写，

```
tell application "System Events"
    -- 你希望应用 System Events 做的事
end tell
```

1. 所有带 UI 结构的应用，都是 System Events 下的进程（process），包括 Money Pro。如果我要「告诉」Money Pro 做某事，

2. 因为它是套在 System Events 之内的，就要这么写

```
tell application "System Events"
    tell process "Money Pro"
        -- 你希望进程 Money Pro 做的事
    end tell
end tell
```

```
tell application "System Events"
    tell process "Money Pro"
        tell window 1
            -- 你希望 window 1 做的事
        end tell 
    end tell
end tell
```

```
tell application "System Events"
    tell process "Money Pro"
        tell window 1
            tell something
                tell something
                    tell something
                        -- System Events: 你他喵的够了
                    end tell
                end tell
            end tell
        end tell
    end tell
end tell
```

我们需要控制的 UI 元素一般藏在比较深的层级中，它们的完整描述都很长。拿下图中的「软件」菜单项举例，这个例子将会会贯穿全文。



图中被选中的这个「软件」菜单项的完整描述是「Money Pro 应用中的第一个窗口中的第一个可滚动区域中的第一个表单中的第二列中的第一个 UI 元素中的第一个弹出菜单按钮的第一个弹出菜单中的菜单项“买买买”的第一个菜单中的“软件”菜单项」，在 AppleScript 中就是

```
menu item "软件" of menu 1 of menu item "买买买" of menu 1 of pop up button 1 of UI element 1 of row 2 of table 1 of scroll area 1 of window 1 of process "Money Pro" of application "System Events"
```



### 睡眠

```
delay 0.5   
```

### 注释

AppleScript 中凡是双短线 `--` 和井号 `#` 后的内容都会被认为是注释，不会被执行。



### 模拟键盘操作

```
keystroke "一串字符"

```

也可以用 `key code` 来实现单键操作，比如利用

```
key code 53

```

来模拟点击键盘上的 Escape 键。完整的键位代码你可以在[这里](https://eastmanreference.com/complete-list-of-applescript-key-codes/)找到。



### 如何定位 UI 元素，获取它的完整描述

#### 获取所有内容

这一节将重点说明如何去获取一个 UI 元素。紧接着上文 Money Pro 的例子——关于我是如何获得那个「软件」菜单项的 AppleScript 完整描述。

用 AppleScript 获取某个区域内所有 UI 元素只需两个单词 `entire contents`。

```
tell application "System Events"
    tell process "Money Pro" -- 告诉 Money Pro
        entire contents -- 获取所有 UI 元素
    end tell
end tell
```

你就会得到 Money Pro 这整个应用所有的 UI 元素的**完整描述**，甚至是顶部菜单栏中的内容。UI 元素之间被逗号隔开。

### 筛选窗口

如果你需要进一步缩小范围，比如我不想看菜单栏的内容，那就再套一层 `tell window 1` 2 的语句：

```
tell application "System Events"
    tell process "Money Pro" -- 告诉 Money Pro
        tell window 1 -- 再告诉 Money Pro 的第一个窗口
            entire contents -- 获取所有 UI 元素
        end tell
    end tell
end tell
```





### 筛选可能是目标 UI 元素的内容

1. 如果这个元素有名称，比如菜单项显示的文字，那就直接查找这个文字！比如对于那个菜单项「软件」，如果你搜「软件」，直接就能定位到。 

2. 你还可能遇到不熟悉的 UI 元素类型（比如不确定一个按钮的类型该是 `button`，还是 `radio button`），

3. 你可以利用原生应用「Accessibility Inspector（需安装 Xcode）」去审查它，它长这个样子，Spotlight 一搜就能搜到：



### 定位 UI 元素之后可以做什么？

```
tell application "System Events"
    tell process "Money Pro"
        click menu item "软件" of menu 1 of menu item "买买买" of menu 1 of pop up button 1 of UI element 1 of row 2 of table 1 of scroll area 1 of window 1 -- of process "Money Pro" of application "System Events"
    end tell
end tell
```

因为我们处在进程 Money Pro 的 `tell` 中，所以需要注释掉后面 `of process "Money Pro" of application "System Events"` 的部分。



#### 如果是文本输入框，你可以设置文本框内容

```
set value of text field 1 of ... to "一些文本内容"
```

也可以设置激活该输入框的光标，

```
set value of attribute "AXFocused" of text field 1 of ... to true

```

#### 两次 click 事件之间的延迟问题 

正常情况下你无法快速点击两次菜单项——两次 `click` 事件之间会被强行插入一个 5 秒左右的延迟。这是 macOS 的保护机制，为了应用的 UI 反馈能够被成功接收。但是 5 秒的延迟太长太不讲道理了。

所幸的是，Stack Overflow 里的[这篇帖子](https://stackoverflow.com/questions/21270264/speed-up-applescript-ui-scripting?answertab=active#tab-top)提供了一个有效解决方案。

简言之就是先忽略第一次点击按钮后应用的 UI 反馈：

```
ignoring application responses
    -- 这里是你的第一次点击操作
    click button 1
end ignoring

delay 0.1
do shell script "killall System\\ Events"



tell application "System Events"
    tell process "Reeder"
        ignoring application responses --忽略应用的反馈
            click button 1 of window "Day One 2"
        end ignoring
    end tell
end tell

-- 杀掉 System Events 应用
delay 0.1 --自定义 UI 反馈的等待时间为0.1 秒
do shell script "killall System\\ Events"

tell application "System Events"
    tell process "Reeder"
        -- 第二次点击操作
    end tell
end tell
```



### 检测屏幕内容

1. 一套自动化流程必定包含许多操作，这些操作之间会有不可避免的等待时间。

2. 比如，等待一个应用的主窗口打开。最简单的方法是自己估计所需的时间，然后用 `delay`语句让 AppleScript 暂停一会儿。

3. 然而为了 AppleScript 能够有效执行，等待时间需比实际时间要长，这样就不效率了！

```
tell application "Safari" to activate --打开 Safari
tell application "System Events"
    tell process "Safari"
        repeat until window 1 exists
            -- 直到 Safari 应用的一个窗口存在之前，不停循环这段空语句
        end repeat
        -- 第一个窗口出现之后，继续要做的事……
    end tell
end tell
```

