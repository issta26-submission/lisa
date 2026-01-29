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
//<ID> 906
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
    cJSON *name_item = cJSON_CreateString("original_name");
    cJSON_AddItemToObject(child, "name", name_item);
    cJSON *optional_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", optional_item);

    // step 2: Configure
    char *current_name = cJSON_SetValuestring(name_item, "updated_name");
    double name_len = (double)strlen(current_name);
    cJSON_AddNumberToObject(root, "name_length", name_len);
    cJSON *replacement = cJSON_CreateObject();
    cJSON *rep_name = cJSON_CreateString("replacement_name");
    cJSON_AddItemToObject(replacement, "name", rep_name);

    // step 3: Operate and Validate
    cJSON_bool was_object = cJSON_IsObject(child);
    cJSON_bool was_null = cJSON_IsNull(optional_item);
    cJSON_bool did_replace = cJSON_ReplaceItemViaPointer(root, child, replacement);
    cJSON_AddNumberToObject(root, "child_was_object", (double)was_object);
    cJSON_AddNumberToObject(root, "optional_was_null", (double)was_null);
    cJSON_AddNumberToObject(root, "replace_result", (double)did_replace);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}