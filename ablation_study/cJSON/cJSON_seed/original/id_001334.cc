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
//<ID> 1334
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"arr\":[10,20],\"flag\":false}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItemCaseSensitive(parsed, "arr");
    cJSON *flag = cJSON_GetObjectItemCaseSensitive(parsed, "flag");
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "original", parsed);
    cJSON *new_arr_container = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "copy_arr", new_arr_container);
    cJSON *arr_dup = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToArray(new_arr_container, arr_dup);
    cJSON_bool flag_is_false = cJSON_IsFalse(flag);
    cJSON *flag_copy = cJSON_CreateBool(flag_is_false);
    cJSON_AddItemToObject(root, "flag_copy", flag_copy);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    int buf_len = (int)printed_len + 16;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    memcpy(buf, printed, printed_len + 1);
    cJSON_free(printed);

    // step 4: Validate & Cleanup
    cJSON_Minify(buf);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}