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
//<ID> 835
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "app", "fuzz_app");
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    const char *json_text = "{\"user\":\"alice\",\"active\":false,\"meta\":{\"role\":\"admin\"}}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 3: Operate and Validate
    cJSON *user_item = cJSON_GetObjectItem(parsed, "user");
    cJSON_AddStringToObject(root, "user_name", cJSON_GetStringValue(user_item));
    cJSON *detached_user = cJSON_DetachItemFromObject(parsed, "user");
    cJSON_AddItemToObject(root, "imported_user", detached_user);
    cJSON *meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *role_item = cJSON_GetObjectItem(meta, "role");
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_parsed = cJSON_PrintUnformatted(parsed);

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_parsed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}