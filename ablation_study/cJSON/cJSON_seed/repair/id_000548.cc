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
//<ID> 548
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *name = cJSON_CreateString("device42");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", enabled);

    // step 2: Configure
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *name_item = cJSON_GetObjectItem(config, "name");
    char *name_val = cJSON_GetStringValue(name_item);
    cJSON *label = cJSON_CreateString(name_val);
    cJSON_AddItemToArray(items, label);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromObject(config, "name");
    cJSON_AddItemToArray(items, detached);
    cJSON *extra_true = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, extra_true);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}