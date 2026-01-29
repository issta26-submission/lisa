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
//<ID> 375
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
    cJSON *got_arr = (cJSON *)0;
    cJSON *item_idx2 = (cJSON *)0;
    cJSON *detached_item = (cJSON *)0;
    double nums[4];
    double val_index2 = 0.0;
    double val_detached = 0.0;
    double combined = 0.0;

    // step 2: Initialize
    nums[0] = 1.1;
    nums[1] = 2.2;
    nums[2] = 3.3;
    nums[3] = 4.4;
    root = cJSON_CreateObject();
    arr = cJSON_CreateDoubleArray(nums, 4);

    // step 3: Configure
    cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate
    got_arr = cJSON_GetObjectItem(root, "values");
    item_idx2 = cJSON_GetArrayItem(got_arr, 2);
    val_index2 = cJSON_GetNumberValue(item_idx2);
    detached_item = cJSON_DetachItemFromArray(got_arr, 1);
    val_detached = cJSON_GetNumberValue(detached_item);

    // step 5: Validate
    combined = val_index2 + val_detached;
    (void)combined;
    (void)val_index2;
    (void)val_detached;

    // step 6: Cleanup
    cJSON_Delete(detached_item);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}