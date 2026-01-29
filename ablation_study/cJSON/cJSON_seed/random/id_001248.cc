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
//<ID> 1248
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *arr = (cJSON *)0;
    cJSON *extra = (cJSON *)0;
    cJSON *item_at2 = (cJSON *)0;
    int nums[4] = {10, 20, 30, 40};
    int count = 4;
    cJSON_bool is_array = 0;
    double extracted_val = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create an integer array from a C array
    arr = cJSON_CreateIntArray(nums, count);

    // step 3: Configure - create an extra numeric item and append it to the array
    extra = cJSON_CreateNumber(99);
    cJSON_AddItemToArray(arr, extra);

    // step 4: Operate - verify the node is an array, extract the third element value
    is_array = cJSON_IsArray(arr);
    item_at2 = cJSON_GetArrayItem(arr, 2);
    extracted_val = cJSON_GetNumberValue(item_at2);
    validation_score = (int)is_array + (int)(extracted_val == 30.0) + (int)(arr != (cJSON *)0) + (int)(extra != (cJSON *)0);

    // step 5: Cleanup - remove one item by index and free the entire array structure
    cJSON_DeleteItemFromArray(arr, 1);
    cJSON_Delete(arr);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}