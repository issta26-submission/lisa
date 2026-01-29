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
//<ID> 358
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *const fruits[] = { "apple", "banana", "cherry" };
    cJSON *str_arr = cJSON_CreateStringArray(fruits, 3);
    cJSON *nums = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(nums, n0);
    cJSON_AddItemToArray(nums, n1);
    cJSON_AddItemToObject(root, "fruits", str_arr);
    cJSON_AddItemToObject(root, "numbers", nums);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInArray(nums, 1, replacement);
    cJSON *first_elem = cJSON_GetArrayItem(nums, 0);
    cJSON_SetNumberHelper(first_elem, 3.1415);

    // step 3: Operate & Validate
    double a = cJSON_GetNumberValue(cJSON_GetArrayItem(nums, 0));
    double b = cJSON_GetNumberValue(cJSON_GetArrayItem(nums, 1));
    double sum = a + b;
    cJSON *sum_item = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(sum_item, sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    char *printed = cJSON_Print(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}