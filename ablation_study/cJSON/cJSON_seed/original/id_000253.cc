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
//<ID> 253
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
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, n2);
    cJSON *b_true = cJSON_CreateTrue();
    cJSON_bool inserted_flag = cJSON_InsertItemInArray(arr, 1, b_true);

    // step 2: Configure
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "array_size", (double)arr_size);
    cJSON_bool is_bool_flag = cJSON_IsBool(b_true);
    cJSON_AddBoolToObject(root, "b_is_bool", is_bool_flag);
    cJSON_bool is_true_flag = cJSON_IsTrue(b_true);
    cJSON_AddBoolToObject(root, "b_is_true", is_true_flag);
    cJSON_AddBoolToObject(root, "inserted_success", inserted_flag);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_bool equal = cJSON_Compare(arr, dup_arr, 1);
    cJSON_AddBoolToObject(root, "arr_equals_dup", equal);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}