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
//<ID> 1262
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
    cJSON *num_ref_extra = (cJSON *)0;
    cJSON *num_ref_root = (cJSON *)0;
    cJSON_bool is_array = 0;
    int size_floats = 0;
    int size_refs = 0;
    int validation_score = 0;
    float nums[3] = {1.25f, 2.5f, 3.75f};

    // step 2: Initialize - create root object, a float array, and a numeric item to be referenced
    root = cJSON_CreateObject();
    float_arr = cJSON_CreateFloatArray((const float *)nums, 3);
    num_ref_root = cJSON_CreateNumber(7.7);

    // step 3: Configure - create a false item, create an array reference to the numeric item, and attach items
    false_item = cJSON_CreateFalse();
    arr_ref = cJSON_CreateArrayReference((const cJSON *)num_ref_root);
    cJSON_AddItemToArray(float_arr, false_item);
    num_ref_extra = cJSON_CreateNumber(8.8);
    cJSON_AddItemToArray(arr_ref, num_ref_extra);
    cJSON_AddItemToObject(root, "floats", float_arr);
    cJSON_AddItemToObject(root, "refs", arr_ref);
    cJSON_AddBoolToObject(root, "enabled", (cJSON_bool)1);

    // step 4: Operate - inspect arrays and items
    is_array = cJSON_IsArray(float_arr);
    size_floats = cJSON_GetArraySize(float_arr);
    size_refs = cJSON_GetArraySize(arr_ref);

    // step 5: Validate - compute a simple validation score to ensure data flowed through APIs
    validation_score = (int)is_array + size_floats + size_refs + (int)(root != (cJSON *)0) + (int)(float_arr != (cJSON *)0) + (int)(arr_ref != (cJSON *)0) + (int)(false_item != (cJSON *)0) + (int)(num_ref_root != (cJSON *)0) + (int)(num_ref_extra != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}