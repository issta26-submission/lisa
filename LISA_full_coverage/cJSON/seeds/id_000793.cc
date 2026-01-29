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
//<ID> 793
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
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON *num_item = cJSON_CreateNumber(3.14);
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddItemToArray(arr, null_item);

    // step 2: Configure
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *count_item = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "count", count_item);
    cJSON_bool was_null = cJSON_IsNull(null_item);
    cJSON_bool was_raw = cJSON_IsRaw(raw_item);
    cJSON *null_flag = cJSON_CreateBool(was_null);
    cJSON *raw_flag = cJSON_CreateBool(was_raw);
    cJSON_AddItemToObject(root, "has_null", null_flag);
    cJSON_AddItemToObject(root, "has_raw", raw_flag);

    // step 3: Operate
    cJSON *arr_copy = cJSON_Duplicate(arr, 1);
    cJSON_bool equal = cJSON_Compare(arr, arr_copy, 1);
    cJSON *equal_flag = cJSON_CreateBool(equal);
    cJSON_AddItemToObject(root, "array_equal", equal_flag);
    cJSON *first_detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_AddItemToObject(root, "first_moved", first_detached);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(arr_copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}