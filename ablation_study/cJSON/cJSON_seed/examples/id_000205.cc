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
//<ID> 205
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "obj", obj);
    cJSON *name_ref = cJSON_CreateStringReference("original_name");
    cJSON_AddItemToObject(obj, "name", name_ref);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateStringReference("replaced_root");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, obj, replacement);

    // step 3: Operate & Validate
    cJSON *n = cJSON_CreateNull();
    cJSON_bool is_null = cJSON_IsNull(n);
    cJSON_AddBoolToObject(root, "replaced_flag", replaced);
    cJSON_AddBoolToObject(root, "is_null_flag", is_null);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(n);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}