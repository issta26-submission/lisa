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
//<ID> 1084
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
    cJSON *n2 = cJSON_CreateNumber(3.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);

    // step 2: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *label = cJSON_CreateString("sequence");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate & Validate
    cJSON_InsertItemInArray(arr, 1, n1);
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInArray(arr, 0, replacement);
    cJSON *middle = cJSON_GetArrayItem(arr, 1);
    cJSON_bool middle_is_number = cJSON_IsNumber(middle);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    (void)middle_is_number;

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}