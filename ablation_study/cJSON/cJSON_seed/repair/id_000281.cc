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
//<ID> 281
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double nums[3] = {1.1, 2.2, 3.3};
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "values", arr);

    // step 2: Configure
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, true_item);
    cJSON *false_item = cJSON_AddFalseToObject(root, "enabled");

    // step 3: Operate and Validate
    cJSON_bool is_true_flag = cJSON_IsTrue(true_item);
    cJSON_AddNumberToObject(root, "is_true_numeric", (double)is_true_flag);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}