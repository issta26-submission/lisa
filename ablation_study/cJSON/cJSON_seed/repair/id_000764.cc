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
//<ID> 764
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
    cJSON *status_old = cJSON_CreateString("initial");
    cJSON_AddItemToObject(config, "status", status_old);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(config, "enabled", flag_false);

    // step 2: Configure
    cJSON_AddNumberToObject(config, "threshold", 4.2);
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddItemToObject(config, "nested", nested);
    cJSON_AddNumberToObject(nested, "value", 7.0);
    cJSON *nested_ref = cJSON_CreateObjectReference(nested);
    cJSON_AddItemToObject(root, "nested_ref", nested_ref);
    cJSON *config_dup = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", config_dup);
    cJSON *new_status = cJSON_CreateFalse();
    cJSON_ReplaceItemInObject(config, "status", new_status);

    // step 3: Operate and Validate
    cJSON *cfg = cJSON_GetObjectItem(root, "config");
    cJSON *nest = cJSON_GetObjectItem(cfg, "nested");
    cJSON *val_item = cJSON_GetObjectItem(nest, "value");
    double val = cJSON_GetNumberValue(val_item);
    double doubled = val * 2.0;
    cJSON_AddNumberToObject(meta, "doubled_value", doubled);
    cJSON_AddNumberToObject(meta, "threshold", cJSON_GetNumberValue(cJSON_GetObjectItem(cfg, "threshold")));
    cJSON_AddNumberToObject(meta, "status_is_false", (double)cJSON_IsFalse(cJSON_GetObjectItem(cfg, "status")));
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}