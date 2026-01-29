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
//<ID> 1592
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
    cJSON *array_ref = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *pi_item = (cJSON *)0;
    cJSON *num_ptr = (cJSON *)0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_ref = 0;
    cJSON_bool is_pi_number = 0;
    int arr_size = 0;
    double first_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    {
        float values[3];
        values[0] = 1.5f;
        values[1] = -2.0f;
        values[2] = 3.25f;
        float_arr = cJSON_CreateFloatArray(values, 3);
    }
    num_ptr = cJSON_AddNumberToObject(root, "pi", 3.1415);

    // step 3: Configure
    array_ref = cJSON_CreateArrayReference(float_arr);
    added_arr = cJSON_AddItemToObject(root, "data", float_arr);
    added_ref = cJSON_AddItemToObject(root, "data_ref", array_ref);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "data");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    first_value = cJSON_GetNumberValue(first_elem);
    pi_item = cJSON_GetObjectItem(root, "pi");
    is_pi_number = cJSON_IsNumber(pi_item);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0)
        + (int)(float_arr != (cJSON *)0)
        + (int)added_arr
        + (int)(array_ref != (cJSON *)0)
        + (int)added_ref
        + (int)(arr_size == 3)
        + (int)(first_elem != (cJSON *)0)
        + (int)(first_value == 1.5)
        + (int)(num_ptr != (cJSON *)0)
        + (int)is_pi_number;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_arr;
    (void)first_elem;
    (void)pi_item;
    (void)num_ptr;
    // API sequence test completed successfully
    return 66;
}