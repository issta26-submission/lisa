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
//<ID> 524
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"device\":\"SensorX\",\"metrics\":{\"temp\":23.5,\"unit\":\"C\"}}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *location_item = cJSON_CreateString("WarehouseA");
    cJSON_AddItemToObjectCS(root, "location", location_item);
    cJSON *metrics_orig = cJSON_GetObjectItem(root, "metrics");

    // step 2: Configure
    cJSON *device_item = cJSON_GetObjectItem(root, "device");
    char *old_device_str = cJSON_SetValuestring(device_item, "SensorY");
    cJSON *detached_metrics = cJSON_DetachItemFromObject(root, "metrics");
    cJSON *metrics_backup = cJSON_Duplicate(detached_metrics, 1);
    cJSON_AddItemToObjectCS(root, "metrics_backup", metrics_backup);
    cJSON *unit_item_backup = cJSON_GetObjectItem(metrics_backup, "unit");
    char *old_unit_str = cJSON_SetValuestring(unit_item_backup, "Celsius");

    // step 3: Operate
    cJSON_AddItemToObjectCS(root, "active", cJSON_CreateTrue());
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 64;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    char *device_name_extracted = cJSON_GetStringValue(device_item);
    double temp_value = cJSON_GetNumberValue(cJSON_GetObjectItem(metrics_backup, "temp"));
    (void)old_device_str;
    (void)old_unit_str;
    (void)device_name_extracted;
    (void)temp_value;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(detached_metrics);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}