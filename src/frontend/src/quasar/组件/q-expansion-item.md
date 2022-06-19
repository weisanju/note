# 展开项

QExpansionItem组件允许隐藏与用户不立即相关的内容。 将它们视为单击时会扩展的手风琴元素。 也称为可折叠。





# 基础用法

```vue
<q-expansion-item
                  expand-separator
                  icon="perm_identity"
                  label="Account settings"
                  caption="John Doe"
                  >
    <q-card>
        <q-card-section>
            Lorem ipsum dolor sit amet, consectetur adipisicing elit. Quidem, eius reprehenderit eos corrupti
            commodi magni quaerat ex numquam, dolorum officiis modi facere maiores architecto suscipit iste
            eveniet doloribus ullam aliquid.
        </q-card-section>
    </q-card>
</q-expansion-item>
```







# 行为

## 路由集成

| name               | type            | description                                                  | defaultValue | example                         |
| ------------------ | --------------- | ------------------------------------------------------------ | ------------ | ------------------------------- |
| to                 | String \|Object | 等价于 `<router-link>` 'to' property                         |              | :to="{ name: 'my-route-name' }" |
| exact              | Boolean         | Equivalent to Vue Router <router-link> 'exact' property      |              |                                 |
| append             | Boolean         | Equivalent to Vue Router <router-link> 'append' property     |              |                                 |
| replace            | Boolean         | Equivalent to Vue Router <router-link> 'replace' property    |              |                                 |
| active-class       | String          | Equivalent to Vue Router <router-link> 'active-class' property |              | my-active-class                 |
| exact-active-class |                 | Equivalent to Vue Router <router-link> 'active-class' property |              | my-exact-active-class           |

## 行为

| name               | type    | description                            | defaultValue            | example          |
| ------------------ | ------- | -------------------------------------- | ----------------------- | ---------------- |
| duration           | Number  | 展开与收起的速度                       | 300                     | :duration="1000" |
| default-opened     | Boolean | 默认是否展开                           | false                   |                  |
| expand-icon-toggle | boolean | 触发展开事件，只有 icon 才触发展开事件 | false，整个item都会触发 |                  |
| group              | String  | 将item分组 的key                       |                         |                  |
| popup              | Boolean | 将展开收起动画 改成 弹出动画           |                         |                  |

## 内容

| name                | type            | description                                    | defaultValue | example                  |
| ------------------- | --------------- | ---------------------------------------------- | ------------ | ------------------------ |
| icon                | String          | 左侧icon图标                                   |              |                          |
| expand-icon         | String          | 右侧展开项 图标                                |              |                          |
| expanded-icon       | String          | 右侧展开后图标                                 |              |                          |
| label               | String          | 标题                                           |              |                          |
| label-lines         | Number \|String | 如果内容在指定行无法完成渲染 标题 则以省略号   |              | :label-lines="2"         |
| caption             | String          | 副标题                                         |              |                          |
| caption-lines       | String          | 如果内容在指定行无法完成渲染 副标题 则以省略号 |              |                          |
| header-inset-level  | Number          | 应用缩进，给整个item应用 padding-left          |              | :header-inset-level="1"  |
| content-inset-level | Number          | 给隐藏的内容区 应用 padding-left               |              | :content-inset-level="1" |
| expand-separator    | Boolean         | 给展开项之间 应用 分割线                       |              |                          |
| switch-toggle-side  | Boolean         | 切换 expand icon  左右 方向                    | false,right  | true left                |

## 样式

| name              | type                    | description                     | defaultValue | example                                              |
| ----------------- | ----------------------- | ------------------------------- | ------------ | ---------------------------------------------------- |
| expand-icon-class | Array \|String \|Object | 对于展开图标 应用自定义的样式   |              | expand-icon-class="text-purple"                      |
| dark              | Boolean                 | 黑暗模式                        |              |                                                      |
| dense             | Boolean                 | Dense mode; occupies less space |              |                                                      |
| dense-toggle      | Boolean                 | Use dense mode for expand icon  |              |                                                      |
| header-style      | Array \|String \|Object | 对 header应用自定义的 样式      |              |                                                      |
| header-class      | Array \|String \|Object | 对 header应用自定义的 样式      |              | :header-class="{ 'my-custom-class': someCondition }" |





## 其他

| name    | type    | description            | defaultValue | example |
| ------- | ------- | ---------------------- | ------------ | ------- |
| disable | Boolean | 禁用图标               |              |         |
| value   | Boolean | Model of the component |              |         |



## SLOT槽

| name    | description                             | example |
| ------- | --------------------------------------- | ------- |
| default | Slot used for expansion item's content  |         |
| header  | Slot used for overriding default header |         |

