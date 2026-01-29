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
//<ID> 374
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
    cJSON *elem_index1 = (cJSON *)0;
    cJSON *detached_elem = (cJSON *)0;
    cJSON *got_arr_from_obj = (cJSON *)0;
    double nums[3] = { 1.1, 2.2, 3.3 };
    double value_index1 = 0.0;
    double value_detached = 0.0;
    const char *arr_key = "arr";

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateDoubleArray(nums, 3);

    // step 3: Configure
    cJSON_AddItemToObject(root, arr_key, arr);

    // step 4: Operate
    elem_index1 = cJSON_GetArrayItem(arr, 1);
    detached_elem = cJSON_DetachItemFromArray(arr, 2);
    got_arr_from_obj = cJSON_GetObjectItem(root, arr_key);
    value_index1 = cJSON_GetNumberValue(elem_index1);
    value_detached = cJSON_GetNumberValue(detached_elem);

    // step 5: Validate
    double combined = value_index1 + value_detached;
    (void)combined;
    (void)got_arr_from_obj;
    (void)value_index1;
    (void)value_detached;

    // step 6: Cleanup
    cJSON_Delete(detached_elem);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}