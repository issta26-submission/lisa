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
//<ID> 1099
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
    cJSON *s_mode = cJSON_CreateString("auto");
    cJSON *state_str = cJSON_CreateString("idle");

    // step 2: Configure
    cJSON_AddItemToObject(settings, "mode", s_mode);
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddItemToObject(root, "state", state_str);

    // step 3: Operate & Validate
    cJSON *mode_item = cJSON_GetObjectItem(settings, "mode");
    const char *mode_val = cJSON_GetStringValue(mode_item);
    cJSON *replacement_str = cJSON_CreateString(mode_val);
    cJSON_ReplaceItemViaPointer(settings, mode_item, replacement_str);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_ReplaceItemInObject(root, "state", false_item);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}