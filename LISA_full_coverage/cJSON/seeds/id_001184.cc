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
//<ID> 1184
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"num\": 42, \"items\": [\"x\",\"y\"]}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(array, str_item);
    cJSON *arr_ref = cJSON_CreateArray();
    cJSON_AddItemReferenceToArray(arr_ref, str_item);
    cJSON_AddItemToObject(root, "array", array);
    cJSON_AddItemToObject(root, "array_ref", arr_ref);
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *to_replace = cJSON_CreateNumber(7);
    cJSON_AddItemToObject(root, "replace_me", to_replace);

    // step 2: Configure
    cJSON *new_item = cJSON_CreateString("I am replacement");
    cJSON_ReplaceItemViaPointer(root, to_replace, new_item);
    cJSON_AddNullToObject(root, "a_null");

    // step 3: Operate
    cJSON_bool is_arr = cJSON_IsArray(array);
    cJSON_bool is_arr_ref_array = cJSON_IsArray(arr_ref);
    cJSON_AddBoolToObject(root, "array_is_array", is_arr);
    cJSON_AddBoolToObject(root, "array_ref_is_array", is_arr_ref_array);
    char *flat = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}