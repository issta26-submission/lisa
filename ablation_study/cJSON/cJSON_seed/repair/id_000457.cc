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
//<ID> 457
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
    cJSON *active_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "active", active_item);

    // step 2: Configure
    cJSON *replacement_name = cJSON_CreateString("updated_name");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, name_item, replacement_name);
    (void)replaced;
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "info", "configuration complete");

    // step 3: Operate
    cJSON *name_from_root = cJSON_GetObjectItem(root, "name");
    char *name_str = cJSON_GetStringValue(name_from_root);
    char greeting[128];
    snprintf(greeting, sizeof(greeting), "Hello, %s", name_str);
    cJSON_AddStringToObject(root, "greeting", greeting);
    cJSON *active_from_root = cJSON_GetObjectItem(root, "active");
    cJSON_bool active_is_false = cJSON_IsFalse(active_from_root);
    (void)active_is_false;

    // step 4: Validate and Cleanup
    char *printed = cJSON_Print(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}