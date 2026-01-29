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
//<ID> 669
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
    cJSON *true_flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", true_flag);
    cJSON *copy_config = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", copy_config);

    // step 2: Configure
    cJSON *remObj = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "remObj", remObj);
    cJSON *rem_key = cJSON_CreateTrue();
    cJSON_AddItemToObject(remObj, "key", rem_key);

    // step 3: Operate & Validate
    cJSON *detached_key = cJSON_DetachItemFromObjectCaseSensitive(remObj, "key");
    cJSON_AddItemToObject(root, "detached_key", detached_key);
    cJSON *detached_config = cJSON_DetachItemFromObject(root, "config");
    cJSON_AddItemToObject(copy_config, "migrated_config", detached_config);
    cJSON *enabled_in_copy = cJSON_GetObjectItem(copy_config, "enabled");
    cJSON_bool enabled_flag = cJSON_IsTrue(enabled_in_copy);
    cJSON_AddBoolToObject(root, "copy_enabled", enabled_flag);
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}