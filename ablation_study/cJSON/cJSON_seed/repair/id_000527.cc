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
//<ID> 527
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *name = cJSON_CreateString("original_name");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", active);

    // step 2: Configure
    cJSON *replacement_false = cJSON_CreateFalse();
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, active, replacement_false);
    (void)replaced;
    const char *name_val = cJSON_GetStringValue(name);
    cJSON *copied_name = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(config, "copied_name", copied_name);

    // step 3: Operate
    cJSON *found_name = cJSON_GetObjectItemCaseSensitive(root, "name");
    const char *found_name_str = cJSON_GetStringValue(found_name);
    cJSON_AddStringToObject(root, "echo", found_name_str);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    cJSON *found_active = cJSON_GetObjectItemCaseSensitive(root, "active");
    (void)cJSON_GetStringValue(found_active);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}