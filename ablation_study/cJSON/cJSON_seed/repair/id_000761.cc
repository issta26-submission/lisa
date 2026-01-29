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
//<ID> 761
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON *threshold = cJSON_CreateNumber(2.5);
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON *name = cJSON_CreateString("device-alpha");
    cJSON_AddItemToObject(meta, "name", name);

    // step 3: Operate and Validate
    cJSON *config_dup = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", config_dup);
    cJSON *config_ref = cJSON_CreateObjectReference(config_dup);
    cJSON_AddItemToObject(meta, "config_ref", config_ref);
    cJSON *new_threshold = cJSON_CreateNumber(5.0);
    cJSON_ReplaceItemInObject(config, "threshold", new_threshold);
    cJSON *effective_item = cJSON_GetObjectItem(config, "threshold");
    double effective_value = cJSON_GetNumberValue(effective_item);
    cJSON_AddNumberToObject(meta, "effective_threshold", effective_value);
    cJSON_bool was_false = cJSON_IsFalse(cJSON_GetObjectItem(config, "enabled"));
    cJSON_AddNumberToObject(meta, "enabled_was_false", (double)was_false);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}