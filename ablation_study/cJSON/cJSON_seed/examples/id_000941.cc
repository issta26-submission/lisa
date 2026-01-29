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
//<ID> 941
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
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(3.0);
    cJSON *n_insert = cJSON_CreateNumber(2.0);
    cJSON *flag = cJSON_CreateTrue();
    cJSON *name = cJSON_CreateString("sensor-007");

    // step 2: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_InsertItemInArray(arr, 1, n_insert);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "active", flag);
    cJSON_AddItemToObject(root, "name", name);

    // step 3: Operate & Validate
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    cJSON *got_flag = cJSON_GetObjectItem(root, "active");
    cJSON_bool flag_is_bool = cJSON_IsBool(got_flag);
    cJSON *arr_dup = cJSON_Duplicate(arr, 1);
    cJSON_bool equal_arrays = cJSON_Compare(arr, arr_dup, 1);
    (void)second_value;
    (void)flag_is_bool;
    (void)equal_arrays;

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(arr_dup);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}