# [工具栏](http://www.quasarchs.com/vue-components/toolbar#Introduction)

> QToolbar通常是布局页眉和页脚的一部分，但可以在页面上的任何位置使用。





# 布局

```vue
  <q-toolbar class="text-primary">
      <q-btn flat round dense icon="menu" />
      <q-toolbar-title>
        Toolbar
      </q-toolbar-title>
      <q-btn flat round dense icon="more_vert" />
    </q-toolbar>

```

* `toolbarTitle` 将位于 *toolbar* 的最左边，并有一定的 留白
* *toolbar* 的所有直接子元素 如果写在 *title* 的左边 则位于左边如果写在右边 则位于右边
* 所有直接子元素 均垂直居中



# 组合嵌套

## 垂直组合

* **insert缩进一个单位**

```vue
<q-toolbar>
    <q-btn flat round dense icon="menu" class="q-mr-sm" />
    <q-space />
    <q-btn flat round dense icon="search" class="q-mr-xs" />
    <q-btn flat round dense icon="group_add" />
</q-toolbar>
<q-toolbar inset>
    <q-toolbar-title><strong>Quasar</strong> Framework</q-toolbar-title>
</q-toolbar>
```

## 水平组合

* row表示该容器为 弹性容器。且元素为水平分布
* *col-8* 表示 该子元素占 2/3,col-4 表示该元素占 1/3

```vue
<div class="row no-wrap shadow-1">
    <q-toolbar class="col-8 bg-grey-3">
        <q-btn flat round dense icon="menu" />
        <q-toolbar-title>Title</q-toolbar-title>
        <q-btn flat round dense icon="search" />
    </q-toolbar>
    <q-toolbar class="col-4 bg-primary text-white">
        <q-space />
        <q-btn flat round dense icon="bluetooth" class="q-mr-sm" />
        <q-btn flat round dense icon="more_vert" />
    </q-toolbar>
</div>
```



# 与其他组件结合

## 与Tabs结合

```vue
 <q-toolbar class="bg-purple text-white shadow-2 rounded-borders">
      <q-btn flat label="Homepage" />
      <q-space />

      <!--
        notice shrink property since we are placing it
        as child of QToolbar
      -->
      <q-tabs v-model="tab" shrink>
        <q-tab name="tab1" label="Tab 1" />
        <q-tab name="tab2" label="Tab 2" />
        <q-tab name="tab3" label="Tab 3" />
      </q-tabs>
    </q-toolbar>
```

## With Button Dropdown

```vue
   <q-toolbar class="bg-primary text-white q-my-md shadow-2">
      <q-btn flat round dense icon="menu" class="q-mr-sm" />
      <q-separator dark vertical inset />
      <q-btn stretch flat label="Link" />

      <q-space />

      <q-btn-dropdown stretch flat label="Dropdown">
        <q-list>
          <q-item-label header>Folders</q-item-label>
          <q-item v-for="n in 3" :key="`x.${n}`" clickable v-close-popup tabindex="0">
            <q-item-section avatar>
              <q-avatar icon="folder" color="secondary" text-color="white" />
            </q-item-section>
            <q-item-section>
              <q-item-label>Photos</q-item-label>
              <q-item-label caption>February 22, 2016</q-item-label>
            </q-item-section>
            <q-item-section side>
              <q-icon name="info" />
            </q-item-section>
          </q-item>
          <q-separator inset spaced />
          <q-item-label header>Files</q-item-label>
          <q-item v-for="n in 3" :key="`y.${n}`" clickable v-close-popup tabindex="0">
            <q-item-section avatar>
              <q-avatar icon="assignment" color="primary" text-color="white" />
            </q-item-section>
            <q-item-section>
              <q-item-label>Vacation</q-item-label>
              <q-item-label caption>February 22, 2016</q-item-label>
            </q-item-section>
            <q-item-section side>
              <q-icon name="info" />
            </q-item-section>
          </q-item>
        </q-list>
      </q-btn-dropdown>
      <q-separator dark vertical />
      <q-btn stretch flat label="Link" />
      <q-separator dark vertical />
      <q-btn stretch flat label="Link" />
    </q-toolbar>
```

## With Button Toggle

```vue
  <q-toolbar class="bg-secondary text-white q-my-md shadow-2">
      <q-btn flat round dense icon="menu" class="q-mr-sm" />

      <q-space />

      <q-btn-toggle
        v-model="model"
        flat stretch
        toggle-color="yellow"
        :options="[
          {label: 'One', value: 'one'},
          {label: 'Two', value: 'two'},
          {label: 'Three', value: 'three'}
        ]"
      />
    </q-toolbar>
```

# API





# 样式

| 样式名   | 说明             | 默认值 | 可选值 |
| -------- | ---------------- | ------ | ------ |
| `glossy` | 使工具栏具有光泽 | 无     | bool值 |

