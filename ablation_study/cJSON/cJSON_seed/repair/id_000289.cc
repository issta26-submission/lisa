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
//<ID> 289
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double nums[3] = {1.0, 2.0, 3.0};
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateDoubleArray(nums, 3);

    // step 2: Configure
    cJSON_AddFalseToObject(root, "enabled");
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 3: Operate and Validate
    cJSON *flag = cJSON_GetObjectItem(root, "enabled");
    int flag_true = (int)cJSON_IsTrue(flag);
    cJSON *extra = cJSON_CreateNumber(10.0 * (1 + flag_true));
    cJSON_AddItemToArray(arr, extra);
    cJSON *numbers_item = cJSON_GetObjectItem(root, "numbers");
    int size = cJSON_GetArraySize(numbers_item);
    double last_val = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers_item, size - 1));
    (void)last_val;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}