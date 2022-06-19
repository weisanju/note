# Huffman编码

## 编码原理

1. huffman二叉树构造过程

   1. 由给定的m{1..m}个权值，构造m课二叉树,每个二叉树只有一个根节点,它的权值为m(i)
   2. 选取根结点的权值最小的两个结点，将他们作为左右子树，构造成一棵新的二叉树，它的根结点的权值置为其左、右子树根结点权值之和
   3. 重复执行步骤（2）最后得到Huffman树

2. Huffman二叉树的性质

   1. 为满二叉树(不存在度为1的结点) : 设叶子结点的个数为  n 则 Huffman树的总

      结点个数 = 2 * n  -1 

   2. 二叉树与字符编码对应关系
      1. `结点` 对应 文档中出现的所有字符
      2. `叶子结点路径`:从根据结点到叶子结点的路径,对应每个字符的编码,(由于到叶子结点的各个路径不同)
      3. `叶子结点路径长度`:从根结点到叶子结点的长度, 对应每个编码的 二进制位数
      4. `叶子结点带权路径长度`:编码后该篇文档中某个字符的数据量
      5. `树的带权路径长度`:编码后,改变文档总的数据量
   
3. 如下是 文本 `abbcccdddd`的Huffman树
   
      1. 图
      
         ![image-20200313200238710](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20200313200238710.png)
      
      2. 对应编码图
      
         |                                    | a    | b    | c    | d    |      |
         | ---------------------------------- | ---- | ---- | ---- | ---- | ---- |
         | 编码                               | 100  | 101  | 11   | 0    |      |
         | 路径长度                           | 3    | 3    | 2    | 1    |      |
         | 结点的权值                         | 1    | 2    | 3    | 4    |      |
         | 结点的带权路径长度                 | 3    | 6    | 6    | 4    |      |
         | 整个Huffman树的带权路径长度(bit位) |      |      |      |      | 19   |

   



## 编码步骤

### 编码 --以文本 "abbcccdddd"为例

1. 统计字符个数

   | a    | b    | c    | d    |
   | ---- | ---- | ---- | ---- |
   | 1    | 2    | 3    | 4    |

   

2. 依据字符出现的次数构建优先级队列

3. 依据优先级队列构建Huffman树

4. 遍历Huffman表 构建Huffman编码对照表

   | a    | b    | c    | d    |
   | ---- | ---- | ---- | ---- |
   | 100  | 101  | 11   | 0    |

5. 依据编码对照表,重新对字符串编码

   abbcccdddd ->  1001011011111110000

6. 将编码写入文件

   1. 写入表头
      1. 表头部分 =  编码对照表长度 + 编码对照表
   2. 写入数据

### 解码

1. 读入表头部分,形成Huffman编码
2. 将Huffman编码表 转换成 Huffman树 便于解析数据部分
3. 读入数据,依据Huffman树转换字符

## 数据量分析

1. 原数据量(bit数) = 10* 8 = 80 ;
2. 编码后数据量 = wpl值 = 19 ;
3. 理论压缩比率 = 19 / 80 ;
4. 实际压缩比率 = (19 + 表头部分数据) / 80



## C语言实现

