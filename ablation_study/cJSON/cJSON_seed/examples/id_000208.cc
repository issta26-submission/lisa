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
//<ID> 208
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
    cJSON *field = cJSON_CreateString("initial_value");
    cJSON_AddItemToObject(child, "field", field);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateStringReference("replaced_ref");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(child, field, replacement);
    cJSON_AddBoolToObject(root, "replaced", replaced);

    // step 3: Operate & Validate
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_null", maybe_null);
    cJSON_bool is_null = cJSON_IsNull(maybe_null);
    cJSON_AddBoolToObject(root, "is_null", is_null);

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}