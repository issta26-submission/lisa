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
//<ID> 268
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
    cJSON *arr2 = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON_bool cmp_equal = (cJSON_bool)0;
    char *printed = (char *)0;
    float nums[3];

    // step 2: Initialize
    nums[0] = 1.1f;
    nums[1] = 2.2f;
    nums[2] = 3.3f;
    root = cJSON_CreateObject();
    arr = cJSON_CreateFloatArray(nums, 3);
    arr2 = cJSON_CreateFloatArray(nums, 3);

    // step 3: Configure
    cJSON_AddItemToObject(root, "floats", arr);
    num_item = cJSON_CreateNumber(4.4);
    cJSON_AddItemToArray(arr, num_item);

    // step 4: Operate
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "floats");
    cmp_equal = cJSON_Compare(detached, arr2, (cJSON_bool)1);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(detached);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(arr2);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}