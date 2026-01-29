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
//<ID> 768
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *params = cJSON_CreateObject();
    cJSON_AddItemToObject(config, "params", params);
    cJSON_AddNumberToObject(config, "threshold", 4.25);
    cJSON *enabled_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(config, "enabled", enabled_false);
    cJSON_AddStringToObject(params, "mode", "auto");

    // step 3: Operate and Validate
    cJSON *config_dup = cJSON_Duplicate(config, 1);
    cJSON *config_ref = cJSON_CreateObjectReference(config_dup);
    cJSON_ReplaceItemInObject(root, "config", config_ref);
    cJSON *snapshot_copy = cJSON_Duplicate(config_dup, 1);
    cJSON_AddItemToObject(meta, "snapshot_of_config", snapshot_copy);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}