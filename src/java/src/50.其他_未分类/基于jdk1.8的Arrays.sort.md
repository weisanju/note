# 前言

* 本文基于 java1.8_251 版本的 Arrays.sort方法，关于整数数组排序的方法
* 方法签名为 *java.util.Arrays#sort(int[])*
* 其内部调用的 是 **DualPivotQuicksort.sort** 方法
* *Arrays.sort* 内部基于 多种排序算法
    * 当数组中的元素 基本有序时， 采用 **改进的归并排序**
    * 当数组的个数 较少，且基本有序，采用 传统的**插入排序** 否则 采用 **双元素插入排序**
    * 当数组中的元素 无序时 采用 **双枢纽 快速排序**



# DualPivotQuicksort.sort

## 流程图

![image-20210213195936815](..\..\images\dualPivotQuicksort_sort.png)

## code

```java
{
        // Use Quicksort on small arrays
        if (right - left < QUICKSORT_THRESHOLD) {
            sort(a, left, right, true); //小数组 采用 插入排序
            return;
        }

        /*
         * Index run[i] is the start of i-th run
         * (ascending or descending sequence).
         */
        int[] run = new int[MAX_RUN_COUNT + 1];
        int count = 0; run[0] = left;

        // Check if the array is nearly sorted
		......//判断数组是否是 基本有序的,如果基本有序 则直接 采用归并排序

        // Check special cases
        // Implementation note: variable "right" is increased by 1.
        if (run[count] == right++) { // The last run contains one element
            run[++count] = right;
        } else if (count == 1) { // The array is already sorted，已经有序
            return;
        }

...... //采用改进的归并排序
```





# 判断数组是否基本有序

## 如何判断

* 任意数组都可以看作 由  正序数列+逆序数列的 组合，故使用 `run[count]=k`，记录 第 count个数列的 结束索引的位置为K
* 当第 run数组的大小超过了 *MAX_RUN_LENGTH* 则认为 无序
*  当 *count* 为1则认为数组 已经排序好



## code

```java
        for (int k = left; k < right; run[count] = k) {
            if (a[k] < a[k + 1]) { // ascending
                while (++k <= right && a[k - 1] <= a[k]);
            } else if (a[k] > a[k + 1]) { // descending
                while (++k <= right && a[k - 1] >= a[k]);
                for (int lo = run[count] - 1, hi = k; ++lo < --hi; ) {
                    int t = a[lo]; a[lo] = a[hi]; a[hi] = t;
                }
            } else { // equal
                for (int m = MAX_RUN_LENGTH; ++k <= right && a[k - 1] == a[k]; ) {
                    if (--m == 0) {
                        sort(a, left, right, true);
                        return;
                    }
                }
            }

            /*
             * The array is not highly structured,
             * use Quicksort instead of merge sort.
             */
            if (++count == MAX_RUN_COUNT) {
                sort(a, left, right, true);
                return;
            }
        }


        // Check special cases
        // Implementation note: variable "right" is increased by 1.
        if (run[count] == right++) { // The last run contains one element
            run[++count] = right;
        } else if (count == 1) { // The array is already sorted，已经有序
            return;
        }
```



# 二路归并排序

## 原理

* 循环的每一轮迭代都将数组a中的两个相邻的单调子序列“归并”为一个新的单调子序列，并存储在数组b中。
* 循环中的每轮迭代将两个相邻的单调子序列进行归并，第一个子序列的范围是run[k-2] - run[k-1], 第二个子序列的范围是run[k-1] - run[k]，内层循环每次迭代都将k增加2。同时last变量记录了归并得到的新的子序列的个数，同时合并run 数组的内容。

**PS：**这里需要注意，如果a数组中的子序列个数是奇数，那么最后一个子序列就无法进行配对、归并操作，此时，直接将这个子序列复制到b中。 

* 每轮迭代以后，子序列的数目都会减少，因此，反复地进行迭代归并后，最终会使得整个数组只包含1个单调递增的子序列，此时整个数组排序完成。
* 因此，每轮迭代后，交换a、b指针，继续执行下一轮迭代时，同样对a数组进行归并，存储在b数组中。就这样，利用a和b代表的存储空间反复的进行归并，就可以完成数组的排序。107-108行的代码完成了交换a和b指针的工作

## code

