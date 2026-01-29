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
//<ID> 1266
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    cJSON *dup_first = (cJSON *)0;
    cJSON *false_node = (cJSON *)0;
    cJSON *got_item = (cJSON *)0;
    float numbers[3] = {1.5f, 2.5f, 3.5f};
    int size_arr = 0;
    int size_ref = 0;
    cJSON_bool is_array = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and a float array
    root = cJSON_CreateObject();
    arr = cJSON_CreateFloatArray((const float *)numbers, 3);

    // step 3: Configure - duplicate first element, create an array reference to it, and create a false item
    first = cJSON_GetArrayItem(arr, 0);
    dup_first = cJSON_Duplicate((const cJSON *)first, 1);
    arr_ref = cJSON_CreateArrayReference((const cJSON *)dup_first);
    false_node = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr_ref, false_node);
    cJSON_AddItemToObject(root, "floats", arr);
    cJSON_AddItemToObject(root, "ref", arr_ref);
    cJSON_AddBoolToObject(root, "explicit_false", 0);

    // step 4: Operate - inspect sizes and types, retrieve items
    size_arr = cJSON_GetArraySize(arr);
    size_ref = cJSON_GetArraySize(arr_ref);
    is_array = cJSON_IsArray(arr_ref);
    got_item = cJSON_GetArrayItem(arr_ref, 0);

    // step 5: Validate - compute a simple validation score ensuring data flowed through APIs
    validation_score = size_arr + size_ref + (int)is_array + (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(arr_ref != (cJSON *)0) + (int)(got_item != (cJSON *)0) + (int)(false_node != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete the root (which frees attached arrays and items)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}