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
//<ID> 1268
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
    cJSON *standalone = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_array = 0;
    int size_before = 0;
    int printed_flag = 0;
    int validation_score = 0;
    float numbers[4] = {1.1f, 2.2f, 3.3f, 4.4f};
    cJSON_bool enabled_flag = 0;

    // step 2: Initialize - create a root object and a float array from C floats
    root = cJSON_CreateObject();
    float_arr = cJSON_CreateFloatArray((const float *)numbers, 4);

    // step 3: Configure - create a standalone number, create an array reference to it, and attach arrays/booleans
    standalone = cJSON_CreateNumber(5.5);
    arr_ref = cJSON_CreateArrayReference((const cJSON *)standalone);
    cJSON_AddItemToArray(arr_ref, cJSON_CreateNumber(6.6));
    cJSON_AddItemToObject(root, "reference", arr_ref);
    cJSON_AddItemToObject(root, "measurements", float_arr);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(float_arr, false_item);
    cJSON_AddBoolToObject(root, "enabled", enabled_flag);

    // step 4: Operate - inspect array and produce an unformatted print
    size_before = cJSON_GetArraySize(float_arr);
    is_array = cJSON_IsArray(float_arr);
    printed = cJSON_PrintUnformatted(root);
    printed_flag = (int)(printed != (char *)0);

    // step 5: Validate - combine checks into a simple validation score
    validation_score = size_before + (int)is_array + (int)(arr_ref != (cJSON *)0) + (int)(false_item != (cJSON *)0) + printed_flag + (int)(root != (cJSON *)0) + (int)(float_arr != (cJSON *)0) + (int)(standalone != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - free printed buffer and delete root (which frees attached items)
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}