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
//<ID> 798
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
    cJSON *num_item = cJSON_CreateNumber(3.14159);
    cJSON *str_item = cJSON_CreateString("hello world");
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");

    // step 2: Configure
    cJSON_AddItemToArray(arr, null_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddStringToObject(root, "note", "sample");

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_null = cJSON_IsNull(first);
    (void)first_is_null;
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    (void)raw_is_raw;
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool same_case = cJSON_Compare(root, dup, 1);
    (void)same_case;
    char *out_root = cJSON_PrintUnformatted(root);
    char *out_dup = cJSON_PrintUnformatted(dup);

    // step 4: Validate & Cleanup
    cJSON_free(out_root);
    cJSON_free(out_dup);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}