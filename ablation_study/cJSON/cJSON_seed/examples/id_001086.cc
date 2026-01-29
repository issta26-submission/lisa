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
//<ID> 1086
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
    cJSON *s2 = cJSON_CreateString("three");

    // step 2: Configure
    cJSON_InsertItemInArray(arr, 0, n0);
    cJSON_InsertItemInArray(arr, 1, n1);
    cJSON_InsertItemInArray(arr, 2, s2);
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate & Validate
    cJSON *before = cJSON_GetArrayItem(arr, 1);
    cJSON_bool was_number_before = cJSON_IsNumber(before);
    cJSON *replacement = cJSON_CreateNumber(2.5);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *after = cJSON_GetArrayItem(arr, 1);
    cJSON_bool is_number_after = cJSON_IsNumber(after);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    (void)was_number_before;
    (void)replaced;
    (void)is_number_after;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}