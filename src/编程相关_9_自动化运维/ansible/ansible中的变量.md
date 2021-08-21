# 合法变量名

变量名可以为字母,数字以及下划线.变量始终应该以字母开头

# 在Inventory中定义变量



# 在playbook中定义变量

```sh
- hosts: webservers
  vars:
    http_port: 80
```

