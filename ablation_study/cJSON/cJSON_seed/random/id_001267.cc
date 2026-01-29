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
//<ID> 1267
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *flt_arr = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *bool_node = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *third_elem = (cJSON *)0;
    cJSON *got_bool = (cJSON *)0;
    int size_before = 0;
    int validation_score = 0;
    cJSON_bool is_array = 0;
    cJSON_bool is_false = 0;
    cJSON_bool is_bool = 0;
    float numbers[3] = {1.5f, 2.5f, 3.5f};

    // step 2: Initialize - create root object and a float array from C floats
    root = cJSON_CreateObject();
    flt_arr = cJSON_CreateFloatArray((const float *)numbers, 3);

    // step 3: Configure - create an array reference to the float array, append a false item, attach to root, and add a boolean property
    arr_ref = cJSON_CreateArrayReference((const cJSON *)flt_arr);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr_ref, false_item);
    cJSON_AddItemToObject(root, "data", arr_ref);
    bool_node = cJSON_AddBoolToObject(root, "active", 1);

    // step 4: Operate - inspect array size and elements, and check boolean types
    size_before = cJSON_GetArraySize(arr_ref);
    first_elem = cJSON_GetArrayItem(arr_ref, 0);
    third_elem = cJSON_GetArrayItem(arr_ref, 2);
    is_array = cJSON_IsArray(arr_ref);
    is_false = cJSON_IsFalse(false_item);
    got_bool = cJSON_GetObjectItem(root, "active");
    is_bool = cJSON_IsBool(got_bool);

    // step 5: Validate - compute a simple validation score to ensure data flowed through APIs
    validation_score = size_before + (int)(first_elem != (cJSON *)0) + (int)(third_elem != (cJSON *)0) + (int)is_array + (int)is_false + (int)is_bool + (int)(root != (cJSON *)0) + (int)(flt_arr != (cJSON *)0) + (int)(arr_ref != (cJSON *)0) + (int)(false_item != (cJSON *)0) + (int)(bool_node != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}