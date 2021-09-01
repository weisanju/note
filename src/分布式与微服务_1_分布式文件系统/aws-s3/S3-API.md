# 创建连接

```java
String accessKey = "insert your access key here!";
String secretKey = "insert your secret key here!";

AWSCredentials credentials = new BasicAWSCredentials(accessKey, secretKey);
AmazonS3 conn = new AmazonS3Client(credentials);
conn.setEndpoint("objects.dreamhost.com");
```



# LISTING OWNED BUCKETS

> 这将获取您拥有的存储桶列表。这还会打印出每个存储桶的存储桶名称和创建日期。

```java
List<Bucket> buckets = conn.listBuckets();
for (Bucket bucket : buckets) {
        System.out.println(bucket.getName() + "\t" +
                StringUtils.fromDate(bucket.getCreationDate()));
}
```

# CREATING A BUCKET

```java
Bucket bucket = conn.createBucket("my-new-bucket");
```



# LISTING A BUCKET’S CONTENT¶



```java
ObjectListing objects = conn.listObjects(bucket.getName());
do {
        for (S3ObjectSummary objectSummary : objects.getObjectSummaries()) {
                System.out.println(objectSummary.getKey() + "\t" +
                        objectSummary.getSize() + "\t" +
                        StringUtils.fromDate(objectSummary.getLastModified()));
        }
        objects = conn.listNextBatchOfObjects(objects);
} while (objects.isTruncated());
```

# DELETING A BUCKET

```java
conn.deleteBucket(bucket.getName());
```

# CREATING AN OBJECT

```java
ByteArrayInputStream input = new ByteArrayInputStream("Hello World!".getBytes());
conn.putObject(bucket.getName(), "hello.txt", input, new ObjectMetadata());
```

# CHANGE AN OBJECT’S ACL

```java
conn.setObjectAcl(bucket.getName(), "hello.txt", CannedAccessControlList.PublicRead);
conn.setObjectAcl(bucket.getName(), "secret_plans.txt", CannedAccessControlList.Private)
```



# DOWNLOAD AN OBJECT (TO A FILE)

```java
conn.getObject(
        new GetObjectRequest(bucket.getName(), "perl_poetry.pdf"),
        new File("/home/larry/documents/perl_poetry.pdf")
);
```



# DELETE AN OBJECT

```java
conn.deleteObject(bucket.getName(), "goodbye.txt");
```



# GENERATE OBJECT DOWNLOAD URLS (SIGNED AND UNSIGNED)

这会为 hello.txt 生成一个未签名的下载 URL。这是有效的，因为我们通过设置上面的 ACL 公开了 hello.txt。然后，这将为 secret_plans.txt 生成一个签名的下载 URL，该 URL 将工作 1 小时。即使对象是私有的，签名的下载 URL 也将在该时间段内工作（当时间段结束时，该 URL 将停止工作）。

注意 java 库没有生成未签名 URL 的方法，因此下面的示例仅生成签名 URL。

```java
GeneratePresignedUrlRequest request = new GeneratePresignedUrlRequest(bucket.getName(), "secret_plans.txt");
System.out.println(conn.generatePresignedUrl(request));
```

**The output will look something like this:**

```
https://my-bucket-name.objects.dreamhost.com/secret_plans.txt?Signature=XXXXXXXXXXXXXXXXXXXXXXXXXXX&Expires=1316027075&AWSAccessKeyId=XXXXXXXXXXXXXXXXXXX
```



