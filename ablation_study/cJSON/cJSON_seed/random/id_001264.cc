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
//<ID> 1264
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
    cJSON *auto_bool = (cJSON *)0;
    float numbers[5] = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f};
    int size_before = 0;
    int size_after = 0;
    cJSON_bool is_array = 0;
    cJSON_bool is_bool = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    float_arr = cJSON_CreateFloatArray(numbers, 5);

    // step 3: Configure
    arr_ref = cJSON_CreateArrayReference((const cJSON *)float_arr);
    cJSON_AddItemToObject(root, "values_ref", arr_ref);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemReferenceToObject(root, "manual_flag", false_item);
    auto_bool = cJSON_AddBoolToObject(root, "auto_flag", (cJSON_bool)1);

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr_ref);
    is_array = cJSON_IsArray(arr_ref);
    is_bool = cJSON_IsBool(auto_bool);
    size_after = cJSON_GetArraySize(float_arr);

    // step 5: Validate
    validation_score = size_before + size_after + (int)is_array + (int)is_bool + (int)(root != (cJSON *)0) + (int)(float_arr != (cJSON *)0) + (int)(arr_ref != (cJSON *)0) + (int)(false_item != (cJSON *)0) + (int)(auto_bool != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}