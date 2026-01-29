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
//<ID> 836
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "status", "ok");
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    const char *json_text = "{\"payload\":{\"id\":\"device-123\",\"valid\":false},\"meta\":\"v1\"}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 3: Operate and Validate
    cJSON *payload = cJSON_GetObjectItem(parsed, "payload");
    cJSON *detached_id = cJSON_DetachItemFromObject(payload, "id");
    cJSON_AddItemToObject(root, "imported_id", detached_id);
    cJSON *meta_item = cJSON_GetObjectItem(parsed, "meta");
    cJSON_AddStringToObject(root, "source_meta", cJSON_GetStringValue(meta_item));
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