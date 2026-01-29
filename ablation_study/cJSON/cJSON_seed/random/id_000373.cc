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
//<ID> 373
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *dbl_arr = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *got_arr = (cJSON *)0;
    cJSON *item_at_index = (cJSON *)0;
    cJSON *detached_item = (cJSON *)0;
    cJSON *got_meta = (cJSON *)0;
    double nums[3];
    double extracted_value = 0.0;
    int validation_flag = 0;

    // step 2: Initialize
    nums[0] = 1.1;
    nums[1] = 2.2;
    nums[2] = 3.3;
    root = cJSON_CreateObject();
    dbl_arr = cJSON_CreateDoubleArray(nums, 3);
    meta = cJSON_CreateObject();

    // step 3: Configure
    cJSON_AddItemToObject(root, "values", dbl_arr);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 4: Operate
    got_arr = cJSON_GetObjectItem(root, "values");
    item_at_index = cJSON_GetArrayItem(got_arr, 1);
    detached_item = cJSON_DetachItemFromArray(got_arr, 1);
    got_meta = cJSON_GetObjectItem(root, "meta");
    extracted_value = cJSON_GetNumberValue(detached_item);

    // step 5: Validate
    validation_flag = ((int)(extracted_value > 0.0)) + ((int)(got_meta != (cJSON *)0));
    (void)validation_flag;
    (void)item_at_index;
    (void)got_arr;

    // step 6: Cleanup
    cJSON_Delete(detached_item);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}