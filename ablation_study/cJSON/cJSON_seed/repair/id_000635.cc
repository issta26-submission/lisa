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
//<ID> 635
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
    cJSON_AddItemToObject(root, "config", config);
    cJSON *threshold = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON_AddItemToObject(config, "enabled", enabled);

    // step 2: Configure
    double previous_value = cJSON_SetNumberHelper(threshold, 2.718);
    const char *version_str = cJSON_Version();
    cJSON *version_item = cJSON_CreateString(version_str);
    cJSON_AddItemToObject(root, "version", version_item);

    // step 3: Operate and Validate
    cJSON *replacement_flag = cJSON_CreateBool(0);
    cJSON_bool replace_result = cJSON_ReplaceItemViaPointer(config, enabled, replacement_flag);
    double current_threshold = cJSON_GetNumberValue(threshold);
    cJSON_AddNumberToObject(root, "threshold_check", current_threshold);
    double ratio = 0.0;
    ratio = current_threshold / previous_value;
    cJSON_AddNumberToObject(root, "threshold_ratio", ratio);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON_AddStringToObject(root, "serialized", buffer);
    cJSON_AddNumberToObject(root, "print_succeeded", (double)printed);
    cJSON_AddNumberToObject(root, "replace_succeeded", (double)replace_result);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}