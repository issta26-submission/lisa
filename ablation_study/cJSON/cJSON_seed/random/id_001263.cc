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
//<ID> 1263
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
    cJSON *first_elem = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    float numbers[4] = {1.0f, 2.5f, -3.75f, 4.125f};
    cJSON_bool is_ref_array = 0;
    int size_float_arr = 0;
    int size_arr_ref = 0;
    double first_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object and a float array
    root = cJSON_CreateObject();
    float_arr = cJSON_CreateFloatArray(numbers, 4);

    // step 3: Configure - create an array reference to the first element and a false item; attach items
    first_elem = cJSON_GetArrayItem(float_arr, 0);
    arr_ref = cJSON_CreateArrayReference((const cJSON *)first_elem);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr_ref, false_item);
    cJSON_AddItemToObject(root, "floats", float_arr);
    cJSON_AddItemToObject(root, "ref", arr_ref);
    cJSON_AddBoolToObject(root, "enabled", 0);

    // step 4: Operate - inspect sizes and values
    size_float_arr = cJSON_GetArraySize(float_arr);
    size_arr_ref = cJSON_GetArraySize(arr_ref);
    is_ref_array = cJSON_IsArray(arr_ref);
    first_value = cJSON_GetNumberValue(first_elem);

    // step 5: Validate - compute a validation score
    validation_score = size_float_arr + size_arr_ref + (int)is_ref_array + (int)(root != (cJSON *)0) + (int)(float_arr != (cJSON *)0) + (int)(arr_ref != (cJSON *)0) + (int)(false_item != (cJSON *)0) + (int)(first_value != 0.0);
    (void)validation_score;
    (void)first_value;

    // step 6: Cleanup - delete root to free everything
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}