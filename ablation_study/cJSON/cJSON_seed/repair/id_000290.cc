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
//<ID> 290
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"alice\",\"active\":true,\"meta\":{\"role\":\"user\"}}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *new_name = cJSON_CreateString("bob");
    cJSON *old_name_item = cJSON_GetObjectItem(root, "name");
    cJSON_ReplaceItemViaPointer(root, old_name_item, new_name);

    // step 3: Operate and Validate
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    cJSON_bool active_is_true = cJSON_IsTrue(active_item);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    cJSON *role_item = cJSON_GetObjectItem(meta, "role");
    char *role_str = cJSON_GetStringValue(role_item);
    char *name_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddStringToObject(summary, "user", name_str);
    cJSON_AddStringToObject(summary, "role", role_str);
    cJSON_AddNumberToObject(summary, "active_flag", (double)active_is_true);
    cJSON_AddItemToObject(root, "summary", summary);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}