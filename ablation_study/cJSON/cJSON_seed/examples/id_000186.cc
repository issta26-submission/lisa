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
//<ID> 186
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);

    // step 2: Configure & Operate
    cJSON *ref_str = cJSON_CreateStringReference("static_string");
    cJSON_AddItemToArray(items, ref_str);
    cJSON *num = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(items, num);
    cJSON *replacement = cJSON_CreateStringReference("replaced_string");
    cJSON_ReplaceItemInArray(items, 0, replacement);
    cJSON *cfg_val = cJSON_CreateString("cfg_value");
    cJSON_AddItemToObject(settings, "mode", cfg_val);

    // step 3: Validate
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "settings");
    cJSON_AddItemToArray(items, detached);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}