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
//<ID> 946
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
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *b1 = cJSON_CreateTrue();
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToObject(root, "array", arr);

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, b1);
    cJSON_InsertItemInArray(arr, 1, n2);

    // step 3: Operate & Validate
    cJSON *got0 = cJSON_GetArrayItem(arr, 0);
    cJSON *got1 = cJSON_GetArrayItem(arr, 1);
    double val0 = cJSON_GetNumberValue(got0);
    double val1 = cJSON_GetNumberValue(got1);
    double sum = val0 + val1;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON *third = cJSON_GetArrayItem(arr, 2);
    cJSON_bool third_is_bool = cJSON_IsBool(third);
    cJSON_AddItemToObject(root, "third_is_bool", cJSON_CreateBool(third_is_bool));
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_bool arrays_equal = cJSON_Compare(arr, dup_arr, 1);
    cJSON_AddItemToObject(root, "arrays_equal", cJSON_CreateBool(arrays_equal));

    // step 4: Cleanup
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}