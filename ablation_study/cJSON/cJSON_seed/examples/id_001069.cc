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
//<ID> 1069
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
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON *label = cJSON_CreateString("example");
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "label", label);
    cJSON_AddItemToObject(root, "ok", false_item);
    cJSON *count_num_before = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "count", count_num_before);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(arr);
    cJSON_SetNumberHelper(count_num_before, (double)count);
    cJSON *item1 = cJSON_GetArrayItem(arr, 1);
    cJSON_bool was_number = cJSON_IsNumber(item1);
    cJSON *replacement = cJSON_CreateNumber(25.0);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(arr, item1, replacement);
    cJSON *new_item1 = cJSON_GetArrayItem(arr, 1);
    cJSON_bool is_number_after = cJSON_IsNumber(new_item1);
    cJSON_bool is_false_flag = cJSON_IsFalse(false_item);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    (void)was_number;
    (void)replaced;
    (void)is_number_after;
    (void)is_false_flag;
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}