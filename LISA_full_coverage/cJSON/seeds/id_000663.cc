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
//<ID> 663
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
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON *other_flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "other_flag", other_flag);

    // step 2: Configure
    cJSON *config_dup = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", config_dup);

    // step 3: Operate
    cJSON *detached_enabled = cJSON_DetachItemFromObjectCaseSensitive(config, "enabled");
    cJSON_AddItemToObject(root, "detached_enabled", detached_enabled);
    cJSON *detached_other = cJSON_DetachItemFromObject(root, "other_flag");
    cJSON_AddItemToObject(config_dup, "reattached_flag", detached_other);
    cJSON *root_shallow = cJSON_Duplicate(root, 0);

    // step 4: Validate & Cleanup
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(root_shallow);
    // API sequence test completed successfully
    return 66;
}