1. 编码

   ```C
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   typedef struct __tag_binary_node
   {
       struct __tag_binary_node *right;
       struct __tag_binary_node *left;
       struct __tag_binary_node *next;
       int data;
       char c;
   } binary_node;
   
   typedef struct __tag_huffmanTable
   {
       int val;
       int start;
   } huffmanTable;
   
   //[字符 - 字符出现次数]  对照表
   int charTable[255] = {0};
   // [字符 - 对应bit编码表 ]
   huffmanTable *htable[255] = {0};
   
   binary_node *new_binary_node(int data, char c)
   {
       binary_node *node = (binary_node *)malloc(sizeof(binary_node));
       node->data = data;
       node->right = NULL;
       node->left = NULL;
       node->next = NULL;
       node->c = c;
       return node;
   }
   
   void priority_enqueue(binary_node **head, binary_node *node)
   {
       if (head == NULL)
       {
           return;
       }
   
       binary_node *tmpnode = *head;
   
       if (*head != NULL)
       {
           if (node->data - tmpnode->data > 0)
           {
               while (tmpnode->next && node->data - tmpnode->next->data > 0)
               {
                   tmpnode = tmpnode->next;
               };
               node->next = tmpnode->next;
               tmpnode->next = node;
           }
           else
           {
               node->next = tmpnode;
               *head = node;
           }
       }
       else
       {
           *head = node;
       }
   }
   
   binary_node *priority_dequeue(binary_node **head)
   {
       if (*head == NULL)
       {
           return NULL;
       }
       binary_node *node = *head;
       *head = (*head)->next;
       return node;
   }
   
   void countChar(FILE *f)
   {
       char buf[1024];
       int len;
       while ((len = fread(buf, sizeof(char), sizeof(buf), f)) > 0)
       {
           for (size_t i = 0; i < len; i++)
           {
               charTable[buf[i]]++;
           }
       }
   }
   //构造Huffman树
   void buildPriorityQueue(binary_node **head)
   {
       for (size_t i = 0; i < sizeof(charTable) / sizeof(int); i++)
       {
           if (charTable[i])
           {
               binary_node *node = new_binary_node(charTable[i], i);
               priority_enqueue(head, node);
           }
       }
   }
   
   //根据 优先级队列转换 成 huaffmantree
   binary_node *buildHuffman(binary_node **head)
   {
       binary_node *tmpnode1 = NULL;
       binary_node *tmpnode2 = NULL;
       while (*head)
       {
           tmpnode1 = priority_dequeue(head);
           tmpnode2 = priority_dequeue(head);
           if (tmpnode2 != NULL)
           {
               binary_node *node = new_binary_node(tmpnode1->data + tmpnode2->data, 0);
               node->left = tmpnode1;
               node->right = tmpnode2;
               priority_enqueue(head, node);
           }
       }
       return tmpnode1;
   }
   
   //按习惯, 字节都是从高位 开始写入,所以 逆序转换
   int reverseByte(int sum, int start)
   {
       int count = 0;
       for (size_t i = 0; i < start; i++)
       {
           count += ((sum >> i) & 1) * (1 << start - i - 1);
       }
       return count;
   }
   // 根据 Huffman树 构建Huffman编码对照表, sum计算每次走过路径的值, i表示路径长度, wpl = 所有叶子结点的权值 * 路径长度
   int tohuffManTable(binary_node *node, int i, int sum)
   {
       int wpl = 0;
       if (node->left != NULL)
       {
           wpl += tohuffManTable(node->left, i + 1, sum);
       }
   
       if (node->right != NULL)
       {
           wpl += tohuffManTable(node->right, i + 1, sum + (1 << i));
       }
   
       if (node->left == NULL && node->right == NULL)
       {
   
           htable[node->c] = (huffmanTable *)malloc(sizeof(huffmanTable));
           htable[node->c]->val = reverseByte(sum, i);
           htable[node->c]->start = i;
           wpl += node->data * i;
       }
       return wpl;
   }
   /* 
       编码文件的格式  = 表头 + 编码后的数据
       表头 =  出现字符的个数 (int)  +  wpl(int)  +  {字符:Huffman编码对照表}
   */
   void encodeFile(FILE *src, int wpl, FILE *f)
   {
       int size_metadata = 0;
       int size_data = 0;
       int size_real = 0;
       //插入表头
       //统计字符的个数
       int tablesize = 0;
       for (size_t i = 0; i < 255; i++)
       {
           if (htable[i])
           {
               tablesize++;
           }
       }
       //出现字符的个数 (int)
       fwrite(&tablesize, sizeof(int), 1, f);
       size_metadata += sizeof(int);
       //wpl
       fwrite(&wpl, sizeof(int), 1, f);
       size_metadata += sizeof(int);
   
       for (size_t i = 0; i < 255; i++)
       {
           if (htable[i])
           {
               //该字符
               fwrite(&i, sizeof(char), 1, f);
               size_metadata += sizeof(char);
               //该字符对应的Huffman编码
               fwrite(htable[i], sizeof(huffmanTable), 1, f);
               size_metadata += sizeof(huffmanTable);
           }
       }
   
       unsigned int sum = 0, k = 0;
       char buf[1024];
   
       // 初始化
       int len = fread(buf, sizeof(char), sizeof(buf), src);
       size_real += len;
       huffmanTable *t = htable[buf[0]];
       int start = t->start;
       int toLen = sizeof(int) * 8;
   
       while (1)
       {
           if (toLen == 0)
           {
               //如果 toLen,32 位 都已填充完则,写入sum到文件, 并重置 sum,toLen
               toLen = sizeof(int) * 8;
               fwrite(&sum, sizeof(sum), 1, f);
               size_data += sizeof(int);
               sum = 0;
           }
   
           if (start == 0)
           {
               //若某个字符 已写完,则K++, 更换下一个字符
               ++k;
               //若buf缓冲区已读完,则继续从文件中读下一个
               if (k == len)
               {
                   //若文件已读完则跳出循环,while循环唯一出口
                   if ((len = fread(buf, sizeof(char), sizeof(buf), src)) <= 0)
                   {
                       break;
                   }
                   //缓冲区读入之后,重置k
                   k = 0;
                   size_real += len;
               }
               //重置 t,start
               t = htable[buf[k]];
               start = t->start;
           }
           /*
               32位无符号整型中对应的权重: 1 << toLen - 1
               判断编码 在start位的 0或1: (t->val >> start - 1) & 1 
           */
           sum += (1 << toLen - 1) * ((t->val >> start - 1) & 1);
           toLen--;
           start--;
       }
       // 退出后 还剩最后一个没写完
       fwrite(&sum, sizeof(sum), 1, f);
       size_data += sizeof(int);
   
       printf("元数据:%d,编码数据:%d,真实数据:%d,理论压缩率%.2f,实际压缩率%.2f\n", size_metadata, size_data, size_real, size_data * 1.0 / size_real, (size_data + size_metadata) * 1.0 / size_real);
   }
   
   int main(int argc, char const *argv[])
   {
       if (argc != 3)
       {
           printf("usage: %s srcFilePath  destFilePath", argv[0]);
           return 1;
       }
       FILE *newfile = fopen(argv[1], "r");
       if (newfile == NULL)
       {
           printf("源文件不存在,或者目标路径不可用\n");
           return 1;
       }
       FILE *f = fopen(argv[2], "wb");
       // 统计字符出现个数
       countChar(newfile);
       rewind(newfile);
   
       //构建优先级队列
       binary_node *head = NULL;
       buildPriorityQueue(&head);
   
       //构建Huffman树
       binary_node *rootNode = buildHuffman(&head);
   
       //遍历Huffman树形成 编码表,并求出 wpl(weighted path length),即对应 编码后的 bit数
       int wpl = tohuffManTable(rootNode, 0, 0);
       //求出remain
       int remain = (sizeof(int) * 8 - wpl % (sizeof(int) * 8));
   
       //编码文件
       encodeFile(newfile, wpl, f);
   
       return 0;
   }
   ```

