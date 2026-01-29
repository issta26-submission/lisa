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
//<ID> 160
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *str_item = cJSON_CreateString("alpha");
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_InsertItemInArray(arr, 0, str_item);
    cJSON_InsertItemInArray(arr, 1, num_item);
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *cfg = cJSON_AddObjectToObject(root, "config");
    cJSON *enabled = cJSON_AddTrueToObject(cfg, "enabled");

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_InsertItemInArray(arr, 0, detached);
    cJSON_bool is_bool = cJSON_IsBool(enabled);
    int array_size = cJSON_GetArraySize(arr);
    int summary = array_size + (int)is_bool + root->type + cfg->type + detached->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}