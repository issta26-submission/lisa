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
//<ID> 1598
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *farr = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *num_count = (cJSON *)0;
    cJSON_bool added_orig = 0;
    cJSON_bool added_ref = 0;
    cJSON_bool added_num = 0;
    int array_size = 0;
    cJSON_bool is_array_orig = 0;
    cJSON_bool is_array_ref = 0;
    cJSON_bool num_is_number = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    float nums[3];
    nums[0] = 1.0f;
    nums[1] = 2.5f;
    nums[2] = -3.75f;
    farr = cJSON_CreateFloatArray(nums, 3);

    // step 3: Configure
    added_orig = cJSON_AddItemToObject(root, "orig", farr);
    arr_ref = cJSON_CreateArrayReference(farr);
    added_ref = cJSON_AddItemToObject(root, "ref", arr_ref);
    num_count = cJSON_AddNumberToObject(root, "count", 3.0);

    // step 4: Operate
    array_size = cJSON_GetArraySize(farr);
    is_array_orig = cJSON_IsArray(farr);
    is_array_ref = cJSON_IsArray(arr_ref);
    num_is_number = cJSON_IsNumber(num_count);

    // step 5: Validate
    validation_score = (int)added_orig
        + (int)added_ref
        + (int)(array_size == 3)
        + (int)is_array_orig
        + (int)is_array_ref
        + (int)num_is_number
        + (int)(num_count != (cJSON *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)added_num;
    (void)added_orig;
    (void)added_ref;
    (void)nums;
    (void)farr;
    (void)arr_ref;
    (void)num_count;
    // API sequence test completed successfully
    return 66;
}