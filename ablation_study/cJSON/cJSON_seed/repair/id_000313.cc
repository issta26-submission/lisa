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
//<ID> 313
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"config\":{\"enabled\":true,\"threshold\":10},\"name\":\"device\"}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *config = cJSON_GetObjectItem(root, "config");
    cJSON *enabled_item = cJSON_GetObjectItem(config, "enabled");
    cJSON *new_enabled = cJSON_CreateBool(0);
    cJSON_ReplaceItemViaPointer(config, enabled_item, new_enabled);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *new_name = cJSON_CreateString("device-updated");
    cJSON_ReplaceItemViaPointer(root, name_item, new_name);

    // step 3: Operate and Validate
    cJSON_bool config_is_obj = cJSON_IsObject(config);
    cJSON_bool enabled_is_bool = cJSON_IsBool(new_enabled);
    cJSON_AddNumberToObject(root, "config_is_object", (double)config_is_obj);
    cJSON_AddNumberToObject(root, "enabled_is_bool", (double)enabled_is_bool);
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(config);
    cJSON_free(pretty);
    cJSON_free(compact);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}