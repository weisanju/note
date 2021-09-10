# bucket命名规范

The following rules apply for naming buckets in Amazon S3:

- 存储桶名称的长度必须介于 3 到 63 个字符之间。
- 存储桶名称只能由小写字母、数字、点 (.) 和连字符 (-) 组成。
- 存储桶名称必须以字母或数字开头和结尾。
- 存储桶名称不得格式化为 IP 地址（例如，192.168.5.4）。
- 存储桶名称不得以前缀 xn-- 开头。
- 存储桶名称不得以后缀 -s3alias 结尾。此后缀是为接入点别名保留的。有关更多信息，请参阅为您的访问点使用存储桶式别名。
- Bucket 名称在一个分区内必须是唯一的。分区是一组 Region。 AWS 目前有三个分区：aws（标准区域）、aws-cn（中国区域）和 aws-us-gov（AWS GovCloud [美国] 区域）。



