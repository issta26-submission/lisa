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
//<ID> 844
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "note", "child ok");
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);
    cJSON_AddStringToObject(root, "id", "root1");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_item);

    // step 2: Configure
    const char *json_text = "{\"sensor\":42.5,\"status\":\"ok\"}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *sensor_item = cJSON_GetObjectItem(parsed, "sensor");
    double sensor_value = cJSON_GetNumberValue(sensor_item);
    cJSON_AddNumberToObject(root, "sensor_imported", sensor_value);

    // step 3: Operate and Validate
    cJSON *imported = cJSON_GetObjectItem(root, "sensor_imported");
    double imported_value = cJSON_GetNumberValue(imported);
    (void)imported_value;
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    cJSON_Delete(child);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}