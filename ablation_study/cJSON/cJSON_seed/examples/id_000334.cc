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
//<ID> 334
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
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(9.9);
    cJSON *numbers_obj = cJSON_GetObjectItem(root, "numbers");
    cJSON_ReplaceItemInArray(numbers_obj, 1, replacement);
    double replacement_value = cJSON_GetNumberValue(replacement);
    cJSON_AddNumberToObject(root, "replacement_value", replacement_value);

    // step 3: Operate & Validate
    cJSON *nums = cJSON_GetObjectItem(root, "numbers");
    cJSON *e0 = cJSON_GetArrayItem(nums, 0);
    cJSON *e1 = cJSON_GetArrayItem(nums, 1);
    cJSON *e2 = cJSON_GetArrayItem(nums, 2);
    double sum = cJSON_GetNumberValue(e0) + cJSON_GetNumberValue(e1) + cJSON_GetNumberValue(e2);
    cJSON_AddNumberToObject(root, "sum", sum);
    char *printed = cJSON_Print(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}