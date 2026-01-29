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
//<ID> 254
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
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *t = cJSON_CreateTrue();
    cJSON *f = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, t);
    cJSON_AddItemToArray(arr, f);
    cJSON *inserted = cJSON_CreateString("middle");
    cJSON_InsertItemInArray(arr, 1, inserted);

    // step 2: Configure
    int size_i = cJSON_GetArraySize(arr);
    int t_is_bool = (int)cJSON_IsBool(t);
    int f_is_bool = (int)cJSON_IsBool(f);
    int t_is_true = (int)cJSON_IsTrue(t);
    cJSON_AddNumberToObject(root, "array_size", (double)size_i);
    cJSON_AddNumberToObject(root, "true_is_bool", (double)t_is_bool);
    cJSON_AddNumberToObject(root, "false_is_bool", (double)f_is_bool);
    cJSON_AddNumberToObject(root, "true_is_true", (double)t_is_true);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    cJSON *arr_dup = cJSON_Duplicate(arr, 1);
    cJSON_bool equal = cJSON_Compare(arr, arr_dup, 1);
    cJSON_AddNumberToObject(root, "dup_equal", (double)(int)equal);
    int summary = size_i + t_is_bool + f_is_bool + t_is_true + (int)printed[0] + (int)equal;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(arr_dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}