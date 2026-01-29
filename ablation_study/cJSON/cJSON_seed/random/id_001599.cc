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
//<ID> 1599
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
    cJSON *num_count = (cJSON *)0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_ref = 0;
    float numbers[4];
    int count = 4;
    int validation_score = 0;

    // step 2: Initialize
    numbers[0] = 3.14f;
    numbers[1] = -1.5f;
    numbers[2] = 0.0f;
    numbers[3] = 42.0f;
    root = cJSON_CreateObject();
    float_arr = cJSON_CreateFloatArray(numbers, count);

    // step 3: Configure
    added_arr = cJSON_AddItemToObject(root, "values", float_arr);
    arr_ref = cJSON_CreateArrayReference(float_arr);
    added_ref = cJSON_AddItemToObject(root, "values_ref", arr_ref);
    num_count = cJSON_AddNumberToObject(root, "count", (double)count);

    // step 4: Operate
    (void)root;
    (void)float_arr;
    (void)arr_ref;
    (void)num_count;

    // step 5: Validate
    validation_score = (int)added_arr
        + (int)added_ref
        + (int)(float_arr != (cJSON *)0)
        + (int)(arr_ref != (cJSON *)0)
        + (int)(num_count != (cJSON *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    // API sequence test completed successfully
    return 66;
}