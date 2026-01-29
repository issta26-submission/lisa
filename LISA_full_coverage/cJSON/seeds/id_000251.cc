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
//<ID> 251
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
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *btrue = cJSON_CreateTrue();
    cJSON *bfalse = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, btrue);
    cJSON_AddItemToArray(arr, bfalse);

    // step 2: Configure
    cJSON *num2 = cJSON_CreateNumber(7.5);
    cJSON_InsertItemInArray(arr, 1, num2); // now: [42.0, 7.5, true, false]
    cJSON *s = cJSON_CreateString("greeting");
    cJSON_AddItemToArray(arr, s); // append string
    cJSON *sum_node = cJSON_AddNumberToObject(root, "sum", 0.0);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(arr);
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    double second_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    double computed_sum = first_val + second_val;
    cJSON_SetNumberHelper(sum_node, computed_sum);
    cJSON *maybe_true = cJSON_GetArrayItem(arr, 2);
    cJSON_bool is_bool_flag = cJSON_IsBool(maybe_true);
    cJSON_bool is_true_flag = cJSON_IsTrue(maybe_true);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    (void)size;
    (void)is_bool_flag;
    (void)is_true_flag;
    (void)printed[0];
    (void)tmp;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}