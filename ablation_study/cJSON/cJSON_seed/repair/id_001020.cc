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
//<ID> 1020
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int *nums = (int *)cJSON_malloc(sizeof(int) * 3);
    memset(nums, 0, sizeof(int) * 3);
    nums[0] = 7;
    nums[1] = 13;
    nums[2] = 21;
    cJSON *values_array = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "values", values_array);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "added_direct", 42.5);
    cJSON *retrieved_array = cJSON_GetObjectItem(root, "values");
    cJSON_bool array_check = cJSON_IsArray(retrieved_array);
    cJSON_AddNumberToObject(root, "is_array_flag", (double)array_check);

    // step 3: Operate and Validate
    cJSON *direct_item = cJSON_GetObjectItem(root, "added_direct");
    double direct_val = cJSON_GetNumberValue(direct_item);
    double combined = direct_val + (double)nums[0];
    cJSON_AddNumberToObject(root, "sum", combined);

    // step 4: Cleanup
    cJSON_free(nums);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}