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
//<ID> 741
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *status_null = cJSON_CreateNull();
    cJSON *mode_item = cJSON_CreateString("automatic");
    cJSON *threshold_item = cJSON_CreateNumber(2.71828);
    (void)version;

    // step 2: Configure
    cJSON_AddItemToObject(settings, "mode", mode_item);
    cJSON_AddItemToObject(settings, "threshold", threshold_item);
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddItemToObject(root, "status", status_null);
    cJSON_AddItemToObject(root, "device", cJSON_CreateString("sensor-A"));

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *detached_status = cJSON_DetachItemFromObjectCaseSensitive(parsed, "status");
    cJSON *parsed_settings = cJSON_GetObjectItem(parsed, "settings");
    cJSON *parsed_mode = cJSON_GetObjectItem(parsed_settings, "mode");
    char *mode_value = cJSON_GetStringValue(parsed_mode);
    (void)mode_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(detached_status);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}