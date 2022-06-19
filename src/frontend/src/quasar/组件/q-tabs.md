# 选项卡

选项卡是一种使用较少的窗口空间显示更多信息的方法。 本页通过QTab、QTab和QRouteTab描述选项卡选择部分。

该组件的一种常见用例是在布局的页眉/页脚中。



> 与[QTabPanels](http://www.quasarchs.com/vue-components/tab-panels)配合使用，这是一个严格指向面板（选项卡内容）本身的组件。



# 基本使用

```vue
      <q-tabs
        v-model="tab"
        class="text-red"
      >
        <q-tab name="mails" icon="mail" label="Mails" />
        <q-tab name="alarms" icon="alarm" label="Alarms" />
        <q-tab name="movies" icon="movie" label="Movies" />
      </q-tabs>
```





# 样式

## 溢出样式

当宽度大于容器宽度时，QTab可以水平滚动

* 在桌面上，你会看到两边有可以点击的V形符号。

* 在手机上，你可以用手指平移选项卡。
* 如果要强制箭头在手机上可见，请使用`mobile-arrows`属性。

使用`outside-arrows`        `mobile-arrows` 属性指定外部，内部箭头

## 箭头

**外部箭头**

```vue
  <q-tabs
        v-model="tab"
        inline-label
        outside-arrows
        mobile-arrows
        class="bg-primary text-white shadow-2"
      >
        <q-tab name="mails" icon="mail" label="Mails" />
        <q-tab name="alarms" icon="alarm" label="Alarms" />
        <q-tab name="movies" icon="movie" label="Movies" />
        <q-tab name="photos" icon="photo" label="Photos" />
        <q-tab name="videos" icon="slow_motion_video" label="Videos" />
        <q-tab name="addressbook" icon="people" label="Address Book" />
      </q-tabs>
```

**内部箭头**

```vue
   <q-tabs
        v-model="tab"
        inline-label
        mobile-arrows
        class="bg-purple text-white shadow-2"
      >
        <q-tab name="mails" icon="mail" label="Mails" />
        <q-tab name="alarms" icon="alarm" label="Alarms" />
        <q-tab name="movies" icon="movie" label="Movies" />
        <q-tab name="photos" icon="photo" label="Photos" />
        <q-tab name="videos" icon="slow_motion_video" label="Videos" />
        <q-tab name="addressbook" icon="people" label="Address Book" />
      </q-tabs>

```

## 垂直布局

> vertical
>
> 作用于容器

```vue
  <q-splitter
      v-model="splitterModel"
      style="height: 250px"
    > //确定了两个分隔区域布局

      //before区域 tab垂直分布
      <template v-slot:before>
        <q-tabs
          v-model="tab"
          vertical 
          class="text-teal"
        >
          <q-tab name="mails" icon="mail" label="Mails" />
          <q-tab name="alarms" icon="alarm" label="Alarms" />
          <q-tab name="movies" icon="movie" label="Movies" />
        </q-tabs>
      </template>

      //after区域 使用 tab-panels
      <template v-slot:after>
        <q-tab-panels
          v-model="tab"
          animated
          swipeable
          vertical
          transition-prev="jump-up"
          transition-next="jump-up"
        >
          <q-tab-panel name="mails">
            <div class="text-h4 q-mb-md">Mails</div>
            <p>Lorem ipsum dolor sit, amet consectetur adipisicing elit. Quis praesentium cumque magnam odio iure quidem, quod illum numquam possimus obcaecati commodi minima assumenda consectetur culpa fuga nulla ullam. In, libero.</p>
            <p>Lorem ipsum dolor sit, amet consectetur adipisicing elit. Quis praesentium cumque magnam odio iure quidem, quod illum numquam possimus obcaecati commodi minima assumenda consectetur culpa fuga nulla ullam. In, libero.</p>
          </q-tab-panel>

          <q-tab-panel name="alarms">
            <div class="text-h4 q-mb-md">Alarms</div>
            <p>Lorem ipsum dolor sit, amet consectetur adipisicing elit. Quis praesentium cumque magnam odio iure quidem, quod illum numquam possimus obcaecati commodi minima assumenda consectetur culpa fuga nulla ullam. In, libero.</p>
            <p>Lorem ipsum dolor sit, amet consectetur adipisicing elit. Quis praesentium cumque magnam odio iure quidem, quod illum numquam possimus obcaecati commodi minima assumenda consectetur culpa fuga nulla ullam. In, libero.</p>
          </q-tab-panel>

          <q-tab-panel name="movies">
            <div class="text-h4 q-mb-md">Movies</div>
            <p>Lorem ipsum dolor sit, amet consectetur adipisicing elit. Quis praesentium cumque magnam odio iure quidem, quod illum numquam possimus obcaecati commodi minima assumenda consectetur culpa fuga nulla ullam. In, libero.</p>
            <p>Lorem ipsum dolor sit, amet consectetur adipisicing elit. Quis praesentium cumque magnam odio iure quidem, quod illum numquam possimus obcaecati commodi minima assumenda consectetur culpa fuga nulla ullam. In, libero.</p>
            <p>Lorem ipsum dolor sit, amet consectetur adipisicing elit. Quis praesentium cumque magnam odio iure quidem, quod illum numquam possimus obcaecati commodi minima assumenda consectetur culpa fuga nulla ullam. In, libero.</p>
          </q-tab-panel>
        </q-tab-panels>
      </template>

    </q-splitter>
```

## 稠密

>  dense
>
> 作用于容器

高度减小

## 颜色跟随

> narrow-indicator
>
> 作用于容器

指示器跟随 子元素的颜色

```vue
 <q-tabs
        v-model="tab"
        narrow-indicator
        dense
        align="justify"
      >
        <q-tab class="text-purple" name="mails" icon="mail" label="Mails" />
        <q-tab class="text-orange" name="alarms" icon="alarm" label="Alarms" />
        <q-tab class="text-teal" name="movies" icon="movie" label="Movies" />
      </q-tabs>

      <q-tabs
        v-model="tab"
        class="bg-grey-1"
        dense
        align="justify"
      >
        <q-tab class="text-orange" name="mails" icon="mail" label="Mails" />
        <q-tab class="text-cyan" name="alarms" icon="alarm" label="Alarms" />
        <q-tab class="text-red" name="movies" icon="movie" label="Movies" />
      </q-tabs>
```

## 波纹

> 点击 波纹
>
> 作用于 子元素
>
> 属性值 json

```vue
   <q-tabs
        v-model="tab"
        narrow-indicator
        dense
        align="justify"
        class="text-primary"
      >
        <q-tab :ripple="false" name="mails" icon="mail" label="Mails" />
        <q-tab :ripple="false" name="alarms" icon="alarm" label="Alarms" />
        <q-tab :ripple="false" name="movies" icon="movie" label="Movies" />
      </q-tabs>

      <q-tabs
        v-model="tab"
        narrow-indicator
        dense
        align="justify"
        class="text-purple"
      >
        <q-tab :ripple="{ color: 'orange' }" name="mails" icon="mail" label="Mails" />
        <q-tab :ripple="{ color: 'orange' }" name="alarms" icon="alarm" label="Alarms" />
        <q-tab :ripple="{ color: 'orange' }" name="movies" icon="movie" label="Movies" />
      </q-tabs>
```

## 自定义指示器

修改颜色

>   indicator-color="transparent"

修改长短

> narrow-indicator

```vue
   <q-tabs
        v-model="tab"
        narrow-indicator
        class="bg-purple text-white shadow-2"
      >
        <q-tab name="mails" icon="mail" label="Mails" />
        <q-tab name="alarms" icon="alarm" label="Alarms" />
        <q-tab name="movies" icon="movie" label="Movies" />
      </q-tabs>

```



## 对齐

>  当容器宽度（而非窗口宽度）大于配置的断点时，QTab会做出响应，并且`align`属性（请参见下文）会被激活

**align**

* left
* right
* center
* justify

```vue
        <q-tabs v-model="tab" :breakpoint="1000" align="left">
          <q-tab name="tab1" label="Tab 1" />
          <q-tab name="tab2" label="Tab 2" />
          <q-tab name="tab3" label="Tab 3" />
        </q-tabs>
```

## 提示与通知

> alert， alert-icon

```vue

      <q-tabs
        v-model="tab"
        class="bg-purple text-white shadow-2"
      >
        <q-tab alert="yellow" alert-icon="warning" name="mails" label="Mails" />
        <q-tab alert alert-icon="event" label="Alarms" name="alarms" />
        <q-tab alert="orange" alert-icon="announcement" name="movies" label="Movies" />
      </q-tabs>

```



# 与其他组件结合

## 与q-badge使用

```vue
        <q-tabs v-model="tab" :breakpoint="1000" align="left">
          <q-tab name="tab1" label="Tab 1">
            <q-badge color="red" floating>2</q-badge>
          </q-tab>
          <q-tab name="tab2" label="Tab 2" />
          <q-tab name="tab3" label="Tab 3" />
        </q-tabs>
```

## **与tabsPanel一起使用**

```vue
      <q-card>
        <q-tabs
          v-model="tab"
          dense
          class="text-grey"
          active-color="primary"
          indicator-color="primary"
          align="justify"
          narrow-indicator
        >
          <q-tab name="mails" label="Mails" />
          <q-tab name="alarms" label="Alarms" />
          <q-tab name="movies" label="Movies" />
        </q-tabs>

        <q-separator />

        <q-tab-panels v-model="tab" animated>
          <q-tab-panel name="mails">
            <div class="text-h6">Mails</div>
            Lorem ipsum dolor sit amet consectetur adipisicing elit.
          </q-tab-panel>

          <q-tab-panel name="alarms">
            <div class="text-h6">Alarms</div>
            Lorem ipsum dolor sit amet consectetur adipisicing elit.
          </q-tab-panel>

          <q-tab-panel name="movies">
            <div class="text-h6">Movies</div>
            Lorem ipsum dolor sit amet consectetur adipisicing elit.
          </q-tab-panel>
        </q-tab-panels>
      </q-card>
```







# 案例

## 根据屏幕宽度响应式设计

```vue
      <q-tabs
        v-model="tab"
        inline-label
        :breakpoint="0"
        align="justify"
        class="bg-purple text-white shadow-2"
      >
        <q-tab name="mails" label="Mails" />
        <q-tab name="alarms" label="Alarms" />
        <q-tab v-if="$q.screen.gt.sm" name="movies" label="Movies" />
        <q-tab v-if="$q.screen.gt.sm" name="photos" label="Photos" />
        <q-btn-dropdown v-if="$q.screen.lt.md" auto-close stretch flat label="More...">
          <q-list>
            <q-item clickable @click="tab = 'movies'">
              <q-item-section>Movies</q-item-section>
            </q-item>

            <q-item clickable @click="tab = 'photos'">
              <q-item-section>Photos</q-item-section>
            </q-item>
          </q-list>
        </q-btn-dropdown>
      </q-tabs>
```

## 动态更新

```vue
<template>
  <div class="q-pa-md">
    <div class="q-gutter-y-md" style="max-width: 600px">
      <q-list>
        <q-item v-for="item in allTabs" :key="item.tab.name" tag="label" dense v-ripple>
          <q-item-section side>
            <q-checkbox :value="item.selected" @input="status => { setTabSelected(item.tab, status) }" />
          </q-item-section>

          <q-item-section>
            <q-item-label>{{ item.tab.label }}</q-item-label>
          </q-item-section>

          <q-item-section side>
            <q-icon :name="item.tab.icon" />
          </q-item-section>
        </q-item>
      </q-list>

      <q-toolbar class="bg-purple text-white shadow-2 rounded-borders">
        <q-btn flat label="Homepage" />
        <q-space />

        <!--
          notice shrink property since we are placing it
          as child of QToolbar
        -->
        <q-tabs
          v-model="tab"
          inline-label
          shrink
          stretch
        >
          <q-tab v-for="tab in tabs" :key="tab.name" v-bind="tab" />
        </q-tabs>
      </q-toolbar>
    </div>
  </div>
</template>
```

## 连接Vue-router

```vue
<q-tabs>
  <q-route-tab
    icon="mail"
    to="/mails"
    exact
  />
  <q-route-tab
    icon="alarm"
    to="/alarms"
    exact
  />
</q-tabs>

<template>
  <div class="q-pa-md">
    <div class="q-gutter-y-md" style="max-width: 600px">
      <q-tabs
        no-caps
        class="bg-orange text-white shadow-2"
      >
        <q-route-tab :to="{ query: { tab: '1' } }" exact replace label="Activate in 2s" @click="navDelay" />
        <q-route-tab :to="{ query: { tab: '2' } }" exact replace label="Do nothing" @click="navCancel" />
        <q-route-tab :to="{ query: { tab: '3' } }" exact replace label="Navigate to the second tab" @click="navRedirect" />
        <q-route-tab :to="{ query: { tab: '4' } }" exact replace label="Navigate immediatelly" @click="navPass" />
      </q-tabs>
    </div>
  </div>
</template>

<script>
export default {
  methods: {
    navDelay (e, go) {
      e.navigate = false // we cancel the default navigation

      // console.log('triggering navigation in 2s')
      setTimeout(() => {
        // console.log('navigating as promised 2s ago')
        go()
      }, 2000)
    },

    navCancel (e) {
      e.navigate = false // we cancel the default navigation
    },

    navRedirect (e, go) {
      e.navigate = false // we cancel the default navigation

      go({ query: { tab: '2', noScroll: true } })
    },

    navPass () {}
  }
}
</script>
```

# [API](http://www.quasarchs.com/vue-components/tabs#qtabs-api)