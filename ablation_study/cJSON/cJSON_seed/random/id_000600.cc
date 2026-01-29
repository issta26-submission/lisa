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
//<ID> 600
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *arr1 = (cJSON *)0;
    cJSON *arr2 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *item_after = (cJSON *)0;
    cJSON *single_arr = (cJSON *)0;
    int nums[3];
    int single_val = 0;
    double item_after_val = 0.0;
    cJSON_bool eq_before = 0;
    cJSON_bool eq_after = 0;

    // step 2: Initialize
    nums[0] = 10;
    nums[1] = 20;
    nums[2] = 30;
    arr1 = cJSON_CreateIntArray(nums, 3);
    arr2 = cJSON_CreateIntArray(nums, 3);

    // step 3: Configure
    (void)0; // placeholder to keep straight-line sequence; no branching allowed

    // step 4: Operate
    eq_before = cJSON_Compare(arr1, arr2, 1);
    detached = cJSON_DetachItemFromArray(arr1, 1); // detach the middle element (20)
    item_after = cJSON_GetArrayItem(arr1, 1);     // now should point to the former third element (30)
    item_after_val = cJSON_GetNumberValue(item_after);
    single_val = (int)item_after_val;
    single_arr = cJSON_CreateIntArray(&single_val, 1);
    eq_after = cJSON_Compare(arr1, arr2, 1);

    // step 5: Validate
    (void)eq_before;
    (void)eq_after;
    (void)item_after_val;
    (void)single_arr;
    (void)item_after;

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(arr1);
    cJSON_Delete(arr2);
    cJSON_Delete(single_arr);

    // API sequence test completed successfully
    return 66;
}