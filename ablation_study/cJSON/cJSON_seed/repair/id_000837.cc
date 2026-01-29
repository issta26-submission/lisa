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
//<ID> 837
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "title", "Example Document");
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "author", "tester");

    // step 2: Configure
    const char *json_text = "{\"user\":\"alice\",\"active\":true,\"note\":\"to be moved\"}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 3: Operate and Validate
    cJSON *user_item = cJSON_GetObjectItem(parsed, "user");
    cJSON *active_item = cJSON_GetObjectItem(parsed, "active");
    cJSON_AddStringToObject(root, "import_user", cJSON_GetStringValue(user_item));
    cJSON_AddBoolToObject(meta, "user_active", active_item ? 1 : 0);
    cJSON *detached_note = cJSON_DetachItemFromObject(parsed, "note");
    cJSON_AddItemToObject(root, "migrated_note", detached_note);
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