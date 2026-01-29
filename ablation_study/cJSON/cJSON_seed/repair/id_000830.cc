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
//<ID> 830
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "status", "initialized");
    cJSON_AddBoolToObject(root, "ready", 0);

    // step 2: Configure
    const char *json_text = "{\"device\":\"sensor\",\"active\":true,\"meta\":{\"id\":\"X123\"}}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 3: Operate and Validate
    cJSON *meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *id_item = cJSON_GetObjectItem(meta, "id");
    char *id_str = cJSON_GetStringValue(id_item);
    cJSON_AddStringToObject(root, "device_id", id_str);
    cJSON *detached_device = cJSON_DetachItemFromObject(parsed, "device");
    cJSON_AddItemToObject(root, "device", detached_device);
    cJSON *active_item = cJSON_GetObjectItem(parsed, "active");
    cJSON_AddBoolToObject(root, "active_imported", cJSON_IsTrue(active_item));
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