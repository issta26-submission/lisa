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
//<ID> 295
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
    cJSON *num2 = cJSON_CreateNumber(4.56);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, false_item);
    cJSON *inner_obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(inner_obj, "inner_val", 7.0);
    cJSON_AddItemToArray(arr, inner_obj);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_bool replaced_ok = cJSON_ReplaceItemInArray(arr, 0, replacement);
    cJSON *raw_added = cJSON_AddRawToObject(root, "raw_field", "{\"raw\":true}");
    cJSON *mirror = cJSON_CreateObject();
    cJSON *arr_dup = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(mirror, "items_copy", arr_dup);
    cJSON_AddItemToObject(root, "mirror", mirror);

    // step 3: Operate and Validate
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_bool was_false = cJSON_IsFalse(false_item);
    int array_count = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "array_count", (double)array_count);
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