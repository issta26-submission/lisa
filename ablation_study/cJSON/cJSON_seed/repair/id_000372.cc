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
//<ID> 372
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
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *optional = cJSON_CreateNull();
    cJSON_AddItemToObject(meta, "optional", optional);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddTrueToObject(root, "enabled");

    // step 2: Configure
    cJSON *desc = cJSON_CreateString("A short description");
    cJSON_AddItemToObject(root, "desc", desc);

    // step 3: Operate and Validate
    const char *name_val = cJSON_GetStringValue(name);
    const char *desc_val = cJSON_GetStringValue(desc);
    cJSON_bool optional_was_null = cJSON_IsNull(optional);
    const char *opt_status = optional_was_null ? "absent" : "present";
    cJSON *status = cJSON_CreateString(opt_status);
    cJSON_AddItemToObject(root, "optional_status", status);
    const char *status_val = cJSON_GetStringValue(status);
    cJSON *summary = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "summary", summary);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}