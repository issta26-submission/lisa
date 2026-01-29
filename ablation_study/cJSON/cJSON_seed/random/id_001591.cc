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
//<ID> 1591
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
    cJSON *num_item = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON_bool added_float = 0;
    cJSON_bool added_ref = 0;
    cJSON_bool added_num_bool = 0;
    int array_size = 0;
    cJSON_bool first_is_number = 0;
    double first_val = 0.0;
    int validation_score = 0;
    float numbers[3] = { 1.1f, 2.2f, 3.3f };
    int count = 3;

    // step 2: Initialize
    root = cJSON_CreateObject();
    float_arr = cJSON_CreateFloatArray(numbers, count);

    // step 3: Configure
    added_float = cJSON_AddItemToObject(root, "floats", float_arr);
    arr_ref = cJSON_CreateArrayReference(float_arr);
    added_ref = cJSON_AddItemToObject(root, "floats_ref", arr_ref);
    num_item = cJSON_AddNumberToObject(root, "count", (double)count);
    added_num_bool = (num_item != (cJSON *)0);

    // step 4: Operate
    array_size = cJSON_GetArraySize(float_arr);
    first_elem = cJSON_GetArrayItem(float_arr, 0);
    first_is_number = cJSON_IsNumber(first_elem);
    first_val = cJSON_GetNumberValue(first_elem);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0)
        + (int)added_float
        + (int)added_ref
        + (int)added_num_bool
        + (int)(array_size == count)
        + (int)first_is_number
        + (int)(first_val > 0.0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)first_val;
    (void)first_elem;
    (void)num_item;
    // API sequence test completed successfully
    return 66;
}