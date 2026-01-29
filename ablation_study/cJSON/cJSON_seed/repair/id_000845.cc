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
//<ID> 845
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = cJSON_Parse("{\"sensor\":\"temp\",\"reading\":23.5,\"config\":{\"mode\":\"auto\"}}");

    // step 2: Configure
    cJSON *sensor_item = cJSON_GetObjectItem(parsed, "sensor");
    char *sensor_name = cJSON_GetStringValue(sensor_item);
    cJSON_AddStringToObject(root, "sensor_id", sensor_name);
    cJSON *reading_item = cJSON_GetObjectItem(parsed, "reading");
    double reading_val = cJSON_GetNumberValue(reading_item);
    cJSON_AddNumberToObject(root, "reading_copy", reading_val);

    // step 3: Operate and Validate
    cJSON *config_item = cJSON_GetObjectItem(parsed, "config");
    cJSON *config_ref = cJSON_CreateObjectReference(config_item);
    cJSON_AddItemToObject(root, "config_ref", config_ref);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", true_item);
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