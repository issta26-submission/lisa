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
//<ID> 1082
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
    cJSON *n0 = cJSON_CreateNumber(1.5);
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON *n2 = cJSON_CreateNumber(3.5);
    cJSON *label = cJSON_CreateString("numbers");

    // step 2: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "label", label);
    cJSON *extra = cJSON_CreateNumber(4.5);
    cJSON_InsertItemInArray(arr, 1, extra);

    // step 3: Operate & Validate
    cJSON *item1 = cJSON_GetArrayItem(arr, 1);
    cJSON_bool is_num1 = cJSON_IsNumber(item1);
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 2, replacement);
    cJSON *after_item2 = cJSON_GetArrayItem(arr, 2);
    cJSON_bool is_num2 = cJSON_IsNumber(after_item2);
    int size = cJSON_GetArraySize(arr);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    (void)is_num1;
    (void)replaced;
    (void)is_num2;
    (void)size;
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}