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
//<ID> 1097
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
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *count = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(root, "count", count);
    cJSON *label = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "label", label);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_null", maybe_null);

    // step 2: Configure
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON *replacement = cJSON_CreateString("two_replaced");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(arr, second, replacement);
    cJSON_bool array_check = cJSON_IsArray(arr);
    cJSON_bool null_check = cJSON_IsNull(maybe_null);

    // step 3: Operate
    double count_value = cJSON_GetNumberValue(count);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate & Cleanup
    (void)replaced;
    (void)array_check;
    (void)null_check;
    (void)count_value;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}