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
//<ID> 294
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
    cJSON *num1 = cJSON_CreateNumber(1.23);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, false_item);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddTrueToObject(child, "ok");
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *new_num = cJSON_CreateNumber(9.99);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 0, new_num);
    cJSON *raw_added = cJSON_AddRawToObject(root, "raw_payload", "{\"raw\":true}");
    (void)raw_added;

    // step 3: Operate and Validate
    cJSON_bool child_is_object = cJSON_IsObject(child);
    cJSON_bool second_is_false = cJSON_IsFalse(false_item);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "arr_size", (double)arr_size);
    cJSON_AddNumberToObject(root, "replace_succeeded", (double)replaced);
    cJSON_AddNumberToObject(root, "child_is_object", (double)child_is_object);
    cJSON_AddNumberToObject(root, "second_is_false", (double)second_is_false);
    char *dump = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);

    // step 4: Cleanup
    cJSON_free(dump);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}