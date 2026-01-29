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
//<ID> 855
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
    cJSON *ref_name = cJSON_CreateStringReference("initial_name");
    cJSON_AddItemToObject(config, "name", ref_name);
    cJSON_AddTrueToObject(root, "feature_flag");

    // step 2: Configure
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "values", values);
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(values, n1);
    cJSON *new_config = cJSON_CreateObject();
    cJSON *new_name_ref = cJSON_CreateStringReference("replaced_name");
    cJSON_AddItemToObject(new_config, "name", new_name_ref);
    cJSON_AddTrueToObject(new_config, "enabled");
    cJSON_ReplaceItemInObject(root, "config", new_config);

    // step 3: Operate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    cJSON *cfg = cJSON_GetObjectItem(root, "config");
    const char *name_val = cJSON_GetStringValue(cJSON_GetObjectItem(cfg, "name"));
    (void)printed;
    (void)name_val;
    cJSON_DeleteItemFromObjectCaseSensitive(root, "feature_flag");

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}