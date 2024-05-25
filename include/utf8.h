#include <stdint.h>
#include <wchar.h>

typedef struct
{
    uint32_t codePoint;
    uint8_t size;
} Utf8CodePoint;

typedef struct
{
    uint8_t point[4];
    uint8_t size;
} Utf8CharInfo;

int8_t utf8CodePoint(const char utf8[], Utf8CodePoint *info);

int8_t utf8Char(const uint32_t codePoint, Utf8CharInfo *info);