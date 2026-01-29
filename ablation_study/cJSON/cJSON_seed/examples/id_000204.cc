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
//<ID> 204
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateString("original");

    // step 2: Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON *nested = cJSON_CreateObject();
    cJSON *inner = cJSON_CreateString("inner");
    cJSON_AddItemToObject(nested, "inner_key", inner);
    cJSON_AddItemToObject(root, "nested", nested);

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateStringReference("replaced_ref");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, child, replacement);
    cJSON *null_value = cJSON_CreateNull();
    cJSON_bool isnull = cJSON_IsNull(null_value);
    cJSON_AddBoolToObject(root, "replaced_flag", replaced);
    cJSON_AddBoolToObject(root, "is_null", isnull);
    cJSON_AddItemToObject(root, "null_value", null_value);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}