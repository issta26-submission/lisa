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
//<ID> 666
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Duplicate & Add
    cJSON *config_clone = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_clone", config_clone);

    // step 3: Operate (detach and reattach)
    cJSON *detached_enabled = cJSON_DetachItemFromObjectCaseSensitive(config, "enabled");
    cJSON_AddItemToObject(root, "orphan_enabled", detached_enabled);
    cJSON *detached_clone = cJSON_DetachItemFromObject(root, "config_clone");
    cJSON_AddItemToObject(config, "readded_clone", detached_clone);

    // step 4: Validate & Cleanup
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}