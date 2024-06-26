#include "../include/utf8.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char *symbol;
} Word;

#define UTF8_CHINESE_SIZE 22000
#define UTF8_CHINESE_CODE_POINT_MIN 0X4E00
#define UTF8_CHINESE_CODE_POINT_MAX 0X9FA5

void setup_ts_mapping(uint32_t mapping[], char *path)
{
    char buff[128];
    int pointIdx = 0;
    int charIdx = 0;
    Utf8CodePoint info = {.codePoint = -1, .size = -1};

    FILE *fp = fopen(path, "r");
    while (fgets(buff, 1024, fp) != NULL)
    {
        pointIdx = 0;
        charIdx = 0;

        if (-1 == utf8CodePoint(&buff[charIdx], &info))
        {
            exit(1);
        }
        uint32_t t_code_point = info.codePoint;
        charIdx += info.size;
        pointIdx++;

        if (-1 == utf8CodePoint(&buff[charIdx], &info))
        {
            exit(1);
        }
        mapping[t_code_point % UTF8_CHINESE_SIZE] = info.codePoint;
    }
    fclose(fp);
}