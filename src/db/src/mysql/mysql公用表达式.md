# 什么是公用表表达式(CTE)?

*common table expression*

1. 公用表表达式是一个**命名的临时结果集**

2. 仅在单个SQL语句  SELECT，INSERTUPDATE或DELETE的执行范围内存在

3. *CTE*不作为对象存储，仅在查询执行期间持续。 与派生表不同，*CTE*可以是自引用([递归CTE](http://www.yiibai.com/mysql/recursive-cte.html))，也可以在同一查询中多次引用。 此外，与派生表相比，*CTE*提供了更好的可读性和性能。



# MySQL CTE语法

**组成**

```mysql
with cte_name (column_list) as (
	query
)
SELECT * FROM cte_name
```

请注意，查询中的列数必须与`column_list`中的列数相同。 如果省略`column_list`，*CTE*将使用定义*CTE*的查询的列列表



# 多个CTE

```mysql
WITH salesrep AS (
    SELECT 
        employeeNumber,
        CONCAT(firstName, ' ', lastName) AS salesrepName
    FROM
        employees
    WHERE
        jobTitle = 'Sales Rep'
),
customer_salesrep AS (
    SELECT 
        customerName, salesrepName
    FROM
        customers
            INNER JOIN
        salesrep ON employeeNumber = salesrepEmployeeNumber
)
SELECT 
    *
FROM
    customer_salesrep
ORDER BY customerName;
```



# 递归查询

```mysql

WITH RECURSIVE test(id, name, path)
AS
(
    // 初始值
SELECT id, name, CAST(id AS CHAR(200))
FROM emp WHERE manager_id IS NULL
UNION ALL
    //递归查询
SELECT e.id, e.name, CONCAT(ep.path, ',', e.id)
FROM test AS ep JOIN emp AS e  ON ep.id = e.manager_id
)SELECT * FROM test ORDER BY path;
```



# 使用CTE的好处

○ 可读性：CTE提高了可读性。而不是将所有查询逻辑都集中到一个大型查询中，而是创建几个CTE，它们将在语句的后面组合。这使您可以获得所需的数据块，并将它们组合在最终的SELECT中。

○ 替代视图：您可以用CTE替换视图。如果您没有创建视图对象的权限，或者您不想创建一个视图对象，因为它仅在此一个查询中使用，这很方便。

○ 递归：使用CTE会创建递归查询，即可以调用自身的查询。当您需要处理组织结构图等分层数据时，这很方便。

○ 限制：克服SELECT语句限制，例如引用自身（递归）或使用非确定性函数执行GROUP BY。

○ 排名：每当你想使用排名函数，如ROW_NUMBER()，RANK()，NTILE()等。





# 子查询,派生表,临时表

## 子查询

子查询是嵌套在另一个查询(如select、insert、update和delete)中的查询。子查询又称为内部查询，而包含子查询的查询称为外部查询。 子查询可以在使用表达式的任何地方使用，并且必须在括号中关闭。



# 派生表

派生表和子查询通常可以互换使用，但是与子查询不同的是，派生表必须具有别名

```sql

SELECT column_list  FROM
    ( SELECT column_list  FROM table_1) derived_table_name   --派生表
WHERE derived_table_name.c1 > 0;
```

**`派生表之间不可以相互引用。例如：`SELECT ... FROM (SELECT ... FROM ...) AS d1, (SELECT ... FROM d1 ...) AS d2，第一个查询标记为d1，在第二个查询语句中使用d1是不允许的。**



## 临时表

临时表是一种特殊类型的表，它允许您存储一个临时结果集，可以在单个会话中多次重用。

* 使用`CREATE TEMPORARY TABLE`语句创建临时表。请注意，在`CREATE`和`TABLE`关键字之间添加`TEMPORARY`关键字。
* 当会话结束或连接终止时，MySQL会自动删除临时表。当您不再使用临时表时，也可以使用[DROP TABLE](http://www.yiibai.com/mysql/drop-table.html)语句来显式删除临时表。

* 一个临时表只能由创建它的客户机访问。不同的客户端可以创建具有相同名称的临时表，而不会导致错误，因为只有创建临时表的客户端才能看到它。 但是，在同一个会话中，两个临时表不能共享相同的名称。

* 临时表可以与数据库中的普通表具有相同的名称。 不推荐使用相同名称。例如，如果在[示例数据库(yiibaidb)](http://www.yiibai.com/mysql/sample-database.html)中创建一个名为`employees`的临时表，则现有的`employees`表将变得无法访问。 对`employees`表发出的每个查询现在都是指`employees`临时表。 当删除`您`临时表时，永久`employees`表可以再次访问。

```SQL

CREATE TEMPORARY TABLE table_name (
    name VARCHAR(10) NOT NULL,
    value INTEGER NOT NULL
  )
```









