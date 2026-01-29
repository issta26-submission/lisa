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
//<ID> 163
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *placeholder = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "data", placeholder);
    cJSON *node_new = cJSON_CreateObject();
    cJSON_AddItemToObject(node_new, "status", cJSON_CreateString("ok"));

    // step 2: Configure
    int *nums = (int *)cJSON_malloc(2 * sizeof(int));
    nums[0] = 10;
    nums[1] = 20;
    cJSON *arr = cJSON_CreateIntArray(nums, 2);
    cJSON_AddItemToObject(node_new, "values", arr);
    cJSON_free(nums);

    // step 3: Operate and Validate
    cJSON *dup_node = cJSON_Duplicate(node_new, 1);
    cJSON_ReplaceItemViaPointer(root, placeholder, dup_node);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(node_new);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}