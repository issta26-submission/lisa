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
//<ID> 760
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(config, "settings", settings);

    // step 2: Configure
    cJSON *threshold = cJSON_AddNumberToObject(config, "threshold", 4.25);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(config, "enabled", flag_false);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "example");
    cJSON_AddNumberToObject(child, "id", 7.0);
    cJSON_AddItemToObject(meta, "child", child);
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(config, "child_ref", child_ref);

    // step 3: Operate and Validate
    cJSON *child_copy = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_copy", child_copy);
    cJSON *new_true = cJSON_CreateTrue();
    cJSON_bool replaced = cJSON_ReplaceItemInObject(config, "enabled", new_true);
    double thr_val = cJSON_GetNumberValue(threshold);
    double computed = thr_val * 2.0;
    cJSON_AddNumberToObject(meta, "computed", computed);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}