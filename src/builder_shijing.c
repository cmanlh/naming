#include "../include/yyjson.h"
#include <wchar.h>
#include <stdlib.h>
#include <sys/stat.h>

void setup_ts_dic(){}

int main(int argc, char const *argv[])
{
    wchar_t *path = L"C:\\codespace\\chinese-poetry\\诗经\\shijing.json";
    struct stat fileStat = {};
    wstat(path, &fileStat);

    FILE *fp = _wfopen(path, L"r");
    char json[fileStat.st_size + 1];
    int jsonCounter = 0;
    char buff[1024];
    while (fgets(buff, 1024, fp) != NULL)
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
    size_t idx, max, inner_idx, inner_max;
    yyjson_val *hit, *hit_content;
    yyjson_arr_foreach(root, idx, max, hit)
    {
        yyjson_arr_foreach(yyjson_obj_get(hit, "content"), inner_idx, inner_max, hit_content)
        {
            printf("%d/%d: %s\n", (int)inner_idx, (int)inner_max, yyjson_get_str(hit_content));
        }
    }
    return 0;
}
