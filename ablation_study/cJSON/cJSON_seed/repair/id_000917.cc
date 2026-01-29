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
//<ID> 917
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToArray(items, flag);
    cJSON *name = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *count = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "count", count);
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *sub = cJSON_CreateObject();
    cJSON_AddItemToObject(config, "sub", sub);
    cJSON *label = cJSON_CreateString("old_label");
    cJSON_AddItemToObject(sub, "label", label);

    // step 3: Operate and Validate
    int array_size = cJSON_GetArraySize(items);
    cJSON *size_num = cJSON_CreateNumber((double)array_size);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "count", size_num);
    cJSON *detached_name = cJSON_DetachItemFromObject(root, "name");
    cJSON_AddItemToArray(items, detached_name);
    cJSON *replacement_label = cJSON_CreateString("new_label");
    cJSON_ReplaceItemInObjectCaseSensitive(sub, "label", replacement_label);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *dup_snapshot = cJSON_PrintUnformatted(dup);
    cJSON_free(dup_snapshot);
    cJSON_Delete(dup);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}