#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFLEN 200

int main(int argc, char **argv)
{
    if (argc < 6)
    {
        printf("参数错误，参数列表：outputpath targetsize automode [position path ...]\n");
        printf("例子：启用引导有效位自动填充，输出到1.img\nbootimg-patch.exe ./1.img 1474560 1 0 ./boot.com 4096 ./process.com");
        getchar();
        return 0;
    }
    char *outputpath = argv[1];

    int targetsize = atoi(argv[2]), automode = atoi(argv[3]);

    printf(automode ? "自动配置开启，将会在512字节前填充0x55 0xAA\n" : "自动配置已关闭\n");

    // fill 11
    FILE *output = fopen(outputpath, "wb");
    if (output == NULL)
    {
        printf("open %s error!\n", outputpath);
        getchar();
        return 1;
    }
    char fill = 17;
    for (int i = 0; i < targetsize; i++)
    {
        fwrite(&fill, 1, 1, output);
    }
    fclose(output);
    printf("创建空软盘成功(大小%d)\n", targetsize);

    // insert custom file
    for (int i = 4; i < argc; i += 2)
    {
        int position = atoi(argv[i]);
        char *inputpath = argv[i + 1];
        FILE *input = fopen(inputpath, "rb");
        FILE *output = fopen(outputpath, "rb+");
        if (output == NULL)
        {
            printf("open %s error!\n", outputpath);
            getchar();
            return 1;
        }
        if (-1 == (fseek(output, position, SEEK_SET)))
        {
            printf("seek error\n");
            getchar();
            return 1;
        }

        char buf[BUFLEN + 1];
        int bufcnt;
        while ((bufcnt = fread(buf, 1, BUFLEN, input)) == BUFLEN)
        {
            fwrite(buf, BUFLEN, 1, output);
        }
        fwrite(buf, bufcnt, 1, output);

        fclose(input);
        fclose(output);
    }

    // 写入有效标志
    if (automode)
    {
        if (targetsize < 512)
        {
            printf("输出大小小于512，无法写入\n");
            return 1;
        }
        FILE *output = fopen(outputpath, "rb+");
        if (output == NULL)
        {
            printf("open %s error!\n", outputpath);
            getchar();
            return 1;
        }
        if (-1 == (fseek(output, 510, SEEK_SET)))
        {
            printf("seek error\n");
            return 1;
        }
        char temp[3];
        temp[0] = 85;
        temp[1] = 170;
        temp[2] = 0;
        fwrite(temp, 2, 1, output);
        fclose(output);
        printf("写入有效标志成功\n");
    }
    printf("按任意键继续\n");
    getchar();
    return 0;
}