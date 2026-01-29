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
//<ID> 668
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *state = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddItemToObject(root, "state", state);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(settings, "enabled", enabled);
    cJSON_AddNumberToObject(settings, "threshold", 2.718);
    cJSON_AddStringToObject(state, "status", "idle");

    // step 2: Configure
    cJSON *settings_copy = cJSON_Duplicate(settings, 1);
    cJSON_AddItemToObject(root, "settings_copy", settings_copy);

    // step 3: Operate
    cJSON *detached_settings = cJSON_DetachItemFromObject(root, "settings");
    cJSON_AddItemToObject(state, "migrated", detached_settings);
    cJSON *detached_state = cJSON_DetachItemFromObjectCaseSensitive(root, "state");
    cJSON_AddItemToObject(root, "restored_state", detached_state);

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