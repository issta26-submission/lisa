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
//<ID> 961
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *label = cJSON_CreateString("initial");
    cJSON *modifiable = cJSON_CreateString("old");
    cJSON *updatable = cJSON_CreateString("to-be-set");

    // step 2: Configure
    cJSON_AddItemToObject(child, "label", label);
    cJSON_AddFalseToObject(child, "enabled");
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "modifiable", modifiable);
    cJSON_AddItemToObject(root, "updatable", updatable);

    // step 3: Operate & Validate
    char *set_ret1 = cJSON_SetValuestring(modifiable, "new-value");
    const char *val_modifiable = cJSON_GetStringValue(modifiable);
    cJSON_AddItemToObject(root, "copied_mod_value", cJSON_CreateString(val_modifiable));
    cJSON_AddItemToObject(root, "set_result_modifiable", cJSON_CreateString(set_ret1));

    char *set_ret2 = cJSON_SetValuestring(updatable, "updated-now");
    const char *val_updatable = cJSON_GetStringValue(updatable);
    cJSON_AddItemToObject(root, "copied_up_value", cJSON_CreateString(val_updatable));
    cJSON_AddItemToObject(root, "set_result_updatable", cJSON_CreateString(set_ret2));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}