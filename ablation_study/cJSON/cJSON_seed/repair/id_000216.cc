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
//<ID> 216
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
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(settings, "enabled", true_item);
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *auto_mode = cJSON_AddTrueToObject(root, "auto_mode");
    cJSON *label = cJSON_CreateString("fuzz-test");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    cJSON *note = cJSON_CreateString("primary");
    cJSON_AddItemToObject(settings, "note", note);

    // step 3: Operate and Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "settings");
    cJSON_bool is_obj = cJSON_IsObject(retrieved);
    cJSON_AddNumberToObject(root, "is_config_object", (double)is_obj);
    cJSON *detached = cJSON_DetachItemViaPointer(settings, true_item);
    cJSON_AddItemToObject(root, "enabled_copy", detached);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}