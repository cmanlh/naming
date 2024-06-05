/*
 * 用于维护繁转简的字典文件
 */
#include "../include/utf8.h"
#include <stdio.h>
#include <stdlib.h>

#define UTF8_CHINESE_SIZE 22000

static void _setup_mapping(uint32_t index[], uint32_t mapping[], char *path)
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
        uint32_t s_code_point = info.codePoint;

        int idx = t_code_point % UTF8_CHINESE_SIZE;
        index[idx] = t_code_point;
        mapping[idx] = s_code_point;
    }
    fclose(fp);
}

static void _update_mapping(uint32_t index[], uint32_t mapping[], uint32_t t_code_point, uint32_t s_code_point)
{
    if (t_code_point == s_code_point)
    {
        return;
    }

    int idx = t_code_point % UTF8_CHINESE_SIZE;
    if (0 == index[idx])
    {
        index[idx] = t_code_point;
        mapping[idx] = s_code_point;
    }
}

#define TRUE 1
#define FALSE 0
static void _parse_mapping_fix(uint32_t index[], uint32_t mapping[], char *path, uint8_t with_delimiter)
{
    char buff[1024];
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

        if (TRUE == with_delimiter)
        {
            if (-1 == utf8CodePoint(&buff[charIdx], &info))
            {
                exit(1);
            }
            charIdx += info.size;
            pointIdx++;
        }

        if (-1 == utf8CodePoint(&buff[charIdx], &info))
        {
            exit(1);
        }
        uint32_t s_code_point = info.codePoint;

        _update_mapping(index, mapping, t_code_point, s_code_point);
    }
    fclose(fp);
}

static void _save_to_file(uint32_t index[], uint32_t mapping[], char *path)
{
    Utf8CharInfo info = {.point = {'\0', '\0', '\0', '\0'}, .size = -1};
    char buff[10];
    int buffIdx = 0;

    FILE *fp = fopen(path, "w");
    for (int i = 0; i < UTF8_CHINESE_SIZE; i++)
    {
        if (0 == index[i])
        {
            continue;
        }

        buffIdx = 0;

        if (-1 == utf8Char(index[i], &info))
        {
            exit(1);
        }
        for (int j = 0; j < info.size; j++)
        {
            buff[buffIdx++] = info.point[j];
        }

        if (-1 == utf8Char(mapping[i], &info))
        {
            exit(1);
        }
        for (int j = 0; j < info.size; j++)
        {
            buff[buffIdx] = info.point[j];
            buffIdx++;
        }

        buff[buffIdx++] = '\n';
        buff[buffIdx] = '\0';

        fputs(buff, fp);
    }
    fclose(fp);
}

int main(int argc, char const *argv[])
{
    uint32_t index[UTF8_CHINESE_SIZE] = {0};
    uint32_t mapping[UTF8_CHINESE_SIZE] = {0};

    _setup_mapping(index, mapping, "C:\\codespace\\naming\\ts.txt");
    _parse_mapping_fix(index, mapping, "C:\\Users\\luhong\\Downloads\\ts.txt", TRUE);
    _save_to_file(index, mapping, "C:\\codespace\\naming\\ts_new.txt");
    return 0;
}
