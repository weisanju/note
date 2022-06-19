# CFS Bandwidth Control

本文档仅讨论 SCHED_NORMAL 的 CPU 带宽控制。 

CFS 带宽控制是一个 CONFIG_FAIR_GROUP_SCHED 扩展，它允许指定组或层次结构可用的最大 CPU 带宽

组允许的带宽是使用 配额和周期 (quota and period) 指定的。

在每个给定的“周期”（微秒）内，一个组最多只能消耗“配额”微秒的 CPU 时间。

当某个组的 CPU 带宽消耗超过此限制（在该时间段内）时，属于其层次结构的任务将受到限制，并且在下一个时间段之前不允许再次运行。

一个组的未使用运行时间被全局跟踪，在每个周期边界用上述配额单位进行刷新。

当线程消耗此带宽时，它会根据需要传输到 CPU 本地“筒仓”。

在这些更新中的每一个中传输的数量都是可调的，并被描述为“切片”。



# Management

通过 cgroupfs 在 cpu 子系统内管理配额和周期。

`cpu.cfs_quota_us`：一个周期内的总可用运行时间（以微秒为单位） 

`cpu.cfs_period_us`：一个周期的长度（以微秒为单位） 

`cpu.stat`：导出  throttling  统计信息 [下面进一步解释]

```
The default values are:
	cpu.cfs_period_us=100ms
	cpu.cfs_quota=-1
```

cpu.cfs_quota_us 的值为 -1 表示该组没有任何带宽限制，这样的组被描述为无约束带宽组。



写入任何（有效）正值将制定指定的带宽限制。

配额或周期允许的最小配额为 1 毫秒。 

1s 的周期长度也有一个上限。

当以分层方式使用带宽限制时，存在其他限制，下面将更详细地解释这些限制。

向 cpu.cfs_quota_us 写入任何负值将取消带宽限制并使组再次返回到不受约束的状态。

如果组处于受限状态，则对组带宽规范的任何更新都将导致其不受限制。







# System wide settings

效率运行时间以批处理方式在全局池和 CPU 本地“筒仓”之间传输。

这大大减轻了大型系统的全局统计压力。

这可以通过 procfs 进行调整：/proc/sys/kernel/sched_cfs_bandwidth_slice_us（默认值=5ms）

较大的切片值将减少传输开销，而较小的值允许更细粒度的消费。



# Statistics

组的带宽统计通过 cpu.stat 中的 3 个字段导出。

```
cpu.stat:
- nr_periods:已过去的强制执行间隔数。
- nr_throttled:组被节流/限制的次数。
- throttled_time: 组的实体受到限制的总持续时间（以纳秒为单位）。
```

该接口是只读的。



# 层级考虑

```
[ Where C is the parent's bandwidth, and c_i its children ]
```

该接口强制要求始终可以获得单个实体的带宽，即：max(c_i) <= C。但是，明确允许聚合情况下的超额订阅以在层次结构中启用工作节约语义。

**组在以下两种情况可能会被限制**

a. 它在一段时间内完全消耗了自己的配额
b. 父母的配额在其期限内被完全消耗

即使子级可能还有剩余的运行时，在父级的运行时更新 之前也不允许这样做。



# Examples

```
1. Limit a group to 1 CPU worth of runtime.

	If period is 250ms and quota is also 250ms, the group will get
	1 CPU worth of runtime every 250ms.

	# echo 250000 > cpu.cfs_quota_us /* quota = 250ms */
	# echo 250000 > cpu.cfs_period_us /* period = 250ms */

2. Limit a group to 2 CPUs worth of runtime on a multi-CPU machine.

	With 500ms period and 1000ms quota, the group can get 2 CPUs worth of
	runtime every 500ms.

	# echo 1000000 > cpu.cfs_quota_us /* quota = 1000ms */
	# echo 500000 > cpu.cfs_period_us /* period = 500ms */

	The larger period here allows for increased burst capacity.

3. Limit a group to 20% of 1 CPU.

	With 50ms period, 10ms quota will be equivalent to 20% of 1 CPU.

	# echo 10000 > cpu.cfs_quota_us /* quota = 10ms */
	# echo 50000 > cpu.cfs_period_us /* period = 50ms */

	By using a small period here we are ensuring a consistent latency
	response at the expense of burst capacity.
	
通过在这里使用一小段时间，我们以牺牲突发容量为代价确保一致的延迟响应。
```

**周期越长 响应延时越高，但总的吞吐量越大**



翻译自

[CFS BandWithControl](https://www.kernel.org/doc/Documentation/scheduler/sched-bwc.txt)

