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
//<ID> 1596
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *float_array = (cJSON *)0;
    cJSON *array_ref = (cJSON *)0;
    cJSON *number_ptr = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_ref = (cJSON *)0;
    cJSON_bool added_main = 0;
    cJSON_bool added_ref = 0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    float nums[3];
    nums[0] = 0.5f;
    nums[1] = 1.5f;
    nums[2] = -2.25f;
    float_array = cJSON_CreateFloatArray(nums, 3);

    // step 3: Configure
    added_main = cJSON_AddItemToObject(root, "values", float_array);
    array_ref = cJSON_CreateArrayReference(float_array);
    added_ref = cJSON_AddItemToObject(root, "values_ref", array_ref);
    number_ptr = cJSON_AddNumberToObject(root, "length", 3.0);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    retrieved_ref = cJSON_GetObjectItem(root, "values_ref");
    arr_size = cJSON_GetArraySize(retrieved_arr);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0)
        + (int)(float_array != (cJSON *)0)
        + (int)added_main
        + (int)(array_ref != (cJSON *)0)
        + (int)added_ref
        + (int)(number_ptr != (cJSON *)0)
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(retrieved_ref != (cJSON *)0)
        + (int)(arr_size == 3);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_arr;
    (void)retrieved_ref;
    (void)number_ptr;
    // API sequence test completed successfully
    return 66;
}