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
//<ID> 297
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
    cJSON *num1 = cJSON_CreateNumber(1.234);
    cJSON_AddItemToArray(arr, num1);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, false_item);
    cJSON *inner = cJSON_CreateObject();
    cJSON_AddStringToObject(inner, "name", "inner");
    cJSON_AddItemToArray(arr, inner);

    // step 2: Configure
    cJSON *raw_added = cJSON_AddRawToObject(root, "raw_json", "{\"k\":true}");
    cJSON *num_replace = cJSON_CreateNumber(9.99);
    cJSON_bool replace_result = cJSON_ReplaceItemInArray(arr, 0, num_replace);

    // step 3: Operate and Validate
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_was_false = cJSON_IsFalse(second_item);
    cJSON_AddBoolToObject(root, "root_is_object", root_is_obj);
    cJSON_AddBoolToObject(root, "second_was_false", second_was_false);
    int array_count = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "array_count", (double)array_count);
    char *printed = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(256);
    memset(buffer, 0, 256);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}