2. 解码

   ```C
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   typedef struct __tag_binary_node
   {
       struct __tag_binary_node *right;
       struct __tag_binary_node *left;
       struct __tag_binary_node *next;
       int data;
       char c;
   } binary_node;
   
   typedef struct __tag_huffmanTable
   {
       int val;
       int start;
   } huffmanTable;
   
   binary_node *new_binary_node(int data, char c)
   {
       binary_node *node = (binary_node *)malloc(sizeof(binary_node));
       node->data = data;
       node->right = NULL;
       node->left = NULL;
       node->next = NULL;
       node->c = c;
       return node;
   }
   
   
   
   void decodeTable()
   {
       FILE *f = fopen("C:\\Users\\Administrator\\Documents\\Tencent Files\\1259103745\\FileRecv\\b.txt", "rb");
   
       fseek(f, 0, SEEK_END);
       long size = ftell(f);
       fseek(f, 0, SEEK_SET);
   
       //读 tableSize
       int tableSize = 0;
       fread(&tableSize, sizeof(int), 1, f);
       size -= sizeof(int);
       //读 wpl
       int wpl = 0;
       fread(&wpl, sizeof(int), 1, f);
       size -= sizeof(int);
       int remain = 32 - wpl % 32;
       printf("wpl=%d,remain=%d", wpl, remain);
       char ch = 0;
       //读Huffmantable,并生成Huffman树
       binary_node *head = new_binary_node(0, 0), *tmpNode = head;
       int start;
       for (size_t i = 0; i < tableSize; i++)
       {
           huffmanTable *t = (huffmanTable *)malloc(sizeof(huffmanTable));
           fread(&ch, sizeof(char), 1, f);
           size -= sizeof(char);
           fread(t, sizeof(huffmanTable), 1, f);
           size -= sizeof(huffmanTable);
           start = t->start;
           while (--start >= 0)
           {
               if (t->val & (1 << start))
               {
                   if (tmpNode->right == NULL)
                   {
                       tmpNode->right = new_binary_node(0, 0);
                       if (start == 0)
                       {
                           tmpNode->right->c = ch;
                       }
                   }
                   tmpNode = tmpNode->right;
               }
               else
               {
                   if (tmpNode->left == NULL)
                   {
                       tmpNode->left = new_binary_node(0, 0);
                       if (start == 0)
                       {
                           tmpNode->left->c = ch;
                       }
                   }
                   tmpNode = tmpNode->left;
               }
           }
           tmpNode = head;
       }
       unsigned int read;
       long alreadyRead = 0;
       int toLen = 0;
       do
       {
           if (toLen == 0)
           {
               toLen = sizeof(int) * 8;
               fread(&read, sizeof(int), 1, f);
               alreadyRead += sizeof(int);
               if (alreadyRead == size)
               {
                   toLen -= remain;
                   read >>= remain;
               }
           }
           if (tmpNode->right == NULL && tmpNode->left == NULL)
           {
               putchar(tmpNode->c);
               tmpNode = head;
           }
           if (read & (1 << toLen - 1))
           {
               tmpNode = tmpNode->right;
           }
           else
           {
               tmpNode = tmpNode->left;
           }
           toLen--;
       } while (alreadyRead < size || toLen != 0);
       putchar(tmpNode->c);
   }
   
   int main(int argc, char const *argv[])
   {
   
       //解码文字
       decodeTable();
       return 0;
   }
   ```

   

## 写在最后

1. 编码的难点在于 位与 字符间 的切换
2. 目前只支持 ASCII编码的字符
3. 写入表头部分格式 是本人自定义的
4. 本篇博客的目的供自己学习参考用,如有错误 欢迎批评指正

