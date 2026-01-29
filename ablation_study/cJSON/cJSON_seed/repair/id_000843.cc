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
//<ID> 843
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "test_device");
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", enabled);

    // step 2: Configure
    const char *json_text = "{\"sensor\":{\"reading\":23.5,\"unit\":\"C\"},\"id\":\"sensor-1\"}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 3: Operate and Validate
    cJSON *sensor = cJSON_GetObjectItem(parsed, "sensor");
    cJSON *reading = cJSON_GetObjectItem(sensor, "reading");
    double reading_value = cJSON_GetNumberValue(reading);
    cJSON_AddNumberToObject(root, "imported_reading", reading_value);
    cJSON *sensor_ref = cJSON_CreateObjectReference(sensor);
    cJSON_AddItemToObject(root, "sensor_ref", sensor_ref);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}