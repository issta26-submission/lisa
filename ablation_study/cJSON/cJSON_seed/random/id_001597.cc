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
//<ID> 1597
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
    cJSON *retrieved_ref = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *direct_num = (cJSON *)0;
    cJSON_bool added_ref = 0;
    cJSON_bool added_number = 0;
    int arr_size = 0;
    double first_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    {
        float values[3];
        values[0] = 1.5f;
        values[1] = -2.25f;
        values[2] = 3.75f;
        float_arr = cJSON_CreateFloatArray(values, 3);
    }

    // step 3: Configure
    arr_ref = cJSON_CreateArrayReference(float_arr);
    added_ref = cJSON_AddItemToObject(root, "floats_ref", arr_ref);
    direct_num = cJSON_AddNumberToObject(root, "expected_count", 3.0);
    added_number = (direct_num != (cJSON *)0);

    // step 4: Operate
    retrieved_ref = cJSON_GetObjectItem(root, "floats_ref");
    arr_size = cJSON_GetArraySize(retrieved_ref);
    first_elem = cJSON_GetArrayItem(retrieved_ref, 0);
    first_value = cJSON_GetNumberValue(first_elem);

    // step 5: Validate
    validation_score = (int)added_ref
        + (int)added_number
        + (int)(retrieved_ref != (cJSON *)0)
        + (int)(arr_size == 3)
        + (int)(first_elem != (cJSON *)0)
        + (int)(first_value == 1.5);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)float_arr;
    (void)first_value;
    (void)validation_score;
    (void)direct_num;
    // API sequence test completed successfully
    return 66;
}