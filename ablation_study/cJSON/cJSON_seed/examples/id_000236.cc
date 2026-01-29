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
//<ID> 236
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
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.5);
    cJSON *n2 = cJSON_CreateNumber(-3.14);
    cJSON *n3 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *raw = cJSON_CreateRaw("raw_payload");
    cJSON_AddItemToObject(root, "meta", raw);

    // step 2: Configure
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "values_copy", dup_arr);

    // step 3: Operate & Validate
    cJSON *meta_ptr = cJSON_GetObjectItem(root, "meta");
    cJSON_bool meta_is_raw = cJSON_IsRaw(meta_ptr);
    int orig_count = cJSON_GetArraySize(arr);
    int dup_count = cJSON_GetArraySize(dup_arr);
    cJSON *orig_num = cJSON_CreateNumber((double)orig_count);
    cJSON *dup_num = cJSON_CreateNumber((double)dup_count);
    cJSON *meta_flag = cJSON_CreateBool(meta_is_raw);
    cJSON_AddItemToObject(root, "orig_size", orig_num);
    cJSON_AddItemToObject(root, "dup_size", dup_num);
    cJSON_AddItemToObject(root, "meta_is_raw", meta_flag);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}