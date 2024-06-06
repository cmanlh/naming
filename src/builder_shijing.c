#include "../include/yyjson.h"
#include <wchar.h>
#include <sys/stat.h>
#include "builder_dictionary.h"

static void _build_dic(uint32_t ts_mapping[], uint32_t dic[], const char *words)
{
    int charIdx = 0;

    Utf8CodePoint info = {.codePoint = -1, .size = -1};
    while (words[charIdx] != '\0')
    {
        if (-1 == utf8CodePoint(&words[charIdx], &info))
        {
            exit(1);
        }
        charIdx += info.size;

        if (is_skipable_word(info.codePoint) == TRUE)
        {
            continue;
        }

        int index = info.codePoint % UTF8_CHINESE_SIZE;
        uint32_t s_code_point = ts_mapping[index];
        if (s_code_point == 0)
        {
            dic[index] = info.codePoint;
        }
        else
        {
            dic[index] = ts_mapping[index];
        }
    };
}

static void _parse(uint32_t ts_mapping[], uint32_t dic[], wchar_t *path)
{
    struct stat fileStat = {};
    wstat(path, &fileStat);

    FILE *fp = _wfopen(path, L"r");
    char json[fileStat.st_size + 1];
    int jsonCounter = 0;
    char buff[2048];
    while (fgets(buff, 2048, fp) != NULL)
    {
        int i = 0;
        while (true)
        {
            if ('\0' == buff[i])
            {
                break;
            }

            json[jsonCounter++] = buff[i++];
        }
    }
    fclose(fp);
    json[jsonCounter] = '\0';

    yyjson_doc *doc = yyjson_read(json, strlen(json), 0);
    yyjson_val *root = yyjson_doc_get_root(doc);
    size_t idx, max, idx_from_item, max_from_item;
    yyjson_val *item, *words;
    yyjson_arr_foreach(root, idx, max, item)
    {
        yyjson_arr_foreach(yyjson_obj_get(item, "content"), idx_from_item, max_from_item, words)
        {
            _build_dic(ts_mapping, dic, yyjson_get_str(words));
        }
    }
    yyjson_doc_free(doc);
}

int main(int argc, char const *argv[])
{
    uint32_t ts_mapping[UTF8_CHINESE_SIZE] = {0};
    uint32_t dic[UTF8_CHINESE_SIZE] = {0};

    setup_ts_mapping(ts_mapping, "C:\\codespace\\naming\\ts.txt");
    _parse(ts_mapping, dic, L"C:\\codespace\\chinese-poetry\\诗经\\shijing.json");
    save_to_file(dic, "C:\\codespace\\naming\\data\\shijing.txt");

    return 0;
}
