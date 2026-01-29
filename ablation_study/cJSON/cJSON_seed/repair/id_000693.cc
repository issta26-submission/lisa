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
//<ID> 693
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON *arr = cJSON_CreateArray();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, false_item);
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num_item);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(child, "arr", arr);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "note", "sample");
    cJSON_AddItemToObject(meta, "details", cJSON_CreateObject());

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "array_size", (double)arr_size);
    cJSON_bool child_is_obj = cJSON_IsObject(child);
    cJSON_AddBoolToObject(root, "child_is_object", child_is_obj);
    cJSON *child_copy = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_copy", child_copy);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}