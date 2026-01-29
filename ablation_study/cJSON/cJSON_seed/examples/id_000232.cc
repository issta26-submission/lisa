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
//<ID> 232
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
    cJSON *arr = cJSON_CreateArray();

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON *raw = cJSON_CreateRaw("{\"raw_key\":true}");
    cJSON_AddItemToObject(child, "arr", arr);
    cJSON_AddItemToObject(child, "raw", raw);
    cJSON_AddItemToObject(root, "child1", child);
    cJSON *child_dup = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child2", child_dup);

    // step 3: Operate & Validate
    cJSON *got_child1 = cJSON_GetObjectItem(root, "child1");
    cJSON *got_arr = cJSON_GetObjectItem(got_child1, "arr");
    int arr_size = cJSON_GetArraySize(got_arr);
    cJSON *arr_size_num = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "arr_size", arr_size_num);
    cJSON *got_child2 = cJSON_GetObjectItem(root, "child2");
    cJSON *got_raw = cJSON_GetObjectItem(got_child2, "raw");
    cJSON_bool raw_flag = cJSON_IsRaw(got_raw);
    cJSON *raw_flag_num = cJSON_CreateNumber((double)raw_flag);
    cJSON_AddItemToObject(root, "is_raw", raw_flag_num);
    cJSON *arr_copy = cJSON_Duplicate(got_arr, 1);
    cJSON_AddItemToObject(root, "arr_copy", arr_copy);
    cJSON *arr_copy_size_num = cJSON_CreateNumber((double)cJSON_GetArraySize(arr_copy));
    cJSON_AddItemToObject(root, "arr_copy_size", arr_copy_size_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}