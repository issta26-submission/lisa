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
//<ID> 1244
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
    cJSON *extra_num = (cJSON *)0;
    cJSON *third_item = (cJSON *)0;
    double third_value = 0.0;
    double added_value = 0.0;
    cJSON_bool is_array = 0;
    int nums[3];
    int validation_score = 0;

    // step 2: Initialize
    nums[0] = 1;
    nums[1] = 2;
    nums[2] = 3;
    root = cJSON_CreateObject();
    arr = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 3: Configure
    extra_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, extra_num);

    // step 4: Operate
    is_array = cJSON_IsArray(arr);
    third_item = cJSON_GetArrayItem(arr, 2);
    third_value = cJSON_GetNumberValue(third_item);
    added_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 3));

    // step 5: Validate
    validation_score = (int)is_array + (int)(third_value == 3.0) + (int)(added_value == 42.0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_DeleteItemFromArray(arr, 1);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}