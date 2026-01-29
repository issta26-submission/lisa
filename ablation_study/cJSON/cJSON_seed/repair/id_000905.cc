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
//<ID> 905
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddNumberToObject(meta, "count", 1.0);
    cJSON *placeholder = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "placeholder", placeholder);

    // step 3: Operate and Validate
    cJSON_bool placeholder_null = cJSON_IsNull(placeholder);
    cJSON_AddNumberToObject(root, "placeholder_was_null", (double)placeholder_null);
    char *set_ret = cJSON_SetValuestring(name, "updated_name");
    cJSON *replacement = cJSON_CreateObject();
    cJSON_AddStringToObject(replacement, "replaced", "yes");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, placeholder, replacement);
    cJSON_AddNumberToObject(root, "replaced_flag", (double)replaced);
    cJSON_bool replacement_is_obj = cJSON_IsObject(replacement);
    cJSON_AddNumberToObject(root, "replacement_is_object", (double)replacement_is_obj);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)set_ret;
    return 66;
}