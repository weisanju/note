### 物理节点

```java
package org.apache.rocketmq.common.consistenthash;

/**
 * Represent a node which should be mapped to a hash ring
 */
public interface Node {
    /**
     * @return the key which will be used for hash mapping
     */
    String getKey();
}

```

### 虚拟节点

```java
package org.apache.rocketmq.common.consistenthash;

public class VirtualNode<T extends Node> implements Node {
    final T physicalNode;
    final int replicaIndex;

    public VirtualNode(T physicalNode, int replicaIndex) {
        this.replicaIndex = replicaIndex;
        this.physicalNode = physicalNode;
    }

    @Override
    public String getKey() {
        return physicalNode.getKey() + "-" + replicaIndex;
    }

    public boolean isVirtualNodeOf(T pNode) {
        return physicalNode.getKey().equals(pNode.getKey());
    }

    public T getPhysicalNode() {
        return physicalNode;
    }
}

```

### MQ节点

```java
private static class ClientNode implements Node {
  private final String clientID;

  public ClientNode(String clientID) {
    this.clientID = clientID;
  }

  @Override
  public String getKey() {
    return clientID;
  }
}
```

### 环对象

**利用 sortedMap 实现的 Hash环**

```java
private final SortedMap<Long, VirtualNode<T>> ring = new TreeMap<Long, VirtualNode<T>>();
```

### ConsistentHashRouter

#### 添加节点

```java
//org.apache.rocketmq.common.consistenthash.ConsistentHashRouter#addNode

public void addNode(T pNode, int vNodeCount) {
  if (vNodeCount < 0)
    throw new IllegalArgumentException("illegal virtual node counts :" + vNodeCount);
  //先判断 是否之前有加入过环，如果有则 从上次的虚拟节点开始
  int existingReplicas = getExistingReplicas(pNode);
  for (int i = 0; i < vNodeCount; i++) {
    VirtualNode<T> vNode = new VirtualNode<T>(pNode, i + existingReplicas);
    ring.put(hashFunction.hash(vNode.getKey()), vNode);
  }
}
```

#### 移除节点

**移除物理节点对应的所有虚拟节点**

```java
public void removeNode(T pNode) {
  Iterator<Long> it = ring.keySet().iterator();
  while (it.hasNext()) {
    Long key = it.next();
    VirtualNode<T> virtualNode = ring.get(key);
    if (virtualNode.isVirtualNodeOf(pNode)) {
      it.remove();
    }
  }
}
```

#### 路由

```java
public T routeNode(String objectKey) {
  if (ring.isEmpty()) {
    return null;
  }
  Long hashVal = hashFunction.hash(objectKey);
  SortedMap<Long, VirtualNode<T>> tailMap = ring.tailMap(hashVal);
  Long nodeHashVal = !tailMap.isEmpty() ? tailMap.firstKey() : ring.firstKey();
  return ring.get(nodeHashVal).getPhysicalNode();
}
```

