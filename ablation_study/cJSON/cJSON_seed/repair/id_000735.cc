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
//<ID> 735
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"settings\": {\"threshold\": 2.5, \"mode\": \"auto\"}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *settings = cJSON_GetObjectItem(root, "settings");
    cJSON *threshold = cJSON_GetObjectItem(settings, "threshold");
    cJSON *threshold_copy = cJSON_Duplicate(threshold, 0);

    // step 2: Configure
    cJSON *new_threshold = cJSON_CreateNumber(7.75);
    cJSON_ReplaceItemViaPointer(settings, threshold, new_threshold);

    // step 3: Operate and Validate
    cJSON *settings_dup = cJSON_Duplicate(settings, 1);
    cJSON *dup_threshold = cJSON_GetObjectItem(settings_dup, "threshold");
    double sum_val = cJSON_GetNumberValue(threshold_copy) + cJSON_GetNumberValue(dup_threshold);
    cJSON *sum_node = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "threshold_sum", sum_node);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(settings_dup);
    cJSON_Delete(threshold_copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}