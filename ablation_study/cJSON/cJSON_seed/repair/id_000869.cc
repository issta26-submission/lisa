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
//<ID> 869
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddStringToObject(root, "app", "cjson_test");
    cJSON_AddNumberToObject(root, "major", 1.0);

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "childA");
    cJSON_AddNumberToObject(child, "count", 5.0);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(child, "optional", null_item);
    cJSON_AddItemToObject(settings, "child", child);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(settings, "feature_enabled", true_item);

    // step 3: Operate and Validate
    cJSON *count_item = cJSON_GetObjectItem(child, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON_AddNumberToObject(root, "copied_count", count_val);
    cJSON_bool enabled = cJSON_IsTrue(true_item);
    cJSON_AddNumberToObject(root, "enabled_flag", (double)enabled);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}