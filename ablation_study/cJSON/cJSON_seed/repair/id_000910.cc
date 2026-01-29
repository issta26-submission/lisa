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
//<ID> 910
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
    cJSON *flag_true = cJSON_CreateBool(1);
    cJSON_AddItemToArray(items, flag_true);
    cJSON *flag_false = cJSON_CreateBool(0);
    cJSON_AddItemToArray(items, flag_false);
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *label = cJSON_CreateString("old_label");
    cJSON_AddItemToObject(config, "label", label);

    // step 3: Operate and Validate
    cJSON *replacement_label = cJSON_CreateString("new_label");
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(config, "label", replacement_label);
    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "count", (double)count);
    cJSON_AddNumberToObject(root, "replaced_flag", (double)replaced);
    cJSON *detached = cJSON_DetachItemFromObject(root, "config");
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(detached);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}