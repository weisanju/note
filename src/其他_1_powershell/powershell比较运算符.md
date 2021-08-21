| 比较运算符   | 含义                       | 示例（返回 True）            |
| :----------- | :------------------------- | :--------------------------- |
| -eq          | 等于                       | 1 -eq 1                      |
| -ne          | 不等于                     | 1 -ne 2                      |
| -lt          | 小于                       | 1 -lt 2                      |
| -le          | 小于或等于                 | 1 -le 2                      |
| -gt          | 大于                       | 2 -gt 1                      |
| -ge          | 大于或等于                 | 2 -ge 1                      |
| -like        | 相似（文本的通配符比较）   | "file.doc" -like "f*.do?"    |
| -notlike     | 不相似（文本的通配符比较） | "file.doc" -notlike "p*.doc" |
| -contains    | 包含                       | 1,2,3 -contains 1            |
| -notcontains | 不包含                     | 1,2,3 -notcontains 4         |