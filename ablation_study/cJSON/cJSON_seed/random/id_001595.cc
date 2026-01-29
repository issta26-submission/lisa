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
//<ID> 1595
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
    cJSON *arr_ref = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    double first_value = 0.0;
    int arr_size = 0;
    cJSON_bool added_float_arr = 0;
    cJSON_bool added_arr_ref = 0;
    int validation_score = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    float nums[3] = { 1.5f, -2.5f, 4.25f };
    float_arr = cJSON_CreateFloatArray(nums, 3);

    // step 3: Configure
    added_float_arr = cJSON_AddItemToObject(root, "floats", float_arr);
    arr_ref = cJSON_CreateArrayReference(float_arr);
    added_arr_ref = cJSON_AddItemToObject(root, "floats_ref", arr_ref);
    count_item = cJSON_AddNumberToObject(root, "count", 3.0);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "floats");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    first_value = cJSON_GetNumberValue(first_elem);

    // step 5: Validate
    validation_score = (int)added_float_arr
        + (int)added_arr_ref
        + (int)(count_item != (cJSON *)0)
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(arr_size == 3)
        + (int)(first_elem != (cJSON *)0)
        + (int)(first_value == (double)nums[0]);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)first_value;
    (void)first_elem;
    (void)retrieved_arr;
    (void)count_item;
    // API sequence test completed successfully
    return 66;
}