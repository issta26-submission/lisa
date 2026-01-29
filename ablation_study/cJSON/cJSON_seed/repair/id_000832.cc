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
//<ID> 832
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "app", "fuzzer");
    cJSON_AddBoolToObject(root, "active", 1);
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 2: Configure
    const char *json_text = "{\"device\":\"sensorX\",\"status\":false,\"meta\":{\"id\":\"abc123\"}}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 3: Operate and Validate
    cJSON *device_item = cJSON_GetObjectItem(parsed, "device");
    const char *device_name = cJSON_GetStringValue(device_item);
    cJSON_AddStringToObject(root, "device_name", device_name);
    cJSON *detached_status = cJSON_DetachItemFromObject(parsed, "status");
    cJSON_AddItemToObject(root, "imported_status", detached_status);
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