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
//<ID> 831
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "id", "root1");
    cJSON_AddBoolToObject(root, "active", 1);

    // step 2: Configure
    const char *json_text = "{\"device\":\"sensor\",\"enabled\":false,\"meta\":{\"owner\":\"alice\"}}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 3: Operate and Validate
    cJSON *meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *owner = cJSON_GetObjectItem(meta, "owner");
    char *owner_name = cJSON_GetStringValue(owner);
    (void)owner_name;
    cJSON *detached_meta = cJSON_DetachItemFromObject(parsed, "meta");
    cJSON_AddItemToObject(root, "imported_meta", detached_meta);
    cJSON *imported_meta = cJSON_GetObjectItem(root, "imported_meta");
    cJSON *imported_owner = cJSON_GetObjectItem(imported_meta, "owner");
    char *imported_owner_name = cJSON_GetStringValue(imported_owner);
    (void)imported_owner_name;
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