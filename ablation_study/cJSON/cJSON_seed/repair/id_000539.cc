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
//<ID> 539
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", true_item);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "disabled", false_item);
    cJSON *threshold = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(config, "threshold", threshold);

    // step 2: Configure
    cJSON *config_copy = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", config_copy);
    cJSON *threshold_item = cJSON_GetObjectItem(config, "threshold");
    double threshold_value = cJSON_GetNumberValue(threshold_item);

    // step 3: Operate and Validate
    cJSON *metrics = cJSON_AddObjectToObject(root, "metrics");
    cJSON *double_threshold = cJSON_CreateNumber(threshold_value * 2.0);
    cJSON_AddItemToObject(metrics, "double_threshold", double_threshold);
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    cJSON *dup_config_copy = cJSON_GetObjectItem(root_dup, "config_copy");
    cJSON *dup_threshold_item = cJSON_GetObjectItem(dup_config_copy, "threshold");
    double dup_threshold_value = cJSON_GetNumberValue(dup_threshold_item);
    cJSON *sum_item = cJSON_CreateNumber(threshold_value + dup_threshold_value);
    cJSON_AddItemToObject(root_dup, "sum_thresholds", sum_item);
    char *out = cJSON_PrintUnformatted(root_dup);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root_dup);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}