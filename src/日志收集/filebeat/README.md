## 快速安装

### Step 1: Install Filebeat

```sh
curl -L -O https://artifacts.elastic.co/downloads/beats/filebeat/filebeat-8.1.1-darwin-x86_64.tar.gz
tar xzvf filebeat-8.1.1-darwin-x86_64.tar.gz
```

```sh
curl -L -O https://artifacts.elastic.co/downloads/beats/filebeat/filebeat-8.1.1-linux-x86_64.tar.gz
tar xzvf filebeat-8.1.1-linux-x86_64.tar.gz
```

### Step 2: Connect to the Elastic Stack

```yaml
output.elasticsearch:
  hosts: ["https://myEShost:9200"]
  username: "filebeat_internal"
  password: "YOUR_PASSWORD" 
  ssl:
    enabled: true
    ca_trusted_fingerprint: "b9a10bbe64ee9826abeda6546fc988c8bf798b41957c33d05db736716513dc9c" 
```

### Step 3: Collect log data

```sh
filebeat modules list
```

```
filebeat modules enable nginx
```

在*modules.d*下的模块配置中，启用所需的数据集并更改模块设置以匹配您的环境。**Datasets are disabled by default.**

例如，日志位置是根据操作系统设置的。如果您的日志不在默认位置，请设置路径变量:

```yaml
- module: nginx
  access:
    enabled: true
    var.paths: ["/var/log/nginx/access.log*"] 
```

To see the full list of variables for a module, see the documentation under [Modules](https://www.elastic.co/guide/en/beats/filebeat/current/filebeat-modules.html).

```
./filebeat test config -e
```

Make sure your config files are in the path expected by Filebeat (see [Directory layout](https://www.elastic.co/guide/en/beats/filebeat/current/directory-layout.html)), or use the `-c` flag to specify the path to the config file.



For more information about configuring Filebeat, also see:

- [Configure Filebeat](https://www.elastic.co/guide/en/beats/filebeat/current/configuring-howto-filebeat.html)
- [Config file format](https://www.elastic.co/guide/en/beats/libbeat/8.1/config-file-format.html)
- [`filebeat.reference.yml`](https://www.elastic.co/guide/en/beats/filebeat/current/filebeat-reference-yml.html): This reference configuration file shows all non-deprecated options. You’ll find it in the same location as `filebeat.yml`.



#### Enable and configure ECS loggers for application log collection

虽然Filebeat可用于摄取原始的纯文本应用程序日志，但我们建议您在摄取时结构化你的日志。这使您可以提取字段，例如日志级别和异常堆栈跟踪。

Elastic通过提供各种流行编程语言的应用程序日志格式化程序来简化此过程。这些插件将您的日志格式化为与ECS兼容的JSON，从而无需手动解析日志。

See [ECS loggers](https://www.elastic.co/guide/en/ecs-logging/overview/master/intro.html) to get started.

#### Configure Filebeat manually

 see [configure the input](https://www.elastic.co/guide/en/beats/filebeat/current/configuration-filebeat-options.html) manually.

### Step 4: Set up assets

Filebeat comes with predefined assets for parsing, indexing, and visualizing your data. To load these assets:

Filebeat带有预定义的assets，用于解析，索引和可视化数据。要加载这些assets:

```sh
filebeat setup -e
```

This step loads the recommended [index template](https://www.elastic.co/guide/en/elasticsearch/reference/8.1/index-templates.html) for writing to Elasticsearch and deploys the sample dashboards for visualizing the data in Kibana.

This step does not load the ingest pipelines used to parse log lines. By default, ingest pipelines are set up automatically the first time you run the module and connect to Elasticsearch.

此步骤不会加载用于解析日志行的摄取管道。默认情况下，第一次运行模块并连接到Elasticsearch时会自动设置ingest管道。

需要连接到Elasticsearch (或Elasticsearch服务) 才能设置初始环境。如果您使用的是其他输出，例如Logstash，请参阅:

- [Load the index template manually](https://www.elastic.co/guide/en/beats/filebeat/current/filebeat-template.html#load-template-manually)
- [*Load Kibana dashboards*](https://www.elastic.co/guide/en/beats/filebeat/current/load-kibana-dashboards.html)
- [*Load ingest pipelines*](https://www.elastic.co/guide/en/beats/filebeat/current/load-ingest-pipelines.html)

### Step 5: Start Filebeat

```sh
sudo chown root filebeat.yml 
sudo chown root modules.d/nginx.yml 
sudo ./filebeat -e
```

### Step 6: View your data in Kibana

