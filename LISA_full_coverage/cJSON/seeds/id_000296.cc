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
//<ID> 296
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
    cJSON *num1 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(arr, num1);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, false_item);
    cJSON *inner = cJSON_CreateObject();
    cJSON_AddStringToObject(inner, "msg", "hello");
    cJSON_AddItemToArray(arr, inner);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 0, replacement);
    const char *raw_json = "{\"note\":true}";
    cJSON *raw_added = cJSON_AddRawToObject(root, "raw_blob", raw_json);
    cJSON *inner_dup = cJSON_Duplicate(inner, 1);
    cJSON_AddItemToObject(root, "inner_copy", inner_dup);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON *third = cJSON_GetArrayItem(arr, 2);
    cJSON_bool third_is_object = cJSON_IsObject(third);
    cJSON_bool second_is_false = cJSON_IsFalse(second);
    cJSON_AddBoolToObject(root, "third_is_object", third_is_object);
    cJSON_AddBoolToObject(root, "second_is_false", second_is_false);
    cJSON_AddBoolToObject(root, "replace_succeeded", replaced);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "array_size", (double)arr_size);
    char *printed = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}