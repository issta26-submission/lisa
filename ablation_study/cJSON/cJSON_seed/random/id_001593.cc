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
//<ID> 1593
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *float_arr = (cJSON *)0;
    cJSON *ref_arr = (cJSON *)0;
    cJSON *num_ptr = (cJSON *)0;
    cJSON *retrieved_float_arr = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    float nums[3];
    cJSON_bool added_float_arr = 0;
    cJSON_bool added_ref_arr = 0;
    int float_arr_size = 0;
    double num_value = 0.0;
    cJSON_bool is_num = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    nums[0] = 1.5f;
    nums[1] = 2.5f;
    nums[2] = -3.75f;
    float_arr = cJSON_CreateFloatArray(nums, 3);

    // step 3: Configure
    added_float_arr = cJSON_AddItemToObject(root, "floats", float_arr);
    num_ptr = cJSON_AddNumberToObject(root, "single", 42.0);
    ref_arr = cJSON_CreateArrayReference((const cJSON *)num_ptr);
    added_ref_arr = cJSON_AddItemToObject(root, "ref", ref_arr);

    // step 4: Operate
    retrieved_float_arr = cJSON_GetObjectItem(root, "floats");
    float_arr_size = cJSON_GetArraySize(retrieved_float_arr);
    retrieved_num = cJSON_GetObjectItem(root, "single");
    num_value = cJSON_GetNumberValue(retrieved_num);
    is_num = cJSON_IsNumber(retrieved_num);

    // step 5: Validate
    validation_score = (int)added_float_arr
        + (int)added_ref_arr
        + (int)(retrieved_float_arr != (cJSON *)0)
        + (int)(float_arr_size == 3)
        + (int)(retrieved_num != (cJSON *)0)
        + (int)is_num
        + (int)(num_value == 42.0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)float_arr_size;
    (void)num_value;
    (void)retrieved_float_arr;
    (void)retrieved_num;
    // API sequence test completed successfully
    return 66;
}