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
//<ID> 294
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("Alice");
    cJSON *active_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "active", active_item);
    cJSON_AddItemToObject(root, "info", info);
    cJSON_AddNumberToObject(info, "age", 30.0);

    // step 2: Configure
    cJSON *orig_name_item = cJSON_GetObjectItem(root, "name");
    char *orig_name = cJSON_GetStringValue(orig_name_item);
    cJSON *orig_name_ref = cJSON_CreateStringReference(orig_name);
    cJSON_AddItemToObject(info, "orig_name", orig_name_ref);

    // step 3: Operate and Validate
    cJSON *active_check = cJSON_GetObjectItem(root, "active");
    cJSON_bool active_true = cJSON_IsTrue(active_check);
    cJSON *info_check = cJSON_GetObjectItem(root, "info");
    cJSON_bool info_is_obj = cJSON_IsObject(info_check);
    (void)active_true;
    (void)info_is_obj;
    cJSON *replacement = cJSON_CreateString("Bob");
    cJSON_ReplaceItemViaPointer(root, name_item, replacement);
    cJSON *new_name_item = cJSON_GetObjectItem(root, "name");
    char *new_name = cJSON_GetStringValue(new_name_item);
    (void)new_name;

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}