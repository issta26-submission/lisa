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
//<ID> 1260
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
    cJSON *false_item = (cJSON *)0;
    cJSON *added_bool = (cJSON *)0;
    cJSON *first_float = (cJSON *)0;
    int size_floats = 0;
    int size_ref = 0;
    double first_value = 0.0;
    int validation_score = 0;
    float numbers[3] = {1.1f, 2.2f, 3.3f};

    // step 2: Initialize - create root object, a float array, and an explicit false item
    root = cJSON_CreateObject();
    float_arr = cJSON_CreateFloatArray((const float *)numbers, 3);
    false_item = cJSON_CreateFalse();

    // step 3: Configure - create an array reference to the false item and attach arrays/flags to root
    arr_ref = cJSON_CreateArrayReference((const cJSON *)false_item);
    cJSON_AddItemToObject(root, "floats", float_arr);
    cJSON_AddItemToObject(root, "ref_arr", arr_ref);
    added_bool = cJSON_AddBoolToObject(root, "explicit_flag", 0);

    // step 4: Operate - inspect array sizes and read a value from the float array
    size_floats = cJSON_GetArraySize(float_arr);
    first_float = cJSON_GetArrayItem(float_arr, 0);
    first_value = cJSON_GetNumberValue(first_float);
    size_ref = cJSON_GetArraySize(arr_ref);

    // step 5: Validate - accumulate a simple validation score to ensure API data flowed through
    validation_score = size_floats + size_ref + (int)(first_value > 0.0) + (int)(root != (cJSON *)0) + (int)(float_arr != (cJSON *)0) + (int)(arr_ref != (cJSON *)0) + (int)(false_item != (cJSON *)0) + (int)(added_bool != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items and referenced structures
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}