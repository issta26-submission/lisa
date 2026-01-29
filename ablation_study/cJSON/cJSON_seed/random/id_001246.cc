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
//<ID> 1246
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *new_num = (cJSON *)0;
    cJSON *second_item = (cJSON *)0;
    cJSON *third_item = (cJSON *)0;
    int nums[4] = {10, 20, 30, 40};
    int count = 4;
    cJSON_bool is_arr = 0;
    double value_second = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateIntArray(nums, count);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 3: Configure
    new_num = cJSON_CreateNumber(99.0);
    cJSON_AddItemToArray(arr, new_num);

    // step 4: Operate
    is_arr = cJSON_IsArray(arr);
    second_item = cJSON_GetArrayItem(arr, 1);
    third_item = cJSON_GetArrayItem(arr, 2);
    value_second = cJSON_GetNumberValue(second_item);
    cJSON_DeleteItemFromArray(arr, 2);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0) + (int)is_arr + (int)(second_item != (cJSON *)0) + (int)(value_second == 20.0) + (int)(new_num != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}