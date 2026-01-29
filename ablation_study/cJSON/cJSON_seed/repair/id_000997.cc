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
//<ID> 997
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");

    // step 2: Configure
    cJSON_AddNullToObject(config, "optional");
    cJSON *enabled = cJSON_AddBoolToObject(config, "enabled", 1);
    cJSON_AddStringToObject(config, "name", "demo_project");

    // step 3: Operate and Validate
    cJSON *opt_item = cJSON_GetObjectItem(config, "optional");
    cJSON *enabled_cs = cJSON_GetObjectItemCaseSensitive(config, "enabled");
    cJSON *cfg_from_root = cJSON_GetObjectItem(root, "config");
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(cfg_from_root, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *copied_name = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(meta, "copied_name", copied_name);
    cJSON_AddItemReferenceToObject(meta, "enabled_ref", enabled_cs);
    cJSON_bool enabled_flag = cJSON_IsTrue(enabled_cs);
    cJSON_AddBoolToObject(meta, "copied_flag", enabled_flag);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}