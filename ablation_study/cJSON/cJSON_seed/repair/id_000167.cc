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
//<ID> 167
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nullable", null_item);

    // step 2: Configure
    int *nums = (int*)cJSON_malloc(sizeof(int) * 2);
    nums[0] = 7;
    nums[1] = 3;
    cJSON *nums_item = cJSON_CreateIntArray(nums, 2);
    cJSON_AddItemToObject(root, "numbers", nums_item);
    cJSON_free(nums);
    cJSON *replacement = cJSON_CreateNumber(100.5);
    cJSON_ReplaceItemViaPointer(root, null_item, replacement);

    // step 3: Operate and Validate
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON_AddItemToObject(root, "duplicate", dup_root);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}