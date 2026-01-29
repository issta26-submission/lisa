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
//<ID> 1087
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
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON *n2 = cJSON_CreateNumber(3.0);
    cJSON *label = cJSON_CreateString("numbers");

    // step 2: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n2);
    cJSON_InsertItemInArray(arr, 1, n1);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate & Validate
    cJSON *item_before = cJSON_GetArrayItem(arr, 1);
    cJSON_bool before_is_num = cJSON_IsNumber(item_before);
    cJSON *replacement = cJSON_CreateNumber(20.0);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *item_after = cJSON_GetArrayItem(arr, 1);
    cJSON_bool after_is_num = cJSON_IsNumber(item_after);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    (void)before_is_num;
    (void)replaced;
    (void)after_is_num;
    (void)item_before;
    (void)item_after;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}