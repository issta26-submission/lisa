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
//<ID> 790
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
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":1}");
    cJSON *str_item = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(3.14));
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "maybe", null_item);
    cJSON_AddItemToObject(root, "raw", raw_item);

    // step 2: Configure
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)arr_size);
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "copy", dup_arr);

    // step 3: Operate
    cJSON_bool cmp = cJSON_Compare(arr, dup_arr, 1);
    cJSON_AddItemToObject(root, "equal", cJSON_CreateBool(cmp));
    cJSON_AddNumberToObject(root, "is_null", (double)cJSON_IsNull(null_item));
    cJSON_AddNumberToObject(root, "is_raw", (double)cJSON_IsRaw(raw_item));

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}