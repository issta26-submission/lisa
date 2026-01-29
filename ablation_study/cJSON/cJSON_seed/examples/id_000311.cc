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
//<ID> 311
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

    // step 2: Configure
    cJSON *num_item = cJSON_CreateNumber(3.1415);
    cJSON *false_item = cJSON_CreateFalse();
    int nums[4] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToArray(arr, int_array);
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *doubled = cJSON_CreateNumber(first_val * 2.0);
    cJSON_AddItemToObject(root, "doubled_first", doubled);
    cJSON *third = cJSON_GetArrayItem(arr, 2);
    cJSON *third_first = cJSON_GetArrayItem(third, 0);
    double third_first_val = cJSON_GetNumberValue(third_first);
    cJSON *copied_third_value = cJSON_CreateNumber(third_first_val);
    cJSON_AddItemToObject(root, "copied_third_first", copied_third_value);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}