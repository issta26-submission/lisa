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
//<ID> 1590
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
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_ref = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    const float input_values[3] = { 1.0f, 2.0f, 3.5f };
    cJSON_bool added_arr = 0;
    cJSON_bool added_ref = 0;
    int size_arr = 0;
    int size_ref = 0;
    double first_val = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    float_arr = cJSON_CreateFloatArray(input_values, 3);

    // step 3: Configure
    arr_ref = cJSON_CreateArrayReference(float_arr);
    added_arr = cJSON_AddItemToObject(root, "values", float_arr);
    added_ref = cJSON_AddItemToObject(root, "values_ref", arr_ref);
    num_item = cJSON_AddNumberToObject(root, "pi", 3.1415);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    retrieved_ref = cJSON_GetObjectItem(root, "values_ref");
    size_arr = cJSON_GetArraySize(retrieved_arr);
    size_ref = cJSON_GetArraySize(retrieved_ref);
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    first_val = cJSON_GetNumberValue(first_elem);

    // step 5: Validate
    validation_score = (int)added_arr
        + (int)added_ref
        + (int)(num_item != (cJSON *)0)
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(retrieved_ref != (cJSON *)0)
        + (int)(size_arr == 3)
        + (int)(size_ref == 3)
        + (int)(first_elem != (cJSON *)0)
        + (int)(first_val == 1.0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)size_arr;
    (void)size_ref;
    (void)first_val;
    // API sequence test completed successfully
    return 66;
}