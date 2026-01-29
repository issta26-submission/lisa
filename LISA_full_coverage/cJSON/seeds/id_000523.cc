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
//<ID> 523
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"device\":\"Sensor\",\"values\":[10,20,30],\"status\":\"old\"}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *device = cJSON_GetObjectItem(root, "device");
    cJSON *detached_values = cJSON_DetachItemFromObject(root, "values");
    cJSON *detached_status = cJSON_DetachItemFromObject(root, "status");

    // step 2: Configure
    char *new_device_value = cJSON_SetValuestring(device, "WeatherSensor");
    cJSON_AddItemToObjectCS(root, "data", detached_values);
    cJSON *new_status = cJSON_CreateString("active");
    cJSON_AddItemToObjectCS(root, "status", new_status);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + (new_device_value ? strlen(new_device_value) : 0) + 16;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    cJSON *parsed_copy = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed_copy, 1);

    // step 4: Validate & Cleanup
    cJSON *device_copy = cJSON_GetObjectItem(parsed_copy, "device");
    const char *extracted_name = cJSON_GetStringValue(device_copy);
    (void)extracted_name;
    (void)equal;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed_copy);
    cJSON_Delete(detached_status);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}