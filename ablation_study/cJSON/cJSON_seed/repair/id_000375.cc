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
//<ID> 375
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *optional = cJSON_AddNullToObject(root, "optional");
    cJSON *enabled = cJSON_AddTrueToObject(root, "enabled");

    // step 2: Configure
    const char *name_val = cJSON_GetStringValue(name);
    cJSON *dup_name = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "dup_name", dup_name);
    cJSON_bool optional_is_null = cJSON_IsNull(optional);

    // step 3: Operate and Validate
    cJSON *count = cJSON_CreateNumber((double)optional_is_null + 1.0);
    cJSON_AddItemToObject(root, "count", count);
    char *retrieved = cJSON_GetStringValue(dup_name);
    cJSON *final_str = cJSON_CreateString(retrieved);
    cJSON_AddItemToObject(root, "final", final_str);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}