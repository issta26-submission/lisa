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
//<ID> 1098
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
    cJSON *mode = cJSON_CreateString("on");
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddItemToObject(settings, "mode", mode);
    cJSON_AddItemToObject(settings, "enabled", enabled);

    // step 2: Configure
    const char *mode_val = cJSON_GetStringValue(mode);
    cJSON *mode_copy = cJSON_CreateString(mode_val);
    cJSON_AddItemToObject(root, "mode_copy", mode_copy);

    // step 3: Operate & Validate
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_ReplaceItemInObject(settings, "enabled", false_item);
    cJSON *new_mode = cJSON_CreateString("off");
    cJSON_ReplaceItemViaPointer(settings, mode, new_mode);
    cJSON *mode_after = cJSON_GetObjectItem(settings, "mode");
    const char *mode_after_val = cJSON_GetStringValue(mode_after);
    cJSON *snapshot = cJSON_CreateString(mode_after_val);
    cJSON_AddItemToObject(root, "mode_snapshot", snapshot);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}