```java
        // Determine alternation base for merge
        byte odd = 0;
        for (int n = 1; (n <<= 1) < count; odd ^= 1);

        // Use or create temporary array b for merging
        int[] b;                 // temp array; alternates with a
        int ao, bo;              // array offsets from 'left'
        int blen = right - left; // space needed for b
        if (work == null || workLen < blen || workBase + blen > work.length) {
            work = new int[blen];
            workBase = 0;
        }
        if (odd == 0) {
            System.arraycopy(a, left, work, workBase, blen);
            b = a;
            bo = 0;
            a = work;
            ao = workBase - left;
        } else {
            b = work;
            ao = 0;
            bo = workBase - left;
        }
		//以上是初始化的操作

        // Merging
        for (int last; count > 1; count = last) {
            for (int k = (last = 0) + 2; k <= count; k += 2) {
                int hi = run[k], mi = run[k - 1];
                for (int i = run[k - 2], p = i, q = mi; i < hi; ++i) {
                    if (q >= hi || p < mi && a[p + ao] <= a[q + ao]) {
                        b[i + bo] = a[p++ + ao];
                    } else {
                        b[i + bo] = a[q++ + ao];
                    }
                }
                run[++last] = hi;
            }
            if ((count & 1) != 0) {
                for (int i = right, lo = run[count - 1]; --i >= lo;
                    b[i + bo] = a[i + ao]
                );
                run[++last] = right;
            }
            int[] t = a; a = b; b = t;
            int o = ao; ao = bo; bo = o;
        }
    }
```





# 改进的快速排序

## **改进的快速排序整体流程图**

![image-20210213201130603](..\..\images\dualPivotQuicksort_quick_sort.png)

### 代码

```java
{
        int length = right - left + 1;

        // Use insertion sort on tiny arrays
        if (length < INSERTION_SORT_THRESHOLD) {
            if (leftmost) {
                ......//传统插入排序
            } else {
         		......//双元素插入排序
            }
            return;
        }

        ...... //计算双枢纽位置
            
		//五个点各不相等则 采用 双枢纽
        if (a[e1] != a[e2] && a[e2] != a[e3] && a[e3] != a[e4] && a[e4] != a[e5]) {
			......//双枢纽划分
            // Sort left and right parts recursively, excluding known pivots
            sort(a, left, less - 2, leftmost);
            sort(a, great + 2, right, false);

            /*
             * If center part is too large (comprises > 4/7 of the array),
             * swap internal pivot values to ends.
             */
            if (less < e1 && e5 < great) {
                ......//处理中间区间过长的问题
            }

            // Sort center part recursively
            sort(a, less, great, false);

        } else { // Partitioning with one pivot
            ......//单枢纽划分
            sort(a, left, less - 1, leftmost);
            sort(a, great + 1, right, false);
        }
    }
```



## **双枢纽排序**

### 流程图

![image-20210213202642614](..\..\images\dualPivotQuicksort_quick_dual_pivot_sort.png)



### 寻找双枢纽位置

> java 279~312

```java
        // Inexpensive approximation of length / 7 取差不多数组长度的1/7
        int seventh = (length >> 3) + (length >> 6) + 1;
        /*
         * Sort five evenly spaced elements around (and including) the
         * center element in the range. These elements will be used for
         * pivot selection as described below. The choice for spacing
         * these elements was empirically determined to work well on
         * a wide variety of inputs.
         * 从中间点 分别向 左右扩展 两个点，间距取大约数组长度的1/7，并将这五个点排序
         */
        int e3 = (left + right) >>> 1; // The midpoint
        int e2 = e3 - seventh;
        int e1 = e2 - seventh;
        int e4 = e3 + seventh;
        int e5 = e4 + seventh;

        // Sort these elements using insertion sort
        if (a[e2] < a[e1]) { int t = a[e2]; a[e2] = a[e1]; a[e1] = t; }

        if (a[e3] < a[e2]) { int t = a[e3]; a[e3] = a[e2]; a[e2] = t;
            if (t < a[e1]) { a[e2] = a[e1]; a[e1] = t; }
        }
        if (a[e4] < a[e3]) { int t = a[e4]; a[e4] = a[e3]; a[e3] = t;
            if (t < a[e2]) { a[e3] = a[e2]; a[e2] = t;
                if (t < a[e1]) { a[e2] = a[e1]; a[e1] = t; }
            }
        }
        if (a[e5] < a[e4]) { int t = a[e5]; a[e5] = a[e4]; a[e4] = t;
            if (t < a[e3]) { a[e4] = a[e3]; a[e3] = t;
                if (t < a[e2]) { a[e3] = a[e2]; a[e2] = t;
                    if (t < a[e1]) { a[e2] = a[e1]; a[e1] = t; }
                }
            }
        }
```

