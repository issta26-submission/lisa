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
//<ID> 523
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
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON *count = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(config, "count", count);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("updated_name");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, name, replacement);
    (void)replaced;

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    char *name_str = cJSON_GetStringValue(name_item);
    (void)name_str;
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}