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
//<ID> 314
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
    cJSON *old_name = cJSON_CreateString("alpha");
    cJSON *old_active = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "name", old_name);
    cJSON_AddItemToObject(root, "active", old_active);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON *new_name = cJSON_CreateString("beta");
    cJSON_ReplaceItemViaPointer(root, name_item, new_name);
    cJSON *new_active = cJSON_CreateTrue();
    cJSON_ReplaceItemViaPointer(root, active_item, new_active);
    cJSON *new_meta = cJSON_CreateObject();
    cJSON_AddStringToObject(new_meta, "tag", "v2");
    cJSON_ReplaceItemViaPointer(root, meta_item, new_meta);

    // step 3: Operate and Validate
    cJSON *name_after = cJSON_GetObjectItem(root, "name");
    cJSON *active_after = cJSON_GetObjectItem(root, "active");
    cJSON *meta_after = cJSON_GetObjectItem(root, "meta");
    cJSON_bool active_is_bool = cJSON_IsBool(active_after);
    cJSON_bool meta_is_obj = cJSON_IsObject(meta_after);
    cJSON_AddNumberToObject(root, "active_is_bool", (double)active_is_bool);
    cJSON_AddNumberToObject(root, "meta_is_obj", (double)meta_is_obj);
    char *root_str = cJSON_Print(root);
    char *meta_str = cJSON_PrintUnformatted(meta_after);

    // step 4: Cleanup
    cJSON_free(root_str);
    cJSON_free(meta_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}