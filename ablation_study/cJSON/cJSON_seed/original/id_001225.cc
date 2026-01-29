#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1225
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *str_item = cJSON_CreateString("hello world");
    cJSON_AddItemToArray(arr, str_item);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const char *version = cJSON_Version();
    int buf_len = 512;

    // step 3: Operate
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    if (printed) {
        size_t to_copy = strlen(printed);
        if (to_copy >= (size_t)buf_len) to_copy = (size_t)buf_len - 1;
        memcpy(buf, printed, to_copy);
        buf[to_copy] = '\0';
    }
    cJSON_Minify(buf);
    cJSON *parsed = cJSON_Parse(buf);
    cJSON *items = cJSON_GetObjectItem(parsed, "items");
    cJSON_bool is_array = cJSON_IsArray(items);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    const char *retrieved = cJSON_GetStringValue(first);

    // step 4: Validate & Cleanup
    (void)version;
    (void)is_array;
    (void)retrieved;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}