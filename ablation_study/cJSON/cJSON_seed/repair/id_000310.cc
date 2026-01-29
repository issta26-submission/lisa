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
//<ID> 310
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"status\":true,\"config\":{\"max\":10},\"name\":\"old\"}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *status_item = cJSON_GetObjectItem(root, "status");
    cJSON_bool status_was_bool = cJSON_IsBool(status_item);
    cJSON *new_status = cJSON_CreateFalse();
    cJSON_ReplaceItemViaPointer(root, status_item, new_status);

    cJSON *config_item = cJSON_GetObjectItem(root, "config");
    cJSON_bool config_was_obj = cJSON_IsObject(config_item);
    cJSON *new_config = cJSON_CreateObject();
    cJSON_AddNumberToObject(new_config, "max", 20.0);
    cJSON_ReplaceItemViaPointer(root, config_item, new_config);

    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *new_name = cJSON_CreateString("new");
    cJSON_ReplaceItemViaPointer(root, name_item, new_name);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    cJSON_Minify(printed);
    cJSON *dup = cJSON_Parse(printed);
    cJSON *dup_status = cJSON_GetObjectItem(dup, "status");
    cJSON_bool dup_status_is_bool = cJSON_IsBool(dup_status);
    cJSON *dup_config = cJSON_GetObjectItem(dup, "config");
    cJSON_bool dup_config_is_obj = cJSON_IsObject(dup_config);
    (void)status_was_bool;
    (void)config_was_obj;
    (void)dup_status_is_bool;
    (void)dup_config_is_obj;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}