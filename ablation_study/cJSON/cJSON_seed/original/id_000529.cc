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
//<ID> 529
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"device\":\"Sensor\",\"version\":2}";
    cJSON *root = cJSON_Parse(initial_json);
    cJSON *status_item = cJSON_CreateString("ok");
    cJSON_AddItemToObjectCS(root, "status", status_item);

    // step 2: Configure
    cJSON *device_item = cJSON_DetachItemFromObject(root, "device");
    char *previous_device_name = cJSON_GetStringValue(device_item);
    char *set_result = cJSON_SetValuestring(device_item, "RenamedSensor");
    cJSON *meta_obj = cJSON_CreateObject();
    cJSON *version_item = cJSON_GetObjectItem(root, "version");
    double version_val = cJSON_GetNumberValue(version_item);
    cJSON *adjusted = cJSON_CreateNumber(version_val + 1.0);
    cJSON_AddItemToObjectCS(meta_obj, "adjusted_version", adjusted);
    cJSON_AddItemToObjectCS(root, "meta", meta_obj);
    cJSON_AddItemToObjectCS(root, "device_name", device_item);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 32;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    char *status_after = cJSON_GetStringValue(cJSON_GetObjectItem(root, "status"));
    double adjusted_after = cJSON_GetNumberValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "meta"), "adjusted_version"));
    char *device_name_after = cJSON_GetStringValue(cJSON_GetObjectItem(root, "device_name"));
    (void)previous_device_name;
    (void)set_result;
    (void)status_after;
    (void)adjusted_after;
    (void)device_name_after;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}