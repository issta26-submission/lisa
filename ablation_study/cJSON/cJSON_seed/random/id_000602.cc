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
//<ID> 602
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
    cJSON *first_item = (cJSON *)0;
    cJSON *detached_item = (cJSON *)0;
    cJSON *single_arr = (cJSON *)0;
    cJSON *cloned_item = (cJSON *)0;
    cJSON_bool compare_result = 0;
    int nums[3];
    int single_value = 0;

    // step 2: Initialize
    nums[0] = 10;
    nums[1] = 20;
    nums[2] = 30;
    root = cJSON_CreateObject();
    arr = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 3: Configure
    first_item = cJSON_GetArrayItem(arr, 0);
    detached_item = cJSON_DetachItemFromArray(arr, 1);

    // step 4: Operate
    single_value = detached_item ? detached_item->valueint : 0;
    single_arr = cJSON_CreateIntArray(&single_value, 1);
    cloned_item = cJSON_GetArrayItem(single_arr, 0);

    // step 5: Validate
    compare_result = cJSON_Compare(detached_item, cloned_item, 1);
    (void)first_item;
    (void)compare_result;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(single_arr);
    cJSON_Delete(detached_item);

    // API sequence test completed successfully
    return 66;
}