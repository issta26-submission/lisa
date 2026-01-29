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
//<ID> 292
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
    cJSON_AddItemToObject(root, "data", arr);
    cJSON *num1 = cJSON_CreateNumber(1.23);
    cJSON *bool_false = cJSON_CreateFalse();
    cJSON *inner = cJSON_CreateObject();
    cJSON_AddStringToObject(inner, "name", "inner");
    cJSON_AddNumberToObject(inner, "value", 7.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, bool_false);
    cJSON_AddItemToArray(arr, inner);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_bool replace_ok = cJSON_ReplaceItemInArray(arr, 0, replacement);
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw_blob", "{\"x\":true,\"y\":123}");
    cJSON *inner_dup = cJSON_Duplicate(inner, 1);
    cJSON_AddItemToObject(root, "inner_copy", inner_dup);

    // step 3: Operate and Validate
    cJSON_bool inner_is_obj = cJSON_IsObject(inner);
    cJSON_bool inner_dup_is_obj = cJSON_IsObject(inner_dup);
    cJSON_bool third_is_false = cJSON_IsFalse(cJSON_GetArrayItem(arr, 1));
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "arr_size", (double)arr_size);
    cJSON_AddBoolToObject(root, "replace_ok", replace_ok);
    cJSON_AddBoolToObject(root, "inner_is_object", inner_is_obj);
    cJSON_AddBoolToObject(root, "inner_dup_is_object", inner_dup_is_obj);
    cJSON_AddBoolToObject(root, "third_is_false", third_is_false);
    char *dump = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(256);
    memset(buffer, 0, 256);

    // step 4: Cleanup
    cJSON_free(dump);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}