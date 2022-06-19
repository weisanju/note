### [Loops](https://docs.ansible.com/ansible/latest/user_guide/playbooks_loops.html)

1. Ansible提供 ， `loop`, `with_<lookup>`, and `until` 关键字多次执行任务。

2. 常用循环的示例 包括使用文件模块更改多个文件和/或目录的所有权， [file module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/file_module.html#file-module)
3. creating multiple users with the [user module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/user_module.html#user-module),
4. 并重复轮询步骤，直到达到某个结果。

### [Standard loops](https://docs.ansible.com/ansible/latest/user_guide/playbooks_loops.html#id4)

#### [Iterating over a simple list](https://docs.ansible.com/ansible/latest/user_guide/playbooks_loops.html#id5)

```yml
- name: Add several users
  ansible.builtin.user:
    name: "{{ item }}"
    state: present
    groups: "wheel"
  loop:
     - testuser1
     - testuser2
```

您可以在variables 文件中或  play 的 vars 部分中定义列表，然后参考任务中列表的名称。

```
loop: "{{ somelist }}"

```

```
- name: Add user testuser1
  ansible.builtin.user:
    name: "testuser1"
    state: present
    groups: "wheel"

- name: Add user testuser2
  ansible.builtin.user:
    name: "testuser2"
    state: present
    groups: "wheel"
```



**注意**

1. 您可以将列表直接传递给某些插件的参数。

2. 大多数包装模块，例如yum和apt，都具有此功能。

3. 在可用时，将列表传递给参数比循环任务更好。例如



#### [Iterating over a list of hashes](https://docs.ansible.com/ansible/latest/user_guide/playbooks_loops.html#id6)

```
- name: Add several users
  ansible.builtin.user:
    name: "{{ item.name }}"
    state: present
    groups: "{{ item.groups }}"
  loop:
    - { name: 'testuser1', groups: 'wheel' }
    - { name: 'testuser2', groups: 'root' }
```

1. When combining [conditionals](https://docs.ansible.com/ansible/latest/user_guide/playbooks_conditionals.html#playbooks-conditionals) with a loop, 
2. the `when:` statement is processed separately for each item. See [Basic conditionals with when](https://docs.ansible.com/ansible/latest/user_guide/playbooks_conditionals.html#the-when-statement) for examples.

### [Iterating over a dictionary](https://docs.ansible.com/ansible/latest/user_guide/playbooks_loops.html#id7)

To loop over a dict, use the [dict2items](https://docs.ansible.com/ansible/latest/user_guide/playbooks_filters.html#dict-filter):

```yaml
- name: Using dict2items
  ansible.builtin.debug:
    msg: "{{ item.key }} - {{ item.value }}"
  loop: "{{ tag_data | dict2items }}"
  vars:
    tag_data:
      Environment: dev
      Application: payment
```

#### [Registering variables with a loop](https://docs.ansible.com/ansible/latest/user_guide/playbooks_loops.html#id8)

**output 注册为变量**

```yaml
- name: Register loop output as a variable
  ansible.builtin.shell: "echo {{ item }}"
  loop:
    - "one"
    - "two"
  register: echo
```



1. 使用 loop注册`register`  变量时： 
2. 返回结果包含 `results` 
3. 无loop的注册情况与此不同

```json
{
    "changed": true,
    "msg": "All items completed",
    "results": [
        {
            "changed": true,
            "cmd": "echo \"one\" ",
            "delta": "0:00:00.003110",
            "end": "2013-12-19 12:00:05.187153",
            "invocation": {
                "module_args": "echo \"one\"",
                "module_name": "shell"
            },
            "item": "one",
            "rc": 0,
            "start": "2013-12-19 12:00:05.184043",
            "stderr": "",
            "stdout": "one"
        },
        {
            "changed": true,
            "cmd": "echo \"two\" ",
            "delta": "0:00:00.002920",
            "end": "2013-12-19 12:00:05.245502",
            "invocation": {
                "module_args": "echo \"two\"",
                "module_name": "shell"
            },
            "item": "two",
            "rc": 0,
            "start": "2013-12-19 12:00:05.242582",
            "stderr": "",
            "stdout": "two"
        }
    ]
}
```

在注册变量上的后续循环以检查结果可能看起来像

```yaml
- name: Fail if return code is not 0
  ansible.builtin.fail:
    msg: "The command ({{ item.cmd }}) did not have a 0 return code"
  when: item.rc != 0
  loop: "{{ echo.results }}"
```

在迭代过程中，当前项 的结果将被放置在变量中。

```yaml
- name: Place the result of the current item in the variable
  ansible.builtin.shell: echo "{{ item }}"
  loop:
    - one
    - two
  register: echo
  changed_when: echo.stdout != "one"
```

### [Complex loops](https://docs.ansible.com/ansible/latest/user_guide/playbooks_loops.html#id9)

#### [Iterating over nested lists](https://docs.ansible.com/ansible/latest/user_guide/playbooks_loops.html#id10)

您可以使用Jinja2表达式迭代复杂列表。例如，一个循环可以组合嵌套列表。

```yaml
- name: Give users access to multiple databases
  community.mysql.mysql_user:
    name: "{{ item[0] }}"
    priv: "{{ item[1] }}.*:ALL"
    append_privs: yes
    password: "foo"
  loop: "{{ ['alice', 'bob'] | product(['clientdb', 'employeedb', 'providerdb']) | list }}"
```

#### [Retrying a task until a condition is met](https://docs.ansible.com/ansible/latest/user_guide/playbooks_loops.html#id11)

1. *New in version 1.4.*
2. 您可以使用 "until" 关键字重试任务，直到满足特定条件。下面是一个例子:

```yaml
- name: Retry a task until a certain condition is met
  ansible.builtin.shell: /usr/bin/foo
  register: result
  until: result.stdout.find("all systems go") != -1
  retries: 5
  delay: 10
```

1. 此任务最多运行5次，每次尝试之间延迟10秒。
2. If the result of any attempt has “all systems go” in its stdout, the task succeeds
3. “retries” 的默认值为3，“delay” 为5。



1. 要查看每次重试的结果，请使用-vv运行play。
2. 当使用 until 关键字，注册变量时 会多一个  *attempts* 记录 任务重试的次数

### [Looping over inventory](https://docs.ansible.com/ansible/latest/user_guide/playbooks_loops.html#id12)

1. To loop over your inventory, or just a subset of it, you can use a regular `loop` with the `ansible_play_batch` or `groups` variables.

```yaml
- name: Show all the hosts in the inventory
  ansible.builtin.debug:
    msg: "{{ item }}"
  loop: "{{ groups['all'] }}"

- name: Show all the hosts in the current play
  ansible.builtin.debug:
    msg: "{{ item }}"
  loop: "{{ ansible_play_batch }}"
```



There is also a specific lookup plugin `inventory_hostnames` that can be used like this

```
- name: Show all the hosts in the inventory
  ansible.builtin.debug:
    msg: "{{ item }}"
  loop: "{{ query('inventory_hostnames', 'all') }}"

- name: Show all the hosts matching the pattern, ie all but the group www
  ansible.builtin.debug:
    msg: "{{ item }}"
  loop: "{{ query('inventory_hostnames', 'all:!www') }}"
```

More information on the patterns can be found in [Patterns: targeting hosts and groups](https://docs.ansible.com/ansible/latest/user_guide/intro_patterns.html#intro-patterns).



#### [Ensuring list input for `loop`: using `query` rather than `lookup`](https://docs.ansible.com/ansible/latest/user_guide/playbooks_loops.html#id13)

1. loop关键字需要一个列表作为输入，但是lookup关键字默认返回一个逗号分隔的值字符串
2. Ansible 2.5引入了一个名为 [query](https://docs.ansible.com/ansible/latest/plugins/lookup.html#query) 的新Jinja2函数，该函数始终返回一个列表
3. 使用loop关键字时，提供更简单的接口和更可预测的查找插件输出。
4. 您可以指定 *wantlist = True*  强制  *loop* 返回列表以循环，也可以使用*query*代替。

```
loop: "{{ query('inventory_hostnames', 'all') }}"

loop: "{{ lookup('inventory_hostnames', 'all', wantlist=True) }}"
```

### [Adding controls to loops](https://docs.ansible.com/ansible/latest/user_guide/playbooks_loops.html#id14)

*New in version 2.1.*

The `loop_control` keyword lets you manage your loops in useful ways.

#### [Limiting loop output with `label`](https://docs.ansible.com/ansible/latest/user_guide/playbooks_loops.html#id15)

*New in version 2.2.*

1. 当循环遍历复杂的数据结构时，任务的控制台输出可能是巨大的. 
2. 限制显示的输出, use the `label` directive with `loop_control`.

```yaml
- name: Create servers
  digital_ocean:
    name: "{{ item.name }}"
    state: present
  loop:
    - name: server1
      disks: 3gb
      ram: 15Gb
      network:
        nic01: 100Gb
        nic02: 10Gb
        ...
  loop_control:
    label: "{{ item.name }}"
```



1. 此任务的输出将仅显示每个 item 的 name field ，而不是多行 {{ item }} 变量的全部内容。

2. 这是为了使控制台输出更具可读性，而不是保护敏感数据。如果循环中有敏感数据，请在任务上设置no_log: yes以防止泄露。





#### [Pausing within a loop](https://docs.ansible.com/ansible/latest/user_guide/playbooks_loops.html#id16)

*New in version 2.2.*

要控制任务循环中每个itm执行之间的时间 (以秒为单位)，请使用带有loop_control的pause指令。

```yaml
# main.yml
- name: Create servers, pause 3s before creating next
  community.digitalocean.digital_ocean:
    name: "{{ item }}"
    state: present
  loop:
    - server1
    - server2
  loop_control:
    pause: 3
```



#### [Tracking progress through a loop with `index_var`](https://docs.ansible.com/ansible/latest/user_guide/playbooks_loops.html#id17)

*New in version 2.5.*

1. 跟踪你在循环中的位置
2. 使用 `index_var` directive with `loop_control`. 
3. 此指令指定一个变量名，以包含当前循环索引。

```yaml
- name: Count our fruit
  ansible.builtin.debug:
    msg: "{{ item }} with index {{ my_idx }}"
  loop:
    - apple
    - banana
    - pear
  loop_control:
    index_var: my_idx
```

index_var is 0 indexed.





#### [Defining inner and outer variable names with `loop_var`](https://docs.ansible.com/ansible/latest/user_guide/playbooks_loops.html#id18)

*New in version 2.1.*

1. 通过使用  `include_tasks`  迭代 两个嵌套任务
2. 但是，默认情况下Ansible为每个循环设置循环变量 *item*。
3. 这意味着内部嵌套循环将覆盖外部循环中的*item*值
4. 您可以使用loop_var和loop_control为每个循环指定变量的名称。

```yaml
# main.yml
- include_tasks: inner.yml
  loop:
    - 1
    - 2
    - 3
  loop_control:
    loop_var: outer_item

# inner.yml
- name: Print outer and inner items
  ansible.builtin.debug:
    msg: "outer item={{ outer_item }} inner item={{ item }}"
  loop:
    - a
    - b
    - c
```

如果Ansible检测到当前循环使用的是已经定义的变量，则会引发错误以使任务失败。



#### [Extended loop variables](https://docs.ansible.com/ansible/latest/user_guide/playbooks_loops.html#id19)

*New in version 2.8.*

从Ansible 2.8开始，您可以使用扩展选项来循环控制来获取扩展的循环信息。此选项将公开以下信息。

| Variable                 | Description                                                  |
| ------------------------ | ------------------------------------------------------------ |
| `ansible_loop.allitems`  | The list of all items in the loop                            |
| `ansible_loop.index`     | The current iteration of the loop. (1 indexed)               |
| `ansible_loop.index0`    | The current iteration of the loop. (0 indexed)               |
| `ansible_loop.revindex`  | The number of iterations from the end of the loop (1 indexed) |
| `ansible_loop.revindex0` | The number of iterations from the end of the loop (0 indexed) |
| `ansible_loop.first`     | `True` if first iteration                                    |
| `ansible_loop.last`      | `True` if last iteration                                     |
| `ansible_loop.length`    | The number of items in the loop                              |
| `ansible_loop.previtem`  | The item from the previous iteration of the loop. Undefined during the first iteration. |
| `ansible_loop.nextitem`  | The item from the following iteration of the loop. Undefined during the last iteration. |

```
loop_control:
  extended: yes
```



When using `loop_control.extended` more memory will be utilized on the control node. 

1. 当使用*loop_control.extended*时，控制节点上将利用更多的内存
2. 因为 `ansible_loop.allitems`  包含 所有数据的引用
3. 当序列化结果以显示在主ansible进程内的回调插件中时，这些引用可能会被取消引用，导致内存使用量增加。



#### [Accessing the name of your loop_var](https://docs.ansible.com/ansible/latest/user_guide/playbooks_loops.html#id20)

*New in version 2.8.*



1. 从 Ansible 2.8 可以获取 `loop_control.loop_var` 变量名称
2. 对于role authors, ，编写允许循环的角色， instead of dictating the required `loop_var` value, you can gather the value via the following

```
"{{ lookup('vars', ansible_loop_var) }}"
```





### See also

- [Intro to playbooks](https://docs.ansible.com/ansible/latest/user_guide/playbooks_intro.html#about-playbooks)

  An introduction to playbooks

- [Roles](https://docs.ansible.com/ansible/latest/user_guide/playbooks_reuse_roles.html#playbooks-reuse-roles)

  Playbook organization by roles

- [Tips and tricks](https://docs.ansible.com/ansible/latest/user_guide/playbooks_best_practices.html#playbooks-best-practices)

  Tips and tricks for playbooks

- [Conditionals](https://docs.ansible.com/ansible/latest/user_guide/playbooks_conditionals.html#playbooks-conditionals)

  Conditional statements in playbooks

- [Using Variables](https://docs.ansible.com/ansible/latest/user_guide/playbooks_variables.html#playbooks-variables)

  All about variables

- [User Mailing List](https://groups.google.com/group/ansible-devel)

  Have a question? Stop by the google group!

- [Real-time chat](https://docs.ansible.com/ansible/latest/community/communication.html#communication-irc)

  How to join Ansible chat channels