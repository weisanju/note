# Base64编码

## 原理

1. 将二进制数据编码成字符串
2. 操作说明
   1. 每6位二进制 当作一个 字节 的 低字节位, 高位补0
   2. 根据 转换来的字节的十进制 在 码表中找对应的字符
   3. 如果最后 还剩 4位二进制则, 则补 二个 '=' 号,如果还剩2位二进制则补一个'='

## 数据量比对

设源文件的字节数为  a


$$
\frac{编码后的数据量}{源数据量} =  \frac{a * 8 }{(a*8/6)*8} = \frac{4}{3}
$$




## 码表

| 码值 | 字符 | 码值 | 字符 | 码值 | 字符 |
| ---- | ---- | ---- | ---- | ---- | ---- |
| 0    | A    | 26   | a    | 52   | 0    |
| 1    | B    | 27   | b    | 53   | 1    |
| 2    | C    | 28   | c    | 54   | 2    |
| 3    | D    | 29   | d    | 55   | 3    |
| 4    | E    | 30   | e    | 56   | 4    |
| 5    | F    | 31   | f    | 57   | 5    |
| 6    | G    | 32   | g    | 58   | 6    |
| 7    | H    | 33   | h    | 59   | 7    |
| 8    | I    | 34   | i    | 60   | 8    |
| 9    | J    | 35   | j    | 61   | 9    |
| 10   | K    | 36   | k    | 62   | +    |
| 11   | L    | 37   | l    | 63   | /    |
| 12   | M    | 38   | m    |      |      |
| 13   | N    | 39   | n    |      |      |
| 14   | O    | 40   | o    |      |      |
| 15   | P    | 41   | p    |      |      |
| 16   | Q    | 42   | q    |      |      |
| 17   | R    | 43   | r    |      |      |
| 18   | S    | 44   | s    |      |      |
| 19   | T    | 45   | t    |      |      |
| 20   | U    | 46   | u    |      |      |
| 21   | V    | 47   | v    |      |      |
| 22   | W    | 48   | w    |      |      |
| 23   | X    | 49   | x    |      |      |
| 24   | Y    | 50   | y    |      |      |
| 25   | Z    | 51   | z    |      |      |



## 代码实现

```c
#include <stdio.h>
#include <string.h>
//编码
void encodeToBase64(const char *buf, int len);
//解码
void decodeBase64(const char *buf, int len);
void reverseBase64();

// 编码码表
char codeTable[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '+', '/'};

//解码码表
char reverseTable[255];


int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("缺少参数\n");
        return -1;
    }
    reverseBase64();
    switch (argv[1][0])
    {
        // d 为解码
    case 'd':
         decodeBase64(argv[2], strlen(argv[2]));
        break;
        //  e为编码
    case 'e':
         encodeToBase64(argv[2],strlen(argv[2]));
    default:
        break;
    }
    return 0;
}


void reverseBase64()
{
    for (size_t i = 0; i < sizeof(codeTable); i++)
    {
        reverseTable[codeTable[i]] = i;
    }
}
//  原理: 从高位到低位 每次取前六位二进制的 值相加
void doEncodeToBase64(const char *buf, int len)
{
    int i = 6;
    int j = 8;
    int k = 0;
    int rest = 0;
    int sum = 0;
    char c = buf[0];
    while (k < len)
    {
        // 如果当前字节的位数都已处理完毕,则切换到下一个字节,并恢复标识
        if (j == 0)
        {
            j = 8;
            c = buf[++k];
        }

        // 如果已取满 六位 二进制,则将该 低六位二进制对应的 值 根据码表转换成字符,并恢复标识
        if (i == 0)
        {
            i = 6;
            printf("%c", codeTable[sum]);
            sum = 0;
        }

        /*
            1. 判断当前字节 c, 当前处理 位置 j , 的二进制是 0 或 1
               (c >> (j - 1) & 1
            2. 计算 6 -> 8 位转换时 位置j 所处的 权重 
                1 << (i - 1)
            3. 两者相乘
        */     
        sum += ((c >> (j - 1)) & 1) * (1 << (i - 1));
        i--;
        j--;
    }
    // 补 = 号
    if ((rest = len % 3) == 1)
    {
        printf("%c==\n", codeTable[sum]);
    }
    else if (rest == 2)
    {
        printf("%c=\n", codeTable[sum]);
    }
}
// 解码原理 同上
void decodeBase64(const char *buf, int len)
{
    int i = 6;
    int j = 8;
    int k = 0;
    int rest = 0;
    int codeInt = reverseTable[buf[k]];
    int sum = 0;

    while (k < len && buf[k] != '=')
    {
        if (i == 0)
        {
            i = 6;
            codeInt = reverseTable[buf[++k]];
        }
        if (j == 0)
        {
            j = 8;
            printf("%c", sum);
            sum = 0;
        }

        sum += ((codeInt >> (i - 1)) & 1) * (1 << (j - 1));

        i--;
        j--;
    }
}

void encodeToBase64(const char *argv, int len)
{


    char buf[3072];
    FILE *file;
    if (file = fopen(argv, "rb"))
    {
        while ((len = fread(buf, sizeof(char), sizeof(buf), file)) > 0)
        {
            doEncodeToBase64(buf, len);
        }
    }
    else
    {
        // 当 输入的字符串 不为文件时, 直接当作字符串处理
        doEncodeToBase64(argv, len);
    }
}
```



