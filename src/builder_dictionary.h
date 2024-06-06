#include "../include/utf8.h"
#include <stdio.h>
#include <stdlib.h>

#define UTF8_CHINESE_SIZE 40900
#define UTF8_CHINESE_CODE_POINT_MIN 0X4E00
#define UTF8_CHINESE_CODE_POINT_MAX 0X9FA5
#define TRUE 1
#define FALSE 0

uint8_t is_skipable_word(uint32_t code_point)
{
    if (code_point < UTF8_CHINESE_CODE_POINT_MIN // 非中文字符
        || code_point == 65281                   // ！
        || code_point == 65292                   // ，
        || code_point == 65306                   // ：
        || code_point == 65307                   // ；
        || code_point == 65311                   // ？
    )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void save_to_file(uint32_t dic[], char *path)
{
    Utf8CharInfo info = {.point = {'\0', '\0', '\0', '\0'}, .size = -1};
    char buff[6];
    int buffIdx = 0;

    FILE *fp = fopen(path, "w");
    for (int i = 0; i < UTF8_CHINESE_SIZE; i++)
    {
        if (0 == dic[i])
        {
            continue;
        }

        buffIdx = 0;

        if (-1 == utf8Char(dic[i], &info))
        {
            exit(1);
        }
        for (int j = 0; j < info.size; j++)
        {
            buff[buffIdx++] = info.point[j];
        }

        buff[buffIdx++] = '\n';
        buff[buffIdx] = '\0';

        fputs(buff, fp);
    }
    fclose(fp);
}

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