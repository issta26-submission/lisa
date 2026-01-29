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
//<ID> 606
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
    cJSON *retr = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *single_arr = (cJSON *)0;
    cJSON *single_item = (cJSON *)0;
    cJSON *after_item = (cJSON *)0;
    int nums[3] = {1, 2, 3};
    int single_vals[1] = {0};
    double detached_val = 0.0;
    cJSON_bool cmp_equal = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "ints", arr);

    // step 3: Configure
    retr = cJSON_GetObjectItem(root, "ints");
    item0 = cJSON_GetArrayItem(retr, 0);

    // step 4: Operate
    detached = cJSON_DetachItemFromArray(retr, 1);
    detached_val = cJSON_GetNumberValue(detached);
    single_vals[0] = (int)detached_val;
    single_arr = cJSON_CreateIntArray(single_vals, 1);
    single_item = cJSON_GetArrayItem(single_arr, 0);
    cmp_equal = cJSON_Compare(detached, single_item, 1);
    after_item = cJSON_GetArrayItem(retr, 1);

    // step 5: Validate
    (void)item0;
    (void)detached_val;
    (void)after_item;
    (void)cmp_equal;

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(single_arr);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}