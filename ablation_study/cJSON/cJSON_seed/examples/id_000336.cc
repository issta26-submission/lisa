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
//<ID> 336
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
    cJSON *elem0 = cJSON_CreateNumber(1.0);
    cJSON *elem1 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, elem0);
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddItemToObject(root, "values", arr);

    // step 2: Configure
    cJSON *added_scalar = cJSON_AddNumberToObject(root, "scalar", 3.14);
    cJSON *replacement = cJSON_CreateNumber(42.0);

    // step 3: Operate & Validate
    cJSON *scalar = cJSON_GetObjectItem(root, "scalar");
    double scalar_val = cJSON_GetNumberValue(scalar);
    cJSON *second_in_arr = cJSON_GetArrayItem(arr, 1);
    double second_val = cJSON_GetNumberValue(second_in_arr);
    cJSON *sum_item = cJSON_CreateNumber(scalar_val + second_val);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON_ReplaceItemInArray(arr, 0, replacement);
    char *printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}