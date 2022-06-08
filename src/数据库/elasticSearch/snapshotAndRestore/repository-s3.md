## S3 Repository Plugin

该插件添加了 对 AWS S3 的 快照备份恢复的 支持



### Installation

```
sudo bin/elasticsearch-plugin install repository-s3
```

### Removal

```
sudo bin/elasticsearch-plugin remove repository-s3
```

插件必须安装在集群中的每个节点上，并且每个节点都必须在安装后重新启动。

```shell
multipass exec primary sudo /usr/share/elasticsearch/bin/elasticsearch-plugin install repository-s3

multipass exec primary sudo systemctl restart elasticsearch

multipass exec node-2 sudo /usr/share/elasticsearch/bin/elasticsearch-plugin install repository-s3
multipass exec node-2 sudo systemctl restart elasticsearch

multipass exec node-3 sudo /usr/share/elasticsearch/bin/elasticsearch-plugin install repository-s3
multipass exec node-3 sudo systemctl restart elasticsearch
```





## 配置

The plugin provides a repository type named `s3` which may be used when creating a repository. 

The repository defaults to using [ECS IAM Role](https://docs.aws.amazon.com/AmazonECS/latest/developerguide/task-iam-roles.html) or [EC2 IAM Role](https://docs.aws.amazon.com/AWSEC2/latest/UserGuide/iam-roles-for-amazon-ec2.html) credentials for authentication. 

The only mandatory setting is the bucket name:

```console
PUT _snapshot/my_s3_repository
{
  "type": "s3",
  "settings": {
    "bucket": "my-bucket"
  }
}
```





### 客户端配置

1. 用于连接到S3的客户端具有许多可用设置。

2. The settings have the form `s3.client.CLIENT_NAME.SETTING_NAME`. 
3. By default, `s3` repositories use a client named `default`, but this can be modified using the [repository setting](https://www.elastic.co/guide/en/elasticsearch/plugins/7.13/repository-s3-repository.html) `client`. For example:

```
PUT _snapshot/my_s3_repository
{
  "type": "s3",
  "settings": {
    "bucket": "my-bucket",
    "client": "my-alternate-client"
  }
}
```



1. 大多数配置可以添加到 `elasticsearch.yml`，除了安全配置（他们被添加到Elasticsearch keystore ）

2. For more information about creating and updating the Elasticsearch keystore, see [Secure settings](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/secure-settings.html).



例如，如果要使用特定凭据访问S3，则运行以下命令将这些凭据添加到密钥库:

```
bin/elasticsearch-keystore add s3.client.default.access_key
bin/elasticsearch-keystore add s3.client.default.secret_key
# a session token is optional so the following command may not be needed
bin/elasticsearch-keystore add s3.client.default.session_token
```

相反，如果要使用实例角色或容器角色来访问S3，则应清空这些设置

可以通过 移除以下设置，来从特定凭据   切换回  instance role or container role 的默认凭据

```sh
bin/elasticsearch-keystore remove s3.client.default.access_key
bin/elasticsearch-keystore remove s3.client.default.secret_key
# a session token is optional so the following command may not be needed
bin/elasticsearch-keystore remove s3.client.default.session_token
```



1. **All** client secure settings of this plugin are [reloadable](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/secure-settings.html#reloadable-secure-settings). 
2. 当你手动 重载配置后。内部S3客户端也会重载
3. 正在进行的快照/还原任务不会被  客户端安全设置的重新加载所抢占。
4. 该任务将使用客户端完成，因为它是在操作开始时构建的。



以下列表包含可用的客户端设置，必须存储在密钥库中的配置，且被标记为 “安全” 可以重新加载; 其他设置属于elasticsearch.yml文件。

- **`access_key` ([Secure](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/secure-settings.html), [reloadable](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/secure-settings.html#reloadable-secure-settings))**

  An S3 access key. If set, the `secret_key` setting must also be specified. If unset, the client will use the instance or container role instead.

- **`secret_key` ([Secure](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/secure-settings.html), [reloadable](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/secure-settings.html#reloadable-secure-settings))**

  An S3 secret key. If set, the `access_key` setting must also be specified.

- **`session_token` ([Secure](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/secure-settings.html), [reloadable](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/secure-settings.html#reloadable-secure-settings))**

  An S3 session token. If set, the `access_key` and `secret_key` settings must also be specified.

- **`endpoint`**

  The S3 service endpoint to connect to. This defaults to `s3.amazonaws.com` but the [AWS documentation](https://docs.aws.amazon.com/general/latest/gr/rande.html#s3_region) lists alternative S3 endpoints. If you are using an [S3-compatible service](https://www.elastic.co/guide/en/elasticsearch/plugins/7.13/repository-s3-client.html#repository-s3-compatible-services) then you should set this to the service’s endpoint.

- **`protocol`**

  The protocol to use to connect to S3. Valid values are either `http` or `https`. Defaults to `https`.

- **`proxy.host`**

  The host name of a proxy to connect to S3 through.

- **`proxy.port`**

  The port of a proxy to connect to S3 through.

- **`proxy.username` ([Secure](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/secure-settings.html), [reloadable](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/secure-settings.html#reloadable-secure-settings))**

  The username to connect to the `proxy.host` with.

- **`proxy.password` ([Secure](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/secure-settings.html), [reloadable](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/secure-settings.html#reloadable-secure-settings))**

  The password to connect to the `proxy.host` with.

- **`read_timeout`**

  The socket timeout for connecting to S3. The value should specify the unit. For example, a value of `5s` specifies a 5 second timeout. The default value is 50 seconds.

- **`max_retries`**

  The number of retries to use when an S3 request fails. The default value is `3`.

- **`use_throttle_retries`**

  Whether retries should be throttled (i.e. should back off). Must be `true` or `false`. Defaults to `true`.

- **`path_style_access`**

  Whether to force the use of the path style access pattern. If `true`, the path style access pattern will be used. If `false`, the access pattern will be automatically determined by the AWS Java SDK (See [AWS documentation](https://docs.aws.amazon.com/AWSJavaSDK/latest/javadoc/com/amazonaws/services/s3/AmazonS3Builder.html#setPathStyleAccessEnabled-java.lang.Boolean-) for details). Defaults to `false`.



In versions `7.0`, `7.1`, `7.2` and `7.3` all bucket operations used the [now-deprecated](https://aws.amazon.com/blogs/aws/amazon-s3-path-deprecation-plan-the-rest-of-the-story/) path style access pattern. If your deployment requires the path style access pattern then you should set this setting to `true` when upgrading.

- **`disable_chunked_encoding`**

  Whether chunked encoding should be disabled or not. If `false`, chunked encoding is enabled and will be used where appropriate. If `true`, chunked encoding is disabled and will not be used, which may mean that snapshot operations consume more resources and take longer to complete. It should only be set to `true` if you are using a storage service that does not support chunked encoding. See the [AWS Java SDK documentation](https://docs.aws.amazon.com/AWSJavaSDK/latest/javadoc/com/amazonaws/services/s3/AmazonS3Builder.html#disableChunkedEncoding--) for details. Defaults to `false`.

- **`region`**

  Allows specifying the signing region to use. Specificing this setting manually should not be necessary for most use cases. Generally, the SDK will correctly guess the signing region to use. It should be considered an expert level setting to support S3-compatible APIs that require [v4 signatures](https://docs.aws.amazon.com/general/latest/gr/signature-version-4.html) and use a region other than the default `us-east-1`. Defaults to empty string which means that the SDK will try to automatically determine the correct signing region.

- **`signer_override`**

  Allows specifying the name of the signature algorithm to use for signing requests by the S3 client. Specifying this setting should not be necessary for most use cases. It should be considered an expert level setting to support S3-compatible APIs that do not support the signing algorithm that the SDK automatically determines for them. See the [AWS Java SDK documentation](https://docs.aws.amazon.com/AWSJavaSDK/latest/javadoc/com/amazonaws/ClientConfiguration.html#setSignerOverride-java.lang.String-) for details. Defaults to empty string which means that no signing algorithm override will be used.



### S3-compatible services



1. 有许多存储系统提供了S3-compatible的API
2.  `repository-s3` plugin 可以使 这些系统 与 AWS S3 开箱即用
3. 需要提供 `s3.client.CLIENT_NAME.endpoint`   
4. 也可以提供  `s3.client.CLIENT_NAME.protocol` 



**Minio兼容**

[Minio](https://minio.io/) is an example of a storage system that provides an S3-compatible API. The `repository-s3` plugin allows Elasticsearch to work with Minio-backed repositories as well as repositories stored on AWS S3. Other S3-compatible storage systems may also work with Elasticsearch, but these are not covered by the Elasticsearch test suite.



**完全的S3API兼容**

Note that some storage systems claim to be S3-compatible without correctly supporting the full S3 API. The `repository-s3` plugin requires full compatibility with S3. In particular it must support the same set of API endpoints, return the same errors in case of failures, and offer a consistency model no weaker than S3’s when accessed concurrently by multiple nodes. Incompatible error codes and consistency models may be particularly hard to track down since errors and consistency failures are usually rare and hard to reproduce.

**使用仓库分析来检查兼容性**

You can perform some basic checks of the suitability of your storage system using the [repository analysis API](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/repo-analysis-api.html). If this API does not complete successfully, or indicates poor performance, then your storage system is not fully compatible with AWS S3 and therefore unsuitable for use as a snapshot repository. You will need to work with the supplier of your storage system to address any incompatibilities you encounter.







### Repository Settings

```console
PUT _snapshot/my_s3_repository
{
  "type": "s3",
  "settings": {
    "bucket": "my-bucket",
    "another_setting": "setting-value"
  }
}
```

**`bucket`**

(Required) Name of the S3 bucket to use for snapshots.

The bucket name must adhere to Amazon’s [S3 bucket naming rules](https://docs.aws.amazon.com/AmazonS3/latest/dev/BucketRestrictions.html#bucketnamingrules).

**`client`**

The name of the [S3 client](https://www.elastic.co/guide/en/elasticsearch/plugins/7.13/repository-s3-client.html) to use to connect to S3. Defaults to `default`.

**`base_path`**

Specifies the path to the repository data within its bucket. Defaults to an empty string, meaning that the repository is at the root of the bucket. The value of this setting should not start or end with a `/`.

**`chunk_size`**

Big files can be broken down into chunks during snapshotting if needed. Specify the chunk size as a value and unit, for example: `1TB`, `1GB`, `10MB`. Defaults to the maximum size of a blob in the S3 which is `5TB`.

**`compress`**

When set to `true` metadata files are stored in compressed format. This setting doesn’t affect index files that are already compressed by default. Defaults to `false`.

**`max_restore_bytes_per_sec`**

Throttles per node restore rate. Defaults to unlimited. Note that restores are also throttled through [recovery settings](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/recovery.html).

**`max_snapshot_bytes_per_sec`**

Throttles per node snapshot rate. Defaults to `40mb` per second.

**`readonly`**

Makes repository read-only. Defaults to `false`.

**`server_side_encryption`**

When set to `true` files are encrypted on server side using AES256 algorithm. Defaults to `false`.

**`buffer_size`**

Minimum threshold below which the chunk is uploaded using a single request. Beyond this threshold, the S3 repository will use the [AWS Multipart Upload API](https://docs.aws.amazon.com/AmazonS3/latest/dev/uploadobjusingmpu.html) to split the chunk into several parts, each of `buffer_size` length, and to upload each part in its own request. Note that setting a buffer size lower than `5mb` is not allowed since it will prevent the use of the Multipart API and may result in upload errors. It is also not possible to set a buffer size greater than `5gb` as it is the maximum upload size allowed by S3. Defaults to `100mb` or `5%` of JVM heap, whichever is smaller.

**`canned_acl`**

The S3 repository supports all [S3 canned ACLs](https://docs.aws.amazon.com/AmazonS3/latest/dev/acl-overview.html#canned-acl) : `private`, `public-read`, `public-read-write`, `authenticated-read`, `log-delivery-write`, `bucket-owner-read`, `bucket-owner-full-control`. Defaults to `private`. You could specify a canned ACL using the `canned_acl` setting. When the S3 repository creates buckets and objects, it adds the canned ACL into the buckets and objects.

**`storage_class`**

Sets the S3 storage class for objects stored in the snapshot repository. Values may be `standard`, `reduced_redundancy`, `standard_ia`, `onezone_ia` and `intelligent_tiering`. Defaults to `standard`. Changing this setting on an existing repository only affects the storage class for newly created objects, resulting in a mixed usage of storage classes. Additionally, S3 Lifecycle Policies can be used to manage the storage class of existing objects. Due to the extra complexity with the Glacier class lifecycle, it is not currently supported by the plugin. For more information about the different classes, see [AWS Storage Classes Guide](https://docs.aws.amazon.com/AmazonS3/latest/dev/storage-class-intro.html)





1. 除了上述设置之外，您还可以在存储库设置中指定所有非安全客户端设置。
2. repository settings 优先级 大于 客户端设置

```
PUT _snapshot/my_s3_repository
{
  "type": "s3",
  "settings": {
    "client": "my-client",
    "bucket": "my-bucket",
    "endpoint": "my.s3.endpoint"
  }
}
```







#### Recommended S3 Permissions

1. 为了将Elasticsearch快照进程限制为所需的最低资源

2. 我们建议将Amazon IAM与预先存在的S3存储桶结合使用。
3. Here is an example policy which will allow the snapshot access to an S3 bucket named "snaps.example.com". This may be configured through the AWS IAM console, by creating a Custom Policy, and using a Policy Document similar to this (changing snaps.example.com to your bucket name).

```js
{
  "Statement": [
    {
      "Action": [
        "s3:ListBucket",
        "s3:GetBucketLocation",
        "s3:ListBucketMultipartUploads",
        "s3:ListBucketVersions"
      ],
      "Effect": "Allow",
      "Resource": [
        "arn:aws:s3:::snaps.example.com"
      ]
    },
    {
      "Action": [
        "s3:GetObject",
        "s3:PutObject",
        "s3:DeleteObject",
        "s3:AbortMultipartUpload",
        "s3:ListMultipartUploadParts"
      ],
      "Effect": "Allow",
      "Resource": [
        "arn:aws:s3:::snaps.example.com/*"
      ]
    }
  ],
  "Version": "2012-10-17"
}
```



```js
{
  "Statement": [
    {
      "Action": [
        "s3:ListBucket",
        "s3:GetBucketLocation",
        "s3:ListBucketMultipartUploads",
        "s3:ListBucketVersions"
      ],
      "Condition": {
        "StringLike": {
          "s3:prefix": [
            "foo/*"
          ]
        }
      },
      "Effect": "Allow",
      "Resource": [
        "arn:aws:s3:::snaps.example.com"
      ]
    },
    {
      "Action": [
        "s3:GetObject",
        "s3:PutObject",
        "s3:DeleteObject",
        "s3:AbortMultipartUpload",
        "s3:ListMultipartUploadParts"
      ],
      "Effect": "Allow",
      "Resource": [
        "arn:aws:s3:::snaps.example.com/foo/*"
      ]
    }
  ],
  "Version": "2012-10-17"
}
```



### AWS VPC Bandwidth Settings

AWS instances resolve S3 endpoints to a public IP. If the Elasticsearch instances reside in a private subnet in an AWS VPC then all traffic to S3 will go through the VPC’s NAT instance. If your VPC’s NAT instance is a smaller instance size (e.g. a t2.micro) or is handling a high volume of network traffic your bandwidth to S3 may be limited by that NAT instance’s networking bandwidth limitations. Instead we recommend creating a [VPC endpoint](https://docs.aws.amazon.com/vpc/latest/userguide/vpc-endpoints.html) that enables connecting to S3 in instances that reside in a private subnet in an AWS VPC. This will eliminate any limitations imposed by the network bandwidth of your VPC’s NAT instance.

Instances residing in a public subnet in an AWS VPC will connect to S3 via the VPC’s internet gateway and not be bandwidth limited by the VPC’s NAT instance.



