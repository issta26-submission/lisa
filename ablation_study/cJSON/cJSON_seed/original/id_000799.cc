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
//<ID> 799
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
    cJSON *null_item = cJSON_CreateNull();
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON *num_item = cJSON_CreateNumber(3.14);

    // step 2: Configure
    cJSON_AddItemToArray(arr, null_item);
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);

    // step 3: Operate
    cJSON_bool is_null = cJSON_IsNull(cJSON_GetArrayItem(arr, 0));
    cJSON_bool is_raw = cJSON_IsRaw(cJSON_GetArrayItem(arr, 1));
    cJSON_bool are_equal = cJSON_Compare(arr, dup_arr, 1);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_arr);

    // API sequence test completed successfully
    return 66;
}