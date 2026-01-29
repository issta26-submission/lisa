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
//<ID> 838
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "app", "cJSON_test");
    cJSON_AddBoolToObject(root, "debug", 1);
    cJSON *metrics = cJSON_AddObjectToObject(root, "metrics");
    cJSON_AddNumberToObject(metrics, "count", 3.0);
    cJSON *values = cJSON_AddArrayToObject(metrics, "values");
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.0));

    // step 2: Configure
    const char *json_text = "{\"device\":{\"id\":\"dev123\",\"enabled\":false},\"info\":\"temp-sensor\"}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 3: Operate and Validate
    cJSON *device = cJSON_GetObjectItem(parsed, "device");
    cJSON *device_id = cJSON_GetObjectItem(device, "id");
    cJSON *device_enabled = cJSON_GetObjectItem(device, "enabled");
    cJSON *detached_info = cJSON_DetachItemFromObject(parsed, "info");
    cJSON_Delete(detached_info);
    cJSON *detached_device = cJSON_DetachItemFromObject(parsed, "device");
    cJSON_AddItemToObject(root, "imported_device", detached_device);
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