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
//<ID> 660
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
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(settings, "enabled", enabled);
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddItemToObject(settings, "nested", nested);
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(nested, "value", num);

    // step 2: Configure
    cJSON *dup_nested = cJSON_Duplicate(nested, 1);
    cJSON_AddItemToObject(root, "dup", dup_nested);

    // step 3: Operate
    cJSON *detached_enabled = cJSON_DetachItemFromObjectCaseSensitive(settings, "enabled");
    cJSON_AddItemToObject(root, "detached_enabled", detached_enabled);
    cJSON *detached_dup = cJSON_DetachItemFromObject(root, "dup");
    cJSON_AddItemToObject(root, "reattached_dup", detached_dup);

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