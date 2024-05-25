#include "../include/utf8.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

uint32_t codePoints[] = {20154, 38388, 22235, 26376, 22825, 44, 97, 98, 99, 100, 101, 46, 38543, 39118, 28508, 20837, 22812, 65292, 105, 122, 110, 107, 121, 12290, 0x1F600, 0x0644};
char _char[] = "人间四月天,abcde.随风潜入夜，iznky。😀ل";

void test_utf8CodePoint()
{
    int pointIdx = 0;
    int charIdx = 0;

    Utf8CodePoint info = {.codePoint = -1, .size = -1};
    do
    {
        if ('\0' == _char[charIdx])
        {
            break;
        }
        if (-1 == utf8CodePoint(&_char[charIdx], &info))
        {
            assert(0 == -1);
        }
        if (info.codePoint != codePoints[pointIdx])
        {
            printf("Failed at %d character. Expected : %ld, Actual : %ld\n", pointIdx, codePoints[pointIdx], info.codePoint);
        }

        charIdx += info.size;
        pointIdx++;
        info.codePoint = -1;
        info.size = -1;
    } while (1 == 1);
}

void test_utf8Char()
{
    int charNumber = sizeof(codePoints) / sizeof(uint32_t);
    Utf8CharInfo info = {.point = {'\0', '\0', '\0', '\0'}, .size = -1};
    char *buff = (char *)malloc(sizeof(codePoints) + 1);
    int buffIdx = 0;

    for (int i = 0; i < charNumber; i++)
    {
        if (-1 != utf8Char(codePoints[i], &info))
        {
            for (int j = 0; j < info.size; j++)
            {
                buff[buffIdx] = info.point[j];
                buffIdx++;
            }
        }
    }
    buff[buffIdx] = '\0';

    int _charSize = sizeof(_char);
    for (int i = 0; i < _charSize; i++)
    {
        if (buff[i] != _char[i])
        {
            printf("Failed at %d byte. Expected : %s, Actual : %s\n", i, &_char[i], &buff[i]);
        }
    }

    free(buff);
}

int main(int argc, char const *argv[])
{
    printf("hello world!\n");
    // test_utf8CodePoint();
    // test_utf8Char();
}
