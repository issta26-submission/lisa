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
//<ID> 904
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
    cJSON *name = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *data = cJSON_CreateString("replace_me");
    cJSON_AddItemToObject(meta, "data", data);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_null", maybe_null);

    // step 3: Operate and Validate
    char *set_ret = cJSON_SetValuestring(name, "updated_name");
    cJSON_AddStringToObject(root, "set_name_result", set_ret ? set_ret : "");
    cJSON_bool was_null = cJSON_IsNull(maybe_null);
    cJSON_AddNumberToObject(root, "was_null", (double)was_null);
    cJSON *replacement = cJSON_CreateString("new_data");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(meta, data, replacement);
    cJSON_AddNumberToObject(root, "replaced_flag", (double)replaced);
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    cJSON_AddNumberToObject(root, "meta_is_object", (double)meta_is_obj);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}