### 双枢纽划分

```java
// 判断五个点是否都 各不相同
if (a[e1] != a[e2] && a[e2] != a[e3] && a[e3] != a[e4] && a[e4] != a[e5]) {
    /*
     * Use the second and fourth of the five sorted elements as pivots.
     * These values are inexpensive approximations of the first and
     * second terciles of the array. Note that pivot1 <= pivot2.
     */
    int pivot1 = a[e2];
    int pivot2 = a[e4];

    /*
     * The first and the last elements to be sorted are moved to the
     * locations formerly occupied by the pivots. When partitioning
     * is complete, the pivots are swapped back into their final
     * positions, and excluded from subsequent sorting.
     */
    a[e2] = a[left];
    a[e4] = a[right];

    /*
     * Skip elements, which are less or greater than pivot values.
     */
    while (a[++less] < pivot1);
    while (a[--great] > pivot2);

    /*
     * Partitioning:
     *
     *   left part           center part                   right part
     * +--------------------------------------------------------------+
     * |  < pivot1  |  pivot1 <= && <= pivot2  |    ?    |  > pivot2  |
     * +--------------------------------------------------------------+
     *               ^                          ^       ^
     *               |                          |       |
     *              less                        k     great
     *
     * Invariants:
     *
     *              all in (left, less)   < pivot1
     *    pivot1 <= all in [less, k)     <= pivot2
     *              all in (great, right) > pivot2
     *
     * Pointer k is the first index of ?-part.
     */
    outer:
    for (int k = less - 1; ++k <= great; ) {
        int ak = a[k];
        if (ak < pivot1) { // Move a[k] to left part
            a[k] = a[less];
            /*
             * Here and below we use "a[i] = b; i++;" instead
             * of "a[i++] = b;" due to performance issue.
             */
            a[less] = ak;
            ++less;
        } else if (ak > pivot2) { // Move a[k] to right part
            while (a[great] > pivot2) {
                if (great-- == k) {
                    break outer;
                }
            }
            if (a[great] < pivot1) { // a[great] <= pivot2
                a[k] = a[less];
                a[less] = a[great];
                ++less;
            } else { // pivot1 <= a[great] <= pivot2
                a[k] = a[great];
            }
            /*
             * Here and below we use "a[i] = b; i--;" instead
             * of "a[i--] = b;" due to performance issue.
             */
            a[great] = ak;
            --great;
        }
    }

    // Swap pivots into their final positions
    a[left]  = a[less  - 1]; a[less  - 1] = pivot1;
    a[right] = a[great + 1]; a[great + 1] = pivot2;

    // Sort left and right parts recursively, excluding known pivots
    sort(a, left, less - 2, leftmost);
    sort(a, great + 2, right, false);

    /*
     * If center part is too large (comprises > 4/7 of the array),
     * swap internal pivot values to ends.
     */
    if (less < e1 && e5 < great) {
       ...... //处理 中间区间过大的问题
    }

    // Sort center part recursively
    sort(a, less, great, false);
```

### 单枢纽划分

```java
{ // Partitioning with one pivot
            /*
             * Use the third of the five sorted elements as pivot.
             * This value is inexpensive approximation of the median.
             */
            int pivot = a[e3];

            /*
             * Partitioning degenerates to the traditional 3-way
             * (or "Dutch National Flag") schema:
             *
             *   left part    center part              right part
             * +-------------------------------------------------+
             * |  < pivot  |   == pivot   |     ?    |  > pivot  |
             * +-------------------------------------------------+
             *              ^              ^        ^
             *              |              |        |
             *             less            k      great
             *
             * Invariants:
             *
             *   all in (left, less)   < pivot
             *   all in [less, k)     == pivot
             *   all in (great, right) > pivot
             *
             * Pointer k is the first index of ?-part.
             */
            for (int k = less; k <= great; ++k) {
                if (a[k] == pivot) {
                    continue;
                }
                int ak = a[k];
                if (ak < pivot) { // Move a[k] to left part
                    a[k] = a[less];
                    a[less] = ak;
                    ++less;
                } else { // a[k] > pivot - Move a[k] to right part
                    while (a[great] > pivot) {
                        --great;
                    }
                    if (a[great] < pivot) { // a[great] <= pivot
                        a[k] = a[less];
                        a[less] = a[great];
                        ++less;
                    } else { // a[great] == pivot
                        /*
                         * Even though a[great] equals to pivot, the
                         * assignment a[k] = pivot may be incorrect,
                         * if a[great] and pivot are floating-point
                         * zeros of different signs. Therefore in float
                         * and double sorting methods we have to use
                         * more accurate assignment a[k] = a[great].
                         */
                        a[k] = pivot;
                    }
                    a[great] = ak;
                    --great;
                }
            }

            /*
             * Sort left and right parts recursively.
             * All elements from center part are equal
             * and, therefore, already sorted.
             */
            sort(a, left, less - 1, leftmost);
            sort(a, great + 1, right, false);
        }
```



