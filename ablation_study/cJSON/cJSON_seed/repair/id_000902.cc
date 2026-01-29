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
//<ID> 902
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", cfg);
    cJSON *name = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(cfg, "name", name);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(cfg, "maybe_null", maybe_null);
    cJSON *childA = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "childA", childA);
    cJSON *childB = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "childB", childB);

    // step 2: Configure
    cJSON_SetValuestring(name, "updated_name");
    cJSON *replacement = cJSON_CreateString("replaced_childA");
    cJSON_bool replaced_ok = cJSON_ReplaceItemViaPointer(root, childA, replacement);

    // step 3: Operate and Validate
    cJSON_bool cfg_is_obj = cJSON_IsObject(cfg);
    cJSON_AddNumberToObject(root, "config_is_object", (double)cfg_is_obj);
    cJSON_bool was_null = cJSON_IsNull(maybe_null);
    cJSON_AddNumberToObject(root, "maybe_was_null", (double)was_null);
    cJSON_bool childB_is_obj = cJSON_IsObject(cJSON_GetObjectItem(root, "childB"));
    cJSON_AddNumberToObject(root, "childB_is_object", (double)childB_is_obj);
    cJSON_AddNumberToObject(root, "replace_succeeded", (double)replaced_ok);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}