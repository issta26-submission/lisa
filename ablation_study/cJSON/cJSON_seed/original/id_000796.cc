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
//<ID> 796
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
    cJSON *num_item = cJSON_CreateNumber(3.14);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddItemToArray(arr, null_item);

    // step 2: Configure
    cJSON *aux = cJSON_CreateObject();
    cJSON *dup_array = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(aux, "array_copy", dup_array);
    cJSON *raw_copy = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(aux, "raw_copy", raw_copy);

    // step 3: Operate
    cJSON_bool null_check = cJSON_IsNull(null_item);
    cJSON_bool raw_check = cJSON_IsRaw(raw_item);
    cJSON_AddBoolToObject(aux, "null_check", null_check);
    cJSON_AddBoolToObject(aux, "raw_check", raw_check);
    cJSON_bool equal_check = cJSON_Compare(arr, dup_array, 1);
    cJSON_AddBoolToObject(aux, "equal_check", equal_check);
    char *out_root = cJSON_PrintUnformatted(root);
    char *out_aux = cJSON_PrintUnformatted(aux);

    // step 4: Validate & Cleanup
    cJSON_free(out_root);
    cJSON_free(out_aux);
    cJSON_Delete(root);
    cJSON_Delete(aux);
    // API sequence test completed successfully
    return 66;
}