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
//<ID> 1089
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
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 2: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n2);
    cJSON_InsertItemInArray(arr, 1, n1);

    // step 3: Operate & Validate
    cJSON *item1 = cJSON_GetArrayItem(arr, 1);
    cJSON_bool before_is_num = cJSON_IsNumber(item1);
    cJSON *replacement = cJSON_CreateNumber(20.0);
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *item1_after = cJSON_GetArrayItem(arr, 1);
    cJSON_bool after_is_num = cJSON_IsNumber(item1_after);
    cJSON *count_num = cJSON_CreateNumber((double)cJSON_GetArraySize(arr));
    cJSON_AddItemToObject(root, "count", count_num);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    (void)before_is_num;
    (void)after_is_num;
    (void)item1;
    (void)item1_after;
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}