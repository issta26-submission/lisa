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
//<ID> 1088
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

    // step 2: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_InsertItemInArray(arr, 1, n2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddStringToObject(root, "meta", "example");

    // step 3: Operate & Validate
    cJSON *mid_item = cJSON_GetArrayItem(arr, 1);
    cJSON_bool mid_is_number = cJSON_IsNumber(mid_item);
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 2, replacement);
    cJSON *after = cJSON_GetArrayItem(arr, 2);
    cJSON_bool after_is_number = cJSON_IsNumber(after);
    int sz = cJSON_GetArraySize(arr);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    (void)mid_is_number;
    (void)replaced;
    (void)after_is_number;
    (void)sz;
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}