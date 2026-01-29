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
//<ID> 312
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"alice\",\"settings\":{\"enabled\":false},\"count\":1}";
    cJSON *root = cJSON_Parse(json);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *settings = cJSON_GetObjectItem(root, "settings");
    cJSON *enabled_item = cJSON_GetObjectItem(settings, "enabled");

    // step 2: Configure
    cJSON_bool settings_is_obj = cJSON_IsObject(settings);
    cJSON_bool enabled_is_bool_before = cJSON_IsBool(enabled_item);
    cJSON *new_enabled = cJSON_CreateTrue();
    cJSON_ReplaceItemViaPointer(settings, enabled_item, new_enabled);
    cJSON *new_name = cJSON_CreateString("bob");
    cJSON_ReplaceItemViaPointer(root, name_item, new_name);

    // step 3: Operate and Validate
    cJSON_bool settings_is_obj_after = cJSON_IsObject(settings);
    cJSON_bool new_enabled_is_bool = cJSON_IsBool(new_enabled);
    cJSON_AddNumberToObject(root, "enabled_flag", (double)new_enabled_is_bool);
    char *printed = cJSON_Print(root);
    char *unfmt_settings = cJSON_PrintUnformatted(settings);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(unfmt_settings);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}