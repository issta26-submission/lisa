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
//<ID> 319
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"enabled\":true,\"config\":{\"threshold\":5},\"name\":\"deviceA\"}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *enabled_item = cJSON_GetObjectItem(root, "enabled");
    cJSON *config_item = cJSON_GetObjectItem(root, "config");
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *new_config = cJSON_CreateObject();
    cJSON_AddNumberToObject(new_config, "threshold", 42.0);
    cJSON_AddStringToObject(new_config, "mode", "auto");
    cJSON_ReplaceItemViaPointer(root, config_item, new_config);

    // step 3: Operate and Validate
    cJSON *config_after = cJSON_GetObjectItem(root, "config");
    cJSON_bool config_is_obj = cJSON_IsObject(config_after);
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled_item);
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON_AddNumberToObject(root, "enabled_flag", (double)enabled_is_bool);
    cJSON_AddBoolToObject(root, "config_isobj", config_is_obj);
    cJSON_AddStringToObject(root, "name_copy", name_str);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}