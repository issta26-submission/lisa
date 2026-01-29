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
//<ID> 1062
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
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON *n2 = cJSON_CreateNumber(30.0);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "flag", false_item);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON *target = cJSON_GetArrayItem(arr, 1);
    cJSON_ReplaceItemViaPointer(arr, target, replacement);
    cJSON *status = cJSON_CreateString("configured");
    cJSON_AddItemToObject(root, "status", status);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(arr);
    cJSON *countNum = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "count", countNum);
    cJSON *a0 = cJSON_GetArrayItem(arr, 0);
    cJSON *a1 = cJSON_GetArrayItem(arr, 1);
    cJSON *a2 = cJSON_GetArrayItem(arr, 2);
    cJSON_bool is_num_a0 = cJSON_IsNumber(a0);
    cJSON_bool is_num_a1 = cJSON_IsNumber(a1);
    cJSON_bool is_num_a2 = cJSON_IsNumber(a2);
    double sum = cJSON_GetNumberValue(a0) + cJSON_GetNumberValue(a1) + cJSON_GetNumberValue(a2);
    cJSON *sumNum = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sumNum);
    cJSON_bool flag_is_false = cJSON_IsFalse(false_item);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    (void)is_num_a0;
    (void)is_num_a1;
    (void)is_num_a2;
    (void)flag_is_false;
    return 66; // API sequence test completed successfully
}