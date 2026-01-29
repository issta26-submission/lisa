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
//<ID> 377
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
    cJSON *item_at_index = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    double nums[3];
    double extracted_value = 0.0;

    // step 2: Initialize
    nums[0] = 1.5;
    nums[1] = 2.5;
    nums[2] = 3.5;
    root = cJSON_CreateObject();
    arr = cJSON_CreateDoubleArray(nums, 3);

    // step 3: Configure
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate
    got_arr = cJSON_GetObjectItem(root, "numbers");
    item_at_index = cJSON_GetArrayItem(got_arr, 1);
    detached = cJSON_DetachItemFromArray(got_arr, 1);

    // step 5: Validate
    extracted_value = cJSON_GetNumberValue(detached);
    (void)item_at_index;
    (void)extracted_value;

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}