### 中间区间过大的问题

* 大于数组的 4/7 就认为 中间区间过大
* 将其中 等于 pivot1 pivot2的数据放到 privot1的最右边 或者 privot2的最左边

```java
            /*
             * If center part is too large (comprises > 4/7 of the array),
             * swap internal pivot values to ends.
             */
            if (less < e1 && e5 < great) {
                /*
                 * Skip elements, which are equal to pivot values.
                 */
                while (a[less] == pivot1) {
                    ++less;
                }

                while (a[great] == pivot2) {
                    --great;
                }

                /*
                 * Partitioning:
                 *
                 *   left part         center part                  right part
                 * +----------------------------------------------------------+
                 * | == pivot1 |  pivot1 < && < pivot2  |    ?    | == pivot2 |
                 * +----------------------------------------------------------+
                 *              ^                        ^       ^
                 *              |                        |       |
                 *             less                      k     great
                 *
                 * Invariants:
                 *
                 *              all in (*,  less) == pivot1
                 *     pivot1 < all in [less,  k)  < pivot2
                 *              all in (great, *) == pivot2
                 *
                 * Pointer k is the first index of ?-part.
                 */
                outer:
                for (int k = less - 1; ++k <= great; ) {
                    int ak = a[k];
                    if (ak == pivot1) { // Move a[k] to left part
                        a[k] = a[less];
                        a[less] = ak;
                        ++less;
                    } else if (ak == pivot2) { // Move a[k] to right part
                        while (a[great] == pivot2) {
                            if (great-- == k) {
                                break outer;
                            }
                        }
                        if (a[great] == pivot1) { // a[great] < pivot2
                            a[k] = a[less];
                            /*
                             * Even though a[great] equals to pivot1, the
                             * assignment a[less] = pivot1 may be incorrect,
                             * if a[great] and pivot1 are floating-point zeros
                             * of different signs. Therefore in float and
                             * double sorting methods we have to use more
                             * accurate assignment a[less] = a[great].
                             */
                            a[less] = pivot1;
                            ++less;
                        } else { // pivot1 < a[great] < pivot2
                            a[k] = a[great];
                        }
                        a[great] = ak;
                        --great;
                    }
                }
            }
```





## 传统插入排序

```java
                /*
                 * Traditional (without sentinel) insertion sort,
                 * optimized for server VM, is used in case of
                 * the leftmost part.
                 */
                for (int i = left, j = i; i < right; j = ++i) {
                    int ai = a[i + 1];
                    while (ai < a[j]) {
                        a[j + 1] = a[j];
                        if (j-- == left) {
                            break;
                        }
                    }
                    a[j + 1] = ai;
                }
```

## 双元素插入排序

```java
       /*
                 * Skip the longest ascending sequence.
                 */
                do {
                    if (left >= right) {
                        return;
                    }
                } while (a[++left] >= a[left - 1]);

                /*
                 * Every element from adjoining part plays the role
                 * of sentinel, therefore this allows us to avoid the
                 * left range check on each iteration. Moreover, we use
                 * the more optimized algorithm, so called pair insertion
                 * sort, which is faster (in the context of Quicksort)
                 * than traditional implementation of insertion sort.
                 */
                for (int k = left; ++left <= right; k = ++left) {
                    int a1 = a[k], a2 = a[left];

                    if (a1 < a2) {
                        a2 = a1; a1 = a[left];
                    }
                    while (a1 < a[--k]) {
                        a[k + 2] = a[k];
                    }
                    a[++k + 1] = a1;

                    while (a2 < a[--k]) {
                        a[k + 1] = a[k];
                    }
                    a[k + 1] = a2;
                }
                int last = a[right];

                while (last < a[--right]) {
                    a[right + 1] = a[right];
                }
                a[right + 1] = last;
```

