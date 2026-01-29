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
//<ID> 797
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
    cJSON *num_item = cJSON_CreateNumber(7.0);
    cJSON_bool null_check_initial = cJSON_IsNull(null_item);
    cJSON_bool raw_check_initial = cJSON_IsRaw(raw_item);

    // step 2: Configure
    cJSON_AddItemToArray(arr, null_item);
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *aux = cJSON_CreateObject();
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON_AddItemToObject(aux, "copy", dup_root);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(out);
    cJSON_bool compare_parsed = cJSON_Compare(root, parsed, 1);
    cJSON_bool compare_dup = cJSON_Compare(root, dup_root, 1);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_null = cJSON_IsNull(first);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_raw = cJSON_IsRaw(second);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_Delete(aux);
    // API sequence test completed successfully
    return 66;
}