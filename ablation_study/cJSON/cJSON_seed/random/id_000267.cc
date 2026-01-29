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
//<ID> 267
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
    cJSON *nums_arr = (cJSON *)0;
    cJSON *num_item0 = (cJSON *)0;
    cJSON *num_item1 = (cJSON *)0;
    cJSON *detached_nums = (cJSON *)0;
    cJSON *dup_float_arr = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool arrays_equal = (cJSON_bool)0;
    float floats[3];

    // step 2: Initialize
    floats[0] = 1.25f;
    floats[1] = 2.5f;
    floats[2] = 3.75f;
    root = cJSON_CreateObject();
    float_arr = cJSON_CreateFloatArray(floats, 3);
    cJSON_AddItemToObject(root, "float_array", float_arr);

    // step 3: Configure
    nums_arr = cJSON_CreateArray();
    num_item0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(nums_arr, num_item0);
    num_item1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(nums_arr, num_item1);
    cJSON_AddItemToObject(root, "numbers", nums_arr);

    // step 4: Operate
    detached_nums = cJSON_DetachItemFromObjectCaseSensitive(root, "numbers");
    dup_float_arr = cJSON_Duplicate(float_arr, (cJSON_bool)1);
    arrays_equal = cJSON_Compare(float_arr, dup_float_arr, (cJSON_bool)1);
    cJSON_AddBoolToObject(root, "float_arrays_equal", arrays_equal);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached_nums);
    cJSON_Delete(dup_float_arr);

    // API sequence test completed successfully
    return 66;
}