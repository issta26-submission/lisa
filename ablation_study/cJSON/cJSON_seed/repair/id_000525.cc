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
//<ID> 525
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON *enabled_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", enabled_item);
    cJSON *metadata = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metadata", metadata);

    // step 2: Configure
    name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    char *orig_name = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(metadata, "original_name", orig_name);
    cJSON *replacement = cJSON_CreateString("replaced_name");
    cJSON_bool ok_replace = cJSON_ReplaceItemViaPointer(root, name_item, replacement);
    cJSON_AddBoolToObject(metadata, "replaced_flag", ok_replace);

    // step 3: Operate and Validate
    cJSON *current_name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    char *final_name = cJSON_GetStringValue(current_name_item);
    cJSON_AddStringToObject(metadata, "final_name", final_name);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}