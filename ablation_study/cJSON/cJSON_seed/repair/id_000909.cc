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
//<ID> 909
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

    // step 2: Configure
    cJSON *name = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *maybe_missing = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "maybe_missing", maybe_missing);
    cJSON *sub = cJSON_CreateObject();
    cJSON_AddItemToObject(config, "sub", sub);
    cJSON *label = cJSON_CreateString("sub_old");
    cJSON_AddItemToObject(sub, "label", label);

    // step 3: Operate and Validate
    cJSON_bool was_null = cJSON_IsNull(maybe_missing);
    cJSON_bool sub_is_object = cJSON_IsObject(sub);
    cJSON_AddNumberToObject(root, "was_null_flag", (double)was_null);
    cJSON_AddNumberToObject(root, "sub_is_object_flag", (double)sub_is_object);
    char *set_ret = cJSON_SetValuestring(label, "sub_new");
    (void)set_ret;
    cJSON *replacement = cJSON_CreateString("label_replacement");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(sub, label, replacement);
    (void)replaced;
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}