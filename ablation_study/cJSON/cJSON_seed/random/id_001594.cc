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
//<ID> 1594
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *float_array = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *count_num = (cJSON *)0;
    cJSON *retrieved_floats = (cJSON *)0;
    cJSON *retrieved_ref = (cJSON *)0;
    cJSON_bool added_array = 0;
    cJSON_bool added_ref = 0;
    cJSON_bool count_is_number = 0;
    int size = 0;
    int validation_score = 0;
    float numbers[3];

    // step 2: Initialize
    numbers[0] = 1.5f;
    numbers[1] = -2.25f;
    numbers[2] = 3.75f;
    root = cJSON_CreateObject();
    float_array = cJSON_CreateFloatArray(numbers, 3);

    // step 3: Configure
    added_array = cJSON_AddItemToObject(root, "floats", float_array);
    arr_ref = cJSON_CreateArrayReference(float_array);
    added_ref = cJSON_AddItemToObject(root, "floats_ref", arr_ref);
    count_num = cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(float_array));

    // step 4: Operate
    retrieved_floats = cJSON_GetObjectItem(root, "floats");
    retrieved_ref = cJSON_GetObjectItem(root, "floats_ref");
    size = cJSON_GetArraySize(retrieved_floats);
    count_is_number = cJSON_IsNumber(count_num);

    // step 5: Validate
    validation_score = (int)added_array
        + (int)added_ref
        + (int)(retrieved_floats != (cJSON *)0)
        + (int)(retrieved_ref != (cJSON *)0)
        + (int)(size == 3)
        + (int)count_is_number
        + (int)(count_num != (cJSON *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)numbers;
    (void)size;
    // API sequence test completed successfully
    return 66;
}