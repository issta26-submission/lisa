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
//<ID> 911
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
    cJSON *item1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, item1);
    cJSON *item2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, item2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("node_name");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(meta, "enabled", enabled);

    // step 3: Operate and Validate
    int initial_count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "initial_count", (double)initial_count);
    cJSON *replacement_bool = cJSON_CreateBool(1);
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(meta, "enabled", replacement_bool);
    (void)replaced;
    cJSON *detached_name = cJSON_DetachItemFromObject(meta, "name");
    cJSON_AddItemToArray(items, detached_name);
    int final_count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "final_count", (double)final_count);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}