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
//<ID> 393
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON *threshold = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON *config_lookup = cJSON_GetObjectItemCaseSensitive(root, "config");
    cJSON *threshold_item = cJSON_GetObjectItemCaseSensitive(config_lookup, "threshold");
    double threshold_value = cJSON_GetNumberValue(threshold_item);
    cJSON *double_item = cJSON_CreateNumber(threshold_value * 2.0);
    cJSON_AddItemToObject(root, "double_threshold", double_item);

    // step 3: Operate and Validate
    cJSON *threshold_ref = cJSON_CreateObjectReference(threshold_item);
    cJSON_AddItemToObject(root, "threshold_ref", threshold_ref);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    cJSON *double_lookup = cJSON_GetObjectItemCaseSensitive(root, "double_threshold");
    double double_value = cJSON_GetNumberValue(double_lookup);
    cJSON *validated = cJSON_CreateNumber(double_value + 0.5);
    cJSON_AddItemToObject(root, "validated_double", validated);

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(config_lookup, "threshold");
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}