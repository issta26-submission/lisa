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
//<ID> 377
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
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *optional = cJSON_CreateNull();
    cJSON_AddItemToObject(meta, "optional", optional);

    // step 2: Configure
    cJSON *enabled = cJSON_AddTrueToObject(root, "enabled");
    (void)enabled;
    cJSON *version = cJSON_CreateString("1.2.3");
    cJSON_AddItemToObject(root, "version", version);

    // step 3: Operate and Validate
    const char *name_val = cJSON_GetStringValue(name);
    cJSON *name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(meta, "name_copy", name_copy);
    cJSON_bool opt_is_null = cJSON_IsNull(optional);
    cJSON *optional_flag = cJSON_CreateNumber((double)opt_is_null);
    cJSON_AddItemToObject(meta, "optional_is_null", optional_flag);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}