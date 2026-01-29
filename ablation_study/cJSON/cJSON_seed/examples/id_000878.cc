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
//<ID> 878
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON *temp_value = cJSON_CreateNumber(23.5);
    cJSON *label = cJSON_CreateString("room-temp");
    cJSON *marker = cJSON_CreateString("invalid-marker");
    marker->type = 0;

    // step 2: Configure
    cJSON_AddItemToObject(metrics, "value", temp_value);
    cJSON_AddItemToObject(metrics, "label", label);
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON_AddItemToObject(root, "marker", marker);

    // step 3: Operate & Validate
    cJSON *metrics_ref = cJSON_GetObjectItem(root, "metrics");
    cJSON *value_ref = cJSON_GetObjectItem(metrics_ref, "value");
    cJSON_bool value_is_number = cJSON_IsNumber(value_ref);
    cJSON *value_is_number_item = cJSON_CreateBool(value_is_number);
    cJSON_AddItemToObject(root, "value_is_number", value_is_number_item);
    cJSON *marker_ref = cJSON_GetObjectItem(root, "marker");
    cJSON_bool marker_is_invalid = cJSON_IsInvalid(marker_ref);
    cJSON *marker_invalid_item = cJSON_CreateBool(marker_is_invalid);
    cJSON_AddItemToObject(root, "marker_invalid", marker_invalid_item);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}