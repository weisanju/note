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
//  原理: 从高位到低位 取得 二进制的 值相加
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

        // 如果已取满 六位 二进制,则将该 低六位二进制对应的 值 转换成字符,并恢复标识
        if (i == 0)
        {
            i = 6;
            printf("%c", codeTable[sum]);
            sum = 0;
        }

        /*
            1. 判断当前字节 c, 当前处理 位置 j , 的二进制是 0 或 1
               (c >> (j - 1)
            2. 计算 6 -> 8 位转换时 位置j 所处的 权重 
                1 << (i - 1)
            3. 两者相乘
        */     
        sum += ((c >> (j - 1)) & 1) * (1 << (i - 1));
        i--;
        j--;
    }
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