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
//<ID> 1265
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
    int size_before = 0;
    int size_ref = 0;
    int validation_score = 0;
    float numbers[5] = {1.0f, 2.5f, -3.75f, 4.0f, 5.25f};

    // step 2: Initialize - create a float array and a standalone false item
    float_arr = cJSON_CreateFloatArray(numbers, 5);
    false_item = cJSON_CreateFalse();

    // step 3: Configure - create a reference to the first element and assemble into a root object
    first_elem = cJSON_GetArrayItem(float_arr, 0);
    arr_ref = cJSON_CreateArrayReference((const cJSON *)first_elem);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "floats", float_arr);
    cJSON_AddItemToObject(root, "first_ref", arr_ref);
    cJSON_AddItemToArray(float_arr, false_item);
    cJSON_AddBoolToObject(root, "explicit_false", 0);

    // step 4: Operate - inspect sizes of the arrays
    size_before = cJSON_GetArraySize(float_arr);
    size_ref = cJSON_GetArraySize(arr_ref);

    // step 5: Validate - compute a simple validation score to ensure flow through APIs
    validation_score = size_before + size_ref + (int)(float_arr != (cJSON *)0) + (int)(arr_ref != (cJSON *)0) + (int)(false_item != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}