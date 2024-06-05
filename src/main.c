
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
            exit(1);
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

void readJson()
{
    const char *json = "{\"name\":\"Mash中国\",\"star\":4,\"hits\":[2,2,1,3]}";

    // Read JSON and get root
    yyjson_doc *doc = yyjson_read(json, strlen(json), 0);
    yyjson_val *root = yyjson_doc_get_root(doc);

    // Get root["name"]
    yyjson_val *name = yyjson_obj_get(root, "name");
    printf("name: %s\n", yyjson_get_str(name));
    printf("name length:%d\n", (int)yyjson_get_len(name));

    // Get root["star"]
    yyjson_val *star = yyjson_obj_get(root, "star");
    printf("star: %d\n", (int)yyjson_get_int(star));

    // Get root["hits"], iterate over the array
    yyjson_val *hits = yyjson_obj_get(root, "hits");
    size_t idx, max;
    yyjson_val *hit;
    yyjson_arr_foreach(hits, idx, max, hit)
    {
        printf("hit%d: %d\n", (int)idx, (int)yyjson_get_int(hit));
    }

    // Free the doc
    yyjson_doc_free(doc);
}

void hash()
{
    wchar_t *path = L"C:\\codespace\\chinese-poetry\\全唐诗\\poet.tang.0.json";
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
    size_t idx, max;
    yyjson_val *hit;
    // yyjson_arr_foreach(root, idx, max, hit)
    // {
    //     printf("%d/%d: %s\n", (int)idx, (int)max, yyjson_get_str(yyjson_obj_get(hit, "author")));
    // }

    uint32_t num = 12323;
    printf("%d\n", XXH32(&num, sizeof(num), 32));
    printf("%d\n", XXH32(&num, sizeof(num), 64));
    printf("%d\n", XXH32(&num, sizeof(num), 32));
}