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
//<ID> 791
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
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, null_item);
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":1}");
    cJSON_AddItemToArray(arr, raw_item);
    cJSON *num_item = cJSON_CreateNumber(123.0);
    cJSON_AddItemToArray(arr, num_item);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str_item);

    // step 2: Configure
    cJSON *dup_raw = cJSON_Duplicate(raw_item, 1);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)arr_size);

    // step 3: Operate
    cJSON_bool was_null = cJSON_IsNull(null_item);
    cJSON_bool was_raw = cJSON_IsRaw(raw_item);
    cJSON_bool raw_equal = cJSON_Compare(raw_item, dup_raw, 1);
    cJSON_AddBoolToObject(root, "had_null", was_null);
    cJSON_AddBoolToObject(root, "had_raw", was_raw);
    cJSON_AddBoolToObject(root, "raw_equal", raw_equal);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(dup_raw);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}