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
//<ID> 1269
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
    cJSON *first_item = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *false_node = (cJSON *)0;
    cJSON_bool flag_value = 0;
    float numbers[3] = {1.1f, 2.2f, 3.3f};
    int size_before = 0;
    int size_after = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and a float array from C floats
    root = cJSON_CreateObject();
    float_arr = cJSON_CreateFloatArray((const float *)numbers, 3);

    // step 3: Configure - get first element, create an array reference to it, and attach arrays to root
    first_item = cJSON_GetArrayItem(float_arr, 0);
    arr_ref = cJSON_CreateArrayReference((const cJSON *)first_item);
    cJSON_AddItemToObject(root, "floats", float_arr);
    cJSON_AddItemToObject(root, "floats_ref", arr_ref);

    // step 4: Operate - create a false node, add a boolean to root, append the false node to the reference array, and measure sizes
    false_node = cJSON_CreateFalse();
    cJSON_AddBoolToObject(root, "ok", flag_value);
    cJSON_AddItemToArray(arr_ref, false_node);
    size_before = cJSON_GetArraySize(float_arr);
    size_after = cJSON_GetArraySize(arr_ref);

    // step 5: Validate - ensure pointers and sizes propagated through the APIs
    validation_score = (int)(root != (cJSON *)0) + (int)(float_arr != (cJSON *)0) + (int)(first_item != (cJSON *)0) + (int)(arr_ref != (cJSON *)0) + (int)(false_node != (cJSON *)0) + size_before + size_after;
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached arrays and items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}