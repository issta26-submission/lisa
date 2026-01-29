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
//<ID> 900
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
    cJSON_AddItemToObject(root, "child", child);
    cJSON *name = cJSON_CreateString("original_name");
    cJSON_AddItemToObject(child, "name", name);
    cJSON *count = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(child, "count", count);

    // step 2: Configure
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_null", maybe_null);
    double null_flag = (double)cJSON_IsNull(maybe_null);
    cJSON_AddNumberToObject(root, "null_flag", null_flag);

    // step 3: Operate and Validate
    char *updated = cJSON_SetValuestring(name, "updated_name");
    cJSON_AddStringToObject(root, "updated_name_snapshot", updated);
    cJSON *replacement = cJSON_CreateString("replacement_node");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, child, replacement);
    cJSON_AddNumberToObject(root, "replaced_flag", (double)replaced);
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_AddNumberToObject(root, "root_is_object", (double)root_is_obj);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}