## Mount snapshot API

Mount a snapshot as a searchable snapshot index.

### Request

```
POST /_snapshot/<repository>/<snapshot>/_mount
```

### Prerequisites

If the Elasticsearch security features are enabled, you must have the `manage` cluster privilege and the `manage` index privilege for any included indices to use this API. For more information, see [Security privileges](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/security-privileges.html).

### Description

### Path parameters

- **`<repository>`**

  (Required, string) The name of the repository containing the snapshot of the index to mount.

- **`<snapshot>`**

  (Required, string) The name of the snapshot of the index to mount.

### Query parameters

- **`master_timeout`**

  (Optional, [time units](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/common-options.html#time-units)) Period to wait for a connection to the master node. If no response is received before the timeout expires, the request fails and returns an error. Defaults to `30s`.

- **`wait_for_completion`**

  (Optional, Boolean) If `true`, the request blocks until the operation is complete. Defaults to `false`.

- **`storage`**

  (Optional, string) [Mount option](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/searchable-snapshots.html#searchable-snapshot-mount-storage-options) for the searchable snapshot index. Possible values are:**`full_copy` (Default)**[Fully mounted index](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/searchable-snapshots.html#fully-mounted).**`shared_cache`**[Partially mounted index](https://www.elastic.co/guide/en/elasticsearch/reference/7.13/searchable-snapshots.html#partially-mounted).

### Request body

- **`index`**

  (Required, string) Name of the index contained in the snapshot whose data is to be mounted.

If no `renamed_index` is specified this name will also be used to create the new index.

- **`renamed_index`**

  (Optional, string) Name of the index that will be created.

- **`index_settings`**

  (Optional, object) Settings that should be added to the index when it is mounted.

- **`ignore_index_settings`**

  (Optional, array of strings) Names of settings that should be removed from the index when it is mounted.

### Examples

Mounts the index `my_docs` from an existing snapshot named `my_snapshot` stored in the `my_repository` as a new index `docs`:

```console
POST /_snapshot/my_repository/my_snapshot/_mount?wait_for_completion=true
{
  "index": "my_docs", //快照中的索引
  "renamed_index": "docs", //重命名后的索引
  "index_settings": { //索引设置
    "index.number_of_replicas": 0
  },
  "ignore_index_settings": [ "index.refresh_interval" ]  //忽略的索引设置
}
```

